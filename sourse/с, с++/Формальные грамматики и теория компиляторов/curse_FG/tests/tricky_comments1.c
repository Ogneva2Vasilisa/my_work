/* tricky_comments1.c
   Трюки с комментариями: комментарии между токенами, // внутри строк, много подряд ';'. */


void test_comments(void) {
    int/*inline-comment*/a = 1;;;;  // несколько ';' подряд после объявления
    ; // дополнительный пустой оператор

    printf("a = %d\n", a);

    // Комментарий // внутри строки — но внутри строк он не закрывает строковый литерал:
    printf("This is not a comment: // appears inside the string\n");

    // Комментарий может содержать кавычки и слэши, это не ломает парсер:
    /* Complex comment: "quoted text" and // and /* inner? still OK as not nested */

    // Комментарий между ключевым словом и идентификатором:
    int /* type comment */ b = 2;
    printf("b = %d\n", b);

    // Комментарий между идентификатором и оператором вызова:
    // пример: вызов с комментарием между именем и скобкой (законно, парсер должен это принять)
    printf /*call*/ ("hello after comment\n");

    // Метки с комментариями и пустыми операторами:
label_one: /* label comment */ ; ;  /* label: then two empty statements */
    printf("After label_one\n");

    // Комментарий может следовать сразу после '*/' и тут же код:
    /* end comment */ printf("Code after comment\n");
}

int main(void) {
    test_comments();
    return 0;
}
