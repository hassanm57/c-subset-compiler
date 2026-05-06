#ifndef AST_H
#define AST_H

/*
 * ast.h - Abstract Syntax Tree Node Definitions
 */

#include "symtable.h"

typedef enum {
    /* Literals */
    NODE_INT_LIT,
    NODE_FLOAT_LIT,
    NODE_CHAR_LIT,
    NODE_STRING_LIT,

    /* Identifier / array access */
    NODE_IDENT,
    NODE_ARRAY_ACCESS,

    /* Expressions */
    NODE_BINOP,
    NODE_UNOP,
    NODE_ASSIGN,
    NODE_CALL,

    /* Declarations */
    NODE_VAR_DECL,
    NODE_ARRAY_DECL,
    NODE_FUNC_DECL,
    NODE_PARAM,

    /* Statements */
    NODE_BLOCK,
    NODE_IF,
    NODE_WHILE,
    NODE_FOR,
    NODE_RETURN,
    NODE_BREAK,
    NODE_CONTINUE,
    NODE_EXPR_STMT,
    NODE_PRINTF,
    NODE_SCANF,

    /* Program root */
    NODE_PROGRAM,

    /* Argument/statement list helper */
    NODE_LIST
} NodeType;

typedef struct ASTNode {
    NodeType    type;
    DataType    data_type;    /* type inferred during semantic analysis */
    int         line;

    union {
        int         ival;
        double      fval;
        char       *sval;
    } lit;

    char       *name;         /* identifier name, operator string */

    struct ASTNode *left;
    struct ASTNode *right;
    struct ASTNode *cond;
    struct ASTNode *init;
    struct ASTNode *update;
    struct ASTNode *body;
    struct ASTNode *else_body;

    /* For call args / block stmts / param lists — dynamic list */
    struct ASTNode **children;
    int              child_count;
    int              child_cap;
} ASTNode;

ASTNode *ast_make_int(int val, int line);
ASTNode *ast_make_float(double val, int line);
ASTNode *ast_make_char(int val, int line);
ASTNode *ast_make_string(const char *s, int line);
ASTNode *ast_make_ident(const char *name, int line);
ASTNode *ast_make_array_access(const char *name, ASTNode *idx, int line);
ASTNode *ast_make_binop(const char *op, ASTNode *l, ASTNode *r, int line);
ASTNode *ast_make_unop(const char *op, ASTNode *operand, int line);
ASTNode *ast_make_assign(const char *op, ASTNode *lhs, ASTNode *rhs, int line);
ASTNode *ast_make_call(const char *name, int line);
ASTNode *ast_make_var_decl(DataType t, const char *name, ASTNode *init, int line);
ASTNode *ast_make_array_decl(DataType t, const char *name, int size, int line);
ASTNode *ast_make_func_decl(DataType ret, const char *name, int line);
ASTNode *ast_make_param(DataType t, const char *name, int line);
ASTNode *ast_make_block(int line);
ASTNode *ast_make_if(ASTNode *cond, ASTNode *body, ASTNode *else_body, int line);
ASTNode *ast_make_while(ASTNode *cond, ASTNode *body, int line);
ASTNode *ast_make_for(ASTNode *init, ASTNode *cond, ASTNode *update,
                      ASTNode *body, int line);
ASTNode *ast_make_return(ASTNode *expr, int line);
ASTNode *ast_make_break(int line);
ASTNode *ast_make_continue(int line);
ASTNode *ast_make_expr_stmt(ASTNode *expr, int line);
ASTNode *ast_make_printf(const char *fmt, int line);
ASTNode *ast_make_scanf(const char *fmt, int line);
ASTNode *ast_make_program(int line);

void ast_add_child(ASTNode *parent, ASTNode *child);

void ast_print(ASTNode *node, int depth);
void ast_free(ASTNode *node);

#endif /* AST_H */
