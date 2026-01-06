#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include "poly.h"

extern PolyVar poly_vars[100];
extern int poly_var_count;

extern ProgramVar program_vars[100];
extern int program_var_count;

static int compare_variables_for_sort(const void *a, const void *b) {
    const Variable *va = a;
    const Variable *vb = b;
    return strcmp(va->name, vb->name);
}

static bool are_same_monomials(const Monomial *a, const Monomial *b) {
    if (a->var_count != b->var_count) return false;
    Variable va[8];
    Variable vb[8];
    memcpy(va, a->variables, a->var_count * sizeof(Variable));
    memcpy(vb, b->variables, b->var_count * sizeof(Variable));
    qsort(va, a->var_count, sizeof(Variable), compare_variables_for_sort);
    qsort(vb, b->var_count, sizeof(Variable), compare_variables_for_sort);
    for (int i = 0; i < a->var_count; ++i) {
        if (strcmp(va[i].name, vb[i].name) != 0) return false;
        if (va[i].degree != vb[i].degree) return false;
    }
    return true;
}

static bool polynomials_equal(const Polynomial *a, const Polynomial *b) {
    if (!a || !b) return false;
    if (a->mono_count == 0 && b->mono_count == 0) {
        if (!a->base && !b->base) return true;
        if (!a->base || !b->base) return false;
        if (!polynomials_equal(a->base, b->base)) return false;
        if (!polynomials_equal(a->exponent, b->exponent)) return false;
        return true;
    }
    if (a->mono_count == 0 || b->mono_count == 0) return false;
    if (a->mono_count != b->mono_count) return false;
    for (int i = 0; i < a->mono_count; ++i) {
        if (a->monomials[i].coefficient != b->monomials[i].coefficient) return false;
        if (!are_same_monomials(&a->monomials[i], &b->monomials[i])) return false;
    }
    return true;
}

static int monomial_degree_if_matches_base(const Polynomial *base, const Polynomial *p) {
    if (!base || !p) return -1;
    if (base->mono_count != 1 || p->mono_count != 1) return -1;
    const Monomial *mb = &base->monomials[0];
    const Monomial *mp = &p->monomials[0];
    if (mb->var_count != 1 || mp->var_count != 1) return -1;
    if (strcmp(mb->variables[0].name, mp->variables[0].name) != 0) return -1;
    if (mb->variables[0].degree != 1) return -1;
    return mp->variables[0].degree;
}

Polynomial *copy_polynomial(const Polynomial *src) {
    if (!src) return NULL;
    Polynomial *p = calloc(1, sizeof(Polynomial));
    p->mono_count = src->mono_count;
    p->is_power = src->is_power;
    for (int i = 0; i < src->mono_count && i < 64; ++i) {
        p->monomials[i].coefficient = src->monomials[i].coefficient;
        p->monomials[i].var_count = src->monomials[i].var_count;
        for (int j = 0; j < src->monomials[i].var_count && j < 8; ++j) {
            strncpy(p->monomials[i].variables[j].name, src->monomials[i].variables[j].name, 15);
            p->monomials[i].variables[j].name[15] = '\0';
            p->monomials[i].variables[j].degree = src->monomials[i].variables[j].degree;
        }
    }
    if (src->mono_count == 0) {
        if (src->base) p->base = copy_polynomial(src->base);
        else p->base = NULL;
        if (src->exponent) p->exponent = copy_polynomial(src->exponent);
        else p->exponent = NULL;
    } else {
        p->base = NULL;
        p->exponent = NULL;
    }
    return p;
}

void simplify_polynomial(Polynomial *p) {
    if (!p) return;
    if (p->mono_count <= 1) return;
    for (int i = 0; i < p->mono_count; ++i) {
        for (int j = i + 1; j < p->mono_count; ) {
            if (are_same_monomials(&p->monomials[i], &p->monomials[j])) {
                p->monomials[i].coefficient += p->monomials[j].coefficient;
                for (int k = j; k < p->mono_count - 1; ++k) p->monomials[k] = p->monomials[k + 1];
                p->mono_count--;
            } else ++j;
        }
    }
    for (int i = 0; i < p->mono_count; ) {
        if (p->monomials[i].coefficient == 0) {
            for (int k = i; k < p->mono_count - 1; ++k) p->monomials[k] = p->monomials[k + 1];
            p->mono_count--;
        } else ++i;
    }
}

Polynomial *create_polynomial_from_number(int n) {
    Polynomial *p = calloc(1, sizeof(Polynomial));
    p->mono_count = 1;
    p->monomials[0].coefficient = n;
    p->monomials[0].var_count = 0;
    return p;
}

