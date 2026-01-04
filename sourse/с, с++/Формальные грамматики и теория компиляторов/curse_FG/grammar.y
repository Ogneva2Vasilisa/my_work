%{
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
%}

%union {
    int num;
    float fnum;
    char *str;
}

/* --- Терминалы (сначала типизированные) --- */
%token <num> NUMBER HEX_NUMBER
%token <fnum> FLOAT
%token <str> IDENTIFIER STRING CHAR

/* --- Ключевые слова и типы --- */
%token IF ELSE WHILE FOR RETURN BREAK DO
%token SWITCH CASE DEFAULT COLON
%token TYPE STRUCT UNION ENUM LONG SHORT UNSIGNED SIGNED FILE_TYPE
%token NULLVAL EOFVAL TRUE_VAL FALSE_VAL
%token SIZEOF
%token TYPEDEF
%token LONG_LONG
%token UNSIGNED_LONG_LONG
%token SIGNED_LONG_LONG
%token UNSIGNED_SHORT
%token SIGNED_SHORT
%token UNSIGNED_LONG
%token SIGNED_LONG
%token UNSIGNED_TYPE
%token SIGNED_TYPE
%token LONG_DOUBLE
%token STATIC

/* --- Операторы и знаки препинания --- */
%token PLUS MINUS MULTIPLY DIVIDE ASSIGN SEMICOLON COMMA LPAREN RPAREN LBRACE RBRACE
%token LBRACKET RBRACKET
%token GREATER LESS EQUAL NEQUAL AND OR NOT AMPERSAND PIPE CARET MODULO
%token INCREMENT DECREMENT DOT ARROW
%token PLUS_ASSIGN MINUS_ASSIGN MULTIPLY_ASSIGN DIVIDE_ASSIGN MODULO_ASSIGN
%token GE_OP LE_OP
%token LEFT_SHIFT RIGHT_SHIFT LEFT_SHIFT_ASSIGN RIGHT_SHIFT_ASSIGN

/* --- Приоритеты операторов --- */
%left OR
%left AND
%left PIPE
%left CARET
%left AMPERSAND
%left EQUAL NEQUAL
%left GREATER LESS GE_OP LE_OP
%left LEFT_SHIFT RIGHT_SHIFT
%left PLUS MINUS
%left MULTIPLY DIVIDE MODULO
%right NOT
%left DOT ARROW
%left LBRACKET RBRACKET
%left LPAREN RPAREN

%%

/* ----------------- Вход / внешний уровень ----------------- */

program:
      program external_declaration
    | external_declaration
    ;

/* Внешние декларации (функции, типы, декларации, typedef) */
external_declaration:
      function_definition
    | main_function
    | struct_declaration
    | union_declaration
    | enum_declaration
    | declaration_statement
    | typedef_declaration
    ;

/* ----------------- Функции / main ----------------- */

function_definition:
      type IDENTIFIER LPAREN parameter_list RPAREN compound_statement
    | type point IDENTIFIER LPAREN parameter_list RPAREN compound_statement
    | type IDENTIFIER LPAREN parameter_list RPAREN semicolons
    | type IDENTIFIER LPAREN RPAREN semicolons
    | type declarator compound_statement
    | type declarator semicolons
    ;

main_function:
      type IDENTIFIER LPAREN RPAREN compound_statement
    ;

/* точка/множитель (используется для указателей в сигнатурах) */
point:
      MULTIPLY
    | point point
    ;

/* ----------------- typedef (общий случай) ----------------- */

typedef_declaration:
      TYPEDEF type declarator semicolons
    | TYPEDEF type init_declarator_list semicolons
    | TYPEDEF STRUCT IDENTIFIER LBRACE struct_member_list RBRACE IDENTIFIER semicolons
    ;

/* ----------------- struct / union / enum ----------------- */

