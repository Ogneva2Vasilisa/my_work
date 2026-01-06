/* tricky_enum1.c
   Сложные enum: вычисляемые значения, повторные значения, смешение hex и двоичных значений.
   (Добавь #include <stdio.h> при компиляции или используй предоставленный extern.) */


enum Complex {
    A = 1,
    B = A << 1,          /* 2  */
    C = B | 0x1,         /* 3 (uses hex) */
    E = C + 8,           /* 11 */
    F = E,               /* duplicate value (allowed) */
    G = (int)0x10U,      /* 16 (hex with U suffix) */
    H = /* split by comment */ G + 3 /* comment after expr */
};

enum { X = 0, Y, Z = Y + 5 } Simple;

int main(void) {
    enum Complex v = E;
    printf("A=%d B=%d C=%d D=%d E=%d F=%d G=%d H=%d\n", A, B, C, D, E, F, G, H);

    switch (v) {
        case A: ; /* empty statement after case — valid */
        case B:
            printf("fell into B (or A)\n");
            break;
        case /* comment */ C:  /* comment between tokens */
            printf("case C reached\n");
            break;
        default:
            printf("default\n");
    }

    Simple s = Z;
    printf("Simple s=%d (X=%d Y=%d Z=%d)\n", s, X, Y, Z);
    return 0;
}