Polynomial *create_polynomial_from_identifier(char* id) {
    Polynomial *p = calloc(1, sizeof(Polynomial));
    p->mono_count = 1;
    p->monomials[0].coefficient = 1;
    p->monomials[0].var_count = 1;
    strncpy(p->monomials[0].variables[0].name, id, 15);
    p->monomials[0].variables[0].name[15] = '\0';
    p->monomials[0].variables[0].degree = 1;
    return p;
}

Polynomial *create_power_expression(const Polynomial *base, const Polynomial *exp) {
    if (!base || !exp) return create_polynomial_from_number(0);
    Polynomial *p = calloc(1, sizeof(Polynomial));
    p->mono_count = 0;
    p->is_power = 1;
    p->base = copy_polynomial(base);
    p->exponent = copy_polynomial(exp);
    return p;
}

Polynomial *negate_polynomial(const Polynomial *p) {
    if (!p) return NULL;
    if (p->mono_count == 0) {
        fprintf(stderr, "[ERROR] Negation of formal power expressions is not supported\n");
        return create_polynomial_from_number(0);
    }
    Polynomial *res = calloc(1, sizeof(Polynomial));
    res->mono_count = p->mono_count;
    for (int i = 0; i < p->mono_count; ++i) {
        res->monomials[i] = p->monomials[i];
        res->monomials[i].coefficient = -res->monomials[i].coefficient;
    }
    return res;
}

Polynomial *add_polynomials(const Polynomial *a, const Polynomial *b) {
    if (!a && !b) return create_polynomial_from_number(0);
    if (!a) return copy_polynomial(b);
    if (!b) return copy_polynomial(a);
    if (a->mono_count == 0 || b->mono_count == 0) {
        fprintf(stderr, "[ERROR] Addition of formal power expressions is not supported\n");
        return create_polynomial_from_number(0);
    }
    Polynomial *res = calloc(1, sizeof(Polynomial));
    res->mono_count = 0;
    for (int i = 0; i < a->mono_count && res->mono_count < 64; ++i) res->monomials[res->mono_count++] = a->monomials[i];
    for (int i = 0; i < b->mono_count && res->mono_count < 64; ++i) res->monomials[res->mono_count++] = b->monomials[i];
    simplify_polynomial(res);
    return res;
}

Polynomial *subtract_polynomials(const Polynomial *a, const Polynomial *b) {
    if (!a && !b) return create_polynomial_from_number(0);
    if (!a) return negate_polynomial(b);
    if (!b) return copy_polynomial(a);
    if (a->mono_count == 0 || b->mono_count == 0) {
        fprintf(stderr, "[ERROR] Subtraction involving formal power expressions is not supported\n");
        return create_polynomial_from_number(0);
    }
    Polynomial *neg = negate_polynomial(b);
    Polynomial *res = add_polynomials(a, neg);
    free_polynomial(neg);
    return res;
}

Monomial multiply_monomials_internal(const Monomial *a, const Monomial *b) {
    Monomial m;
    memset(&m, 0, sizeof(Monomial));
    m.coefficient = a->coefficient * b->coefficient;
    int idx = 0;
    for (int i = 0; i < a->var_count && idx < 8; ++i) m.variables[idx++] = a->variables[i];
    for (int j = 0; j < b->var_count; ++j) {
        int found = 0;
        for (int k = 0; k < idx; ++k) {
            if (strcmp(m.variables[k].name, b->variables[j].name) == 0) {
                m.variables[k].degree += b->variables[j].degree;
                found = 1;
                break;
            }
        }
        if (!found && idx < 8) m.variables[idx++] = b->variables[j];
    }
    m.var_count = idx;
    return m;
}

