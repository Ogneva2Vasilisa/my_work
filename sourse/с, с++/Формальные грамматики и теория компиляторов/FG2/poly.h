#ifndef POLY_H
#define POLY_H

#include <stdbool.h>

typedef struct {
    char name[16];
    int degree;
} Variable;

typedef struct {
    int coefficient;
    Variable variables[8];
    int var_count;
} Monomial;

typedef struct Polynomial {
    Monomial monomials[64];
    int mono_count;
    int is_power;
    struct Polynomial *base;
    struct Polynomial *exponent;
} Polynomial;

typedef struct PolyVar {
    char name[16];
    int value;
    bool is_defined;
} PolyVar;

typedef struct ProgramVar {
    char name[32];
    Polynomial *value;
} ProgramVar;

extern PolyVar poly_vars[100];
extern int poly_var_count;

extern ProgramVar program_vars[100];
extern int program_var_count;

Polynomial *create_polynomial_from_number(int n);
Polynomial *create_polynomial_from_identifier(char* id);
Polynomial *copy_polynomial(const Polynomial *p);
Polynomial *add_polynomials(const Polynomial *a, const Polynomial *b);
Polynomial *subtract_polynomials(const Polynomial *a, const Polynomial *b);
Polynomial *multiply_polynomials(const Polynomial *a, const Polynomial *b);
Polynomial *power_polynomial(const Polynomial *base, const Polynomial *exponent);
Polynomial *negate_polynomial(const Polynomial *p);
/* divide_polynomials возвращает частное и через remainder — остаток (может быть NULL) */
Polynomial *divide_polynomials(const Polynomial *numer, const Polynomial *denom, Polynomial **remainder);
int evaluate_polynomial(const Polynomial *p);
bool is_constant(const Polynomial *p);
void print_polynomial(const Polynomial *p);
void free_polynomial(Polynomial *p);

#endif
