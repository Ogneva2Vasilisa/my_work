
int f();              // прототип без параметров (не то же самое, что (void))
int g(void);          // прототип без параметров, именно void
int h(int, double);   // параметры без имён допустимы
int (*fp)(int);       // указатель на функцию

int g(void) { return 42; }

int arr1[10];        // обычный массив
int arr2[] = {1,2,3}; // размер выведен из инициализатора
char str[] = "abc";  // строковый литерал → '\0' автоматически добавлен


struct Node {
    int value;
    struct Node *next;  // рекурсивное определение через указатель
};

union Number {
    int i;
    double d;
};

struct Node n1 = {42, 0};


enum Weekday {
    MON = 1,
    TUE,         // = 2
    WED = 10,
    THU,         // = 11
    FRI, SAT, SUN
};

void f(void) {
    goto again;
    printf("skipped\n");
again:
    printf("jumped here\n");
}


void test_loops(void) {
    for (;;);          // пустой вечный цикл
    while (0);         // пустой while
    do ; while (0);    // do-while пустой
}


// --- 7. if/else ---
void test_if(void) {
    if (1);            // пустой if
    else ;             // пустой else
    if (0) ; else printf("else branch executed\n");
}

// --- 8. Scope и скрытие имён ---
int x = 1;
void test_scope(void) {
    int x = 2;
    {
        int x = 3;
        printf("inner x = %d\n", x);
    }
    printf("outer x = %d\n", x);
}


// --- 9. Многомерные массивы ---
int mat[2][2] = {
    {1, 2},
    {3, 4}
};

// --- 10. Неполные типы ---
struct A;
struct A *p;
struct A { int x; };

// --- 11. sizeof ---
void test_sizeof(void) {
    int arr[10];
    printf("sizeof arr = %zu\n", sizeof arr);
    printf("sizeof arr[0] = %zu\n", sizeof(arr[0]));
    printf("sizeof(1 + 2) = %zu\n", sizeof(1 + 2));
}

// --- 12. break / continue ---
void test_break_continue(void) {
    for (int i = 0; i < 5; i++) {
        if (i == 2) continue;
        if (i == 4) break;
        printf("%d\n", i);
    }
}

// --- main ---
int main(void) {
    printf("Tricky C standard tests\n");

    printf("g() = %d\n", g());
    test_goto();
    test_loops();
    test_if();
    test_scope();
    test_sizeof();
    test_break_continue();

    printf("enum example: MON=%d, THU=%d, SUN=%d\n", MON, THU, SUN);

    return 0;
}