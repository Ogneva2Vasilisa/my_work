#define _CRT_SECURE_NO_WARNINGS
#include <winsock2.h>
#include <windows.h>    
#include <mswsock.h>  
#include <stdio.h>  
#include <sysinfoapi.h>
#include <AclAPI.h>
#include <sddl.h>
#include <iostream>
#include <filesystem>
#include <shlwapi.h>
#include <wincrypt.h>
#include "../BSIT_3_Func.cpp"

#pragma comment(lib, "ws2_32.lib")  
#pragma comment(lib, "mswsock.lib") 
#pragma comment(lib, "Shlwapi.lib")
#pragma comment(lib, "crypt32.lib")

#define MAX_CLIENTS (100) 
#define WIN32_LEAN_AND_MEAN  
#define SESSION_KEY_SIZE 16

double getSysOpType()
{
    double ret = 0.0;
    NTSTATUS(WINAPI * RtlGetVersion)(LPOSVERSIONINFOEXW);
    OSVERSIONINFOEXW osInfo;

    *(FARPROC*)&RtlGetVersion = GetProcAddress(GetModuleHandleA("ntdll"), "RtlGetVersion");

    if (NULL != RtlGetVersion)
    {
        osInfo.dwOSVersionInfoSize = sizeof(osInfo);
        RtlGetVersion(&osInfo);
        ret = (double)osInfo.dwMajorVersion;
    }
    return ret;
}

struct client_ctx

{
    int socket;
    CHAR buf_recv[10240];           // Буфер приема   
    CHAR buf_send[10240];           // Буфер отправки   
    unsigned int sz_recv;         // Принято данных 
    unsigned int sz_send_total;   // Данных в буфере отправки  
    unsigned int sz_send;         // Данных отправлено 
    // Структуры OVERLAPPED для уведомлений о завершении
    OVERLAPPED overlap_recv;
    OVERLAPPED overlap_send;
    OVERLAPPED overlap_cancel;

    DWORD flags_recv; // Флаги для WSARecv  

};


// Прослушивающий сокет и все сокеты подключения хранятся  
// в массиве структур (вместе с overlapped и буферами)  
struct client_ctx g_ctxs[1 + MAX_CLIENTS];
int g_accepted_socket;
HANDLE g_io_port;

// Функция стартует операцию чтения из сокета  

void schedule_read(DWORD idx)

{
    WSABUF buf;
    buf.buf = g_ctxs[idx].buf_recv + g_ctxs[idx].sz_recv;
    buf.len = sizeof(g_ctxs[idx].buf_recv) - g_ctxs[idx].sz_recv;
    memset(&g_ctxs[idx].overlap_recv, 0, sizeof(OVERLAPPED));
    g_ctxs[idx].flags_recv = 0;
    WSARecv(g_ctxs[idx].socket, &buf, 1, NULL, &g_ctxs[idx].flags_recv, &g_ctxs[idx].overlap_recv, NULL);
}
// Функция стартует операцию отправки подготовленных данных в сокет 
void schedule_write(DWORD idx)

{
    WSABUF buf; buf.buf = g_ctxs[idx].buf_send + g_ctxs[idx].sz_send;
    buf.len = g_ctxs[idx].sz_send_total - g_ctxs[idx].sz_send;
    memset(&g_ctxs[idx].overlap_send, 0, sizeof(OVERLAPPED));
    WSASend(g_ctxs[idx].socket, &buf, 1, NULL, 0, &g_ctxs[idx].overlap_send, NULL);
}
// Функция добавляет новое принятое подключение клиента  

void add_accepted_connection()

