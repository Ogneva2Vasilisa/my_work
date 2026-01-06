#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <fstream>
#include <string>
#include <conio.h>
#include <Windows.h>
using namespace std;

//алфавит с максимальной частотой в начале
char* Alfavit_right = (char*)"etaoinshrdlcumwfgypbvkjxqz";
//char* Alfavit_right = "arenositpldhufmgbckvjyzw";
//char* Alfavit_right = "ethsoaincrfkpldvquwmygbjxz";//для 99%
char Alfavit_real[33];
long long Chastota_real[33] = { 0 };
int n = 0;//кол-во switch
char* name_input = (char*)"input1.txt";
char* name_right = (char*)"right.txt";
char s[20];//строка ввода с клавиатуры
int k = 0;


string passs = "etaoinshrdlcumwfgypbvkjxqz";
string true_passs = "UPEIKRYTHDJMGAALCWZVXSBFQN"; // правильный пароль etaoinshrdlcummfgypbvkjxqz

void my_xor(string msg) { // Афинный шифр
	string cipher = "";
	char a = 17;
	char b = 6;
	for (int i = 0; i < msg.length(); i++) {
		if (msg[i] != ' ')
			cipher = cipher + (char)((((a * (msg[i] - 'A')) + b) % 26) + 'A');
		else
			cipher += msg[i];
	}
	passs = cipher;
}

DWORD CalcFuncCrc(PUCHAR funcBegin, PUCHAR funcEnd)
{
	DWORD crc = 0;
	for (; funcBegin < funcEnd; ++funcBegin)
	{
		crc += *funcBegin;
	}
	return crc;
}
#pragma auto_inline(off)
VOID DebuggeeFunction()
{}
int check_pas_from_var() {
	if (passs == true_passs)
		return 1;
	else {
		printf("ALERT !!! WRONG PASSWORD !!!\n");
		return 0;
	}
}
int check_pas_from_file(char* u) {
	char otv = '0';
	if (passs.find(otv) != NULL) {
		if (passs.length() > 20) {
			otv = 0;
			if (passs[0] == 0)
				return 0;
		}
		else {
			otv = 1;
		}
	}
	else {
		if (passs.length() < 20)
			return 0;
	}
	ifstream file("password.txt");
	getline(file, passs);
	my_xor(passs);
	otv = check_pas_from_var();
	file.close();
	return otv;
}
VOID DebuggeeFunctionEnd()
{};
#pragma auto_inline(on)

DWORD g_origCrc = 8853;


//ищет индекс частоты по букве, если ее нет, то добавляет
int freq_id_from_letter(char a) {
	int i = 0;
	for (i; i < strlen(Alfavit_real); i++) {
		if (a == Alfavit_real[i]) { return i; }
	}
	Alfavit_real[i] = a;
	Chastota_real[i] = 0;
	return i;
}

//проверяет это буква? в определенной кодировке
short prov(char a) {
	if ((a >= 97 && a <= 122)) { return 1; }
	if (a >= 65 && a <= 90) { k++; return 1; }
	else { return 0; }
};

void result1() {
	FILE* input1; fopen_s(&input1, name_input, "r");
	FILE* input2; fopen_s(&input2, name_right, "r");
	char ai1;//буква, которую мы сейчас взяли с файла1
	char ai2;//буква, которую мы сейчас взяли с файла2
	long flag = 0; long n2 = 0;
	while ((ai1 = fgetc(input1)) != EOF)
	{	//проверка на конец слова (сейчас взяли не букву, а слово идет)
		ai2 = fgetc(input2);
		n2++;
		if (ai1 == ai2) {
			flag++;
		}
		else {
			printf("%c %c ", ai1, ai2); printf("%ld %ld\n", flag, n2);
		}
	}
	printf("%ld %ld\n", flag, n2);
	long double res = (long double)flag / (long double)n2;
	printf("Right symbols:%llf10 % \n", res * 100);
	fclose(input1);
	fclose(input2);

}

void switchsymbol() {
	FILE* history;  fopen_s(&history, "history.txt", "a");
	fprintf(history, "%c %c\n", s[7], s[9]); n++; fclose(history);
	FILE* input; fopen_s(&input, name_input, "r");
	char ai; //буква, которую мы сейчас взяли с файла
	FILE* output;  fopen_s(&output, "output.txt", "w");
	while ((ai = fgetc(input)) != EOF)
	{
		if (ai == s[7]) { fprintf(output, "%c", s[9]); }
		else { fprintf(output, "%c", ai); }
	}
	fclose(input); fclose(output);
	remove(name_input);
	rename("output.txt", name_input);
	printf("Done!\n");
}

