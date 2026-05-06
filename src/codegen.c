/*
 * codegen.c - Three-Address Code Generation
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "codegen.h"
#include "symtable.h"

static TACList *G = NULL;      /* current list being built */
static int temp_count  = 0;
static int label_count = 0;

/* ---- Utility ---- */
char *tac_new_temp(void) {
    static char buf[MAX_OPERAND];
    snprintf(buf, sizeof(buf), "t%d", temp_count++);
    return buf;
}
char *tac_new_label(void) {
    static char buf[MAX_OPERAND];
    snprintf(buf, sizeof(buf), "L%d", label_count++);
    return buf;
}

static TACInstr *emit(TACOp op,
                       const char *result,
                       const char *arg1,
                       const char *arg2) {
    if (G->count >= MAX_TAC) {
        fprintf(stderr, "[Codegen] TAC overflow\n");
        exit(1);
    }
    TACInstr *i = &G->instrs[G->count++];
    i->op = op;
    strncpy(i->result, result ? result : "", MAX_OPERAND - 1);
    strncpy(i->arg1,   arg1   ? arg1   : "", MAX_OPERAND - 1);
    strncpy(i->arg2,   arg2   ? arg2   : "", MAX_OPERAND - 1);
    i->n_args = 0;
    return i;
}

/* ---- Forward declarations ---- */
static char *gen_expr(ASTNode *node, char *dest);
static void  gen_stmt(ASTNode *node);

/* ================================================================
   Expression code generation — returns name of result operand
   ================================================================ */
