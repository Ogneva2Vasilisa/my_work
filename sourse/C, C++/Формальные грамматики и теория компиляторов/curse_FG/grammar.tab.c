/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "grammar.y"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

extern int yylex();
extern int yyparse();
extern FILE *yyin;
extern int yylineno;
extern char *yytext;
void yyerror(const char *s);

#line 85 "grammar.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "grammar.tab.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_NUMBER = 3,                     /* NUMBER  */
  YYSYMBOL_HEX_NUMBER = 4,                 /* HEX_NUMBER  */
  YYSYMBOL_FLOAT = 5,                      /* FLOAT  */
  YYSYMBOL_IDENTIFIER = 6,                 /* IDENTIFIER  */
  YYSYMBOL_STRING = 7,                     /* STRING  */
  YYSYMBOL_CHAR = 8,                       /* CHAR  */
  YYSYMBOL_IF = 9,                         /* IF  */
  YYSYMBOL_ELSE = 10,                      /* ELSE  */
  YYSYMBOL_WHILE = 11,                     /* WHILE  */
  YYSYMBOL_FOR = 12,                       /* FOR  */
  YYSYMBOL_RETURN = 13,                    /* RETURN  */
  YYSYMBOL_BREAK = 14,                     /* BREAK  */
  YYSYMBOL_DO = 15,                        /* DO  */
  YYSYMBOL_SWITCH = 16,                    /* SWITCH  */
  YYSYMBOL_CASE = 17,                      /* CASE  */
  YYSYMBOL_DEFAULT = 18,                   /* DEFAULT  */
  YYSYMBOL_COLON = 19,                     /* COLON  */
  YYSYMBOL_TYPE = 20,                      /* TYPE  */
  YYSYMBOL_STRUCT = 21,                    /* STRUCT  */
  YYSYMBOL_UNION = 22,                     /* UNION  */
  YYSYMBOL_ENUM = 23,                      /* ENUM  */
  YYSYMBOL_LONG = 24,                      /* LONG  */
  YYSYMBOL_SHORT = 25,                     /* SHORT  */
  YYSYMBOL_UNSIGNED = 26,                  /* UNSIGNED  */
  YYSYMBOL_SIGNED = 27,                    /* SIGNED  */
  YYSYMBOL_FILE_TYPE = 28,                 /* FILE_TYPE  */
  YYSYMBOL_NULLVAL = 29,                   /* NULLVAL  */
  YYSYMBOL_EOFVAL = 30,                    /* EOFVAL  */
  YYSYMBOL_TRUE_VAL = 31,                  /* TRUE_VAL  */
  YYSYMBOL_FALSE_VAL = 32,                 /* FALSE_VAL  */
  YYSYMBOL_SIZEOF = 33,                    /* SIZEOF  */
  YYSYMBOL_TYPEDEF = 34,                   /* TYPEDEF  */
  YYSYMBOL_LONG_LONG = 35,                 /* LONG_LONG  */
  YYSYMBOL_UNSIGNED_LONG_LONG = 36,        /* UNSIGNED_LONG_LONG  */
  YYSYMBOL_SIGNED_LONG_LONG = 37,          /* SIGNED_LONG_LONG  */
  YYSYMBOL_UNSIGNED_SHORT = 38,            /* UNSIGNED_SHORT  */
  YYSYMBOL_SIGNED_SHORT = 39,              /* SIGNED_SHORT  */
  YYSYMBOL_UNSIGNED_LONG = 40,             /* UNSIGNED_LONG  */
  YYSYMBOL_SIGNED_LONG = 41,               /* SIGNED_LONG  */
  YYSYMBOL_UNSIGNED_TYPE = 42,             /* UNSIGNED_TYPE  */
  YYSYMBOL_SIGNED_TYPE = 43,               /* SIGNED_TYPE  */
  YYSYMBOL_LONG_DOUBLE = 44,               /* LONG_DOUBLE  */
  YYSYMBOL_STATIC = 45,                    /* STATIC  */
  YYSYMBOL_PLUS = 46,                      /* PLUS  */
  YYSYMBOL_MINUS = 47,                     /* MINUS  */
  YYSYMBOL_MULTIPLY = 48,                  /* MULTIPLY  */
  YYSYMBOL_DIVIDE = 49,                    /* DIVIDE  */
  YYSYMBOL_ASSIGN = 50,                    /* ASSIGN  */
  YYSYMBOL_SEMICOLON = 51,                 /* SEMICOLON  */
  YYSYMBOL_COMMA = 52,                     /* COMMA  */
  YYSYMBOL_LPAREN = 53,                    /* LPAREN  */
  YYSYMBOL_RPAREN = 54,                    /* RPAREN  */
  YYSYMBOL_LBRACE = 55,                    /* LBRACE  */
  YYSYMBOL_RBRACE = 56,                    /* RBRACE  */
  YYSYMBOL_LBRACKET = 57,                  /* LBRACKET  */
  YYSYMBOL_RBRACKET = 58,                  /* RBRACKET  */
  YYSYMBOL_GREATER = 59,                   /* GREATER  */
  YYSYMBOL_LESS = 60,                      /* LESS  */
  YYSYMBOL_EQUAL = 61,                     /* EQUAL  */
  YYSYMBOL_NEQUAL = 62,                    /* NEQUAL  */
  YYSYMBOL_AND = 63,                       /* AND  */
  YYSYMBOL_OR = 64,                        /* OR  */
  YYSYMBOL_NOT = 65,                       /* NOT  */
  YYSYMBOL_AMPERSAND = 66,                 /* AMPERSAND  */
  YYSYMBOL_PIPE = 67,                      /* PIPE  */
  YYSYMBOL_CARET = 68,                     /* CARET  */
  YYSYMBOL_MODULO = 69,                    /* MODULO  */
  YYSYMBOL_INCREMENT = 70,                 /* INCREMENT  */
  YYSYMBOL_DECREMENT = 71,                 /* DECREMENT  */
  YYSYMBOL_DOT = 72,                       /* DOT  */
  YYSYMBOL_ARROW = 73,                     /* ARROW  */
  YYSYMBOL_PLUS_ASSIGN = 74,               /* PLUS_ASSIGN  */
  YYSYMBOL_MINUS_ASSIGN = 75,              /* MINUS_ASSIGN  */
  YYSYMBOL_MULTIPLY_ASSIGN = 76,           /* MULTIPLY_ASSIGN  */
  YYSYMBOL_DIVIDE_ASSIGN = 77,             /* DIVIDE_ASSIGN  */
  YYSYMBOL_MODULO_ASSIGN = 78,             /* MODULO_ASSIGN  */
  YYSYMBOL_GE_OP = 79,                     /* GE_OP  */
  YYSYMBOL_LE_OP = 80,                     /* LE_OP  */
  YYSYMBOL_LEFT_SHIFT = 81,                /* LEFT_SHIFT  */
  YYSYMBOL_RIGHT_SHIFT = 82,               /* RIGHT_SHIFT  */
  YYSYMBOL_LEFT_SHIFT_ASSIGN = 83,         /* LEFT_SHIFT_ASSIGN  */
  YYSYMBOL_RIGHT_SHIFT_ASSIGN = 84,        /* RIGHT_SHIFT_ASSIGN  */
  YYSYMBOL_YYACCEPT = 85,                  /* $accept  */
  YYSYMBOL_program = 86,                   /* program  */
  YYSYMBOL_external_declaration = 87,      /* external_declaration  */
  YYSYMBOL_function_definition = 88,       /* function_definition  */
  YYSYMBOL_main_function = 89,             /* main_function  */
  YYSYMBOL_point = 90,                     /* point  */
  YYSYMBOL_typedef_declaration = 91,       /* typedef_declaration  */
  YYSYMBOL_struct_declaration = 92,        /* struct_declaration  */
  YYSYMBOL_union_declaration = 93,         /* union_declaration  */
  YYSYMBOL_union_variable_declaration = 94, /* union_variable_declaration  */
  YYSYMBOL_union_declarator_list = 95,     /* union_declarator_list  */
  YYSYMBOL_union_declarator = 96,          /* union_declarator  */
  YYSYMBOL_enum_declaration = 97,          /* enum_declaration  */
  YYSYMBOL_enum_list = 98,                 /* enum_list  */
  YYSYMBOL_enum_item = 99,                 /* enum_item  */
  YYSYMBOL_struct_member_list = 100,       /* struct_member_list  */
  YYSYMBOL_struct_member = 101,            /* struct_member  */
  YYSYMBOL_bitfield_list = 102,            /* bitfield_list  */
  YYSYMBOL_bitfield = 103,                 /* bitfield  */
  YYSYMBOL_type = 104,                     /* type  */
  YYSYMBOL_cast_type = 105,                /* cast_type  */
  YYSYMBOL_opt_type = 106,                 /* opt_type  */
  YYSYMBOL_pointer_opt = 107,              /* pointer_opt  */
  YYSYMBOL_pointer = 108,                  /* pointer  */
  YYSYMBOL_amper = 109,                    /* amper  */
  YYSYMBOL_parameter_list = 110,           /* parameter_list  */
  YYSYMBOL_parameter_declaration = 111,    /* parameter_declaration  */
  YYSYMBOL_abstract_declarator = 112,      /* abstract_declarator  */
  YYSYMBOL_direct_abstract_declarator = 113, /* direct_abstract_declarator  */
  YYSYMBOL_declarator = 114,               /* declarator  */
  YYSYMBOL_direct_declarator = 115,        /* direct_declarator  */
  YYSYMBOL_declaration = 116,              /* declaration  */
  YYSYMBOL_init_declarator_list = 117,     /* init_declarator_list  */
  YYSYMBOL_init_declarator = 118,          /* init_declarator  */
  YYSYMBOL_initializer = 119,              /* initializer  */
  YYSYMBOL_initializer_list = 120,         /* initializer_list  */
  YYSYMBOL_designator_list = 121,          /* designator_list  */
  YYSYMBOL_designator = 122,               /* designator  */
  YYSYMBOL_compound_statement = 123,       /* compound_statement  */
  YYSYMBOL_statement_list = 124,           /* statement_list  */
  YYSYMBOL_statement = 125,                /* statement  */
  YYSYMBOL_declaration_statement = 126,    /* declaration_statement  */
  YYSYMBOL_expression_statement = 127,     /* expression_statement  */
  YYSYMBOL_jump_statement = 128,           /* jump_statement  */
  YYSYMBOL_do_while_statement = 129,       /* do_while_statement  */
  YYSYMBOL_if_statement = 130,             /* if_statement  */
  YYSYMBOL_if_state = 131,                 /* if_state  */
  YYSYMBOL_elif1 = 132,                    /* elif1  */
  YYSYMBOL_elif = 133,                     /* elif  */
  YYSYMBOL_while_statement = 134,          /* while_statement  */
  YYSYMBOL_for_statement = 135,            /* for_statement  */
  YYSYMBOL_switch_statement = 136,         /* switch_statement  */
  YYSYMBOL_case_list = 137,                /* case_list  */
  YYSYMBOL_case_statement = 138,           /* case_statement  */
  YYSYMBOL_statement_list1 = 139,          /* statement_list1  */
  YYSYMBOL_statement1 = 140,               /* statement1  */
  YYSYMBOL_constant_expression = 141,      /* constant_expression  */
  YYSYMBOL_expression = 142,               /* expression  */
  YYSYMBOL_assignment_expression = 143,    /* assignment_expression  */
  YYSYMBOL_conditional_expression = 144,   /* conditional_expression  */
  YYSYMBOL_logical_or_expression = 145,    /* logical_or_expression  */
  YYSYMBOL_inclusive_or_expression = 146,  /* inclusive_or_expression  */
  YYSYMBOL_exclusive_or_expression = 147,  /* exclusive_or_expression  */
  YYSYMBOL_and_expression = 148,           /* and_expression  */
  YYSYMBOL_relational_expression = 149,    /* relational_expression  */
  YYSYMBOL_shift_expression = 150,         /* shift_expression  */
  YYSYMBOL_additive_expression = 151,      /* additive_expression  */
  YYSYMBOL_multiplicative_expression = 152, /* multiplicative_expression  */
  YYSYMBOL_cast_expression = 153,          /* cast_expression  */
  YYSYMBOL_unary_expression = 154,         /* unary_expression  */
  YYSYMBOL_unary_operator = 155,           /* unary_operator  */
  YYSYMBOL_function_call = 156,            /* function_call  */
  YYSYMBOL_postfix_expression = 157,       /* postfix_expression  */
  YYSYMBOL_primary_expression = 158,       /* primary_expression  */
  YYSYMBOL_constant = 159,                 /* constant  */
  YYSYMBOL_argument_list = 160,            /* argument_list  */
  YYSYMBOL_non_empty_argument_list = 161,  /* non_empty_argument_list  */
  YYSYMBOL_op_exp = 162,                   /* op_exp  */
  YYSYMBOL_for_declaration = 163,          /* for_declaration  */
  YYSYMBOL_semicolons = 164                /* semicolons  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_int16 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

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


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

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
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
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
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
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
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  71
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   2219

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  85
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  80
/* YYNRULES -- Number of rules.  */
#define YYNRULES  275
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  516

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   339


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
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
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,    75,    75,    76,    81,    82,    83,    84,    85,    86,
      87,    93,    94,    95,    96,    97,    98,   102,   107,   108,
     114,   115,   116,   122,   123,   124,   128,   129,   130,   131,
     135,   139,   140,   144,   145,   146,   150,   151,   152,   153,
     156,   158,   159,   163,   164,   169,   170,   174,   175,   176,
     177,   178,   182,   183,   187,   188,   194,   195,   196,   200,
     201,   202,   203,   204,   205,   206,   207,   208,   209,   210,
     211,   212,   213,   214,   215,   216,   217,   218,   219,   220,
     221,   222,   223,   224,   227,   229,   232,   234,   238,   239,
     243,   244,   250,   251,   255,   256,   257,   258,   259,   263,
     264,   265,   266,   270,   271,   272,   273,   274,   275,   276,
     277,   278,   282,   283,   284,   288,   289,   290,   291,   292,
     293,   294,   300,   304,   305,   309,   310,   311,   315,   316,
     317,   318,   319,   322,   324,   325,   326,   327,   331,   332,
     336,   337,   343,   347,   348,   352,   353,   354,   355,   356,
     357,   358,   359,   360,   361,   362,   363,   364,   365,   369,
     370,   374,   378,   379,   380,   381,   387,   391,   394,   396,
     397,   398,   402,   403,   407,   411,   415,   419,   422,   424,
     425,   429,   430,   431,   434,   436,   437,   441,   442,   448,
     452,   453,   454,   458,   459,   460,   461,   462,   463,   464,
     465,   466,   470,   474,   475,   476,   480,   481,   482,   483,
     487,   491,   492,   493,   497,   498,   499,   500,   501,   505,
     506,   507,   511,   512,   513,   517,   518,   519,   520,   524,
     525,   529,   530,   531,   532,   536,   536,   536,   536,   536,
     536,   536,   540,   544,   545,   546,   547,   548,   549,   550,
     551,   555,   556,   557,   558,   559,   560,   564,   565,   566,
     567,   568,   569,   570,   571,   576,   578,   582,   583,   586,
     588,   591,   593,   594,   599,   600
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "NUMBER", "HEX_NUMBER",
  "FLOAT", "IDENTIFIER", "STRING", "CHAR", "IF", "ELSE", "WHILE", "FOR",
  "RETURN", "BREAK", "DO", "SWITCH", "CASE", "DEFAULT", "COLON", "TYPE",
  "STRUCT", "UNION", "ENUM", "LONG", "SHORT", "UNSIGNED", "SIGNED",
  "FILE_TYPE", "NULLVAL", "EOFVAL", "TRUE_VAL", "FALSE_VAL", "SIZEOF",
  "TYPEDEF", "LONG_LONG", "UNSIGNED_LONG_LONG", "SIGNED_LONG_LONG",
  "UNSIGNED_SHORT", "SIGNED_SHORT", "UNSIGNED_LONG", "SIGNED_LONG",
  "UNSIGNED_TYPE", "SIGNED_TYPE", "LONG_DOUBLE", "STATIC", "PLUS", "MINUS",
  "MULTIPLY", "DIVIDE", "ASSIGN", "SEMICOLON", "COMMA", "LPAREN", "RPAREN",
  "LBRACE", "RBRACE", "LBRACKET", "RBRACKET", "GREATER", "LESS", "EQUAL",
  "NEQUAL", "AND", "OR", "NOT", "AMPERSAND", "PIPE", "CARET", "MODULO",
  "INCREMENT", "DECREMENT", "DOT", "ARROW", "PLUS_ASSIGN", "MINUS_ASSIGN",
  "MULTIPLY_ASSIGN", "DIVIDE_ASSIGN", "MODULO_ASSIGN", "GE_OP", "LE_OP",
  "LEFT_SHIFT", "RIGHT_SHIFT", "LEFT_SHIFT_ASSIGN", "RIGHT_SHIFT_ASSIGN",
  "$accept", "program", "external_declaration", "function_definition",
  "main_function", "point", "typedef_declaration", "struct_declaration",
  "union_declaration", "union_variable_declaration",
  "union_declarator_list", "union_declarator", "enum_declaration",
  "enum_list", "enum_item", "struct_member_list", "struct_member",
  "bitfield_list", "bitfield", "type", "cast_type", "opt_type",
  "pointer_opt", "pointer", "amper", "parameter_list",
  "parameter_declaration", "abstract_declarator",
  "direct_abstract_declarator", "declarator", "direct_declarator",
  "declaration", "init_declarator_list", "init_declarator", "initializer",
  "initializer_list", "designator_list", "designator",
  "compound_statement", "statement_list", "statement",
  "declaration_statement", "expression_statement", "jump_statement",
  "do_while_statement", "if_statement", "if_state", "elif1", "elif",
  "while_statement", "for_statement", "switch_statement", "case_list",
  "case_statement", "statement_list1", "statement1", "constant_expression",
  "expression", "assignment_expression", "conditional_expression",
  "logical_or_expression", "inclusive_or_expression",
  "exclusive_or_expression", "and_expression", "relational_expression",
  "shift_expression", "additive_expression", "multiplicative_expression",
  "cast_expression", "unary_expression", "unary_operator", "function_call",
  "postfix_expression", "primary_expression", "constant", "argument_list",
  "non_empty_argument_list", "op_exp", "for_declaration", "semicolons", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-284)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-192)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    2070,   -31,   -31,    68,     6,     8,   177,   213,    98,    98,
     -31,  2096,   -31,   -31,   -31,   -31,   -31,   -31,   -31,   -31,
     -31,   -31,  2122,  2044,  -284,  -284,  -284,  -284,  -284,  -284,
      82,  -284,   289,  -284,    82,  -284,  -284,  -284,   -31,  -284,
     -31,   342,    16,  2148,   184,   151,   -31,  -284,   -31,  -284,
    -284,   -31,   -31,  -284,   159,   168,   171,   427,  -284,  -284,
    -284,  -284,  -284,  -284,  -284,  -284,  -284,  -284,   194,   170,
    -284,  -284,  -284,  -284,   212,   228,  -284,   448,    97,  -284,
     239,    93,    93,   422,   260,   214,  -284,   212,   -31,  -284,
    2148,  -284,  -284,   217,  2148,   249,  -284,  -284,    41,  -284,
    -284,  1814,  -284,    89,   151,  -284,   234,   385,  -284,  -284,
    -284,  -284,  -284,   280,   -31,   -31,  -284,   266,    18,   407,
     -31,  -284,  1918,   114,   271,  -284,   492,   114,  -284,  -284,
     260,   260,  1326,   859,  -284,   212,  1958,  1357,    97,  1840,
    -284,   345,  1866,   132,   291,    46,  -284,  1531,   479,  -284,
     312,   407,   412,  1531,   151,    47,  2148,    30,   212,   212,
     343,   119,   -10,  -284,  -284,   295,  -284,  2122,  -284,  -284,
     455,   314,   467,  -284,  -284,  -284,  -284,  -284,  1582,  -284,
    -284,  -284,  1119,   651,  -284,  -284,  -284,  -284,  -284,  -284,
    -284,   415,   325,   339,   433,   309,   471,   534,   337,  -284,
     451,  1613,  -284,   417,  -284,  -284,  -284,   376,  -284,   323,
     328,   365,  1413,    82,   928,   382,  -284,  -284,  -284,   427,
    -284,   721,  -284,  -284,  -284,  -284,  -284,  -284,  -284,  -284,
    -284,   531,  -284,   368,   212,  -284,   190,  -284,   386,  -284,
    -284,   403,  -284,    72,  -284,   101,   172,  -284,    82,   212,
    -284,    95,   212,  1531,   212,   212,   105,  -284,  -284,    82,
     212,  1892,  -284,   212,  1716,  1444,   327,    19,  -284,   463,
    -284,  2122,   343,   347,  1531,  1171,  -284,   193,  2174,   413,
     373,  1531,   475,  -284,   480,   137,  -284,  1531,  1531,  1531,
    1531,  1531,  1531,  1531,  1531,  1531,  1531,  1531,  1531,  1531,
    1531,  1531,  1531,  1531,  1531,  1531,  1531,  1531,  1531,  1531,
    1531,  1531,  1531,  1664,  -284,  1531,  1531,  -284,  -284,   478,
     498,   859,  1531,  1531,  1223,  1275,   531,   212,   212,   790,
    -284,   487,  1531,  -284,  -284,  1531,   212,  -284,  -284,    82,
     212,    82,   212,   212,  -284,   524,  -284,    82,   212,   212,
     504,   -31,  -284,   330,   391,   542,  -284,   488,  1765,   463,
    -284,   463,  -284,   260,  1998,  1500,  -284,  -284,   212,   548,
     554,   553,   556,   134,  -284,  -284,  1531,  -284,   551,  -284,
     581,  -284,  1326,  -284,  -284,   552,  -284,   529,   529,   309,
     309,   529,   471,   471,   471,   471,   534,   534,   337,   337,
    -284,  -284,  -284,  -284,  -284,  -284,  -284,  -284,  -284,  -284,
    -284,   539,   562,   121,  -284,  -284,  -284,   554,   563,   564,
     443,  -284,   554,   568,   355,   212,  -284,   567,   398,  -284,
     212,   212,   212,    82,   463,  -284,  -284,  -284,   313,   491,
    -284,   525,  -284,   565,  -284,  -284,  1531,  -284,  -284,  -284,
    -284,  -284,   302,  -284,  -284,  -284,   928,   928,  1531,    82,
    1531,   566,   212,  -284,  -284,   554,  1326,   612,  -284,   573,
     212,   571,   577,  -284,   997,  -284,   616,   928,  1531,   579,
     509,   613,   218,  -284,   578,  -284,   997,   625,  -284,   585,
    -284,  -284,   621,   622,   859,  -284,  -284,  1531,  -284,   633,
     928,   859,   859,   859,   589,   592,  -284,   859,   232,  -284,
    -284,  1066,  1531,   298,   594,  -284
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       0,    86,    59,     0,     0,     0,    86,    86,    84,    84,
      86,     0,    86,    86,    86,    86,    86,    86,    86,    86,
      86,    86,     0,     0,     3,     4,     5,    10,     6,     7,
       0,     8,     0,    56,     0,     9,    88,    65,    87,    61,
      60,    86,    86,     0,    86,    40,    86,    69,    86,    70,
      85,    86,    86,    68,     0,     0,     0,     0,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,     0,    57,
      56,     1,     2,   274,   160,   115,    58,    18,     0,    90,
       0,     0,     0,   125,   112,   122,   123,   159,    89,    23,
       0,    62,    33,    88,     0,    30,    31,    63,     0,    50,
      51,     0,    46,     0,    40,    64,    43,     0,    41,    66,
      67,    71,    72,    86,    86,    86,   115,     0,   125,     0,
      86,   275,     0,     0,     0,    91,   115,    19,   126,    18,
     113,   114,     0,     0,    15,    16,     0,     0,     0,     0,
      34,     0,     0,     0,    86,     0,    45,     0,     0,    52,
     125,     0,     0,     0,     0,     0,     0,   115,    20,    21,
       0,    96,     0,    93,   116,    19,   117,     0,   257,   258,
     254,   251,   253,   260,   261,   262,   263,   264,     0,   235,
     236,   239,     0,   133,   237,   238,   240,   241,   127,   128,
     193,   202,   203,   206,   210,   211,   214,   219,   222,   225,
     229,     0,   250,   231,   243,   252,   254,   251,   253,     0,
       0,     0,     0,     0,     0,     0,   153,   154,   155,     0,
     146,     0,   143,   152,   156,   158,   151,   147,   148,   149,
     150,     0,   190,   243,   157,   120,     0,   118,     0,   189,
     229,   125,   124,     0,    35,     0,     0,    32,     0,    28,
      55,     0,    49,     0,    47,    48,     0,    44,    42,     0,
      38,     0,    17,    14,     0,     0,    99,     0,    95,   101,
      94,     0,     0,     0,   265,     0,   233,   251,     0,     0,
       0,     0,     0,   134,     0,     0,   138,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   232,   265,     0,   248,   249,     0,
       0,     0,   269,   269,   271,     0,     0,   164,   165,     0,
     188,     0,     0,   142,   144,     0,   161,   121,   119,     0,
      24,     0,    26,    29,    53,     0,    54,     0,    36,    39,
       0,    86,   108,     0,     0,     0,   104,     0,     0,   100,
      98,   101,    97,   112,     0,     0,    92,    11,    13,     0,
     267,     0,   266,     0,   256,    83,     0,   255,     0,   140,
       0,   131,     0,   139,   205,   204,   207,   210,   208,   212,
     213,   209,   216,   215,   218,   217,   220,   221,   223,   224,
     226,   227,   228,   194,   195,   196,   197,   198,   199,   200,
     201,   251,     0,     0,   246,   247,   145,   270,     0,     0,
     251,   272,   273,     0,   251,   162,   187,     0,     0,   192,
      25,    27,    37,     0,   102,   109,   103,   105,    99,     0,
     110,     0,   106,     0,    12,   242,     0,   234,   230,   141,
     132,   135,     0,   136,   245,   244,     0,     0,   269,   256,
     269,     0,    22,   111,   107,   268,     0,   168,   175,     0,
     163,     0,   178,   137,     0,   167,   171,     0,   269,     0,
       0,     0,     0,   180,     0,   169,     0,   173,   172,     0,
     166,   259,     0,     0,     0,   177,   179,   269,   170,     0,
       0,   184,   184,   183,     0,     0,   176,     0,   143,   182,
     181,   174,   269,   144,     0,   174
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -284,  -284,   626,  -284,  -284,   359,  -284,    50,    92,  -284,
    -284,   507,    77,   557,   506,   -81,   -91,  -284,   411,    59,
     -18,   654,    14,    -2,   -56,   -79,   393,  -150,  -221,   241,
     -63,   341,   -20,   528,  -177,  -284,   287,  -283,   -76,  -198,
    -130,   124,  -284,  -284,  -284,  -284,  -284,   201,  -284,  -284,
    -284,  -284,  -284,   187,   169,  -206,   -41,    25,   -53,   -43,
     310,  -284,   381,   261,   308,   279,   303,   304,  -263,   268,
    -284,  -284,  -284,   -46,   192,   358,  -284,  -152,  -284,   -29
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,    23,    24,    25,    26,   123,    27,   216,   217,    30,
      95,    96,   218,   107,   108,   101,   102,   148,   149,   219,
      33,    51,    81,    38,    82,   354,   163,   355,   269,   124,
      84,    34,    85,    86,   188,   284,   285,   286,   220,   221,
     330,   223,   224,   225,   226,   227,   475,   487,   477,   228,
     229,   230,   482,   483,   509,   467,   238,   231,   232,   190,
     191,   192,   193,   194,   195,   196,   197,   198,   199,   200,
     201,   202,   203,   204,   205,   371,   372,   418,   423,   234
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      40,    74,   383,   222,    70,    87,   283,   134,   331,   139,
     146,   268,    42,   142,    44,    37,    39,    36,   130,   131,
      47,    49,    92,   125,    53,   116,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    88,   119,    88,   400,
     401,   402,   271,   162,   272,   359,   361,   144,   146,  -116,
      28,   146,   248,   259,   135,    91,    97,   236,   105,    32,
     109,    43,   110,    45,    93,   111,   112,    77,   132,    73,
      57,    94,   264,    28,    41,   261,   265,    31,   339,   189,
    -116,    69,    32,   151,   262,    79,    88,   233,   273,   158,
     159,   334,    29,    99,   239,   116,    43,    73,    73,   116,
      31,   116,   103,   116,   239,   267,   250,   341,   147,    76,
     239,   347,   257,   448,   147,    29,   249,   360,    50,   252,
     164,   254,   255,    73,    35,   116,   260,    91,    97,   105,
     189,   263,   434,    73,    91,   100,   233,    77,    92,    76,
      99,   129,    78,    77,    99,    77,    78,    35,    78,   103,
      78,    99,    73,   103,    76,    79,    73,   106,    97,   266,
     103,    79,   129,    79,   279,   113,   233,    77,   233,   383,
     146,   419,   264,   335,   114,   233,   265,   115,   140,   455,
     246,   161,   100,   327,   328,    79,   100,   382,   447,    99,
      76,   416,    99,   100,   281,   161,   367,    46,   103,   222,
     120,   103,   336,   451,   363,   453,    99,   280,   353,   282,
     239,   353,   346,   -86,   340,   103,   342,   359,   434,   343,
     141,    37,   239,   140,   357,    36,   161,   348,   233,   233,
     349,   100,    36,    48,   100,   480,   481,   326,   239,   104,
     378,    36,   271,   368,   337,   126,   274,   374,   100,  -185,
    -185,   468,   403,   404,   405,   406,   407,   408,   409,   410,
     375,    36,   266,   121,    88,   266,   138,   233,   485,   233,
     233,   488,   157,    83,   495,   233,   233,   233,   233,   233,
     498,   122,   429,   233,   153,   441,   233,    77,  -185,   473,
     131,    37,    78,   444,   506,    75,   503,   425,   118,   370,
     280,   143,   439,   507,   507,    79,   469,   279,   471,    76,
     430,    99,   431,   136,    77,  -186,  -186,   137,   432,    78,
     103,   128,   239,   161,   443,   166,   489,   189,    36,   189,
     161,   253,    79,   -87,   373,   156,   116,    77,   280,    36,
     370,   413,    78,   129,   150,   504,    94,   417,   417,   422,
     280,   244,   466,   100,  -186,    79,   438,   428,   128,   281,
     514,    36,   132,    73,   222,    37,   358,   274,   294,   295,
     265,   508,   508,   334,   282,    36,   322,   513,   129,   241,
     358,   323,   -86,   264,   265,   302,   303,   265,   296,   297,
      36,    80,   289,    89,    73,   321,   -86,    90,   133,   271,
     233,   369,   270,    36,   462,   240,   304,   290,   274,   459,
     233,   233,   233,   189,   233,   240,   117,   161,   324,  -191,
    -191,   240,  -191,   161,    36,   335,  -191,   377,   233,   274,
     470,   233,   233,   116,    37,   332,    88,   154,    37,   127,
     233,   155,   -86,   271,   338,   435,   276,    76,   233,   -86,
     335,   233,   461,   132,   233,   233,   233,   233,    73,   138,
     241,   233,   117,   -86,   154,   233,   233,   376,   256,   314,
     315,   465,   132,    73,   316,    77,   127,   133,   287,   288,
      78,   379,   165,   417,   414,   417,   165,   317,   318,   319,
     320,    36,   345,    79,   291,   292,   274,   117,   427,   293,
     -88,   305,   -88,   417,   415,   -88,  -129,  -129,   362,   -86,
     433,  -129,   168,   169,   491,   492,   364,   173,  -130,  -130,
     365,   240,   417,  -130,   165,   306,   307,   308,   309,   310,
      73,   251,   380,   240,   311,   312,   381,   417,   174,   175,
     176,   177,  -116,   253,   358,   167,   437,  -116,   265,   240,
     387,   388,   298,   299,   391,   240,   240,   240,   240,   240,
     240,   240,   240,   240,   240,   240,   240,   240,   240,   240,
     240,   240,   240,   392,   393,   394,   395,   271,   117,   463,
     300,   301,    73,   335,   168,   169,   170,   171,   172,   173,
     291,   292,   274,   374,   480,   481,   436,   384,   385,   389,
     390,   396,   397,   133,   398,   399,   335,   445,   446,   449,
     174,   175,   176,   177,   178,   287,   454,   456,   457,   458,
     460,   472,   474,   464,   478,   479,   486,   179,   180,   181,
     490,   497,   494,   240,   182,   499,   183,   450,   281,   500,
     501,   502,   505,   511,   240,   512,   184,   185,   515,    72,
     247,   186,   187,   282,   168,   169,   170,   171,   172,   173,
     258,   152,   344,    52,   366,   421,   242,   452,   476,   496,
     386,   510,   493,   412,     0,     0,     0,     0,     0,     0,
     174,   175,   176,   177,   178,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   179,   180,   181,
       0,     0,     0,     0,   182,     0,   183,     0,   281,     0,
       0,     0,     0,     0,     0,     0,   184,   185,     0,     0,
       0,   186,   187,   282,   168,   169,   206,   207,   208,   173,
     209,     0,   210,   211,   212,   213,   214,   215,     0,     0,
       0,     2,     3,     4,     5,     6,     7,     8,     9,    10,
     174,   175,   176,   177,   178,     0,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,   179,   180,   181,
       0,     0,    73,     0,   182,     0,   133,   333,     0,     0,
       0,     0,     0,     0,     0,     0,   184,   185,     0,     0,
       0,   186,   187,   168,   169,   206,   207,   208,   173,   209,
       0,   210,   211,   212,   213,   214,   215,     0,     0,     0,
       2,     3,     4,     5,     6,     7,     8,     9,    10,   174,
     175,   176,   177,   178,     0,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,   179,   180,   181,     0,
       0,    73,     0,   182,     0,   133,   426,     0,     0,     0,
       0,     0,     0,     0,     0,   184,   185,     0,     0,     0,
     186,   187,   168,   169,   206,   207,   208,   173,   209,     0,
     210,   211,   212,   213,   214,   215,     0,     0,     0,     2,
       3,     4,     5,     6,     7,     8,     9,    10,   174,   175,
     176,   177,   178,     0,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,   179,   180,   181,     0,     0,
      73,     0,   182,     0,   133,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   184,   185,     0,     0,     0,   186,
     187,   168,   169,   206,   207,   208,   173,   209,     0,   210,
     211,   212,   213,   214,   215,     0,     0,     0,     2,     3,
       4,     5,     6,     7,     8,     9,    10,   174,   175,   176,
     177,   178,     0,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,   179,   180,   181,     0,     0,    73,
       0,   182,     0,   329,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   184,   185,     0,     0,     0,   186,   187,
     168,   169,   206,   207,   208,   173,   484,     0,   210,   211,
     212,   213,   214,   215,     0,     0,     0,     2,     3,     4,
       5,     6,     7,     8,     9,    10,   174,   175,   176,   177,
     178,     0,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,   179,   180,   181,     0,     0,    73,     0,
     182,     0,   329,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   184,   185,     0,     0,     0,   186,   187,   168,
     169,   206,   207,   208,   173,   209,     0,   210,   211,   212,
     213,   214,   215,     0,     0,     0,     2,     3,     4,     5,
       6,     7,     8,     9,    10,   174,   175,   176,   177,   178,
       0,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,     0,     0,     0,     0,     0,    73,     0,     0,
       0,   329,   168,   169,   206,   277,   208,   173,     0,     0,
       0,     0,     0,     0,     0,     0,   186,   187,     0,     2,
      68,    55,    56,     6,     7,     8,     9,    10,   174,   175,
     176,   177,   178,     0,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,   278,   179,   180,   181,     0,     0,
       0,     0,   182,     0,   168,   169,   206,   277,   208,   173,
       0,     0,     0,     0,   184,   185,     0,     0,     0,   186,
     187,     2,    68,    55,    56,     6,     7,     8,     9,    10,
     174,   175,   176,   177,   178,     0,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,   179,   180,   181,
       0,     0,     0,     0,   182,     0,   168,   169,   206,   420,
     208,   173,     0,     0,     0,     0,   184,   185,     0,     0,
       0,   186,   187,     2,    68,    55,    56,     6,     7,     8,
       9,    10,   174,   175,   176,   177,   178,     0,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,   179,
     180,   181,     0,     0,     0,     0,   182,     0,   168,   169,
     206,   424,   208,   173,     0,     0,     0,     0,   184,   185,
       0,     0,     0,   186,   187,     2,    68,    55,    56,     6,
       7,     8,     9,    10,   174,   175,   176,   177,   178,     0,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
     278,   179,   180,   181,     0,     0,     0,     0,   182,   168,
     169,   170,   171,   172,   173,     0,     0,     0,     0,     0,
     184,   185,     0,     0,     0,   186,   187,     0,     0,     0,
       0,     0,     0,     0,     0,   174,   175,   176,   177,   178,
     168,   169,   206,   171,   208,   173,     0,     0,     0,     0,
       0,     0,   179,   180,   181,     0,     0,     0,     0,   182,
       0,   183,     0,     0,     0,     0,   174,   175,   176,   177,
     178,   184,   185,     0,     0,     0,   186,   187,     0,     0,
       0,     0,     0,   179,   180,   181,     0,     0,     0,     0,
     182,     0,     0,     0,     0,   237,   168,   169,   206,   171,
     208,   173,   184,   185,     0,     0,     0,   186,   187,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   174,   175,   176,   177,   178,   168,   169,   206,
     171,   208,   173,     0,     0,     0,     0,     0,     0,   179,
     180,   181,     0,     0,    73,     0,   325,     0,     0,     0,
       0,     0,     0,   174,   175,   176,   177,   178,   184,   185,
       0,     0,     0,   186,   187,     0,     0,     0,     0,     0,
     179,   180,   181,     0,     0,     0,     0,   182,     0,     0,
       0,     0,   356,   168,   169,   206,   171,   208,   173,   184,
     185,     0,     0,     0,   186,   187,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   174,
     175,   176,   177,   178,   168,   169,   206,   171,   208,   173,
       0,     0,     0,     0,     0,     0,   179,   180,   181,     0,
       0,     0,     0,   182,     0,     0,     0,     0,   442,     0,
     174,   175,   176,   177,   178,   184,   185,     0,     0,     0,
     186,   187,     0,     0,     0,     0,     0,   179,   180,   181,
       0,     0,     0,     0,   182,   168,   169,   206,   171,   208,
     173,     0,     0,     0,     0,     0,   184,   185,     0,     0,
       0,   186,   187,     0,     0,     0,     0,     0,     0,     0,
       0,   174,   175,   176,   177,   178,   168,   169,   206,   171,
     208,   173,     0,     0,     0,     0,     0,     0,   179,   180,
     181,     0,     0,     0,     0,   275,     0,     0,     0,     0,
       0,     0,   174,   175,   176,   177,   178,   184,   185,     0,
       0,     0,   186,   187,     0,     0,     0,     0,     0,   179,
     180,   181,     0,     0,     0,     0,   313,   168,   169,   206,
     411,   208,   173,     0,     0,     0,     0,     0,   184,   185,
       0,     0,     0,   186,   187,     0,     0,     0,     0,     0,
       0,     0,     0,   174,   175,   176,   177,   178,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     179,   180,   181,     0,     0,     0,     0,   182,     0,     0,
       0,     0,   351,     0,     0,     0,     0,     0,     0,   184,
     185,     0,     0,     0,   186,   187,     2,    68,    55,    56,
       6,     7,     8,     9,    10,     0,     0,     0,     0,     0,
       0,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,     0,     0,    77,     0,     0,     0,     0,   264,
     352,     1,     0,   265,     0,     0,     0,     0,     0,     0,
       0,     0,    79,     0,     0,     2,    68,    55,    56,     6,
       7,     8,     9,    10,     0,     0,     0,     0,     0,     0,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,     0,     0,    36,     0,     0,     0,     0,   358,   352,
       1,     0,   265,     0,     0,     0,     0,     0,     0,     0,
       0,    79,     0,     0,     2,     3,    98,    56,     6,     7,
       8,     9,    10,     0,     0,     0,     1,     0,     0,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
       2,     3,    98,    56,     6,     7,     8,     9,    10,     0,
     145,     0,     1,     0,     0,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,     2,     3,    98,    56,
       6,     7,     8,     9,    10,     0,   243,     0,     1,     0,
       0,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,     2,     3,    98,    56,     6,     7,     8,     9,
      10,     0,   245,     0,     1,     0,     0,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,     2,    68,
      55,    56,     6,     7,     8,     9,    10,     0,   350,     0,
       0,     0,     0,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,     1,     0,     0,     0,     0,     0,
       0,     0,   160,     0,     0,     0,     0,     0,     2,    68,
      55,    56,     6,     7,     8,     9,    10,     0,     0,     0,
       0,     0,     0,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,     1,     0,     0,     0,     0,     0,
       0,     0,   235,     0,     0,     0,     0,     0,     2,    68,
      55,    56,     6,     7,     8,     9,    10,     0,     0,     0,
       0,     0,     0,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    71,     0,     0,     0,     0,     0,
       1,     0,   440,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     2,     3,     4,     5,     6,     7,
       8,     9,    10,     0,     0,     0,     1,     0,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
       2,     3,     4,     5,     6,     7,     8,     9,    10,     0,
       0,     0,     1,     0,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,     2,    54,    55,    56,
       6,     7,     8,     9,    10,     0,     0,     0,     1,     0,
       0,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,     2,    68,    55,    56,     6,     7,     8,     9,
      10,     0,     0,     0,     1,     0,     0,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,     2,     3,
      98,    56,     6,     7,     8,     9,    10,     0,     0,     0,
       1,     0,     0,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,     2,    68,    55,    56,     6,     7,
       8,     9,    10,     0,     0,     0,     0,     0,     0,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,   278
};

