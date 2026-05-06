/*
 * semantic.c - Semantic Analysis
 *
 * Walks the AST and:
 *   1. Populates the symbol table
 *   2. Resolves identifiers → symbol entries
 *   3. Infers / checks types
 *   4. Reports errors without crashing (continues to find more errors)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "semantic.h"
#include "symtable.h"

/* Currently-enclosing function (for return type check) */
static Symbol *current_func  = NULL;
static int     loop_depth    = 0;

/* ---------------------------------------------------------------- */
static void sem_error(int line, const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    fprintf(stderr, "[Semantic Error] Line %d: ", line);
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    va_end(ap);
    semantic_errors++;
}
static void sem_warn(int line, const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    fprintf(stderr, "[Semantic Warning] Line %d: ", line);
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    va_end(ap);
}

/* Numeric type promotion: int/char → float if either operand is float */
static DataType arith_result(DataType a, DataType b) {
    if (a == TYPE_FLOAT || b == TYPE_FLOAT) return TYPE_FLOAT;
    return TYPE_INT;
}

/* ----------------------------------------------------------------
   Forward declaration
   ---------------------------------------------------------------- */
static DataType analyze_expr(ASTNode *node);
static void     analyze_stmt(ASTNode *node);
static void     analyze_decl(ASTNode *node);

/* ================================================================
   Expression analysis — returns inferred DataType
   ================================================================ */
static DataType analyze_expr(ASTNode *node) {
    if (!node) return TYPE_VOID;

    switch (node->type) {
    case NODE_INT_LIT:
        node->data_type = TYPE_INT;
        return TYPE_INT;

    case NODE_FLOAT_LIT:
        node->data_type = TYPE_FLOAT;
        return TYPE_FLOAT;

    case NODE_CHAR_LIT:
        node->data_type = TYPE_CHAR;
        return TYPE_CHAR;

    case NODE_STRING_LIT:
        node->data_type = TYPE_CHAR;
        return TYPE_CHAR;

    case NODE_IDENT: {
        Symbol *s = sym_lookup(node->name);
        if (!s) {
            sem_error(node->line, "Undeclared identifier '%s'", node->name);
            node->data_type = TYPE_INT; /* recover */
        } else {
            node->data_type = s->type;
            if (!s->is_initialized && s->kind == SYM_VAR)
                sem_warn(node->line, "'%s' may be used before initialization",
                         node->name);
        }
        return node->data_type;
    }

    case NODE_ARRAY_ACCESS: {
        Symbol *s = sym_lookup(node->name);
        if (!s) {
            sem_error(node->line, "Undeclared array '%s'", node->name);
            node->data_type = TYPE_INT;
        } else if (s->kind != SYM_ARRAY && s->kind != SYM_PARAM) {
            sem_error(node->line, "'%s' is not an array", node->name);
            node->data_type = TYPE_INT;
        } else {
            node->data_type = s->type;
        }
        DataType idx_t = analyze_expr(node->left);
        if (idx_t != TYPE_INT && idx_t != TYPE_CHAR)
            sem_error(node->line, "Array index must be integer, got '%s'",
                      type_to_str(idx_t));
        return node->data_type;
    }

    case NODE_BINOP: {
        DataType lt = analyze_expr(node->left);
        DataType rt = analyze_expr(node->right);
        /* Relational / logical → int (boolean) */
        const char *op = node->name;
        if (!strcmp(op,"==") || !strcmp(op,"!=") ||
            !strcmp(op,"<")  || !strcmp(op,"<=") ||
            !strcmp(op,">")  || !strcmp(op,">=") ||
            !strcmp(op,"&&") || !strcmp(op,"||")) {
            node->data_type = TYPE_INT;
        } else {
            /* arithmetic */
            if (lt == TYPE_VOID || rt == TYPE_VOID)
                sem_error(node->line, "Void operand in arithmetic expression");
            node->data_type = arith_result(lt, rt);
        }
        return node->data_type;
    }

    case NODE_UNOP: {
        DataType t = analyze_expr(node->left);
        const char *op = node->name;
        if (!strcmp(op, "!")) {
            node->data_type = TYPE_INT;
        } else if (!strcmp(op, "&")) {
            node->data_type = t; /* simplified: treat as same type */
        } else {
            node->data_type = t;
        }
        return node->data_type;
    }

    case NODE_ASSIGN: {
        DataType rhs = analyze_expr(node->right);
        DataType lhs = TYPE_UNKNOWN;

        /* Determine LHS type */
        if (node->left->type == NODE_IDENT) {
            Symbol *s = sym_lookup(node->left->name);
            if (!s) {
                sem_error(node->left->line,
                          "Assignment to undeclared variable '%s'",
                          node->left->name);
                lhs = rhs;
            } else {
                if (s->kind == SYM_FUNC) {
                    sem_error(node->left->line,
                              "Cannot assign to function '%s'", s->name);
                }
                lhs = s->type;
                s->is_initialized = 1;
            }
        } else if (node->left->type == NODE_ARRAY_ACCESS) {
            lhs = analyze_expr(node->left);
        }

        /* Warn on lossy narrowing */
        if (lhs == TYPE_INT && rhs == TYPE_FLOAT)
            sem_warn(node->line,
                     "Implicit conversion from float to int (possible loss)");
        node->data_type = lhs;
        return lhs;
    }

    case NODE_CALL: {
        Symbol *s = sym_lookup(node->name);
        if (!s) {
            sem_error(node->line, "Call to undeclared function '%s'", node->name);
            node->data_type = TYPE_INT;
            return TYPE_INT;
        }
        if (s->kind != SYM_FUNC) {
            sem_error(node->line, "'%s' is not a function", node->name);
            node->data_type = TYPE_INT;
            return TYPE_INT;
        }
        /* Check argument count */
        int given = node->child_count;
        if (given != s->param_count) {
            sem_error(node->line,
                      "Function '%s' expects %d argument(s), got %d",
                      node->name, s->param_count, given);
        }
        /* Type-check each argument */
        for (int i = 0; i < node->child_count && i < s->param_count; i++) {
            DataType at = analyze_expr(node->children[i]);
            DataType pt = s->param_types[i];
            if (at != pt && !(at == TYPE_INT && pt == TYPE_FLOAT) &&
                !(at == TYPE_CHAR && pt == TYPE_INT)) {
                sem_warn(node->line,
                         "Argument %d of '%s': passing '%s' where '%s' expected",
                         i + 1, node->name, type_to_str(at), type_to_str(pt));
            }
        }
        node->data_type = s->type;
        return s->type;
    }

    default:
        return TYPE_UNKNOWN;
    }
}

