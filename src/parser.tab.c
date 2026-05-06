/* A Bison parser, made by GNU Bison 2.7.  */

/* Bison implementation for Yacc-like parsers in C
   
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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.7"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
/* Line 371 of yacc.c  */
#line 1 "src/parser.y"

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


/* Line 371 of yacc.c  */
#line 110 "src/parser.tab.c"

# ifndef YY_NULL
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULL nullptr
#  else
#   define YY_NULL 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "parser.tab.h".  */
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
/* Line 387 of yacc.c  */
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


/* Line 387 of yacc.c  */
#line 221 "src/parser.tab.c"
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

/* Copy the second part of user declarations.  */

/* Line 390 of yacc.c  */
#line 249 "src/parser.tab.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(N) (N)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (YYID (0))
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   567

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  56
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  25
/* YYNRULES -- Number of rules.  */
#define YYNRULES  92
/* YYNRULES -- Number of states.  */
#define YYNSTATES  180

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   310

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     5,     6,     9,    11,    13,    15,    17,
      19,    21,    23,    27,    33,    40,    51,    58,    59,    61,
      63,    67,    70,    75,    79,    80,    83,    85,    87,    90,
      92,    94,    96,    98,   101,   104,   106,   108,   110,   112,
     118,   126,   132,   142,   151,   153,   158,   159,   163,   166,
     172,   180,   188,   190,   192,   194,   196,   198,   203,   208,
     212,   219,   223,   227,   231,   235,   239,   243,   247,   251,
     255,   259,   263,   267,   271,   274,   277,   280,   283,   286,
     289,   292,   295,   298,   302,   304,   306,   308,   310,   312,
     313,   315,   317
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      57,     0,    -1,    58,    -1,    -1,    58,    59,    -1,    63,
      -1,    61,    -1,    62,    -1,     8,    -1,     9,    -1,    10,
      -1,    11,    -1,    60,     7,    49,    -1,    60,     7,    26,
      77,    49,    -1,    60,     7,    47,     3,    48,    49,    -1,
      60,     7,    47,     3,    48,    26,    45,    80,    46,    49,
      -1,    60,     7,    43,    64,    44,    67,    -1,    -1,    65,
      -1,    66,    -1,    65,    50,    66,    -1,    60,     7,    -1,
      60,     7,    47,    48,    -1,    45,    68,    46,    -1,    -1,
      68,    69,    -1,    61,    -1,    62,    -1,    77,    49,    -1,
      70,    -1,    71,    -1,    72,    -1,    74,    -1,    17,    49,
      -1,    18,    49,    -1,    75,    -1,    76,    -1,    67,    -1,
      49,    -1,    12,    43,    77,    44,    69,    -1,    12,    43,
      77,    44,    69,    13,    69,    -1,    14,    43,    77,    44,
      69,    -1,    15,    43,    73,    49,    77,    49,    77,    44,
      69,    -1,    15,    43,    73,    49,    49,    77,    44,    69,
      -1,    77,    -1,    60,     7,    26,    77,    -1,    -1,    16,
      77,    49,    -1,    16,    49,    -1,    19,    43,     6,    44,
      49,    -1,    19,    43,     6,    50,    80,    44,    49,    -1,
      20,    43,     6,    50,    80,    44,    49,    -1,     3,    -1,
       5,    -1,     4,    -1,     6,    -1,     7,    -1,     7,    47,
      77,    48,    -1,     7,    43,    79,    44,    -1,     7,    78,
      77,    -1,     7,    47,    77,    48,    78,    77,    -1,    77,
      21,    77,    -1,    77,    22,    77,    -1,    77,    23,    77,
      -1,    77,    24,    77,    -1,    77,    25,    77,    -1,    77,
      33,    77,    -1,    77,    34,    77,    -1,    77,    35,    77,
      -1,    77,    36,    77,    -1,    77,    37,    77,    -1,    77,
      38,    77,    -1,    77,    39,    77,    -1,    77,    40,    77,
      -1,    41,    77,    -1,    22,    77,    -1,    21,    77,    -1,
      42,     7,    -1,    23,     7,    -1,    31,     7,    -1,    32,
       7,    -1,     7,    31,    -1,     7,    32,    -1,    43,    77,
      44,    -1,    26,    -1,    27,    -1,    28,    -1,    29,    -1,
      30,    -1,    -1,    80,    -1,    77,    -1,    80,    50,    77,
      -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   110,   110,   122,   123,   137,   138,   139,   147,   148,
     149,   150,   158,   164,   173,   179,   197,   213,   214,   219,
     226,   238,   244,   258,   269,   270,   284,   285,   286,   287,
     288,   289,   290,   291,   292,   293,   294,   295,   296,   300,
     302,   307,   312,   314,   319,   320,   325,   329,   330,   334,
     339,   350,   365,   366,   367,   368,   369,   372,   379,   389,
     395,   403,   404,   405,   406,   407,   408,   409,   410,   411,
     412,   413,   414,   415,   418,   419,   420,   421,   427,   435,
     441,   447,   453,   461,   465,   466,   467,   468,   469,   473,
     474,   478,   485
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "INT_LITERAL", "CHAR_LITERAL",
  "FLOAT_LITERAL", "STRING_LITERAL", "IDENTIFIER", "INT", "FLOAT", "CHAR",
  "VOID", "IF", "ELSE", "WHILE", "FOR", "RETURN", "BREAK", "CONTINUE",
  "PRINTF", "SCANF", "PLUS", "MINUS", "STAR", "SLASH", "MOD", "ASSIGN",
  "PLUS_ASSIGN", "MINUS_ASSIGN", "STAR_ASSIGN", "SLASH_ASSIGN", "INC",
  "DEC", "EQ", "NEQ", "LT", "LE", "GT", "GE", "AND", "OR", "NOT",
  "AMPERSAND", "LPAREN", "RPAREN", "LBRACE", "RBRACE", "LBRACKET",
  "RBRACKET", "SEMICOLON", "COMMA", "PREDEC", "PREINC", "UPLUS", "UMINUS",
  "THEN", "$accept", "program", "decl_list", "declaration", "type_spec",
  "var_decl", "array_decl", "func_decl", "param_list", "param_list_ne",
  "param_decl", "block", "stmt_list", "stmt", "if_stmt", "while_stmt",
  "for_stmt", "for_init", "return_stmt", "printf_stmt", "scanf_stmt",
  "expr", "assign_op", "arg_list", "arg_list_ne", YY_NULL
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    56,    57,    58,    58,    59,    59,    59,    60,    60,
      60,    60,    61,    61,    62,    62,    63,    64,    64,    65,
      65,    66,    66,    67,    68,    68,    69,    69,    69,    69,
      69,    69,    69,    69,    69,    69,    69,    69,    69,    70,
      70,    71,    72,    72,    73,    73,    73,    74,    74,    75,
      75,    76,    77,    77,    77,    77,    77,    77,    77,    77,
      77,    77,    77,    77,    77,    77,    77,    77,    77,    77,
      77,    77,    77,    77,    77,    77,    77,    77,    77,    77,
      77,    77,    77,    77,    78,    78,    78,    78,    78,    79,
      79,    80,    80
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     0,     2,     1,     1,     1,     1,     1,
       1,     1,     3,     5,     6,    10,     6,     0,     1,     1,
       3,     2,     4,     3,     0,     2,     1,     1,     2,     1,
       1,     1,     1,     2,     2,     1,     1,     1,     1,     5,
       7,     5,     9,     8,     1,     4,     0,     3,     2,     5,
       7,     7,     1,     1,     1,     1,     1,     4,     4,     3,
       6,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     3,     1,     1,     1,     1,     1,     0,
       1,     1,     3
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       3,     0,     2,     1,     8,     9,    10,    11,     4,     0,
       6,     7,     5,     0,     0,    17,     0,    12,    52,    54,
      53,    55,    56,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    18,    19,     0,    84,    85,    86,
      87,    88,    81,    82,    89,     0,     0,    76,    75,    78,
      79,    80,    74,    77,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    13,    21,
       0,     0,     0,    91,     0,    90,     0,    59,    83,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,    73,     0,    24,    16,    20,     0,    14,    58,     0,
      57,    22,     0,     0,    92,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    23,    38,     0,    26,    27,    37,
      25,    29,    30,    31,    32,    35,    36,     0,     0,    60,
       0,     0,    46,    48,     0,    33,    34,     0,     0,     0,
      28,     0,     0,     0,     0,     0,    44,    47,     0,     0,
      15,     0,     0,     0,     0,     0,     0,     0,    39,    41,
       0,     0,     0,    49,     0,     0,     0,    45,     0,     0,
       0,     0,    40,     0,     0,    50,    51,    43,     0,    42
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,     8,   116,   117,   118,    12,    33,    34,
      35,   119,   102,   120,   121,   122,   123,   145,   124,   125,
     126,   127,    46,    74,    75
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -146
static const yytype_int16 yypact[] =
{
    -146,     5,   115,  -146,  -146,  -146,  -146,  -146,  -146,     1,
    -146,  -146,  -146,   -23,   107,   115,    19,  -146,  -146,  -146,
    -146,  -146,    37,   107,   107,    22,    33,    49,   107,    50,
     107,   289,    65,    29,    25,  -146,    31,  -146,  -146,  -146,
    -146,  -146,  -146,  -146,   107,   107,   107,  -146,  -146,  -146,
    -146,  -146,  -146,  -146,   389,   107,   107,   107,   107,   107,
     107,   107,   107,   107,   107,   107,   107,   107,  -146,    41,
      45,   115,   -15,   509,    48,    43,   369,   509,  -146,    58,
      58,  -146,  -146,  -146,    84,    84,    14,    14,    14,    14,
     529,   268,    46,  -146,  -146,  -146,    70,  -146,  -146,   107,
      73,  -146,   153,   107,   509,   107,    88,    90,    91,    55,
      78,    86,    93,    94,  -146,  -146,   134,  -146,  -146,  -146,
    -146,  -146,  -146,  -146,  -146,  -146,  -146,   309,    24,   509,
     107,   107,   277,  -146,   329,  -146,  -146,   138,   139,   -22,
    -146,   102,   413,   437,   145,   104,   509,  -146,   -32,   116,
    -146,   200,   200,   128,   230,   129,   107,   107,   164,  -146,
     107,   107,   349,  -146,   -31,   -27,   200,   509,   461,   107,
     130,   131,  -146,   200,   485,  -146,  -146,  -146,   200,  -146
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -146,  -146,  -146,  -146,     0,   179,   180,  -146,  -146,  -146,
     112,   117,  -146,  -145,  -146,  -146,  -146,  -146,  -146,  -146,
    -146,   -14,    89,  -146,  -102
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_uint8 yytable[] =
{
      31,   128,     9,    14,    14,     3,   158,   159,    13,    47,
      48,    96,   155,   170,    52,    32,    54,   171,   156,    99,
      15,   172,    36,    99,    16,    16,    17,    17,   177,    49,
      73,    76,    77,   179,    97,    55,    56,    57,    58,    59,
      50,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,   164,   165,    51,    53,    18,    19,
      20,    21,    22,    37,    38,    39,    40,    41,    42,    43,
     141,    32,    69,    70,    99,    71,    23,    24,    25,    72,
      44,    57,    58,    59,    45,   104,    26,    27,    92,    73,
      93,   129,    98,    99,   101,   134,    28,    29,    30,    37,
      38,    39,    40,    41,   133,    55,    56,    57,    58,    59,
      18,    19,    20,    21,    22,   103,   142,   143,   146,    62,
      63,    64,    65,     4,     5,     6,     7,   135,    23,    24,
      25,   130,   144,   131,   132,   136,   137,   138,    26,    27,
     162,   139,    73,    73,   148,   149,   167,   168,    28,    29,
      30,   150,   153,   154,   160,   174,    18,    19,    20,    21,
      22,     4,     5,     6,     7,   106,   157,   107,   108,   109,
     110,   111,   112,   113,    23,    24,    25,   166,   163,   175,
     176,    10,    11,    95,    26,    27,     0,    94,     0,   105,
       0,     0,     0,     0,    28,    29,    30,     0,    93,   114,
       0,     0,   115,    18,    19,    20,    21,    22,     4,     5,
       6,     7,   106,     0,   107,   108,   109,   110,   111,   112,
     113,    23,    24,    25,     0,     0,     0,     0,     0,     0,
       0,    26,    27,    18,    19,    20,    21,    22,     0,     0,
       0,    28,    29,    30,     0,    93,     0,     0,     0,   115,
       0,    23,    24,    25,     0,     0,     0,     0,     0,     0,
       0,    26,    27,     0,     0,     0,     0,     0,     0,     0,
       0,    28,    29,    30,     0,     0,     0,     0,     0,   161,
      18,    19,    20,    21,    22,     4,     5,     6,     7,    55,
      56,    57,    58,    59,     0,     0,     0,     0,    23,    24,
      25,    60,    61,    62,    63,    64,    65,    66,    26,    27,
      55,    56,    57,    58,    59,     0,     0,     0,    28,    29,
      30,     0,    60,    61,    62,    63,    64,    65,    66,    67,
      55,    56,    57,    58,    59,     0,     0,     0,    68,     0,
       0,     0,    60,    61,    62,    63,    64,    65,    66,    67,
      55,    56,    57,    58,    59,     0,     0,     0,   140,     0,
       0,     0,    60,    61,    62,    63,    64,    65,    66,    67,
      55,    56,    57,    58,    59,     0,     0,     0,   147,     0,
       0,     0,    60,    61,    62,    63,    64,    65,    66,    67,
      55,    56,    57,    58,    59,     0,     0,     0,   169,     0,
       0,     0,    60,    61,    62,    63,    64,    65,    66,    67,
      55,    56,    57,    58,    59,     0,     0,   100,     0,     0,
       0,     0,    60,    61,    62,    63,    64,    65,    66,    67,
       0,     0,     0,    78,    55,    56,    57,    58,    59,     0,
       0,     0,     0,     0,     0,     0,    60,    61,    62,    63,
      64,    65,    66,    67,     0,     0,     0,   151,    55,    56,
      57,    58,    59,     0,     0,     0,     0,     0,     0,     0,
      60,    61,    62,    63,    64,    65,    66,    67,     0,     0,
       0,   152,    55,    56,    57,    58,    59,     0,     0,     0,
       0,     0,     0,     0,    60,    61,    62,    63,    64,    65,
      66,    67,     0,     0,     0,   173,    55,    56,    57,    58,
      59,     0,     0,     0,     0,     0,     0,     0,    60,    61,
      62,    63,    64,    65,    66,    67,     0,     0,     0,   178,
      55,    56,    57,    58,    59,     0,     0,     0,     0,     0,
       0,     0,    60,    61,    62,    63,    64,    65,    66,    67,
      55,    56,    57,    58,    59,     0,     0,     0,     0,     0,
       0,     0,    60,    61,    62,    63,    64,    65
};

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-146)))

