#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
// Директива линковщику: использовать библиотеку сокетов
#pragma comment(lib, "ws2_32.lib")
#include <stdio.h>
#include <string>
//#include <string.h>
//#define WEBHOST "127.0.0.1"
//#define WEBHOST "192.168.1.118"
int init()
{
#ifdef _WIN32
	// Для Windows следует вызвать WSAStartup перед началом использования сокетов
	WSADATA wsa_data;
	return (0 == WSAStartup(MAKEWORD(2, 2), &wsa_data));
#else
	return 1; // Для других ОС действий не требуется
#endif
}
void deinit()
{
#ifdef _WIN32
	// Для Windows следует вызвать WSACleanup в конце работы
	WSACleanup();
#else
	// Для других ОС действий не требуется
#endif
}
int sock_err(const char* function, int s)
{
	int err;
#ifdef _WIN32
	err = WSAGetLastError();
#else
	err = errno;
#endif
	fprintf(stderr, "%s: socket error: %d\n", function, err);
	return -1;
}
void s_close(int s)
{
#ifdef _WIN32
	closesocket(s);
#else
	close(s);
#endif
}
// Функция определяет IP-адрес узла по его имени.
// Адрес возвращается в сетевом порядке байтов.
unsigned int get_host_ipn(const char* name)
{
	struct addrinfo* addr = 0;
	unsigned int ip4addr = 0;
	// Функция возвращает все адреса указанного хоста
	// в виде динамического однонаправленного списка
	if (0 == getaddrinfo(name, 0, 0, &addr))
	{
		struct addrinfo* cur = addr;
		while (cur)
		{
			// Интересует только IPv4 адрес, если их несколько - то первый
			if (cur->ai_family == AF_INET)
			{
				ip4addr = ((struct sockaddr_in*)cur->ai_addr)->sin_addr.s_addr;
				break;
			}
			cur = cur->ai_next;
		}
		freeaddrinfo(addr);
	}
	return ip4addr;
}
// Отправляет http-запрос на удаленный сервер
int send_request(int s, char* request)
{
	int size = strlen(request);
	int sent = 0;
#ifdef _WIN32
	int flags = 0;
#else
	int flags = MSG_NOSIGNAL;
#endif
	while (sent < size)
	{
		// Отправка очередного блока данных
		int res = send(s, request + sent, size - sent, flags);
		if (res < 0)
			return sock_err("send", s);
		sent += res;
		printf(" %d bytes sent.\n", sent);
	}
	return 0;
}


int writeToFile(char* buffer, int length, char* client, char* namefile)
{
	//puts(buffer);
	int y_s = 0;
	char* Mes = (char*)calloc(length-4, 1);
	while (y_s<length-1)
	{
		y_s = y_s + 4;
		char d_d = buffer[0 + y_s];
		char d_m = buffer[1 + y_s];
		uint16_t d_y = 0;
		int16_t A_y_p = 0;
		short AA = 0;
		char num[13] = { 0 };
		memcpy(&A_y_p, buffer + 2 + y_s, 2);
		d_y = ntohs(A_y_p);

		memcpy(&A_y_p, buffer + 4 + y_s, 2);
		AA = ntohs(A_y_p);
		memcpy(num, buffer + 6 + y_s, 12);

		strcpy(Mes, (buffer + 18+y_s));
		y_s = y_s + 18+ strlen(Mes)+1;
		printf("%d	%d	%d	%d	%s  %s %d\n", d_d, d_m, d_y, AA, num, Mes, y_s); //!!!!!!!!!
		FILE* file = fopen(namefile, "a");
		fprintf(file, "%s ", client);
		fprintf(file, "%.2d.%.2d.%d %d %s %s\n", d_d, d_m, d_y, AA, num, Mes);
		//fprintf(file, "Считали r=%d\n", length);
		fclose(file);
		printf("%d %d %d\n", length, length - 18, strncmp(Mes, "stop", strlen(Mes)));
		if (strncmp(Mes, "stop", strlen(Mes)) == 0)
		{
			free(Mes);
			return -1;
		}
	}
	free(Mes);
	return 0;
}