static char *gen_expr(ASTNode *node, char *dest) {
    /* dest: if non-null, place result there (optimization hint) */
    if (!node) return "";

    static char tmp_buf[MAX_OPERAND];

    switch (node->type) {

    case NODE_INT_LIT:
        snprintf(tmp_buf, sizeof(tmp_buf), "%d", node->lit.ival);
        return tmp_buf;

    case NODE_FLOAT_LIT:
        snprintf(tmp_buf, sizeof(tmp_buf), "%g", node->lit.fval);
        return tmp_buf;

    case NODE_CHAR_LIT:
        snprintf(tmp_buf, sizeof(tmp_buf), "%d", node->lit.ival);
        return tmp_buf;

    case NODE_STRING_LIT:
        snprintf(tmp_buf, sizeof(tmp_buf), "%s", node->lit.sval);
        return tmp_buf;

    case NODE_IDENT:
        snprintf(tmp_buf, sizeof(tmp_buf), "%s", node->name);
        return tmp_buf;

    case NODE_ARRAY_ACCESS: {
        char *idx = gen_expr(node->left, NULL);
        char *t   = dest ? dest : tac_new_temp();
        snprintf(tmp_buf, sizeof(tmp_buf), "%s", t);
        TACInstr *ins = emit(TAC_ARRAY_LOAD, t, node->name, idx);
        (void)ins;
        return tmp_buf;
    }

    case NODE_BINOP: {
        char *la = gen_expr(node->left,  NULL);
        char a_buf[MAX_OPERAND]; strncpy(a_buf, la, MAX_OPERAND-1);
        char *rb = gen_expr(node->right, NULL);
        char r_buf[MAX_OPERAND]; strncpy(r_buf, rb, MAX_OPERAND-1);
        char *t  = dest ? dest : tac_new_temp();
        snprintf(tmp_buf, sizeof(tmp_buf), "%s", t);
        TACInstr *ins = emit(TAC_BINOP, t, a_buf, "");
        snprintf(ins->arg2, MAX_OPERAND, "%s %s", node->name, r_buf);
        return tmp_buf;
    }

    case NODE_UNOP: {
        char *la = gen_expr(node->left, NULL);
        char  a_save[MAX_OPERAND]; strncpy(a_save, la, MAX_OPERAND-1);
        char *t  = dest ? dest : tac_new_temp();
        snprintf(tmp_buf, sizeof(tmp_buf), "%s", t);

        if (!strcmp(node->name, "++pre")) {
            emit(TAC_BINOP, a_save, a_save, "+ 1");
            emit(TAC_ASSIGN, t, a_save, "");
        } else if (!strcmp(node->name, "--pre")) {
            emit(TAC_BINOP, a_save, a_save, "- 1");
            emit(TAC_ASSIGN, t, a_save, "");
        } else if (!strcmp(node->name, "++post")) {
            emit(TAC_ASSIGN, t, a_save, "");
            emit(TAC_BINOP, a_save, a_save, "+ 1");
        } else if (!strcmp(node->name, "--post")) {
            emit(TAC_ASSIGN, t, a_save, "");
            emit(TAC_BINOP, a_save, a_save, "- 1");
        } else {
            /* - ! & * */
            snprintf(a_save, sizeof(a_save), "%s %s", node->name, la);
            emit(TAC_UNOP, t, a_save, "");
        }
        return tmp_buf;
    }

    case NODE_ASSIGN: {
        char *rhs = gen_expr(node->right, NULL);
        char  r_save[MAX_OPERAND]; strncpy(r_save, rhs, MAX_OPERAND-1);

        /* Handle compound assignments by expanding */
        const char *op = node->name;
        char expanded[MAX_OPERAND * 3];
        if (strcmp(op, "=") != 0) {
            /* += -= *= /= → lhs = lhs OP rhs */
            char lhs_name[MAX_OPERAND];
            if (node->left->type == NODE_IDENT)
                strncpy(lhs_name, node->left->name, MAX_OPERAND-1);
            else
                strncpy(lhs_name, "?", MAX_OPERAND-1);
            char bin_op[4] = { op[0], '\0', '\0', '\0' };
            snprintf(expanded, sizeof(expanded), "%s %s", bin_op, r_save);
            char *t2 = tac_new_temp();
            emit(TAC_BINOP, t2, lhs_name, expanded);
            strncpy(r_save, t2, MAX_OPERAND-1);
        }

        if (node->left->type == NODE_IDENT) {
            emit(TAC_ASSIGN, node->left->name, r_save, "");
            snprintf(tmp_buf, sizeof(tmp_buf), "%s", node->left->name);
        } else if (node->left->type == NODE_ARRAY_ACCESS) {
            char *idx = gen_expr(node->left->left, NULL);
            char  idx_save[MAX_OPERAND]; strncpy(idx_save, idx, MAX_OPERAND-1);
            emit(TAC_ARRAY_STORE, node->left->name, idx_save, r_save);
            snprintf(tmp_buf, sizeof(tmp_buf), "%s", r_save);
        } else {
            snprintf(tmp_buf, sizeof(tmp_buf), "%s", r_save);
        }
        return tmp_buf;
    }

    case NODE_CALL: {
        /* Push args as PARAM instructions */
        for (int i = 0; i < node->child_count; i++) {
            char *a = gen_expr(node->children[i], NULL);
            char  a_s[MAX_OPERAND]; strncpy(a_s, a, MAX_OPERAND-1);
            emit(TAC_PARAM, "", a_s, "");
        }
        char *t = dest ? dest : tac_new_temp();
        TACInstr *ins = emit(TAC_CALL, t, node->name, "");
        ins->n_args = node->child_count;
        snprintf(tmp_buf, sizeof(tmp_buf), "%s", t);
        return tmp_buf;
    }

    default:
        return "";
    }
}

/* ================================================================
   Statement code generation
   ================================================================ */
