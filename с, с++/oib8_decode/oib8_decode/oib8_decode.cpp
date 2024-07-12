#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <Windows.h>
#include <malloc.h>
#define BARRIER 337

int length = 0;
int volume = 0;

int symbol(int* bin)
{
	int number = 0;

	for (int i = 0; i < 8; i++)
		number = number + bin[i] * pow(2, i);

	return number;
}

int decoder()
{
	FILE* outpu_bmp = fopen("output_bmp.bmp", "rb"); //Открываем двоичный файл для чтения
	FILE* ouput_text = fopen("output_text.txt", "wb"); //Создаем двоичный файл для записи

	int bin[8] = { 0 };

	int degree;
	char symbol0;
	for (int i = 0; i < 6; i++)
	{
		symbol0 = fgetc(outpu_bmp);
	}
	degree = fgetc(outpu_bmp);

	if (degree == 0)
	{
		printf("Wrong degree or there message is not\n");
		return 1;
	}

	symbol0 = fgetc(outpu_bmp);

	int size1 = fgetc(outpu_bmp);
	length = size1 * 256;
	int size2 = fgetc(outpu_bmp);
	length = length + size2;

	for (int i = 0; i < 45; i++)
	{
		symbol0 = fgetc(outpu_bmp);
	}

	int number = 0;

	unsigned char c;
	int i = 0;
	while (volume < length)
	{
		c = fgetc(outpu_bmp);
		for (int k = 0; k < degree; k++)
		{
			int c0 = c >> (degree - k - 1);
			bin[i] = c0 % 2;
			i++;
			if (i == 8)
			{
				number = symbol(bin);
				fputc((char)number, ouput_text);
				volume = volume + 8;
				i = 0;
				for (int j = 0; j < 8; j++)
				{
					bin[j] = 0;
				}
			}
			if (volume == length)
				break;
		}
	}

	fclose(outpu_bmp);
	fclose(ouput_text);
	return 0;
}

int main()
{
	
	decoder();
	
	printf("Done\n");

	return 0;
}
