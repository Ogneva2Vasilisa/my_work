#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <locale.h>
#include <string.h>
#include <time.h>
#include <iostream>
#include <map>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
/*
принцип отбора слова:
после того, как мы собрали все возможные существующие слова, мы отсортировали их по убыванию их веса,
далее, мы выбрали самое большое по весу слова (в массиве будет 0-ым), подбираем к нему другие слова, с более мелким весом, соблюдая след усл:
                1) нет слоеного пирога
                2) слова не должны сливаться (j первого слова не равно i-1 второго)
                3) букв из мешка должно хватить, одна и та же буква, не берется дважды
        если случилось такое, что нам подошло только 1 слово, т.е. самое "тяжелое", то делаем еще одну проверку,
        взяв, в качестве первого слово, не 0-ое, а 1-ое в отсортированном массиве слов


*/
//int st - строка или столбец: 0 или 1,int I - координата начала,int J - координата конца,int number - номер строки или столбца
//using_letters - массив из букв, которые использует комп для дополнения слова
//letter - индекс бувы в этом массиве
FILE* slov;
char A[15][15];
int DOP = 0;
int Pl_1 = 0;
int Pl_2 = 0;
char letters[32] = { 'а','б', 'в', 'г', 'д', 'е', 'ж', 'з', 'и', 'й', 'к', 'л', 'м', 'н',
'о', 'п', 'р', 'с', 'т', 'у', 'ф', 'х', 'ц', 'ч', 'ш', 'щ', 'ъ', 'ы', 'ь', 'э', 'ю', 'я' };
char letters2[103] = { 'а','а','а','а','а','а','а','а','б','б', 'в','в','в','в', 'г','г','д','д','д', 'д', 'е','е','е','е','е','е','е','е','е',
'ж', 'з', 'з', 'и','и','и','и','и','и', 'й', 'к', 'к', 'к', 'к', 'л', 'л', 'л', 'л', 'м','м','м', 'н', 'н', 'н', 'н', 'н',
'о','о','о','о','о','о','о','о','о','о', 'п', 'п', 'п', 'п', 'р','р','р','р' ,'р', 'с', 'с', 'с', 'с', 'с',
'т','т','т','т','т', 'у','у','у','у', 'ф', 'х', 'ц', 'ч', 'ш', 'щ', 'ъ', 'ы','ы', 'ь',  'ь','э', 'ю', 'я','я' };
char player_bank_1[7] = { 'z','z','z','z','z','z','z' };
char player_bank_2[7] = { 'z','z','z','z','z','z','z' };
int propusk_1 = 0;
int propusk_2 = 0;
time_t start;
time_t end;
std::map<char, unsigned> cost = { {'а',1},{'б',3},{'в',1},{'г',3},{'д',2},{'е',1},{'ж',5},{'з',5}, {'и',1},
        {'й',4},{'к',2}, {'л', 2}, {'м',2}, {'н',1},{'о',1},{'п',2},{'р',1},{'с',1},{'т',1},
        {'у',2},{'ф',8},{'х',5},{'ц',5},{'ч',5},{'ш',8},{'щ',10},{'ъ',15},{'ы',4},{'ь',3},{'э',8},{'ю',8},{'я',3} };


std::map<char, unsigned> ostatok = { {'а',8},{'б',2},{'в',4},{'г',2},{'д',4},{'е',9},{'ж',1},{'з',2}, {'и',6},
    {'й',1},{'к',4}, {'л', 4}, {'м',3}, {'н',5},{'о',10},{'п',4},{'р',5},{'с',5},{'т',5},
    {'у',4},{'ф',1},{'х',1},{'ц',1},{'ч',1},{'ш',1},{'щ',1},{'ъ',1},{'ы',2},{'ь',2},{'э',1},{'ю',1},{'я',2} };


