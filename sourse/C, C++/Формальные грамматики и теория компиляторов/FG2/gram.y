%{
#include <math.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include "poly.h"
extern char current_line[];
extern int yylineno;
extern char *yytext;
extern FILE *yyin;
void yyerror(const char *s);
int yylex();
ProgramVar program_vars[100];
int program_var_count = 0;
PolyVar poly_vars[100];
int poly_var_count = 0;
Polynomial *get_program_var(const char *name);
ProgramVar *find_program_var(const char *name);
%}

%union {
    int num;
    char *str;
    struct Polynomial *poly;
}

%token <num> NUMBER
%token <str> VAR IDENTIFIER
%token ASSIGN LPAREN RPAREN END_LINE PRINT
%token PLUS MINUS MULTIPLY DIVIDE POW
%type <poly> expr term factor line

%left PLUS MINUS
%left MULTIPLY DIVIDE 
%left NEG
%right POW

%%

program:
    | program line END_LINE
    ;

line:
    expr {
        printf("%s\n", current_line);
        printf("~   ");
        print_polynomial($1);
        current_line[0] = '\0';
        free_polynomial($1);
    }
    | VAR ASSIGN expr {
        printf("%s\n", current_line);
        current_line[0] = '\0';
        ProgramVar *var = find_program_var($1);
        if (var) {
            free_polynomial(var->value);
            var->value = $3;
        } else if (program_var_count < 100) {
            strncpy(program_vars[program_var_count].name, $1, 31);
            program_vars[program_var_count].value = $3;
            program_var_count++;
        } else {
            fprintf(stderr, "[SYN ERROR] Too many program variables\n");
            free_polynomial($3);
        }
        free($1);
    }
    | IDENTIFIER ASSIGN NUMBER {
        bool found = false;
        for (int i = 0; i < poly_var_count; i++) {
            if (strcmp(poly_vars[i].name, $1) == 0) {
                poly_vars[i].value = $3;
                poly_vars[i].is_defined = true;
                found = true;
                break;
            }
        }
        if (!found && poly_var_count < 100) {
            strncpy(poly_vars[poly_var_count].name, $1, 15);
            poly_vars[poly_var_count].value = $3;
            poly_vars[poly_var_count].is_defined = true;
            poly_var_count++;
        }
        free($1);
    }
    | PRINT expr {
        printf("%s\n", current_line);
        printf("print ~ ");
        print_polynomial($2);
        free_polynomial($2);
        current_line[0] = '\0';
    }
    | error {
        fprintf(stderr, "[SYN ERROR] Line %d: invalid expression near '%s'\n", yylineno, yytext);
        yyclearin;
        yyerrok;
    }
    ;

expr:
    expr PLUS term      { $$ = add_polynomials($1, $3); free_polynomial($1); free_polynomial($3); }
    | expr MINUS term   { $$ = subtract_polynomials($1, $3); free_polynomial($1); free_polynomial($3); }
    | MINUS expr %prec NEG { $$ = negate_polynomial($2); }
    | term             { $$ = $1; }
    ;

term:
    term MULTIPLY factor { $$ = multiply_polynomials($1, $3); free_polynomial($1); free_polynomial($3); }
    | term factor        { $$ = multiply_polynomials($1, $2); free_polynomial($1); free_polynomial($2); }
    | term DIVIDE factor {
        if ($3->mono_count == 0 || (is_constant($3) && $3->monomials[0].coefficient == 0)) {
            fprintf(stderr, "[ERROR] Line %d: division by zero!\n", yylineno-1);
            free_polynomial($1);
            free_polynomial($3);
            $$ = create_polynomial_from_number(0);
        } else {
            Polynomial *remainder = NULL;
            $$ = divide_polynomials($1, $3, &remainder);
            if (remainder && remainder->mono_count > 0) {
                printf("Remainder: ");
                print_polynomial(remainder);
            }
            free_polynomial($1);
            free_polynomial($3);
            if (remainder) free_polynomial(remainder);
        }
    }
    | factor             { $$ = $1; }
    ;

factor:
    factor POW factor { $$ = power_polynomial($1, $3); free_polynomial($1); free_polynomial($3); }
    | LPAREN expr RPAREN { $$ = $2; }
    | NUMBER             { $$ = create_polynomial_from_number($1); }
    | IDENTIFIER         { $$ = create_polynomial_from_identifier($1); }
    | VAR                { $$ = get_program_var($1); free($1); }
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "[SYN ERROR] Line %d: %s (token: '%s')\n", yylineno, s, yytext);
}

Polynomial *get_program_var(const char *name) {
    ProgramVar *var = find_program_var(name);
    if (var) return copy_polynomial(var->value);
    fprintf(stderr, "[SYN ERROR] Line %d: Program variable %s is not initialized!\n", yylineno, name);
    return create_polynomial_from_number(0);
}

ProgramVar *find_program_var(const char *name) {
    for (int i = 0; i < program_var_count; i++) {
        if (strcmp(program_vars[i].name, name) == 0) return &program_vars[i];
    }
    return NULL;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "[SYN ERROR] Use: %s <filename>\n", argv[0]);
        return 1;
    }
    FILE *file = fopen(argv[1], "r");
    if (!file) {
        fprintf(stderr, "[SYN ERROR] Failed to open file %s\n", argv[1]);
        return 1;
    }
    yyin = file;
    yyparse();
    fclose(file);
    return 0;
}
