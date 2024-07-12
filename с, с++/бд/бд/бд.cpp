#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <Windows.h>
#include <ctype.h>
#include <math.h>


typedef enum role_0 { abit, student, teacher };

typedef struct t {
	// enum { number, FIO, role, info, vidimost, pologenie } for_file;
	int number;//это нельзя поменять пользователю
	char FIO[100];
	role_0 role;
	union {
		int ege;
		float otsenka;
		char zvanie;
	} info;
	short vidimost;//имитация bool, необязательно для ввода
} Pole;


int K = 0;//кол-во элементов в базе

char s[256];//строка ввода с клавиатуры
char name_input[11] = "input1.txt" ;

int add_elem() {
	//FILE* input;
	FILE* input;
	if ((input = fopen(name_input, "a")) == NULL)
	{ printf("Error, in base 0 elem\n"); return 0; };
	int	i = 9;//счетчик по s
	int k = 1;//счетчик по структуре
	int u = 0;//счетчик по m
	int u1 = 0;//счетчик по m1
	char m[256];//ввод k-того поля
	char m1[256];//для дробой части
	memset(m, 0, sizeof(m));
	memset(m1, 0, sizeof(m1));
	char d = 'f';// элемент по m
	d = s[i];
	i++;
	Pole* person = (Pole*)calloc(1,sizeof(Pole));
	person->number = K + 1;
	strcpy(person->FIO, (char *)"hg");

	while (d != ';') {
		if (d == ',')//запись в структуру
		{
			//if (k == 0) { 
			//	if (strtol(m, 0, 10) > K) { person.number = strtol(m,0, 10); memset(m,0,sizeof(m)); u = 0; }
			//	else {  printf("Oops #0, you enter something wrong. Please, enter data again.\n"); return 0; }
			//}
			if (k == 1) { u = 0; }
			else if (k == 2) {
				if (strncmp(m, "abit", 4) == 0 || strtol(m, 0, 10) == 0) { person->role = abit; }
				else if (strncmp(m, "student", 7) == 0 || strtol(m, 0, 10) == 1) { person->role = student; }
				else if (strncmp(m, "teacher", 7) == 0 || strtol(m, 0, 10) == 2) { person->role = teacher; }
				else { printf("Oops #2, you enter something wrong. Please, enter data again.\n"); return 0; }
				memset(m, 0, sizeof(m)); u = 0;
			}
			else if (k == 3) {
				if (person->role == 0) { person->info.ege = strtol(m, 0, 10); }
				else if (person->role == 1) { person->info.otsenka = strtol(m, 0, 10) + pow(0.1, u1-1) * strtol(m1, 0, 10);  memset(m1, 0, sizeof(m1)); u1 = 0; }
				else if (person->role == 2) { person->info.zvanie = m[0]; }
				else { printf("Oops #3, you enter something wrong. Please, enter data again.\n"); return 0; }//если вдруг ошибка, хоть и не должна быть
				memset(m, 0, sizeof(m)); u = 0;
			}
			k++;
			if (k > 5) { printf("Oops #5, you enter something wrong. Please, enter data again.\n"); return 0; };
		}
		else {//вычленение из s
			//if (k == 0) { m[u] = d, u++; }//number
			//else
			if (k == 1) { person->FIO[u] = d, u++; }//FIO
			else if (k == 2) { m[u] = d; u++; }//role
			else if (k == 3) {
				if (d != '.') {
					if (u1 == 0)
					{
						m[u] = d; u++;
					}//целая часть
					if (u1 != 0) { m1[u1 - 1] = d; u1++; }//дробная часть
				}
				else { u1 = 1; }
			}
			else if (k == 4) { m[u] = d; u++; };//vidimost

		}
		d = s[i];
		i++;
	}
	if (k == 4) {
		if (strncmp(m, "True", 4) == 0 || strtol(m, 0, 10) == 1 ) { person->vidimost = (short)1; }
		else if (strncmp(m, "False", 5) == 0 || strtol(m, 0, 10) == 0) { person->vidimost = (short)0; }
		else { printf("Oops #4, you enter something wrong. Please, enter data again.\n"); return 0; }
		memset(m, 0, sizeof(m)); u = 0;
	}
	else if (k == 3) {
		if (person->role == 0) { person->info.ege = strtol(m, 0, 10); }
		else if (person->role == 1) { person->info.otsenka = strtol(m, 0, 10) + pow(0.1, u1) * strtol(m1, 0, 10);  memset(m1, 0, sizeof(m1)); u1 = 0; }
		else if (person->role == 2) { person->info.zvanie = m[0]; }
		else { printf("Oops #3, you enter something wrong. Please, enter data again.\n"); return 0; }//если вдруг ошибка, хоть и не должна быть
		memset(m, 0, sizeof(m)); u = 0;
	}
	else if (k > 4 || k < 3 ) { printf("Oops #5, you enter something wrong. Please, enter data again.\n"); return 0; }
	else { person->vidimost = 1; };

	//if(k>5) { printf("Oops #5, you enter something wrong. Please, enter data again."); return 0; };
	//все ок, записываем
	//ввели номер

	fprintf(input, "\n%d,", K+1);
	//ввели имя
	for (int i = 0; i < strlen(person->FIO); i++) {
		fputc(person->FIO[i], input);
	}
	fprintf(input, ",");
	//ввели	роль
	fprintf(input, "%d", person->role);
	fprintf(input, ",");
	//ввели инфо
	if (person->role == 0) { fprintf(input, "%d", person->info.ege); }
	else if (person->role == 1) { fprintf(input, "%f", person->info.otsenka); }
	else if (person->role == 2) { fprintf(input, "%c", person->info.zvanie); }
	fprintf(input, ",");
	//ввели видимость, по умолчанию 1
	fprintf(input, "%d", person->vidimost);
	fprintf(input, ";");
	u = 0;
	K++;
	//DATA[K] = person;
	fclose(input);
	free(person);
	return 1;
}
void show_number(int num) {
	//if (file = empty){return 0;}
	FILE* input;  if (fopen_s(&input, name_input, "r") != 0) { printf("Error, in base 0 elem\n"); return; };
	char st[256];
	for (int i = 0; i < num; i++)
	{
		fgets(st, 256, input);
	}
	fgets(st, 256, input);
	puts(st);
	fclose(input);
	printf("\n");
}
int poisk_stud(char s1[10]) {
	//FILE* input;
	FILE* input;
	if ((input = fopen(name_input, "r")) == NULL)
	{
		printf("Error, in base 0 elem\n"); return 0;
	};
	//fopen_s(&input, name_input, "a");
	int u = 0;//счетчик по m
	char m[256];
	memset(m, 0, sizeof(m));
	Pole* person = (Pole*)calloc(1, sizeof(Pole));
	role_0 Role;
	person->number = K + 1;
	strcpy(person->FIO, (char*)"hg");
	while (s1[u] != ';') 
	{//
		{ m[u] = s1[u]; u++; }//role
	}
	     if (strncmp(m, "abit",    4) == 0 ) { Role = abit; }
	else if (strncmp(m, "student", 7) == 0 ) { Role = student; } 
	else if (strncmp(m, "teacher", 7) == 0 ) { Role = teacher; }
	else if (strtol(m, 0, 10) == 0) { Role = abit; }
	else if (strtol(m, 0, 10) == 1) { Role = student; }
	else if (strtol(m, 0, 10) == 2) { Role = teacher; }
	else { printf("Oops #2, you enter something wrong. Please, enter data again.\n"); return 0; }
	memset(m, 0, sizeof(m)); u = 0;


	while ((fgets(s, 256, input)) != NULL) {//новый элемент
		int	i = 0;//счетчик по s
		int k = 0;//счетчик по структуре
		int u = 0;//счетчик по m
		int u1 = 0;//счетчик по m1
		char m[256];
		char m1[256];//для дробой части
		memset(m, 0, sizeof(m));
		memset(m1, 0, sizeof(m1));
		char d = 'f';
		d = s[i];
		i++;
		while (d != ';') {
			if (d == ',')//запись в структуру
			{
				if (k == 0) { 
					if (strtol(m, 0, 10) <= K) { person->number = strtol(m,0, 10); memset(m,0,sizeof(m)); u = 0; }
					else {  printf("Oops #0, you enter something wrong. Please, enter data again.\n"); return 0; }
				}
				if (k == 1) { u = 0; }
				else if (k == 2) {
					if (strtol(m, 0, 10) == 0) { person->role = abit; }
					else if (strtol(m, 0, 10) == 1) { person->role = student; }
					else if (strtol(m, 0, 10) == 2) { person->role = teacher; }
					else { printf("Oops #2, you enter something wrong. Please, enter data again.\n"); return 0; }
					memset(m, 0, sizeof(m)); u = 0;
				}
				else if (k == 3) {
					if (person->role == 0) { person->info.ege = strtol(m, 0, 10); }
					else if (person->role == 1) { person->info.otsenka = strtol(m, 0, 10) + pow(0.1, u1) * strtol(m1, 0, 10);  memset(m1, 0, sizeof(m1)); u1 = 0; }
					else if (person->role == 2) { person->info.zvanie = m[0]; }
					else { printf("Oops #3, you enter something wrong. Please, enter data again.\n"); return 0; }//если вдруг ошибка, хоть и не должна быть
					memset(m, 0, sizeof(m)); u = 0;
				}
				else if (k == 4) {
					if (strtol(m, 0, 10) == 1) { person->vidimost = (short)1; }
					else if (strtol(m, 0, 10) == 0) { person->vidimost = (short)0; }
					else { printf("Oops #4, you enter something wrong. Please, enter data again.\n"); return 0; }
					memset(m, 0, sizeof(m)); u = 0;
				}
				k++;
				if (k > 5) { printf("Oops #5, you enter something wrong. Please, enter data again.\n"); return 0; };
			}
			else {//вычленение из s
				if (k == 0) { m[u] = d, u++; }//number
				else if (k == 1) { person->FIO[u] = d, u++; }//FIO
				else if (k == 2) { m[u] = d; u++; }//role
				else if (k == 3) {
					if (d != '.') {
						if (u1 == 0)
						{
							m[u] = d; u++;
						}//целая часть
						if (u1 != 0) { m1[u1 - 1] = d; u1++; }//дробная часть
					}
					else { u1 = 1; }
				}
				else if (k == 4) { m[u] = d; u++; };//vidimost
			}
			d = s[i];
			i++;
		}
		if (k == 4) {
			if (strtol(m, 0, 10) == 1) { person->vidimost = (short)1; }
			else if (strtol(m, 0, 10) == 0) { person->vidimost = (short)0; }
			else { printf("Oops #4, you enter something wrong. Please, enter data again.\n"); return 0; }
			memset(m, 0, sizeof(m)); u = 0;
		}
		if (k > 4 || k<3) { printf("Oops #5, you enter something wrong. Please, enter data again.\n"); return 0; }
		


		if (person->role == Role) { puts(s); }
	}
	u = 0;
	fclose(input);
	free(person);
	return 1;
}
int show() {
	//if (file = empty):
	FILE* input; if (fopen_s(&input, name_input, "r") != 0) { printf("Error, in base 0 elem\n"); return 0; };
	//fopen_s(&input, name_input, "r");
	char ai; //буква, которую мы сейчас взяли с файла
	printf("\nrole==0 => abit || role==1 => student || role==2 => teacher\n");
	printf("info(abit) => int EGE || info(student) => float otsenka || info(teacher) => char zvanie \n");
	printf("num, FIO, role, info, vidimost\n\n");
	int u = 0;//счетчик по m
	char m[256];
	memset(m, 0, sizeof(m));
	Pole* person = (Pole*)calloc(1, sizeof(Pole));
	while ((fgets(s, 256, input)) != NULL) {//новый элемент
		int	i = 0;//счетчик по s
		int k = 0;//счетчик по структуре
		int u = 0;//счетчик по m
		int u1 = 0;//счетчик по m1
		char m[256];
		char m1[256];//для дробой части
		memset(m, 0, sizeof(m));
		memset(m1, 0, sizeof(m1));
		char d = 'f';
		d = s[i];
		i++;
		while (d != ';') {
			if (d == ',')//запись в структуру
			{
				if (k == 0) {
					if (strtol(m, 0, 10) <= K) { person->number = strtol(m, 0, 10); memset(m, 0, sizeof(m)); u = 0; }
					else { printf("Oops #0, you enter something wrong. Please, enter data again.\n"); return 0; }
				}
				if (k == 1) { u = 0; }
				else if (k == 2) {
					if (strtol(m, 0, 10) == 0) { person->role = abit; }
					else if (strtol(m, 0, 10) == 1) { person->role = student; }
					else if (strtol(m, 0, 10) == 2) { person->role = teacher; }
					else { printf("Oops #2, you enter something wrong. Please, enter data again.\n"); return 0; }
					memset(m, 0, sizeof(m)); u = 0;
				}
				else if (k == 3) {
					if (person->role == 0) { person->info.ege = strtol(m, 0, 10); }
					else if (person->role == 1) { person->info.otsenka = strtol(m, 0, 10) + pow(0.1, u1) * strtol(m1, 0, 10);  memset(m1, 0, sizeof(m1)); u1 = 0; }
					else if (person->role == 2) { person->info.zvanie = m[0]; }
					else { printf("Oops #3, you enter something wrong. Please, enter data again.\n"); return 0; }//если вдруг ошибка, хоть и не должна быть
					memset(m, 0, sizeof(m)); u = 0;
				}
				else if (k == 4) {
					if (strtol(m, 0, 10) == 1) { person->vidimost = (short)1; }
					else if (strtol(m, 0, 10) == 0) { person->vidimost = (short)0; }
					else { printf("Oops #4, you enter something wrong. Please, enter data again.\n"); return 0; }
					memset(m, 0, sizeof(m)); u = 0;
				}
				k++;
				if (k > 5) { printf("Oops #5, you enter something wrong. Please, enter data again.\n"); return 0; };
			}
			else {//вычленение из s
				if (k == 0) { m[u] = d, u++; }//number
				else if (k == 1) { person->FIO[u] = d, u++; }//FIO
				else if (k == 2) { m[u] = d; u++; }//role
				else if (k == 3) {
					if (d != '.') {
						if (u1 == 0)
						{
							m[u] = d; u++;
						}//целая часть
						if (u1 != 0) { m1[u1 - 1] = d; u1++; }//дробная часть
					}
					else { u1 = 1; }
				}
				else if (k == 4) { m[u] = d; u++; };//vidimost
			}
			d = s[i];
			i++;
		}
		if (k == 4) {
			if (strtol(m, 0, 10) == 1) { person->vidimost = (short)1; }
			else if (strtol(m, 0, 10) == 0) { person->vidimost = (short)0; }
			else { printf("Oops #4, you enter something wrong. Please, enter data again.\n"); return 0; }
			memset(m, 0, sizeof(m)); u = 0;
		}
		if (k > 4 || k < 3) { printf("Oops #5, you enter something wrong. Please, enter data again.\n"); return 0; }



		if (person->vidimost == 1) { puts(s); }
	}
	u = 0;
	fclose(input);
	free(person);
	return 1;
	printf("\n\n");
	return 0;
}
void del_elem(int num) {
	FILE* history;  if (fopen_s(&history, name_input, "r") != 0) { printf("Error, in base 0 elem\n"); return; };
	FILE* output1;  fopen_s(&output1, "output1.txt", "w");
	char st[256] = { 0 };
	for (int i = 1; i < K; i++)
	{
		fgets(st, 256, history);
		if (i != num)fputs(st, output1);
	}
	fgets(st, 256, history);
	K--;
	fclose(history);
	fclose(output1);
	remove(name_input);
	rename("output1.txt", name_input);
	printf("\n");
}
int edit_num(int num, char* st) {
	FILE* input;  if (fopen_s(&input, name_input, "r") != 0) { printf("Error, in base 0 elem\n"); return 0; };
	FILE* output1;  fopen_s(&output1, "output1.txt", "w");
	char st1[256];
	for (int i = 1; i < num; i++)
	{
		fgets(st1, 256, input);
		fputs(st1, output1);
	}
	fgets(st1, 256, input);//num-ый - элемент, который сейчас
	//дошли до нужного номера - st - старая строка, те эта функция не менят номер
	Pole* person = (Pole*)calloc(1, sizeof(Pole));
	int	i = 0;//счетчик по s
	int k = 0;//счетчик по структуре
	int u = 0;//счетчик по m
	int u1 = 0;//счетчик по m1
	char m[256];
	char m1[256];//для дробой части
	memset(m, 0, sizeof(m));
	memset(m1, 0, sizeof(m1));
	char d = 'f';
	d = s[i];
	i++;
	while (d != ';') {
		if (d == ','){//запись в структуру
			if (k == 0) {
				if (strtol(m, 0, 10) <= K) { person->number = strtol(m, 0, 10); memset(m, 0, sizeof(m)); u = 0; }
				else { printf("Oops #0, you enter something wrong. Please, enter data again.\n"); fclose(input);
				fclose(output1);
				remove("output1.txt"); return 0; }
			}
			if (k == 1) { u = 0; }
			else if (k == 2) {
				if (strtol(m, 0, 10) == 0) { person->role = abit; }
				else if (strtol(m, 0, 10) == 1) { person->role = student; }
				else if (strtol(m, 0, 10) == 2) { person->role = teacher; }
				else { printf("Oops #2, you enter something wrong. Please, enter data again.\n"); fclose(input);
				fclose(output1);
				remove("output1.txt"); return 0; }
				memset(m, 0, sizeof(m)); u = 0;
			}
			else if (k == 3) {
				if (person->role == 0) { person->info.ege = strtol(m, 0, 10); }
				else if (person->role == 1) { person->info.otsenka = strtol(m, 0, 10) + pow(0.1, u1) * strtol(m1, 0, 10);  memset(m1, 0, sizeof(m1)); u1 = 0; }
				else if (person->role == 2) { person->info.zvanie = m[0]; }
				else { //если вдруг ошибка, хоть и не должна быть
					printf("Oops #3, you enter something wrong. Please, enter data again.\n"); 
					fclose(input);
					fclose(output1);
					remove("output1.txt"); return 0; }
				memset(m, 0, sizeof(m)); u = 0;
			}
			else if (k == 4) {
				if (strtol(m, 0, 10) == 1) { person->vidimost = (short)1; }
				else if (strtol(m, 0, 10) == 0) { person->vidimost = (short)0; }
				else { printf("Oops #4, you enter something wrong. Please, enter data again.\n"); fclose(input);
				fclose(output1);
				remove("output1.txt"); return 0; }
				memset(m, 0, sizeof(m)); u = 0;
			}
			k++;
			if (k > 5) { printf("Oops #5, you enter something wrong. Please, enter data again.\n"); fclose(input);
			fclose(output1);
			remove("output1.txt"); return 0; };
		}
		else {//вычленение из s
			if (k == 0) { m[u] = d, u++; }//number
			else if (k == 1) { person->FIO[u] = d, u++; }//FIO
			else if (k == 2) { m[u] = d; u++; }//role
			else if (k == 3) {
				if (d != '.') {
					if (u1 == 0)
					{
						m[u] = d; u++;
					}//целая часть
					if (u1 != 0) { m1[u1 - 1] = d; u1++; }//дробная часть
				}
				else { u1 = 1; }
			}
			else if (k == 4) { m[u] = d; u++; };//vidimost
		}
		d = s[i];
		i++;
	}
	if (k == 4) {
		if (strtol(m, 0, 10) == 1) { person->vidimost = (short)1; }
		else if (strtol(m, 0, 10) == 0) { person->vidimost = (short)0; }
		else { printf("Oops #4, you enter something wrong. Please, enter data again.\n"); 
		fclose(input);
		fclose(output1);
		remove("output1.txt"); return 0; }
		memset(m, 0, sizeof(m)); u = 0;
	}
	if (k > 4 || k < 3) { printf("Oops #5, you enter something wrong. Please, enter data again.\n");
	fclose(input);
	fclose(output1);
	remove("output1.txt"); return 0; }
	
	person->number = num;
	//ввели номер
	fprintf(output1, "%d", person->number);
	fprintf(output1, ",");
	//ввели имя
	for (int i = 0; i < strlen(person->FIO); i++) {
		fputc(person->FIO[i], output1);
	}
	fprintf(output1, ",");
	//ввели	роль
	fprintf(output1, "%d", person->role);
	fprintf(output1, ",");
	//ввели инфо
	if (person->role == 0) { fprintf(output1, "%d", person->info.ege); }
	else if (person->role == 1) { fprintf(output1, "%f", person->info.otsenka); }
	else if (person->role == 2) { fprintf(output1, "%c", person->info.zvanie); }
	fprintf(output1, ",");
	//ввели видимость, по умолчанию 1
	fprintf(output1, "%d", person->vidimost);
	fprintf(output1, ";");
	u = 0;

	for (int i = num+1; i <= K; i++)//лушче сделать через while, чтобы не было ошибки
	{
		fgets(st1, 256, input);
		fputs(st1, output1);
	}
	fclose(input);
	fclose(output1);
	remove(name_input);
	rename("output1.txt", name_input);
	printf("\n");
}
int show_all() {
	//if (file = empty):
	FILE* input; if (fopen_s(&input, name_input, "r") != 0) { printf("Error, in base 0 elem\n"); return 0; };
	//fopen_s(&input, name_input, "r");
	char ai; //буква, которую мы сейчас взяли с файла
	printf("\nrole==0 => abit || role==1 => student || role==2 => teacher\n");
	printf("info(abit) => int EGE || info(student) => float otsenka || info(teacher) => char zvanie \n");
	printf("num, FIO, role, info, vidimost\n\n");

	while ((ai = fgetc(input)) != EOF)
	{
		printf("%c", ai);
	}
	fclose(input);
	printf("\n\n");
	return 0;
}