static void gen_stmt(ASTNode *node) {
    if (!node) return;

    switch (node->type) {

    case NODE_BLOCK:
        for (int i = 0; i < node->child_count; i++)
            gen_stmt(node->children[i]);
        break;

    case NODE_EXPR_STMT:
        gen_expr(node->left, NULL);
        break;

    case NODE_VAR_DECL:
        if (node->init) {
            char *rhs = gen_expr(node->init, NULL);
            char  r[MAX_OPERAND]; strncpy(r, rhs, MAX_OPERAND-1);
            emit(TAC_ASSIGN, node->name, r, "");
        }
        break;

    case NODE_ARRAY_DECL:
        /* Emit array initializers if present */
        for (int i = 0; i < node->child_count; i++) {
            char *v = gen_expr(node->children[i], NULL);
            char  vs[MAX_OPERAND]; strncpy(vs, v, MAX_OPERAND-1);
            char  idx[16]; snprintf(idx, sizeof(idx), "%d", i);
            emit(TAC_ARRAY_STORE, node->name, idx, vs);
        }
        break;

    case NODE_IF: {
        char *cond = gen_expr(node->cond, NULL);
        char  c[MAX_OPERAND]; strncpy(c, cond, MAX_OPERAND-1);
        char *l_else = tac_new_label();
        char  le[MAX_OPERAND]; strncpy(le, l_else, MAX_OPERAND-1);
        char *l_end  = tac_new_label();
        char  lend[MAX_OPERAND]; strncpy(lend, l_end, MAX_OPERAND-1);

        emit(TAC_IF_FALSE, "", c, le);
        gen_stmt(node->body);
        if (node->else_body) emit(TAC_GOTO, "", lend, "");
        emit(TAC_LABEL, le, "", "");
        if (node->else_body) {
            gen_stmt(node->else_body);
            emit(TAC_LABEL, lend, "", "");
        }
        break;
    }

    case NODE_WHILE: {
        char *l_cond = tac_new_label();
        char  lc[MAX_OPERAND]; strncpy(lc, l_cond, MAX_OPERAND-1);
        char *l_end  = tac_new_label();
        char  le[MAX_OPERAND]; strncpy(le, l_end, MAX_OPERAND-1);

        emit(TAC_LABEL, lc, "", "");
        char *cond = gen_expr(node->cond, NULL);
        char  c[MAX_OPERAND]; strncpy(c, cond, MAX_OPERAND-1);
        emit(TAC_IF_FALSE, "", c, le);
        gen_stmt(node->body);
        emit(TAC_GOTO, "", lc, "");
        emit(TAC_LABEL, le, "", "");
        break;
    }

    case NODE_FOR: {
        char *l_cond = tac_new_label();
        char  lc[MAX_OPERAND]; strncpy(lc, l_cond, MAX_OPERAND-1);
        char *l_end  = tac_new_label();
        char  le[MAX_OPERAND]; strncpy(le, l_end, MAX_OPERAND-1);

        if (node->init) gen_stmt(node->init);
        emit(TAC_LABEL, lc, "", "");
        if (node->cond) {
            char *cond = gen_expr(node->cond, NULL);
            char  c[MAX_OPERAND]; strncpy(c, cond, MAX_OPERAND-1);
            emit(TAC_IF_FALSE, "", c, le);
        }
        gen_stmt(node->body);
        if (node->update) gen_expr(node->update, NULL);
        emit(TAC_GOTO, "", lc, "");
        emit(TAC_LABEL, le, "", "");
        break;
    }

    case NODE_RETURN: {
        if (node->left) {
            char *v = gen_expr(node->left, NULL);
            char  vs[MAX_OPERAND]; strncpy(vs, v, MAX_OPERAND-1);
            emit(TAC_RETURN, "", vs, "");
        } else {
            emit(TAC_RETURN, "", "", "");
        }
        break;
    }

    case NODE_PRINTF: {
        /* emit PARAM for each arg, then special PRINTF instr */
        for (int i = 0; i < node->child_count; i++) {
            char *a = gen_expr(node->children[i], NULL);
            char  as[MAX_OPERAND]; strncpy(as, a, MAX_OPERAND-1);
            emit(TAC_PARAM, "", as, "");
        }
        emit(TAC_PRINTF, "", node->lit.sval, "");
        TACInstr *ins = &G->instrs[G->count - 1];
        ins->n_args = node->child_count;
        break;
    }

    case NODE_SCANF: {
        for (int i = 0; i < node->child_count; i++) {
            char *a = gen_expr(node->children[i], NULL);
            char  as[MAX_OPERAND]; strncpy(as, a, MAX_OPERAND-1);
            emit(TAC_PARAM, "", as, "");
        }
        emit(TAC_SCANF, "", node->lit.sval, "");
        TACInstr *ins = &G->instrs[G->count - 1];
        ins->n_args = node->child_count;
        break;
    }

    case NODE_FUNC_DECL: {
        emit(TAC_FUNC_BEGIN, node->name, type_to_str(node->data_type), "");
        /* params */
        for (int i = 0; i < node->child_count; i++) {
            ASTNode *p = node->children[i];
            if (p->type == NODE_PARAM)
                emit(TAC_PARAM, p->name, type_to_str(p->data_type), "");
        }
        if (node->body) gen_stmt(node->body);
        emit(TAC_FUNC_END, node->name, "", "");
        break;
    }

    default:
        /* VAR/ARRAY decl at top-level — already handled */
        break;
    }
}

