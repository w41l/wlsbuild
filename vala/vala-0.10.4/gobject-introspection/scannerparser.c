/* A Bison parser, made by GNU Bison 2.4.3.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006,
   2009, 2010 Free Software Foundation, Inc.
   
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
#define YYBISON_VERSION "2.4.3"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Copy the first part of user declarations.  */

/* Line 189 of yacc.c  */
#line 29 "scannerparser.y"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "scanner.h"

extern FILE *yyin;
extern int lineno;
extern char *yytext;

extern int yylex (GIGenerator *igenerator);
static void yyerror(GIGenerator *igenerator, const char *s);
 
static int last_enum_value = -1;
static GHashTable *const_table = NULL;

CSymbol *
csymbol_new (CSymbolType type)
{
  CSymbol *s = g_new0 (CSymbol, 1);
  s->type = type;
  return s;
}

static void
ctype_free (CType * type)
{
  g_free (type);
  g_free (type->name);
  g_list_foreach (type->child_list, (GFunc)ctype_free, NULL);
  g_list_free (type->child_list);
}

void
csymbol_free (CSymbol * symbol)
{
  g_free (symbol->ident);
  ctype_free (symbol->base_type);
  g_free (symbol->const_string);
  g_free (symbol);
  g_slist_foreach (symbol->directives, (GFunc)cdirective_free, NULL);
  g_slist_free (symbol->directives);
}
 
gboolean
csymbol_get_const_boolean (CSymbol * symbol)
{
  return (symbol->const_int_set && symbol->const_int) || symbol->const_string;
}

CType *
ctype_new (CTypeType type)
{
  CType *t = g_new0 (CType, 1);
  t->type = type;
  return t;
}

CType *
ctype_copy (CType * type)
{
  return g_memdup (type, sizeof (CType));
}

CType *
cbasic_type_new (const char *name)
{
  CType *basic_type = ctype_new (CTYPE_BASIC_TYPE);
  basic_type->name = g_strdup (name);
  return basic_type;
}

CType *
ctypedef_new (const char *name)
{
  CType *typedef_ = ctype_new (CTYPE_TYPEDEF);
  typedef_->name = g_strdup (name);
  return typedef_;
}

CType *
cstruct_new (const char *name)
{
  CType *struct_ = ctype_new (CTYPE_STRUCT);
  struct_->name = g_strdup (name);
  return struct_;
}

CType *
cunion_new (const char *name)
{
  CType *union_ = ctype_new (CTYPE_UNION);
  union_->name = g_strdup (name);
  return union_;
}

CType *
cenum_new (const char *name)
{
  CType *enum_ = ctype_new (CTYPE_ENUM);
  enum_->name = g_strdup (name);
  return enum_;
}

CType *
cpointer_new (CType * base_type)
{
  CType *pointer = ctype_new (CTYPE_POINTER);
  pointer->base_type = ctype_copy (base_type);
  return pointer;
}

CType *
carray_new (void)
{
  CType *array = ctype_new (CTYPE_ARRAY);
  return array;
}

CType *
cfunction_new (void)
{
  CType *func = ctype_new (CTYPE_FUNCTION);
  return func;
}

/* use specified type as base type of symbol */
static void
csymbol_merge_type (CSymbol *symbol, CType *type)
{
  CType **foundation_type = &(symbol->base_type);
  while (*foundation_type != NULL) {
    foundation_type = &((*foundation_type)->base_type);
  }
  *foundation_type = ctype_copy (type);
}

CDirective *
cdirective_new (const gchar *name,
		const gchar *value)
{
  CDirective *directive;
    
  directive = g_slice_new (CDirective);
  directive->name = g_strdup (name);
  directive->value = g_strdup (value);
  return directive;
}

void
cdirective_free (CDirective *directive)
{
  g_free (directive->name);
  g_free (directive->value);
  g_slice_free (CDirective, directive);
}



/* Line 189 of yacc.c  */
#line 233 "scannerparser.c"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 1
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     IDENTIFIER = 258,
     TYPEDEF_NAME = 259,
     INTEGER = 260,
     FLOATING = 261,
     CHARACTER = 262,
     STRING = 263,
     ELLIPSIS = 264,
     ADDEQ = 265,
     SUBEQ = 266,
     MULEQ = 267,
     DIVEQ = 268,
     MODEQ = 269,
     XOREQ = 270,
     ANDEQ = 271,
     OREQ = 272,
     SL = 273,
     SR = 274,
     SLEQ = 275,
     SREQ = 276,
     EQ = 277,
     NOTEQ = 278,
     LTEQ = 279,
     GTEQ = 280,
     ANDAND = 281,
     OROR = 282,
     PLUSPLUS = 283,
     MINUSMINUS = 284,
     ARROW = 285,
     AUTO = 286,
     BOOL = 287,
     BREAK = 288,
     CASE = 289,
     CHAR = 290,
     CONST = 291,
     CONTINUE = 292,
     DEFAULT = 293,
     DO = 294,
     DOUBLE = 295,
     ELSE = 296,
     ENUM = 297,
     EXTERN = 298,
     FLOAT = 299,
     FOR = 300,
     GOTO = 301,
     IF = 302,
     INLINE = 303,
     INT = 304,
     LONG = 305,
     REGISTER = 306,
     RESTRICT = 307,
     RETURN = 308,
     SHORT = 309,
     SIGNED = 310,
     SIZEOF = 311,
     STATIC = 312,
     STRUCT = 313,
     SWITCH = 314,
     TYPEDEF = 315,
     UNION = 316,
     UNSIGNED = 317,
     VOID = 318,
     VOLATILE = 319,
     WHILE = 320,
     FUNCTION_MACRO = 321,
     OBJECT_MACRO = 322
   };
#endif
/* Tokens.  */
#define IDENTIFIER 258
#define TYPEDEF_NAME 259
#define INTEGER 260
#define FLOATING 261
#define CHARACTER 262
#define STRING 263
#define ELLIPSIS 264
#define ADDEQ 265
#define SUBEQ 266
#define MULEQ 267
#define DIVEQ 268
#define MODEQ 269
#define XOREQ 270
#define ANDEQ 271
#define OREQ 272
#define SL 273
#define SR 274
#define SLEQ 275
#define SREQ 276
#define EQ 277
#define NOTEQ 278
#define LTEQ 279
#define GTEQ 280
#define ANDAND 281
#define OROR 282
#define PLUSPLUS 283
#define MINUSMINUS 284
#define ARROW 285
#define AUTO 286
#define BOOL 287
#define BREAK 288
#define CASE 289
#define CHAR 290
#define CONST 291
#define CONTINUE 292
#define DEFAULT 293
#define DO 294
#define DOUBLE 295
#define ELSE 296
#define ENUM 297
#define EXTERN 298
#define FLOAT 299
#define FOR 300
#define GOTO 301
#define IF 302
#define INLINE 303
#define INT 304
#define LONG 305
#define REGISTER 306
#define RESTRICT 307
#define RETURN 308
#define SHORT 309
#define SIGNED 310
#define SIZEOF 311
#define STATIC 312
#define STRUCT 313
#define SWITCH 314
#define TYPEDEF 315
#define UNION 316
#define UNSIGNED 317
#define VOID 318
#define VOLATILE 319
#define WHILE 320
#define FUNCTION_MACRO 321
#define OBJECT_MACRO 322




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 214 of yacc.c  */
#line 190 "scannerparser.y"

  char *str;
  GList *list;
  CSymbol *symbol;
  CType *ctype;
  StorageClassSpecifier storage_class_specifier;
  TypeQualifier type_qualifier;
  FunctionSpecifier function_specifier;
  UnaryOperator unary_operator;



