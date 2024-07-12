#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <Windows.h>
#include <malloc.h>
#define BARRIER 337

int length = 0;
char* binarycode;

int bin(unsigned int symbol)
{
	binarycode = (char*)realloc(binarycode, length + 8 * sizeof(char));

	for (int i = 0; i < 8; i++)
	{
		binarycode[length + i] = symbol % 2;
		symbol = symbol / 2;
	}
	length = length + 8;

	return 0;
}

int coder()
{
	FILE* input_bmp = fopen("input.bmp", "rb"); //Открываем двоичный файл для чтения
	FILE* output_bmp = fopen("output_bmp.bmp", "wb+"); //Создает файл для чтения / записи
	FILE* iput_text = fopen("text.txt", "r");

	char symb;
	//текст
	while (!feof(iput_text))
	{
		symb = fgetc(iput_text);
		if (!feof(iput_text))
			bin((int)symb);
	}

	
	//заголовок bmp
	for (int i = 0; i < 6; i++)
	{
		symb = fgetc(input_bmp);
		fputc(symb, output_bmp);
	}
	
	fclose(iput_text);
	
	int degree;
	printf("Enter degree: ");
	scanf("%d", &degree);

	if (degree == 0)
	{
		printf("Wrong degree\n");
		return 1;
	}

	symb = fgetc(input_bmp);
	//чтобы записать в наименее значимый бит
	fputc(degree, output_bmp);
	symb = fgetc(input_bmp);
	fputc(symb, output_bmp);

	if (length >= BARRIER)
	{
		printf("Text don't put in the stegocontainer\n");
		return 1;
	}

	printf("%d\n", length);
	if (length % degree != 0)
	{
		int new_size = 0;
		new_size = length % degree;
		binarycode = (char*)realloc(binarycode, length + new_size * sizeof(int));

		for (int i = 0; i < new_size; i++)
			binarycode[length + i] = 0;
	}

	symb = fgetc(input_bmp);

	int size1 = length / 256;
	fputc(size1, output_bmp);
	symb = fgetc(input_bmp);

	int size2 = length % 256;
	fputc(size2, output_bmp);

	for (int i = 0; i < 8; i++)
	{
		symb = fgetc(input_bmp);
		fputc(symb, output_bmp);
	}

	int* height;
	int* width;
	fread(&width, sizeof(int), 1, input_bmp);
	fwrite(&width, sizeof(int), 1, output_bmp);
	fread(&height, sizeof(int), 1, input_bmp);
	fwrite(&height, sizeof(int), 1, output_bmp);

	for (int i = 0; i < 29; i++)
	{
		symb = fgetc(input_bmp);
		fputc(symb, output_bmp);
	}

	int all = (int)width * (int)height * 3;
	int pixels = (int)width * (int)height;

	int k = 0;
	printf("Max lenght of text %d\n", pixels * degree);
	//printf("%d", pixels);
	if (((pixels) * degree > length) && (length <= BARRIER))
	{
		for (int i = 0; i < length;)
		{
			symb = fgetc(input_bmp);
			symb = (symb >> degree);

			for (int j = 0; j < degree; j++)
			{
				symb = symb << 1;
				symb = symb + binarycode[i];
				i++;
			}
			//printf("%с %d \n",symb,i);
			fputc(symb, output_bmp);
			k++;
		}

		while (k != (all - 1))
		{
			symb = fgetc(input_bmp);
			fputc(symb, output_bmp);
			k++;
		}
	}
	else
	{
		printf("Text don't put in the stegocontainer!\n"
			"Lenght of text %d, but max lenght of text %d", length, pixels * degree);
		return 1;
	}

	fclose(input_bmp);
	fclose(output_bmp);
	/*
	char sty [10]= { 0 };
	_itoa(degree, sty, 10);
	strcat(sty, ".bmp");
	rename("output_bmp.bmp", sty);
	*/
	return 0;
}

int main()
{
	//setlocale(LC_ALL, "Rus");
	//float time1 = clock();

	coder();

	//float time2 = clock();
	//float time_code = (time2 - time1) / (CLOCKS_PER_SEC);
	
	printf("done!\n");

	Sleep(3000);
	return 0;
}
