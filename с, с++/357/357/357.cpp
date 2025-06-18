#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#define _CRT_SECURE_DEPRECATE_MEMORY
#include <memory.h>
#include <string.h>
#include <set>
#include <math.h>
#include <limits.h>

//добавение элемента в очередь
typedef struct ls {
    unsigned long long int data; // поле данных какого-либо произвольного типа
    struct ls* next; // ls – линейный список, next – указатель на ls (структуру  л. с.)
};

ls* lin_list3 = (ls*)calloc(1, sizeof(ls));
ls* lin_list5 = (ls*)calloc(1, sizeof(ls));
ls* lin_list7 = (ls*)calloc(1, sizeof(ls));
ls* tail3 = lin_list3;
ls* tail5 = lin_list5;
ls* tail7 = lin_list7;
unsigned long long int massiv[5000];
unsigned long long int last = 7;
FILE* yy;
int over = 0;
int K = 4;

void PRINT(FILE* t1);
short prov(unsigned long long int te) {
    int i1 = 1;
    while (massiv[i1] != 0) {
        //printf("==mas[%d] %llu==\n", i1, massiv[i1]);
        if (massiv[i1] == te) return 1;
        i1++;
    }
    return 0;
}

void InsertionSort1(int n, unsigned long long int mass[])
{
    unsigned long long int newElement, location;

    for (int i = 1; i < n; i++)
    {
        newElement = mass[i];
        location = i - 1;
        while (location >= 0 && mass[location] > newElement)
        {
            mass[location + 1] = mass[location];
            location = location - 1;
        }
        mass[location + 1] = newElement;
    }
}

unsigned long long int  getelem3()
{
    unsigned long long int elem = lin_list3->data;
    ls* list1;
    list1 = (lin_list3);
    //free(list);
    lin_list3 = lin_list3->next;
    free(list1);
    return elem;
}
unsigned long long int  getelem5()
{
    unsigned long long int elem = lin_list5->data;
    ls* list1;
    list1 = (lin_list5);
    //free(list);
    lin_list5 = lin_list5->next;
    free(list1);
    return elem;
}
unsigned long long int  getelem7()
{
    unsigned long long int elem = lin_list7->data;
    ls* list1;
    list1 = (lin_list7);
    //free(list);
    lin_list7 = lin_list7->next;
    free(list1);
    return elem;
}
//старый вариант, сделаю минус время, плюс память
ls* addelem1(ls* head, ls* tail1, unsigned long long int data)
{
    if (data < last) { return tail1; }//если число меньше того, что взяли
    printf("\ndata--- %llu ---\n", data);
    if (data > tail1->data)//если data классически больше tail
    {
        ls* New_elem = (ls*)malloc(sizeof(ls));
        //New_elem = tail1;
        New_elem->data = data;
        New_elem->next = NULL;
        tail1->next = New_elem;
        tail1 = New_elem;
        return New_elem;
    }
    else if (prov(data))//проверка, что число есть до этого в цепочке - ничего не делаем
    {
        return tail1;
    }
    else
    {
        //вставка в нужное место, если число меньше, чем хвост
        //t = lin_list7;
        if (head->data > data) {//если надо вставить до head
            ls* New_elem = (ls*)malloc(sizeof(ls));
            //New_elem = tail1;
            New_elem->data = data;
            New_elem->next = head;
            head = New_elem;
            return tail1;
        }

        ls* t = head;
        while (t->next != NULL) {
            if (t->next->data > data && t->data < data)
            {
                ls* New_elem = (ls*)malloc(sizeof(ls));
                //New_elem = tail1;
                New_elem->data = data;
                New_elem->next = t->next;
                t->next = New_elem;

                return tail1;
            }
            t = t->next;
            if (t != NULL)
                if (t->next->data > data && t->data < data)
                {
                    ls* New_elem = (ls*)malloc(sizeof(ls));
                    //New_elem = tail1;
                    New_elem->data = data;
                    New_elem->next = t->next;
                    t->next = New_elem;

                    return tail1;
                }
            t = t->next;
        }
        printf("head--- %llu tail---%llu-- t---%llu", head, tail1, t->data);
        fprintf(yy, "--- %llu ---", head);
        PRINT(yy);
        exit(3);// не знаю что
    }

}

