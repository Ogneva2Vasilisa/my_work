// LINUX
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
using namespace std;
int sock_err(const char* function, int s)
{
    int err;
    #ifdef _WIN32
        err = WSAGetLastError();
    #else
        err = errno;
    #endif
    fprintf(stderr, "%s: socket %d error: %d\n", function, s, err);
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


int set_non_block_mode(int s)
{
    int fl = fcntl(s, F_GETFL, 0);
    return fcntl(s, F_SETFL, fl | O_NONBLOCK);
}
char buffer1[3]={};
string findClient(struct sockaddr_in *addr)
{
    char ip[16];
    char port[5];
    string client;
    inet_ntop(AF_INET, &addr->sin_addr, ip, sizeof(ip));
    client.clear();
    snprintf(port, 5, "%d", htons(addr->sin_port));
    client.append((char *)ip, strlen(ip));
    client += ':';
    client.append((char *)port, strlen(port));
    return client;
}
int writeToFile(char* buffer, int length, string client)
{
    puts(buffer);
    char d_d = buffer[0];
    char d_m = buffer[1];
    uint16_t d_y = 0;
    int16_t A_y_p = 0;
    short AA = 0;
    char num[13]={0};
    memcpy(&A_y_p, buffer+2,2 );
    d_y = ntohs(A_y_p);
    
    memcpy(&A_y_p, buffer+4, 2);
    AA = ntohs(A_y_p);
    memcpy(num, buffer+6,12);
    char* Mes=(char* )malloc(length);
    memcpy(Mes,(buffer+18),length);
    printf("%d	%d	%d	%d	%s  %s\n", d_d, d_m, d_y, AA, num, Mes); //!!!!!!!!!
    FILE* file=fopen("msg.txt","a");
    fprintf(file,"%s ", client.c_str());
    fprintf(file,"%.2d.%.2d.%d %d %s %s\n", d_d, d_m, d_y, AA, num, Mes);
    //fprintf(file, "Считали r=%d\n", length);
    fclose(file);
    printf("%d %d %d\n",length,length-18, strncmp(Mes,"stop", strlen(Mes)));
    if (strncmp(Mes,"stop", strlen(Mes))==0)
    {
        free(Mes);
        return -1;
    }
    
    free(Mes);
    return 0;
}

static int readPacket(struct pollfd pfd1, int sock, sockaddr_in *addr, int index)
{
    char *buffer = new char[1000000];
    char *buffer1 = new char[150];
    int r = 0, write = 0, bu = 0;
    bool mes_ok = false;
    r = recv(sock, buffer, 1000000, 0);
    //printf("before while\n");
    while ( r > 0)
    {
        int j = 0;
        if (buffer[r-1]==0 && r>18){
                printf("mes is ok\n");
                mes_ok=true;
            }
        else
        {
            //printf("I'm sleep, r=%d\n",r);
            usleep(1000000);
            //continue;
        }
        if(!mes_ok){
            j = recv(sock, buffer+r, 1000000, 0);
            while(j > 0){
                    r=r+j;
                    //usleep(100);
                    if (buffer[r-1]==0){
                    printf("mes is ok\n");
                    mes_ok=true;
                    break;
                    }
                    j = recv(sock, buffer+r, 1000000, 0);
            }
            printf("mes is not ok!?\n");
        }
        
        printf("Считали r=%d\n",r);
        string client = findClient(addr);  
        int s_status = send(sock, "ok", 2, 0);
        printf("s_status=%d\n",s_status);
        //проверить!!! - жив ли клиент (16 тест)
        usleep(10);
        if ( mes_ok==false ){
            printf("Вырубился клиент - некому отправлять ok\n");
            delete[] buffer;
            return write;
        }
        printf("Пишем в файл\n");
        write = writeToFile(buffer+4, r, client);
        if (write==-1) {
            return write;
        }
        memset(buffer,0,r);
        memset(buffer1,0,150);
        r=0;mes_ok = false; s_status=0;

    }
    delete[] buffer;
    delete[] buffer1;
    return write;
}

int main (int argc, char **argv)
{
    if (argc != 2)
    {
        printf( "Usage: ./tcpserver <port>\n");
        return 0;
    }
    uint32_t port = atoi(argv[1]);
    int s;
    struct sockaddr_in addr;
    // Создание TCP-сокета
    s = socket(AF_INET, SOCK_STREAM, 0);
    if (s < 0)
        return sock_err("socket", s);
    // Заполнение адреса прослушивания
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port); // Сервер прослушивает порт 9000
    addr.sin_addr.s_addr = htonl(INADDR_ANY); // Все адреса
    // Связывание сокета и адреса прослушивания
    if (bind(s, (struct sockaddr*) &addr, sizeof(addr)) < 0)
        return sock_err("bind", s);
    // Начало прослушивания
    if (listen(s, 1) < 0)
        return sock_err("listen", s);
    int N=255;
    int cs[N]; // Сокеты с подключенными клиентами
    struct pollfd pfd[N+1];
    int i;
    // В отличие от select, массив pfd не обязательно заполнять перед каждым вызовом poll
    for (i = 0; i < N; i++)
    {
        cs[i]=-1;
    }
    pfd[N].fd = s;
    pfd[N].events = POLLIN;
 
    int s_conn=0;//кол-во подключенных сокетов на данный момент
    while (1)
    {
        // Ожидание событий в течение 1 сек
        int ev_cnt = poll(pfd, sizeof(pfd) / sizeof(pfd[0]), 1000);
        if (ev_cnt > 0)
        {
            for (i = 0; i < N; i++)
            {
                //printf("i is %d",i);
                if (pfd[i].revents & POLLHUP)
                {
                    // Сокет cs[i] - клиент отключился. Можно закрывать сокет
                    s_close(pfd[i].fd);
                    printf("connection closed %d\n", cs[i]);
                    continue;
                    //return 0;

                }
                if (pfd[i].revents & POLLERR)
                {
                    // Сокет cs[i] - возникла ошибка. Можно закрывать сокет
                    s_close(pfd[i].fd);
                    printf("connection closed with error %d\n", cs[i]);
                    continue;
                }
                if (pfd[i].revents & POLLIN)
                {   
                    int r_status = readPacket(pfd[i], pfd[i].fd, &addr, i);
                    if (r_status == -1)
                    {
                        printf("powering off \n");
                        for (size_t i = 0; i < s_conn; i++)
                        {
                            close(pfd[i].fd);
                        }
                        close(s);
                        exit(0);
                    }            
                }
            }
            if (pfd[N].revents & POLLIN)
            {
                printf("New connection %d\n", cs[i]);
                // Сокет ls доступен на чтение - можно вызывать accept, принимать
                // новое подключение. Новый сокет следует добавить в cs и создать для
                // него структуру в pfd.
                socklen_t addrlen = sizeof(addr);
                int new_connection = accept(pfd[N].fd, (struct sockaddr *)&addr, &addrlen);
                set_non_block_mode(pfd[N].fd);
                if (new_connection < 0)
                    {
                        break;
                    }
                    s_conn++;
                    size_t i;
                    for (i = 0; i < s_conn; i++)
                    {
                        if (cs[i] == -1)
                        {
                            cs[i] = new_connection;
                            pfd[i].fd = new_connection;
                            pfd[i].events = POLLIN | POLLOUT;
                            break;
                        }
                    }
                    printf("New connection %d\n", cs[i]);

                    memset(buffer1, 0, sizeof(buffer1));
                    int rcv_len = read(pfd[i].fd, buffer1, 1);
                    rcv_len = rcv_len+read(pfd[i].fd, buffer1+1, 1);
                    rcv_len = rcv_len+read(pfd[i].fd, buffer1+1, 1);
                    if (strcmp( buffer1,"put")==0)
                    {
                        printf("He sended \'put\'1 \n");
                    }
            }
        }
        else
        {
            printf("Timeout or error\n");
            // Тайтмаут или ошибка
        }
    }
    for (size_t i = 0; i < s_conn; i++)
    {
        close(pfd[i].fd);
    }
    close(s);
    return 0;
}