#define yytable_value_is_error(Yytable_value) \
  YYID (0)

static const yytype_int16 yycheck[] =
{
      14,   103,     2,    26,    26,     0,   151,   152,     7,    23,
      24,    26,    44,    44,    28,    15,    30,    44,    50,    50,
      43,   166,     3,    50,    47,    47,    49,    49,   173,     7,
      44,    45,    46,   178,    49,    21,    22,    23,    24,    25,
       7,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      64,    65,    66,    67,   156,   157,     7,     7,     3,     4,
       5,     6,     7,    26,    27,    28,    29,    30,    31,    32,
      46,    71,     7,    44,    50,    50,    21,    22,    23,    48,
      43,    23,    24,    25,    47,    99,    31,    32,    47,   103,
      45,   105,    44,    50,    48,   109,    41,    42,    43,    26,
      27,    28,    29,    30,    49,    21,    22,    23,    24,    25,
       3,     4,     5,     6,     7,    45,   130,   131,   132,    35,
      36,    37,    38,     8,     9,    10,    11,    49,    21,    22,
      23,    43,   132,    43,    43,    49,    43,    43,    31,    32,
     154,     7,   156,   157,     6,     6,   160,   161,    41,    42,
      43,    49,     7,    49,    26,   169,     3,     4,     5,     6,
       7,     8,     9,    10,    11,    12,    50,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    13,    49,    49,
      49,     2,     2,    71,    31,    32,    -1,    70,    -1,   100,
      -1,    -1,    -1,    -1,    41,    42,    43,    -1,    45,    46,
      -1,    -1,    49,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    12,    -1,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    32,     3,     4,     5,     6,     7,    -1,    -1,
      -1,    41,    42,    43,    -1,    45,    -1,    -1,    -1,    49,
      -1,    21,    22,    23,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    31,    32,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    41,    42,    43,    -1,    -1,    -1,    -1,    -1,    49,
       3,     4,     5,     6,     7,     8,     9,    10,    11,    21,
      22,    23,    24,    25,    -1,    -1,    -1,    -1,    21,    22,
      23,    33,    34,    35,    36,    37,    38,    39,    31,    32,
      21,    22,    23,    24,    25,    -1,    -1,    -1,    41,    42,
      43,    -1,    33,    34,    35,    36,    37,    38,    39,    40,
      21,    22,    23,    24,    25,    -1,    -1,    -1,    49,    -1,
      -1,    -1,    33,    34,    35,    36,    37,    38,    39,    40,
      21,    22,    23,    24,    25,    -1,    -1,    -1,    49,    -1,
      -1,    -1,    33,    34,    35,    36,    37,    38,    39,    40,
      21,    22,    23,    24,    25,    -1,    -1,    -1,    49,    -1,
      -1,    -1,    33,    34,    35,    36,    37,    38,    39,    40,
      21,    22,    23,    24,    25,    -1,    -1,    -1,    49,    -1,
      -1,    -1,    33,    34,    35,    36,    37,    38,    39,    40,
      21,    22,    23,    24,    25,    -1,    -1,    48,    -1,    -1,
      -1,    -1,    33,    34,    35,    36,    37,    38,    39,    40,
      -1,    -1,    -1,    44,    21,    22,    23,    24,    25,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    33,    34,    35,    36,
      37,    38,    39,    40,    -1,    -1,    -1,    44,    21,    22,
      23,    24,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33,    34,    35,    36,    37,    38,    39,    40,    -1,    -1,
      -1,    44,    21,    22,    23,    24,    25,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    33,    34,    35,    36,    37,    38,
      39,    40,    -1,    -1,    -1,    44,    21,    22,    23,    24,
      25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    33,    34,
      35,    36,    37,    38,    39,    40,    -1,    -1,    -1,    44,
      21,    22,    23,    24,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    34,    35,    36,    37,    38,    39,    40,
      21,    22,    23,    24,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    33,    34,    35,    36,    37,    38
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    57,    58,     0,     8,     9,    10,    11,    59,    60,
      61,    62,    63,     7,    26,    43,    47,    49,     3,     4,
       5,     6,     7,    21,    22,    23,    31,    32,    41,    42,
      43,    77,    60,    64,    65,    66,     3,    26,    27,    28,
      29,    30,    31,    32,    43,    47,    78,    77,    77,     7,
       7,     7,    77,     7,    77,    21,    22,    23,    24,    25,
      33,    34,    35,    36,    37,    38,    39,    40,    49,     7,
      44,    50,    48,    77,    79,    80,    77,    77,    44,    77,
      77,    77,    77,    77,    77,    77,    77,    77,    77,    77,
      77,    77,    47,    45,    67,    66,    26,    49,    44,    50,
      48,    48,    68,    45,    77,    78,    12,    14,    15,    16,
      17,    18,    19,    20,    46,    49,    60,    61,    62,    67,
      69,    70,    71,    72,    74,    75,    76,    77,    80,    77,
      43,    43,    43,    49,    77,    49,    49,    43,    43,     7,
      49,    46,    77,    77,    60,    73,    77,    49,     6,     6,
      49,    44,    44,     7,    49,    44,    50,    50,    69,    69,
      26,    49,    77,    49,    80,    80,    13,    77,    77,    49,
      44,    44,    69,    44,    77,    49,    49,    69,    44,    69
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  However,
   YYFAIL appears to be in use.  Nevertheless, it is formally deprecated
   in Bison 2.4.2's NEWS entry, where a plan to phase it out is
   discussed.  */

#define YYFAIL		goto yyerrlab
#if defined YYFAIL
  /* This is here to suppress warnings from the GCC cpp's
     -Wunused-macros.  Normally we don't worry about that warning, but
     some users do, and we want to make it easy for users to remove
     YYFAIL uses, which will produce warnings from Bison 2.5.  */
#endif

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))