struct_declaration:
      STRUCT IDENTIFIER SEMICOLON
    | STRUCT IDENTIFIER LBRACE struct_member_list RBRACE semicolons
    | STRUCT IDENTIFIER LBRACE struct_member_list RBRACE IDENTIFIER semicolons
    ;

union_declaration:
      UNION IDENTIFIER LBRACE struct_member_list RBRACE semicolons
    | UNION IDENTIFIER LBRACE struct_member_list RBRACE IDENTIFIER semicolons
    | UNION LBRACE struct_member_list RBRACE semicolons
    | UNION LBRACE struct_member_list RBRACE IDENTIFIER semicolons
    ;

union_variable_declaration:
      UNION IDENTIFIER union_declarator_list
    ;

union_declarator_list:
      union_declarator
    | union_declarator_list COMMA union_declarator
    ;

union_declarator:
      IDENTIFIER
    | MULTIPLY IDENTIFIER
    | MULTIPLY MULTIPLY IDENTIFIER
    ;

enum_declaration:
      ENUM IDENTIFIER LBRACE enum_list RBRACE semicolons
    | ENUM IDENTIFIER LBRACE enum_list RBRACE IDENTIFIER semicolons
    | ENUM LBRACE enum_list RBRACE semicolons
    | ENUM LBRACE enum_list RBRACE IDENTIFIER semicolons
    ;

enum_list:
      /* empty */
    | enum_item
    | enum_list COMMA enum_item
    ;

enum_item:
      IDENTIFIER
    | IDENTIFIER ASSIGN constant_expression
    ;

/* Члены struct/union */
struct_member_list:
      struct_member_list struct_member
    | struct_member
    ;

struct_member:
      type declarator semicolons
    | type init_declarator_list semicolons
    | type bitfield_list semicolons
    | struct_declaration
    | union_declaration
    ;

bitfield_list:
      bitfield
    | bitfield_list COMMA bitfield
    ;

bitfield:
      declarator COLON constant_expression
    | COLON constant_expression
    ;

/* ----------------- Типы и связанные правила ----------------- */

type:
      cast_type
    | STATIC type
    | type TYPE
    ;

cast_type:
      TYPE
    | TYPE pointer
    | TYPE pointer_opt
    | STRUCT IDENTIFIER pointer_opt
    | UNION IDENTIFIER pointer_opt
    | ENUM IDENTIFIER pointer_opt
    | IDENTIFIER pointer_opt
    | LONG TYPE pointer_opt
    | SHORT TYPE pointer_opt
    | FILE_TYPE pointer_opt
    | LONG pointer_opt
    | SHORT pointer_opt
    | UNSIGNED opt_type pointer_opt
    | SIGNED opt_type pointer_opt
    | LONG_LONG pointer_opt
    | UNSIGNED_LONG_LONG pointer_opt
    | SIGNED_LONG_LONG pointer_opt
    | UNSIGNED_SHORT pointer_opt
    | SIGNED_SHORT pointer_opt
    | UNSIGNED_LONG pointer_opt
    | SIGNED_LONG pointer_opt
    | UNSIGNED_TYPE pointer_opt
    | SIGNED_TYPE pointer_opt
    | LONG_DOUBLE pointer_opt
    | STATIC cast_type
    ;

opt_type:
      /* empty */
    | TYPE
    ;

pointer_opt:
      /* empty */
    | pointer
    ;

pointer:
      MULTIPLY
    | pointer pointer
    ;

amper:
      AMPERSAND
    | AMPERSAND amper
    ;

/* ----------------- Деклараторы / параметры ----------------- */

parameter_list:
      parameter_list COMMA parameter_declaration
    | parameter_declaration
    ;

parameter_declaration:
      type declarator
    | type abstract_declarator
    | type
    | type amper declarator
    | type amper abstract_declarator
    ;

abstract_declarator:
      pointer
    | pointer direct_abstract_declarator
    | direct_abstract_declarator
    | amper direct_abstract_declarator
    ;

