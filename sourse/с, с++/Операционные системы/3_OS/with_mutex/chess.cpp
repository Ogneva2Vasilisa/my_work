#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <queue>
#include <windows.h>

using namespace std;

int N_threads = 0;
int Result = 0;
int N, L, K;

int** Board;
HANDLE Mutex;
HANDLE event;
CRITICAL_SECTION cs;
CRITICAL_SECTION cs_result;
HANDLE* tids;

// Структура для хранения диапазона, который нужно отсортировать
struct Borders {
	int X;
	int Y;
};

queue<Borders> Queue; // Очередь 

void Print_Board(int** Local_Board) {
	printf("LocalBoard:\n");
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++)
			printf("%d  ", Local_Board[i][j]);
		printf("\n");
	}
	printf("\n");
}

int Set_On_Board(int** Board, int x, int y, int Value) {
	//Проверка корректности x и y
		int dr[] = { -2, -2, -1, -1, 1, 1, 2, 2, 0 };
		int dc[] = { -1, 1, -2, 2, -2, 2, -1, 1, 0 };
		for (int i = 0; i < 9; i++) {
			if (x + dr[i]<N && y + dc[i]<N && x + dr[i]>=0 && y+dc[i]>=0) {
				Board[x + dr[i]][y + dc[i]] += Value;
			}
		}

		//Print_Board(Board);
		return 0;
}

void Find_Place(int** Local_Board, int X, int Y, int Seted) {
	/*for (int i = 0; i < Seted; i++)
		printf("-");*/
	//printf("%d %d ---\n", X, Y);
	//Если количество установленных фигур равно заданному, получили новое уникальное решение
	if (Seted == L) {
		//printf("   %d %d\n", X, Y);
		EnterCriticalSection(&cs_result);
		//printf("yes!!\n");
		Result++;
		LeaveCriticalSection(&cs_result);
		return;
	}
	//printf("-");
	int j = Y;
	for (int i = X; i < N; i++) {
		while (j < N) {
			//Если клетка не под боем, то ставим
			//printf(" %d %d\n", i, j);
			if (Local_Board[i][j] <= 0) {
				//printf("yes");
				//Установили фигуру
				Set_On_Board(Local_Board, i, j, +1);
				//printf("++++\n");
				//Print_Board(Local_Board);
				//Проверили, либо вышли за пределы доски, поставили нужное количество
				Find_Place(Local_Board, i ,j, Seted + 1);
				//Убрали фигуру
				Set_On_Board(Local_Board, i, j, -1);
				//printf("----\n");
			}
			//printf("\n");
			j++;
		}
		j = 0;
	}
}

void Fill_Local_Board(int** Local_Board) {
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			Local_Board[i][j] = Board[i][j];
}

DWORD WINAPI Thread_Function(void* Empty_Parametr) {
	//Захват мьютекса для копирования доски
	EnterCriticalSection(&cs);
	//Для каждого потока создается отдельная доска
	int** Local_Board = (int**)calloc(N, sizeof(int*));
	for (int i = 0; i < N; i++) {
		Local_Board[i] = (int*)calloc(N, sizeof(int));
	}
	//Заполнение доски
	if (K != 0) Fill_Local_Board(Local_Board);
	LeaveCriticalSection(&cs);

	while (!Queue.empty()) {
		//Захват мьютекса
		
		//Если очередь пуста, то завершаем программу
		if (Queue.empty()) {
			return 0;
		}
		WaitForSingleObject(&Mutex, INFINITY);
		if (Queue.empty()) {
			return 0;
		}
		{
			// Получаем первую границу из очереди
			Borders Border = Queue.front();
			// Удаляем ее из очереди
			Queue.pop();
			//printf("%d %d:\n", Border.X, Border.Y);
			//Освобождение мьютекса
			ReleaseMutex(&Mutex);
			//ставим первую фигуру
			Set_On_Board(Local_Board, Border.X, Border.Y, +1);
			Find_Place(Local_Board, (Border.X), Border.Y, 1);
			Set_On_Board(Local_Board, Border.X, Border.Y, -1);
		}
	}
	return 0;
}



int Write_File(long Time_End) {
	//Запись результатов
	FILE* Output_File = fopen("output.txt", "w");
	fprintf(Output_File, "%d", Result);
	fclose(Output_File);
	return 0;
}
void Close() {
	CloseHandle(&Mutex);

	DeleteCriticalSection(&cs);
	DeleteCriticalSection(&cs_result);
	free(Board);
}

int main(void) {
	
	//Считывание входных данных
	FILE* Input_File = fopen("input.txt", "r");
	fscanf(Input_File, "%d", &N_threads);
	fscanf(Input_File, "%d %d %d", &N, &L, &K);
	Board = (int**)calloc(N, sizeof(int*));
	for (int i = 0; i < N; i++)
		Board[i] = (int*)calloc(N, sizeof(int));
	int x = 0, y = 0;
	for (int i = 0; i < K; i++) {
		fscanf(Input_File, "%d %d", &x, &y);
		Set_On_Board(Board, x, y, +1);
	}
	//Закрытие файла
	fclose(Input_File);
	Print_Board(Board);
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++)
			if (Board[i][j] <= 0) {
				Borders Elem = { i, j };
				Queue.push(Elem);
			}

	
	//Инициализация мьютекса
	event = CreateEvent(NULL, FALSE, TRUE, NULL);
	Mutex = CreateMutex(NULL, FALSE, NULL);
	InitializeCriticalSection(&cs);
	InitializeCriticalSection(&cs_result);
	//Создание потоков
	tids = (HANDLE*)calloc(N_threads, sizeof(HANDLE));
	for (int i = 0; i < N_threads; i++) {
		tids[i] = CreateThread(0, 0, Thread_Function, NULL, 0, NULL);
	}

	//Ожидание завершения всех потоков
	WaitForMultipleObjects(N_threads, tids, TRUE, INFINITE);

	//Запись выходных данных
	if (Write_File(0)) return 1;
}