Polynomial *multiply_polynomials(const Polynomial *a, const Polynomial *b) {
    if (!a || !b) return create_polynomial_from_number(0);
    if (a->mono_count == 0 && b->mono_count == 0) {
        if (polynomials_equal(a->base, b->base)) {
            Polynomial *new_exp = add_polynomials(a->exponent, b->exponent);
            Polynomial *res = create_power_expression(a->base, new_exp);
            free_polynomial(new_exp);
            return res;
        } else {
            fprintf(stderr, "[ERROR] Multiplication of unsupported formal power expressions\n");
            return create_polynomial_from_number(0);
        }
    }
    if (a->mono_count == 0 && b->mono_count != 0) {
        int k = monomial_degree_if_matches_base(a->base, b);
        if (k >= 0) {
            Polynomial *kpoly = create_polynomial_from_number(k);
            Polynomial *new_exp = add_polynomials(a->exponent, kpoly);
            free_polynomial(kpoly);
            Polynomial *res = create_power_expression(a->base, new_exp);
            free_polynomial(new_exp);
            return res;
        } else {
            fprintf(stderr, "[ERROR] Multiplication of unsupported formal power expressions\n");
            return create_polynomial_from_number(0);
        }
    }
    if (b->mono_count == 0 && a->mono_count != 0) {
        int k = monomial_degree_if_matches_base(b->base, a);
        if (k >= 0) {
            Polynomial *kpoly = create_polynomial_from_number(k);
            Polynomial *new_exp = add_polynomials(b->exponent, kpoly);
            free_polynomial(kpoly);
            Polynomial *res = create_power_expression(b->base, new_exp);
            free_polynomial(new_exp);
            return res;
        } else {
            fprintf(stderr, "[ERROR] Multiplication of unsupported formal power expressions\n");
            return create_polynomial_from_number(0);
        }
    }
    Polynomial *res = calloc(1, sizeof(Polynomial));
    for (int i = 0; i < a->mono_count; ++i) {
        for (int j = 0; j < b->mono_count; ++j) {
            if (res->mono_count >= 64) break;
            Monomial m = multiply_monomials_internal(&a->monomials[i], &b->monomials[j]);
            res->monomials[res->mono_count++] = m;
        }
    }
    simplify_polynomial(res);
    return res;
}

Polynomial *divide_polynomials(const Polynomial *a, const Polynomial *b, Polynomial **remainder) {
    if (remainder) *remainder = NULL;
    if (!a || !b) return create_polynomial_from_number(0);
    if (a->mono_count == 0 && b->mono_count == 0) {
        if (polynomials_equal(a->base, b->base)) {
            Polynomial *new_exp = subtract_polynomials(a->exponent, b->exponent);
            Polynomial *res = create_power_expression(a->base, new_exp);
            free_polynomial(new_exp);
            return res;
        } else {
            fprintf(stderr, "[ERROR] Division of unsupported formal power expressions\n");
            return create_polynomial_from_number(0);
        }
    }
    if (a->mono_count == 0 && b->mono_count != 0) {
        int k = monomial_degree_if_matches_base(a->base, b);
        if (k >= 0) {
            Polynomial *kpoly = create_polynomial_from_number(k);
            Polynomial *new_exp = subtract_polynomials(a->exponent, kpoly);
            free_polynomial(kpoly);
            Polynomial *res = create_power_expression(a->base, new_exp);
            free_polynomial(new_exp);
            return res;
        } else {
            fprintf(stderr, "[ERROR] Division of unsupported formal power expressions\n");
            return create_polynomial_from_number(0);
        }
    }
    if (b->mono_count == 0 && a->mono_count != 0) {
        int k = monomial_degree_if_matches_base(b->base, a);
        if (k >= 0) {
            Polynomial *kpoly = create_polynomial_from_number(k);
            Polynomial *new_exp = subtract_polynomials(kpoly, b->exponent);
            free_polynomial(kpoly);
            Polynomial *res = create_power_expression(b->base, new_exp);
            free_polynomial(new_exp);
            return res;
        } else {
            fprintf(stderr, "[ERROR] Division of unsupported formal power expressions\n");
            return create_polynomial_from_number(0);
        }
    }
    if (b->mono_count == 0 && b->monomials[0].var_count == 0 && b->monomials[0].coefficient == 0) {
        fprintf(stderr, "[ERROR] Division by zero constant!\n");
        if (remainder) *remainder = copy_polynomial(a);
        return create_polynomial_from_number(0);
    }
    if (is_constant(a) && is_constant(b)) {
        int q = a->monomials[0].coefficient / b->monomials[0].coefficient;
        int r = a->monomials[0].coefficient % b->monomials[0].coefficient;
        Polynomial *quot = create_polynomial_from_number(q);
        if (remainder) *remainder = create_polynomial_from_number(r);
        return quot;
    }
    Polynomial *quotient = calloc(1, sizeof(Polynomial));
    Polynomial *r = copy_polynomial(a);
    simplify_polynomial(r);
    while (r->mono_count > 0) {
        Monomial lead_r = r->monomials[0];
        Monomial lead_b = b->monomials[0];
        bool can_divide = true;
        for (int i = 0; i < lead_b.var_count; ++i) {
            bool found = false;
            for (int j = 0; j < lead_r.var_count; ++j) {
                if (strcmp(lead_r.variables[j].name, lead_b.variables[i].name) == 0 &&
                    lead_r.variables[j].degree >= lead_b.variables[i].degree) {
                    found = true;
                    break;
                }
            }
            if (!found) { can_divide = false; break; }
        }
        if (!can_divide) break;
        Monomial q_mono;
        memset(&q_mono, 0, sizeof(Monomial));
        q_mono.coefficient = lead_r.coefficient / lead_b.coefficient;
        int idx = 0;
        for (int i = 0; i < lead_r.var_count && idx < 8; ++i) {
            strncpy(q_mono.variables[idx].name, lead_r.variables[i].name, 15);
            q_mono.variables[idx].name[15] = '\0';
            q_mono.variables[idx].degree = lead_r.variables[i].degree;
            for (int j = 0; j < lead_b.var_count; ++j) {
                if (strcmp(lead_r.variables[i].name, lead_b.variables[j].name) == 0) {
                    q_mono.variables[idx].degree -= lead_b.variables[j].degree;
                }
            }
            idx++;
        }
        q_mono.var_count = idx;
        if (quotient->mono_count < 64) quotient->monomials[quotient->mono_count++] = q_mono;
        Polynomial *temp = calloc(1, sizeof(Polynomial));
        temp->monomials[0] = q_mono;
        temp->mono_count = 1;
        Polynomial *product = multiply_polynomials(temp, b);
        Polynomial *new_r = subtract_polynomials(r, product);
        free_polynomial(r);
        free_polynomial(temp);
        free_polynomial(product);
        r = new_r;
        simplify_polynomial(r);
    }
    simplify_polynomial(quotient);
    simplify_polynomial(r);
    if (remainder) *remainder = r;
    else free_polynomial(r);
    return quotient;
}