/* ================================================================
   Statement analysis
   ================================================================ */
static void analyze_stmt(ASTNode *node) {
    if (!node) return;

    switch (node->type) {

    case NODE_BLOCK:
        sym_enter_scope();
        for (int i = 0; i < node->child_count; i++)
            analyze_stmt(node->children[i]);
        sym_exit_scope();
        break;

    case NODE_EXPR_STMT:
        analyze_expr(node->left);
        break;

    case NODE_VAR_DECL:
        analyze_decl(node);
        break;

    case NODE_ARRAY_DECL:
        analyze_decl(node);
        break;

    case NODE_IF:
        analyze_expr(node->cond);
        analyze_stmt(node->body);
        if (node->else_body) analyze_stmt(node->else_body);
        break;

    case NODE_WHILE:
        analyze_expr(node->cond);
        loop_depth++;
        analyze_stmt(node->body);
        loop_depth--;
        break;

    case NODE_FOR:
        sym_enter_scope(); /* for-init may declare a variable */
        if (node->init) analyze_stmt(node->init);
        if (node->cond) analyze_expr(node->cond);
        if (node->update) analyze_expr(node->update);
        loop_depth++;
        analyze_stmt(node->body);
        loop_depth--;
        sym_exit_scope();
        break;

    case NODE_RETURN: {
        DataType ret_t = TYPE_VOID;
        if (node->left) ret_t = analyze_expr(node->left);
        if (current_func) {
            DataType expected = current_func->type;
            if (expected == TYPE_VOID && node->left) {
                sem_warn(node->line,
                         "Return with value in void function '%s'",
                         current_func->name);
            } else if (expected != TYPE_VOID && !node->left) {
                sem_error(node->line,
                          "Return without value in non-void function '%s'",
                          current_func->name);
            } else if (expected != ret_t && node->left) {
                if (!(expected == TYPE_FLOAT && ret_t == TYPE_INT))
                    sem_warn(node->line,
                             "Return type mismatch: '%s' vs expected '%s'",
                             type_to_str(ret_t), type_to_str(expected));
            }
        }
        break;
    }

    case NODE_BREAK:
        if (loop_depth == 0)
            sem_error(node->line, "'break' outside loop");
        break;

    case NODE_CONTINUE:
        if (loop_depth == 0)
            sem_error(node->line, "'continue' outside loop");
        break;

    case NODE_PRINTF:
        for (int i = 0; i < node->child_count; i++)
            analyze_expr(node->children[i]);
        break;

    case NODE_SCANF:
        for (int i = 0; i < node->child_count; i++)
            analyze_expr(node->children[i]);
        break;

    default:
        break;
    }
}