void inter_face() {
	printf("Enter help or info\n");
	while (1) {
		//printf("%d", n);
		//int num = 0;
		fgets(s,256,stdin);
		//puts(s);
		if (strncmp(s, "info", 4) == 0) {
			printf("\
  help/info : list of all available commands\n \
  show: show only visible text of Data base\n \
  show_all : show text of Data base\n \
  show_num <number>; : show text of person in Data base\n \
  edit_num <number> <string>; :edit person № num in Data base\n \
	example string: \"34,Petrov Petya Petrovich,<role>(0,1,2),info(int,float,char);\"))\n \
  del_elem <!number!>; : delete person from Data base\n \
  add_elem <string>;: adds element (please, entry piece data with split=',' and end=';',\n \
	example string: \"Petrov Petya Petrovich,<role>(0,1,2),info(int,float,char);\"))\n \
  exit : exit the program\n");
			continue;
		}
		if (strncmp(s, "help", 4) == 0) {
			printf("\
  help/info: list of all available commands\n \
  show: show only visible text of Data base\n \
  show_all: show text of Data base\n \
  show_num <number>; : show text of person in Data base\n \
  edit_num <number> <string>; :edit person № num in Data base\n \
	example string: \"34,Petrov Petya Petrovich,<role>(0,1,2),info(int,float,char);\"))\n \
  poisk <Role>; : show text of  all person in Role in Data base\n \
  del_elem <!number!>: delete person from Data base\n \
  add_elem: adds element (please, entry piece data with split=',' and end=';',\n \
	example: \"Petrov Petya Petrovich,<role>(0,1,2),info(int,float,char);\"))\n \
  exit: exit the program\n");
			continue;
		}

		if (strncmp(s, "show_all", 8) == 0) {
			show_all();
			continue;
		}
		if (strncmp(s, "show", 4) == 0) {
			show();
			continue;
		}
		if (strncmp(s, "poisk_Role ", 11) == 0) {
			int i = 11;
			char m1[10];
			while (s[i] != ';') { m1[i - 11] = s[i]; i++; }
			m1[i - 11] = s[i]; i++;
			poisk_stud(m1);
			continue;
		}
		if (strncmp(s, "show_num ", 9) == 0) {
			int i = 9;
			char m[10];
			while (s[i] != ';') { m[i - 9] = s[i]; i++; }
			show_number(strtol(m, 0, 10)); continue;
		}
		if (strncmp(s, "edit_num ", 9) == 0) {
			int i = 9;
			char m[10];
			char stre[256];
			while (s[i] != ' ') { m[i - 9] = s[i]; i++; }
			i++;
			while (s[i] != ';') { stre[i-9] = s[i]; i++; }
			edit_num(strtol(m, 0, 10), stre); continue;
		}
		if (strncmp(s, "add_elem ", 9) == 0) {

			add_elem(); continue;
		}
		if (strncmp(s, "del_elem ", 9) == 0) {
			int i = 10;
			char m[10];
			while (s[i] != ';') { m[i - 10] = s[i]; i++; if (i == 11) { break; } }
			if (i == 11) { printf("Error of del\n"); continue; }
			del_elem(strtol(m, 0, 10)); continue;
		}
		if (strncmp(s, "exit", 4) == 0) { exit; break; }
		else {
			printf("Unknown command, please enter again (enter info for more information)\n");
			continue;
		}
	}

}
int main(void)
{
	FILE* input;
	// подсчсет кол-ва элементов в 
	if (fopen_s(&input, name_input, "r") == 0) {
		char ai = 0; //строка
		ai = fgetc(input);
		while (ai != EOF)
		{
			ai = fgetc(input);
			if (ai == ';')K++;
		}
		printf("%d\n", K);
		fclose(input);
		printf("\n");
	}
	inter_face();
	return 1;
}
