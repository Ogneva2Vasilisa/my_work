

int *p;                // указатель на int
int (*fp)(int);        // указатель на функцию, принимающую int и возвращающую int
int *ap[5];            // массив указателей на int
int (*pa)[5];          // указатель на массив из 5 int
int f(int a, int b);   // объявление функции
int (*fpa)(int, int);  // указатель на функцию с двумя параметрами
int *(*complex)[3];    // указатель на массив указателей на int

 int add(int a, int b) { return a + b; }

int main(void) {
    int x = 42;
    int y = 7;

    p = &x;
    printf("p points to %d\n", *p);

    // указатель на функцию
   
    fpa = add;
    printf("fpa(2, 3) = %d\n", fpa(2, 3));

    // массив указателей
    int v0 = 10, v1 = 20, v2 = 30, v3 = 40, v4 = 50;
    ap[0] = &v0; ap[1] = &v1; ap[2] = &v2; ap[3] = &v3; ap[4] = &v4;
    for (int i = 0; i < 5; i++) {
        printf("ap[%d] -> %d\n", i, *ap[i]);
    }

    // указатель на массив
    int arr[5] = {100, 200, 300, 400, 500};
    pa = &arr;
    for (int i = 0; i < 5; i++) {
        printf("(*pa)[%d] = %d\n", i, (*pa)[i]);
    }

    // сложный случай: указатель на массив указателей
    int *inners[3];
    int a0 = 111, a1 = 222, a2 = 333;
    inners[0] = &a0; inners[1] = &a1; inners[2] = &a2;
    complex = &inners;
    for (int i = 0; i < 3; i++) {
        printf("(*complex)[%d] -> %d\n", i, *(*complex)[i]);
    }

    return 0;
}
