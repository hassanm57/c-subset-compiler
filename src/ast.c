/*
 * ast.c - AST Node Constructors and Utilities
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

static ASTNode *ast_alloc(NodeType type, int line) {
    ASTNode *n = calloc(1, sizeof(ASTNode));
    if (!n) { perror("ast_alloc"); exit(1); }
    n->type      = type;
    n->data_type = TYPE_UNKNOWN;
    n->line      = line;
    return n;
}

ASTNode *ast_make_int(int val, int line) {
    ASTNode *n = ast_alloc(NODE_INT_LIT, line);
    n->lit.ival  = val;
    n->data_type = TYPE_INT;
    return n;
}
ASTNode *ast_make_float(double val, int line) {
    ASTNode *n = ast_alloc(NODE_FLOAT_LIT, line);
    n->lit.fval  = val;
    n->data_type = TYPE_FLOAT;
    return n;
}
ASTNode *ast_make_char(int val, int line) {
    ASTNode *n = ast_alloc(NODE_CHAR_LIT, line);
    n->lit.ival  = val;
    n->data_type = TYPE_CHAR;
    return n;
}
ASTNode *ast_make_string(const char *s, int line) {
    ASTNode *n = ast_alloc(NODE_STRING_LIT, line);
    n->lit.sval  = strdup(s);
    n->data_type = TYPE_CHAR; /* pointer-like */
    return n;
}
ASTNode *ast_make_ident(const char *name, int line) {
    ASTNode *n = ast_alloc(NODE_IDENT, line);
    n->name = strdup(name);
    return n;
}
ASTNode *ast_make_array_access(const char *name, ASTNode *idx, int line) {
    ASTNode *n = ast_alloc(NODE_ARRAY_ACCESS, line);
    n->name  = strdup(name);
    n->left  = idx;
    return n;
}
ASTNode *ast_make_binop(const char *op, ASTNode *l, ASTNode *r, int line) {
    ASTNode *n = ast_alloc(NODE_BINOP, line);
    n->name  = strdup(op);
    n->left  = l;
    n->right = r;
    return n;
}
ASTNode *ast_make_unop(const char *op, ASTNode *operand, int line) {
    ASTNode *n = ast_alloc(NODE_UNOP, line);
    n->name = strdup(op);
    n->left = operand;
    return n;
}
ASTNode *ast_make_assign(const char *op, ASTNode *lhs, ASTNode *rhs, int line) {
    ASTNode *n = ast_alloc(NODE_ASSIGN, line);
    n->name  = strdup(op);
    n->left  = lhs;
    n->right = rhs;
    return n;
}
ASTNode *ast_make_call(const char *name, int line) {
    ASTNode *n = ast_alloc(NODE_CALL, line);
    n->name = strdup(name);
    return n;
}
ASTNode *ast_make_var_decl(DataType t, const char *name, ASTNode *init, int line) {
    ASTNode *n = ast_alloc(NODE_VAR_DECL, line);
    n->data_type = t;
    n->name      = strdup(name);
    n->init      = init;
    return n;
}
ASTNode *ast_make_array_decl(DataType t, const char *name, int size, int line) {
    ASTNode *n = ast_alloc(NODE_ARRAY_DECL, line);
    n->data_type = t;
    n->name      = strdup(name);
    n->lit.ival  = size;
    return n;
}
ASTNode *ast_make_func_decl(DataType ret, const char *name, int line) {
    ASTNode *n = ast_alloc(NODE_FUNC_DECL, line);
    n->data_type = ret;
    n->name      = strdup(name);
    return n;
}
ASTNode *ast_make_param(DataType t, const char *name, int line) {
    ASTNode *n = ast_alloc(NODE_PARAM, line);
    n->data_type = t;
    n->name      = strdup(name);
    return n;
}
ASTNode *ast_make_block(int line) {
    return ast_alloc(NODE_BLOCK, line);
}
ASTNode *ast_make_if(ASTNode *cond, ASTNode *body, ASTNode *else_body, int line) {
    ASTNode *n   = ast_alloc(NODE_IF, line);
    n->cond      = cond;
    n->body      = body;
    n->else_body = else_body;
    return n;
}
ASTNode *ast_make_while(ASTNode *cond, ASTNode *body, int line) {
    ASTNode *n = ast_alloc(NODE_WHILE, line);
    n->cond    = cond;
    n->body    = body;
    return n;
}
ASTNode *ast_make_for(ASTNode *init, ASTNode *cond, ASTNode *update,
                      ASTNode *body, int line) {
    ASTNode *n = ast_alloc(NODE_FOR, line);
    n->init    = init;
    n->cond    = cond;
    n->update  = update;
    n->body    = body;
    return n;
}
ASTNode *ast_make_return(ASTNode *expr, int line) {
    ASTNode *n = ast_alloc(NODE_RETURN, line);
    n->left    = expr;
    return n;
}
ASTNode *ast_make_break(int line)    { return ast_alloc(NODE_BREAK, line); }
ASTNode *ast_make_continue(int line) { return ast_alloc(NODE_CONTINUE, line); }
ASTNode *ast_make_expr_stmt(ASTNode *expr, int line) {
    ASTNode *n = ast_alloc(NODE_EXPR_STMT, line);
    n->left    = expr;
    return n;
}
ASTNode *ast_make_printf(const char *fmt, int line) {
    ASTNode *n  = ast_alloc(NODE_PRINTF, line);
    n->lit.sval = strdup(fmt);
    return n;
}
ASTNode *ast_make_scanf(const char *fmt, int line) {
    ASTNode *n  = ast_alloc(NODE_SCANF, line);
    n->lit.sval = strdup(fmt);
    return n;
}
ASTNode *ast_make_program(int line) { return ast_alloc(NODE_PROGRAM, line); }