bool is_constant(const Polynomial *p) {
    if (!p) return false;
    return p->mono_count == 1 && p->monomials[0].var_count == 0;
}

Polynomial *power_polynomial(const Polynomial *base, const Polynomial *exponent) {
    if (!base || !exponent) return create_polynomial_from_number(0);
    if (is_constant(exponent)) {
        int exp_val = exponent->monomials[0].coefficient;
        if (exp_val < 0) {
            fprintf(stderr, "[ERROR] Negative exponent not supported\n");
            return create_polynomial_from_number(0);
        }
        if (exp_val == 0) return create_polynomial_from_number(1);
        if (exp_val == 1) return copy_polynomial(base);
        Polynomial *result = create_polynomial_from_number(1);
        Polynomial *base_copy = copy_polynomial(base);
        int e = exp_val;
        while (e > 0) {
            if (e & 1) {
                Polynomial *tmp = multiply_polynomials(result, base_copy);
                free_polynomial(result);
                result = tmp;
            }
            e >>= 1;
            if (e) {
                Polynomial *tmp2 = multiply_polynomials(base_copy, base_copy);
                free_polynomial(base_copy);
                base_copy = tmp2;
            }
        }
        free_polynomial(base_copy);
        return result;
    }
    return create_power_expression(base, exponent);
}

static void print_poly_inline(const Polynomial *p) {
    if (!p) return;
    if (p->mono_count == 0) {
        printf("(");
        if (p->base) print_poly_inline(p->base);
        printf(")^( ");
        if (p->exponent) print_poly_inline(p->exponent);
        printf(")");
        return;
    }
    for (int i = 0; i < p->mono_count; ++i) {
        Monomial m = p->monomials[i];
        if (i > 0 && m.coefficient > 0) printf("+");
        if (m.coefficient != 1 || m.var_count == 0) {
            if (m.coefficient != -1 || m.var_count == 0) printf("%d", m.coefficient);
            else printf("-");
        }
        for (int j = 0; j < m.var_count; ++j) {
            printf("%s", m.variables[j].name);
            if (m.variables[j].degree > 1) printf("^%d", m.variables[j].degree);
        }
    }
}

void print_polynomial(const Polynomial *p) {
    if (!p) { printf("0\n"); return; }
    print_poly_inline(p);
    printf("\n");
}

int evaluate_polynomial(const Polynomial *p) {
    if (!p || p->mono_count == 0) return INT_MIN;
    int result = 0;
    for (int i = 0; i < p->mono_count; ++i) {
        Monomial m = p->monomials[i];
        int term_value = m.coefficient;
        for (int j = 0; j < m.var_count; ++j) {
            bool found = false;
            for (int k = 0; k < poly_var_count; ++k) {
                if (strcmp(poly_vars[k].name, m.variables[j].name) == 0 && poly_vars[k].is_defined) {
                    term_value *= (int)pow(poly_vars[k].value, m.variables[j].degree);
                    found = true;
                    break;
                }
            }
            if (!found) return INT_MIN;
        }
        result += term_value;
    }
    return result;
}

void free_polynomial(Polynomial *p) {
    if (!p) return;
    if (p->mono_count == 0) {
        if (p->base) free_polynomial(p->base);
        if (p->exponent) free_polynomial(p->exponent);
    }
    free(p);
}
