/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_GRAMMAR_TAB_H_INCLUDED
# define YY_YY_GRAMMAR_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    NUMBER = 258,                  /* NUMBER  */
    HEX_NUMBER = 259,              /* HEX_NUMBER  */
    FLOAT = 260,                   /* FLOAT  */
    IDENTIFIER = 261,              /* IDENTIFIER  */
    STRING = 262,                  /* STRING  */
    CHAR = 263,                    /* CHAR  */
    IF = 264,                      /* IF  */
    ELSE = 265,                    /* ELSE  */
    WHILE = 266,                   /* WHILE  */
    FOR = 267,                     /* FOR  */
    RETURN = 268,                  /* RETURN  */
    BREAK = 269,                   /* BREAK  */
    DO = 270,                      /* DO  */
    SWITCH = 271,                  /* SWITCH  */
    CASE = 272,                    /* CASE  */
    DEFAULT = 273,                 /* DEFAULT  */
    COLON = 274,                   /* COLON  */
    TYPE = 275,                    /* TYPE  */
    STRUCT = 276,                  /* STRUCT  */
    UNION = 277,                   /* UNION  */
    ENUM = 278,                    /* ENUM  */
    LONG = 279,                    /* LONG  */
    SHORT = 280,                   /* SHORT  */
    UNSIGNED = 281,                /* UNSIGNED  */
    SIGNED = 282,                  /* SIGNED  */
    FILE_TYPE = 283,               /* FILE_TYPE  */
    NULLVAL = 284,                 /* NULLVAL  */
    EOFVAL = 285,                  /* EOFVAL  */
    TRUE_VAL = 286,                /* TRUE_VAL  */
    FALSE_VAL = 287,               /* FALSE_VAL  */
    SIZEOF = 288,                  /* SIZEOF  */
    TYPEDEF = 289,                 /* TYPEDEF  */
    LONG_LONG = 290,               /* LONG_LONG  */
    UNSIGNED_LONG_LONG = 291,      /* UNSIGNED_LONG_LONG  */
    SIGNED_LONG_LONG = 292,        /* SIGNED_LONG_LONG  */
    UNSIGNED_SHORT = 293,          /* UNSIGNED_SHORT  */
    SIGNED_SHORT = 294,            /* SIGNED_SHORT  */
    UNSIGNED_LONG = 295,           /* UNSIGNED_LONG  */
    SIGNED_LONG = 296,             /* SIGNED_LONG  */
    UNSIGNED_TYPE = 297,           /* UNSIGNED_TYPE  */
    SIGNED_TYPE = 298,             /* SIGNED_TYPE  */
    LONG_DOUBLE = 299,             /* LONG_DOUBLE  */
    STATIC = 300,                  /* STATIC  */
    PLUS = 301,                    /* PLUS  */
    MINUS = 302,                   /* MINUS  */
    MULTIPLY = 303,                /* MULTIPLY  */
    DIVIDE = 304,                  /* DIVIDE  */
    ASSIGN = 305,                  /* ASSIGN  */
    SEMICOLON = 306,               /* SEMICOLON  */
    COMMA = 307,                   /* COMMA  */
    LPAREN = 308,                  /* LPAREN  */
    RPAREN = 309,                  /* RPAREN  */
    LBRACE = 310,                  /* LBRACE  */
    RBRACE = 311,                  /* RBRACE  */
    LBRACKET = 312,                /* LBRACKET  */
    RBRACKET = 313,                /* RBRACKET  */
    GREATER = 314,                 /* GREATER  */
    LESS = 315,                    /* LESS  */
    EQUAL = 316,                   /* EQUAL  */
    NEQUAL = 317,                  /* NEQUAL  */
    AND = 318,                     /* AND  */
    OR = 319,                      /* OR  */
    NOT = 320,                     /* NOT  */
    AMPERSAND = 321,               /* AMPERSAND  */
    PIPE = 322,                    /* PIPE  */
    CARET = 323,                   /* CARET  */
    MODULO = 324,                  /* MODULO  */
    INCREMENT = 325,               /* INCREMENT  */
    DECREMENT = 326,               /* DECREMENT  */
    DOT = 327,                     /* DOT  */
    ARROW = 328,                   /* ARROW  */
    PLUS_ASSIGN = 329,             /* PLUS_ASSIGN  */
    MINUS_ASSIGN = 330,            /* MINUS_ASSIGN  */
    MULTIPLY_ASSIGN = 331,         /* MULTIPLY_ASSIGN  */
    DIVIDE_ASSIGN = 332,           /* DIVIDE_ASSIGN  */
    MODULO_ASSIGN = 333,           /* MODULO_ASSIGN  */
    GE_OP = 334,                   /* GE_OP  */
    LE_OP = 335,                   /* LE_OP  */
    LEFT_SHIFT = 336,              /* LEFT_SHIFT  */
    RIGHT_SHIFT = 337,             /* RIGHT_SHIFT  */
    LEFT_SHIFT_ASSIGN = 338,       /* LEFT_SHIFT_ASSIGN  */
    RIGHT_SHIFT_ASSIGN = 339       /* RIGHT_SHIFT_ASSIGN  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 15 "grammar.y"

    int num;
    float fnum;
    char *str;

#line 154 "grammar.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_GRAMMAR_TAB_H_INCLUDED  */
