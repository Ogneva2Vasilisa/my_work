#include <windows.h>
#include <tlhelp32.h>
#include <tchar.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


typedef HMODULE(WINAPI* LoadLibraryA_t)(LPCSTR);

// Структура для хранения параметров
struct ProgramParams {
    DWORD pid;
    char processName[256];
    char functionName[256];
    char hideFileName[256];
    bool usePID;
    bool useProcessName;
    bool useFunction;
    bool useHideFile;
};

// Функция для получения PID процесса по имени
DWORD GetProcessIdByName(const char* processName) {
    DWORD processId = 0;
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot != INVALID_HANDLE_VALUE) {
        PROCESSENTRY32W processEntry;
        processEntry.dwSize = sizeof(processEntry);

        if (Process32FirstW(snapshot, &processEntry)) {
            do {
                wchar_t wProcessName[256];
                MultiByteToWideChar(CP_UTF8, 0, processName, -1, wProcessName, 256);
                if (_wcsicmp(processEntry.szExeFile, wProcessName) == 0) {
                    processId = processEntry.th32ProcessID;
                    break;
                }
            } while (Process32NextW(snapshot, &processEntry));
        }
        CloseHandle(snapshot);
    }
    return processId;
}

// Функция для парсинга аргументов командной строки
ProgramParams ParseCommandLine(int argc, char* argv[]) {
    ProgramParams params = {0};
    
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-pid") == 0 && i + 1 < argc) {
            params.pid = atoi(argv[i + 1]);
            params.usePID = true;
            i++;
        }
        else if (strcmp(argv[i], "-name") == 0 && i + 1 < argc) {
            strcpy_s(params.processName, argv[i + 1]);
            params.useProcessName = true;
            i++;
        }
        else if (strcmp(argv[i], "-func") == 0 && i + 1 < argc) {
            strcpy_s(params.functionName, argv[i + 1]);
            params.useFunction = true;
            i++;
        }
        else if (strcmp(argv[i], "-hide") == 0 && i + 1 < argc) {
            strcpy_s(params.hideFileName, argv[i + 1]);
            params.useHideFile = true;
            i++;
        }
    }
    return params;
}


int main(int argc, char* argv[])
{
    // Парсим аргументы командной строки
    ProgramParams params = ParseCommandLine(argc, argv);
    
    // Получаем PID процесса
    DWORD targetPID = params.usePID ? params.pid : GetProcessIdByName(params.processName);
    if (targetPID == 0) {
        printf("Error finding proccess\n");
        return 1;
    }
    LoadLibraryA_t pLoadLibrary = (LoadLibraryA_t)GetProcAddress(GetModuleHandle(TEXT("kernel32.dll")), "LoadLibraryA");
    
    printf("pid = %d\n", params.pid);
    HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, targetPID);
    if (hProc == NULL) {
        printf("Error opening proccess\n");
        return 1;
    }

    LPVOID pBase = VirtualAllocEx(hProc, 0, 50, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    BYTE jump[55] = "E:\\_весна 2025!!!!!\\trspo\\Dll1\\x64\\Debug\\Dll1.dll";
    WriteProcessMemory(hProc, pBase, jump, 50, NULL);
    printf("Dll succsess loaded\n");

    HANDLE hPipe;
    char buffer[256];
    DWORD dwRead;
    hPipe = CreateNamedPipe(TEXT("\\\\.\\pipe\\Pipe"),
        PIPE_ACCESS_DUPLEX,
        PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT,   
        1,
        1024 * 16,
        1024 * 16,
        NMPWAIT_USE_DEFAULT_WAIT,
        NULL);

    CreateRemoteThread(hProc, NULL, 0, (LPTHREAD_START_ROUTINE)pLoadLibrary, pBase, NULL, NULL); 

    // Отправляем параметры в DLL через пайп
    char command[256];
    if (params.useFunction) {
        printf("func:%s\n", params.functionName);
        sprintf_s(command, "func:%s", params.functionName);
    }
    else if (params.useHideFile) {
        printf("hide:%s\n", params.hideFileName);
        sprintf_s(command, "hide:%s", params.hideFileName);
    }

    while (hPipe != INVALID_HANDLE_VALUE)
    {
        if (ConnectNamedPipe(hPipe, NULL) != FALSE)   
        {
            DWORD dwWritten;
            WriteFile(hPipe,
                command,
                strlen(command),   
                &dwWritten,
                NULL);

            while (ReadFile(hPipe, buffer, sizeof(buffer) - 1, &dwRead, NULL) != FALSE)
            {
                buffer[dwRead] = '\0';
                printf("client: %s\n", buffer);

                //// Проверяем, содержит ли сообщение "Success unpatch"
                //if (strstr(buffer, "Success unpatch") != NULL) {
                //    printf("Detected unpatch, restarting DLL...\n");
                //    // Создаем новый поток для загрузки DLL
                //    CreateRemoteThread(hProc, NULL, 0, (LPTHREAD_START_ROUTINE)pLoadLibrary, pBase, NULL, NULL);
                //    printf("Dll loaded\n");
                //    Sleep(100);
                //}
            }
        }

        DisconnectNamedPipe(hPipe);
    }

    CloseHandle(hProc);
    return 0;
}