void back_history() {
	FILE* history;  
	if (fopen_s(&history, "history.txt", "r") != 0) {
		printf("until there were no letter replacements\n");
		return;
	};
	FILE* output1;  fopen_s(&output1, "output1.txt", "w");
	char st[5] = { 0 };
	for (int i = 1; i < n; i++)
	{
		fgets(st, 5, history);
		fputs(st, output1);
	}
	fgets(st, 5, history);
	n--;
	fclose(history); fclose(output1); remove("history.txt"); rename("output1.txt", "history.txt");
	FILE* input; fopen_s(&input, name_input, "r");
	char ai; //буква, которую мы сейчас взяли с файла
	FILE* output;  fopen_s(&output, "output.txt", "w");
	while ((ai = fgetc(input)) != EOF)
	{
		if (ai == st[2]) { fprintf(output, "%c", st[0]); }
		else { fprintf(output, "%c", ai); }
	}
	fclose(input); fclose(output);
	remove(name_input);
	rename("output.txt", name_input);
	printf("\n");
}

void show() {
	//if (file = empty):
	FILE* input; fopen_s(&input, name_input, "r");
	char ai; //буква, которую мы сейчас взяли с файла
	while ((ai = fgetc(input)) != EOF)
	{
		printf("%c", ai);
	}
	fclose(input);
	printf("\n");
}

void stat_file() {
	memset(Alfavit_real, 0, sizeof(Alfavit_real));
	FILE* input; fopen_s(&input, name_input, "r");
	char ai;//буква, которую мы сейчас взяли с файла
	char t1;//вспомогательная переменная для swap
	long long t2;//вспомогательная переменная для swap
	int id;//индекс частоты буквы и иднекс буквы в Alhavit_real
	//при каждой новой частоте надо двигать частоту и букву
	//сначала берем новую букву
	//надо придумать, если у букв одинаковая частота 
	while ((ai = fgetc(input)) != EOF)
	{
		//в этом цикле мы считаем частоту буквы
			//проверка на то, что ai это буква 
		if (prov(ai)) {
			//отвечает за кол-во букв в Alphavit_real
			id = freq_id_from_letter(ai);
			Chastota_real[id]++;
			while (id > 0) {
				//здесь мы сдвигаем к максимальному варианту
				if (Chastota_real[id - 1] < Chastota_real[id]) {
					t1 = Chastota_real[id - 1];
					Chastota_real[id - 1] = Chastota_real[id];
					Chastota_real[id] = t1;
					t2 = Alfavit_real[id - 1];
					Alfavit_real[id - 1] = Alfavit_real[id];
					Alfavit_real[id] = t2;
					id--;
				}
				else { break; }
			}
		}
	}
	fclose(input);
	//здесь выводится статистика встречаемости букв в файле
	std::cout <<Alfavit_real<< endl;
}

void group_count() {
	FILE* input; fopen_s(&input, name_input, "r");
	char ai; //буква, которую мы сейчас взяли с файла
	char st[100] = ""; //слово, которое сейчас читаем
	int id = 0;//номер ai в st
	int max_id = 0;//максимальная длинна слова
	short flag = 0;//если 1 - значит идет счет слова\
						если 0 - значит это не буквы???\
						если пробел, то закончить flag, если он 1????
	short flag_not_bukva = 0;//если 1 - значит буква\
								  если 0 - значит это не буквы
	int a[20] = { 0 };//массив для вариаций длин слов =1 если есть такая длина
	char file_name_output[10] = { 0 };//название файла с кол-вом букв в слове
	FILE* output;
	while ((ai = fgetc(input)) != EOF)
	{	//проверка на конец слова (сейчас взяли не букву, а слово идет)
		if (flag == 1 && flag_not_bukva == 1) {
			flag = 0;
			if (max_id < id) { max_id = id; };
			a[id + 1] = 1;
			st[id + 1] = ' ';
			_itoa(id + 1, file_name_output, 10);
			strcat(file_name_output, ".txt");
			fopen_s(&output, file_name_output, "a");
			fputs(st, output);
			fclose(output);
			memset(st, 0, sizeof(st));//очищение строки
			memset(file_name_output, 0, sizeof(file_name_output));
		}
		//проверка на то, что ai это буква
		if (prov(ai)) {
			flag_not_bukva = 0;
			if (flag == 1) { id++; st[id] = ai; continue; }
			else { flag = 1; id = 0; st[id] = ai; }
		}
		else {
			if ((flag == 1) && (ai == '\'' || ai == '-')) {
				id++; st[id] = ai; continue;
			}
			else { flag_not_bukva = 1; }
		}
	}
	fclose(input);
	printf("\n");

	//вывод результатов, так как сейчас он считает слова при повторном вызове
	for (int i = 1; i <= max_id + 1; i++) {
		_itoa(i, file_name_output, 10);
		strcat(file_name_output, ".txt");
		fopen_s(&output, file_name_output, "r");
		if (a[i] == 0) { continue; }
		else {
			fgets(st, 100, output);
			printf("%d: ", i);
			cout<<st<<endl;
		}
		fclose(output);
		memset(st, 0, sizeof(st));
		memset(file_name_output, 0, sizeof(file_name_output));
	}
	k = 0;
}

