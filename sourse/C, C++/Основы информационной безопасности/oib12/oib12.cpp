#define _CRT_SECURE_NO_WARNINGS
#define INVALID_FILE_ATTRIBUTES ((DWORD)-1)
#include <stdio.h>
#include <Windows.h>
int main()
{
	//SetConsoleCP(1251);
	//SetConsoleOutputCP(1251);
	wchar_t File[] = TEXT("C:/Users/vas/source/repos/oib12/test_folder1/text.txt");
	int n;
	printf("1 - Архивный файл\n2 - Скрыть файл\n3 - Для чтения и записи\n4 - Только для чтения\n5 - Cоздать файл\n6 - Записать в файл\n7 - Прочитать из файла\n");
	scanf_s("%d", &n);
	if (n == 1) SetFileAttributes(File, FILE_ATTRIBUTE_ARCHIVE);
	if (n == 2) SetFileAttributes(File, FILE_ATTRIBUTE_HIDDEN);
	if (n == 3) SetFileAttributes(File, FILE_ATTRIBUTE_NORMAL);
	if (n == 4) SetFileAttributes(File, FILE_ATTRIBUTE_READONLY);
	if (n == 5) {
		WIN32_FIND_DATAA findData; ///Структура WIN32_FIND_DATA описывает файл, найденный функцией FindFirstFile
		HANDLE hFile = FindFirstFileA("C:/Users/vas/source/repos/oib12/test_folder1/text.txt", &findData);/// Функция FindFirstFile ищет каталог файла или подкаталог, название которого соответствует указанному имени файла
		///HANDLE - дескриптор, т.е. число, с помощью которого можно идентифицировать ресурс. С помощью дескприторов можно ссылаться на окна, объекты ядра, графические объекты и т.п.
		if (findData.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE)
			printf("Архивный файл\n");
		else {
			FILE* file1 = fopen("C:/Users/vas/source/repos/oib12/test_folder1/text.txt", "w");
			printf("Файл создан\n");
			fclose(file1);
		}
	}
	if (n == 6) {
		WIN32_FIND_DATAA findData;
		HANDLE hFile = FindFirstFileA("C:/Users/vas/source/repos/oib12/test_folder1/text.txt", &findData);
		if ((findData.dwFileAttributes & FILE_ATTRIBUTE_READONLY))
			printf("Файл только для чтения\n");
		else if (!(findData.dwFileAttributes & INVALID_FILE_ATTRIBUTES))
			printf("Ошибка\n");

		else if ((findData.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE))
			printf("Архивный файл\n");
		else {
			FILE* file2 = fopen("C:/Users/vas/source/repos/oib12/test_folder1/text.txt", "w");
			char m[1000];
			printf("\n");
			fgets(m, 1000, stdin);
			if (m == NULL) { printf("Пустая строка\n"); exit(0); }
			fputs(m, file2);
			fclose(file2);
			SetFileAttributes(File, FILE_ATTRIBUTE_NORMAL);
		}
	}
	if (n == 7) {
		WIN32_FIND_DATAA findData;
		HANDLE hFile = FindFirstFileA("C:/Users/vas/source/repos/oib12/test_folder1/text.txt", &findData);
		 {
			FILE* file3 = fopen("C:/Users/vas/source/repos/oib12/test_folder1/text.txt", "r");
			char a[1000] = { 0 };
			//fscanf(file3, "%s", &a);
			while (!feof(file3)) {
				fgets(a,1000,file3 );
				puts(a);
				memset(&a, 0, sizeof(a));
			}
			fclose(file3);
		}
	}

	return 0;
}