char using_letters[7];
//вывод поля на экран
void PRINT() {
    for (int i = 0; i < 15; i++) {
        for (int j = 0; j < 15; j++) {
            if (A[i][j] == 'z') {
                printf("- ");
            }
            else   printf("%c ", A[i][j]);
        }
        printf("\n");
    }
}
//сколько букв осталось
int sum_ost() {
    int c = 0;
    for (int i = 0; i < 32; i++) {
        c += ostatok[letters[i]];
    }
    return c;
}
typedef struct slova {
    int ukaz; // строка или столбец
    int number; //номер строки или столбца
    int i;// координата начала слова
    int j;//координата конца слова
    int massa;//вес слова
    char use_letters[7]; //используемые буквы из кучи
    char slovvo[15];
    int num_z;
    struct slova* next = NULL;
    struct slova* previous = NULL;

}list;

struct slova massive;
struct slova* tail = &massive;

typedef struct letter_bank {
    char letter_b;
    struct letter_bank* next = NULL;
}list2;

struct letter_bank L;
struct letter_bank* l_bank = &L;
void BANK() {
    for (int i = 0; i < 103; i++) {
        l_bank->next = new list2;
        l_bank->next->letter_b = letters2[i];
        l_bank = l_bank->next;
    }
}
int BANK_count = 103;
char Bank_delete(int a) {
    struct letter_bank* t = &L;
    struct letter_bank* t2 = &L;
    int opred = 0;
    char b;
    for (int i = 0; i < a; i++) {
        t2 = t;
        t = t->next;
    }

    if (a < BANK_count - 1) {
        b = t->next->letter_b;
        t->next = t->next->next;

    }
    else {
        b = t->next->letter_b;
        l_bank = t;
        t->next = NULL;
    }

    BANK_count -= 1;

    return b;
}