/* ================================================================
   Declaration analysis (both global and local)
   ================================================================ */
static void analyze_decl(ASTNode *node) {
    if (!node) return;

    if (node->type == NODE_VAR_DECL) {
        Symbol *s = sym_declare(node->name, node->data_type, SYM_VAR, node->line);
        if (node->init) {
            DataType init_t = analyze_expr(node->init);
            if (s) {
                s->is_initialized = 1;
                if (s->type == TYPE_INT && init_t == TYPE_FLOAT)
                    sem_warn(node->line,
                             "Initializing int '%s' with float (truncation)", s->name);
            }
        }
        return;
    }

    if (node->type == NODE_ARRAY_DECL) {
        if (node->lit.ival <= 0) {
            sem_error(node->line, "Array '%s' size must be > 0", node->name);
            return;
        }
        sym_declare_array(node->name, node->data_type,
                          node->lit.ival, node->line);
        /* Validate initializer count */
        if (node->child_count > 0 && node->child_count > node->lit.ival) {
            sem_error(node->line,
                      "Too many initializers for array '%s' (size %d, got %d)",
                      node->name, node->lit.ival, node->child_count);
        }
        for (int i = 0; i < node->child_count; i++)
            analyze_expr(node->children[i]);
        return;
    }

    if (node->type == NODE_FUNC_DECL) {
        /* Collect param info BEFORE entering new scope */
        DataType ptypes[16];
        int pcount = 0;
        for (int i = 0; i < node->child_count; i++) {
            ASTNode *p = node->children[i];
            if (p->type == NODE_PARAM && pcount < 16)
                ptypes[pcount++] = p->data_type;
        }
        Symbol *fsym = sym_declare_func(node->name, node->data_type,
                                        pcount, ptypes, node->line);

        /* Enter function scope */
        sym_enter_scope();
        Symbol *prev_func = current_func;
        current_func = fsym;

        /* Declare params in function scope */
        for (int i = 0; i < node->child_count; i++) {
            ASTNode *p = node->children[i];
            if (p->type == NODE_PARAM) {
                Symbol *ps = sym_declare(p->name, p->data_type,
                                         SYM_PARAM, p->line);
                if (ps) {
                    ps->is_initialized = 1;
                    if (p->lit.ival == -1) /* array param */
                        ps->kind = SYM_ARRAY;
                }
            }
        }

        /* Analyze body (already a block) */
        if (node->body) {
            /* body is NODE_BLOCK; analyze its children directly in same scope */
            for (int i = 0; i < node->body->child_count; i++)
                analyze_stmt(node->body->children[i]);
        }

        current_func = prev_func;
        sym_exit_scope();
        return;
    }
}

/* ================================================================
   Entry point
   ================================================================ */
void semantic_analyze(ASTNode *root) {
    if (!root) return;
    sym_init();

    /* Process all top-level declarations */
    for (int i = 0; i < root->child_count; i++) {
        ASTNode *decl = root->children[i];
        if (decl->type == NODE_FUNC_DECL)
            analyze_decl(decl);
        else
            analyze_decl(decl); /* var/array at global scope */
    }

    if (semantic_errors == 0)
        printf("[Semantic] Analysis complete — no errors.\n");
    else
        printf("[Semantic] Analysis complete — %d error(s) found.\n",
               semantic_errors);
}