{
    DWORD i; // Поиск места в массиве g_ctxs для вставки нового подключения  
    for (i = 0; i < sizeof(g_ctxs) / sizeof(g_ctxs[0]); i++)
    {
        if (g_ctxs[i].socket == 0)
        {
            unsigned int ip = 0;
            struct sockaddr_in* local_addr = 0, * remote_addr = 0;
            int local_addr_sz, remote_addr_sz;
            GetAcceptExSockaddrs(g_ctxs[0].buf_recv, g_ctxs[0].sz_recv, sizeof(struct sockaddr_in) + 16,
                sizeof(struct sockaddr_in) + 16, (struct sockaddr**)&local_addr, &local_addr_sz, (struct sockaddr**)&remote_addr,
                &remote_addr_sz);
            if (remote_addr) ip = ntohl(remote_addr->sin_addr.s_addr);
            printf(" connection %u created, remote IP: %u.%u.%u.%u\n", i, (ip >> 24) & 0xff, (ip >> 16) & 0xff,
                (ip >> 8) & 0xff, (ip) & 0xff);
            g_ctxs[i].socket = g_accepted_socket;
            // Связь сокета с портом IOCP, в качестве key используется индекс массива  
            if (NULL == CreateIoCompletionPort((HANDLE)g_ctxs[i].socket, g_io_port, i, 0))
            {
                printf("CreateIoCompletionPort error: %x\n", GetLastError());
                return;
            }
            // Ожидание данных от сокета  
            schedule_read(i);
            return;
        }
    }
    // Место не найдено => нет ресурсов для принятия соединения  
    closesocket(g_accepted_socket);
    g_accepted_socket = 0;
}
// Функция стартует операцию приема соединения  

void schedule_accept()
{
    // Создание сокета для принятия подключения (AcceptEx не создает сокетов)  
    g_accepted_socket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
    memset(&g_ctxs[0].overlap_recv, 0, sizeof(OVERLAPPED));
    // Принятие подключения. 
    // Как только операция будет завершена - порт завершения пришлет уведомление. 
    // Размеры буферов должны быть на 16 байт больше размера адреса согласно документации разработчика ОС
    AcceptEx(g_ctxs[0].socket, g_accepted_socket, g_ctxs[0].buf_recv, 0, sizeof(struct sockaddr_in) + 16, sizeof(struct
        sockaddr_in) + 16, NULL, &g_ctxs[0].overlap_recv);
}

int is_string_received(DWORD idx, int* len)
{
    DWORD i;
    for (i = 0; i < g_ctxs[idx].sz_recv; i++)
    {
        if (g_ctxs[idx].buf_recv[i] == '\n')
        {
            *len = (int)(i + 1);
            return 1;
        }
    }
    if (g_ctxs[idx].sz_recv == sizeof(g_ctxs[idx].buf_recv))
    {
        *len = sizeof(g_ctxs[idx].buf_recv);
        return 1;
    }
    return 0;
}

void read_after_recv(int key) {
    memset(g_ctxs[key].buf_recv, 0, sizeof(g_ctxs[key].buf_recv));
    g_ctxs[key].sz_recv = 0;
    schedule_read(key);
}

void clear_send_buffer(int key) {
    memset(g_ctxs[key].buf_send, 0, sizeof(g_ctxs[key].buf_send));
    g_ctxs[key].sz_send = 0;
}

void KeyExchange(BYTE* sessionKey, DWORD sessionKeyLen) {
    while (1) {
        DWORD transferred;
        ULONG_PTR key;
        OVERLAPPED* lp_overlap;
        DWORD KeyLen = 72;
        BOOL b = GetQueuedCompletionStatus(g_io_port, &transferred, &key, &lp_overlap, 1000);
        if (b) {
            if (&g_ctxs[key].overlap_recv == lp_overlap) {
                int len;
                g_ctxs[key].sz_recv += transferred;
                if (g_ctxs[key].sz_recv > 147) {
                    BYTE* publicKey = new BYTE[148];
                    int publicKeyLen = 148;
                    for (int i = 0; i < publicKeyLen; i++) {
                        publicKey[i] = g_ctxs[key].buf_recv[i];
                    }
                    HCRYPTKEY hKey;
                    HCRYPTKEY hPubKey;
                    BYTE encryptedSessionKey[1000];
                    memset(encryptedSessionKey, 0, 1000);
                    for (int i = 0; i < 72; i++) {
                        encryptedSessionKey[i] = sessionKey[i];
                    }

                    CryptAcquireContextA(&hKey, NULL, NULL, PROV_RSA_AES, CRYPT_VERIFYCONTEXT);
                    CryptImportKey(hKey, publicKey, publicKeyLen, 0, 0, &hPubKey);
                    CryptEncrypt(hPubKey, NULL, TRUE, 0, encryptedSessionKey, &KeyLen, 1000);

                    for (int i = 0; i < KeyLen; i++) {
                        g_ctxs[key].buf_send[i] = encryptedSessionKey[i];
                    }

                    memset(encryptedSessionKey, 0, 1000);
                    for (int i = 0; i < 72; i++) {
                        encryptedSessionKey[i] = sessionKey[i + 72];
                    }

                    //std::string dataToEncrypt = "Hello";
                    //DWORD dataLen = dataToEncrypt.size() + 1; // Включаем нуль-терминатор

                    // Шифрование с использованием публичного ключа
                    //BYTE* encryptedData = new BYTE[1000];
                    //memcpy(encryptedData, dataToEncrypt.c_str(), dataLen);

                    //DWORD encryptedDataLen = dataLen;

                    //CryptEncrypt(hPubKey, NULL, TRUE, 0, encryptedData, &encryptedDataLen, 1000);

                    CryptEncrypt(hPubKey, NULL, TRUE, 0, encryptedSessionKey, &KeyLen, 1000);

                    for (int i = 0; i < KeyLen; i++) {
                        g_ctxs[key].buf_send[i + 128] = encryptedSessionKey[i];
                    }
                    /*for (int i = 0; i < encryptedDataLen; i++) {
                        g_ctxs[key].buf_send[i] = encryptedData[i];
                    }*/


                    g_ctxs[key].sz_send_total = KeyLen * 2;
                    g_ctxs[key].sz_send = 0;
                    schedule_write(key);
                    read_after_recv(key);
                    break;
                }
                else {
                    schedule_read(0);
                }
            }
        }
    }
}