void clearStruct(slova* tail)
{
    while (tail->next != NULL)
    {
        tail = tail->next;
    }
    while (tail->previous != NULL)
    {
        tail = tail->previous;
        delete tail->next;
        tail->next = NULL;
    }
}
void PutIn1(int str_st, int num, int I, int J, int mass, char letters[7], char slovo[15], int num_z) {

    tail->next = new list;
    tail->next->ukaz = str_st;
    tail->next->number = num;
    tail->next->i = I;
    tail->next->j = J;
    tail->next->massa = mass;
    tail->next->num_z = num_z;
    for (int i = 0; i < 7; i++) {
        tail->next->use_letters[i] = letters[i];
    }
    strcpy(tail->next->slovvo, slovo);

    tail->next->previous = tail;
    tail = tail->next;
    tail->next = NULL;

}
void sorting() {

    struct slova* tail23 = &massive;
    tail23 = &massive;

    int a;
    int b;
    struct slova* dop_current = &massive;
    struct slova* sortstart = dop_current;

    while (sortstart != NULL) {
        tail23 = &massive;
        sortstart = sortstart->next;

        while (tail23->next != NULL && tail23->next->next != NULL) {

            a = tail23->next->massa;
            b = tail23->next->next->massa;

            if (a < b) {
                //printf("%d/", 33);
                dop_current = tail23->next;
                tail23->next = tail23->next->next;
                dop_current->next = tail23->next->next;
                tail23->next->next = dop_current;

                // a = tail23->next->massa;
                 //b = tail23->next->next->massa;
                 // printf("%d %d \n",a,b);
            }
            dop_current = NULL;
            tail23 = tail23->next;
        }
    }
    tail23 = &massive;
    while (tail23->next != NULL) {
        tail23->next->previous = tail23;
        tail23 = tail23->next;
    }
}
int slovar(char sl[15]) {
    //fopen_s(&slov, "slova.txt", "r");
    char dop[17];
    int c = strlen(sl);
    //printf("%d\n", strlen(sl));
    switch (c)
    {
    case 2:
        fopen_s(&slov, "2_new.txt", "r");
        break;
    case 3:
        fopen_s(&slov, "3_new.txt", "r");
        break;
    case 4:
        fopen_s(&slov, "4_new.txt", "r");
        break;
    case 5:
        fopen_s(&slov, "5_new.txt", "r");
        break;
    case 6:
        fopen_s(&slov, "6_new.txt", "r");
        break;
    case 7:
        fopen_s(&slov, "7_new.txt", "r");
        break;
    case 8:
        fopen_s(&slov, "8_new.txt", "r");
        break;
    case 9:
        fopen_s(&slov, "9_new.txt", "r");
        break;
    case 10:
        fopen_s(&slov, "10_new.txt", "r");
        break;
    case 11:
        fopen_s(&slov, "11_new.txt", "r");
        break;
    case 12:
        fopen_s(&slov, "12_new.txt", "r");
        break;
    case 13:
        fopen_s(&slov, "13_new.txt", "r");
        break;
    case 14:
        fopen_s(&slov, "14_new.txt", "r");
        break;
    case 15:
        fopen_s(&slov, "15_new.txt", "r");
        break;
    }

    while (!feof(slov)) {
        fgets(dop, 17, slov);
        c++;
        //printf("%d %d %c %d\n", sl[0], dop[0],dop[0],c);
        if (sl[0] == dop[0]) {
            int t = 0;
            for (int u = 1; u < strlen(sl); u++) {
                t += 1;
                if (sl[u] != dop[u]) {
                    t = -10;
                    break;
                }
            }
            if (t != -10) {
                fclose(slov);
                return 1;
            }
        }
        if ((int)sl[0] < (int)dop[0]) {
            //printf("%d\n", c);
            fclose(slov);
            return 0;
        }

    }
    //printf("%d\n", c);
    fclose(slov);
    return 0;
}
int massa_sl(int num, int I, int J, char sl[15]) {
    int m = 0;//масса всего слова
    int c = 1;// на нее домножается буква
    int c2 = 1;// на нее домножается итоговое слово 

    // не играет значение рассматриваем мы слово в строке или столбце, поле симметрично
    for (int i = I; i <= J; i++) {
        c = 1;
        //условие на синюю клетку
        if ((num == i && ((i >= 1 && i <= 4) || (i >= 10 && i <= 13))) || (num == 15 - 1 - i && ((i >= 1 && i <= 4) || (i >= 10 && i <= 13)))) {
            c2 = 2;
        }
        //условие на красную клетку
        if (((num == 0 || num == 14) && (i == 0 || i == 7 || i == 14)) || (num == 7 && (i == 0 || i == 14))) {
            c2 = 3;
        }
        //условие на зеленую
        if (((num == 0 || num == 14) && (i == 3 || i == 11)) || ((num == 2 || num == 12) && (i == 6 || i == 8)) || ((num == 3 || num == 11) && (i == 0 || i == 7 || i == 14)) || ((num == 6 || num == 8) && (i == 2 || i == 6 || i == 8 || i == 12)) || (num == 7 && (i == 4 || i == 11))) {
            c = 2;
        }
        //желтая клетка
        if (((num == 1 || num == 13) && (i == 5 || i == 9)) || ((num == 5 || num == 9) && (i == 1 || i == 13))) {
            c = 3;
        }
        m += cost[sl[i - I]] * c;

    }
    m *= c2;
    return m;


}
//проверить num_z для проги с низким уровнем
int pirog(int str_st, int num, int I, int J, int num_z) {
    // int t = 0;
    int t1 = 0;
    int t2 = 0;
    int t3 = 0;
    if (str_st == 0) {//если строка
        if (I != 0 && A[num][I - 1] != 'z') {
            return 0;

        }
        if (J != 14 && A[num][J + 1] != 'z') {
            return 0;

        }
        for (int i = I; i <= J; i++) {
            if (num != 0 && A[num - 1][i] != 'z' && t1 == 1) {
                return 0;
            }
            if (num != 0 && A[num - 1][i] != 'z' && t1 == 0) {
                if (A[num][i] != 'z') {
                    t1 = 1;
                }
                else {
                    return 0;
                }

            }
            if (num != 0 && A[num - 1][i] == 'z') {
                t1 = 0;
            }
            if (num != 14 && A[num + 1][i] != 'z' && t2 == 1) {
                return 0;
            }
            if (num != 14 && A[num + 1][i] != 'z' && t2 == 0) {
                if (A[num][i] != 'z') {
                    t2 = 1;
                }
                else {
                    return 0;
                }

            }
            if (num != 14 && A[num + 1][i] == 'z') {
                t2 = 0;
            }
            //чтобы не записывал поверх одного слова другое (условеи ниже)                                               
            if (A[num][i] != 'z') {
                t3 += 1;
            }

        }
        if (t3 != num_z) { return 0; }
        return 1;
    }
    else {
        if (I != 0 && A[I - 1][num] != 'z') {
            return 0;

        }
        if (J != 14 && A[J + 1][num] != 'z') {
            return 0;

        }
        for (int i = I; i <= J; i++) {
            if (num > 0 && A[i][num - 1] != 'z' && t1 == 1) {
                return 0;
            }
            if (num > 0 && A[i][num - 1] != 'z' && t1 == 0) {
                if (A[i][num] != 'z') {
                    t1 = 1;
                }
                else {
                    return 0;
                }
            }
            if (num != 0 && A[i][num - 1] == 'z') {
                t1 = 0;
            }
            if (num < 14 && A[i][num + 1] != 'z' && t2 == 1) {
                return 0;
            }
            if (num < 14 && A[i][num + 1] != 'z' && t2 == 0) {
                if (A[i][num] != 'z') {
                    t2 = 1;
                }
                else {
                    return 0;
                }
            }
            if (num != 14 && A[i][num + 1] == 'z') {
                t2 = 0;
            }
            //ан-но случаю строки (if)
            if (i >= 0 && i <= 14 && A[i][num] != 'z') {
                t3 += 1;
            }

        }
        if (t3 != num_z) { return 0; }
        return 1;
    }
}
int proverka_slova(char sl[15], int uk, int kol, int st, int I, int J, int number, int letter, int num_z, int ukaz, char mas[7]) {
    char t;
    int count = 0;
    int masi = 0;
    setlocale(LC_ALL, ".1251");
    for (int j = 0; j < 7; j++) {
        for (int i = uk; i <= J - I; i++) {

            if (mas[j] != 'z' && (sl[i] == 'z' || sl[i] == 'k')) {
                sl[i] = mas[j];
                if ((i == 0) && (mas[j] == 'ь' || mas[j] == 'ъ' || mas[j] == 'ы')) {
                    break;
                }

                t = mas[j];
                using_letters[letter] = t;
                mas[j] = 'z';
                proverka_slova(sl, i + 1, kol - 1, st, I, J, number, letter + 1, num_z, ukaz, mas);
                //&&((DOP>0&& ukaz%2==1)||ukaz%2==0)
                if (kol == 1 && ((DOP > 0 && ukaz % 2 == 1) || ukaz % 2 == 0)) {
                    //printf("\n%s\n",sl);
                    if (slovar(sl) == 1) {
                        masi = massa_sl(number, I, J, sl);
                        //printf("\n");
                        //printf("--- %d %d %d %d %d\n", masi, st, number, I, J);
                        PutIn1(st, number, I, J, masi, using_letters, sl, num_z);
                        DOP -= 1;
                        //printf("ukaz= %d, DOP = %d слово:%s\n", ukaz % 2, DOP, sl);
                        if (ukaz % 2 == 1 && DOP == 0) {
                            using_letters[letter] = NULL;
                            mas[j] = t;
                            sl[i] = 'z';
                            // time(&end);
                            // printf("%lf - 2\n", difftime(end, start));
                             //printf("6666\n");
                            return 0;
                        }
                    }
                }
                using_letters[letter] = NULL;
                mas[j] = t;
                sl[i] = 'z';
            }
        }
    }
    return -1;
}

