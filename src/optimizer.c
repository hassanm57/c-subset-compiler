/*
 * optimizer.c - TAC Optimization Passes
 *
 * Passes applied in order:
 *   1. Algebraic simplification  (x * 1 → x, x + 0 → x, etc.)
 *   2. Constant folding           (2 + 3 → 5)
 *   3. Constant propagation       (t0 = 5; t1 = t0 + 1 → t1 = 5 + 1)
 *   4. Copy propagation           (t0 = x; y = t0 → y = x)
 *   5. Dead code elimination      (assignments whose result is never read)
 *
 * Each pass is run to fixed point (i.e., repeated until no change).
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "optimizer.h"

/* ---- Helpers ---- */

static int is_int_const(const char *s, long *out) {
    if (!s || !*s) return 0;
    char *end;
    *out = strtol(s, &end, 10);
    return (*end == '\0');
}

static int is_float_const(const char *s, double *out) {
    if (!s || !*s) return 0;
    char *end;
    *out = strtod(s, &end);
    return (*end == '\0');
}

/* Parse "OP value" stored in arg2 field */
static int parse_op_val(const char *arg2, char *op_out, char *val_out) {
    if (!arg2 || !*arg2) return 0;
    /* format: "OP value" where OP is 1-2 chars */
    int i = 0;
    while (arg2[i] && !isspace((unsigned char)arg2[i])) i++;
    if (i == 0 || !isspace((unsigned char)arg2[i])) return 0;
    strncpy(op_out, arg2, i); op_out[i] = '\0';
    /* skip spaces */
    while (isspace((unsigned char)arg2[i])) i++;
    strncpy(val_out, arg2 + i, MAX_OPERAND - 1);
    return 1;
}

/* Mark instruction as NOP */
static void nop(TACInstr *ins) {
    ins->op = TAC_ASSIGN;
    strcpy(ins->result, "");
    strcpy(ins->arg1, "");
    strcpy(ins->arg2, "");
}

/* ================================================================
   Pass 1: Algebraic Simplification
   x + 0, x - 0, x * 1, x / 1, x * 0, 0 * x, x ^ 0 → x or 0
   ================================================================ */
static int pass_algebraic(TACList *tl) {
    int changed = 0;
    for (int i = 0; i < tl->count; i++) {
        TACInstr *ins = &tl->instrs[i];
        if (ins->op != TAC_BINOP) continue;
        if (!ins->result[0]) continue;

        char op[8], rhs[MAX_OPERAND];
        if (!parse_op_val(ins->arg2, op, rhs)) continue;

        long rv = 0; double rf = 0.0;
        int rhs_is_int   = is_int_const(rhs, &rv);
        int rhs_is_float = !rhs_is_int && is_float_const(rhs, &rf);
        double rval = rhs_is_int ? (double)rv : rf;
        int rhs_is_num = rhs_is_int || rhs_is_float;

        /* x + 0 → x,  x - 0 → x */
        if (rhs_is_num && rval == 0.0 &&
            (!strcmp(op, "+") || !strcmp(op, "-"))) {
            ins->op = TAC_ASSIGN;
            /* result = arg1 */
            strcpy(ins->arg2, "");
            changed = 1;
            continue;
        }
        /* x * 1 → x,  x / 1 → x */
        if (rhs_is_num && rval == 1.0 &&
            (!strcmp(op, "*") || !strcmp(op, "/"))) {
            ins->op = TAC_ASSIGN;
            strcpy(ins->arg2, "");
            changed = 1;
            continue;
        }
        /* x * 0 → 0,  0 * x → 0 */
        if (rhs_is_num && rval == 0.0 && !strcmp(op, "*")) {
            ins->op = TAC_ASSIGN;
            strcpy(ins->arg1, "0");
            strcpy(ins->arg2, "");
            changed = 1;
            continue;
        }
    }
    return changed;
}

/* ================================================================
   Pass 2: Constant Folding
   Evaluate binop/unop where both operands are compile-time constants
   ================================================================ */