ls* addelem(ls* head, ls* tail1, unsigned long long int data1, unsigned long long int temp)
{
    unsigned long long int data = data1 * temp;
    if (data1== 4470348358154296875 || data1 == 2682209014892578125)
        printf("");
    if (data / temp != data1) {
        over = 1;
        printf("==%d ==",temp);

        return tail1;
    }
    //printf("\ndata--- %llu ---\n", data);
    if (prov(data))//проверка, что число есть до этого в цепочке - ничего не делаем
    {
        return tail1;
    }
    else
    {
        //вставка в нужное место, если число меньше, чем хвост
        massiv[K] = data;
        K++;
        //если надо вставить до head
        if (head->data > data) {
            ls* New_elem = (ls*)malloc(sizeof(ls));
            //New_elem = tail1;
            New_elem->data = data;
            New_elem->next = head;
            head = New_elem;
            return tail1;
        }

        ls* t = head;//счетчик
        while (t->next != NULL) {
            //printf("head--- %llu\ntail--- %llu\n   t--- %llu\n", head->data, tail1->data, t->data);
            if (t->next->data > data && t->data < data)
            {
                ls* New_elem = (ls*)malloc(sizeof(ls));
                //New_elem = tail1;
                New_elem->data = data;
                New_elem->next = t->next;
                t->next = New_elem;

                return tail1;
            }
            t = t->next;
        }
        //printf("head--- %llu\ntail--- %llu\n   t--- %llu\n", head->data, tail1->data, t->data);
        if (t->data < data)
        {
            ls* New_elem = (ls*)malloc(sizeof(ls));
            //New_elem = tail1;
            New_elem->data = data;
            New_elem->next = NULL;
            t->next = New_elem;
            tail1 = t;
            return tail1;
        }

        printf("\ndata--- %llu ---\n", data);
        printf("head--- %llu\ntail--- %llu\n   t--- %llu\n", head->data, tail1->data, t->data);
        fprintf(yy, "--- %llu ---", head);
        PRINT(yy);
        exit(3);// не знаю что
    }

}