/* Line 214 of yacc.c  */
#line 416 "scannerparser.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 428 "scannerparser.c"

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
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
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
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
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
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
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

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

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

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  65
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1815

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  92
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  75
/* YYNRULES -- Number of rules.  */
#define YYNRULES  240
/* YYNRULES -- Number of states.  */
#define YYNSTATES  396

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   322

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    79,     2,     2,     2,    81,    74,     2,
      68,    69,    75,    76,    73,    77,    72,    80,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    87,    89,
      82,    88,    83,    86,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    70,     2,    71,    84,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    90,    85,    91,    78,     2,     2,     2,
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
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     5,     7,     9,    11,    13,    17,    19,
      22,    24,    26,    28,    30,    35,    40,    44,    48,    52,
      55,    58,    60,    64,    66,    69,    72,    75,    78,    83,
      85,    87,    89,    91,    93,    95,    97,   102,   104,   108,
     112,   116,   118,   122,   126,   128,   132,   136,   138,   142,
     146,   150,   154,   156,   160,   164,   166,   170,   172,   176,
     178,   182,   184,   188,   190,   194,   196,   202,   204,   208,
     210,   212,   214,   216,   218,   220,   222,   224,   226,   228,
     230,   232,   236,   238,   242,   245,   248,   250,   253,   255,
     258,   260,   263,   265,   267,   271,   273,   277,   279,   281,
     283,   285,   287,   289,   291,   293,   295,   297,   299,   301,
     303,   305,   307,   309,   311,   313,   319,   324,   327,   329,
     331,   333,   336,   340,   343,   345,   348,   350,   352,   356,
     357,   359,   362,   366,   372,   377,   384,   390,   393,   395,
     399,   401,   405,   407,   409,   411,   413,   416,   418,   420,
     424,   429,   433,   438,   443,   447,   450,   452,   456,   459,
     461,   464,   466,   470,   472,   476,   479,   482,   484,   486,
     490,   492,   495,   497,   499,   502,   506,   509,   513,   517,
     522,   525,   529,   533,   538,   540,   542,   546,   551,   553,
     557,   559,   561,   563,   565,   567,   569,   573,   578,   582,
     585,   589,   591,   594,   596,   598,   600,   603,   609,   617,
     623,   629,   637,   644,   652,   660,   669,   677,   686,   695,
     705,   709,   712,   715,   718,   722,   724,   727,   729,   731,
     733,   738,   742,   744,   747,   749,   751,   756,   759,   761,
     763
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     158,     0,    -1,    95,    -1,     5,    -1,     7,    -1,     6,
      -1,    94,    -1,    68,   115,    69,    -1,     8,    -1,    94,
       8,    -1,     3,    -1,    95,    -1,   146,    -1,    93,    -1,
      97,    70,   115,    71,    -1,    97,    68,    98,    69,    -1,
      97,    68,    69,    -1,    97,    72,    96,    -1,    97,    30,
      96,    -1,    97,    28,    -1,    97,    29,    -1,   113,    -1,
      98,    73,   113,    -1,    97,    -1,    28,    99,    -1,    29,
      99,    -1,   100,   101,    -1,    56,    99,    -1,    56,    68,
     143,    69,    -1,    74,    -1,    75,    -1,    76,    -1,    77,
      -1,    78,    -1,    79,    -1,    99,    -1,    68,   143,    69,
     101,    -1,   101,    -1,   102,    75,   101,    -1,   102,    80,
     101,    -1,   102,    81,   101,    -1,   102,    -1,   103,    76,
     102,    -1,   103,    77,   102,    -1,   103,    -1,   104,    18,
     103,    -1,   104,    19,   103,    -1,   104,    -1,   105,    82,
     104,    -1,   105,    83,   104,    -1,   105,    24,   104,    -1,
     105,    25,   104,    -1,   105,    -1,   106,    22,   105,    -1,
     106,    23,   105,    -1,   106,    -1,   107,    74,   106,    -1,
     107,    -1,   108,    84,   107,    -1,   108,    -1,   109,    85,
     108,    -1,   109,    -1,   110,    26,   109,    -1,   110,    -1,
     111,    27,   110,    -1,   111,    -1,   111,    86,   115,    87,
     112,    -1,   112,    -1,    99,   114,   113,    -1,    88,    -1,
      12,    -1,    13,    -1,    14,    -1,    10,    -1,    11,    -1,
      20,    -1,    21,    -1,    16,    -1,    15,    -1,    17,    -1,
     113,    -1,   115,    73,   113,    -1,   112,    -1,   118,   119,
      89,    -1,   118,    89,    -1,   121,   118,    -1,   121,    -1,
     122,   118,    -1,   122,    -1,   133,   118,    -1,   133,    -1,
     134,   118,    -1,   134,    -1,   120,    -1,   119,    73,   120,
      -1,   135,    -1,   135,    88,   147,    -1,    60,    -1,    43,
      -1,    57,    -1,    31,    -1,    51,    -1,    63,    -1,    35,
      -1,    54,    -1,    49,    -1,    50,    -1,    44,    -1,    40,
      -1,    55,    -1,    62,    -1,    32,    -1,   123,    -1,   130,
      -1,   146,    -1,   124,    96,    90,   125,    91,    -1,   124,
      90,   125,    91,    -1,   124,    96,    -1,    58,    -1,    61,
      -1,   126,    -1,   125,   126,    -1,   127,   128,    89,    -1,
     122,   127,    -1,   122,    -1,   133,   127,    -1,   133,    -1,
     129,    -1,   128,    73,   129,    -1,    -1,   135,    -1,    87,
     116,    -1,   135,    87,   116,    -1,    42,    96,    90,   131,
      91,    -1,    42,    90,   131,    91,    -1,    42,    96,    90,
     131,    73,    91,    -1,    42,    90,   131,    73,    91,    -1,
      42,    96,    -1,   132,    -1,   131,    73,   132,    -1,    95,
      -1,    95,    88,   116,    -1,    36,    -1,    52,    -1,    64,
      -1,    48,    -1,   137,   136,    -1,   136,    -1,    95,    -1,
      68,   135,    69,    -1,   136,    70,   113,    71,    -1,   136,
      70,    71,    -1,   136,    68,   139,    69,    -1,   136,    68,
     142,    69,    -1,   136,    68,    69,    -1,    75,   138,    -1,
      75,    -1,    75,   138,   137,    -1,    75,   137,    -1,   133,
      -1,   138,   133,    -1,   140,    -1,   140,    73,     9,    -1,
     141,    -1,   140,    73,   141,    -1,   118,   135,    -1,   118,
     144,    -1,   118,    -1,    95,    -1,   142,    73,    95,    -1,
     127,    -1,   127,   144,    -1,   137,    -1,   145,    -1,   137,
     145,    -1,    68,   144,    69,    -1,    70,    71,    -1,    70,
     113,    71,    -1,   145,    70,    71,    -1,   145,    70,   113,
      71,    -1,    68,    69,    -1,    68,   139,    69,    -1,   145,
      68,    69,    -1,   145,    68,   139,    69,    -1,     4,    -1,
     113,    -1,    90,   148,    91,    -1,    90,   148,    73,    91,
      -1,   147,    -1,   148,    73,   147,    -1,   150,    -1,   151,
      -1,   154,    -1,   155,    -1,   156,    -1,   157,    -1,    96,
      87,   149,    -1,    34,   116,    87,   149,    -1,    38,    87,
     149,    -1,    90,    91,    -1,    90,   152,    91,    -1,   153,
      -1,   152,   153,    -1,   117,    -1,   149,    -1,    89,    -1,
     115,    89,    -1,    47,    68,   115,    69,   149,    -1,    47,
      68,   115,    69,   149,    41,   149,    -1,    59,    68,   115,
      69,   149,    -1,    65,    68,   115,    69,   149,    -1,    39,
     149,    65,    68,   115,    69,    89,    -1,    45,    68,    89,
      89,    69,   149,    -1,    45,    68,   115,    89,    89,    69,
     149,    -1,    45,    68,    89,   115,    89,    69,   149,    -1,
      45,    68,   115,    89,   115,    89,    69,   149,    -1,    45,
      68,    89,    89,   115,    69,   149,    -1,    45,    68,   115,
      89,    89,   115,    69,   149,    -1,    45,    68,    89,   115,
      89,   115,    69,   149,    -1,    45,    68,   115,    89,   115,
      89,   115,    69,   149,    -1,    46,    96,    89,    -1,    37,
      89,    -1,    33,    89,    -1,    53,    89,    -1,    53,   115,
      89,    -1,   159,    -1,   158,   159,    -1,   160,    -1,   117,
      -1,   166,    -1,   118,   135,   161,   151,    -1,   118,   135,
     151,    -1,   117,    -1,   161,   117,    -1,    66,    -1,    67,
      -1,   162,    68,   142,    69,    -1,   163,   116,    -1,   164,
      -1,   165,    -1,     1,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   279,   279,   286,   298,   302,   306,   307,   315,   321,
     335,   342,   343,   347,   348,   352,   356,   360,   364,   368,
     372,   379,   380,   384,   385,   389,   393,   416,   420,   427,
     431,   435,   439,   443,   447,   454,   455,   462,   463,   469,
     477,   486,   487,   493,   502,   503,   509,   518,   519,   525,
     531,   537,   546,   547,   553,   562,   563,   572,   573,   582,
     583,   592,   593,   602,   603,   612,   613,   620,   621,   628,
     629,   630,   631,   632,   633,   634,   635,   636,   637,   638,
     642,   643,   650,   656,   672,   676,   681,   686,   691,   692,
     697,   702,   707,   715,   719,   726,   727,   731,   735,   739,
     743,   747,   754,   758,   762,   766,   770,   774,   778,   782,
     786,   790,   794,   795,   796,   803,   821,   826,   834,   838,
     845,   846,   853,   869,   874,   875,   880,   888,   892,   900,
     903,   904,   908,   912,   918,   924,   930,   936,   943,   947,
     954,   962,   974,   978,   982,   989,   996,  1001,  1005,  1010,
    1014,  1019,  1024,  1034,  1041,  1050,  1055,  1059,  1064,  1071,
    1072,  1079,  1080,  1084,  1088,  1095,  1100,  1105,  1113,  1119,
    1128,  1129,  1133,  1138,  1139,  1147,  1151,  1156,  1161,  1166,
    1171,  1177,  1187,  1193,  1206,  1213,  1214,  1215,  1219,  1220,
    1226,  1227,  1228,  1229,  1230,  1231,  1235,  1236,  1237,  1241,
    1242,  1246,  1247,  1251,  1252,  1256,  1257,  1261,  1262,  1263,
    1267,  1268,  1269,  1270,  1271,  1272,  1273,  1274,  1275,  1276,
    1280,  1281,  1282,  1283,  1284,  1290,  1291,  1295,  1296,  1297,
    1301,  1302,  1306,  1307,  1313,  1320,  1327,  1331,  1341,  1342,
    1343
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "\"identifier\"", "\"typedef-name\"",
  "INTEGER", "FLOATING", "CHARACTER", "STRING", "ELLIPSIS", "ADDEQ",
  "SUBEQ", "MULEQ", "DIVEQ", "MODEQ", "XOREQ", "ANDEQ", "OREQ", "SL", "SR",
  "SLEQ", "SREQ", "EQ", "NOTEQ", "LTEQ", "GTEQ", "ANDAND", "OROR",
  "PLUSPLUS", "MINUSMINUS", "ARROW", "AUTO", "BOOL", "BREAK", "CASE",
  "CHAR", "CONST", "CONTINUE", "DEFAULT", "DO", "DOUBLE", "ELSE", "ENUM",
  "EXTERN", "FLOAT", "FOR", "GOTO", "IF", "INLINE", "INT", "LONG",
  "REGISTER", "RESTRICT", "RETURN", "SHORT", "SIGNED", "SIZEOF", "STATIC",
  "STRUCT", "SWITCH", "TYPEDEF", "UNION", "UNSIGNED", "VOID", "VOLATILE",
  "WHILE", "FUNCTION_MACRO", "OBJECT_MACRO", "'('", "')'", "'['", "']'",
  "'.'", "','", "'&'", "'*'", "'+'", "'-'", "'~'", "'!'", "'/'", "'%'",
  "'<'", "'>'", "'^'", "'|'", "'?'", "':'", "'='", "';'", "'{'", "'}'",
  "$accept", "primary_expression", "strings", "identifier",
  "identifier_or_typedef_name", "postfix_expression",
  "argument_expression_list", "unary_expression", "unary_operator",
  "cast_expression", "multiplicative_expression", "additive_expression",
  "shift_expression", "relational_expression", "equality_expression",
  "and_expression", "exclusive_or_expression", "inclusive_or_expression",
  "logical_and_expression", "logical_or_expression",
  "conditional_expression", "assignment_expression", "assignment_operator",
  "expression", "constant_expression", "declaration",
  "declaration_specifiers", "init_declarator_list", "init_declarator",
  "storage_class_specifier", "type_specifier", "struct_or_union_specifier",
  "struct_or_union", "struct_declaration_list", "struct_declaration",
  "specifier_qualifier_list", "struct_declarator_list",
  "struct_declarator", "enum_specifier", "enumerator_list", "enumerator",
  "type_qualifier", "function_specifier", "declarator",
  "direct_declarator", "pointer", "type_qualifier_list",
  "parameter_type_list", "parameter_list", "parameter_declaration",
  "identifier_list", "type_name", "abstract_declarator",
  "direct_abstract_declarator", "typedef_name", "initializer",
  "initializer_list", "statement", "labeled_statement",
  "compound_statement", "block_item_list", "block_item",
  "expression_statement", "selection_statement", "iteration_statement",
  "jump_statement", "translation_unit", "external_declaration",
  "function_definition", "declaration_list", "function_macro",
  "object_macro", "function_macro_define", "object_macro_define", "macro", 0
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
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,    40,    41,
      91,    93,    46,    44,    38,    42,    43,    45,   126,    33,
      47,    37,    60,    62,    94,   124,    63,    58,    61,    59,
     123,   125
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    92,    93,    93,    93,    93,    93,    93,    94,    94,
      95,    96,    96,    97,    97,    97,    97,    97,    97,    97,
      97,    98,    98,    99,    99,    99,    99,    99,    99,   100,
     100,   100,   100,   100,   100,   101,   101,   102,   102,   102,
     102,   103,   103,   103,   104,   104,   104,   105,   105,   105,
     105,   105,   106,   106,   106,   107,   107,   108,   108,   109,
     109,   110,   110,   111,   111,   112,   112,   113,   113,   114,
     114,   114,   114,   114,   114,   114,   114,   114,   114,   114,
     115,   115,   116,   117,   117,   118,   118,   118,   118,   118,
     118,   118,   118,   119,   119,   120,   120,   121,   121,   121,
     121,   121,   122,   122,   122,   122,   122,   122,   122,   122,
     122,   122,   122,   122,   122,   123,   123,   123,   124,   124,
     125,   125,   126,   127,   127,   127,   127,   128,   128,   129,
     129,   129,   129,   130,   130,   130,   130,   130,   131,   131,
     132,   132,   133,   133,   133,   134,   135,   135,   136,   136,
     136,   136,   136,   136,   136,   137,   137,   137,   137,   138,
     138,   139,   139,   140,   140,   141,   141,   141,   142,   142,
     143,   143,   144,   144,   144,   145,   145,   145,   145,   145,
     145,   145,   145,   145,   146,   147,   147,   147,   148,   148,
     149,   149,   149,   149,   149,   149,   150,   150,   150,   151,
     151,   152,   152,   153,   153,   154,   154,   155,   155,   155,
     156,   156,   156,   156,   156,   156,   156,   156,   156,   156,
     157,   157,   157,   157,   157,   158,   158,   159,   159,   159,
     160,   160,   161,   161,   162,   163,   164,   165,   166,   166,
     166
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     1,     1,     1,     1,     3,     1,     2,
       1,     1,     1,     1,     4,     4,     3,     3,     3,     2,
       2,     1,     3,     1,     2,     2,     2,     2,     4,     1,
       1,     1,     1,     1,     1,     1,     4,     1,     3,     3,
       3,     1,     3,     3,     1,     3,     3,     1,     3,     3,
       3,     3,     1,     3,     3,     1,     3,     1,     3,     1,
       3,     1,     3,     1,     3,     1,     5,     1,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     3,     1,     3,     2,     2,     1,     2,     1,     2,
       1,     2,     1,     1,     3,     1,     3,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     5,     4,     2,     1,     1,
       1,     2,     3,     2,     1,     2,     1,     1,     3,     0,
       1,     2,     3,     5,     4,     6,     5,     2,     1,     3,
       1,     3,     1,     1,     1,     1,     2,     1,     1,     3,
       4,     3,     4,     4,     3,     2,     1,     3,     2,     1,
       2,     1,     3,     1,     3,     2,     2,     1,     1,     3,
       1,     2,     1,     1,     2,     3,     2,     3,     3,     4,
       2,     3,     3,     4,     1,     1,     3,     4,     1,     3,
       1,     1,     1,     1,     1,     1,     3,     4,     3,     2,
       3,     1,     2,     1,     1,     1,     2,     5,     7,     5,
       5,     7,     6,     7,     7,     8,     7,     8,     8,     9,
       3,     2,     2,     2,     3,     1,     2,     1,     1,     1,
       4,     3,     1,     2,     1,     1,     4,     2,     1,     1,
       1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,   240,   184,   100,   111,   103,   142,   108,     0,    98,
     107,   145,   105,   106,   101,   143,   104,   109,    99,   118,
      97,   119,   110,   102,   144,   234,   235,   228,     0,    86,
      88,   112,     0,   113,    90,    92,   114,     0,   225,   227,
       0,     0,   238,   239,   229,    10,     0,    11,   137,    12,
       0,   156,    84,   148,     0,    93,    95,   147,     0,    85,
      87,     0,   117,    89,    91,     1,   226,     0,     3,     5,
       4,     8,     0,     0,     0,     0,    29,    30,    31,    32,
      33,    34,    13,     6,     2,    23,    35,     0,    37,    41,
      44,    47,    52,    55,    57,    59,    61,    63,    65,    82,
     237,   140,     0,   138,     0,     0,   159,   158,   155,     0,
      83,     0,     0,   232,     0,   231,     0,     0,     0,   146,
     124,     0,   120,   129,   126,     0,   168,     0,     0,    24,
      25,     0,    27,    35,    67,    80,     0,   170,     0,     9,
      19,    20,     0,     0,     0,     0,    26,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   134,     0,
     149,   160,   157,    94,    95,     0,   185,    96,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   205,
     199,     2,     0,     0,   203,   114,   204,   190,   191,     0,
     201,   192,   193,   194,   195,   233,   230,   154,   167,     0,
     161,   163,     0,   151,     0,   123,   116,   121,     0,     0,
     127,   130,   125,     0,   236,     0,     0,    73,    74,    70,
      71,    72,    78,    77,    79,    75,    76,    69,     0,     7,
       0,     0,     0,   172,   171,   173,     0,    18,    16,     0,
      21,     0,    17,    38,    39,    40,    42,    43,    45,    46,
      50,    51,    48,    49,    53,    54,    56,    58,    60,    62,
      64,     0,   141,   136,   139,     0,   133,   188,     0,   222,
       0,   221,     0,     0,     0,     0,     0,   223,     0,     0,
       0,     0,   206,   200,   202,     0,   165,   172,   166,   152,
       0,   153,   150,   131,   129,   122,     0,   115,   169,    28,
      68,    81,   180,     0,     0,   176,     0,   174,     0,     0,
      36,    15,     0,    14,     0,   135,     0,   186,     0,   198,
       0,     0,     0,   220,     0,   224,     0,     0,   196,   162,
     164,   128,   132,   181,   175,   177,   182,     0,   178,     0,
      22,    66,   187,   189,   197,     0,     0,     0,     0,     0,
       0,     0,   183,   179,     0,     0,     0,     0,     0,     0,
     207,   209,   210,     0,   212,     0,     0,     0,     0,     0,
       0,     0,   211,   216,   214,     0,   213,     0,     0,     0,
     208,   218,   217,   215,     0,   219
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    82,    83,    84,   192,    85,   249,   133,    87,    88,
      89,    90,    91,    92,    93,    94,    95,    96,    97,    98,
     134,   135,   238,   193,   100,    27,   208,    54,    55,    29,
      30,    31,    32,   121,   122,   123,   219,   220,    33,   102,
     103,    34,    35,   105,    57,    58,   108,   313,   210,   211,
     127,   138,   314,   245,    36,   177,   278,   196,   197,   198,
     199,   200,   201,   202,   203,   204,    37,    38,    39,   116,
      40,    41,    42,    43,    44
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -199
static const yytype_int16 yypact[] =
{
    1546,  -199,  -199,  -199,  -199,  -199,  -199,  -199,    18,  -199,
    -199,  -199,  -199,  -199,  -199,  -199,  -199,  -199,  -199,  -199,
    -199,  -199,  -199,  -199,  -199,  -199,  -199,  -199,    52,  1718,
    1718,  -199,    31,  -199,  1718,  1718,  -199,  1509,  -199,  -199,
     -39,  1318,  -199,  -199,  -199,  -199,    37,  -199,   -47,  -199,
      57,   217,  -199,  -199,   -37,  -199,   742,    60,    12,  -199,
    -199,  1751,   -12,  -199,  -199,  -199,  -199,    37,  -199,  -199,
    -199,  -199,  1333,  1333,  1347,  1013,  -199,  -199,  -199,  -199,
    -199,  -199,  -199,   105,  -199,   143,  -199,  1318,  -199,   168,
     130,   203,   127,   222,    49,   -19,   123,   166,    24,  -199,
    -199,   102,   -29,  -199,    37,   177,  -199,  -199,   217,    57,
    -199,   733,   422,  -199,    52,  -199,   809,  1588,  1077,    60,
    1751,   626,  -199,    13,  1751,  1751,  -199,   -32,  1318,  -199,
    -199,  1013,  -199,   319,  -199,  -199,   120,   126,   188,  -199,
    -199,  -199,   299,  1090,  1318,   299,  -199,  1318,  1318,  1318,
    1318,  1318,  1318,  1318,  1318,  1318,  1318,  1318,  1318,  1318,
    1318,  1318,  1318,  1318,  1318,  1318,  1318,    11,  -199,   104,
    -199,  -199,  -199,  -199,   175,   733,  -199,  -199,   176,  1318,
     181,   206,   618,   210,   299,   243,   819,   247,   255,  -199,
    -199,   237,   238,   -10,  -199,   239,  -199,  -199,  -199,   499,
    -199,  -199,  -199,  -199,  -199,  -199,  -199,  -199,    54,   221,
     254,  -199,   183,  -199,   266,  -199,  -199,  -199,  1318,    -4,
    -199,   251,  -199,   669,  -199,    37,   272,  -199,  -199,  -199,
    -199,  -199,  -199,  -199,  -199,  -199,  -199,  -199,  1318,  -199,
    1318,  1437,  1106,    85,  -199,   111,  1318,  -199,  -199,   189,
    -199,   -43,  -199,  -199,  -199,  -199,   168,   168,   130,   130,
     203,   203,   203,   203,   127,   127,   222,    49,   -19,   123,
     166,    46,  -199,  -199,  -199,    14,  -199,  -199,   107,  -199,
     256,  -199,   618,   277,   873,   257,  1318,  -199,    65,  1318,
    1318,   618,  -199,  -199,  -199,  1396,  -199,    22,  -199,  -199,
    1684,  -199,  -199,  -199,    13,  -199,  1318,  -199,  -199,  -199,
    -199,  -199,  -199,   275,   276,  -199,   278,   111,  1623,  1183,
    -199,  -199,  1318,  -199,  1318,  -199,   563,  -199,   618,  -199,
     279,   904,   113,  -199,   191,  -199,   199,   202,  -199,  -199,
    -199,  -199,  -199,  -199,  -199,  -199,  -199,   281,  -199,   283,
    -199,  -199,  -199,  -199,  -199,  1318,  1196,   114,   958,   618,
     618,   618,  -199,  -199,   204,   618,   207,  1212,  1227,   115,
     307,  -199,  -199,   267,  -199,   618,   618,   226,   618,   231,
    1304,   618,  -199,  -199,  -199,   618,  -199,   618,   618,   232,
    -199,  -199,  -199,  -199,   618,  -199
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -199,  -199,  -199,    -8,     1,  -199,  -199,    76,  -199,   -81,
     163,   164,   152,   160,   195,   201,   209,   200,   205,  -199,
     -40,   -72,  -199,   -70,  -148,   -30,    19,  -199,   263,  -199,
     -48,  -199,  -199,   241,  -109,    16,  -199,    70,  -199,   260,
    -157,    36,  -199,   -25,   -56,   -44,  -199,  -113,  -199,    83,
     280,   253,  -114,  -198,     0,  -164,  -199,    30,  -199,    -9,
    -199,   193,  -199,  -199,  -199,  -199,  -199,   356,  -199,  -199,
    -199,  -199,  -199,  -199,  -199
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -13
static const yytype_int16 yytable[] =
{
      47,    99,   119,    56,   209,   136,   146,   107,    49,    48,
     274,   277,   217,   120,    45,    45,    45,    45,   272,    28,
      53,    45,     2,   244,    47,    45,   113,   120,   323,    67,
     240,   280,    49,    62,    45,     2,   109,   224,   101,   176,
      45,   225,    53,   104,   167,   317,   214,   115,    59,    60,
      53,   164,   110,    63,    64,    45,    28,    45,   136,   126,
      45,   136,   168,   240,   172,   161,   253,   254,   255,   304,
     303,   250,   120,   120,   251,   114,   120,   120,   125,   292,
      50,    50,   194,   120,   174,   305,   205,   106,    51,   174,
     295,   137,   242,   243,   298,   271,   101,   124,   221,   317,
     218,    53,   273,   176,   191,   325,    53,   206,    46,   126,
     165,   124,   195,   139,   217,    53,   288,    86,   274,   240,
      50,    61,   295,   160,   242,    50,    99,    51,   117,    51,
     118,   114,    51,   324,    47,   114,   215,    47,   240,    99,
     222,    52,    49,   247,   171,    49,   252,   137,   129,   130,
     132,   154,   155,   241,   335,   242,   124,   124,   342,   101,
     124,   124,   353,    86,   297,   320,   310,   124,   311,   194,
     316,   140,   141,   142,   191,   120,    47,   275,    99,   318,
     326,   319,    49,   296,    49,   285,   240,   240,   240,   239,
     166,   191,   163,   240,   241,   276,   242,   243,   327,   195,
      53,    51,   358,   367,   380,   347,   150,   151,   162,   156,
     157,   143,   283,   144,   332,   145,   334,   308,   114,   336,
     337,   152,   153,    86,    86,    86,    86,    86,    86,    86,
      86,    86,    86,    86,    86,    86,    86,    86,    86,    86,
      86,   119,    86,   147,   158,   159,   170,   349,   148,   149,
     350,   297,   301,     6,   176,    86,   225,   246,   321,   124,
     359,   357,   322,   111,   240,   279,    99,   101,   360,    15,
     281,   361,   240,   373,   191,   240,   375,   240,   284,   221,
     240,    24,    49,   191,   351,   364,   366,    53,   369,    53,
     299,    49,    51,   282,    86,   385,    53,   377,   379,   240,
     387,   394,    45,     2,   240,   240,   260,   261,   262,   263,
     389,   286,   329,   256,   257,   289,   258,   259,   264,   265,
     191,   338,    86,   290,   -11,   291,   -12,   300,    49,   227,
     228,   229,   230,   231,   232,   233,   234,   302,   306,   235,
     236,   309,   330,   328,   343,   344,   333,   355,   381,   345,
     362,   191,   191,   191,   363,   266,   382,   191,   354,    49,
      49,    49,   267,   269,   169,    49,   223,   191,   191,   270,
     191,   268,   173,   191,   341,    49,    49,   191,    49,   191,
     191,    49,    86,   340,   226,    49,   191,    49,    49,   370,
     371,   372,   294,    66,    49,   374,     0,   212,     0,     0,
      86,     0,     0,     0,     0,   383,   384,   237,   386,     0,
       0,   390,     0,     0,     0,   391,     0,   392,   393,     0,
       0,     0,     0,     0,   395,    45,     2,    68,    69,    70,
      71,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      72,    73,     0,     3,     4,   178,   179,     5,     6,   180,
     181,   182,     7,     0,     8,     9,    10,   183,   184,   185,
      11,    12,    13,    14,    15,   186,    16,    17,    74,    18,
      19,   187,    20,    21,    22,    23,    24,   188,     0,     0,
      75,     0,     0,     0,     0,     0,    76,    77,    78,    79,
      80,    81,    45,     2,    68,    69,    70,    71,     0,     0,
       0,   189,   112,   190,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    72,    73,     0,
       3,     4,   178,   179,     5,     6,   180,   181,   182,     7,
       0,     8,     9,    10,   183,   184,   185,    11,    12,    13,
      14,    15,   186,    16,    17,    74,    18,    19,   187,    20,
      21,    22,    23,    24,   188,     0,    45,    75,    68,    69,
      70,    71,     0,    76,    77,    78,    79,    80,    81,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   189,   112,
     293,    72,    73,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    74,
       0,    45,     2,    68,    69,    70,    71,     0,     0,     0,
       2,    75,     0,     0,     0,     0,     0,    76,    77,    78,
      79,    80,    81,     0,     0,     0,    72,    73,     0,     0,
       0,   178,   179,   175,   352,   180,   181,   182,     4,     0,
       0,     5,     6,   183,   184,   185,     7,     0,     8,     0,
      10,   186,     0,     2,    74,    12,    13,   187,    15,     0,
      16,    17,     0,   188,    19,     0,    75,    21,    22,    23,
      24,     0,    76,    77,    78,    79,    80,    81,     0,     0,
       0,     4,     0,     0,     5,     6,     0,   189,   112,     7,
       0,     8,     0,    10,     0,     0,     0,   216,    12,    13,
       0,    15,     0,    16,    17,     0,     0,    19,     0,     0,
      21,    22,    23,    24,     0,     0,    45,     0,    68,    69,
      70,    71,     0,     0,     0,     0,     2,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     307,    72,    73,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     3,     4,     0,     0,     5,     6,     0,
       0,     0,     7,     0,     8,     9,    10,     0,     0,    74,
      11,    12,    13,    14,    15,     0,    16,    17,     0,    18,
      19,    75,    20,    21,    22,    23,    24,    76,    77,    78,
      79,    80,    81,     2,     0,     0,     0,     0,     0,     0,
       0,     0,    45,   175,    68,    69,    70,    71,     0,     0,
     111,     0,   112,     0,     0,     0,     0,     0,     0,     0,
       3,     4,     0,     0,     5,     6,     0,    72,    73,     7,
       0,     8,     9,    10,     0,     0,     0,    11,    12,    13,
      14,    15,     0,    16,    17,     0,    18,    19,     0,    20,
      21,    22,    23,    24,     0,    74,    45,     0,    68,    69,
      70,    71,     0,     0,     0,     0,     0,    75,     0,     0,
       0,     0,     0,    76,    77,    78,    79,    80,    81,   112,
       0,    72,    73,     0,     0,     0,     0,    45,   287,    68,
      69,    70,    71,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    74,
       0,     0,    72,    73,     0,     0,     0,     0,     0,     0,
       0,    75,     0,     0,     0,     0,     0,    76,    77,    78,
      79,    80,    81,     0,     0,     0,     0,     0,     0,     0,
      74,    45,   331,    68,    69,    70,    71,     0,     0,     0,
       0,     0,    75,     0,     0,     0,     0,     0,    76,    77,
      78,    79,    80,    81,     0,     0,    72,    73,     0,     0,
       0,     0,     0,   356,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    74,     0,    45,     2,    68,    69,
      70,    71,     0,     0,     0,     0,    75,     0,     0,     0,
       0,     0,    76,    77,    78,    79,    80,    81,     0,     0,
       0,    72,    73,     0,     0,     4,     0,   368,     5,     6,
       0,     0,     0,     7,     0,     8,     0,    10,     0,     0,
       0,     0,    12,    13,     0,    15,     0,    16,    17,    74,
       0,    19,     0,     0,    21,    22,    23,    24,     0,     0,
      45,    75,    68,    69,    70,    71,     0,    76,    77,    78,
      79,    80,    81,    45,     0,    68,    69,    70,    71,     0,
       0,     0,     0,     0,     0,    72,    73,     0,     0,    45,
       0,    68,    69,    70,    71,     0,     0,     0,    72,    73,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    74,    72,    73,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    75,    74,     0,   213,     0,
       0,    76,    77,    78,    79,    80,    81,     0,    75,   248,
       0,     0,    74,     0,    76,    77,    78,    79,    80,    81,
       0,     0,     0,     0,    75,     0,     0,   315,     0,     0,
      76,    77,    78,    79,    80,    81,    45,     0,    68,    69,
      70,    71,     0,     0,     0,     0,     0,     0,     0,    45,
       0,    68,    69,    70,    71,     0,     0,     0,     0,     0,
       0,    72,    73,     0,     0,    45,     0,    68,    69,    70,
      71,     0,     0,     0,    72,    73,     0,     0,     0,     0,
      45,     0,    68,    69,    70,    71,     0,     0,     0,    74,
      72,    73,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    75,    74,     0,   348,    72,    73,    76,    77,    78,
      79,    80,    81,     0,    75,   365,     0,     0,    74,     0,
      76,    77,    78,    79,    80,    81,     0,     0,     0,     0,
      75,   376,     0,    74,     0,     0,    76,    77,    78,    79,
      80,    81,     0,     0,     0,    75,   378,     0,     0,     0,
       0,    76,    77,    78,    79,    80,    81,    45,     0,    68,
      69,    70,    71,     0,     0,     0,     0,     0,     0,     0,
       0,    45,     0,    68,    69,    70,    71,     0,     0,     0,
       0,     0,    72,    73,     0,     0,    45,     0,    68,    69,
      70,    71,     0,     0,     0,     0,    72,    73,     0,     0,
      45,     0,    68,    69,    70,    71,     0,     0,     0,     0,
      74,    72,    73,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    75,   388,    74,    72,    73,     0,    76,    77,
      78,    79,    80,    81,     0,     0,    75,     0,     0,    74,
       0,     0,    76,    77,    78,    79,    80,    81,     0,    45,
       2,   128,     0,    74,     0,     0,     0,    76,    77,    78,
      79,    80,    81,     0,     0,   131,     0,     0,     0,     0,
       0,    76,    77,    78,    79,    80,    81,     3,     4,     0,
       0,     5,     6,     0,     0,     0,     7,     0,     8,     9,
      10,     2,     0,     0,    11,    12,    13,    14,    15,     0,
      16,    17,     0,    18,    19,     0,    20,    21,    22,    23,
      24,     0,     0,     0,   295,   312,   242,     0,     3,     4,
       0,    51,     5,     6,     0,     0,     0,     7,     0,     8,
       9,    10,     0,     0,     0,    11,    12,    13,    14,    15,
       0,    16,    17,     0,    18,    19,     0,    20,    21,    22,
      23,    24,     0,     0,     0,   241,   312,   242,     0,    65,
       1,     0,    51,     2,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       3,     4,     0,     0,     5,     6,     0,     1,     0,     7,
       2,     8,     9,    10,     0,     0,     0,    11,    12,    13,
      14,    15,     0,    16,    17,     0,    18,    19,     0,    20,
      21,    22,    23,    24,     0,    25,    26,     3,     4,     0,
       0,     5,     6,     0,     0,     0,     7,     0,     8,     9,
      10,    45,     2,     0,    11,    12,    13,    14,    15,     0,
      16,    17,     0,    18,    19,     0,    20,    21,    22,    23,
      24,     0,    25,    26,     0,     0,     0,     0,     0,     3,
       4,     0,     0,     5,     6,     0,     0,     2,     7,     0,
       8,     9,    10,     0,     0,     0,    11,    12,    13,    14,
      15,     0,    16,    17,     0,    18,    19,     0,    20,    21,
      22,    23,    24,     0,     3,     4,     0,   207,     5,     6,
       0,     0,     0,     7,     0,     8,     9,    10,     0,     0,
       0,    11,    12,    13,    14,    15,     0,    16,    17,     0,
      18,    19,     0,    20,    21,    22,    23,    24,     2,     0,
       0,     0,   346,   339,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     3,     4,     0,     0,     5,
       6,     0,     2,     0,     7,     0,     8,     9,    10,     0,
       0,     0,    11,    12,    13,    14,    15,     0,    16,    17,
       0,    18,    19,     0,    20,    21,    22,    23,    24,     3,
       4,     0,     0,     5,     6,     2,     0,     0,     7,     0,
       8,     9,    10,     0,     0,     0,    11,    12,    13,    14,
      15,     0,    16,    17,     0,    18,    19,     0,    20,    21,
      22,    23,    24,     4,     0,     0,     5,     6,     0,     0,
       0,     7,     0,     8,     0,    10,     0,     0,     0,     0,
      12,    13,     0,    15,     0,    16,    17,     0,     0,    19,
       0,     0,    21,    22,    23,    24
};

static const yytype_int16 yycheck[] =
{
       8,    41,    58,    28,   117,    75,    87,    51,     8,     8,
     167,   175,   121,    61,     3,     3,     3,     3,   166,     0,
      28,     3,     4,   137,    32,     3,    56,    75,    71,    68,
      73,   179,    32,    32,     3,     4,    73,    69,    46,   111,
       3,    73,    50,    90,    73,   243,   118,    56,    29,    30,
      58,    27,    89,    34,    35,     3,    37,     3,   128,    67,
       3,   131,    91,    73,   108,    84,   147,   148,   149,    73,
     218,   143,   120,   121,   144,    56,   124,   125,    90,    89,
      68,    68,   112,   131,   109,    89,   116,    51,    75,   114,
      68,    75,    70,   137,   208,   165,   104,    61,   123,   297,
      87,   109,    91,   175,   112,    91,   114,   116,    90,   117,
      86,    75,   112,     8,   223,   123,   186,    41,   275,    73,
      68,    90,    68,    74,    70,    68,   166,    75,    68,    75,
      70,   112,    75,    87,   142,   116,   120,   145,    73,   179,
     124,    89,   142,   142,   108,   145,   145,   131,    72,    73,
      74,    24,    25,    68,    89,    70,   120,   121,   306,   167,
     124,   125,   326,    87,   208,   246,   238,   131,   240,   199,
     242,    28,    29,    30,   182,   223,   184,    73,   218,    68,
      73,    70,   182,   208,   184,   184,    73,    73,    73,    69,
      88,   199,    26,    73,    68,    91,    70,   241,    91,   199,
     208,    75,    89,    89,    89,   318,    76,    77,    85,    82,
      83,    68,   182,    70,   284,    72,   286,   225,   199,   289,
     290,    18,    19,   147,   148,   149,   150,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   297,   166,    75,    22,    23,    69,   319,    80,    81,
     322,   295,    69,    36,   326,   179,    73,    69,    69,   223,
      69,   331,    73,    88,    73,    89,   306,   275,    69,    52,
      89,    69,    73,    69,   282,    73,    69,    73,    68,   304,
      73,    64,   282,   291,   324,   355,   356,   295,   358,   297,
      69,   291,    75,    87,   218,    69,   304,   367,   368,    73,
      69,    69,     3,     4,    73,    73,   154,   155,   156,   157,
     380,    68,   282,   150,   151,    68,   152,   153,   158,   159,
     328,   291,   246,    68,    87,    87,    87,    73,   328,    10,
      11,    12,    13,    14,    15,    16,    17,    71,    87,    20,
      21,    69,    65,    87,    69,    69,    89,    68,    41,    71,
      69,   359,   360,   361,    71,   160,    89,   365,   328,   359,
     360,   361,   161,   163,   104,   365,   125,   375,   376,   164,
     378,   162,   109,   381,   304,   375,   376,   385,   378,   387,
     388,   381,   306,   300,   131,   385,   394,   387,   388,   359,
     360,   361,   199,    37,   394,   365,    -1,   117,    -1,    -1,
     324,    -1,    -1,    -1,    -1,   375,   376,    88,   378,    -1,
      -1,   381,    -1,    -1,    -1,   385,    -1,   387,   388,    -1,
      -1,    -1,    -1,    -1,   394,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      28,    29,    -1,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    -1,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    -1,    -1,
      68,    -1,    -1,    -1,    -1,    -1,    74,    75,    76,    77,
      78,    79,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    89,    90,    91,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    28,    29,    -1,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      -1,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    -1,     3,    68,     5,     6,
       7,     8,    -1,    74,    75,    76,    77,    78,    79,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    89,    90,
      91,    28,    29,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    56,
      -1,     3,     4,     5,     6,     7,     8,    -1,    -1,    -1,
       4,    68,    -1,    -1,    -1,    -1,    -1,    74,    75,    76,
      77,    78,    79,    -1,    -1,    -1,    28,    29,    -1,    -1,
      -1,    33,    34,    90,    91,    37,    38,    39,    32,    -1,
      -1,    35,    36,    45,    46,    47,    40,    -1,    42,    -1,
      44,    53,    -1,     4,    56,    49,    50,    59,    52,    -1,
      54,    55,    -1,    65,    58,    -1,    68,    61,    62,    63,
      64,    -1,    74,    75,    76,    77,    78,    79,    -1,    -1,
      -1,    32,    -1,    -1,    35,    36,    -1,    89,    90,    40,
      -1,    42,    -1,    44,    -1,    -1,    -1,    91,    49,    50,
      -1,    52,    -1,    54,    55,    -1,    -1,    58,    -1,    -1,
      61,    62,    63,    64,    -1,    -1,     3,    -1,     5,     6,
       7,     8,    -1,    -1,    -1,    -1,     4,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      91,    28,    29,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    31,    32,    -1,    -1,    35,    36,    -1,
      -1,    -1,    40,    -1,    42,    43,    44,    -1,    -1,    56,
      48,    49,    50,    51,    52,    -1,    54,    55,    -1,    57,
      58,    68,    60,    61,    62,    63,    64,    74,    75,    76,
      77,    78,    79,     4,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,     3,    90,     5,     6,     7,     8,    -1,    -1,
      88,    -1,    90,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    32,    -1,    -1,    35,    36,    -1,    28,    29,    40,
      -1,    42,    43,    44,    -1,    -1,    -1,    48,    49,    50,
      51,    52,    -1,    54,    55,    -1,    57,    58,    -1,    60,
      61,    62,    63,    64,    -1,    56,     3,    -1,     5,     6,
       7,     8,    -1,    -1,    -1,    -1,    -1,    68,    -1,    -1,
      -1,    -1,    -1,    74,    75,    76,    77,    78,    79,    90,
      -1,    28,    29,    -1,    -1,    -1,    -1,     3,    89,     5,
       6,     7,     8,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    56,
      -1,    -1,    28,    29,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    68,    -1,    -1,    -1,    -1,    -1,    74,    75,    76,
      77,    78,    79,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      56,     3,    89,     5,     6,     7,     8,    -1,    -1,    -1,
      -1,    -1,    68,    -1,    -1,    -1,    -1,    -1,    74,    75,
      76,    77,    78,    79,    -1,    -1,    28,    29,    -1,    -1,
      -1,    -1,    -1,    89,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    56,    -1,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    -1,    68,    -1,    -1,    -1,
      -1,    -1,    74,    75,    76,    77,    78,    79,    -1,    -1,
      -1,    28,    29,    -1,    -1,    32,    -1,    89,    35,    36,
      -1,    -1,    -1,    40,    -1,    42,    -1,    44,    -1,    -1,
      -1,    -1,    49,    50,    -1,    52,    -1,    54,    55,    56,
      -1,    58,    -1,    -1,    61,    62,    63,    64,    -1,    -1,
       3,    68,     5,     6,     7,     8,    -1,    74,    75,    76,
      77,    78,    79,     3,    -1,     5,     6,     7,     8,    -1,
      -1,    -1,    -1,    -1,    -1,    28,    29,    -1,    -1,     3,
      -1,     5,     6,     7,     8,    -1,    -1,    -1,    28,    29,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    56,    28,    29,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    68,    56,    -1,    71,    -1,
      -1,    74,    75,    76,    77,    78,    79,    -1,    68,    69,
      -1,    -1,    56,    -1,    74,    75,    76,    77,    78,    79,
      -1,    -1,    -1,    -1,    68,    -1,    -1,    71,    -1,    -1,
      74,    75,    76,    77,    78,    79,     3,    -1,     5,     6,
       7,     8,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,
      -1,     5,     6,     7,     8,    -1,    -1,    -1,    -1,    -1,
      -1,    28,    29,    -1,    -1,     3,    -1,     5,     6,     7,
       8,    -1,    -1,    -1,    28,    29,    -1,    -1,    -1,    -1,
       3,    -1,     5,     6,     7,     8,    -1,    -1,    -1,    56,
      28,    29,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    68,    56,    -1,    71,    28,    29,    74,    75,    76,
      77,    78,    79,    -1,    68,    69,    -1,    -1,    56,    -1,
      74,    75,    76,    77,    78,    79,    -1,    -1,    -1,    -1,
      68,    69,    -1,    56,    -1,    -1,    74,    75,    76,    77,
      78,    79,    -1,    -1,    -1,    68,    69,    -1,    -1,    -1,
      -1,    74,    75,    76,    77,    78,    79,     3,    -1,     5,
       6,     7,     8,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     3,    -1,     5,     6,     7,     8,    -1,    -1,    -1,
      -1,    -1,    28,    29,    -1,    -1,     3,    -1,     5,     6,
       7,     8,    -1,    -1,    -1,    -1,    28,    29,    -1,    -1,
       3,    -1,     5,     6,     7,     8,    -1,    -1,    -1,    -1,
      56,    28,    29,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    68,    69,    56,    28,    29,    -1,    74,    75,
      76,    77,    78,    79,    -1,    -1,    68,    -1,    -1,    56,
      -1,    -1,    74,    75,    76,    77,    78,    79,    -1,     3,
       4,    68,    -1,    56,    -1,    -1,    -1,    74,    75,    76,
      77,    78,    79,    -1,    -1,    68,    -1,    -1,    -1,    -1,
      -1,    74,    75,    76,    77,    78,    79,    31,    32,    -1,
      -1,    35,    36,    -1,    -1,    -1,    40,    -1,    42,    43,
      44,     4,    -1,    -1,    48,    49,    50,    51,    52,    -1,
      54,    55,    -1,    57,    58,    -1,    60,    61,    62,    63,
      64,    -1,    -1,    -1,    68,    69,    70,    -1,    31,    32,
      -1,    75,    35,    36,    -1,    -1,    -1,    40,    -1,    42,
      43,    44,    -1,    -1,    -1,    48,    49,    50,    51,    52,
      -1,    54,    55,    -1,    57,    58,    -1,    60,    61,    62,
      63,    64,    -1,    -1,    -1,    68,    69,    70,    -1,     0,
       1,    -1,    75,     4,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      31,    32,    -1,    -1,    35,    36,    -1,     1,    -1,    40,
       4,    42,    43,    44,    -1,    -1,    -1,    48,    49,    50,
      51,    52,    -1,    54,    55,    -1,    57,    58,    -1,    60,
      61,    62,    63,    64,    -1,    66,    67,    31,    32,    -1,
      -1,    35,    36,    -1,    -1,    -1,    40,    -1,    42,    43,
      44,     3,     4,    -1,    48,    49,    50,    51,    52,    -1,
      54,    55,    -1,    57,    58,    -1,    60,    61,    62,    63,
      64,    -1,    66,    67,    -1,    -1,    -1,    -1,    -1,    31,
      32,    -1,    -1,    35,    36,    -1,    -1,     4,    40,    -1,
      42,    43,    44,    -1,    -1,    -1,    48,    49,    50,    51,
      52,    -1,    54,    55,    -1,    57,    58,    -1,    60,    61,
      62,    63,    64,    -1,    31,    32,    -1,    69,    35,    36,
      -1,    -1,    -1,    40,    -1,    42,    43,    44,    -1,    -1,
      -1,    48,    49,    50,    51,    52,    -1,    54,    55,    -1,
      57,    58,    -1,    60,    61,    62,    63,    64,     4,    -1,
      -1,    -1,    69,     9,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    31,    32,    -1,    -1,    35,
      36,    -1,     4,    -1,    40,    -1,    42,    43,    44,    -1,
      -1,    -1,    48,    49,    50,    51,    52,    -1,    54,    55,
      -1,    57,    58,    -1,    60,    61,    62,    63,    64,    31,
      32,    -1,    -1,    35,    36,     4,    -1,    -1,    40,    -1,
      42,    43,    44,    -1,    -1,    -1,    48,    49,    50,    51,
      52,    -1,    54,    55,    -1,    57,    58,    -1,    60,    61,
      62,    63,    64,    32,    -1,    -1,    35,    36,    -1,    -1,
      -1,    40,    -1,    42,    -1,    44,    -1,    -1,    -1,    -1,
      49,    50,    -1,    52,    -1,    54,    55,    -1,    -1,    58,
      -1,    -1,    61,    62,    63,    64
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     1,     4,    31,    32,    35,    36,    40,    42,    43,
      44,    48,    49,    50,    51,    52,    54,    55,    57,    58,
      60,    61,    62,    63,    64,    66,    67,   117,   118,   121,
     122,   123,   124,   130,   133,   134,   146,   158,   159,   160,
     162,   163,   164,   165,   166,     3,    90,    95,    96,   146,
      68,    75,    89,    95,   119,   120,   135,   136,   137,   118,
     118,    90,    96,   118,   118,     0,   159,    68,     5,     6,
       7,     8,    28,    29,    56,    68,    74,    75,    76,    77,
      78,    79,    93,    94,    95,    97,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     116,    95,   131,   132,    90,   135,   133,   137,   138,    73,
      89,    88,    90,   117,   118,   151,   161,    68,    70,   136,
     122,   125,   126,   127,   133,    90,    95,   142,    68,    99,
      99,    68,    99,    99,   112,   113,   115,   127,   143,     8,
      28,    29,    30,    68,    70,    72,   101,    75,    80,    81,
      76,    77,    18,    19,    24,    25,    82,    83,    22,    23,
      74,    84,    85,    26,    27,    86,    88,    73,    91,   131,
      69,   133,   137,   120,   135,    90,   113,   147,    33,    34,
      37,    38,    39,    45,    46,    47,    53,    59,    65,    89,
      91,    95,    96,   115,   117,   146,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   117,   151,    69,   118,   139,
     140,   141,   142,    71,   113,   127,    91,   126,    87,   128,
     129,   135,   127,   125,    69,    73,   143,    10,    11,    12,
      13,    14,    15,    16,    17,    20,    21,    88,   114,    69,
      73,    68,    70,   137,   144,   145,    69,    96,    69,    98,
     113,   115,    96,   101,   101,   101,   102,   102,   103,   103,
     104,   104,   104,   104,   105,   105,   106,   107,   108,   109,
     110,   115,   116,    91,   132,    73,    91,   147,   148,    89,
     116,    89,    87,   149,    68,    96,    68,    89,   115,    68,
      68,    87,    89,    91,   153,    68,   135,   137,   144,    69,
      73,    69,    71,   116,    73,    89,    87,    91,    95,    69,
     113,   113,    69,   139,   144,    71,   113,   145,    68,    70,
     101,    69,    73,    71,    87,    91,    73,    91,    87,   149,
      65,    89,   115,    89,   115,    89,   115,   115,   149,     9,
     141,   129,   116,    69,    69,    71,    69,   139,    71,   113,
     113,   112,    91,   147,   149,    68,    89,   115,    89,    69,
      69,    69,    69,    71,   115,    69,   115,    89,    89,   115,
     149,   149,   149,    69,   149,    69,    69,   115,    69,   115,
      89,    41,    89,   149,   149,    69,   149,    69,    69,   115,
     149,   149,   149,   149,    69,   149
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

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (igenerator, YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex (igenerator)
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
		  Type, Value, igenerator); \
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
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, GIGenerator* igenerator)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep, igenerator)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    GIGenerator* igenerator;
#endif
{
  if (!yyvaluep)
    return;
  YYUSE (igenerator);
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
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, GIGenerator* igenerator)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep, igenerator)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    GIGenerator* igenerator;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep, igenerator);
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
yy_reduce_print (YYSTYPE *yyvsp, int yyrule, GIGenerator* igenerator)
#else
static void
yy_reduce_print (yyvsp, yyrule, igenerator)
    YYSTYPE *yyvsp;
    int yyrule;
    GIGenerator* igenerator;
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
		       		       , igenerator);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule, igenerator); \
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

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, GIGenerator* igenerator)
#else
static void
yydestruct (yymsg, yytype, yyvaluep, igenerator)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
    GIGenerator* igenerator;