void group_no() {
	FILE* input; fopen_s(&input, name_input, "r");
	char ai;//буква, которую мы сейчас взяли с файла
	k = 0;//кол-во неизвестных (больших) букв
	char st[100] = ""; //слово, которое сейчас читаем
	int id = 0;//номер ai в st
	int max_k = 0;//максимальное кол-во нерасшифрованных букв в слове
	short flag = 0;//если 1 - значит идет счет слова\
						если 0 - значит это не буквы???\
						если пробел, то закончить flag, если он 1????
	short flag_not_bukva = 0;//если 1 - значит буква\
								  если 0 - значит это не буквы
	int a[20] = { 0 };//массив для вариаций длин слов =1 если есть такая длина
	char file_name_output[20] = { 0 };//название файла с кол-вом букв в слове
	FILE* output;
	while ((ai = fgetc(input)) != EOF)
	{	 //проверка на конец слова (сейчас взяли не букву, а слово идет)
		if (flag == 1 && flag_not_bukva == 1) {
			flag = 0;
			if (max_k < k) { max_k = k; };
			a[k] = 1;//наличие слов именно с этим k нерасш. букв
			st[id + 1] = ' ';
			_itoa(k, file_name_output, 10);
			k = 0;
			strcat(file_name_output, "no.txt");
			fopen_s(&output, file_name_output, "a");
			fputs(st, output);
			fclose(output);
			memset(st, 0, sizeof(st));//очищение строки
			memset(file_name_output, 0, sizeof(file_name_output));
		}
		//проверка на то, что ai это буква
		if (prov(ai)) {
			flag_not_bukva = 0;
			if (flag == 1) { id++; st[id] = ai; continue; }
			else { flag = 1; id = 0; st[id] = ai; }
		}
		else {
			if ((flag == 1) && (ai == '\'' || ai == '-')) {
				id++; st[id] = ai; continue;
			}
			else { flag_not_bukva = 1; }
		}
	}
	fclose(input);
	printf("\n");

	//вывод результатов, так как сейчас он считает слова при повторном вызове
	for (int i = 0; i <= max_k; i++) {
		_itoa(i, file_name_output, 10);
		strcat(file_name_output, "no.txt");
		//puts(file_name_output);
		fopen_s(&output, file_name_output, "r");
		if (a[i] == 0) { continue; }
		else {
			fgets(st, 100, output);
			printf("%d: ", i);
			puts(st);
		}
		fclose(output);
		memset(st, 0, sizeof(st));
		memset(file_name_output, 0, sizeof(file_name_output));
	}
}

void auto_stat() {
	//stat для подчсета realAlfavit
	FILE* input; fopen_s(&input, name_input, "r");
	char ai;//буква, которую мы сейчас взяли с файла
	char t1;//вспомогательная переменная для swap
	long long t2;//вспомогательная переменная для swap
	int id;//индекс частоты буквы и иднекс буквы в Alhavit_real
	while ((ai = fgetc(input)) != EOF)
	{
		if (prov(ai)) {
			id = freq_id_from_letter(ai);
			Chastota_real[id]++;
			while (id > 0) {
				if (Chastota_real[id - 1] < Chastota_real[id]) {
					t1 = Chastota_real[id - 1];
					Chastota_real[id - 1] = Chastota_real[id];
					Chastota_real[id] = t1;
					t2 = Alfavit_real[id - 1];
					Alfavit_real[id - 1] = Alfavit_real[id];
					Alfavit_real[id] = t2;
					id--;
				}
				else { break; }
			}
		}
	}
	fclose(input);
	stat_file();
	puts(Alfavit_right);
	for (int i = 0; i < strlen(Alfavit_real); i++) {
		fopen_s(&input, name_input, "r");
		char ai; //буква, которую мы сейчас взяли с файла
		FILE* output;  fopen_s(&output, "output.txt", "w");
		while ((ai = fgetc(input)) != EOF)
		{
			if (ai == Alfavit_real[i]) { //printf("%c %c\n",ai, Alfavit_right[i]); 
				fprintf(output, "%c", Alfavit_right[i]);
			}
			else { fprintf(output, "%c", ai); }
		}
		fclose(input); fclose(output);
		remove(name_input);
		rename("output.txt", name_input);
	}
	result1();
}