static const yytype_int16 yycheck[] =
{
       2,    30,   285,   133,    22,    34,   183,    83,   214,    90,
     101,   161,     6,    94,     6,     1,     2,    48,    81,    82,
       6,     7,     6,    79,    10,     6,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    38,    57,    40,   302,
     303,   304,    52,   122,    54,   266,   267,     6,   139,    19,
       0,   142,     6,     6,    83,    41,    42,   136,    44,     0,
      46,    55,    48,    55,    48,    51,    52,    48,    50,    51,
      11,    55,    53,    23,     6,   156,    57,     0,     6,   132,
      50,    22,    23,   103,   160,    66,    88,   133,   167,   118,
     119,   221,     0,    43,   137,     6,    55,    51,    51,     6,
      23,     6,    43,     6,   147,   161,   147,     6,    19,    20,
     153,     6,   153,   376,    19,    23,   145,   267,    20,   148,
       6,   150,   151,    51,     0,     6,   155,   113,   114,   115,
     183,   160,   353,    51,   120,    43,   182,    48,     6,    20,
      90,    48,    53,    48,    94,    48,    53,    23,    53,    90,
      53,   101,    51,    94,    20,    66,    51,     6,   144,   161,
     101,    66,    48,    66,   182,     6,   212,    48,   214,   452,
     261,   323,    53,    52,     6,   221,    57,     6,     6,    58,
      48,   122,    90,   212,   213,    66,    94,    50,    54,   139,
      20,   321,   142,   101,    57,   136,   272,    20,   139,   329,
       6,   142,   231,   380,   267,   382,   156,   182,   264,    72,
     253,   267,   253,    20,   243,   156,   245,   438,   439,   248,
      48,   207,   265,     6,   265,    48,   167,   256,   274,   275,
     259,   139,    48,    20,   142,    17,    18,   212,   281,    55,
     281,    48,    52,   272,    54,     6,    53,    54,   156,    17,
      18,   457,   305,   306,   307,   308,   309,   310,   311,   312,
     278,    48,   264,    51,   266,   267,    52,   313,   474,   315,
     316,   477,     6,    32,    56,   321,   322,   323,   324,   325,
     486,    53,   335,   329,    50,   364,   332,    48,    56,   466,
     353,   277,    53,   369,   500,     6,   494,   326,    57,   274,
     275,    52,   358,   501,   502,    66,   458,   325,   460,    20,
     339,   261,   341,    53,    48,    17,    18,    57,   347,    53,
     261,    80,   365,   264,   365,    54,   478,   380,    48,   382,
     271,    19,    66,     6,   275,    55,     6,    48,   313,    48,
     315,   316,    53,    48,   103,   497,    55,   322,   323,   324,
     325,     6,    50,   261,    56,    66,   358,   332,   117,    57,
     512,    48,    50,    51,   494,   351,    53,    53,    59,    60,
      57,   501,   502,   503,    72,    48,    53,   507,    48,   138,
      53,    53,     6,    53,    57,    48,    49,    57,    79,    80,
      48,    32,    67,    51,    51,    19,    20,    55,    55,    52,
     446,    54,   161,    48,   433,   137,    69,    68,    53,    54,
     456,   457,   458,   466,   460,   147,    57,   358,    53,    51,
      52,   153,    54,   364,    48,    52,    58,    54,   474,    53,
     459,   477,   478,     6,   420,    53,   438,    52,   424,    80,
     486,    56,    66,    52,    58,    54,   178,    20,   494,     6,
      52,   497,    54,    50,   500,   501,   502,   503,    51,    52,
     219,   507,   103,    20,    52,   511,   512,    54,    56,   201,
      53,   446,    50,    51,    57,    48,   117,    55,    63,    64,
      53,     6,   123,   458,     6,   460,   127,    70,    71,    72,
      73,    48,   251,    66,    61,    62,    53,   138,    11,    66,
      52,    50,    54,   478,     6,    57,    51,    52,   267,    66,
       6,    56,     3,     4,     5,     6,    53,     8,    51,    52,
      57,   253,   497,    56,   165,    74,    75,    76,    77,    78,
      51,    52,    52,   265,    83,    84,    56,   512,    29,    30,
      31,    32,    50,    19,    53,    53,    58,    55,    57,   281,
     289,   290,    81,    82,   293,   287,   288,   289,   290,   291,
     292,   293,   294,   295,   296,   297,   298,   299,   300,   301,
     302,   303,   304,   294,   295,   296,   297,    52,   219,    54,
      46,    47,    51,    52,     3,     4,     5,     6,     7,     8,
      61,    62,    53,    54,    17,    18,    54,   287,   288,   291,
     292,   298,   299,    55,   300,   301,    52,    54,    52,    58,
      29,    30,    31,    32,    33,    63,    54,    54,    54,    51,
      53,    55,    10,    58,    51,    54,    10,    46,    47,    48,
      51,    53,    19,   365,    53,    10,    55,    56,    57,    54,
      19,    19,     9,    54,   376,    53,    65,    66,    54,    23,
     143,    70,    71,    72,     3,     4,     5,     6,     7,     8,
     154,   104,   251,     9,   271,   324,   138,   380,   467,   482,
     289,   502,   480,   315,    -1,    -1,    -1,    -1,    -1,    -1,
      29,    30,    31,    32,    33,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    46,    47,    48,
      -1,    -1,    -1,    -1,    53,    -1,    55,    -1,    57,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    65,    66,    -1,    -1,
      -1,    70,    71,    72,     3,     4,     5,     6,     7,     8,
       9,    -1,    11,    12,    13,    14,    15,    16,    -1,    -1,
      -1,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    -1,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      -1,    -1,    51,    -1,    53,    -1,    55,    56,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    65,    66,    -1,    -1,
      -1,    70,    71,     3,     4,     5,     6,     7,     8,     9,
      -1,    11,    12,    13,    14,    15,    16,    -1,    -1,    -1,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    -1,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    -1,
      -1,    51,    -1,    53,    -1,    55,    56,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    65,    66,    -1,    -1,    -1,
      70,    71,     3,     4,     5,     6,     7,     8,     9,    -1,
      11,    12,    13,    14,    15,    16,    -1,    -1,    -1,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    -1,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    -1,    -1,
      51,    -1,    53,    -1,    55,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    65,    66,    -1,    -1,    -1,    70,
      71,     3,     4,     5,     6,     7,     8,     9,    -1,    11,
      12,    13,    14,    15,    16,    -1,    -1,    -1,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    -1,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    -1,    -1,    51,
      -1,    53,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    65,    66,    -1,    -1,    -1,    70,    71,
       3,     4,     5,     6,     7,     8,     9,    -1,    11,    12,
      13,    14,    15,    16,    -1,    -1,    -1,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    -1,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    -1,    -1,    51,    -1,
      53,    -1,    55,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    65,    66,    -1,    -1,    -1,    70,    71,     3,
       4,     5,     6,     7,     8,     9,    -1,    11,    12,    13,
      14,    15,    16,    -1,    -1,    -1,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      -1,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    -1,    -1,    -1,    -1,    -1,    51,    -1,    -1,
      -1,    55,     3,     4,     5,     6,     7,     8,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    70,    71,    -1,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    -1,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    -1,    -1,
      -1,    -1,    53,    -1,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    -1,    65,    66,    -1,    -1,    -1,    70,
      71,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    -1,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      -1,    -1,    -1,    -1,    53,    -1,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    -1,    65,    66,    -1,    -1,
      -1,    70,    71,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    -1,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    -1,    -1,    -1,    -1,    53,    -1,     3,     4,
       5,     6,     7,     8,    -1,    -1,    -1,    -1,    65,    66,
      -1,    -1,    -1,    70,    71,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    -1,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    -1,    -1,    -1,    -1,    53,     3,
       4,     5,     6,     7,     8,    -1,    -1,    -1,    -1,    -1,
      65,    66,    -1,    -1,    -1,    70,    71,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    29,    30,    31,    32,    33,
       3,     4,     5,     6,     7,     8,    -1,    -1,    -1,    -1,
      -1,    -1,    46,    47,    48,    -1,    -1,    -1,    -1,    53,
      -1,    55,    -1,    -1,    -1,    -1,    29,    30,    31,    32,
      33,    65,    66,    -1,    -1,    -1,    70,    71,    -1,    -1,
      -1,    -1,    -1,    46,    47,    48,    -1,    -1,    -1,    -1,
      53,    -1,    -1,    -1,    -1,    58,     3,     4,     5,     6,
       7,     8,    65,    66,    -1,    -1,    -1,    70,    71,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    29,    30,    31,    32,    33,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    -1,    -1,    -1,    46,
      47,    48,    -1,    -1,    51,    -1,    53,    -1,    -1,    -1,
      -1,    -1,    -1,    29,    30,    31,    32,    33,    65,    66,
      -1,    -1,    -1,    70,    71,    -1,    -1,    -1,    -1,    -1,
      46,    47,    48,    -1,    -1,    -1,    -1,    53,    -1,    -1,
      -1,    -1,    58,     3,     4,     5,     6,     7,     8,    65,
      66,    -1,    -1,    -1,    70,    71,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    29,
      30,    31,    32,    33,     3,     4,     5,     6,     7,     8,
      -1,    -1,    -1,    -1,    -1,    -1,    46,    47,    48,    -1,
      -1,    -1,    -1,    53,    -1,    -1,    -1,    -1,    58,    -1,
      29,    30,    31,    32,    33,    65,    66,    -1,    -1,    -1,
      70,    71,    -1,    -1,    -1,    -1,    -1,    46,    47,    48,
      -1,    -1,    -1,    -1,    53,     3,     4,     5,     6,     7,
       8,    -1,    -1,    -1,    -1,    -1,    65,    66,    -1,    -1,
      -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    29,    30,    31,    32,    33,     3,     4,     5,     6,
       7,     8,    -1,    -1,    -1,    -1,    -1,    -1,    46,    47,
      48,    -1,    -1,    -1,    -1,    53,    -1,    -1,    -1,    -1,
      -1,    -1,    29,    30,    31,    32,    33,    65,    66,    -1,
      -1,    -1,    70,    71,    -1,    -1,    -1,    -1,    -1,    46,
      47,    48,    -1,    -1,    -1,    -1,    53,     3,     4,     5,
       6,     7,     8,    -1,    -1,    -1,    -1,    -1,    65,    66,
      -1,    -1,    -1,    70,    71,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    29,    30,    31,    32,    33,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      46,    47,    48,    -1,    -1,    -1,    -1,    53,    -1,    -1,
      -1,    -1,     6,    -1,    -1,    -1,    -1,    -1,    -1,    65,
      66,    -1,    -1,    -1,    70,    71,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    -1,    -1,    -1,    -1,    -1,
      -1,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    -1,    -1,    48,    -1,    -1,    -1,    -1,    53,
      54,     6,    -1,    57,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    66,    -1,    -1,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    -1,    -1,    -1,    -1,    -1,    -1,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    -1,    -1,    48,    -1,    -1,    -1,    -1,    53,    54,
       6,    -1,    57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    66,    -1,    -1,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    -1,    -1,    -1,     6,    -1,    -1,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    -1,
      56,    -1,     6,    -1,    -1,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    -1,    56,    -1,     6,    -1,
      -1,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    -1,    56,    -1,     6,    -1,    -1,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    -1,    56,    -1,
      -1,    -1,    -1,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,     6,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    54,    -1,    -1,    -1,    -1,    -1,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    -1,    -1,    -1,
      -1,    -1,    -1,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,     6,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    54,    -1,    -1,    -1,    -1,    -1,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    -1,    -1,    -1,
      -1,    -1,    -1,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,     0,    -1,    -1,    -1,    -1,    -1,
       6,    -1,    54,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    -1,    -1,    -1,     6,    -1,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    -1,
      -1,    -1,     6,    -1,    34,    35,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    -1,    -1,    -1,     6,    -1,
      -1,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    -1,    -1,    -1,     6,    -1,    -1,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    -1,    -1,    -1,
       6,    -1,    -1,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    -1,    -1,    -1,    -1,    -1,    -1,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     6,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    86,    87,    88,    89,    91,    92,    93,
      94,    97,   104,   105,   116,   126,    48,   107,   108,   107,
     108,     6,     6,    55,     6,    55,    20,   107,    20,   107,
      20,   106,   106,   107,    21,    22,    23,   104,   107,   107,
     107,   107,   107,   107,   107,   107,   107,   107,    21,   104,
     105,     0,    87,    51,   164,     6,    20,    48,    53,    66,
      90,   107,   109,   114,   115,   117,   118,   164,   108,    51,
      55,   107,     6,    48,    55,    95,    96,   107,    22,    92,
      93,   100,   101,   104,    55,   107,     6,    98,    99,   107,
     107,   107,   107,     6,     6,     6,     6,    90,   114,   117,
       6,    51,    53,    90,   114,   109,     6,    90,   114,    48,
     115,   115,    50,    55,   123,   164,    53,    57,    52,   100,
       6,    48,   100,    52,     6,    56,   101,    19,   102,   103,
     114,   117,    98,    50,    52,    56,    55,     6,   164,   164,
      54,   104,   110,   111,     6,    90,    54,    53,     3,     4,
       5,     6,     7,     8,    29,    30,    31,    32,    33,    46,
      47,    48,    53,    55,    65,    66,    70,    71,   119,   143,
     144,   145,   146,   147,   148,   149,   150,   151,   152,   153,
     154,   155,   156,   157,   158,   159,     5,     6,     7,     9,
      11,    12,    13,    14,    15,    16,    92,    93,    97,   104,
     123,   124,   125,   126,   127,   128,   129,   130,   134,   135,
     136,   142,   143,   158,   164,    54,   110,    58,   141,   144,
     154,   114,   118,    56,     6,    56,    48,    96,     6,   164,
     141,    52,   164,    19,   164,   164,    56,   141,    99,     6,
     164,   100,   123,   164,    53,    57,   108,   109,   112,   113,
     114,    52,    54,   110,    53,    53,   154,     6,    45,   105,
     142,    57,    72,   119,   120,   121,   122,    63,    64,    67,
      68,    61,    62,    66,    59,    60,    79,    80,    81,    82,
      46,    47,    48,    49,    69,    50,    74,    75,    76,    77,
      78,    83,    84,    53,   154,    53,    57,    70,    71,    72,
      73,    19,    53,    53,    53,    53,   142,   164,   164,    55,
     125,   140,    53,    56,   125,    52,   164,    54,    58,     6,
     164,     6,   164,   164,   103,   114,   141,     6,   164,   164,
      56,     6,    54,   109,   110,   112,    58,   141,    53,   113,
     112,   113,   114,   115,    53,    57,   111,   123,   164,    54,
     142,   160,   161,   104,    54,   105,    54,    54,   141,     6,
      52,    56,    50,   122,   145,   145,   147,   148,   148,   149,
     149,   148,   150,   150,   150,   150,   151,   151,   152,   152,
     153,   153,   153,   143,   143,   143,   143,   143,   143,   143,
     143,     6,   160,   142,     6,     6,   125,   142,   162,   162,
       6,   116,   142,   163,     6,   164,    56,    11,   142,   143,
     164,   164,   164,     6,   113,    54,    54,    58,   108,   109,
      54,   110,    58,   141,   123,    54,    52,    54,   153,    58,
      56,   119,   121,   119,    54,    58,    54,    54,    51,    54,
      53,    54,   164,    54,    58,   142,    50,   140,   140,   162,
     164,   162,    55,   119,    10,   131,   132,   133,    51,    54,
      17,    18,   137,   138,     9,   140,    10,   132,   140,   162,
      51,     5,     6,   159,    19,    56,   138,    53,   140,    10,
      54,    19,    19,   124,   162,     9,   140,   124,   125,   139,
     139,    54,    53,   125,   162,    54
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_uint8 yyr1[] =
{
       0,    85,    86,    86,    87,    87,    87,    87,    87,    87,
      87,    88,    88,    88,    88,    88,    88,    89,    90,    90,
      91,    91,    91,    92,    92,    92,    93,    93,    93,    93,
      94,    95,    95,    96,    96,    96,    97,    97,    97,    97,
      98,    98,    98,    99,    99,   100,   100,   101,   101,   101,
     101,   101,   102,   102,   103,   103,   104,   104,   104,   105,
     105,   105,   105,   105,   105,   105,   105,   105,   105,   105,
     105,   105,   105,   105,   105,   105,   105,   105,   105,   105,
     105,   105,   105,   105,   106,   106,   107,   107,   108,   108,
     109,   109,   110,   110,   111,   111,   111,   111,   111,   112,
     112,   112,   112,   113,   113,   113,   113,   113,   113,   113,
     113,   113,   114,   114,   114,   115,   115,   115,   115,   115,
     115,   115,   116,   117,   117,   118,   118,   118,   119,   119,
     119,   119,   119,   120,   120,   120,   120,   120,   121,   121,
     122,   122,   123,   124,   124,   125,   125,   125,   125,   125,
     125,   125,   125,   125,   125,   125,   125,   125,   125,   126,
     126,   127,   128,   128,   128,   128,   129,   130,   131,   131,
     131,   131,   132,   132,   133,   134,   135,   136,   137,   137,
     137,   138,   138,   138,   139,   139,   139,   140,   140,   141,
     142,   142,   142,   143,   143,   143,   143,   143,   143,   143,
     143,   143,   144,   145,   145,   145,   146,   146,   146,   146,
     147,   148,   148,   148,   149,   149,   149,   149,   149,   150,
     150,   150,   151,   151,   151,   152,   152,   152,   152,   153,
     153,   154,   154,   154,   154,   155,   155,   155,   155,   155,
     155,   155,   156,   157,   157,   157,   157,   157,   157,   157,
     157,   158,   158,   158,   158,   158,   158,   159,   159,   159,
     159,   159,   159,   159,   159,   160,   160,   161,   161,   162,
     162,   163,   163,   163,   164,   164
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     2,     1,     1,     1,     1,     1,     1,     1,
       1,     6,     7,     6,     5,     3,     3,     5,     1,     2,
       4,     4,     8,     3,     6,     7,     6,     7,     5,     6,
       3,     1,     3,     1,     2,     3,     6,     7,     5,     6,
       0,     1,     3,     1,     3,     2,     1,     3,     3,     3,
       1,     1,     1,     3,     3,     2,     1,     2,     2,     1,
       2,     2,     3,     3,     3,     2,     3,     3,     2,     2,
       2,     3,     3,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     0,     1,     0,     1,     1,     2,
       1,     2,     3,     1,     2,     2,     1,     3,     3,     1,
       2,     1,     2,     3,     2,     3,     3,     4,     2,     3,
       3,     4,     1,     2,     2,     1,     2,     3,     3,     4,
       3,     4,     2,     1,     3,     1,     2,     3,     1,     1,
       1,     3,     4,     0,     1,     3,     3,     5,     1,     2,
       2,     3,     3,     1,     2,     3,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     2,
       2,     2,     3,     5,     2,     2,     7,     6,     0,     2,
       3,     1,     2,     2,     5,     5,     9,     7,     0,     2,
       1,     4,     4,     3,     0,     1,     2,     2,     1,     1,
       1,     1,     3,     1,     3,     3,     3,     3,     3,     3,
       3,     3,     1,     1,     3,     3,     1,     3,     3,     3,
       1,     1,     3,     3,     1,     3,     3,     3,     3,     1,
       3,     3,     1,     3,     3,     1,     3,     3,     3,     1,
       4,     1,     2,     2,     4,     1,     1,     1,     1,     1,
       1,     1,     4,     1,     4,     4,     3,     3,     2,     2,
       1,     1,     1,     1,     1,     3,     3,     1,     1,     1,
       1,     1,     1,     1,     1,     0,     1,     1,     3,     0,
       1,     0,     1,     1,     1,     2
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
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
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
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






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


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

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
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
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
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
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {

#line 1953 "grammar.tab.c"

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
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
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
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
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
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
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
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 603 "grammar.y"


void yyerror(const char *s) {
    fprintf(stderr, "Error: %s in line %d\n", s, yylineno);
    fprintf(stderr, "Current token: %s\n", yytext);
}

int main(int argc, char *argv[]) {
    extern FILE *yyin;
    
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }
    
    yyin = fopen(argv[1], "r");
    if (!yyin) {
        perror("Error opening file");
        return 1;
    }
    if (yyparse() == 0) {
        printf("Program successfully scanned\n");
    } else {
        fprintf(stderr, "Error scanning program\n");
    }
    fclose(yyin);
    return 0;
}
