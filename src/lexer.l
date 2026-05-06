%{
/*
 * lexer.l - Lexical Analyzer for C-Subset Compiler
 * Handles: keywords, identifiers, literals, operators, punctuation
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.tab.h"
#include "symtable.h"

int line_num = 1;
int col_num  = 1;

void lex_error(const char *msg) {
    fprintf(stderr, "[Lexer Error] Line %d, Col %d: %s near '%s'\n",
            line_num, col_num, msg, yytext);
}

/* Track column */
#define YY_USER_ACTION col_num += yyleng;

%}

%option noyywrap
%option yylineno

DIGIT       [0-9]
LETTER      [a-zA-Z_]
IDENT       {LETTER}({LETTER}|{DIGIT})*
INT_LIT     {DIGIT}+
FLOAT_LIT   {DIGIT}+"."{DIGIT}*
STR_LIT     \"([^\"\\\n]|\\.)*\"
CHAR_LIT    \'([^\'\\\n]|\\.)\'

%%

[ \t\r]+            { col_num += yyleng; }  /* whitespace */
\n                  { line_num++; col_num = 1; }

"/*"                {   /* Block comment */
                        int c1 = 0, c2 = input();
                        while (c2 != EOF) {
                            if (c1 == '*' && c2 == '/') break;
                            if (c2 == '\n') { line_num++; col_num = 1; }
                            c1 = c2; c2 = input();
                        }
                    }
"//"[^\n]*          { /* Line comment */ }

"int"               { return INT; }
"float"             { return FLOAT; }
"char"              { return CHAR; }
"void"              { return VOID; }
"if"                { return IF; }
"else"              { return ELSE; }
"while"             { return WHILE; }
"for"               { return FOR; }
"return"            { return RETURN; }
"break"             { return BREAK; }
"continue"          { return CONTINUE; }
"printf"            { return PRINTF; }
"scanf"             { return SCANF; }

{IDENT}             {
                        yylval.sval = strdup(yytext);
                        return IDENTIFIER;
                    }

{INT_LIT}           {
                        yylval.ival = atoi(yytext);
                        return INT_LITERAL;
                    }

{FLOAT_LIT}         {
                        yylval.fval = atof(yytext);
                        return FLOAT_LITERAL;
                    }

{STR_LIT}           {
                        yylval.sval = strdup(yytext);
                        return STRING_LITERAL;
                    }

{CHAR_LIT}          {
                        yylval.ival = (int)yytext[1];
                        return CHAR_LITERAL;
                    }

"+"                 { return PLUS; }
"-"                 { return MINUS; }
"*"                 { return STAR; }
"/"                 { return SLASH; }
"%"                 { return MOD; }
"="                 { return ASSIGN; }
"+="                { return PLUS_ASSIGN; }
"-="                { return MINUS_ASSIGN; }
"*="                { return STAR_ASSIGN; }
"/="                { return SLASH_ASSIGN; }
"++"                { return INC; }
"--"                { return DEC; }
"=="                { return EQ; }
"!="                { return NEQ; }
"<"                 { return LT; }
"<="                { return LE; }
">"                 { return GT; }
">="                { return GE; }
"&&"                { return AND; }
"||"                { return OR; }
"!"                 { return NOT; }
"&"                 { return AMPERSAND; }

"("                 { return LPAREN; }
")"                 { return RPAREN; }
"{"                 { return LBRACE; }
"}"                 { return RBRACE; }
"["                 { return LBRACKET; }
"]"                 { return RBRACKET; }
";"                 { return SEMICOLON; }
","                 { return COMMA; }

.                   { lex_error("Unexpected character"); }

%%
