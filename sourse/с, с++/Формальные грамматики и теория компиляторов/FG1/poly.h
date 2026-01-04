#ifndef POLY_H
#define POLY_H

typedef struct term {
    int coeff;
    char* vars;
} Term;

typedef struct poly {
    Term* terms;
    int count;
} Poly;

Poly multiply_poly(Poly a, Poly b);
Poly add_poly(Poly a, Poly b);
Poly subtract_poly(Poly a, Poly b);
void print_poly(Poly p);
void normalize_vars(char* vars);

#endif