void change(char mas[7]) {
    struct letter_bank* t = l_bank;
    int o = 0;
    int c;
    while (o < 7) {
        l_bank->next = new list2;
        l_bank->next->letter_b = mas[o];
        BANK_count += 1;
        l_bank = l_bank->next;
        mas[o] = 'z';
        o += 1;
        //srand(time(NULL));
    }
    o = 0;
    srand(time(NULL) + rand());
    //srand(time(NULL));
    while (o < 7) {
        c = rand() % BANK_count;
        mas[o] = Bank_delete(c);
        o += 1;
    }
}

void giving_letters() {
    int o = 0;
    int c;
    char t;
    //выдача букв игроку 1
    srand(time(NULL) + rand());
    while (o < 7) {
        if (player_bank_1[o] == 'z' && BANK_count > 0) {
            c = rand() % BANK_count;
            player_bank_1[o] = Bank_delete(c);
        }
        else if (BANK_count == 0) { break; }
        o += 1;
    }
    o = 0;
    //выдача букв игроку 2
    //srand(time(NULL) + rand() % 103);
    srand(time(NULL) + rand());
    while (o < 7 && BANK_count > 0) {
        if (player_bank_2[o] == 'z' && BANK_count > 0) {
            c = rand() % BANK_count;
            player_bank_2[o] = Bank_delete(c);
        }
        else if (BANK_count == 0) { break; }
        o += 1;
    }
}