static int pass_const_fold(TACList *tl) {
    int changed = 0;
    for (int i = 0; i < tl->count; i++) {
        TACInstr *ins = &tl->instrs[i];
        if (ins->op != TAC_BINOP || !ins->result[0]) continue;

        char op[8], rhs[MAX_OPERAND];
        if (!parse_op_val(ins->arg2, op, rhs)) continue;

        long lv, rv; double lf, rf;
        int lhs_int = is_int_const(ins->arg1, &lv);
        int rhs_int = is_int_const(rhs, &rv);
        int lhs_flt = !lhs_int && is_float_const(ins->arg1, &lf);
        int rhs_flt = !rhs_int && is_float_const(rhs, &rf);

        if (!((lhs_int || lhs_flt) && (rhs_int || rhs_flt))) continue;

        double L = lhs_int ? (double)lv : lf;
        double R = rhs_int ? (double)rv : rf;
        int    use_int = lhs_int && rhs_int;
        double result;

        if      (!strcmp(op, "+"))  result = L + R;
        else if (!strcmp(op, "-"))  result = L - R;
        else if (!strcmp(op, "*"))  result = L * R;
        else if (!strcmp(op, "/"))  {
            if (R == 0.0) continue; /* avoid div-by-zero fold */
            result = L / R;
        }
        else if (!strcmp(op, "%"))  {
            if (!use_int || rv == 0) continue;
            result = (double)(lv % rv); use_int = 1;
        }
        else if (!strcmp(op, "==")) { result = (L == R) ? 1 : 0; use_int = 1; }
        else if (!strcmp(op, "!=")) { result = (L != R) ? 1 : 0; use_int = 1; }
        else if (!strcmp(op, "<"))  { result = (L < R)  ? 1 : 0; use_int = 1; }
        else if (!strcmp(op, "<=")) { result = (L <= R) ? 1 : 0; use_int = 1; }
        else if (!strcmp(op, ">"))  { result = (L > R)  ? 1 : 0; use_int = 1; }
        else if (!strcmp(op, ">=")) { result = (L >= R) ? 1 : 0; use_int = 1; }
        else if (!strcmp(op, "&&")) { result = (L != 0 && R != 0) ? 1 : 0; use_int = 1; }
        else if (!strcmp(op, "||")) { result = (L != 0 || R != 0) ? 1 : 0; use_int = 1; }
        else continue;

        /* Replace with ASSIGN of folded constant */
        ins->op = TAC_ASSIGN;
        if (use_int)
            snprintf(ins->arg1, MAX_OPERAND, "%ld", (long)result);
        else
            snprintf(ins->arg1, MAX_OPERAND, "%g", result);
        strcpy(ins->arg2, "");
        changed = 1;
    }
    return changed;
}

/* ================================================================
   Pass 3: Constant Propagation
   If t = <constant>, replace later uses of t with <constant>
   (within the same basic block / function — simple version)
   ================================================================ */
#define MAX_CONSTS 256
typedef struct { char var[MAX_OPERAND]; char val[MAX_OPERAND]; } ConstEntry;

static void replace_in_field(char *field, ConstEntry *table, int n) {
    for (int k = 0; k < n; k++) {
        if (!strcmp(field, table[k].var)) {
            strncpy(field, table[k].val, MAX_OPERAND - 1);
            return;
        }
    }
}

static int pass_const_prop(TACList *tl) {
    int changed = 0;
    ConstEntry table[MAX_CONSTS];
    int n = 0;

    for (int i = 0; i < tl->count; i++) {
        TACInstr *ins = &tl->instrs[i];

        /* Invalidate if result is reassigned */
        if (ins->result[0]) {
            for (int k = 0; k < n; k++) {
                if (!strcmp(table[k].var, ins->result)) {
                    /* Remove entry */
                    table[k] = table[--n];
                    break;
                }
            }
        }

        /* Propagate into operands */
        if (ins->op == TAC_ASSIGN || ins->op == TAC_BINOP ||
            ins->op == TAC_UNOP   || ins->op == TAC_IF_TRUE ||
            ins->op == TAC_IF_FALSE || ins->op == TAC_PARAM ||
            ins->op == TAC_RETURN) {

            char old1[MAX_OPERAND], old2[MAX_OPERAND];
            strncpy(old1, ins->arg1, MAX_OPERAND-1);
            strncpy(old2, ins->arg2, MAX_OPERAND-1);

            replace_in_field(ins->arg1, table, n);

            /* For binop, arg2 is "OP value" — only replace the value part */
            if (ins->op == TAC_BINOP) {
                char op[8], val[MAX_OPERAND];
                if (parse_op_val(ins->arg2, op, val)) {
                    replace_in_field(val, table, n);
                    snprintf(ins->arg2, MAX_OPERAND, "%s %s", op, val);
                }
            } else {
                replace_in_field(ins->arg2, table, n);
            }

            if (strcmp(old1, ins->arg1) || strcmp(old2, ins->arg2))
                changed = 1;
        }

        /* Record new constant */
        if (ins->op == TAC_ASSIGN && ins->result[0] && ins->arg1[0]) {
            long lv; double df;
            if (is_int_const(ins->arg1, &lv) ||
                is_float_const(ins->arg1, &df)) {
                if (n < MAX_CONSTS) {
                    strncpy(table[n].var, ins->result, MAX_OPERAND-1);
                    strncpy(table[n].val, ins->arg1,   MAX_OPERAND-1);
                    n++;
                }
            }
        }

        /* Reset table at function boundaries / labels (conservative) */
        if (ins->op == TAC_LABEL || ins->op == TAC_FUNC_BEGIN) {
            n = 0;
        }
    }
    return changed;
}

/* ================================================================
   Pass 4: Copy Propagation
   If t = x (var-to-var copy), replace uses of t with x
   ================================================================ */