/* ================================================================
   Public API
   ================================================================ */
void tac_init(TACList *tl) {
    tl->count  = 0;
    temp_count = 0;
    label_count = 0;
}

void tac_generate(TACList *tl, ASTNode *root) {
    G = tl;
    if (!root) return;
    for (int i = 0; i < root->child_count; i++)
        gen_stmt(root->children[i]);
}

static const char *op_name(TACOp op) {
    switch (op) {
    case TAC_ASSIGN:      return "=";
    case TAC_BINOP:       return "binop";
    case TAC_UNOP:        return "unop";
    case TAC_LABEL:       return "label";
    case TAC_GOTO:        return "goto";
    case TAC_IF_TRUE:     return "if_true";
    case TAC_IF_FALSE:    return "if_false";
    case TAC_PARAM:       return "param";
    case TAC_CALL:        return "call";
    case TAC_RETURN:      return "return";
    case TAC_ARRAY_STORE: return "store";
    case TAC_ARRAY_LOAD:  return "load";
    case TAC_PRINTF:      return "printf";
    case TAC_SCANF:       return "scanf";
    case TAC_FUNC_BEGIN:  return "func_begin";
    case TAC_FUNC_END:    return "func_end";
    default:              return "?";
    }
}

void tac_print(TACList *tl, FILE *out) {
    fprintf(out, "\n===== Three-Address Code (TAC) =====\n");
    for (int i = 0; i < tl->count; i++) {
        TACInstr *ins = &tl->instrs[i];
        switch (ins->op) {
        case TAC_FUNC_BEGIN:
            fprintf(out, "\nfunc %s %s:\n", ins->arg1, ins->result);
            break;
        case TAC_FUNC_END:
            fprintf(out, "end func %s\n", ins->result);
            break;
        case TAC_LABEL:
            fprintf(out, "%s:\n", ins->result);
            break;
        case TAC_ASSIGN:
            fprintf(out, "    %s = %s\n", ins->result, ins->arg1);
            break;
        case TAC_BINOP:
            fprintf(out, "    %s = %s %s\n", ins->result, ins->arg1, ins->arg2);
            break;
        case TAC_UNOP:
            fprintf(out, "    %s = %s\n", ins->result, ins->arg1);
            break;
        case TAC_GOTO:
            fprintf(out, "    goto %s\n", ins->arg1);
            break;
        case TAC_IF_TRUE:
            fprintf(out, "    if %s goto %s\n", ins->arg1, ins->arg2);
            break;
        case TAC_IF_FALSE:
            fprintf(out, "    if_false %s goto %s\n", ins->arg1, ins->arg2);
            break;
        case TAC_PARAM:
            if (ins->result[0])
                fprintf(out, "    param %s : %s\n", ins->result, ins->arg1);
            else
                fprintf(out, "    param %s\n", ins->arg1);
            break;
        case TAC_CALL:
            fprintf(out, "    %s = call %s, %d\n",
                    ins->result, ins->arg1, ins->n_args);
            break;
        case TAC_RETURN:
            if (ins->arg1[0])
                fprintf(out, "    return %s\n", ins->arg1);
            else
                fprintf(out, "    return\n");
            break;
        case TAC_ARRAY_STORE:
            fprintf(out, "    %s[%s] = %s\n",
                    ins->result, ins->arg1, ins->arg2);
            break;
        case TAC_ARRAY_LOAD:
            fprintf(out, "    %s = %s[%s]\n",
                    ins->result, ins->arg1, ins->arg2);
            break;
        case TAC_PRINTF:
            fprintf(out, "    printf %s, %d args\n", ins->arg1, ins->n_args);
            break;
        case TAC_SCANF:
            fprintf(out, "    scanf %s, %d args\n", ins->arg1, ins->n_args);
            break;
        default:
            fprintf(out, "    [%s %s %s %s]\n",
                    op_name(ins->op), ins->result, ins->arg1, ins->arg2);
            break;
        }
    }
    fprintf(out, "====================================\n\n");
}

void tac_print_file(TACList *tl, const char *filename) {
    FILE *f = fopen(filename, "w");
    if (!f) { perror(filename); return; }
    tac_print(tl, f);
    fclose(f);
}
