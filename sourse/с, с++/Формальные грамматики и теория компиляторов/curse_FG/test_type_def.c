// #include <stdio.h>

// создаём новый тип для int
typedef int myinp;

extern int y;
// создаём структуру с typedef

typedef struct Students {
char name[50];
char branch[50];
int ID_no;
} stu;



struct p {
    int x;
    int y;
} Point;


// создаём указатель на функцию через typedef
typedef void (*FuncPtr)(int);

// функция, подходящая под typedef
void printValue(int v) {
    printf("Value: %d\n", v);
}

int main() {
    myinp a = 10;           // переменная нового типа
    Point p = {1, 2};       // использование typedef для структуры
    FuncPtr f = printValue; // указатель на функцию
    int x =2;
    int y=0;
    int result = x + y;
    //int result = x ++ y;
    //int result = x  y;
    //int result = (x + y;
    //int result = ((x + y) +20 )*3);

    printf("a = %d\n", a);
    printf("Point: (%d, %d)\n", p.x, p.y);
    f(42);

    return 0;
}
