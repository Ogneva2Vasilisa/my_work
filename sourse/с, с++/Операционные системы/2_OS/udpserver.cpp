#define _CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <time.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>
#include <vector>
#pragma comment(lib,"ws2_32.lib")

using namespace std;



struct my_sockets {
	struct sockaddr_in addr;
	int descr;
};
struct my_clientss {
	time_t times;
	sockaddr_in addr;
	unsigned int ip;
	unsigned int id [100];
	unsigned short n_id;
	unsigned int i_port;
};



int N = 0;
struct my_sockets cs[100];
vector<struct my_clientss> clientss;
unsigned int port0, port1;

int init()
{
	// Для Windows следует вызвать WSAStartup перед началом использования сокетов
	WSADATA wsa_data;
	return (0 == WSAStartup(MAKEWORD(2, 2), &wsa_data));
}
int sock_err(const char* function, int s)
{
	int err;
	err = WSAGetLastError();
	fprintf(stderr, "%s: socket error: %d\n", function, err);
	return -1;
}
void s_close(int s)
{
	closesocket(s);
}
void deinit()
{
	// Для Windows следует вызвать WSACleanup в конце работы
	WSACleanup();
	for (int i = 0; i < N; i++)

		s_close(cs[i].descr);
}

int writeToFile(char buffer[256], int length, int i) {
	clientss[i].times = time(NULL);//****************************
	FILE* file = fopen("msg.txt", "a");
	if (file == NULL) {
		return -1;
	}
	puts(buffer);
	unsigned short d_y = 0;
	unsigned short A_y_p = 0;
	short AA = 0;
	char num[12] = { 0 };
	memcpy(&A_y_p, buffer + 2, 2);
	d_y = ntohs(A_y_p);

	memcpy(&A_y_p, buffer + 4, 2);
	AA = ntohs(A_y_p);
	memcpy(num, buffer + 6, 12);
	char* Mes = (char*)malloc(length);
	if (Mes == NULL) {
		printf("finish %d", -1);
		return -1;
	}
	memcpy(Mes, (buffer + 18), length);
	//printf("%d	%d	%d	%d	%s  %s\n", d_d, d_m, d_y, AA, num, Mes); //!!!!!!!!!
	fprintf(file, "%u.%u.%u.%u:%d ", (clientss[i].ip >> 24) & 0xFF, (clientss[i].ip >> 16) & 0xFF, (clientss[i].ip >> 8) & 0xFF, (clientss[i].ip) & 0xFF, clientss[i].i_port);
	fprintf(file, "%.2d.%.2d.%d %d %s %s\n", buffer[0], buffer[1], d_y, AA, num, Mes);
	//fprintf(file, "Считали r=%d\n", length);
	fclose(file);
	//printf("%d %d %d\n", length, length - 18, strncmp(Mes, "stop", strlen(Mes)));
	if (strncmp(Mes, "stop", strlen(Mes)) == 0)
	{
		free(Mes);
		return 0;
	}

	free(Mes);
	return 1;
}
int FLAG = 1;
//------------------------------------------------------------M-A-I-N---------------------------------------------------------------
//udpserver.exe 9030 9030
//ruby udpclientemul.rb 127.0.0.1:9030 tosend.txt
int main(int argc, char** argv)
{
	//argc = 3;
	//argv[1] = (char*) "9930";
	//argv[2] = (char*) "9935";
	if (argc != 3)
	{
		printf("Usage: udpserver.exe <port> <port>\n");
		return 0;
	}
	port0 = atoi(argv[1]);
	port1 = atoi(argv[2]);
	N = port1 - port0 + 1;
	init();

	for (int i = 0; i < N; i++)
	{
		cs[i].descr = socket(AF_INET, SOCK_DGRAM, 0);
		if (cs[i].descr < 0)
			return sock_err("socket", cs[i].descr);

		// Заполнение структуры с адресом прослушивания узла
		memset(&(cs[i].addr), 0, sizeof(cs[i].addr));
		cs[i].addr.sin_family = AF_INET;
		cs[i].addr.sin_port = htons(port0 + i); // Будет прослушиваться порт 8000
		cs[i].addr.sin_addr.s_addr = htonl(INADDR_ANY);
		// Связь адреса и сокета, чтобы он мог принимать входящие дейтаграммы
		if (bind(cs[i].descr, (struct sockaddr*)&(cs[i].addr), sizeof(cs[i].addr)) < 0)
			return sock_err("bind", cs[i].descr);
		unsigned long mode = 1;
		ioctlsocket(cs[i].descr, FIONBIO, &mode);
		listen(cs[i].descr, 200);
	}
	printf("Connected %d sockets ", N);
	fd_set rfd;
	fd_set wfd;
	int nfds = cs[N - 1].descr;
	struct timeval tv = { 0, 1 };
	char datagram[256] = { 0 };
	int i, len = 0;
	sockaddr_in from;
	memset(&from, 0, sizeof(from));
	int fromlen = sizeof(from);
	vector<int>::size_type j;
	while (FLAG)
		//while (1)
	{
		FD_ZERO(&rfd);
		FD_ZERO(&wfd);
		for (i = 0; i < N; i++)
		{
			FD_SET(cs[i].descr, &rfd);
			FD_SET(cs[i].descr, &wfd);
			if (nfds < cs[i].descr)
				nfds = cs[i].descr;
		}
		//printf("%d ", nfds);

		vector<my_clientss>::iterator it = clientss.begin();
		while (clientss.end() != clientss.begin() && it != clientss.end()) {
			time_t mytime = time(NULL);
			if (it->times + 30 <= mytime || it->n_id>=20) {
				printf("Client in port %d is removed", it->i_port );
				clientss.erase(it);
				//it = clientss.begin();
			}
			else {
				++it;
			}
		}

		select(nfds + 1, &rfd, &wfd, 0, &tv);
		{
			for (i = 0; i < N; i++)
			{
				if (FD_ISSET(cs[i].descr, &rfd))
				{
					// Сокет cs[i] доступен для чтения. Функция recv вернет данные,	recvfrom - дейтаграмму
					memset(datagram, 0, sizeof(datagram));
					len = 0;
					int rcv = recvfrom(cs[i].descr, datagram, sizeof(datagram), 0, (sockaddr*)&from, &fromlen);
					printf("%d  -  ", recv);
					if (rcv > 0) {
						string client;
						unsigned int ip = ntohl(from.sin_addr.s_addr);
						unsigned short port = ntohs(from.sin_port);
						//printf("Datagram received from address: %u. %u. %u. %u", (ip >> 24) & 0xFF, (ip >> 16) & 0xFF, (ip >> 8) & 0xFF, (ip) & 0xFF);

						//проверка на уникальность порта или его добавление в базу
						for (j = 0; j < clientss.size(); j++) {
							if (clientss[j].i_port == port && clientss[i].ip == ip) {
								break;
							}
						}
						if (j == clientss.size()) {
							struct my_clientss cli_i;
							cli_i.ip = ip;
							cli_i.addr = from;
							cli_i.i_port = port;
							cli_i.times = 0;
							clientss.push_back(cli_i);
						}
						//clientss[j] - наш клиент

						for (int y = 0; y < rcv; y++) {
							if (datagram[y] == '\n')
								break;
							len++;
						}
						printf(" string len is: %d\n", len);
						unsigned int A_y = 0;
						memcpy(&A_y, datagram, 4);
						//printf("%d %d %d %d   ", buffer[0], buffer[1], buffer[2], buffer[3]);
						unsigned int idd = ntohl(A_y);
						//printf("%d  :   ", idd);

						// проверка на уникальность сообщения или его добавление
						unsigned int g;
						for (g = 0; g < clientss[j].n_id; g++) {
							if (clientss[j].id[g] == idd) {
								break;
							}
						}
						if (g == clientss[j].n_id) {
							clientss[j].id[clientss[j].n_id] = idd;
							clientss[j].n_id++;
							// запись в файл'
							FLAG = writeToFile(datagram + 4, len, j);
							//exit(0);
						}

						if (FD_ISSET(cs[i].descr, &wfd)) {
							printf("fff\n");
							// Сокет cs[i] доступен для записи. Функция send и sendto будет	успешно завершена

							char buf[200];
							for (g = 0; g < clientss[j].n_id; g++) {
								A_y = htonl(clientss[j].id[g]);
								//A_y = clientss[j].id[g];
								memcpy(buf + g * 4, &A_y, 4);
							}
							for (int g = 0; g < clientss[j].n_id * 4; g++)
							{
								printf("%d ", buf[g]);
							}
							printf("= %d\n", clientss[j].n_id * 4);
							sendto(cs[i].descr, (char*)&buf, clientss[j].n_id * 4, 0, (struct sockaddr*)&clientss[j].addr, sizeof(clientss[j].addr));
							//if(sendto(cs[i].descr, (char*)&buf, clientss[j].n_id * 4, 0, (struct sockaddr*)&clientss[j].addr, sizeof(clientss[j].addr)))
								//printf("yyyyyyyyyyyyyyyyyy");

						}


					}

					

				}

			}
		}

	}

	deinit();
	printf("finish %d",FLAG);
	return 0;
}