void ast_add_child(ASTNode *parent, ASTNode *child) {
    if (!parent || !child) return;
    if (parent->child_count >= parent->child_cap) {
        parent->child_cap = parent->child_cap ? parent->child_cap * 2 : 8;
        parent->children  = realloc(parent->children,
                                    parent->child_cap * sizeof(ASTNode *));
        if (!parent->children) { perror("ast_add_child"); exit(1); }
    }
    parent->children[parent->child_count++] = child;
}

static const char *node_type_name(NodeType t) {
    switch (t) {
        case NODE_INT_LIT:    return "IntLit";
        case NODE_FLOAT_LIT:  return "FloatLit";
        case NODE_CHAR_LIT:   return "CharLit";
        case NODE_STRING_LIT: return "StringLit";
        case NODE_IDENT:      return "Ident";
        case NODE_ARRAY_ACCESS:return "ArrayAccess";
        case NODE_BINOP:      return "BinOp";
        case NODE_UNOP:       return "UnOp";
        case NODE_ASSIGN:     return "Assign";
        case NODE_CALL:       return "Call";
        case NODE_VAR_DECL:   return "VarDecl";
        case NODE_ARRAY_DECL: return "ArrayDecl";
        case NODE_FUNC_DECL:  return "FuncDecl";
        case NODE_PARAM:      return "Param";
        case NODE_BLOCK:      return "Block";
        case NODE_IF:         return "If";
        case NODE_WHILE:      return "While";
        case NODE_FOR:        return "For";
        case NODE_RETURN:     return "Return";
        case NODE_BREAK:      return "Break";
        case NODE_CONTINUE:   return "Continue";
        case NODE_EXPR_STMT:  return "ExprStmt";
        case NODE_PRINTF:     return "Printf";
        case NODE_SCANF:      return "Scanf";
        case NODE_PROGRAM:    return "Program";
        case NODE_LIST:       return "List";
        default:              return "Unknown";
    }
}

void ast_print(ASTNode *node, int depth) {
    if (!node) return;
    for (int i = 0; i < depth; i++) printf("  ");
    printf("[%s", node_type_name(node->type));
    if (node->name)
        printf(" name='%s'", node->name);
    if (node->type == NODE_INT_LIT)
        printf(" val=%d", node->lit.ival);
    if (node->type == NODE_FLOAT_LIT)
        printf(" val=%g", node->lit.fval);
    if (node->type == NODE_CHAR_LIT)
        printf(" val='%c'", (char)node->lit.ival);
    if (node->type == NODE_STRING_LIT)
        printf(" val=%s", node->lit.sval);
    if (node->data_type != TYPE_UNKNOWN)
        printf(" type=%s", type_to_str(node->data_type));
    printf("] (line %d)\n", node->line);

    ast_print(node->init, depth + 1);
    ast_print(node->cond, depth + 1);
    ast_print(node->left, depth + 1);
    ast_print(node->right, depth + 1);
    ast_print(node->update, depth + 1);
    ast_print(node->body, depth + 1);
    ast_print(node->else_body, depth + 1);
    for (int i = 0; i < node->child_count; i++)
        ast_print(node->children[i], depth + 1);
}

void ast_free(ASTNode *node) {
    if (!node) return;
    free(node->name);
    if (node->type == NODE_STRING_LIT || node->type == NODE_PRINTF ||
        node->type == NODE_SCANF)
        free(node->lit.sval);
    ast_free(node->init);
    ast_free(node->cond);
    ast_free(node->left);
    ast_free(node->right);
    ast_free(node->update);
    ast_free(node->body);
    ast_free(node->else_body);
    for (int i = 0; i < node->child_count; i++)
        ast_free(node->children[i]);
    free(node->children);
    free(node);
}