//----------------------------
uint32_t itoe = 0;
unsigned char d_d = 0;
unsigned char d_m = 0;
uint16_t d_y = 0;
uint16_t AA = 0;
char gg = 0;//flag to recv
//---------------------------
//using namespace std;
int main(int argc, char* argv[])
{
	//argc = 4;
	//argv[2] = (char*)"get";
	//argv[1] = (char*)"127.0.0.1:9000";
	////argv[2] = (char*)"file1.txt";
	//argv[3] = (char*)"file1.txt";

	//допка
	if (argc == 4 || strcmp(argv[2],(char*)"get") == 0) {
		char namefile[256] = { 0 };
		int s;
		struct sockaddr_in addr;
		short g = 0;
		char ok[3] = { 0 };
		char* token1 = NULL;
		char* next_token1 = NULL;
		strcpy(namefile, argv[3]);
		next_token1 = (char*)malloc(sizeof(char) * 21);
		strcpy(next_token1, argv[1]);
		char* ip = strtok(next_token1, ":");
		char* port = strtok(NULL, "\0");
		init();
		// Создание TCP-сокета
		s = socket(AF_INET, SOCK_STREAM, 0);//интернет, тсп, 0
		if (s < 0)
			return sock_err("socket", s);
		// Заполнение структуры с адресом удаленного узла
		memset(&addr, 0, sizeof(addr));//sin_zero заполняем нулями
		addr.sin_family = AF_INET;//интернет, а не локальная машина
		addr.sin_port = htons(atoi(port));//порт
		addr.sin_addr.s_addr = get_host_ipn(ip); //IP-адрес
		// Установка соединения с удаленным хостом
		while (g < 10)
		{
			if (connect(s, (struct sockaddr*)&addr, sizeof(addr)) == 0)
				break;
			g++;
			Sleep(10);
		}
		if (g == 10)
			return sock_err("connect", s);
		free(next_token1);
		// Отправка запроса на удаленный сервер
		send_request(s, (char*)"get");
		char* buffer = new char[1000000];
		int r = recv(s, buffer, 1000000, 0);
		for (int i = 0; i < r; i++) {
			printf("%d ", buffer[i]);
		}
		printf("\n");
		while (r>0) {
			//int s_status = send(s, "ok", 2, 0);
			writeToFile(buffer, r, argv[1], namefile);
			r = recv(s, buffer, 1000000, 0);
			for (int i = 0; i < r; i++) {
				printf("%d ", buffer[i]);
			}
			printf("\n");
			printf("r=%d\n", r);
		}
		delete[] buffer;
		return 0;
	}
	else {
		char namefile[256] = { 0 };
		char* num = (char*)malloc(sizeof(char) * (12 + 1));
		char* Mes = (char*)malloc(sizeof(char) * 12);
		short g = 0;
		int s;
		struct sockaddr_in addr;
		FILE* f1;
		int r;
		char ok[3] = { 0 };
		char* token1 = NULL;
		char* next_token1 = NULL;
		next_token1 = (char*)malloc(sizeof(char) * 21);
		strcpy(next_token1, argv[1]);
		char* ip = strtok(next_token1, ":");
		char* port = strtok(NULL, "\0");
		init();
		// Создание TCP-сокета
		s = socket(AF_INET, SOCK_STREAM, 0);//интернет, тсп, 0
		if (s < 0)
			return sock_err("socket", s);
		// Заполнение структуры с адресом удаленного узла
		memset(&addr, 0, sizeof(addr));//sin_zero заполняем нулями
		addr.sin_family = AF_INET;//интернет, а не локальная машина
		addr.sin_port = htons(atoi(port));//порт
		addr.sin_addr.s_addr = get_host_ipn(ip); //IP-адрес
		// Установка соединения с удаленным хостом
		while (g < 10)
		{
			if (connect(s, (struct sockaddr*)&addr, sizeof(addr)) == 0)
				break;
			g++;
			Sleep(10);
		}
		if (g == 10)
			return sock_err("connect", s);
		free(next_token1);
		// Отправка запроса на удаленный сервер
		send_request(s, (char*)"put");
		char* string1 = (char*)malloc(1000000);
		char seps[4] = { 0 };
		strcpy(seps, " .\n");
		int i = 0;
		strcpy(namefile, argv[2]);
		if (fopen_s(&f1, namefile, "r") == 0) {
			{
				memset(string1, 0, 1000000);
				while (fgets(string1, 1000000, f1) != NULL) {
					if (strcmp(Mes, "stop") == 0) { break; }
					memset(Mes, 0, sizeof(Mes));
					itoe = i;
					{
						token1 = NULL;
						next_token1 = NULL;
						token1 = strtok_s(string1, seps, &next_token1);
						if (token1 == NULL)
						{
							continue;
						}
						d_d = (char)atoi(token1);
						d_m = (char)atoi(strtok_s(NULL, seps, &next_token1));
						d_y = htons(atoi(strtok_s(NULL, seps, &next_token1)));
						AA = htons(atoi(strtok_s(NULL, seps, &next_token1)));
						strcpy(num, strtok_s(NULL, seps, &next_token1));
						if (strlen(next_token1) == 0) {
							printf("Data Error!\n");
							break;
						}
						Mes = (char*)realloc(Mes, (strlen(next_token1) + 1) * sizeof(char));
						if (Mes == NULL) {
							printf("No such memory!\n");
							break;
						}
						strncpy(Mes, next_token1, strlen(next_token1));
						Mes[strlen(next_token1) - 1] = 0;
						//printf("%d		%d		%d		%d		%d		%s		%s\n", itoe, d_d, d_m, d_y, AA, num, Mes);//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
						// Get next messege  ?:
						/*for (int p = 0; p < strlen(Mes); p++)
						{
							printf("%d ", Mes);
						}
						printf("\n");*/
						token1 = strtok_s(NULL, seps, &next_token1);
						if (token1 != NULL)
						{
							token1 = strtok_s(NULL, seps, &next_token1);
						}
					}
					int status = 0;
					if (d_d > 31 || d_d < 1 || d_m < 1 || d_m>12 || ntohs(d_y) < 2000 || ntohs(d_y) > 2024) {
						printf("FileData is error date\n");
						return -1;
					}
					if (atoi(num) > 70000000000) {
						printf("FileData is error num\n");
						return -1;
					}
					int y_s = sizeof(char) * 10;
					char* to_send = (char*)malloc(sizeof(char) * 10);
					memset(to_send, 0, 10);
					unsigned int k = htonl(itoe);
					memcpy(to_send, &k, 4);
					memcpy((to_send + 4), &d_d, 1);
					memcpy((to_send + 5), &d_m, 1);
					memcpy((to_send + 6), &d_y, 2);
					memcpy((to_send + 8), &AA, 2);

					y_s = y_s + sizeof(char) * (strlen(num));
					to_send = (char*)realloc(to_send, y_s);
					if (to_send != NULL)
					{
						to_send[y_s - 1] = 0;
						memcpy(&to_send[10], num, strlen(num));
					}
					printf("\n");
					y_s = y_s + (strlen(Mes)) + 1;
					to_send = (char*)realloc(to_send, y_s);
					if (to_send != NULL)
					{
						memcpy(to_send + 22, Mes, strlen(Mes));
					}
					to_send[y_s - 1] = 0;
					printf("\n");
					if (y_s < 19) {
						printf("FileData is error!!\n");
						return -1;
					}
					send(s, to_send, y_s, 0);
					while ((r = recv(s, ok, 2, 0)))
					{
						if (strcmp(ok, "o") == 0) {
							gg = 1;
						}
						if (strcmp(ok, "k") == 0 && gg == 1) {
							printf("ok\n");
							memset(ok, 0, 2);
							break;
						}
						if (strcmp(ok, "ok") == 0)
						{
							printf("ok\n");
							memset(ok, 0, 2);
							break;
						}
					}
					gg = 0;
				}
				i++;
			}
		}
		else
		{
			printf("Open file is er\n");
			return -1;
		}
		fclose(f1);
		s_close(s);
		deinit();
		free(string1);
		free(num);
		free(Mes);
		return 0;
	}
}