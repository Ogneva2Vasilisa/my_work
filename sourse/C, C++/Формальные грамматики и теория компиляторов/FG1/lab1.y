%{
#include <math.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

extern int yylineno;
extern char *yytext;
extern FILE *yyin;

void yyerror(const char *s);
int yylex();

typedef struct Variable {
    char name[16];
    int degree;
} Variable;

typedef struct Monomial {
    int coefficient;
    Variable variables[8];
    int var_count;
} Monomial;

typedef struct Polynomial {
    Monomial monomials[64];
    int mono_count;
} Polynomial;

typedef struct ProgramVar {
    char name[32];
    Polynomial *value;
} ProgramVar;

ProgramVar program_vars[100];
int program_var_count = 0;

typedef struct PolyVar {
    char name[16];
    int value;
    bool is_defined;
} PolyVar;

PolyVar poly_vars[100];
int poly_var_count = 0;

Polynomial *create_polynomial_from_number(int n);
Polynomial *create_polynomial_from_identifier(char* id);
Polynomial *copy_polynomial(const Polynomial *p);
Polynomial *add_polynomials(const Polynomial *a, const Polynomial *b);
Polynomial *subtract_polynomials(const Polynomial *a, const Polynomial *b);
Polynomial *multiply_polynomials(const Polynomial *a, const Polynomial *b);
Polynomial *power_polynomial(const Polynomial *base, int exponent);
Polynomial *negate_polynomial(const Polynomial *p);
Polynomial *get_program_var(const char *name);
void print_polynomial(const Polynomial *p);
void free_polynomial(Polynomial *p);
int evaluate_polynomial(const Polynomial *p);

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
%left PLUS MINUS
%left MULTIPLY DIVIDE 
%left NEG
%right POW

%type <poly> expr term factor line

%%
program:
    | program line END_LINE | program END_LINE
    ;

line:
    expr {
        print_polynomial($1);  
        int value = evaluate_polynomial($1);
        free_polynomial($1); 
    }
    | VAR ASSIGN expr { 
        ProgramVar *var = find_program_var($1);
        if (var) {
            free_polynomial(var->value);
            var->value = $3;
        } else if (program_var_count < 100) {
            strncpy(program_vars[program_var_count].name, $1, 31);
            program_vars[program_var_count].value = $3;
            program_var_count++;
        } else {
            fprintf(stderr, "[ERROR] Too many program variables\n");
            exit(1);
        }
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
    }
    | IDENTIFIER ASSIGN MINUS NUMBER {
        bool found = false;
        for (int i = 0; i < poly_var_count; i++) {
            if (strcmp(poly_vars[i].name, $1) == 0) {
                poly_vars[i].value = -$4;
                poly_vars[i].is_defined = true;
                found = true;
                break;
            }
        }
        if (!found && poly_var_count < 100) {
            strncpy(poly_vars[poly_var_count].name, $1, 15);
            poly_vars[poly_var_count].value = -$4;
            poly_vars[poly_var_count].is_defined = true;
            poly_var_count++;
        }
    }
    | PRINT expr {  
        int value = evaluate_polynomial($2);
        if (value != INT_MIN) {
            printf("result %d\n\n", value);
        }
        else {
            printf("value of polynom ");
            print_polynomial($2);
        }
        free_polynomial($2); 
    }
    ;

expr:
    expr PLUS term          { $$ = add_polynomials($1, $3); free_polynomial($1); free_polynomial($3); }
    | expr MINUS term       { $$ = subtract_polynomials($1, $3); free_polynomial($1); free_polynomial($3); }
    | MINUS expr %prec NEG  { $$ = negate_polynomial($2); }
    | term                  { $$ = $1; }
    ;

term:
    term MULTIPLY factor    { $$ = multiply_polynomials($1, $3); free_polynomial($1); free_polynomial($3); }
    | term factor           { $$ = multiply_polynomials($1, $2); free_polynomial($1); free_polynomial($2); }
    | term DIVIDE factor    { printf("[WARNING] Line %d: division is not provided in calculator. The result may be incorrect!\n", yylineno - 1); }
    | factor                { $$ = $1; }
    ;

factor:
    factor POW NUMBER       { $$ = power_polynomial($1, $3); free_polynomial($1); }
    | LPAREN expr RPAREN    { $$ = $2; }
    | NUMBER                { $$ = create_polynomial_from_number($1); }
    | IDENTIFIER            { $$ = create_polynomial_from_identifier($1); }
    | VAR                   { $$ = get_program_var($1); }
    ;
%%

void yyerror(const char *s) {
    fprintf(stderr, "[ERROR] Line %d: %s (token: '%s')\n", yylineno, s, yytext);
}

Polynomial *get_program_var(const char *name) {
    ProgramVar *var = find_program_var(name);
    if (var) {
        return copy_polynomial(var->value);
    }
    fprintf(stderr, "[ERROR] Line %d: Program variable %s is not initialized!\n", yylineno, name);
    exit(1);
}

ProgramVar *find_program_var(const char *name) {
    for (int i = 0; i < program_var_count; i++) {
        if (strcmp(program_vars[i].name, name) == 0) {
            return &program_vars[i];
        }
    }
    return NULL;
}

bool are_same_monomials(const Monomial *a, const Monomial *b) {
    if (a->var_count != b->var_count) return 0;
    for (int i = 0; i < a->var_count; i++) {
        bool flag = false;
        for (int j = 0; j < b->var_count; j++){
            if (strcmp(a->variables[i].name, b->variables[j].name) == 0 &&
                a->variables[i].degree == b->variables[j].degree) flag = true;
        }
        if(!flag) return false;
    }
    return true;
}

Polynomial *create_polynomial_from_number(int n) {
    Polynomial *p = calloc(1, sizeof(Polynomial));
    p->monomials[0].coefficient = n;
    p->monomials[0].var_count = 0;
    p->mono_count = 1;
    return p;
}

Polynomial *create_polynomial_from_identifier(char* id) {
    Polynomial *p = calloc(1, sizeof(Polynomial));
    p->monomials[0].coefficient = 1;
    p->monomials[0].var_count = 1;
    strncpy(p->monomials[0].variables[0].name, id, 15);
    p->monomials[0].variables[0].degree = 1;
    p->mono_count = 1;
    return p;
}

Polynomial *copy_polynomial(const Polynomial *src) {
    Polynomial *p = calloc(1, sizeof(Polynomial));
    memcpy(p, src, sizeof(Polynomial));
    return p;
}

void simplify_polynomial(Polynomial *p) {
    for (int i = 0; i < p->mono_count; ++i) {
        for (int j = i + 1; j < p->mono_count; ) {
            if (are_same_monomials(&p->monomials[i], &p->monomials[j])) {
                p->monomials[i].coefficient += p->monomials[j].coefficient;
                for (int k = j; k < p->mono_count - 1; ++k)
                    p->monomials[k] = p->monomials[k + 1];
                p->mono_count--;
            } else {
                ++j;
            }
        }
    }
    for (int i = 0; i < p->mono_count; ) {
        if (p->monomials[i].coefficient == 0) {
            for (int k = i; k < p->mono_count - 1; ++k)
                p->monomials[k] = p->monomials[k + 1];
            p->mono_count--;
        } else {
            ++i;
        }
    }
}

Polynomial *add_polynomials(const Polynomial *a, const Polynomial *b) {
    Polynomial *res = copy_polynomial(a);
    for (int i = 0; i < b->mono_count; ++i) {
        res->monomials[res->mono_count++] = b->monomials[i];
    }
    simplify_polynomial(res);
    return res;
}

Polynomial *subtract_polynomials(const Polynomial *a, const Polynomial *b) {
    Polynomial *neg = negate_polynomial(b);
    Polynomial *res = add_polynomials(a, neg);
    free_polynomial(neg);
    return res;
}

Polynomial *negate_polynomial(const Polynomial *p) {
    Polynomial *res = copy_polynomial(p);
    for (int i = 0; i < res->mono_count; ++i)
        res->monomials[i].coefficient *= -1;
    return res;
}

Polynomial *multiply_polynomials(const Polynomial *a, const Polynomial *b) {
    Polynomial *res = calloc(1, sizeof(Polynomial));
    for (int i = 0; i < a->mono_count; ++i) {
        for (int j = 0; j < b->mono_count; ++j) {
            Monomial m = {0};
            m.coefficient = a->monomials[i].coefficient * b->monomials[j].coefficient;
            int idx = 0;
            for (int k = 0; k < a->monomials[i].var_count; ++k)
                m.variables[idx++] = a->monomials[i].variables[k];
            for (int k = 0; k < b->monomials[j].var_count; ++k) {
                int found = 0;
                for (int l = 0; l < idx; ++l) {
                    if (strcmp(m.variables[l].name, b->monomials[j].variables[k].name) == 0) {
                        m.variables[l].degree += b->monomials[j].variables[k].degree;
                        found = 1;
                        break;
                    }
                }
                if (!found)
                    m.variables[idx++] = b->monomials[j].variables[k];
            }
            m.var_count = idx;
            res->monomials[res->mono_count++] = m;
        }
    }
    simplify_polynomial(res);
    return res;
}

Polynomial *power_polynomial(const Polynomial *base, int exponent) {
    if (exponent == 0) return create_polynomial_from_number(1);
    Polynomial *res = copy_polynomial(base);
    for (int i = 1; i < exponent; ++i) {
        Polynomial *tmp = multiply_polynomials(res, base);
        free_polynomial(res);
        res = tmp;
    }
    return res;
}

int evaluate_polynomial(const Polynomial *p) {
    int result = 0;
    for (int i = 0; i < p->mono_count; i++) {
        Monomial m = p->monomials[i];
        int term_value = m.coefficient;
        for (int j = 0; j < m.var_count; j++) {
            bool var_found = false;
            for (int k = 0; k < poly_var_count; k++) {
                if (strcmp(poly_vars[k].name, m.variables[j].name) == 0 && poly_vars[k].is_defined) {
                    term_value *= (int)pow(poly_vars[k].value, m.variables[j].degree);
                    var_found = true;
                    break;
                }
            }
            if (!var_found) {
                return INT_MIN;
            }
        }
        result += term_value;
    }
    return result;
}

int compare_monomials(const void *a, const void *b) {
    const Monomial *m1 = (const Monomial *)a;
    const Monomial *m2 = (const Monomial *)b;
    int deg1 = 0, deg2 = 0;
    for (int i = 0; i < m1->var_count; ++i){
        deg1 += m1->variables[i].degree;
    }
    for (int i = 0; i < m2->var_count; ++i){
        deg2 += m2->variables[i].degree;
    }
    return deg2 - deg1;
}

void print_polynomial(const Polynomial *p) {
    if (p->mono_count == 0) {
        printf("0\n\n");
        return;
    }
    Monomial *sorted = malloc(sizeof(Monomial) * p->mono_count);
    for (int i = 0; i < p->mono_count; ++i){
        sorted[i] = p->monomials[i];
    }
    qsort(sorted, p->mono_count, sizeof(Monomial), compare_monomials);
    for (int i = 0; i < p->mono_count; ++i) {
        Monomial m = sorted[i];
        if (i > 0 && m.coefficient > 0) printf("+");
        if (m.coefficient != 1 || m.var_count == 0) {
            if (m.coefficient != -1 || m.var_count == 0) printf("%d", m.coefficient);
            else printf("-");
        }
        for (int j = 0; j < m.var_count; ++j) {
            printf("%s", m.variables[j].name);
            if (m.variables[j].degree > 1)
                printf("^%d", m.variables[j].degree);
        }
    }
    printf("\n\n");
    free(sorted);
}

void free_polynomial(Polynomial *p) {
    free(p);
}

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "[ERROR] Use: %s <filename>\n", argv[0]);
        return 1;
    }
    FILE *file = fopen(argv[1], "r");
    if (!file) {
        fprintf(stderr, "[ERROR] Failed to open file %s\n", argv[1]);
        return 1;
    }
    yyin = file;
    int result = yyparse();
    fclose(file);
    return 0;
}