/* Error token number */
#define YYTERROR	1
#define YYERRCODE	256


/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */
#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
        break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULL, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULL;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - Assume YYFAIL is not used.  It's too flawed to consider.  See
       <http://lists.gnu.org/archive/html/bison-patches/2009-12/msg00024.html>
       for details.  YYERROR is fine as it does not invoke this
       function.
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULL, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
        break;
    }
}




/* The lookahead symbol.  */
int yychar;


#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval YY_INITIAL_VALUE(yyval_default);

/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
/* Line 1792 of yacc.c  */
#line 111 "src/parser.y"
    {
            (yyval.node) = ast_make_program(1);
            for (int i = 0; i < (yyvsp[(1) - (1)].nodelist).count; i++)
                ast_add_child((yyval.node), (yyvsp[(1) - (1)].nodelist).items[i]);
            free((yyvsp[(1) - (1)].nodelist).items);
            ast_root = (yyval.node);
        }
    break;

  case 3:
/* Line 1792 of yacc.c  */
#line 122 "src/parser.y"
    { (yyval.nodelist).items = NULL; (yyval.nodelist).count = 0; (yyval.nodelist).cap = 0; }
    break;

  case 4:
/* Line 1792 of yacc.c  */
#line 124 "src/parser.y"
    {
            (yyval.nodelist) = (yyvsp[(1) - (2)].nodelist);
            if ((yyvsp[(2) - (2)].node)) {
                if ((yyval.nodelist).count >= (yyval.nodelist).cap) {
                    (yyval.nodelist).cap = (yyval.nodelist).cap ? (yyval.nodelist).cap * 2 : 8;
                    (yyval.nodelist).items = realloc((yyval.nodelist).items, (yyval.nodelist).cap * sizeof(ASTNode *));
                }
                (yyval.nodelist).items[(yyval.nodelist).count++] = (yyvsp[(2) - (2)].node);
            }
        }
    break;

  case 5:
