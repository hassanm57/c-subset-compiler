%{
/*
 * parser.y - Bison Grammar for C-Subset Compiler
 *
 * Grammar covers:
 *   - Global variable/array declarations
 *   - Function definitions with parameters
 *   - Arithmetic, relational, logical expressions
 *   - if/else, while, for statements
 *   - return, break, continue
 *   - printf / scanf (built-in)
 *   - Array indexing
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "symtable.h"

extern int  yylex(void);
extern int  line_num;
extern int  col_num;

void yyerror(const char *msg);

ASTNode *ast_root = NULL;   /* filled after successful parse */

/* Helper: collect parameter types from param-node list */
static void collect_param_info(ASTNode *func_node,
                                int *count, DataType *types) {
    *count = 0;
    for (int i = 0; i < func_node->child_count; i++) {
        ASTNode *p = func_node->children[i];
        if (p->type == NODE_PARAM && *count < 16) {
            types[(*count)++] = p->data_type;
        }
    }
}

%}

/* ---- Union ---- */
%union {
    int      ival;
    double   fval;
    char    *sval;
    struct ASTNode *node;
    struct {
        struct ASTNode **items;
        int count;
        int cap;
    } nodelist;
}

/* ---- Tokens ---- */
%token <ival>  INT_LITERAL CHAR_LITERAL
%token <fval>  FLOAT_LITERAL
%token <sval>  STRING_LITERAL IDENTIFIER

%token INT FLOAT CHAR VOID
%token IF ELSE WHILE FOR RETURN BREAK CONTINUE
%token PRINTF SCANF

%token PLUS MINUS STAR SLASH MOD
%token ASSIGN PLUS_ASSIGN MINUS_ASSIGN STAR_ASSIGN SLASH_ASSIGN
%token INC DEC
%token EQ NEQ LT LE GT GE
%token AND OR NOT
%token AMPERSAND
%token LPAREN RPAREN LBRACE RBRACE LBRACKET RBRACKET
%token SEMICOLON COMMA

/* ---- Types ---- */
%type <node>     program declaration func_decl var_decl array_decl
%type <node>     param_decl
%type <node>     block stmt expr
%type <node>     if_stmt while_stmt for_stmt return_stmt
%type <node>     printf_stmt scanf_stmt
%type <node>     type_spec

%type <nodelist> param_list param_list_ne
%type <nodelist> arg_list arg_list_ne
%type <nodelist> stmt_list decl_list

%type <sval>     assign_op

/* ---- Precedence (lowest to highest) ---- */
%right ASSIGN PLUS_ASSIGN MINUS_ASSIGN STAR_ASSIGN SLASH_ASSIGN
%left  OR
%left  AND
%left  EQ NEQ
%left  LT LE GT GE
%left  PLUS MINUS
%left  STAR SLASH MOD
%right NOT UMINUS UPLUS PREINC PREDEC
%left  INC DEC
%left  LBRACKET LPAREN

%nonassoc THEN
%nonassoc ELSE

%%

/* ================================================================
   Top-level
   ================================================================ */

program
    : decl_list
        {
            $$ = ast_make_program(1);
            for (int i = 0; i < $1.count; i++)
                ast_add_child($$, $1.items[i]);
            free($1.items);
            ast_root = $$;
        }
    ;

decl_list
    : /* empty */
        { $$.items = NULL; $$.count = 0; $$.cap = 0; }
    | decl_list declaration
        {
            $$ = $1;
            if ($2) {
                if ($$.count >= $$.cap) {
                    $$.cap = $$.cap ? $$.cap * 2 : 8;
                    $$.items = realloc($$.items, $$.cap * sizeof(ASTNode *));
                }
                $$.items[$$.count++] = $2;
            }
        }
    ;

declaration
    : func_decl   { $$ = $1; }
    | var_decl    { $$ = $1; }
    | array_decl  { $$ = $1; }
    ;

/* ================================================================
   Types
   ================================================================ */

