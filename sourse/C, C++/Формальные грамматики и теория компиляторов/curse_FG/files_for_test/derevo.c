enum Token {
    SYMBOL = 0,
    NUMBER,          // 1
    EXPRESSION = 10,
    OPERATOR,        // 11
    UNDEFINED        // 12
};

union MyUnion {
    int i;
    float f;
    char c;
};

struct Variant {
    DataType type;
    union k {
        int i;
        float f;
    };
};

int main(void){
   enum Token t = 1;
   union MyUnion u, a, *b;
   u.i = 1L;    // Записываем int
   u.f = 3.14;  // Перезаписываем float, память общая
   union MyUnion {
    int i;
    float f;
    char c;
};
   for (i; i<1; ++i){++I;}
   x >> 1;
   static char a = "a";
   //head=(PINFO)malloc(sizeof(struct in));
   switch(x){
    case QUIT: i++;
   }
   if(((((((((((((((i))))))))))))))){}
}