direct_abstract_declarator:
      LPAREN abstract_declarator RPAREN
    | LBRACKET RBRACKET
    | LBRACKET constant_expression RBRACKET
    | direct_abstract_declarator LBRACKET RBRACKET
    | direct_abstract_declarator LBRACKET constant_expression RBRACKET
    | LPAREN RPAREN
    | LPAREN parameter_list RPAREN
    | direct_abstract_declarator LPAREN RPAREN
    | direct_abstract_declarator LPAREN parameter_list RPAREN
    ;

declarator:
      direct_declarator
    | pointer_opt direct_declarator
    | amper direct_declarator
    ;

direct_declarator:
      IDENTIFIER
    | point IDENTIFIER
    | LPAREN declarator RPAREN
    | direct_declarator LBRACKET RBRACKET
    | direct_declarator LBRACKET constant_expression RBRACKET
    | direct_declarator LPAREN RPAREN
    | direct_declarator LPAREN parameter_list RPAREN
    ;

/* ----------------- Объявления (declaration / init_declarator) ----------------- */

declaration:
      type init_declarator_list
    ;

init_declarator_list:
      init_declarator
    | init_declarator_list COMMA init_declarator
    ;

init_declarator:
      declarator
    | point declarator
    | declarator ASSIGN initializer
    ;

initializer:
      assignment_expression
    | FLOAT
    | STRING
    | LBRACE initializer_list RBRACE
    | LBRACE initializer_list COMMA RBRACE
    ;

initializer_list:
      /* empty */
    | initializer
    | initializer_list COMMA initializer
    | designator_list ASSIGN initializer
    | initializer_list COMMA designator_list ASSIGN initializer
    ;

designator_list:
      designator
    | designator_list designator
    ;

designator:
      DOT IDENTIFIER
    | LBRACKET constant_expression RBRACKET
    ;

/* ----------------- Операторы / блоки ----------------- */

compound_statement:
      LBRACE statement_list RBRACE
    ;

statement_list:
      statement
    | statement_list statement
    ;

statement:
      IDENTIFIER COLON statement
    | compound_statement
    | if_statement
    | while_statement
    | for_statement
    | switch_statement
    | do_while_statement
    | declaration_statement
    | struct_declaration
    | union_declaration
    | enum_declaration
    | expression_statement
    | semicolons
    | jump_statement
    ;

declaration_statement:
      declaration semicolons
    | union_variable_declaration semicolons
    ;

expression_statement:
      expression semicolons
    ;

jump_statement:
      RETURN expression semicolons
    | RETURN LPAREN IDENTIFIER RPAREN semicolons
    | RETURN semicolons
    | BREAK semicolons
    ;

/* ----------------- Циклы / ветвления / переключатели ----------------- */

do_while_statement:
      DO statement1 WHILE LPAREN op_exp RPAREN SEMICOLON
    ;

if_statement:
      IF LPAREN op_exp RPAREN statement1 if_state
    ;

if_state:
      /* empty */
    | ELSE statement1
    | elif1 ELSE statement1
    | elif1
    ;

elif1:
      elif statement1
    | elif1 elif1
    ;

elif:
      ELSE IF LPAREN op_exp RPAREN
    ;

while_statement:
      WHILE LPAREN op_exp RPAREN statement1
    ;

for_statement:
      FOR LPAREN for_declaration SEMICOLON op_exp SEMICOLON op_exp RPAREN statement1
    ;

switch_statement:
      SWITCH LPAREN expression RPAREN LBRACE case_list RBRACE
    ;

case_list:
      /* empty */
    | case_list case_statement
    | case_statement
    ;

case_statement:
      CASE constant COLON statement_list1
    | CASE IDENTIFIER COLON statement_list1
    | DEFAULT COLON statement_list
    ;

statement_list1:
      /* empty */
    | statement
    | statement_list statement
    ;

statement1:
      LBRACE RBRACE
    | statement
    ;