int analise_2(char mas[15], int pos, int st_str, int ukaz, char mas_sl[7]) {
    //менятся ли после этого основнео после? появятся ли там k?
    int t = 0;
    for (int i = 0; i < 15; i++) {
        if (mas[i] == 'z') {
            t += 1;
        }
        //t += 1;
    }
    if (t == 15) {
        return 0;
    }
    int count = 0;
    int point_number[100];
    //массив индексов точек привязки
    point_number[count] = -1;
    count += 1;

    int x = 0;
    int k = 1;
    int x2 = 0;
    int posi = -1;
    for (int i = 0; i < 15; i++) {
        // внимание! подумать над этим случаем! когда у нас точка привязски 1 (или 0 ) клетка что суффикс? что префикс?
        if (mas[i] == 'z' && i == 0 && mas[i + 1] != 'z') {
            //случай 1: 
            mas[i] = 'k';
            point_number[count] = 0;
            count += 1;
            //обработаьь этот случай
        }
        else if (mas[i] == 'z' && i != 0 && i != 14 && ((mas[i - 1] != 'z' && mas[i - 1] != 'k') || (mas[i + 1] != 'z' && mas[i + 1] != 'k'))) {
            //сделать условие более логичным
            mas[i] = 'k';
            if (mas[i - 1] != 'z') {
                point_number[count] = i;
                count += 1;
            }
            else {
                x = i - 1;
                while (mas[x] == 'z') {
                    k += 1;
                    x -= 1;
                }
                for (int j = 0; j < k; j++) {
                    point_number[count] = i;
                }
                k = 1;
                count += 1;
            }
        }
        else if (mas[i] == 'z' && i == 14 && ((mas[i - 1] != 'z' && mas[i - 1] != 'k'))) {
            mas[i] = 'k';
            if (mas[i - 1] != 'z') {
                point_number[count] = i;
                count += 1;
            }
        }
    }
    int suffix[200][2];//суффикс для каждого префикса
    int j = 0;
    int su_c = 0;

    for (int i = 1; i < count; i++) {
        j = point_number[i];

        j += 1;

        while (j < 15) {
            if (mas[j] == 'z' || mas[j] == 'k') {
                suffix[su_c][0] = point_number[i];
                suffix[su_c][1] = j - 1;
                su_c += 1;
            }
            j += 1;
        }
        if (mas[14] == 'z' || mas[14] == 'k') {
            suffix[su_c][0] = point_number[i];
            suffix[su_c][1] = j - 1;
            su_c += 1;
            j += 1;
        }
    }
    int preffix[200][2];
    int pre_c = 0;
    int p = 0;

    for (int i = 0; i < count - 1; i++) {
        j = point_number[i];
        j += 1;
        p = 0;

        while (j <= point_number[i + 1]) {
            if (mas[j] == 'z' || mas[j] == 'k') {//условие смотр
                preffix[pre_c][0] = j - p;
                preffix[pre_c][1] = point_number[i + 1];
                pre_c += 1;
            }
            else {
                p += 1;
            }
            j++;
        }
    }
    int c = 0;
    for (int i = 0; i < pre_c; i++) {
        char slovas[16];
        int slo = 0;
        int y = 0;
        for (int jj = preffix[i][0]; jj <= preffix[i][1]; jj++) {
            //printf("%c", mas[j]);
            slovas[slo] = mas[jj];
            if (mas[jj] == 'z' || mas[jj] == 'k') {
                y += 1;
            }
            slo += 1;
        }
        int dop = slo;
        for (int j = 0; j < su_c; j++) {
            int yy = 0;
            int T = 1;
            if (preffix[i][1] == suffix[j][0]) {
                slo = dop;
                for (int jj = suffix[j][0] + 1; jj <= suffix[j][1]; jj++) {

                    slovas[slo] = mas[jj];
                    if (mas[jj] == 'z' || mas[jj] == 'k') {
                        yy += 1;
                    }
                    if (y + yy > 7) {
                        T = 0;
                        break;
                    }
                    slo += 1;
                }
                if (T == 0) {
                    break;
                }

                slovas[slo] = '\0';
                //вообще <=7
                //ИСПРАВЛЕНИЕ:  preffix[i][0] + slo-1
                int kkk = pirog(st_str, pos, preffix[i][0], preffix[i][0] + slo - 1, strlen(slovas) - (y + yy));
                //printf("pirog: %d\n", kkk);
                //((DOP>0 && ukaz%2==1)||ukaz%2==0)
                if (y + yy < 7 && kkk == 1) {
                     //time(&start);
                    if (proverka_slova(slovas, 0, y + yy, st_str, preffix[i][0], preffix[i][0] + slo - 1, pos, 0, strlen(slovas) - (y + yy), ukaz, mas_sl) == 0) {

                        return 10;
                    };
                       //time(&end);
                      // printf("%lf - 1\n", difftime(end, start));

                }
                /*
                if (DOP == 0 && ukaz % 2 == 1) {
                    //printf("2\n");
                    return 10;
                }*/
                //printf("\n");
            }

        }
    }

}
int prov() {

    for (int i = 0; i < 32; i++) {
        if (ostatok[letters[i]] != 0) {
            return 1;
        }
    }
    return 0;

}