#endif
{
  YYUSE (yyvaluep);
  YYUSE (igenerator);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}

/* Prevent warnings from -Wmissing-prototypes.  */
#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (GIGenerator* igenerator);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */


/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*-------------------------.
| yyparse or yypush_parse.  |
`-------------------------*/

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
yyparse (GIGenerator* igenerator)
#else
int
yyparse (igenerator)
    GIGenerator* igenerator;
#endif
#endif
{


    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

       Refer to the stacks thru separate pointers, to allow yyoverflow
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
  int yytoken;
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

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */
  yyssp = yyss;
  yyvsp = yyvs;

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
  if (yyn == YYPACT_NINF)
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
      if (yyn == 0 || yyn == YYTABLE_NINF)
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
  *++yyvsp = yylval;

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

/* Line 1464 of yacc.c  */
#line 280 "scannerparser.y"
    {
		(yyval.symbol) = g_hash_table_lookup (const_table, (yyvsp[(1) - (1)].str));
		if ((yyval.symbol) == NULL) {
			(yyval.symbol) = csymbol_new (CSYMBOL_TYPE_INVALID);
		}
	  }
    break;

  case 3:

/* Line 1464 of yacc.c  */
#line 287 "scannerparser.y"
    {
		(yyval.symbol) = csymbol_new (CSYMBOL_TYPE_CONST);
		(yyval.symbol)->const_int_set = TRUE;
		if (g_str_has_prefix (yytext, "0x") && strlen (yytext) > 2) {
			(yyval.symbol)->const_int = strtol (yytext + 2, NULL, 16);
		} else if (g_str_has_prefix (yytext, "0") && strlen (yytext) > 1) {
			(yyval.symbol)->const_int = strtol (yytext + 1, NULL, 8);
		} else {
			(yyval.symbol)->const_int = atoi (yytext);
		}
	  }
    break;

  case 4:

/* Line 1464 of yacc.c  */
#line 299 "scannerparser.y"
    {
		(yyval.symbol) = csymbol_new (CSYMBOL_TYPE_INVALID);
	  }
    break;

  case 5:

/* Line 1464 of yacc.c  */
#line 303 "scannerparser.y"
    {
		(yyval.symbol) = csymbol_new (CSYMBOL_TYPE_INVALID);
	  }
    break;

  case 7:

/* Line 1464 of yacc.c  */
#line 308 "scannerparser.y"
    {
		(yyval.symbol) = (yyvsp[(2) - (3)].symbol);
	  }
    break;

  case 8:

/* Line 1464 of yacc.c  */
#line 316 "scannerparser.y"
    {
		(yyval.symbol) = csymbol_new (CSYMBOL_TYPE_CONST);
		yytext[strlen (yytext) - 1] = '\0';
		(yyval.symbol)->const_string = g_strcompress (yytext + 1);
	  }
    break;

  case 9:

/* Line 1464 of yacc.c  */
#line 322 "scannerparser.y"
    {
		char *strings, *string2;
		(yyval.symbol) = (yyvsp[(1) - (2)].symbol);
		yytext[strlen (yytext) - 1] = '\0';
		string2 = g_strcompress (yytext + 1);
		strings = g_strconcat ((yyval.symbol)->const_string, string2, NULL);
		g_free ((yyval.symbol)->const_string);
		g_free (string2);
		(yyval.symbol)->const_string = strings;
	  }
    break;

  case 10:

/* Line 1464 of yacc.c  */
#line 336 "scannerparser.y"
    {
		(yyval.str) = g_strdup (yytext);
	  }
    break;

  case 14:

/* Line 1464 of yacc.c  */
#line 349 "scannerparser.y"
    {
		(yyval.symbol) = csymbol_new (CSYMBOL_TYPE_INVALID);
	  }
    break;

  case 15:

/* Line 1464 of yacc.c  */
#line 353 "scannerparser.y"
    {
		(yyval.symbol) = csymbol_new (CSYMBOL_TYPE_INVALID);
	  }
    break;

  case 16:

/* Line 1464 of yacc.c  */
#line 357 "scannerparser.y"
    {
		(yyval.symbol) = csymbol_new (CSYMBOL_TYPE_INVALID);
	  }
    break;

  case 17:

/* Line 1464 of yacc.c  */
#line 361 "scannerparser.y"
    {
		(yyval.symbol) = csymbol_new (CSYMBOL_TYPE_INVALID);
	  }
    break;

  case 18:

/* Line 1464 of yacc.c  */
#line 365 "scannerparser.y"
    {
		(yyval.symbol) = csymbol_new (CSYMBOL_TYPE_INVALID);
	  }
    break;

  case 19:

/* Line 1464 of yacc.c  */
#line 369 "scannerparser.y"
    {
		(yyval.symbol) = csymbol_new (CSYMBOL_TYPE_INVALID);
	  }
    break;

  case 20:

/* Line 1464 of yacc.c  */
#line 373 "scannerparser.y"
    {
		(yyval.symbol) = csymbol_new (CSYMBOL_TYPE_INVALID);
	  }
    break;

  case 24:

/* Line 1464 of yacc.c  */
#line 386 "scannerparser.y"
    {
		(yyval.symbol) = csymbol_new (CSYMBOL_TYPE_INVALID);
	  }
    break;

  case 25:

/* Line 1464 of yacc.c  */
#line 390 "scannerparser.y"
    {
		(yyval.symbol) = csymbol_new (CSYMBOL_TYPE_INVALID);
	  }
    break;

  case 26:

/* Line 1464 of yacc.c  */
#line 394 "scannerparser.y"
    {
		switch ((yyvsp[(1) - (2)].unary_operator)) {
		case UNARY_PLUS:
			(yyval.symbol) = (yyvsp[(2) - (2)].symbol);
			break;
		case UNARY_MINUS:
			(yyval.symbol) = (yyvsp[(2) - (2)].symbol);
			(yyval.symbol)->const_int = -(yyvsp[(2) - (2)].symbol)->const_int;
			break;
		case UNARY_BITWISE_COMPLEMENT:
			(yyval.symbol) = (yyvsp[(2) - (2)].symbol);
			(yyval.symbol)->const_int = ~(yyvsp[(2) - (2)].symbol)->const_int;
			break;
		case UNARY_LOGICAL_NEGATION:
			(yyval.symbol) = (yyvsp[(2) - (2)].symbol);
			(yyval.symbol)->const_int = !csymbol_get_const_boolean ((yyvsp[(2) - (2)].symbol));
			break;
		default:
			(yyval.symbol) = csymbol_new (CSYMBOL_TYPE_INVALID);
			break;
		}
	  }
    break;

  case 27:

/* Line 1464 of yacc.c  */
#line 417 "scannerparser.y"
    {
		(yyval.symbol) = csymbol_new (CSYMBOL_TYPE_INVALID);
	  }
    break;

  case 28:

/* Line 1464 of yacc.c  */
#line 421 "scannerparser.y"
    {
		(yyval.symbol) = csymbol_new (CSYMBOL_TYPE_INVALID);
	  }
    break;

  case 29:

/* Line 1464 of yacc.c  */
#line 428 "scannerparser.y"
    {
		(yyval.unary_operator) = UNARY_ADDRESS_OF;
	  }
    break;

  case 30:

/* Line 1464 of yacc.c  */
#line 432 "scannerparser.y"
    {
		(yyval.unary_operator) = UNARY_POINTER_INDIRECTION;
	  }
    break;

  case 31:

/* Line 1464 of yacc.c  */
#line 436 "scannerparser.y"
    {
		(yyval.unary_operator) = UNARY_PLUS;
	  }
    break;

  case 32:

/* Line 1464 of yacc.c  */
#line 440 "scannerparser.y"
    {
		(yyval.unary_operator) = UNARY_MINUS;
	  }
    break;

  case 33:

/* Line 1464 of yacc.c  */
#line 444 "scannerparser.y"
    {
		(yyval.unary_operator) = UNARY_BITWISE_COMPLEMENT;
	  }
    break;

  case 34:

/* Line 1464 of yacc.c  */
#line 448 "scannerparser.y"
    {
		(yyval.unary_operator) = UNARY_LOGICAL_NEGATION;
	  }
    break;

  case 36:

/* Line 1464 of yacc.c  */
#line 456 "scannerparser.y"
    {
		(yyval.symbol) = (yyvsp[(4) - (4)].symbol);
	  }
    break;

  case 38:

/* Line 1464 of yacc.c  */
#line 464 "scannerparser.y"
    {
		(yyval.symbol) = csymbol_new (CSYMBOL_TYPE_CONST);
		(yyval.symbol)->const_int_set = TRUE;
		(yyval.symbol)->const_int = (yyvsp[(1) - (3)].symbol)->const_int * (yyvsp[(3) - (3)].symbol)->const_int;
	  }
    break;

  case 39:

/* Line 1464 of yacc.c  */
#line 470 "scannerparser.y"
    {
		(yyval.symbol) = csymbol_new (CSYMBOL_TYPE_CONST);
		(yyval.symbol)->const_int_set = TRUE;
		if ((yyvsp[(3) - (3)].symbol)->const_int != 0) {
			(yyval.symbol)->const_int = (yyvsp[(1) - (3)].symbol)->const_int / (yyvsp[(3) - (3)].symbol)->const_int;
		}
	  }
    break;

  case 40:

/* Line 1464 of yacc.c  */
#line 478 "scannerparser.y"
    {
		(yyval.symbol) = csymbol_new (CSYMBOL_TYPE_CONST);
		(yyval.symbol)->const_int_set = TRUE;
		(yyval.symbol)->const_int = (yyvsp[(1) - (3)].symbol)->const_int % (yyvsp[(3) - (3)].symbol)->const_int;
	  }
    break;

  case 42:

/* Line 1464 of yacc.c  */
#line 488 "scannerparser.y"
    {
		(yyval.symbol) = csymbol_new (CSYMBOL_TYPE_CONST);
		(yyval.symbol)->const_int_set = TRUE;
		(yyval.symbol)->const_int = (yyvsp[(1) - (3)].symbol)->const_int + (yyvsp[(3) - (3)].symbol)->const_int;
	  }
    break;

  case 43:

/* Line 1464 of yacc.c  */
#line 494 "scannerparser.y"
    {
		(yyval.symbol) = csymbol_new (CSYMBOL_TYPE_CONST);
		(yyval.symbol)->const_int_set = TRUE;
		(yyval.symbol)->const_int = (yyvsp[(1) - (3)].symbol)->const_int - (yyvsp[(3) - (3)].symbol)->const_int;
	  }
    break;

  case 45:

/* Line 1464 of yacc.c  */
#line 504 "scannerparser.y"
    {
		(yyval.symbol) = csymbol_new (CSYMBOL_TYPE_CONST);
		(yyval.symbol)->const_int_set = TRUE;
		(yyval.symbol)->const_int = (yyvsp[(1) - (3)].symbol)->const_int << (yyvsp[(3) - (3)].symbol)->const_int;
	  }
    break;

  case 46:

/* Line 1464 of yacc.c  */
#line 510 "scannerparser.y"
    {
		(yyval.symbol) = csymbol_new (CSYMBOL_TYPE_CONST);
		(yyval.symbol)->const_int_set = TRUE;
		(yyval.symbol)->const_int = (yyvsp[(1) - (3)].symbol)->const_int >> (yyvsp[(3) - (3)].symbol)->const_int;
	  }
    break;

  case 48:

/* Line 1464 of yacc.c  */
#line 520 "scannerparser.y"
    {
		(yyval.symbol) = csymbol_new (CSYMBOL_TYPE_CONST);
		(yyval.symbol)->const_int_set = TRUE;
		(yyval.symbol)->const_int = (yyvsp[(1) - (3)].symbol)->const_int < (yyvsp[(3) - (3)].symbol)->const_int;
	  }
    break;

  case 49:

/* Line 1464 of yacc.c  */
#line 526 "scannerparser.y"
    {
		(yyval.symbol) = csymbol_new (CSYMBOL_TYPE_CONST);
		(yyval.symbol)->const_int_set = TRUE;
		(yyval.symbol)->const_int = (yyvsp[(1) - (3)].symbol)->const_int > (yyvsp[(3) - (3)].symbol)->const_int;
	  }
    break;

  case 50:

/* Line 1464 of yacc.c  */
#line 532 "scannerparser.y"
    {
		(yyval.symbol) = csymbol_new (CSYMBOL_TYPE_CONST);
		(yyval.symbol)->const_int_set = TRUE;
		(yyval.symbol)->const_int = (yyvsp[(1) - (3)].symbol)->const_int <= (yyvsp[(3) - (3)].symbol)->const_int;
	  }
    break;

  case 51:

/* Line 1464 of yacc.c  */
#line 538 "scannerparser.y"
    {
		(yyval.symbol) = csymbol_new (CSYMBOL_TYPE_CONST);
		(yyval.symbol)->const_int_set = TRUE;
		(yyval.symbol)->const_int = (yyvsp[(1) - (3)].symbol)->const_int >= (yyvsp[(3) - (3)].symbol)->const_int;
	  }
    break;

  case 53:

/* Line 1464 of yacc.c  */
#line 548 "scannerparser.y"
    {
		(yyval.symbol) = csymbol_new (CSYMBOL_TYPE_CONST);
		(yyval.symbol)->const_int_set = TRUE;
		(yyval.symbol)->const_int = (yyvsp[(1) - (3)].symbol)->const_int == (yyvsp[(3) - (3)].symbol)->const_int;
	  }
    break;

  case 54:

/* Line 1464 of yacc.c  */
#line 554 "scannerparser.y"
    {
		(yyval.symbol) = csymbol_new (CSYMBOL_TYPE_CONST);
		(yyval.symbol)->const_int_set = TRUE;
		(yyval.symbol)->const_int = (yyvsp[(1) - (3)].symbol)->const_int != (yyvsp[(3) - (3)].symbol)->const_int;
	  }
    break;

  case 56:

/* Line 1464 of yacc.c  */
#line 564 "scannerparser.y"
    {
		(yyval.symbol) = csymbol_new (CSYMBOL_TYPE_CONST);
		(yyval.symbol)->const_int_set = TRUE;
		(yyval.symbol)->const_int = (yyvsp[(1) - (3)].symbol)->const_int & (yyvsp[(3) - (3)].symbol)->const_int;
	  }
    break;

  case 58:

/* Line 1464 of yacc.c  */
#line 574 "scannerparser.y"
    {
		(yyval.symbol) = csymbol_new (CSYMBOL_TYPE_CONST);
		(yyval.symbol)->const_int_set = TRUE;
		(yyval.symbol)->const_int = (yyvsp[(1) - (3)].symbol)->const_int ^ (yyvsp[(3) - (3)].symbol)->const_int;
	  }
    break;

  case 60:

/* Line 1464 of yacc.c  */
#line 584 "scannerparser.y"
    {
		(yyval.symbol) = csymbol_new (CSYMBOL_TYPE_CONST);
		(yyval.symbol)->const_int_set = TRUE;
		(yyval.symbol)->const_int = (yyvsp[(1) - (3)].symbol)->const_int | (yyvsp[(3) - (3)].symbol)->const_int;
	  }
    break;

  case 62:

/* Line 1464 of yacc.c  */
#line 594 "scannerparser.y"
    {
		(yyval.symbol) = csymbol_new (CSYMBOL_TYPE_CONST);
		(yyval.symbol)->const_int_set = TRUE;
		(yyval.symbol)->const_int = csymbol_get_const_boolean ((yyvsp[(1) - (3)].symbol)) && csymbol_get_const_boolean ((yyvsp[(3) - (3)].symbol));
	  }
    break;

  case 64:

/* Line 1464 of yacc.c  */
#line 604 "scannerparser.y"
    {
		(yyval.symbol) = csymbol_new (CSYMBOL_TYPE_CONST);
		(yyval.symbol)->const_int_set = TRUE;
		(yyval.symbol)->const_int = csymbol_get_const_boolean ((yyvsp[(1) - (3)].symbol)) || csymbol_get_const_boolean ((yyvsp[(3) - (3)].symbol));
	  }
    break;

  case 66:

/* Line 1464 of yacc.c  */
#line 614 "scannerparser.y"
    {
		(yyval.symbol) = csymbol_get_const_boolean ((yyvsp[(1) - (5)].symbol)) ? (yyvsp[(3) - (5)].symbol) : (yyvsp[(5) - (5)].symbol);
	  }
    break;

  case 68:

/* Line 1464 of yacc.c  */
#line 622 "scannerparser.y"
    {
		(yyval.symbol) = csymbol_new (CSYMBOL_TYPE_INVALID);
	  }
    break;

  case 81:

/* Line 1464 of yacc.c  */
#line 644 "scannerparser.y"
    {
		(yyval.symbol) = csymbol_new (CSYMBOL_TYPE_INVALID);
	  }
    break;

  case 83:

/* Line 1464 of yacc.c  */
#line 657 "scannerparser.y"
    {
		GList *l;
		for (l = (yyvsp[(2) - (3)].list); l != NULL; l = l->next) {
			CSymbol *sym = l->data;
			csymbol_merge_type (sym, (yyvsp[(1) - (3)].ctype));
			if ((yyvsp[(1) - (3)].ctype)->storage_class_specifier & STORAGE_CLASS_TYPEDEF) {
				sym->type = CSYMBOL_TYPE_TYPEDEF;
			} else if (sym->base_type->type == CTYPE_FUNCTION) {
				sym->type = CSYMBOL_TYPE_FUNCTION;
			} else {
				sym->type = CSYMBOL_TYPE_OBJECT;
			}
			g_igenerator_add_symbol (igenerator, sym);
		}
	  }
    break;

  case 85:

/* Line 1464 of yacc.c  */
#line 677 "scannerparser.y"
    {
		(yyval.ctype) = (yyvsp[(2) - (2)].ctype);
		(yyval.ctype)->storage_class_specifier |= (yyvsp[(1) - (2)].storage_class_specifier);
	  }
    break;

  case 86:

/* Line 1464 of yacc.c  */
#line 682 "scannerparser.y"
    {
		(yyval.ctype) = ctype_new (CTYPE_INVALID);
		(yyval.ctype)->storage_class_specifier |= (yyvsp[(1) - (1)].storage_class_specifier);
	  }
    break;

  case 87:

/* Line 1464 of yacc.c  */
#line 687 "scannerparser.y"
    {
		(yyval.ctype) = (yyvsp[(1) - (2)].ctype);
		(yyval.ctype)->base_type = (yyvsp[(2) - (2)].ctype);
	  }
    break;

  case 89:

/* Line 1464 of yacc.c  */
#line 693 "scannerparser.y"
    {
		(yyval.ctype) = (yyvsp[(2) - (2)].ctype);
		(yyval.ctype)->type_qualifier |= (yyvsp[(1) - (2)].type_qualifier);
	  }
    break;

  case 90:

/* Line 1464 of yacc.c  */
#line 698 "scannerparser.y"
    {
		(yyval.ctype) = ctype_new (CTYPE_INVALID);
		(yyval.ctype)->type_qualifier |= (yyvsp[(1) - (1)].type_qualifier);
	  }
    break;

  case 91:

/* Line 1464 of yacc.c  */
#line 703 "scannerparser.y"
    {
		(yyval.ctype) = (yyvsp[(2) - (2)].ctype);
		(yyval.ctype)->function_specifier |= (yyvsp[(1) - (2)].function_specifier);
	  }
    break;

  case 92:

/* Line 1464 of yacc.c  */
#line 708 "scannerparser.y"
    {
		(yyval.ctype) = ctype_new (CTYPE_INVALID);
		(yyval.ctype)->function_specifier |= (yyvsp[(1) - (1)].function_specifier);
	  }
    break;

  case 93:

/* Line 1464 of yacc.c  */
#line 716 "scannerparser.y"
    {
		(yyval.list) = g_list_append (NULL, (yyvsp[(1) - (1)].symbol));
	  }
    break;

  case 94:

/* Line 1464 of yacc.c  */
#line 720 "scannerparser.y"
    {
		(yyval.list) = g_list_append ((yyvsp[(1) - (3)].list), (yyvsp[(3) - (3)].symbol));
	  }
    break;

  case 97:

/* Line 1464 of yacc.c  */
#line 732 "scannerparser.y"
    {
		(yyval.storage_class_specifier) = STORAGE_CLASS_TYPEDEF;
	  }
    break;

  case 98:

/* Line 1464 of yacc.c  */
#line 736 "scannerparser.y"
    {
		(yyval.storage_class_specifier) = STORAGE_CLASS_EXTERN;
	  }
    break;

  case 99:

/* Line 1464 of yacc.c  */
#line 740 "scannerparser.y"
    {
		(yyval.storage_class_specifier) = STORAGE_CLASS_STATIC;
	  }
    break;

  case 100:

/* Line 1464 of yacc.c  */
#line 744 "scannerparser.y"
    {
		(yyval.storage_class_specifier) = STORAGE_CLASS_AUTO;
	  }
    break;

  case 101:

/* Line 1464 of yacc.c  */
#line 748 "scannerparser.y"
    {
		(yyval.storage_class_specifier) = STORAGE_CLASS_REGISTER;
	  }
    break;

  case 102:

/* Line 1464 of yacc.c  */
#line 755 "scannerparser.y"
    {
		(yyval.ctype) = ctype_new (CTYPE_VOID);
	  }
    break;

  case 103:

/* Line 1464 of yacc.c  */
#line 759 "scannerparser.y"
    {
		(yyval.ctype) = cbasic_type_new ("char");
	  }
    break;

  case 104:

/* Line 1464 of yacc.c  */
#line 763 "scannerparser.y"
    {
		(yyval.ctype) = cbasic_type_new ("short");
	  }
    break;

  case 105:

/* Line 1464 of yacc.c  */
#line 767 "scannerparser.y"
    {
		(yyval.ctype) = cbasic_type_new ("int");
	  }
    break;

  case 106:

/* Line 1464 of yacc.c  */
#line 771 "scannerparser.y"
    {
		(yyval.ctype) = cbasic_type_new ("long");
	  }
    break;

  case 107:

/* Line 1464 of yacc.c  */
#line 775 "scannerparser.y"
    {
		(yyval.ctype) = cbasic_type_new ("float");
	  }
    break;

  case 108:

/* Line 1464 of yacc.c  */
#line 779 "scannerparser.y"
    {
		(yyval.ctype) = cbasic_type_new ("double");
	  }
    break;

  case 109:

/* Line 1464 of yacc.c  */
#line 783 "scannerparser.y"
    {
		(yyval.ctype) = cbasic_type_new ("signed");
	  }
    break;

  case 110:

/* Line 1464 of yacc.c  */
#line 787 "scannerparser.y"
    {
		(yyval.ctype) = cbasic_type_new ("unsigned");
	  }
    break;

  case 111:

/* Line 1464 of yacc.c  */
#line 791 "scannerparser.y"
    {
		(yyval.ctype) = cbasic_type_new ("bool");
	  }
    break;

  case 114:

/* Line 1464 of yacc.c  */
#line 797 "scannerparser.y"
    {
		(yyval.ctype) = ctypedef_new ((yyvsp[(1) - (1)].str));
	  }
    break;

  case 115:

/* Line 1464 of yacc.c  */
#line 804 "scannerparser.y"
    {
		(yyval.ctype) = (yyvsp[(1) - (5)].ctype);
		(yyval.ctype)->name = (yyvsp[(2) - (5)].str);
		(yyval.ctype)->child_list = (yyvsp[(4) - (5)].list);

		CSymbol *sym = csymbol_new (CSYMBOL_TYPE_INVALID);
		if ((yyval.ctype)->type == CTYPE_STRUCT) {
			sym->type = CSYMBOL_TYPE_STRUCT;
		} else if ((yyval.ctype)->type == CTYPE_UNION) {
			sym->type = CSYMBOL_TYPE_UNION;
		} else {
			g_assert_not_reached ();
		}
		sym->ident = g_strdup ((yyval.ctype)->name);
		sym->base_type = ctype_copy ((yyval.ctype));
		g_igenerator_add_symbol (igenerator, sym);
	  }
    break;

  case 116:

/* Line 1464 of yacc.c  */
#line 822 "scannerparser.y"
    {
		(yyval.ctype) = (yyvsp[(1) - (4)].ctype);
		(yyval.ctype)->child_list = (yyvsp[(3) - (4)].list);
	  }
    break;

  case 117:

/* Line 1464 of yacc.c  */
#line 827 "scannerparser.y"
    {
		(yyval.ctype) = (yyvsp[(1) - (2)].ctype);
		(yyval.ctype)->name = (yyvsp[(2) - (2)].str);
	  }
    break;

  case 118:

/* Line 1464 of yacc.c  */
#line 835 "scannerparser.y"
    {
		(yyval.ctype) = cstruct_new (NULL);
	  }
    break;

  case 119:

/* Line 1464 of yacc.c  */
#line 839 "scannerparser.y"
    {
		(yyval.ctype) = cunion_new (NULL);
	  }
    break;

  case 121:

/* Line 1464 of yacc.c  */
#line 847 "scannerparser.y"
    {
		(yyval.list) = g_list_concat ((yyvsp[(1) - (2)].list), (yyvsp[(2) - (2)].list));
	  }
    break;

  case 122:

/* Line 1464 of yacc.c  */
#line 854 "scannerparser.y"
    {
		GList *l;
		(yyval.list) = NULL;
		for (l = (yyvsp[(2) - (3)].list); l != NULL; l = l->next) {
			CSymbol *sym = l->data;
			if ((yyvsp[(1) - (3)].ctype)->storage_class_specifier & STORAGE_CLASS_TYPEDEF) {
				sym->type = CSYMBOL_TYPE_TYPEDEF;
			}
			csymbol_merge_type (sym, (yyvsp[(1) - (3)].ctype));
			(yyval.list) = g_list_append ((yyval.list), sym);
		}
	  }
    break;

  case 123:

/* Line 1464 of yacc.c  */
#line 870 "scannerparser.y"
    {
		(yyval.ctype) = (yyvsp[(1) - (2)].ctype);
		(yyval.ctype)->base_type = (yyvsp[(2) - (2)].ctype);
	  }
    break;

  case 125:

/* Line 1464 of yacc.c  */
#line 876 "scannerparser.y"
    {
		(yyval.ctype) = (yyvsp[(2) - (2)].ctype);
		(yyval.ctype)->type_qualifier |= (yyvsp[(1) - (2)].type_qualifier);
	  }
    break;

  case 126:

/* Line 1464 of yacc.c  */
#line 881 "scannerparser.y"
    {
		(yyval.ctype) = ctype_new (CTYPE_INVALID);
		(yyval.ctype)->type_qualifier |= (yyvsp[(1) - (1)].type_qualifier);
	  }
    break;

  case 127:

/* Line 1464 of yacc.c  */
#line 889 "scannerparser.y"
    {
		(yyval.list) = g_list_append (NULL, (yyvsp[(1) - (1)].symbol));
	  }
    break;

  case 128:

/* Line 1464 of yacc.c  */
#line 893 "scannerparser.y"
    {
		(yyval.list) = g_list_append ((yyvsp[(1) - (3)].list), (yyvsp[(3) - (3)].symbol));
	  }
    break;

  case 129:

/* Line 1464 of yacc.c  */
#line 900 "scannerparser.y"
    {
		(yyval.symbol) = csymbol_new (CSYMBOL_TYPE_INVALID);
	  }
    break;

  case 131:

/* Line 1464 of yacc.c  */
#line 905 "scannerparser.y"
    {
		(yyval.symbol) = csymbol_new (CSYMBOL_TYPE_INVALID);
	  }
    break;

  case 133:

/* Line 1464 of yacc.c  */
#line 913 "scannerparser.y"
    {
		(yyval.ctype) = cenum_new ((yyvsp[(2) - (5)].str));
		(yyval.ctype)->child_list = (yyvsp[(4) - (5)].list);
		last_enum_value = -1;
	  }
    break;

  case 134:

/* Line 1464 of yacc.c  */
#line 919 "scannerparser.y"
    {
		(yyval.ctype) = cenum_new (NULL);
		(yyval.ctype)->child_list = (yyvsp[(3) - (4)].list);
		last_enum_value = -1;
	  }
    break;

  case 135:

/* Line 1464 of yacc.c  */
#line 925 "scannerparser.y"
    {
		(yyval.ctype) = cenum_new ((yyvsp[(2) - (6)].str));
		(yyval.ctype)->child_list = (yyvsp[(4) - (6)].list);
		last_enum_value = -1;
	  }
    break;

  case 136:

/* Line 1464 of yacc.c  */
#line 931 "scannerparser.y"
    {
		(yyval.ctype) = cenum_new (NULL);
		(yyval.ctype)->child_list = (yyvsp[(3) - (5)].list);
		last_enum_value = -1;
	  }
    break;

  case 137:

/* Line 1464 of yacc.c  */
#line 937 "scannerparser.y"
    {
		(yyval.ctype) = cenum_new ((yyvsp[(2) - (2)].str));
	  }
    break;

  case 138:

/* Line 1464 of yacc.c  */
#line 944 "scannerparser.y"
    {
		(yyval.list) = g_list_append (NULL, (yyvsp[(1) - (1)].symbol));
	  }
    break;

  case 139:

/* Line 1464 of yacc.c  */
#line 948 "scannerparser.y"
    {
		(yyval.list) = g_list_append ((yyvsp[(1) - (3)].list), (yyvsp[(3) - (3)].symbol));
	  }
    break;

  case 140:

/* Line 1464 of yacc.c  */
#line 955 "scannerparser.y"
    {
		(yyval.symbol) = csymbol_new (CSYMBOL_TYPE_OBJECT);
		(yyval.symbol)->ident = (yyvsp[(1) - (1)].str);
		(yyval.symbol)->const_int_set = TRUE;
		(yyval.symbol)->const_int = ++last_enum_value;
		g_hash_table_insert (const_table, g_strdup ((yyval.symbol)->ident), (yyval.symbol));
	  }
    break;

  case 141:

/* Line 1464 of yacc.c  */
#line 963 "scannerparser.y"
    {
		(yyval.symbol) = csymbol_new (CSYMBOL_TYPE_OBJECT);
		(yyval.symbol)->ident = (yyvsp[(1) - (3)].str);
		(yyval.symbol)->const_int_set = TRUE;
		(yyval.symbol)->const_int = (yyvsp[(3) - (3)].symbol)->const_int;
		last_enum_value = (yyval.symbol)->const_int;
		g_hash_table_insert (const_table, g_strdup ((yyval.symbol)->ident), (yyval.symbol));
	  }
    break;

  case 142:

/* Line 1464 of yacc.c  */
#line 975 "scannerparser.y"
    {
		(yyval.type_qualifier) = TYPE_QUALIFIER_CONST;
	  }
    break;

  case 143:

/* Line 1464 of yacc.c  */
#line 979 "scannerparser.y"
    {
		(yyval.type_qualifier) = TYPE_QUALIFIER_RESTRICT;
	  }
    break;

  case 144:

/* Line 1464 of yacc.c  */
#line 983 "scannerparser.y"
    {
		(yyval.type_qualifier) = TYPE_QUALIFIER_VOLATILE;
	  }
    break;

  case 145:

/* Line 1464 of yacc.c  */
#line 990 "scannerparser.y"
    {
		(yyval.function_specifier) = FUNCTION_INLINE;
	  }
    break;

  case 146:

/* Line 1464 of yacc.c  */
#line 997 "scannerparser.y"
    {
		(yyval.symbol) = (yyvsp[(2) - (2)].symbol);
		csymbol_merge_type ((yyval.symbol), (yyvsp[(1) - (2)].ctype));
	  }
    break;

  case 148:

/* Line 1464 of yacc.c  */
#line 1006 "scannerparser.y"
    {
		(yyval.symbol) = csymbol_new (CSYMBOL_TYPE_INVALID);
		(yyval.symbol)->ident = (yyvsp[(1) - (1)].str);
	  }
    break;

  case 149:

/* Line 1464 of yacc.c  */
#line 1011 "scannerparser.y"
    {
		(yyval.symbol) = (yyvsp[(2) - (3)].symbol);
	  }
    break;

  case 150:

/* Line 1464 of yacc.c  */
#line 1015 "scannerparser.y"
    {
		(yyval.symbol) = (yyvsp[(1) - (4)].symbol);
		csymbol_merge_type ((yyval.symbol), carray_new ());
	  }
    break;

  case 151:

/* Line 1464 of yacc.c  */
#line 1020 "scannerparser.y"
    {
		(yyval.symbol) = (yyvsp[(1) - (3)].symbol);
		csymbol_merge_type ((yyval.symbol), carray_new ());
	  }
    break;

  case 152:

/* Line 1464 of yacc.c  */
#line 1025 "scannerparser.y"
    {
		CType *func = cfunction_new ();
		// ignore (void) parameter list
		if ((yyvsp[(3) - (4)].list) != NULL && ((yyvsp[(3) - (4)].list)->next != NULL || ((CSymbol *) (yyvsp[(3) - (4)].list)->data)->base_type->type != CTYPE_VOID)) {
			func->child_list = (yyvsp[(3) - (4)].list);
		}
		(yyval.symbol) = (yyvsp[(1) - (4)].symbol);
		csymbol_merge_type ((yyval.symbol), func);
	  }
    break;

  case 153:

/* Line 1464 of yacc.c  */
#line 1035 "scannerparser.y"
    {
		CType *func = cfunction_new ();
		func->child_list = (yyvsp[(3) - (4)].list);
		(yyval.symbol) = (yyvsp[(1) - (4)].symbol);
		csymbol_merge_type ((yyval.symbol), func);
	  }
    break;

  case 154:

/* Line 1464 of yacc.c  */
#line 1042 "scannerparser.y"
    {
		CType *func = cfunction_new ();
		(yyval.symbol) = (yyvsp[(1) - (3)].symbol);
		csymbol_merge_type ((yyval.symbol), func);
	  }
    break;

  case 155:

/* Line 1464 of yacc.c  */
#line 1051 "scannerparser.y"
    {
		(yyval.ctype) = cpointer_new (NULL);
		(yyval.ctype)->type_qualifier = (yyvsp[(2) - (2)].type_qualifier);
	  }
    break;

  case 156:

/* Line 1464 of yacc.c  */
#line 1056 "scannerparser.y"
    {
		(yyval.ctype) = cpointer_new (NULL);
	  }
    break;

  case 157:

/* Line 1464 of yacc.c  */
#line 1060 "scannerparser.y"
    {
		(yyval.ctype) = cpointer_new ((yyvsp[(3) - (3)].ctype));
		(yyval.ctype)->type_qualifier = (yyvsp[(2) - (3)].type_qualifier);
	  }
    break;

  case 158:

/* Line 1464 of yacc.c  */
#line 1065 "scannerparser.y"
    {
		(yyval.ctype) = cpointer_new ((yyvsp[(2) - (2)].ctype));
	  }
    break;

  case 160:

/* Line 1464 of yacc.c  */
#line 1073 "scannerparser.y"
    {
		(yyval.type_qualifier) = (yyvsp[(1) - (2)].type_qualifier) | (yyvsp[(2) - (2)].type_qualifier);
	  }
    break;

  case 163:

/* Line 1464 of yacc.c  */
#line 1085 "scannerparser.y"
    {
		(yyval.list) = g_list_append (NULL, (yyvsp[(1) - (1)].symbol));
	  }
    break;

  case 164:

/* Line 1464 of yacc.c  */
#line 1089 "scannerparser.y"
    {
		(yyval.list) = g_list_append ((yyvsp[(1) - (3)].list), (yyvsp[(3) - (3)].symbol));
	  }
    break;

  case 165:

/* Line 1464 of yacc.c  */
#line 1096 "scannerparser.y"
    {
		(yyval.symbol) = (yyvsp[(2) - (2)].symbol);
		csymbol_merge_type ((yyval.symbol), (yyvsp[(1) - (2)].ctype));
	  }
    break;

  case 166:

/* Line 1464 of yacc.c  */
#line 1101 "scannerparser.y"
    {
		(yyval.symbol) = (yyvsp[(2) - (2)].symbol);
		csymbol_merge_type ((yyval.symbol), (yyvsp[(1) - (2)].ctype));
	  }
    break;

  case 167:

/* Line 1464 of yacc.c  */
#line 1106 "scannerparser.y"
    {
		(yyval.symbol) = csymbol_new (CSYMBOL_TYPE_INVALID);
		(yyval.symbol)->base_type = (yyvsp[(1) - (1)].ctype);
	  }
    break;

  case 168:

/* Line 1464 of yacc.c  */
#line 1114 "scannerparser.y"
    {
		CSymbol *sym = csymbol_new (CSYMBOL_TYPE_INVALID);
		sym->ident = (yyvsp[(1) - (1)].str);
		(yyval.list) = g_list_append (NULL, sym);
	  }
    break;

  case 169:

/* Line 1464 of yacc.c  */
#line 1120 "scannerparser.y"
    {
		CSymbol *sym = csymbol_new (CSYMBOL_TYPE_INVALID);
		sym->ident = (yyvsp[(3) - (3)].str);
		(yyval.list) = g_list_append ((yyvsp[(1) - (3)].list), sym);
	  }
    break;

  case 172:

/* Line 1464 of yacc.c  */
#line 1134 "scannerparser.y"
    {
		(yyval.symbol) = csymbol_new (CSYMBOL_TYPE_INVALID);
		csymbol_merge_type ((yyval.symbol), (yyvsp[(1) - (1)].ctype));
	  }
    break;

  case 174:

/* Line 1464 of yacc.c  */
#line 1140 "scannerparser.y"
    {
		(yyval.symbol) = (yyvsp[(2) - (2)].symbol);
		csymbol_merge_type ((yyval.symbol), (yyvsp[(1) - (2)].ctype));
	  }
    break;

  case 175:

/* Line 1464 of yacc.c  */
#line 1148 "scannerparser.y"
    {
		(yyval.symbol) = (yyvsp[(2) - (3)].symbol);
	  }
    break;

  case 176:

/* Line 1464 of yacc.c  */
#line 1152 "scannerparser.y"
    {
		(yyval.symbol) = csymbol_new (CSYMBOL_TYPE_INVALID);
		csymbol_merge_type ((yyval.symbol), carray_new ());
	  }
    break;

  case 177:

/* Line 1464 of yacc.c  */
#line 1157 "scannerparser.y"
    {
		(yyval.symbol) = csymbol_new (CSYMBOL_TYPE_INVALID);
		csymbol_merge_type ((yyval.symbol), carray_new ());
	  }
    break;

  case 178:

/* Line 1464 of yacc.c  */
#line 1162 "scannerparser.y"
    {
		(yyval.symbol) = (yyvsp[(1) - (3)].symbol);
		csymbol_merge_type ((yyval.symbol), carray_new ());
	  }
    break;

  case 179:

/* Line 1464 of yacc.c  */
#line 1167 "scannerparser.y"
    {
		(yyval.symbol) = (yyvsp[(1) - (4)].symbol);
		csymbol_merge_type ((yyval.symbol), carray_new ());
	  }
    break;

  case 180:

/* Line 1464 of yacc.c  */
#line 1172 "scannerparser.y"
    {
		CType *func = cfunction_new ();
		(yyval.symbol) = csymbol_new (CSYMBOL_TYPE_INVALID);
		csymbol_merge_type ((yyval.symbol), func);
	  }
    break;

  case 181:

/* Line 1464 of yacc.c  */
#line 1178 "scannerparser.y"
    {
		CType *func = cfunction_new ();
		// ignore (void) parameter list
		if ((yyvsp[(2) - (3)].list) != NULL && ((yyvsp[(2) - (3)].list)->next != NULL || ((CSymbol *) (yyvsp[(2) - (3)].list)->data)->base_type->type != CTYPE_VOID)) {
			func->child_list = (yyvsp[(2) - (3)].list);
		}
		(yyval.symbol) = csymbol_new (CSYMBOL_TYPE_INVALID);
		csymbol_merge_type ((yyval.symbol), func);
	  }
    break;

  case 182:

/* Line 1464 of yacc.c  */
#line 1188 "scannerparser.y"
    {
		CType *func = cfunction_new ();
		(yyval.symbol) = (yyvsp[(1) - (3)].symbol);
		csymbol_merge_type ((yyval.symbol), func);
	  }
    break;

  case 183:

/* Line 1464 of yacc.c  */
#line 1194 "scannerparser.y"
    {
		CType *func = cfunction_new ();
		// ignore (void) parameter list
		if ((yyvsp[(3) - (4)].list) != NULL && ((yyvsp[(3) - (4)].list)->next != NULL || ((CSymbol *) (yyvsp[(3) - (4)].list)->data)->base_type->type != CTYPE_VOID)) {
			func->child_list = (yyvsp[(3) - (4)].list);
		}
		(yyval.symbol) = (yyvsp[(1) - (4)].symbol);
		csymbol_merge_type ((yyval.symbol), func);
	  }
    break;

  case 184:

/* Line 1464 of yacc.c  */
#line 1207 "scannerparser.y"
    {
		(yyval.str) = g_strdup (yytext);
	  }
    break;

  case 234:

/* Line 1464 of yacc.c  */
#line 1314 "scannerparser.y"
    {
		(yyval.str) = g_strdup (yytext + strlen ("#define "));
	  }
    break;

  case 235:

/* Line 1464 of yacc.c  */
#line 1321 "scannerparser.y"
    {
		(yyval.str) = g_strdup (yytext + strlen ("#define "));
	  }
    break;

  case 237:

/* Line 1464 of yacc.c  */
#line 1332 "scannerparser.y"
    {
		if ((yyvsp[(2) - (2)].symbol)->const_int_set || (yyvsp[(2) - (2)].symbol)->const_string != NULL) {
			(yyvsp[(2) - (2)].symbol)->ident = (yyvsp[(1) - (2)].str);
			g_igenerator_add_symbol (igenerator, (yyvsp[(2) - (2)].symbol));
		}
	  }
    break;



/* Line 1464 of yacc.c  */
#line 3750 "scannerparser.c"
      default: break;
    }
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
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (igenerator, YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (igenerator, yymsg);
	  }
	else
	  {
	    yyerror (igenerator, YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
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
		      yytoken, &yylval, igenerator);
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
      if (yyn != YYPACT_NINF)
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
		  yystos[yystate], yyvsp, igenerator);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  *++yyvsp = yylval;


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

#if !defined(yyoverflow) || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (igenerator, YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval, igenerator);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp, igenerator);
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



/* Line 1684 of yacc.c  */
#line 1346 "scannerparser.y"


static void
yyerror (GIGenerator *igenerator, const char *s)
{
  /* ignore errors while doing a macro scan as not all object macros
   * have valid expressions */
  if (!igenerator->macro_scan)
    {
      fprintf(stderr, "%s:%d: %s\n",
	      igenerator->current_filename, lineno, s);
    }
}

gboolean
g_igenerator_parse_file (GIGenerator *igenerator, FILE *file)
{
  g_return_val_if_fail (file != NULL, FALSE);
  
  const_table = g_hash_table_new_full (g_str_hash, g_str_equal,
				       g_free, NULL);
  
  lineno = 1;
  yyin = file;
  yyparse (igenerator);
  
  g_hash_table_destroy (const_table);
  const_table = NULL;
  
  yyin = NULL;

  return TRUE;
}