/* ----------------- Выражения ----------------- */

constant_expression:
      conditional_expression
    ;

expression:
      assignment_expression
    | primary_expression
    | expression COMMA assignment_expression
    ;

assignment_expression:
      conditional_expression
    | unary_expression ASSIGN assignment_expression
    | unary_expression PLUS_ASSIGN assignment_expression
    | unary_expression MINUS_ASSIGN assignment_expression
    | unary_expression MULTIPLY_ASSIGN assignment_expression
    | unary_expression DIVIDE_ASSIGN assignment_expression
    | unary_expression MODULO_ASSIGN assignment_expression
    | unary_expression LEFT_SHIFT_ASSIGN assignment_expression
    | unary_expression RIGHT_SHIFT_ASSIGN assignment_expression
    ;

conditional_expression:
      logical_or_expression
    ;

logical_or_expression:
      inclusive_or_expression
    | logical_or_expression OR logical_or_expression 
    | logical_or_expression AND logical_or_expression 
    ;

inclusive_or_expression:
      exclusive_or_expression
    | inclusive_or_expression PIPE exclusive_or_expression
    | exclusive_or_expression CARET and_expression
    | and_expression AMPERSAND and_expression
    ;

exclusive_or_expression:
      and_expression
    ;

and_expression:
      relational_expression
    | and_expression EQUAL relational_expression
    | and_expression NEQUAL relational_expression
    ;

relational_expression:
      shift_expression
    | relational_expression LESS shift_expression
    | relational_expression GREATER shift_expression
    | relational_expression LE_OP shift_expression
    | relational_expression GE_OP shift_expression
    ;

shift_expression:
      additive_expression
    | shift_expression LEFT_SHIFT additive_expression
    | shift_expression RIGHT_SHIFT additive_expression
    ;

additive_expression:
      multiplicative_expression
    | additive_expression PLUS multiplicative_expression
    | additive_expression MINUS multiplicative_expression
    ;

multiplicative_expression:
      cast_expression
    | multiplicative_expression MULTIPLY cast_expression
    | multiplicative_expression DIVIDE cast_expression
    | multiplicative_expression MODULO cast_expression
    ;

cast_expression:
      unary_expression
    | LPAREN cast_type RPAREN cast_expression
    ;

unary_expression:
      postfix_expression
    | unary_operator unary_expression
    | SIZEOF unary_expression
    | SIZEOF LPAREN type RPAREN
    ;

unary_operator:
      PLUS | MINUS | NOT | AMPERSAND | MULTIPLY | INCREMENT | DECREMENT
    ;

function_call:
      IDENTIFIER LPAREN argument_list RPAREN
    ;

postfix_expression:
      primary_expression
    | postfix_expression LBRACKET expression RBRACKET
    | postfix_expression LPAREN argument_list RPAREN
    | postfix_expression DOT IDENTIFIER
    | postfix_expression ARROW IDENTIFIER
    | postfix_expression INCREMENT
    | postfix_expression DECREMENT
    | function_call
    ;

primary_expression:
      IDENTIFIER
    | constant
    | STRING
    | FLOAT
    | LPAREN expression RPAREN
    | LPAREN IDENTIFIER RPAREN
    ;

constant:
      NUMBER
    | HEX_NUMBER
    | FLOAT
    | CHAR
    | NULLVAL
    | EOFVAL
    | TRUE_VAL
    | FALSE_VAL
    ;

/* ----------------- Вспомогательные: аргументы, условные и пр. ----------------- */

argument_list:
      /* empty */
    | non_empty_argument_list
    ;

non_empty_argument_list:
      expression
    | non_empty_argument_list COMMA expression
    ;

op_exp:
      /* empty */
    | expression
    ;

for_declaration:
      /* empty */
    | declaration
    | expression
    ;

/* --- повторяющиеся точки с запятой --- */
semicolons:
      SEMICOLON
    | semicolons SEMICOLON
    ;

%%

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
