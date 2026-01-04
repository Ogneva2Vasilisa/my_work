// объявление внешней переменной (потом пустой оператор)
extern int external_variable;; // дважды ';' — второй обозначает пустой оператор

/* Тест 1: пустые операторы и множественные ';' */
void test_empty_statements(void) {
    PRINT_TITLE("Empty / multiple semicolons and labels");
    int a = 0;;;; // несколько подряд: допустимо, лишние — это пустые операторы
    ;
    printf("Start: a=%d\n", a);

    label_one: ;; // метка на пустом операторе, затем дополнительный пустой оператор
    printf("After label_one\n");

    goto label_two;
    printf("This line is skipped by goto\n");
    label_two: ; // метка и пустой оператор
    printf("Reached label_two (OK)\n");
}

/* Тест 2: if/else и точка с запятой — пустое тело vs. блок */
void test_if_else_semicolons(void) {
    PRINT_TITLE("if/else tricky semicolons");
    int x = 0;
    if (x == 0) ; else { printf("else branch (should not print)\n"); } // if с пустым оператором
    printf("After if with empty statement\n");

    if (1) {
        printf("if true block\n");
    } // нельзя вставлять ';' BETWEEN block and else: `if(1){ } ; else {}` — это ошибочно
    // Правильный вариант: if (1) { } else { }
    if (0) ; else printf("if(0) ; else single statement\n");
}

/* Тест 3: for/while/do-while — пустые и с точкой с запятой в необычных местах */
void test_loops_semicolons(void) {
    PRINT_TITLE("for/while/do-while with empty statements and semicolons");
    int i = 0;

    // for с пустой частью и пустым телом (но обязательно break, иначе вечный цикл)
    for (i = 0; i < 1; ++i) ; // тело — пустой оператор
    printf("After for(... ) ; (empty body)\n");

    // for(;;) — бесконечный цикл; используем break внутри, тело — пустой оператор с блоком
    for (int j = 0;;) {
        j++;
        if (j > 0) { /* body */ break; } ;
    }
    printf("Broken out of for(;;) safely\n");

    // пустой while — создает цикл, но мы ставим условие false
    while (0) ; // never runs
    printf("while(0) ; completed (never entered body)\n");

    // do-while должен заканчиваться ';'
    int k = 0;
    do {
        k++;
    } while (0);
    printf("do { ... } while(0); executed once, k=%d\n", k);
}

/* Тест 4: switch/case и пустые операторы после меток */
void test_switch_case_semicolons(void) {
    PRINT_TITLE("switch/case with empty statements");
    int v = 1;
    switch (v) {
        case 0:
            printf("case 0\n");
            break;
        case 1:
            ; // пустой оператор сразу после case
            printf("case 1 (after an empty statement)\n");
            // намеренный падение вниз
        case 2:
            printf("case 2 (fall-through)\n");
            break;
        default:
            printf("default\n");
    }
}

/* Тест 5: объявления struct/typedef/enum — обязательные ';' */
void test_struct_typedef_semicolons(void) {
    PRINT_TITLE("struct / typedef / enum and semicolons");
    struct S { int a; }; // структура обязана заканчиваться ';'
    struct S s1 = { .a = 42 };
    printf("struct S created, s1.a=%d\n", s1.a);

    enum E { E1, E2 }; // enum тоже завершается ';'
    enum E e = E2;
    printf("enum E value = %d\n", e);
}

/* Тест 6: декларации функций и точки с запятой */
void declared_function(void); // прототип, требует ';'
void declared_function(void) { printf("declared_function definition executed\n"); } // определение — без ';' после блочного тела

/* Тест 7: макросы и точка с запятой при использовании */
void test_macros_semicolons(void) {
    PRINT_TITLE("Macro invocation and semicolons");
    SAFE_MACRO(printf("SAFE_MACRO executed\n"));
    UNSAFE_MACRO(printf("UNSAFE_MACRO executed without do/while wrapper\n"));
    WEIRD_MACRO_WITH_SEMICOLON; // макрос включает фигурные скобки и внутренний ';'
}

/* Тест 8: вложенные пустые операторы и их влияние на поток */
void test_nested_empty(void) {
    PRINT_TITLE("Nested empty statements and blocks");
    ;; ;; // несколько пустых операторов подряд
    {
        ;; // пустые
        printf("Inside block after multiple empty statements\n");
    };
    ; // пустой оператор после блока
    printf("After block with trailing semicolon\n");
}

/* Тест 9: использование ';' в for-control (пустые выражения) */
void test_for_control_semicolons(void) {
    PRINT_TITLE("for-control empty expressions");
    int sum = 0;
    for (int i = 0; i < 5; /* empty third expr */ ) {
        sum += i;
        i++; // manual increment
    }
    printf("sum after for with empty 3rd expression = %d\n", sum);

    // все выражения пустые — for(;;) handled earlier
}

/* Тест 10: деление деклараций и операторов (точка с запятой отделяет) */
void test_declaration_vs_statement(void) {
    PRINT_TITLE("Declaration vs statement separation by semicolon");
    int x = 10; // объявление + ';'
    x; // выражение-стейтмент без эффекта (валидно — просто выражение, завршенное ';')
    printf("x declared and then expression-statement 'x;' evaluated (no-op), x=%d\n", x);
}

int main(void) {
    printf("Tricky semicolons test program (valid C11 constructs)\n");

    test_empty_statements();
    test_if_else_semicolons();
    test_loops_semicolons();
    test_switch_case_semicolons();
    test_struct_typedef_semicolons();
    declared_function();
    test_macros_semicolons();
    test_nested_empty();
    test_for_control_semicolons();
    test_declaration_vs_statement();

    printf("\nAll tests completed.\n");
    return 0;
}
