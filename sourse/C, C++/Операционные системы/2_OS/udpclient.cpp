#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/select.h>
#include <netdb.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#define WEBHOST "127.0.0.1"

bool massiv[100];
int N;
// Функция извлекает IPv4-адрес из DNS-дейтаграммы.
int sock_err(const char* function, int s) {
	int err;
	err = errno;
	fprintf(stderr, "%s: socket error: %d\n", function, err);
	return -1;
}
// Функция определяет IP-адрес узла по его имени.
// Адрес возвращается в сетевом порядке байтов.
unsigned int get_host_ipn(const char* name) {
	struct addrinfo* addr = 0;
	unsigned int ip4addr = 0;
	// Функция возвращает все адреса указанного хоста
	// в виде динамического однонаправленного списка
	if (0 == getaddrinfo(name, 0, 0, &addr)) {
		struct addrinfo* cur = addr;
		while (cur)	{
			// Интересует только IPv4 адрес, если их несколько - то первый
			if (cur->ai_family == AF_INET)	{
				ip4addr = ((struct sockaddr_in*)cur->ai_addr)->sin_addr.s_addr;
				break;
			}
			cur = cur->ai_next;
		}
		freeaddrinfo(addr);
	}
	return ip4addr;
}
unsigned int recv_response(int s) {
	char datagram[1024];
	while (1){
		int res, chislo;
		fd_set fds;
		struct timeval tv = {0, 100000}; // 100 msec
		FD_ZERO(&fds); FD_SET(s, &fds);
		res = select(s + 1, &fds, 0, 0, &tv);
		if (select(s + 1, &fds, 0, 0, &tv) > 0) {
			// Данные есть, считывание их
			struct sockaddr_in addr;
			int addrlen = sizeof(addr);
			int received = recvfrom(s, datagram, sizeof(datagram), 0, (struct sockaddr*) &addr, (unsigned int*) &addrlen);
			if (received <= 0) { 
				// Ошибка считывания полученной дейтаграммы
				if (sock_err("recvfrom", s)==-1) {
					return 0;
				}
			}
			for (int i=0;i<received;i=i+4){
				memcpy(&chislo,datagram+i,4);
				//printf("          На сервере уже есть id = %d\n",ntohl(chislo));
				massiv[ntohl(chislo)]=1;
			}
		}
		else if (res == 0) {
			// Данных в сокете нет, отправляем те сообщения, которых нет на серваке
			return 1;
		}
		else {
			sock_err("select", s);
			return 0;
		}
	}
}

char to_send_ar[100][200];
int y_s_ar[100];

int File_read (int s, struct sockaddr_in* addr, char namefile[256]){
	FILE* f1 = fopen(namefile, "r");
	if (f1 != NULL) {
	char d_d = 0;
    char d_m = 0;
    uint16_t d_y = 0;
    uint16_t AA = 0;
	char seps[4] = { 0 };
    char* string1=(char*)malloc(1000);
    strcpy(seps, " .\n");
	char* token1 = NULL;
	char* next_token1 = NULL;
    next_token1 = (char*)malloc(sizeof(char) * 21);
    char num[13] ;
    char Mes [500];
	char to_send[500];
	int i= 0;
    int itoe = -1;
		i=0;
		//printf(" file \n");
		while (fgets(string1, 1000, f1) != NULL) {
			memset(Mes, 0, sizeof(Mes));
			{
				token1,next_token1 = NULL;
				token1 = strtok_r(string1, seps, &next_token1);
				if (token1 == NULL) {
					continue;
				}
				itoe++;
				int y_s = 22;
				printf("itoe=%d\n",itoe);
				unsigned int k = htonl(itoe);
				memcpy(to_send, &k, 4);
				d_d = (char)atoi(token1);
				memcpy((to_send + 4), &d_d, 1);
				d_m = (char)atoi(strtok_r(NULL, seps, &next_token1));
				memcpy((to_send + 5), &d_m, 1);
				d_y = htons(atoi(strtok_r(NULL, seps, &next_token1)));
				memcpy((to_send + 6), &d_y, 2);
				AA  = htons(atoi(strtok_r(NULL, seps, &next_token1)));
				memcpy((to_send + 8), &AA, 2);
				strcpy(to_send+10, strtok_r(NULL, seps, &next_token1));
				strcpy(to_send + 22, next_token1);
				//printf("%d %d %d %d %d %s %s\n", itoe, d_d, d_m, d_y, AA, num, Mes);//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
				y_s = y_s + (strlen(next_token1))+1;
				to_send[y_s-1] = 0;// y_s -1?????
				int res = sendto(s, to_send, y_s, MSG_NOSIGNAL, (struct sockaddr*) addr, sizeof(struct sockaddr_in));
				if (res <= 0) {
					return sock_err("sendto", s);
				}
				memcpy(to_send_ar[itoe],to_send,y_s);
				memcpy(&(y_s_ar[itoe]),&y_s,sizeof(y_s));
				if (strcmp(Mes, "stop\n") == 0) {
					N=itoe+1;
					break;
				}
			}
			i++;
		}
		N=itoe+1;
	
	free(string1);
	fclose(f1);
	if (!recv_response(s))
				return -1;
	}
	else{
		printf("что-то не так\n");
		fclose(f1);
		return -1;
	}
	return 1;
}

int send_request(int s, struct sockaddr_in* addr) {
	int res;
	for (int i = 0; i < N; i++)
	{
		if (!massiv[i]){
			res = sendto(s, (to_send_ar[i]), y_s_ar[i], MSG_NOSIGNAL, (struct sockaddr*) addr, sizeof(struct sockaddr_in));
			//printf("res=%d\n",res);
			if (res <= 0)
				return sock_err("sendto", s);
			//usleep(50);
			
		}
	}
	if (!recv_response(s))
				return -1;
	printf("\n");
	return 1;
}


int main(int argc, char* argv[]) {
	char namefile[256] = { 0 };
	struct sockaddr_in addr;
	char* next_token1 = NULL;
	next_token1 = (char*)malloc(sizeof(char) * 21);
	strcpy(next_token1, argv[1]);
	char* ip = strtok(next_token1, ":");
	char* port = strtok(NULL, "\0");
    strcpy(namefile, argv[2]);
	// Создание UDP-сокета
	int s = socket(AF_INET, SOCK_DGRAM, 0);
	if (s < 0)
		return sock_err("socket", s);
	// Заполнение структуры с адресом удаленного узла
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(atoi(port)); // Порт 
	addr.sin_addr.s_addr = get_host_ipn(ip);
	int g=0;    
	File_read(s,&addr,namefile);
	//recv_response(s);
	while (1) {
		// Отправка запроса на удаленный сервер
		printf(" sending request: attempt \n");
		for (g=0; g<N; g++){
			if(!massiv[g]){
				break;
			}
		}
		//printf("\n");
		printf("g= %d, N=%d\n", g, N);//real N=N+1
		for (int j = 0; j < N; j++) {
			printf("%d ", massiv[j]);
		}
		printf("\n");
		if (g==N){
			break;
		}
		if( send_request(s,&addr)<0){
            break;
        }
		// Попытка получить ответ. Если ответ получен - завершение цикла попыток
        //break;
	}
		// Закрытие сокета
		close(s);
		return 0;
}