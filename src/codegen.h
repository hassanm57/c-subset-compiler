#ifndef CODEGEN_H
#define CODEGEN_H

/*
 * codegen.h - Three-Address Code (TAC) Intermediate Representation
 *
 * TAC instruction set:
 *   ASSIGN   t = operand
 *   BINOP    t = a op b
 *   UNOP     t = op a
 *   LABEL    L:
 *   GOTO     goto L
 *   IF_TRUE  if t goto L
 *   IF_FALSE if !t goto L
 *   PARAM    param t
 *   CALL     t = call f, n
 *   RETURN   return [t]
 *   ARRAY_STORE  a[i] = t
 *   ARRAY_LOAD   t = a[i]
 *   PRINTF   printf fmt, args...
 *   SCANF    scanf fmt, &var
 *   FUNC_BEGIN / FUNC_END
 */

#include "ast.h"

#define MAX_TAC       8192
#define MAX_OPERAND   64

typedef enum {
    TAC_ASSIGN,
    TAC_BINOP,
    TAC_UNOP,
    TAC_LABEL,
    TAC_GOTO,
    TAC_IF_TRUE,
    TAC_IF_FALSE,
    TAC_PARAM,
    TAC_CALL,
    TAC_RETURN,
    TAC_ARRAY_STORE,
    TAC_ARRAY_LOAD,
    TAC_PRINTF,
    TAC_SCANF,
    TAC_FUNC_BEGIN,
    TAC_FUNC_END
} TACOp;

typedef struct TACInstr {
    TACOp  op;
    char   result[MAX_OPERAND];
    char   arg1[MAX_OPERAND];
    char   arg2[MAX_OPERAND];
    int    n_args;          /* for CALL: number of params */
} TACInstr;

typedef struct TACList {
    TACInstr instrs[MAX_TAC];
    int      count;
} TACList;

/* IR generation */
void     tac_init(TACList *tl);
void     tac_generate(TACList *tl, ASTNode *root);
void     tac_print(TACList *tl, FILE *out);
void     tac_print_file(TACList *tl, const char *filename);

/* New temp / label generators */
char    *tac_new_temp(void);
char    *tac_new_label(void);

#endif /* CODEGEN_H */