int searching(char let[8], char mas_sl[8]) {//функция проверяет, чтобы все буквы из let (в который мы передаем разыне use_letters) были в computer_bank
    //это делается на случай, если мы хотим записать слово из букв, котррые уже ранее использовали
    char comp_dop[8];
    strcpy(comp_dop, mas_sl);
    //printf("%s %s\n", comp_dop, mas_sl);
    int t = 0;
    for (int i = 0; i < strlen(let); i++) {
        t = 0;
        for (int j = 0; j < 7; j++) {
            if (let[i] == mas_sl[j]) {
                mas_sl[j] = 'z';
                t = 1;
                break;
            }
        }
        if (t != 1) {
            strcpy(mas_sl, comp_dop);
            //printf("%s %s\n", let, mas_sl);
            return 0;
        }
    }
    return 1;
}
//ukaz==1 - простой уровень, ukaz==2 - сложный
int analyse(int ukaz, int num, char mas_sl[8]) {
    // printf("%s\n", "ход передан компьютеру");
    char slovo[15];
    DOP = 3;
    int chet = 0;
    //анализ по стррокам: анализ префиксов и суффиксов
    for (int i = 0; i < 15; i++) {
        for (int j = 0; j < 15; j++) {
            slovo[j] = A[i][j];
        }
        if (analise_2(slovo, i, 0, ukaz, mas_sl) == 10 && ukaz % 2 == 1 && DOP == 0) {
            printf("1, %d\n", DOP);
            break;
        }
        memset(slovo,'z',15);

    }
    //анализ по столбцам: анализ префиксов и суффиксов
    for (int i = 0; i < 15; i++) {
        if (DOP == 0 && ukaz % 2 == 1) {
            break;
        }
        for (int j = 0; j < 15; j++) {
            slovo[j] = A[j][i];
        }
        if (analise_2(slovo, i, 1, ukaz, mas_sl) == 10 && ukaz % 2 == 1 && DOP == 0) {
            printf("2, %d\n", DOP);
            break;
        }
        memset(slovo, 'z', 15);

    }
    sorting();
    struct slova* tail3 = &massive;

    int tt = 0;
    while (tail3->next != NULL) {
        int pir = pirog(tail3->next->ukaz, tail3->next->number, tail3->next->i, tail3->next->j, tail3->next->num_z);
        int serach = searching(tail3->next->use_letters, mas_sl);
        printf("%s\n", tail3->next->slovvo);
        //printf("%s %d %d\n", tail3->next->slovvo,pir, serach);
        if (pir == 1 && serach == 1) {
            tt += 1;
            chet += tail3->next->massa;

            if (tail3->next->ukaz == 0) {
                for (int k = tail3->next->i; k <= tail3->next->j; k++) {
                    A[tail3->next->number][k] = tail3->next->slovvo[k - tail3->next->i];
                }
            }
            else {
                for (int k = tail3->next->i; k <= tail3->next->j; k++) {
                    A[k][tail3->next->number] = tail3->next->slovvo[k - tail3->next->i];
                }
            }
        }
        tail3->next = tail3->next->next;
    }
    ///
    PRINT();
    /// 
    clearStruct(&massive);
    tail = &massive;
    if (num == 1) {
        Pl_1 += chet;
    }
    else {
        Pl_2 += chet;
    }
    if (tt != 0) {
        if (num == 1) {
            propusk_1 = 0;
        }
        else {
            propusk_2 = 0;
        }
        return 1;
    }
    //это подкорректировала, были включения по-другому

    if (tt == 0 && ukaz < 3 && BANK_count != 0) {
        printf("\n%s", "смена букв\n");
        change(mas_sl);
        printf("\nновые: %s\n", mas_sl);
        int a = analyse(3, num, mas_sl);
    }

    if ((tt == 0 && num == 2) || BANK_count == 0 || propusk_2 > 3) {
        printf("\n%s", "конец игры\n");
        return 0;
    }

    if (tt == 0 && ukaz >= 3) {
        if (num == 1) {
            propusk_1 += 1;
        }
        else {
            propusk_2 += 1;
        }
        printf("\n%s", "пропуск хода\n");
        return -1;
    }
}
//начальная инициализация поля
void AAA(int cc) {
    for (int i = 0; i < 15; i++) {
        for (int j = 0; j < 15; j++) {
            A[i][j] = 'z';
        }
    }
    fopen_s(&slov, "7_new.txt", "r");


    char dop[17];
    int c = 0;
    while (c != cc) {
        fgets(dop, 17, slov);
        c++;
    }
    fclose(slov);
    printf("\n %s \n", dop);
    for (int i = 4; i <= 10; i++) {
        A[6][i] = dop[i - 4];
    }
}
void print_bank() {
    struct letter_bank* l = &L;
    for (int i = 0; i < BANK_count; i++) {
        printf("%c ", l->next->letter_b);
        l = l->next;
    }
    printf("\n");
}
int main() {
    setlocale(LC_ALL, "Russian");
    int a = 0;
    srand(time(NULL) + rand());
    int cc = rand() % 7785;
    // printf("%d ", cc);

    BANK();
    //slovar();
    /*
    printf("%c \n", Bank_delete(94));
    printf("%c \n", Bank_delete(BANK_count-1));
    printf("%c \n", Bank_delete(BANK_count-1));
    printf("%c \n", Bank_delete(BANK_count-1));
    printf("%c \n", l_bank->letter_b);
    print_bank();

    print_bank();
    giving_letters();
    printf("bankl_2^ %s\n", player_bank_2);
    printf("bank_1^ %s\n", player_bank_1);
    int ttt = 0;
    while (ttt == 0) {
        scanf_s("%d", &ttt);
        change(player_bank_1);
        printf("%s\n", player_bank_1);
        print_bank();
    }

    printf("%c \n", Bank_delete(102));
    printf("%c \n", Bank_delete(8));
    printf("%c \n", l_bank->letter_b);
    print_bank();
    */
    giving_letters();
    //printf("%s \n", player_bank_1);
   // change(player_bank_1);
   // printf("%s \n", player_bank_1);
    //print_bank();

    printf("\n");

    int res1 = 0;
    int res2 = 0;
    double p1 = 0.0;
    double p2 = 0.0;
    int result;
    int aaa;


    AAA(cc);
    PRINT();
    while (a == 0) {
        //BANK_print();
        giving_letters();
        print_bank();
        printf("ход передан пользователю 1 \n");
        printf("его буквы: ");

        for (int i = 0; i < 7; i++) {
            printf("%c ", player_bank_1[i]);
        }
        printf("\n");
        time(&start);

        /*
        if (analyse(1, 1, player_bank_1) == 0) {
            p1 += difftime(end, start);
            printf("%lf\n", difftime(end, start));
            break;
        };

        time(&end);
        p1 += difftime(end, start);
        printf("%lf\n", difftime(end, start));

        printf("ход передан пользователю 2 \n");
        printf("его буквы: ");

        for (int i = 0; i < 7; i++) {
            printf("%c ", player_bank_2[i]);
        }
        printf("\n");
        time(&start);
        if (analyse(2, 2, player_bank_2) == 0) {
            p2 += difftime(end, start);
            printf("%lf\n", difftime(end, start));
            break;
        }
        */

        res1++;
        result = analyse(1, 1, player_bank_1);
        if (result == 0) {
            a = 1;
            time(&end);
            p1 += difftime(end, start);
            printf("%lf\n", difftime(end, start));
            break;
        }
        else if (result == 22) {
            aaa = analyse(3, 1, player_bank_1);
            if (aaa == 0) {
                time(&end);
                p1 += difftime(end, start);
                printf("%lf\n", difftime(end, start));
                break;
            }
        }

        time(&end);
        p1 += difftime(end, start);
        printf("%lf\n", difftime(end, start));

        printf("person 2 \n");
        printf("his letters: ");

        for (int i = 0; i < 7; i++) {
            printf("%c %d ", player_bank_2[i], player_bank_2[i]);
        }
        printf("\n");
        time(&start);
        res2++;

        //printf("allo\n");
        result = analyse(2, 2, player_bank_2);
        if (result == 0) {
            a = 1;
            time(&end);
            p1 += difftime(end, start);
            printf("%lf\n", difftime(end, start));
            break;
        }
        else if (result == 22) {
            aaa = analyse(4, 2, player_bank_2);
            if (aaa == 0) {
                time(&end);
                p2 += difftime(end, start);
                printf("%lf\n", difftime(end, start));
                break;
            }
        }

        time(&end);
        p2 += difftime(end, start);
        printf("%lf\n", difftime(end, start));
        printf("очки игрока 1: %d\n", Pl_1);
        printf("очки игрока 2: %d\n", Pl_2);
    }
    printf("ochki:::time person 1| %d ::: %f\n", Pl_1, p1);
    printf("ochki:::time person 2| %d ::: %f\n", Pl_2, p2);
    printf("k hodov 1: %d, 2: %d", res1, res2);

}