unsigned long long int minna() {


    //if ((lin_list3->data > last && lin_list5->data > last) && lin_list7->data > last)
    {
        //if(prov_over())
        if (lin_list3->data < lin_list5->data && lin_list3->data < lin_list7->data)
        {
            if (lin_list3->next == NULL)
            {
                tail3 = addelem(lin_list3, tail3, lin_list3->data, (unsigned long long int)powl(3, 1));
                tail3 = addelem(lin_list3, tail3, lin_list5->data, (unsigned long long int)powl(3, 1));
                tail3 = addelem(lin_list3, tail3, lin_list7->data, (unsigned long long int)powl(3, 1));
            }
            //проблема в tail
            last = getelem3();
            tail3 = addelem(lin_list3, tail3, last, 3);
            tail5 = addelem(lin_list5, tail5, last, 5);
            tail7 = addelem(lin_list7, tail7, last, 7);
            
            return last;
        }
        if (lin_list5->data < lin_list3->data && lin_list5->data < lin_list7->data)
        {
            if (lin_list5->next == NULL)
            {
                tail5 = addelem(lin_list5, tail5, lin_list3->data, (unsigned long long int)powl(5, 1));
                tail5 = addelem(lin_list5, tail5, lin_list5->data, (unsigned long long int)powl(5, 1));
                tail5 = addelem(lin_list5, tail5, lin_list7->data, (unsigned long long int)powl(5, 1));
            }
            last = getelem5();
            tail3 = addelem(lin_list3, tail3, last, 3);
            tail5 = addelem(lin_list5, tail5, last, 5);
            tail7 = addelem(lin_list7, tail7, last, 7);
            
            return last;
        }
        if (lin_list7->data < lin_list5->data && lin_list7->data < lin_list3->data)
        {
            if (lin_list7->next == NULL)
            {
                tail7 = addelem(lin_list7, tail7, lin_list3->data, (unsigned long long int)powl(7, 1));
                tail7 = addelem(lin_list7, tail7, lin_list5->data, (unsigned long long int)powl(7, 1));
                tail7 = addelem(lin_list7, tail7, lin_list7->data, (unsigned long long int)powl(7, 1));
            }
            last = getelem7();
            tail3 = addelem(lin_list3, tail3, last, 3);
            tail5 = addelem(lin_list5, tail5, last, 5);
            tail7 = addelem(lin_list7, tail7, last, 7);
            
            return last;
        }
        //=================================================================================================================
        if (lin_list3->data == lin_list5->data && lin_list3->data < lin_list7->data)
        {
            if (lin_list3->next == NULL)
            {
                tail3 = addelem(lin_list3, tail3, lin_list3->data, (unsigned long long int)powl(3, 1));
                tail3 = addelem(lin_list3, tail3, lin_list5->data, (unsigned long long int)powl(3, 1));
                tail3 = addelem(lin_list3, tail3, lin_list7->data, (unsigned long long int)powl(3, 1));
            }
            if (lin_list5->next == NULL)
            {
                tail5 = addelem(lin_list5, tail5, lin_list3->data, (unsigned long long int)powl(5, 1));
                tail5 = addelem(lin_list5, tail5, lin_list5->data, (unsigned long long int)powl(5, 1));
                tail5 = addelem(lin_list5, tail5, lin_list7->data, (unsigned long long int)powl(5, 1));
            }
            last = getelem5();
            getelem3();
            tail3 = addelem(lin_list3, tail3, last, 3);
            tail5 = addelem(lin_list5, tail5, last, 5);
            tail7 = addelem(lin_list7, tail7, last, 7);
            return last;
        }
        if (lin_list3->data == lin_list7->data && lin_list7->data < lin_list5->data)
        {
            if (lin_list7->next == NULL)
            {
                tail7 = addelem(lin_list7, tail7, lin_list3->data, (unsigned long long int)powl(7, 1));
                tail7 = addelem(lin_list7, tail7, lin_list5->data, (unsigned long long int)powl(7, 1));
                tail7 = addelem(lin_list7, tail7, lin_list7->data, (unsigned long long int)powl(7, 1));
            }
            if (lin_list3->next == NULL)
            {
                tail3 = addelem(lin_list3, tail3, lin_list3->data, (unsigned long long int)powl(3, 1));
                tail3 = addelem(lin_list3, tail3, lin_list5->data, (unsigned long long int)powl(3, 1));
                tail3 = addelem(lin_list3, tail3, lin_list7->data, (unsigned long long int)powl(3, 1));
            }
            last = getelem3();
            getelem7();
            tail3 = addelem(lin_list3, tail3, last, 3);
            tail5 = addelem(lin_list5, tail5, last, 5);
            tail7 = addelem(lin_list7, tail7, last, 7);
            
            return last;
        }
        if (lin_list5->data == lin_list7->data && lin_list7->data < lin_list3->data)
        {
            if (lin_list7->next == NULL)
            {
                tail7 = addelem(lin_list7, tail7, lin_list3->data, (unsigned long long int)powl(7, 1));
                tail7 = addelem(lin_list7, tail7, lin_list5->data, (unsigned long long int)powl(7, 1));
                tail7 = addelem(lin_list7, tail7, lin_list7->data, (unsigned long long int)powl(7, 1));
            }
            if (lin_list5->next == NULL)
            {
                tail5 = addelem(lin_list5, tail5, lin_list3->data, (unsigned long long int)powl(5, 1));
                tail5 = addelem(lin_list5, tail5, lin_list5->data, (unsigned long long int)powl(5, 1));
                tail5 = addelem(lin_list5, tail5, lin_list7->data, (unsigned long long int)powl(5, 1));
            }
            last = getelem5();
            getelem7();
            tail3 = addelem(lin_list3, tail3, last, 3);
            tail5 = addelem(lin_list5, tail5, last, 5);
            tail7 = addelem(lin_list3, tail7, last, 7);
            
            return last;
        }
        if (lin_list5->data == lin_list7->data && lin_list7->data == lin_list3->data)
        {
            if (lin_list7->next == NULL)
            {
                tail7 = addelem(lin_list7, tail7, lin_list3->data, (unsigned long long int)powl(7, 1));
                tail7 = addelem(lin_list7, tail7, lin_list5->data, (unsigned long long int)powl(7, 1));
                tail7 = addelem(lin_list7, tail7, lin_list7->data, (unsigned long long int)powl(7, 1));
            }
            if (lin_list5->next == NULL)
            {
                tail5 = addelem(lin_list5, tail5, lin_list3->data, (unsigned long long int)powl(5, 1));
                tail5 = addelem(lin_list5, tail5, lin_list5->data, (unsigned long long int)powl(5, 1));
                tail5 = addelem(lin_list5, tail5, lin_list7->data, (unsigned long long int)powl(5, 1));
            }
            if (lin_list3->next == NULL)
            {
                tail3 = addelem(lin_list3, tail3, lin_list3->data, (unsigned long long int)powl(3, 1));
                tail3 = addelem(lin_list3, tail3, lin_list5->data, (unsigned long long int)powl(3, 1));
                tail3 = addelem(lin_list3, tail3, lin_list7->data, (unsigned long long int)powl(3, 1));
            }
            last = getelem5();
            getelem7();
            getelem3();
            tail3 = addelem(lin_list3, tail3, last, 3);
            tail5 = addelem(lin_list5, tail5, last, 5);
            tail7 = addelem(lin_list7, tail7, last, 7);
            
            return last;
        }
    }
    /*
    else {
        if (lin_list3->data <= last) {
            if (lin_list3->next == NULL)
            {
                tail3 = addelem(lin_list3, tail3, lin_list3->data, (unsigned long long int)powl(3, 1));
                tail3 = addelem(lin_list3, tail3, lin_list5->data, (unsigned long long int)powl(3, 1));
                tail3 = addelem(lin_list3, tail3, lin_list7->data, (unsigned long long int)powl(3, 1));
            }
            getelem3();
            if (lin_list3->next == NULL)
            {
                tail3 = addelem(lin_list3, tail3, lin_list3->data, (unsigned long long int)powl(3, 1));
                tail3 = addelem(lin_list3, tail3, lin_list5->data, (unsigned long long int)powl(3, 1));
                tail3 = addelem(lin_list3, tail3, lin_list7->data, (unsigned long long int)powl(3, 1));
            }
        }
        if (lin_list5->data <= last) {
            if (lin_list5->next == NULL)
            {
                tail5 = addelem(lin_list5, tail5, lin_list3->data, (unsigned long long int)powl(5, 1));
                tail5 = addelem(lin_list5, tail5, lin_list5->data, (unsigned long long int)powl(5, 1));
                tail5 = addelem(lin_list5, tail5, lin_list7->data, (unsigned long long int)powl(5, 1));
            }
            getelem5();
            if (lin_list5->next == NULL)
            {
                tail5 = addelem(lin_list5, tail5, lin_list3->data, (unsigned long long int)powl(5, 1));
                tail5 = addelem(lin_list5, tail5, lin_list5->data, (unsigned long long int)powl(5, 1));
                tail5 = addelem(lin_list5, tail5, lin_list7->data, (unsigned long long int)powl(5, 1));
            }
        }
        if (lin_list7->data <= last) {
            if (lin_list7->next == NULL)
            {
                tail7 = addelem(lin_list7, tail7, lin_list3->data, (unsigned long long int)powl(7, 1));
                tail7 = addelem(lin_list7, tail7, lin_list5->data, (unsigned long long int)powl(7, 1));
                tail7 = addelem(lin_list7, tail7, lin_list7->data, (unsigned long long int)powl(7, 1));
            }
            getelem7();
            if (lin_list7->next == NULL)
            {
                tail7 = addelem(lin_list7, tail7, lin_list3->data, (unsigned long long int)powl(7, 1));
                tail7 = addelem(lin_list7, tail7, lin_list5->data, (unsigned long long int)powl(7, 1));
                tail7 = addelem(lin_list7, tail7, lin_list7->data, (unsigned long long int)powl(7, 1));
            }
        }
        return minna();
    }
    */
}