void inter_face() {
	printf("Enter help or info\n");
	while (1) {
		if (IsDebuggerPresent()) {
			printf("Debugger is detected!!\n");
			return;
		}
		cin>>s;
		BOOL isDebugged = TRUE;
		__try
		{
			__asm
			{
				pushfd
				or dword ptr[esp], 0x100 // set the Trap Flag 
				popfd                    // Load the value into EFLAGS register
				nop
			}
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
			// If an exception has been raised – debugger is not present
			isDebugged = FALSE;
		}
		if (isDebugged)
		{
			std::cout << "Stop debugging program!" << std::endl;
			exit(-1);
		}
		if (strncmp(s, "info", 4) == 0) {
			printf("\
	help/info: list of all available commands\n \
	stat: shows the sequence of letters in the file \n \
	group_count: shows all words grouped by the number of letters\n \
	group_no: shows all words grouped by the number of letters that are not decoded at the moment\n \
	show: show cryptogram with currently decrypted text in capital letters\n \
	switch(char a, char b) : switch letters in the cryptogram\n \
	back_history: cancel letter replacements in the cryptogram \n \
	auto_stat: program replacesand inserts the letters by itself\n \
	exit: exit the program\n");
			continue;
		}
		if (strncmp(s, "help", 4) == 0) {
			printf("\
	help/info: list of all available commands\n \
	stat: shows the sequence of letters in the file \n \
	group_count: shows all words grouped by the number of letters\n \
	group_no: shows all words grouped by the number of letters that are not decoded at the moment\n \
	show: show cryptogram with currently decrypted text in capital letters\n \
	switch(char a, char b) : switch letters in the cryptogram\n \
	back_history: cancel letter replacements in the cryptogram \n \
	auto_stat: program replacesand inserts the letters by itself\n \
	exit: exit the program\n");
			continue;
		}
		if (strncmp(s, "stat", 4) == 0) {
			stat_file(); continue;
		}
		if (strncmp(s, "group_count", 11) == 0) {
			group_count(); continue;
		}
		if (strncmp(s, "group_no", 11) == 0) {
			group_no(); continue;
		}
		if (strncmp(s, "show", 4) == 0) {
			show(); continue;
		}
		if (strncmp(s, "switch(", 7) == 0) {
			switchsymbol(); continue;
		}
		if (strncmp(s, "back_history", 12) == 0) {
			back_history(); continue;
		}
		if (strncmp(s, "auto_stat", 9) == 0) {
			auto_stat(); continue;
		}
		if (strncmp(s, "exit", 4) == 0) {
			FILE* output; fopen_s(&output, "serial.txt", "w"); 
			fprintf(output, "KEY$y0uF1nd1t"); fclose(output);
			break; }
		else {
			printf("Unknown program, please enter again (enter info for more information)\n");
			continue;
		}
	}

}

int main() {
	//   удаление лишних файлов типа 0.txt
	char file_name[10] = { 0 };
	for (int i = 1; i < 20; i++)
	{
		if (IsDebuggerPresent()) {
			printf("Debugger is detected!!\n");
			return 0;
		}
		_itoa(i, file_name, 10);
		//puts(file_name);
		strcat(file_name, ".txt");
		remove(file_name);
		memset(file_name, 0, sizeof(file_name));
	}
	my_xor(true_passs);
	DWORD crc = CalcFuncCrc((PUCHAR)DebuggeeFunction, (PUCHAR)DebuggeeFunctionEnd);
	if (g_origCrc != crc)
	{
		std::cout << "Stop patching program!" << std::endl<<crc<<endl ;
	}

	if (check_pas_from_file(NULL))
	check_pas_from_file(NULL);
	//   удаление лишних файлов типа 0no.txt
	for (int i = 0; i < 20; i++)
	{
		if (IsDebuggerPresent()) {
			printf("Debugger is detected!!\n");
			return 0;
		}
		_itoa(i, file_name, 10);
		//puts(file_name);
		strcat(file_name, "no.txt");
		remove(file_name);
		memset(file_name, 0, sizeof(file_name));
	}
	remove("history.txt");

	//подготовка файла (все буквы должны быть строчными)???????????
	if (check_pas_from_file(NULL))
		inter_face();
	//   удаление лишних файлов типа 0.txt
	for (int i = 1; i < 20; i++)
	{
		_itoa(i, file_name, 10);
		//puts(file_name);
		strcat(file_name, ".txt");
		remove(file_name);
		memset(file_name, 0, sizeof(file_name));
	}
	//   удаление лишних файлов типа 0no.txt
	for (int i = 0; i < 20; i++)
	{
		_itoa(i, file_name, 10);
		//puts(file_name);
		strcat(file_name, "no.txt");
		remove(file_name);
		memset(file_name, 0, sizeof(file_name));
	}
	remove("history.txt");
}