type_spec
    : INT   { $$ = ast_make_var_decl(TYPE_INT,   "__type__", NULL, line_num); }
    | FLOAT { $$ = ast_make_var_decl(TYPE_FLOAT, "__type__", NULL, line_num); }
    | CHAR  { $$ = ast_make_var_decl(TYPE_CHAR,  "__type__", NULL, line_num); }
    | VOID  { $$ = ast_make_var_decl(TYPE_VOID,  "__type__", NULL, line_num); }
    ;

/* ================================================================
   Variable declarations
   ================================================================ */

var_decl
    : type_spec IDENTIFIER SEMICOLON
        {
$$ = ast_make_var_decl($1->data_type, $2, NULL, line_num);
            free($2);
            ast_free($1);
        }
    | type_spec IDENTIFIER ASSIGN expr SEMICOLON
        {
            $$ = ast_make_var_decl($1->data_type, $2, $4, line_num);
            free($2);
            ast_free($1);
        }
    ;

array_decl
    : type_spec IDENTIFIER LBRACKET INT_LITERAL RBRACKET SEMICOLON
        {
            $$ = ast_make_array_decl($1->data_type, $2, $4, line_num);
            free($2);
            ast_free($1);
        }
    | type_spec IDENTIFIER LBRACKET INT_LITERAL RBRACKET
      ASSIGN LBRACE arg_list_ne RBRACE SEMICOLON
        {
            $$ = ast_make_array_decl($1->data_type, $2, $4, line_num);
            /* Store initializer list as children */
            for (int i = 0; i < $8.count; i++)
                ast_add_child($$, $8.items[i]);
            free($8.items);
            free($2);
            ast_free($1);
        }
    ;

/* ================================================================
   Function declarations
   ================================================================ */

func_decl
    : type_spec IDENTIFIER LPAREN param_list RPAREN block
        {
            $$ = ast_make_func_decl($1->data_type, $2, line_num);
            /* Add params as children first */
            for (int i = 0; i < $4.count; i++)
                ast_add_child($$, $4.items[i]);
            free($4.items);
            /* Add body */
            $$->body = $6;
            free($2);
            ast_free($1);
        }
    ;

param_list
    : /* empty */
        { $$.items = NULL; $$.count = 0; $$.cap = 0; }
    | param_list_ne
        { $$ = $1; }
    ;

param_list_ne
    : param_decl
        {
            $$.cap   = 4;
            $$.items = malloc($$.cap * sizeof(ASTNode *));
            $$.count = 0;
            $$.items[$$.count++] = $1;
        }
    | param_list_ne COMMA param_decl
        {
            $$ = $1;
            if ($$.count >= $$.cap) {
                $$.cap *= 2;
                $$.items = realloc($$.items, $$.cap * sizeof(ASTNode *));
            }
            $$.items[$$.count++] = $3;
        }
    ;

param_decl
    : type_spec IDENTIFIER
        {
            $$ = ast_make_param($1->data_type, $2, line_num);
            free($2);
            ast_free($1);
        }
    | type_spec IDENTIFIER LBRACKET RBRACKET
        {
            $$ = ast_make_param($1->data_type, $2, line_num);
            $$->lit.ival = -1; /* marks array param */
            free($2);
            ast_free($1);
        }
    ;

/* ================================================================
   Statements
   ================================================================ */

block
    : LBRACE stmt_list RBRACE
        {
            $$ = ast_make_block(line_num);
            for (int i = 0; i < $2.count; i++)
                ast_add_child($$, $2.items[i]);
            free($2.items);
        }
    ;

stmt_list
    : /* empty */
        { $$.items = NULL; $$.count = 0; $$.cap = 0; }
    | stmt_list stmt
        {
            $$ = $1;
            if ($2) {
                if ($$.count >= $$.cap) {
                    $$.cap = $$.cap ? $$.cap * 2 : 8;
                    $$.items = realloc($$.items, $$.cap * sizeof(ASTNode *));
                }
                $$.items[$$.count++] = $2;
            }
        }
    ;