void PRINT(FILE* t1) {
    fprintf(t1, "\n________________________________________________________________________________\n");
    ls* t = lin_list3;
    while (t->next != NULL) {
        fprintf(t1, "%llu -> ", t->data);
        t = t->next;
    }
    fprintf(t1, "\n !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! \n");
    t = lin_list5;
    while (t->next != NULL) {
        fprintf(t1, "%llu -> ", t->data);
        t = t->next;
    }
    fprintf(t1, "\n !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! \n");
    t = lin_list7;
    while (t->next != NULL) {
        fprintf(t1, "%llu -> ", t->data);
        t = t->next;
    }
    fprintf(t1, "\n________________________________________________________________________________\n");
    fprintf(t1, "\n");

}


int main() {

    lin_list3->data = 3;
    lin_list3->next = NULL;
    lin_list5->data = 5;
    lin_list5->next = NULL;
    lin_list7->data = 7;
    lin_list7->next = NULL;
    memset(massiv, 0, 5000);
    int h = 0;
    scanf("%d", &h);
    if (h == 1) { printf("1 3"); return 0; }
    if (h == 2) { printf("2 5"); return 0; }
    if (h == 3) { printf("3 7"); return 0; }
    massiv[1] = 3;
    massiv[2] = 5;
    massiv[3] = 7;
    yy = fopen("testy.txt", "w");
    fputs("1 3 \n2 5 \n3 7\n", yy);

    //здесь начинается от 4-того элемента
    int k = 4;

    //h = 4924;
    unsigned long long int ooo = minna();
    for (k = 4; k <= h; k++) {
        //PRINT(yy);
        printf("%d %llu\n", k, ooo);
        //massiv[k] = ooo;
        fprintf(yy, "%d %llu\n", k, ooo);
        ooo = minna();
        //if (over) {
        if (false) {
            //ooo = minna(); //continue;
            //int i = 1;
            if (lin_list3 == NULL && lin_list5 == NULL && lin_list7 == NULL) {
                printf("END"); goto lop;
                exit(56);
            }
        }
        //else
        {          
                //ooo = minna();           
            int i = 1;
            if ((lin_list3 == NULL || lin_list5 == NULL || lin_list7 == NULL)) { printf("END"); goto lop; }
            while (lin_list3->next == NULL || lin_list5->next == NULL || lin_list7->next == NULL) {
                if (lin_list3->next == NULL)
                {
                    tail3 = addelem(lin_list3, tail3, lin_list3->data, (unsigned long long int)powl(3, 1));
                    tail3 = addelem(lin_list3, tail3, lin_list5->data, (unsigned long long int)powl(3, 1));
                    tail3 = addelem(lin_list3, tail3, lin_list7->data, (unsigned long long int)powl(3, 1));
                }
                if (lin_list5->next == NULL)
                {
                    tail5 = addelem(lin_list5, tail5, lin_list3->data, (unsigned long long int)powl(5, 1));
                    tail5 = addelem(lin_list5, tail5, lin_list5->data, (unsigned long long int)powl(5, 1));
                    tail5 = addelem(lin_list5, tail5, lin_list7->data, (unsigned long long int)powl(5, 1));
                }
                if (lin_list7->next == NULL)
                {
                    tail7 = addelem(lin_list7, tail7, lin_list3->data, (unsigned long long int)powl(7, 1));
                    tail7 = addelem(lin_list7, tail7, lin_list5->data, (unsigned long long int)powl(7, 1));
                    tail7 = addelem(lin_list7, tail7, lin_list7->data, (unsigned long long int)powl(7, 1));
                }
                i++;
                if (i > 2) { break; }
            }
        }

    }

    for (int i = 0; i <= h; i++) {
        // printf("%d %llu\n", i, massiv[i]);
    }
    //lin_list = (lin_list);
    //printf("%d %llu\n", k, ooo);
    //printf("%d %llu\n", k + 1, tail3->data * 3);
    //printf("%d %llu\n", k + 2, tail5->data * 5);
    //printf("%d %llu\n", k + 3, tail7->data * 7);
    while (lin_list3 != NULL) {
        getelem3();
    }
    while (lin_list5 != NULL) {
        getelem5();
    }
    while (lin_list7 != NULL) {
        getelem7();
    }
    fclose(yy);
    exit(1);
lop:
    while (lin_list3 != NULL) {
        printf("%d %llu\n", k, getelem3());
        k++;
        //pritnf(;)
    }
    while (lin_list5 != NULL) {
        printf("%d %llu\n", k, getelem5());
        k++;
    }
    while (lin_list7 != NULL) {
        printf("%d %llu\n", k, getelem7());
        k++;
    }
    fclose(yy);
    InsertionSort1(K,massiv);
    for (int i = 0; i < K; i++) {
        printf("%d %llu\n", i, massiv[i]);
    }
    exit(1);
}
//
//55 187 244 415 283 203 125
//18 395 748 138 427 734 375 - k=4924
//18 446 744 073 709 551 615 - max
//18 395 748 138 427 734 375
//18 025 897 608 064 828 125 
//2617374298095703125

//18 321 620 086 669 921 875 - после него происходит переполнение

//2635249153387078802 - /7 (?)
//6148914691236517205 - /3
//3689348814741910323 - /5
//10034788569920650697