void io_serv()
{
    WSADATA wsa_data;
    HCRYPTPROV hProv;
    HCRYPTKEY hKey = NULL;
    DWORD keyLength = 256;

    // Инициализация криптографического провайдера
    if (!CryptAcquireContextA(&hProv, NULL, NULL, PROV_RSA_AES, CRYPT_VERIFYCONTEXT)) {
        std::cerr << "Ошибка инициализации криптографического провайдера." << std::endl;
        exit(1);
    }

    if (!CryptGenKey(hProv, CALG_AES_256, CRYPT_EXPORTABLE | (keyLength << SESSION_KEY_SIZE), &hKey)) {
        std::cerr << "Error in CryptGenKey: " << GetLastError() << std::endl;
        CryptReleaseContext(hProv, 0);
        exit(1);
    }

    DWORD dwDataLen = 0;
    if (!CryptExportKey(hKey, 0, OPAQUEKEYBLOB, 0, NULL, &dwDataLen)) {
        std::cerr << "Ошибка получения длины публичного ключа." << std::endl;
        CryptReleaseContext(hProv, 0);
        exit(1);
    }

    //BYTE* pubKeyBlob = new BYTE[pubKeyLen];
    //BYTE pubKeyBlob[149] = {0};
    BYTE* sessionKey = new BYTE[dwDataLen];
    if (!CryptExportKey(hKey, 0, OPAQUEKEYBLOB, 0, sessionKey, &dwDataLen)) {
        std::cerr << "Ошибка экспорта публичного ключа." << std::endl;
        //delete[] pubKeyBlob;
        CryptReleaseContext(hProv, 0);
        exit(1);
    }

    //// Генерация случайного сеансового ключа
    //if (!CryptGenRandom(hProv, SESSION_KEY_SIZE, sessionKey)) {
    //    std::cerr << "Ошибка генерации случайного числа." << std::endl;
    //    CryptReleaseContext(hProv, 0);
    //    exit(1);
    //}

    //CryptReleaseContext(hProv, 0);

    if (WSAStartup(MAKEWORD(2, 2), &wsa_data) == 0)
    {
        printf("WSAStartup ok\n");
    }
    else
    {
        printf("WSAStartup error\n");
    }
    struct sockaddr_in addr;
    // Создание сокета прослушивания  
    SOCKET s = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
    // Создание порта завершения  
    g_io_port = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
    if (NULL == g_io_port)
    {
        printf("CreateIoCompletionPort error: %x\n", GetLastError());
        return;
    }
    // Обнуление структуры данных для хранения входящих соединений  
    memset(g_ctxs, 0, sizeof(g_ctxs));
    memset(&addr, 0, sizeof(addr)); addr.sin_family = AF_INET; addr.sin_port = htons(9000);
    if (bind(s, (struct sockaddr*)&addr, sizeof(addr)) < 0 || listen(s, 1) < 0) { printf("error bind() or listen()\n"); return; }
    printf("Listening: %hu\n", ntohs(addr.sin_port));
    // Присоединение существующего сокета s к порту io_port.  
    // В качестве ключа для прослушивающего сокета используется 0  
    if (NULL == CreateIoCompletionPort((HANDLE)s, g_io_port, 0, 0))
    {
        printf("CreateIoCompletionPort error: %x\n", GetLastError());
        return;
    }
    g_ctxs[0].socket = s;
    // Старт операции принятия подключения. 
    schedule_accept();
    // Бесконечный цикл принятия событий о завершенных операциях 
    while (1)
    {
        DWORD transferred;
        ULONG_PTR key;
        OVERLAPPED* lp_overlap;
        // Ожидание событий в течение 1 секунды  
        BOOL b = GetQueuedCompletionStatus(g_io_port, &transferred, &key, &lp_overlap, 1000);
        if (b)
        {
            // Поступило уведомление о завершении операции  
            if (key == 0) // ключ 0 - для прослушивающего сокета 
            {
                g_ctxs[0].sz_recv += transferred;
                // Принятие подключения и начало принятия следующего 
                add_accepted_connection();
                schedule_accept();
                KeyExchange(sessionKey, dwDataLen);
            }
            else
            {
                // Иначе поступило событие по завершению операции от клиента. // Ключ key - индекс в массиве g_ctxs
                if (&g_ctxs[key].overlap_recv == lp_overlap)
                {
                    int len;
                    // Данные приняты: 
                    if (transferred == 0)
                    {
                        // Соединение разорвано  
                        CancelIo((HANDLE)g_ctxs[key].socket);
                        PostQueuedCompletionStatus(g_io_port, 0, key,
                            &g_ctxs[key].overlap_cancel);
                        continue;
                    }
                    g_ctxs[key].sz_recv += transferred;
                    //if (is_string_received(key, &len))
                    if (g_ctxs[key].buf_recv[g_ctxs[key].sz_recv - 1] == 0)
                    {
                        DWORD startlen = g_ctxs[key].sz_recv - 1;
                        CryptDecrypt(hKey, 0, TRUE, 0, (BYTE*)g_ctxs[key].buf_recv, &startlen);
                        g_ctxs[key].buf_recv[startlen] = 0;
                        // Если строка полностью пришла, то сформировать ответ и начать его отправлять
                        //printf("%s\n", g_ctxs[key].buf_recv);
                        

                        puts(g_ctxs[key].buf_send);

                        if (strcmp(g_ctxs[key].buf_recv, "close") == 0) {
                            sprintf(g_ctxs[key].buf_send, "Terminating..\n");
                            g_ctxs[key].sz_send_total = strlen(g_ctxs[key].buf_send);
                            g_ctxs[key].sz_send = 0; schedule_write(key);
                            memset(g_ctxs[key].buf_recv, 0, sizeof(g_ctxs[key].buf_recv));
                            g_ctxs[key].sz_recv = 0;
                            CancelIo((HANDLE)g_ctxs[key].socket);
                            PostQueuedCompletionStatus(g_io_port, 0, key, &g_ctxs[key].overlap_cancel);
                            return;
                        }
                        else if (strncmp(g_ctxs[key].buf_recv, "help", 4) == 0) {
                            sprintf(g_ctxs[key].buf_send, "help 						- Это сообщение\nclose						- Закрыть соединение\nversionOS					- Тип и версия ОС\ncur_time 					- Текущее время\ntime_OS 					- Время, прошедшее с момента запуска ОС\ninf_memory					- Информация об используемой памяти\ndisks 						- Типы подключенных дисков (локальный/сетевой/съемный, файловая система)\nfree_memory 					- Свободное место на локальных дисках\nrights <file/folder/reestr>			- Права доступа в текстовом виде к указанному файлу/папке/ключу реестра\nowner <file/folder/reestr>			- Владелец файла / папки / ключа реестра\n");
                            printf("%s\n", g_ctxs[key].buf_send);
                        }
                        else if (strncmp(g_ctxs[key].buf_recv, "versionOS", 9) == 0) {

                            sprintf(g_ctxs[key].buf_send, "%s\n", GetWindowsVersionString().c_str()); //

                        }
                        else if (strncmp(g_ctxs[key].buf_recv, "cur_time", 8) == 0) {
                            char* o = curtime();
                            printf("%s\n", o);
                            sprintf(g_ctxs[key].buf_send, "%s", o);//

                        }
                        else if (strncmp(g_ctxs[key].buf_recv, "time_OS",7) == 0) {
                            char* o = time_OS();
                            printf("%s\n", o);
                            sprintf(g_ctxs[key].buf_send, "%s\n", o);//

                        }
                        else if (strncmp(g_ctxs[key].buf_recv, "inf_memory",10) == 0) {
                            char* o = inf_memory();
                            printf("%s\n", o);
                            sprintf(g_ctxs[key].buf_send, "%s\n", o);//

                        }
                        else if (strncmp(g_ctxs[key].buf_recv, "disks",5) == 0) {
                            printf("%s\n", disks1().c_str());
                            sprintf(g_ctxs[key].buf_send, "%s\n", disks1().c_str());//

                        }
                        else if (strncmp(g_ctxs[key].buf_recv, "free_memory", 11) == 0) {
                            printf("%s\n", free_memory().c_str());
                            sprintf(g_ctxs[key].buf_send, "%s\n", free_memory().c_str());//

                        }
                        else if (strncmp(g_ctxs[key].buf_recv, "rights ", 7) == 0) {
                            wchar_t* wstr = new wchar_t[MAX_PATH];
                            printf("rights\n");
                            char yy[MAX_PATH] = { 0 };
                            strncpy(yy, g_ctxs[key].buf_recv, strlen(g_ctxs[key].buf_recv) - 1);
                            
                            MultiByteToWideChar(CP_UTF8, 0, yy + 7, -1, wstr, MAX_PATH);
                            sprintf(g_ctxs[key].buf_send, "%s\n", rights2(wstr).c_str());
                            printf("%s\n", rights2(wstr).c_str());
                        }
                        else if (strncmp(g_ctxs[key].buf_recv, "owner ", 7) == 0) {
                            wchar_t* wstr = new wchar_t[MAX_PATH];
                            char yy[MAX_PATH] = { 0 };
                            strncpy(yy, g_ctxs[key].buf_recv, strlen(g_ctxs[key].buf_recv) - 1);

                            MultiByteToWideChar(CP_UTF8, 0, yy + 7, -1, wstr, MAX_PATH);
                            sprintf(g_ctxs[key].buf_send, "%s\n", owner(wstr).c_str());
                            printf("%s\n", owner(wstr).c_str());
                        }
                        else
                        {
                            sprintf(g_ctxs[key].buf_send, "You are BOBA. Please, input right command.\n");//

                        }
                        DWORD LenBuf = strlen(g_ctxs[key].buf_send);
                        CryptEncrypt(hKey, 0, TRUE, 0, (BYTE*)g_ctxs[key].buf_send, &LenBuf, 1000);
                        g_ctxs[key].buf_send[LenBuf] = '\0';
                        g_ctxs[key].sz_send_total = LenBuf + 1;
                        //g_ctxs[key].sz_send_total = strlen(g_ctxs[key].buf_send);
                        g_ctxs[key].sz_send = 0;
                        schedule_write(key);
                        read_after_recv(key);

                    }
                    else
                    {
                        // Иначе - ждем данные дальше  
                        schedule_read(key);
                    }
                }
                else if (&g_ctxs[key].overlap_send == lp_overlap)
                {
                    // Данные отправлены  
                    g_ctxs[key].sz_send += transferred;
                    if (g_ctxs[key].sz_send < g_ctxs[key].sz_send_total && transferred > 0)
                    {
                        // Если данные отправлены не полностью - продолжить отправлять  
                        schedule_write(key);
                    }
                    else
                    {
                        
                    }
                }
                else if (&g_ctxs[key].overlap_cancel == lp_overlap)
                {
                    // Все коммуникации завершены, сокет может быть закрыт  
                    closesocket(g_ctxs[key].socket);
                    memset(&g_ctxs[key], 0, sizeof(g_ctxs[key]));
                    printf(" connection %u closed\n", key);
                }
            }
        }
        else
        {
          
        }
    }
}
int main()

{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    io_serv();
    return 0;
}