static int pass_copy_prop(TACList *tl) {
    int changed = 0;
    ConstEntry table[MAX_CONSTS]; /* reuse struct for var→var map */
    int n = 0;

    for (int i = 0; i < tl->count; i++) {
        TACInstr *ins = &tl->instrs[i];

        /* Invalidate copies containing result */
        if (ins->result[0]) {
            for (int k = 0; k < n; ) {
                if (!strcmp(table[k].var, ins->result) ||
                    !strcmp(table[k].val, ins->result)) {
                    table[k] = table[--n];
                } else k++;
            }
        }

        /* Propagate */
        if (ins->op == TAC_ASSIGN || ins->op == TAC_BINOP ||
            ins->op == TAC_UNOP   || ins->op == TAC_IF_TRUE ||
            ins->op == TAC_IF_FALSE || ins->op == TAC_PARAM ||
            ins->op == TAC_RETURN || ins->op == TAC_ARRAY_STORE ||
            ins->op == TAC_ARRAY_LOAD) {

            char old1[MAX_OPERAND]; strncpy(old1, ins->arg1, MAX_OPERAND-1);
            replace_in_field(ins->arg1, table, n);
            if (strcmp(old1, ins->arg1)) changed = 1;

            if (ins->op == TAC_BINOP) {
                char op[8], val[MAX_OPERAND];
                char old2[MAX_OPERAND]; strncpy(old2, ins->arg2, MAX_OPERAND-1);
                if (parse_op_val(ins->arg2, op, val)) {
                    replace_in_field(val, table, n);
                    snprintf(ins->arg2, MAX_OPERAND, "%s %s", op, val);
                    if (strcmp(old2, ins->arg2)) changed = 1;
                }
            }
        }

        /* Record var-to-var copy */
        if (ins->op == TAC_ASSIGN && ins->result[0] && ins->arg1[0] &&
            !ins->arg2[0]) {
            long lv; double df;
            /* only if arg1 is a variable name (not a constant) */
            if (!is_int_const(ins->arg1, &lv) &&
                !is_float_const(ins->arg1, &df)) {
                if (n < MAX_CONSTS) {
                    strncpy(table[n].var, ins->result, MAX_OPERAND-1);
                    strncpy(table[n].val, ins->arg1,   MAX_OPERAND-1);
                    n++;
                }
            }
        }

        if (ins->op == TAC_LABEL || ins->op == TAC_FUNC_BEGIN) n = 0;
    }
    return changed;
}

/* ================================================================
   Pass 5: Dead Code Elimination
   Remove assignments to temporaries/variables whose value is never read
   (only for temporaries t0, t1, ... — safe conservative choice)
   ================================================================ */
static int is_temp(const char *s) {
    return s && s[0] == 't' && isdigit((unsigned char)s[1]);
}

static int temp_is_used(TACList *tl, int def_idx, const char *var) {
    /* Scan forward; if var is reassigned before use → not used */
    for (int i = def_idx + 1; i < tl->count; i++) {
        TACInstr *ins = &tl->instrs[i];
        /* Check uses in arg1 */
        if (!strcmp(ins->arg1, var)) return 1;
        /* Check in arg2 (handle "OP val" format) */
        if (ins->op == TAC_BINOP) {
            char op[8], val[MAX_OPERAND];
            if (parse_op_val(ins->arg2, op, val) && !strcmp(val, var)) return 1;
        } else if (!strcmp(ins->arg2, var)) return 1;
        /* If redefined → not used downstream from here */
        if (!strcmp(ins->result, var)) return 0;
        /* Stop at function boundaries */
        if (ins->op == TAC_FUNC_END || ins->op == TAC_FUNC_BEGIN) return 0;
    }
    return 0;
}

static int pass_dce(TACList *tl) {
    int changed = 0;
    for (int i = 0; i < tl->count; i++) {
        TACInstr *ins = &tl->instrs[i];
        if ((ins->op == TAC_ASSIGN || ins->op == TAC_BINOP ||
             ins->op == TAC_UNOP  || ins->op == TAC_CALL) &&
            is_temp(ins->result) && !temp_is_used(tl, i, ins->result)) {
            /* For CALL, don't eliminate — side effects */
            if (ins->op == TAC_CALL) continue;
            nop(ins);
            changed = 1;
        }
    }
    return changed;
}

/* ================================================================
   Remove NOP instructions (ASSIGN with empty result/arg)
   ================================================================ */
static void compact(TACList *tl) {
    int w = 0;
    for (int r = 0; r < tl->count; r++) {
        TACInstr *ins = &tl->instrs[r];
        if (ins->op == TAC_ASSIGN && !ins->result[0] && !ins->arg1[0])
            continue; /* skip NOP */
        if (r != w) tl->instrs[w] = tl->instrs[r];
        w++;
    }
    tl->count = w;
}

/* ================================================================
   Main optimizer entry point
   ================================================================ */
void optimize(TACList *tl) {
    printf("[Optimizer] Running optimization passes...\n");
    int orig_count = tl->count;

    int iter = 0, changed = 1;
    while (changed && iter < 20) {
        changed  = 0;
        changed |= pass_algebraic(tl);
        changed |= pass_const_fold(tl);
        changed |= pass_const_prop(tl);
        changed |= pass_copy_prop(tl);
        changed |= pass_dce(tl);
        iter++;
    }
    compact(tl);

    printf("[Optimizer] Done in %d pass(es). Instructions: %d → %d\n",
           iter, orig_count, tl->count);
}
