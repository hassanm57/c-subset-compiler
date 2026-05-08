%{
/*
 * lexer.l - Lexical Analyzer for C-Subset Compiler
 * Handles: keywords, identifiers, literals, operators, punctuation
 *
 * IMPROVEMENTS OVER ORIGINAL:
 * [IMP-1] Invalid identifier detection (e.g. 1x) now caught at lexer level
 * [IMP-2] Integer overflow detection — warns if literal exceeds INT_MAX
 * [IMP-3] Unterminated string literal detection with clear error message
 * [IMP-4] Unterminated block comment detection with clear error message
 * [IMP-5] Unterminated char literal detection with clear error message
 * [IMP-6] Multi-character char literal detection (e.g. 'ab') caught at lexer level
 * [IMP-7] Column number reset fixed — was double-counting in whitespace rule
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "parser.tab.h"
#include "symtable.h"

int line_num = 1;
int col_num  = 1;

void lex_error(const char *msg) {
    fprintf(stderr, "[Lexer Error] Line %d, Col %d: %s near '%s'\n",
            line_num, col_num, msg, yytext);
    semantic_errors++;
}

void lex_warning(const char *msg) {
    fprintf(stderr, "[Lexer Warning] Line %d, Col %d: %s near '%s'\n",
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
CHAR_LIT    \'([^\'\\\n]|\\.)?\'

%%

[ \t\r]+            { /* [IMP-7] removed col_num += yyleng here — YY_USER_ACTION handles it */ }
\n                  { line_num++; col_num = 1; }

"/*"                {
                        /* [IMP-4] Unterminated block comment detection */
                        int c1 = 0, c2 = input();
                        while (c2 != EOF) {
                            if (c1 == '*' && c2 == '/') break;
                            if (c2 == '\n') { line_num++; col_num = 1; }
                            c1 = c2; c2 = input();
                        }
                        if (c2 == EOF) {
                            lex_error("Unterminated block comment — missing closing '*/'");
                        }
                    }
"//"[^\n]*          { /* Line comment — consume to end of line */ }

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

{DIGIT}+{LETTER}({LETTER}|{DIGIT})* {
                        /* [IMP-1] Identifier starting with digit — lexer error */
                        lex_error("Invalid identifier — identifiers cannot start with a digit");
                    }

{IDENT}             {
                        yylval.sval = strdup(yytext);
                        return IDENTIFIER;
                    }

{INT_LIT}           {
                        /* [IMP-2] Integer overflow detection */
                        long val = atol(yytext);
                        if (val > INT_MAX || val < INT_MIN) {
                            lex_warning("Integer literal overflow — value exceeds INT_MAX");
                        }
                        yylval.ival = (int)val;
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

\"([^\"\\\n]|\\.)*$ {
                        /* [IMP-3] Unterminated string literal — no closing quote before end of line */
                        lex_error("Unterminated string literal — missing closing '\"'");
                    }

\'([^\'\\\n]|\\.){2,}\' {
                        /* [IMP-6] Multi-character char literal e.g. 'ab' */
                        lex_error("Multi-character char literal is not allowed");
                    }

{CHAR_LIT}          {
                        yylval.ival = (int)yytext[1];
                        return CHAR_LITERAL;
                    }

\'([^\'\\\n]|\\.)?$ {
                        /* [IMP-5] Unterminated char literal — no closing quote */
                        lex_error("Unterminated char literal — missing closing \"'\"");
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