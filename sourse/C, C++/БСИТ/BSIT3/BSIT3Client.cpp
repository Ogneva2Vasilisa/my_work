#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <windows.h>    
#include <mswsock.h>  
#include <stdio.h>  
#include <sysinfoapi.h>
#include <AclAPI.h>
#include <sddl.h>
#include <iostream>
#include <wincrypt.h>

#pragma comment(lib, "ws2_32.lib")  
#pragma comment(lib, "mswsock.lib") 
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
struct client_ctx g_ctx;
int g_accepted_socket;
HANDLE g_io_port;

HCRYPTPROV hProv;
HCRYPTKEY hPubKey, hPrivKey, hKey, Pub;

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
    WSABUF buf;
    buf.buf = g_ctxs[idx].buf_send + g_ctxs[idx].sz_send;
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
        if (g_ctxs[idx].buf_recv[i] == '\t')
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

HCRYPTKEY KeyExchange(BYTE* pubKeyBlob, DWORD pubKeyLen) {
    for (int i = 0; i < pubKeyLen; i++) {
        g_ctxs[0].buf_send[i] = pubKeyBlob[i];
    }
    //g_ctxs[0].buf_send[pubKeyLen] = '\n';
    //sprintf(g_ctxs[0].buf_send, (const char*)pubKeyBlob);
    g_ctxs[0].sz_send_total = pubKeyLen;
    g_ctxs[0].sz_send = 0;
    schedule_write(0);
    schedule_read(0);

    while (1) {
        DWORD transferred;
        ULONG_PTR key;
        OVERLAPPED* lp_overlap;
        BOOL b = GetQueuedCompletionStatus(g_io_port, &transferred, &key, &lp_overlap, 1000);
        if (b) {
            if (&g_ctxs[key].overlap_recv == lp_overlap) {
                int len;
                g_ctxs[key].sz_recv += transferred;
                if (g_ctxs[key].sz_recv > 127) {
                    BYTE* sessionKey = new BYTE[500];
                    DWORD Datalen = 128;
                    for (int i = 0; i < 128; i++) {
                        sessionKey[i] = g_ctxs[key].buf_recv[i];
                    }
                    if (!CryptDecrypt(hKey, NULL, TRUE, NULL, sessionKey, &Datalen)) int err = GetLastError();
                    for (int i = 0; i < 128; i++) {
                        sessionKey[i + 72] = g_ctxs[key].buf_recv[i + 128];
                    }
                    printf("%s\n", sessionKey);
                    HCRYPTPROV sesProv;
                    HCRYPTKEY sesKey;
                    CryptAcquireContextA(&sesProv, NULL, NULL, PROV_RSA_AES, CRYPT_VERIFYCONTEXT);
                    CryptImportKey(sesProv, sessionKey, 144, 0, 0, &sesKey);
                    memset(g_ctxs[0].buf_recv, 0, sizeof(g_ctxs[0].buf_recv));
                    g_ctxs[0].sz_recv = 0;
                    return sesKey;
                    break;
                }
                else {
                    schedule_read(0);
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
            }
        }
    }
}


void io_cli(char* ip_address, unsigned short port)
{
    WSADATA wsa_data;
    if (!CryptAcquireContextA(&hProv, NULL, NULL, PROV_RSA_AES, CRYPT_VERIFYCONTEXT)) {
        std::cerr << "Ошибка инициализации криптографического провайдера." << std::endl;
        exit(1);
    }

    if (!CryptGenKey(hProv, AT_KEYEXCHANGE, CRYPT_EXPORTABLE, &hPrivKey)) {
        std::cerr << "Ошибка генерации закрытого ключа." << std::endl;
        CryptReleaseContext(hProv, 0);
        exit(1);
    }

    DWORD pubKeyLen = 0;
    if (!CryptExportKey(hPrivKey, 0, PUBLICKEYBLOB, 0, NULL, &pubKeyLen)) {
        std::cerr << "Ошибка получения длины публичного ключа." << std::endl;
        CryptReleaseContext(hProv, 0);
        exit(1);
    }

    BYTE* pubKeyBlob = new BYTE[pubKeyLen];
    //BYTE pubKeyBlob[149] = {0};
    if (!CryptExportKey(hPrivKey, 0, PUBLICKEYBLOB, 0, pubKeyBlob, &pubKeyLen)) {
        std::cerr << "Ошибка экспорта публичного ключа." << std::endl;
        delete[] pubKeyBlob;
        CryptReleaseContext(hProv, 0);
        exit(1);
    }

    DWORD privKeyLen = 0;
    if (!CryptExportKey(hPrivKey, 0, PRIVATEKEYBLOB, 0, NULL, &privKeyLen)) {
        std::cout << "Error in CryptExportKey (get length)";
        exit(1);
    }

    BYTE* privKeyBlob = new BYTE[privKeyLen];
    if (!CryptExportKey(hPrivKey, 0, PRIVATEKEYBLOB, 0, privKeyBlob, &privKeyLen)) {
        std::cout << "Error in CryptExportKey (export private key)";
        exit(1);
    }

    if (!CryptImportKey(hProv, privKeyBlob, privKeyLen, 0, 0, &hKey)) {
        std::cout << "Error in CryptImportKey";
        exit(1);
    }

    if (!CryptImportKey(hProv, pubKeyBlob, pubKeyLen, 0, 0, &Pub)) {
        std::cout << "Error in CryptImportKey";
        exit(1);
    }

    std::string dataToEncrypt = "Hello";
    DWORD dataLen = dataToEncrypt.size() + 1; // Включаем нуль-терминатор

    // Шифрование с использованием публичного ключа
    BYTE* encryptedData = new BYTE[1000];
    memcpy(encryptedData, dataToEncrypt.c_str(), dataLen);

    DWORD encryptedDataLen = dataLen;
    if (!CryptEncrypt(Pub, 0, TRUE, 0, encryptedData, &encryptedDataLen, 1000)) {
        std::cout << "Error in CryptEncrypt";
    }

    std::cout << "Encrypted data size: " << encryptedDataLen << std::endl;

    DWORD decryptedDataLen = encryptedDataLen;
    if (!CryptDecrypt(hKey, 0, TRUE, 0, encryptedData, &decryptedDataLen)) {
        std::cout << "Error in CryptDecrypt";
    }

    std::cout << "Decrypted data: " << (char*)encryptedData << std::endl;

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
    // Создание сокета  
    SOCKET s = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);

    g_io_port = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
    if (NULL == g_io_port)
    {
        printf("CreateIoCompletionPort error: %x\n", GetLastError());
        return;
    }

    memset(g_ctxs, 0, sizeof(g_ctxs));
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(ip_address);
    if (WSAConnect(s, (struct sockaddr*)&addr, sizeof(addr), NULL, NULL, NULL, NULL) != 0) {
        printf("Connect error\n");
        exit(0);
    }

    if (NULL == CreateIoCompletionPort((HANDLE)s, g_io_port, 0, 0))
    {
        printf("CreateIoCompletionPort error: %x\n", GetLastError());
        return;
    }

    printf("%s:%d connected\n", ip_address, port);

    std::string command;
    char* buffer = new char[10000];
    //fgets(buffer, 1000, stdin);
    //command = buffer;
    g_ctxs[0].socket = s;
    //command.append("\n");
    HCRYPTKEY sesKey = KeyExchange(pubKeyBlob, pubKeyLen);
    //sprintf(g_ctxs[0].buf_send, command.c_str());
    //g_ctxs[0].sz_send_total = strlen(g_ctxs[0].buf_send);
    //g_ctxs[0].sz_send = 0;

    //schedule_write(0);

    //schedule_read(0);

    while (1) {
        fgets(buffer, 1000, stdin);
        command = buffer;
        //command.append("\n");
        sprintf(g_ctxs[0].buf_send, command.c_str());
        DWORD LenBuf = strlen(g_ctxs[0].buf_send);
        CryptEncrypt(sesKey, 0, TRUE, 0, (BYTE*)g_ctxs[0].buf_send, &LenBuf, 1000);
        g_ctxs[0].buf_send[LenBuf] = '\0';
        g_ctxs[0].sz_send_total = LenBuf + 1;
        g_ctxs[0].sz_send = 0;

        schedule_write(0);

        if (!command.compare("9\n")) {
            break;
        }

        schedule_read(0);

        while (1) {
            DWORD transferred;
            ULONG_PTR key;
            OVERLAPPED* lp_overlap;
            BOOL b = GetQueuedCompletionStatus(g_io_port, &transferred, &key, &lp_overlap, 1000);
            if (b) {
                if (&g_ctxs[key].overlap_recv == lp_overlap) {
                    int len;
                    g_ctxs[key].sz_recv += transferred;
                    //if (is_string_received(key, &len)) {
                    if (g_ctxs[key].buf_recv[g_ctxs[key].sz_recv - 1] == 0) {
                        LenBuf = g_ctxs[key].sz_recv - 1;
                        CryptDecrypt(sesKey, 0, TRUE, 0, (BYTE*)g_ctxs[key].buf_recv, &LenBuf);
                        g_ctxs[0].buf_recv[LenBuf] = 0;
                        printf("%s\n", g_ctxs[0].buf_recv);
                        memset(g_ctxs[0].buf_recv, 0, sizeof(g_ctxs[0].buf_recv));
                        g_ctxs[0].sz_recv = 0;
                        break;
                    }
                    else {
                        schedule_read(0);
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
                }
            }
        }
    }
    closesocket(s);
}

int main(int argc, char* argv[])

{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    char ip_address[16] = { 0 };
    unsigned short port = 0;
    //char ip_address[13] = "192.168.78.131";
    //unsigned short port = 9000;
    argc = 3;
    argv[1] = (char*)"192.168.78.1";
    argv[2] = (char*)"9000";
    if (argc == 3) {
        strncpy(ip_address, argv[1], strlen(argv[1]));
        port = (unsigned short)strtol(argv[2], 0, 10);
       

    }
    else {
        printf("Введите IP-адрес и порт через пробел\n");
        return -1;
    }
    io_cli(ip_address, port);
    return 0;
}