stmt
    : var_decl          { $$ = $1; }
    | array_decl        { $$ = $1; }
    | expr SEMICOLON    { $$ = ast_make_expr_stmt($1, line_num); }
    | if_stmt           { $$ = $1; }
    | while_stmt        { $$ = $1; }
    | for_stmt          { $$ = $1; }
    | return_stmt       { $$ = $1; }
    | BREAK SEMICOLON   { $$ = ast_make_break(line_num); }
    | CONTINUE SEMICOLON{ $$ = ast_make_continue(line_num); }
    | printf_stmt       { $$ = $1; }
    | scanf_stmt        { $$ = $1; }
    | block             { $$ = $1; }
    | SEMICOLON         { $$ = NULL; }
    ;

if_stmt
    : IF LPAREN expr RPAREN stmt %prec THEN
        { $$ = ast_make_if($3, $5, NULL, line_num); }
    | IF LPAREN expr RPAREN stmt ELSE stmt
        { $$ = ast_make_if($3, $5, $7, line_num); }
    ;

while_stmt
    : WHILE LPAREN expr RPAREN stmt
        { $$ = ast_make_while($3, $5, line_num); }
    ;

for_stmt
    : FOR LPAREN for_init SEMICOLON expr SEMICOLON expr RPAREN stmt
        { $$ = ast_make_for($<node>3, $5, $7, $9, line_num); }
    | FOR LPAREN for_init SEMICOLON SEMICOLON expr RPAREN stmt
        { $$ = ast_make_for($<node>3, NULL, $6, $8, line_num); }
    ;

for_init
    : expr          { $<node>$ = ast_make_expr_stmt($1, line_num); }
    | type_spec IDENTIFIER ASSIGN expr
        {
            $<node>$ = ast_make_var_decl($1->data_type, $2, $4, line_num);
            free($2); ast_free($1);
        }
    | /* empty */   { $<node>$ = NULL; }
    ;

return_stmt
    : RETURN expr SEMICOLON { $$ = ast_make_return($2, line_num); }
    | RETURN SEMICOLON      { $$ = ast_make_return(NULL, line_num); }
    ;

printf_stmt
    : PRINTF LPAREN STRING_LITERAL RPAREN SEMICOLON
        {
            $$ = ast_make_printf($3, line_num);
            free($3);
        }
    | PRINTF LPAREN STRING_LITERAL COMMA arg_list_ne RPAREN SEMICOLON
        {
            $$ = ast_make_printf($3, line_num);
            for (int i = 0; i < $5.count; i++)
                ast_add_child($$, $5.items[i]);
            free($5.items);
            free($3);
        }
    ;

scanf_stmt
    : SCANF LPAREN STRING_LITERAL COMMA arg_list_ne RPAREN SEMICOLON
        {
            $$ = ast_make_scanf($3, line_num);
            for (int i = 0; i < $5.count; i++)
                ast_add_child($$, $5.items[i]);
            free($5.items);
            free($3);
        }
    ;

/* ================================================================
   Expressions
   ================================================================ */

