/* A Bison parser, made by GNU Bison 2.7.  */

/* Bison interface for Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2012 Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_SRC_PARSER_TAB_H_INCLUDED
# define YY_YY_SRC_PARSER_TAB_H_INCLUDED
/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     INT_LITERAL = 258,
     CHAR_LITERAL = 259,
     FLOAT_LITERAL = 260,
     STRING_LITERAL = 261,
     IDENTIFIER = 262,
     INT = 263,
     FLOAT = 264,
     CHAR = 265,
     VOID = 266,
     IF = 267,
     ELSE = 268,
     WHILE = 269,
     FOR = 270,
     RETURN = 271,
     BREAK = 272,
     CONTINUE = 273,
     PRINTF = 274,
     SCANF = 275,
     PLUS = 276,
     MINUS = 277,
     STAR = 278,
     SLASH = 279,
     MOD = 280,
     ASSIGN = 281,
     PLUS_ASSIGN = 282,
     MINUS_ASSIGN = 283,
     STAR_ASSIGN = 284,
     SLASH_ASSIGN = 285,
     INC = 286,
     DEC = 287,
     EQ = 288,
     NEQ = 289,
     LT = 290,
     LE = 291,
     GT = 292,
     GE = 293,
     AND = 294,
     OR = 295,
     NOT = 296,
     AMPERSAND = 297,
     LPAREN = 298,
     RPAREN = 299,
     LBRACE = 300,
     RBRACE = 301,
     LBRACKET = 302,
     RBRACKET = 303,
     SEMICOLON = 304,
     COMMA = 305,
     PREDEC = 306,
     PREINC = 307,
     UPLUS = 308,
     UMINUS = 309,
     THEN = 310
   };
#endif


#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{
/* Line 2058 of yacc.c  */
#line 44 "src/parser.y"

    int      ival;
    double   fval;
    char    *sval;
    struct ASTNode *node;
    struct {
        struct ASTNode **items;
        int count;
        int cap;
    } nodelist;


/* Line 2058 of yacc.c  */
#line 125 "src/parser.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */

#endif /* !YY_YY_SRC_PARSER_TAB_H_INCLUDED  */