/* Line 1792 of yacc.c  */
#line 137 "src/parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 6:
/* Line 1792 of yacc.c  */
#line 138 "src/parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 7:
/* Line 1792 of yacc.c  */
#line 139 "src/parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 8:
/* Line 1792 of yacc.c  */
#line 147 "src/parser.y"
    { (yyval.node) = ast_make_var_decl(TYPE_INT,   "__type__", NULL, line_num); }
    break;

  case 9:
/* Line 1792 of yacc.c  */
#line 148 "src/parser.y"
    { (yyval.node) = ast_make_var_decl(TYPE_FLOAT, "__type__", NULL, line_num); }
    break;

  case 10:
/* Line 1792 of yacc.c  */
#line 149 "src/parser.y"
    { (yyval.node) = ast_make_var_decl(TYPE_CHAR,  "__type__", NULL, line_num); }
    break;

  case 11:
/* Line 1792 of yacc.c  */
#line 150 "src/parser.y"
    { (yyval.node) = ast_make_var_decl(TYPE_VOID,  "__type__", NULL, line_num); }
    break;

  case 12:
/* Line 1792 of yacc.c  */
#line 159 "src/parser.y"
    {
(yyval.node) = ast_make_var_decl((yyvsp[(1) - (3)].node)->data_type, (yyvsp[(2) - (3)].sval), NULL, line_num);
            free((yyvsp[(2) - (3)].sval));
            ast_free((yyvsp[(1) - (3)].node));
        }
    break;

  case 13:
/* Line 1792 of yacc.c  */
#line 165 "src/parser.y"
    {
            (yyval.node) = ast_make_var_decl((yyvsp[(1) - (5)].node)->data_type, (yyvsp[(2) - (5)].sval), (yyvsp[(4) - (5)].node), line_num);
            free((yyvsp[(2) - (5)].sval));
            ast_free((yyvsp[(1) - (5)].node));
        }
    break;

  case 14:
/* Line 1792 of yacc.c  */
#line 174 "src/parser.y"
    {
            (yyval.node) = ast_make_array_decl((yyvsp[(1) - (6)].node)->data_type, (yyvsp[(2) - (6)].sval), (yyvsp[(4) - (6)].ival), line_num);
            free((yyvsp[(2) - (6)].sval));
            ast_free((yyvsp[(1) - (6)].node));
        }
    break;

  case 15:
/* Line 1792 of yacc.c  */
#line 181 "src/parser.y"
    {
            (yyval.node) = ast_make_array_decl((yyvsp[(1) - (10)].node)->data_type, (yyvsp[(2) - (10)].sval), (yyvsp[(4) - (10)].ival), line_num);
            /* Store initializer list as children */
            for (int i = 0; i < (yyvsp[(8) - (10)].nodelist).count; i++)
                ast_add_child((yyval.node), (yyvsp[(8) - (10)].nodelist).items[i]);
            free((yyvsp[(8) - (10)].nodelist).items);
            free((yyvsp[(2) - (10)].sval));
            ast_free((yyvsp[(1) - (10)].node));
        }
    break;

  case 16:
/* Line 1792 of yacc.c  */
#line 198 "src/parser.y"
    {
            (yyval.node) = ast_make_func_decl((yyvsp[(1) - (6)].node)->data_type, (yyvsp[(2) - (6)].sval), line_num);
            /* Add params as children first */
            for (int i = 0; i < (yyvsp[(4) - (6)].nodelist).count; i++)
                ast_add_child((yyval.node), (yyvsp[(4) - (6)].nodelist).items[i]);
            free((yyvsp[(4) - (6)].nodelist).items);
            /* Add body */
            (yyval.node)->body = (yyvsp[(6) - (6)].node);
            free((yyvsp[(2) - (6)].sval));
            ast_free((yyvsp[(1) - (6)].node));
        }
    break;

  case 17:
/* Line 1792 of yacc.c  */
#line 213 "src/parser.y"
    { (yyval.nodelist).items = NULL; (yyval.nodelist).count = 0; (yyval.nodelist).cap = 0; }
    break;

  case 18:
/* Line 1792 of yacc.c  */
#line 215 "src/parser.y"
    { (yyval.nodelist) = (yyvsp[(1) - (1)].nodelist); }
    break;

  case 19:
/* Line 1792 of yacc.c  */
#line 220 "src/parser.y"
    {
            (yyval.nodelist).cap   = 4;
            (yyval.nodelist).items = malloc((yyval.nodelist).cap * sizeof(ASTNode *));
            (yyval.nodelist).count = 0;
            (yyval.nodelist).items[(yyval.nodelist).count++] = (yyvsp[(1) - (1)].node);
        }
    break;

  case 20:
/* Line 1792 of yacc.c  */
#line 227 "src/parser.y"
    {
            (yyval.nodelist) = (yyvsp[(1) - (3)].nodelist);
            if ((yyval.nodelist).count >= (yyval.nodelist).cap) {
                (yyval.nodelist).cap *= 2;
                (yyval.nodelist).items = realloc((yyval.nodelist).items, (yyval.nodelist).cap * sizeof(ASTNode *));
            }
            (yyval.nodelist).items[(yyval.nodelist).count++] = (yyvsp[(3) - (3)].node);
        }
    break;

  case 21:
/* Line 1792 of yacc.c  */
#line 239 "src/parser.y"
    {
            (yyval.node) = ast_make_param((yyvsp[(1) - (2)].node)->data_type, (yyvsp[(2) - (2)].sval), line_num);
            free((yyvsp[(2) - (2)].sval));
            ast_free((yyvsp[(1) - (2)].node));
        }
    break;

  case 22:
/* Line 1792 of yacc.c  */
#line 245 "src/parser.y"
    {
            (yyval.node) = ast_make_param((yyvsp[(1) - (4)].node)->data_type, (yyvsp[(2) - (4)].sval), line_num);
            (yyval.node)->lit.ival = -1; /* marks array param */
            free((yyvsp[(2) - (4)].sval));
            ast_free((yyvsp[(1) - (4)].node));
        }
    break;

  case 23:
/* Line 1792 of yacc.c  */
#line 259 "src/parser.y"
    {
            (yyval.node) = ast_make_block(line_num);
            for (int i = 0; i < (yyvsp[(2) - (3)].nodelist).count; i++)
                ast_add_child((yyval.node), (yyvsp[(2) - (3)].nodelist).items[i]);
            free((yyvsp[(2) - (3)].nodelist).items);
        }
    break;

  case 24:
/* Line 1792 of yacc.c  */
#line 269 "src/parser.y"
    { (yyval.nodelist).items = NULL; (yyval.nodelist).count = 0; (yyval.nodelist).cap = 0; }
    break;

  case 25:
/* Line 1792 of yacc.c  */
#line 271 "src/parser.y"
    {
            (yyval.nodelist) = (yyvsp[(1) - (2)].nodelist);
            if ((yyvsp[(2) - (2)].node)) {
                if ((yyval.nodelist).count >= (yyval.nodelist).cap) {
                    (yyval.nodelist).cap = (yyval.nodelist).cap ? (yyval.nodelist).cap * 2 : 8;
                    (yyval.nodelist).items = realloc((yyval.nodelist).items, (yyval.nodelist).cap * sizeof(ASTNode *));
                }
                (yyval.nodelist).items[(yyval.nodelist).count++] = (yyvsp[(2) - (2)].node);
            }
        }
    break;

  case 26:
/* Line 1792 of yacc.c  */
#line 284 "src/parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 27:
/* Line 1792 of yacc.c  */
#line 285 "src/parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 28:
/* Line 1792 of yacc.c  */
#line 286 "src/parser.y"
    { (yyval.node) = ast_make_expr_stmt((yyvsp[(1) - (2)].node), line_num); }
    break;

  case 29:
/* Line 1792 of yacc.c  */
#line 287 "src/parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 30:
/* Line 1792 of yacc.c  */
#line 288 "src/parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 31:
/* Line 1792 of yacc.c  */
#line 289 "src/parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 32:
/* Line 1792 of yacc.c  */
#line 290 "src/parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 33:
/* Line 1792 of yacc.c  */
#line 291 "src/parser.y"
    { (yyval.node) = ast_make_break(line_num); }
    break;

  case 34:
/* Line 1792 of yacc.c  */
#line 292 "src/parser.y"
    { (yyval.node) = ast_make_continue(line_num); }
    break;

  case 35:
/* Line 1792 of yacc.c  */
#line 293 "src/parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 36:
/* Line 1792 of yacc.c  */
#line 294 "src/parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 37:
/* Line 1792 of yacc.c  */
#line 295 "src/parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); }
    break;

  case 38:
/* Line 1792 of yacc.c  */
#line 296 "src/parser.y"
    { (yyval.node) = NULL; }
    break;

  case 39:
/* Line 1792 of yacc.c  */
#line 301 "src/parser.y"
    { (yyval.node) = ast_make_if((yyvsp[(3) - (5)].node), (yyvsp[(5) - (5)].node), NULL, line_num); }
    break;

  case 40:
/* Line 1792 of yacc.c  */
#line 303 "src/parser.y"
    { (yyval.node) = ast_make_if((yyvsp[(3) - (7)].node), (yyvsp[(5) - (7)].node), (yyvsp[(7) - (7)].node), line_num); }
    break;

  case 41:
/* Line 1792 of yacc.c  */
#line 308 "src/parser.y"
    { (yyval.node) = ast_make_while((yyvsp[(3) - (5)].node), (yyvsp[(5) - (5)].node), line_num); }
    break;

  case 42:
/* Line 1792 of yacc.c  */
#line 313 "src/parser.y"
    { (yyval.node) = ast_make_for((yyvsp[(3) - (9)].node), (yyvsp[(5) - (9)].node), (yyvsp[(7) - (9)].node), (yyvsp[(9) - (9)].node), line_num); }
    break;

  case 43:
/* Line 1792 of yacc.c  */
#line 315 "src/parser.y"
    { (yyval.node) = ast_make_for((yyvsp[(3) - (8)].node), NULL, (yyvsp[(6) - (8)].node), (yyvsp[(8) - (8)].node), line_num); }
    break;

  case 44:
/* Line 1792 of yacc.c  */
#line 319 "src/parser.y"
    { (yyval.node) = ast_make_expr_stmt((yyvsp[(1) - (1)].node), line_num); }
    break;

  case 45:
/* Line 1792 of yacc.c  */
#line 321 "src/parser.y"
    {
            (yyval.node) = ast_make_var_decl((yyvsp[(1) - (4)].node)->data_type, (yyvsp[(2) - (4)].sval), (yyvsp[(4) - (4)].node), line_num);
            free((yyvsp[(2) - (4)].sval)); ast_free((yyvsp[(1) - (4)].node));
        }
    break;

  case 46:
/* Line 1792 of yacc.c  */
#line 325 "src/parser.y"
    { (yyval.node) = NULL; }
    break;

  case 47:
/* Line 1792 of yacc.c  */
#line 329 "src/parser.y"
    { (yyval.node) = ast_make_return((yyvsp[(2) - (3)].node), line_num); }
    break;

  case 48:
/* Line 1792 of yacc.c  */
#line 330 "src/parser.y"
    { (yyval.node) = ast_make_return(NULL, line_num); }
    break;

  case 49:
/* Line 1792 of yacc.c  */
#line 335 "src/parser.y"
    {
            (yyval.node) = ast_make_printf((yyvsp[(3) - (5)].sval), line_num);
            free((yyvsp[(3) - (5)].sval));
        }
    break;

  case 50:
/* Line 1792 of yacc.c  */
#line 340 "src/parser.y"
    {
            (yyval.node) = ast_make_printf((yyvsp[(3) - (7)].sval), line_num);
            for (int i = 0; i < (yyvsp[(5) - (7)].nodelist).count; i++)
                ast_add_child((yyval.node), (yyvsp[(5) - (7)].nodelist).items[i]);
            free((yyvsp[(5) - (7)].nodelist).items);
            free((yyvsp[(3) - (7)].sval));
        }
    break;

  case 51:
/* Line 1792 of yacc.c  */
#line 351 "src/parser.y"
    {
            (yyval.node) = ast_make_scanf((yyvsp[(3) - (7)].sval), line_num);
            for (int i = 0; i < (yyvsp[(5) - (7)].nodelist).count; i++)
                ast_add_child((yyval.node), (yyvsp[(5) - (7)].nodelist).items[i]);
            free((yyvsp[(5) - (7)].nodelist).items);
            free((yyvsp[(3) - (7)].sval));
        }
    break;

  case 52:
/* Line 1792 of yacc.c  */
#line 365 "src/parser.y"
    { (yyval.node) = ast_make_int((yyvsp[(1) - (1)].ival), line_num); }
    break;

  case 53:
/* Line 1792 of yacc.c  */
#line 366 "src/parser.y"
    { (yyval.node) = ast_make_float((yyvsp[(1) - (1)].fval), line_num); }
    break;

  case 54:
/* Line 1792 of yacc.c  */
#line 367 "src/parser.y"
    { (yyval.node) = ast_make_char((yyvsp[(1) - (1)].ival), line_num); }
    break;

  case 55:
/* Line 1792 of yacc.c  */
#line 368 "src/parser.y"
    { (yyval.node) = ast_make_string((yyvsp[(1) - (1)].sval), line_num); free((yyvsp[(1) - (1)].sval)); }
    break;

  case 56:
/* Line 1792 of yacc.c  */
#line 369 "src/parser.y"
    { (yyval.node) = ast_make_ident((yyvsp[(1) - (1)].sval), line_num); free((yyvsp[(1) - (1)].sval)); }
    break;

  case 57:
/* Line 1792 of yacc.c  */
#line 373 "src/parser.y"
    {
            (yyval.node) = ast_make_array_access((yyvsp[(1) - (4)].sval), (yyvsp[(3) - (4)].node), line_num);
            free((yyvsp[(1) - (4)].sval));
        }
    break;

  case 58:
/* Line 1792 of yacc.c  */
#line 380 "src/parser.y"
    {
            (yyval.node) = ast_make_call((yyvsp[(1) - (4)].sval), line_num);
            for (int i = 0; i < (yyvsp[(3) - (4)].nodelist).count; i++)
                ast_add_child((yyval.node), (yyvsp[(3) - (4)].nodelist).items[i]);
            free((yyvsp[(3) - (4)].nodelist).items);
            free((yyvsp[(1) - (4)].sval));
        }
    break;

  case 59:
/* Line 1792 of yacc.c  */
#line 390 "src/parser.y"
    {
            ASTNode *lhs = ast_make_ident((yyvsp[(1) - (3)].sval), line_num);
            (yyval.node) = ast_make_assign((yyvsp[(2) - (3)].sval), lhs, (yyvsp[(3) - (3)].node), line_num);
            free((yyvsp[(1) - (3)].sval)); free((yyvsp[(2) - (3)].sval));
        }
    break;

  case 60:
/* Line 1792 of yacc.c  */
#line 396 "src/parser.y"
    {
            ASTNode *lhs = ast_make_array_access((yyvsp[(1) - (6)].sval), (yyvsp[(3) - (6)].node), line_num);
            (yyval.node) = ast_make_assign((yyvsp[(5) - (6)].sval), lhs, (yyvsp[(6) - (6)].node), line_num);
            free((yyvsp[(1) - (6)].sval)); free((yyvsp[(5) - (6)].sval));
        }
    break;

  case 61:
/* Line 1792 of yacc.c  */
#line 403 "src/parser.y"
    { (yyval.node) = ast_make_binop("+",  (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node), line_num); }
    break;

  case 62:
/* Line 1792 of yacc.c  */
#line 404 "src/parser.y"
    { (yyval.node) = ast_make_binop("-",  (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node), line_num); }
    break;

  case 63:
/* Line 1792 of yacc.c  */
#line 405 "src/parser.y"
    { (yyval.node) = ast_make_binop("*",  (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node), line_num); }
    break;

  case 64:
/* Line 1792 of yacc.c  */
#line 406 "src/parser.y"
    { (yyval.node) = ast_make_binop("/",  (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node), line_num); }
    break;

  case 65:
/* Line 1792 of yacc.c  */
#line 407 "src/parser.y"
    { (yyval.node) = ast_make_binop("%",  (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node), line_num); }
    break;

  case 66:
/* Line 1792 of yacc.c  */
#line 408 "src/parser.y"
    { (yyval.node) = ast_make_binop("==", (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node), line_num); }
    break;

  case 67:
/* Line 1792 of yacc.c  */
#line 409 "src/parser.y"
    { (yyval.node) = ast_make_binop("!=", (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node), line_num); }
    break;

  case 68:
/* Line 1792 of yacc.c  */
#line 410 "src/parser.y"
    { (yyval.node) = ast_make_binop("<",  (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node), line_num); }
    break;

  case 69:
/* Line 1792 of yacc.c  */
#line 411 "src/parser.y"
    { (yyval.node) = ast_make_binop("<=", (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node), line_num); }
    break;

  case 70:
/* Line 1792 of yacc.c  */
#line 412 "src/parser.y"
    { (yyval.node) = ast_make_binop(">",  (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node), line_num); }
    break;

  case 71:
/* Line 1792 of yacc.c  */
#line 413 "src/parser.y"
    { (yyval.node) = ast_make_binop(">=", (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node), line_num); }
    break;

  case 72:
/* Line 1792 of yacc.c  */
#line 414 "src/parser.y"
    { (yyval.node) = ast_make_binop("&&", (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node), line_num); }
    break;

  case 73:
/* Line 1792 of yacc.c  */
#line 415 "src/parser.y"
    { (yyval.node) = ast_make_binop("||", (yyvsp[(1) - (3)].node), (yyvsp[(3) - (3)].node), line_num); }
    break;

  case 74:
/* Line 1792 of yacc.c  */
#line 418 "src/parser.y"
    { (yyval.node) = ast_make_unop("!",  (yyvsp[(2) - (2)].node), line_num); }
    break;

  case 75:
/* Line 1792 of yacc.c  */
#line 419 "src/parser.y"
    { (yyval.node) = ast_make_unop("-", (yyvsp[(2) - (2)].node), line_num); }
    break;

  case 76:
/* Line 1792 of yacc.c  */
#line 420 "src/parser.y"
    { (yyval.node) = (yyvsp[(2) - (2)].node); }
    break;

  case 77:
/* Line 1792 of yacc.c  */
#line 422 "src/parser.y"
    {
            ASTNode *inner = ast_make_ident((yyvsp[(2) - (2)].sval), line_num);
            (yyval.node) = ast_make_unop("&", inner, line_num);
            free((yyvsp[(2) - (2)].sval));
        }
    break;

  case 78:
/* Line 1792 of yacc.c  */
#line 428 "src/parser.y"
    {
            ASTNode *inner = ast_make_ident((yyvsp[(2) - (2)].sval), line_num);
            (yyval.node) = ast_make_unop("*", inner, line_num);
            free((yyvsp[(2) - (2)].sval));
        }
    break;

  case 79:
/* Line 1792 of yacc.c  */
#line 436 "src/parser.y"
    {
            ASTNode *id = ast_make_ident((yyvsp[(2) - (2)].sval), line_num);
            (yyval.node) = ast_make_unop("++pre", id, line_num);
            free((yyvsp[(2) - (2)].sval));
        }
    break;

  case 80:
/* Line 1792 of yacc.c  */
#line 442 "src/parser.y"
    {
            ASTNode *id = ast_make_ident((yyvsp[(2) - (2)].sval), line_num);
            (yyval.node) = ast_make_unop("--pre", id, line_num);
            free((yyvsp[(2) - (2)].sval));
        }
    break;

  case 81:
/* Line 1792 of yacc.c  */
#line 448 "src/parser.y"
    {
            ASTNode *id = ast_make_ident((yyvsp[(1) - (2)].sval), line_num);
            (yyval.node) = ast_make_unop("++post", id, line_num);
            free((yyvsp[(1) - (2)].sval));
        }
    break;

  case 82:
/* Line 1792 of yacc.c  */
#line 454 "src/parser.y"
    {
            ASTNode *id = ast_make_ident((yyvsp[(1) - (2)].sval), line_num);
            (yyval.node) = ast_make_unop("--post", id, line_num);
            free((yyvsp[(1) - (2)].sval));
        }
    break;

  case 83:
/* Line 1792 of yacc.c  */
#line 461 "src/parser.y"
    { (yyval.node) = (yyvsp[(2) - (3)].node); }
    break;

  case 84:
/* Line 1792 of yacc.c  */
#line 465 "src/parser.y"
    { (yyval.sval) = strdup("=");  }
    break;

  case 85:
/* Line 1792 of yacc.c  */
#line 466 "src/parser.y"
    { (yyval.sval) = strdup("+="); }
    break;

  case 86:
/* Line 1792 of yacc.c  */
#line 467 "src/parser.y"
    { (yyval.sval) = strdup("-="); }
    break;

  case 87:
/* Line 1792 of yacc.c  */
#line 468 "src/parser.y"
    { (yyval.sval) = strdup("*="); }
    break;

  case 88:
/* Line 1792 of yacc.c  */
#line 469 "src/parser.y"
    { (yyval.sval) = strdup("/="); }
    break;

  case 89:
/* Line 1792 of yacc.c  */
#line 473 "src/parser.y"
    { (yyval.nodelist).items = NULL; (yyval.nodelist).count = 0; (yyval.nodelist).cap = 0; }
    break;

  case 90:
/* Line 1792 of yacc.c  */
#line 474 "src/parser.y"
    { (yyval.nodelist) = (yyvsp[(1) - (1)].nodelist); }
    break;

  case 91:
/* Line 1792 of yacc.c  */
#line 479 "src/parser.y"
    {
            (yyval.nodelist).cap   = 4;
            (yyval.nodelist).items = malloc((yyval.nodelist).cap * sizeof(ASTNode *));
            (yyval.nodelist).count = 0;
            (yyval.nodelist).items[(yyval.nodelist).count++] = (yyvsp[(1) - (1)].node);
        }
    break;

  case 92:
/* Line 1792 of yacc.c  */
#line 486 "src/parser.y"
    {
            (yyval.nodelist) = (yyvsp[(1) - (3)].nodelist);
            if ((yyval.nodelist).count >= (yyval.nodelist).cap) {
                (yyval.nodelist).cap *= 2;
                (yyval.nodelist).items = realloc((yyval.nodelist).items, (yyval.nodelist).cap * sizeof(ASTNode *));
            }
            (yyval.nodelist).items[(yyval.nodelist).count++] = (yyvsp[(3) - (3)].node);
        }
    break;


/* Line 1792 of yacc.c  */
#line 2378 "src/parser.tab.c"
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}


/* Line 2055 of yacc.c  */
#line 496 "src/parser.y"


void yyerror(const char *msg) {
    fprintf(stderr, "[Parse Error] Line %d: %s\n", line_num, msg);
}