expr
    : INT_LITERAL                   { $$ = ast_make_int($1, line_num); }
    | FLOAT_LITERAL                 { $$ = ast_make_float($1, line_num); }
    | CHAR_LITERAL                  { $$ = ast_make_char($1, line_num); }
    | STRING_LITERAL                { $$ = ast_make_string($1, line_num); free($1); }
    | IDENTIFIER                    { $$ = ast_make_ident($1, line_num); free($1); }

    /* Array access */
    | IDENTIFIER LBRACKET expr RBRACKET
        {
            $$ = ast_make_array_access($1, $3, line_num);
            free($1);
        }

    /* Function call */
    | IDENTIFIER LPAREN arg_list RPAREN
        {
            $$ = ast_make_call($1, line_num);
            for (int i = 0; i < $3.count; i++)
                ast_add_child($$, $3.items[i]);
            free($3.items);
            free($1);
        }

    /* Assignments */
    | IDENTIFIER assign_op expr
        {
            ASTNode *lhs = ast_make_ident($1, line_num);
            $$ = ast_make_assign($2, lhs, $3, line_num);
            free($1); free($2);
        }
    | IDENTIFIER LBRACKET expr RBRACKET assign_op expr
        {
            ASTNode *lhs = ast_make_array_access($1, $3, line_num);
            $$ = ast_make_assign($5, lhs, $6, line_num);
            free($1); free($5);
        }

    /* Binary operators */
    | expr PLUS  expr  { $$ = ast_make_binop("+",  $1, $3, line_num); }
    | expr MINUS expr  { $$ = ast_make_binop("-",  $1, $3, line_num); }
    | expr STAR  expr  { $$ = ast_make_binop("*",  $1, $3, line_num); }
    | expr SLASH expr  { $$ = ast_make_binop("/",  $1, $3, line_num); }
    | expr MOD   expr  { $$ = ast_make_binop("%",  $1, $3, line_num); }
    | expr EQ    expr  { $$ = ast_make_binop("==", $1, $3, line_num); }
    | expr NEQ   expr  { $$ = ast_make_binop("!=", $1, $3, line_num); }
    | expr LT    expr  { $$ = ast_make_binop("<",  $1, $3, line_num); }
    | expr LE    expr  { $$ = ast_make_binop("<=", $1, $3, line_num); }
    | expr GT    expr  { $$ = ast_make_binop(">",  $1, $3, line_num); }
    | expr GE    expr  { $$ = ast_make_binop(">=", $1, $3, line_num); }
    | expr AND   expr  { $$ = ast_make_binop("&&", $1, $3, line_num); }
    | expr OR    expr  { $$ = ast_make_binop("||", $1, $3, line_num); }

    /* Unary operators */
    | NOT   expr       { $$ = ast_make_unop("!",  $2, line_num); }
    | MINUS expr %prec UMINUS { $$ = ast_make_unop("-", $2, line_num); }
    | PLUS  expr %prec UPLUS  { $$ = $2; }
    | AMPERSAND IDENTIFIER
        {
            ASTNode *inner = ast_make_ident($2, line_num);
            $$ = ast_make_unop("&", inner, line_num);
            free($2);
        }
    | STAR IDENTIFIER %prec UMINUS
        {
            ASTNode *inner = ast_make_ident($2, line_num);
            $$ = ast_make_unop("*", inner, line_num);
            free($2);
        }

    /* Inc/Dec */
    | INC IDENTIFIER %prec PREINC
        {
            ASTNode *id = ast_make_ident($2, line_num);
            $$ = ast_make_unop("++pre", id, line_num);
            free($2);
        }
    | DEC IDENTIFIER %prec PREDEC
        {
            ASTNode *id = ast_make_ident($2, line_num);
            $$ = ast_make_unop("--pre", id, line_num);
            free($2);
        }
    | IDENTIFIER INC
        {
            ASTNode *id = ast_make_ident($1, line_num);
            $$ = ast_make_unop("++post", id, line_num);
            free($1);
        }
    | IDENTIFIER DEC
        {
            ASTNode *id = ast_make_ident($1, line_num);
            $$ = ast_make_unop("--post", id, line_num);
            free($1);
        }

    /* Parenthesized */
    | LPAREN expr RPAREN { $$ = $2; }
    ;

assign_op
    : ASSIGN        { $$ = strdup("=");  }
    | PLUS_ASSIGN   { $$ = strdup("+="); }
    | MINUS_ASSIGN  { $$ = strdup("-="); }
    | STAR_ASSIGN   { $$ = strdup("*="); }
    | SLASH_ASSIGN  { $$ = strdup("/="); }
    ;

arg_list
    : /* empty */ { $$.items = NULL; $$.count = 0; $$.cap = 0; }
    | arg_list_ne { $$ = $1; }
    ;

arg_list_ne
    : expr
        {
            $$.cap   = 4;
            $$.items = malloc($$.cap * sizeof(ASTNode *));
            $$.count = 0;
            $$.items[$$.count++] = $1;
        }
    | arg_list_ne COMMA expr
        {
            $$ = $1;
            if ($$.count >= $$.cap) {
                $$.cap *= 2;
                $$.items = realloc($$.items, $$.cap * sizeof(ASTNode *));
            }
            $$.items[$$.count++] = $3;
        }
    ;

%%

void yyerror(const char *msg) {
    fprintf(stderr, "[Parse Error] Line %d: %s\n", line_num, msg);
}
