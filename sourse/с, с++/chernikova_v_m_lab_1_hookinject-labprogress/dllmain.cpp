
#include "pch.h"
#include <tlhelp32.h>
#include <tchar.h>
#include <stdio.h>
#include <windows.h>
#include <malloc.h>
#include <cstdio>
#include <stdlib.h>
#include <time.h>
#include <iostream>

#pragma once

#ifdef HOOKLIBRARY_EXPORTS
#define HOOKLIBRARY_API __declspec(dllexport)
#else
#define HOOKLIBRARY_API __declspec(dllimport)
#endif


HANDLE hPipe;
DWORD dwWritten;
char buffer[256];
DWORD dwRead;


void* origfunc = nullptr;


extern "C" long long hookfunc(unsigned int num);
extern "C" void perehook(unsigned int num);
int hook;

extern "C" void MultiplyBy10(void);
extern "C" void Shadowfile(void);
extern "C" int hook;
char* hidefile;

void print_my(char* string_my) {
    if (hPipe != INVALID_HANDLE_VALUE)
    {
        WriteFile(hPipe,
            string_my,
            (DWORD)strlen(string_my),   // = length of string + terminating '\0' !!!
            &dwWritten,
            NULL);
    };
}
//----------------------------------------------------------------------------------------------------------------------------------------
class PatchClass {
    unsigned char** savebytes;
    void** address;
    unsigned int lenhooks;

public:
    static PatchClass* instance;
    long long origfuncs[100];
    char** name;

    PatchClass() {
        this->savebytes = (unsigned char**)calloc(100, sizeof(char*));
        this->name = (char**)calloc(100, sizeof(char*));
        this->lenhooks = 0;
        this->address = (void**)calloc(100, sizeof(void*));
    }

    void patch(void* addresshook, char* name) {
        this->name[this->lenhooks] = (char*)calloc(100, sizeof(char));
        this->savebytes[this->lenhooks] = (unsigned char*)calloc(20, sizeof(char));
        strcpy(this->name[this->lenhooks], name);
        this->address[this->lenhooks] = GetProcAddress(GetModuleHandleW(TEXT("kernel32.dll")), name);
        //origfunc = GetProcAddress(GetModuleHandleW(TEXT("kernel32.dll")), name);
        ReadProcessMemory(GetCurrentProcess(), this->address[this->lenhooks], this->savebytes[this->lenhooks], 15, NULL);
        long long origaddr = 0;
        memcpy(&origaddr, &this->savebytes[this->lenhooks][2], 4);
        origaddr += (long long)this->address[this->lenhooks];
        origaddr += 6;
        long long hadrr = 0;
        ReadProcessMemory(GetCurrentProcess(), (LPCVOID)origaddr, &hadrr, 8, NULL);
        unsigned char Patchbytes[15] = { 0x49, 0xBA, 0xB0, 0x1B, 0xDF, 0xFC, 0xF7, 0x7F, 0x00, 0x00, 0x6A, 0x01, 0x41, 0xff,  0xe2 };
        memcpy(Patchbytes + 2, &addresshook, 6);
        //memcpy(Patchbytes + 2, &hadrr, 6);
        //origfunc = (void*)hadrr;
        origfuncs[this->lenhooks] = hadrr;
        *(Patchbytes + 11) = this->lenhooks;
        DWORD OldProtect = 0;
        VirtualAlloc(this->address[this->lenhooks], 16, MEM_COMMIT | MEM_RESERVE, NULL);
        VirtualProtect(this->address[this->lenhooks], 16, PAGE_EXECUTE_READWRITE, &OldProtect);
        WriteProcessMemory(GetCurrentProcess(), this->address[this->lenhooks], Patchbytes, 15, NULL);
        this->lenhooks++;
        printf("patched\n");
    }

    void patch(void* addresshook, unsigned int num) {
        this->address[num] = GetProcAddress(GetModuleHandleW(TEXT("kernel32.dll")), this->name[num]);
        origfunc = GetProcAddress(GetModuleHandleW(TEXT("kernel32.dll")), this->name[num]);
        ReadProcessMemory(GetCurrentProcess(), this->address[num], this->savebytes[num], 20, NULL);
        unsigned char Patchbytes[20] = { 0x49, 0xBA, 0xB0, 0x1B, 0xDF, 0xFC, 0xF7, 0x7F, 0x00, 0x00, 0x6A, 0x01, 0x41, 0xff,  0xe2 };
        memcpy(Patchbytes + 2, &addresshook, 6);
        *(Patchbytes + 11) = num;
        DWORD OldProtect = 0;
        VirtualAlloc(this->address[num], 16, MEM_COMMIT | MEM_RESERVE, NULL);
        VirtualProtect(this->address[num], 16, PAGE_EXECUTE_READWRITE, &OldProtect);
        WriteProcessMemory(GetCurrentProcess(), this->address[num], Patchbytes, 15, NULL);
        printf("patched\n");
    }

    void unHook(unsigned int num) {
        WriteProcessMemory(GetCurrentProcess(), this->address[num], this->savebytes[num], 20, NULL);
        printf("unhook\n");
        print_my((char *)"unhook\n");
    }
    static PatchClass* get_instance();
};

PatchClass* PatchClass::get_instance() {
    if (instance == nullptr) {
        instance = new PatchClass();
    }
    return instance;
}

PatchClass* PatchClass::instance = nullptr;

PatchClass* p;

//------------------------------------------------------------------------------------------------------------------------------------------------

//сделать сигнатуры, проверяет что на ввходе и возвращать ошибку, если файлик надо скрыть

//extern "C" void* func1(TCHAR* u) //перехватчик
//extern "C" HANDLE func1(LPCWSTR arg1, DWORD argv2, DWORD argv3, LPSECURITY_ATTRIBUTES argv4, DWORD argv5, DWORD argv6, HANDLE argv7)
//{
//    time_t curtime;     time(&curtime);
//    bool i = wcscmp(arg1, hp.hide_filename);
//
//    if (i == 0) {
//        //_tprintf(L"%s", hp.hide_filename);
//        //printf("%s %s", hp.name_func, ctime(&curtime));
//        memset(straa, 0, sizeof(straa));
//        strcpy(straa, "     ");
//        sprintf(straa, "%ws", hp.hide_filename);
//        wcstombs(straa, hp.hide_filename, wcslen(hp.hide_filename) + 1);
//        strcat(straa, "  ");
//        strcat(straa, ctime(&curtime));
//        print_my(straa);
//        SetLastError(ERROR_FILE_NOT_FOUND);
//        return INVALID_HANDLE_VALUE;
//    }
//    else {
//        hp.unpatch(pGNSI);
//        HANDLE result = ((HANDLE(WINAPI*)(LPCWSTR, DWORD, DWORD, LPSECURITY_ATTRIBUTES, DWORD, DWORD, HANDLE))pGNSI)(arg1, argv2, argv3, argv4, argv5, argv6, argv7);
//        return result;
//    }
//}


extern "C" long long hookfunc(unsigned int num) {
    printf("Function hooked\n");
    PatchClass* p = PatchClass::get_instance();
    //p->unHook(num);
    std::time_t result = std::time(nullptr);
    char* time = std::asctime(std::localtime(&result));
    
    print_my(p->name[num]);
    print_my(time);

    //printf("%s\n", arg1);
    hook = 1;
    //origfunc = (void*)p->origfuncs[num];
    return p->origfuncs[num];
}

extern "C" long long shadow(char* name) {
    //print_my((char*)"shadow ");
    //print_my(name);
    wchar_t wideProcessName[400];
    std::mbstowcs(wideProcessName, hidefile, 400);
    wchar_t newname[400];
    int j = 0;
    for (int i = 0; i < 200; i++) {
        if (name[i] != 0) {
            newname[j] = name[i];
            j++;
        }
        else {
            if (i < 199 && name[i + 1] == 0) {
                newname[j] = 0;
                break;
            }
        }
    }
    std::wcout << newname << std::endl;
    std::wcout << wideProcessName << std::endl;
    if (!wcscmp(newname, wideProcessName) || !strcmp(name, hidefile)) {
        return 0;
    }
    else {
        return 1;
    }
}

void perehook(unsigned int num) {
    PatchClass* p = PatchClass::get_instance();
    p->patch(MultiplyBy10, num);
}


//--------------------------------------------------------------------------------------------------------------------------------------------

// Функция для обработки команд
void ProcessCommand(char* command) {
    
}


BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        p = PatchClass::get_instance();
        printf("DLL_PROCESS_ATTACH\n");
        hPipe = CreateFile(TEXT("\\\\.\\pipe\\Pipe"),
            GENERIC_READ | GENERIC_WRITE,
            0,
            NULL,
            OPEN_EXISTING,
            0,
            NULL);
        ReadFile(hPipe, buffer, sizeof(buffer) - 1, &dwRead, NULL);
        buffer[dwRead] = '\0';
        printf("pipe_buffer=%s\n", buffer);
        if (strncmp(buffer, "func:", 5) == 0) {
            char* funcName = buffer + 5;
            printf("%s\n", funcName);
            p->patch(MultiplyBy10, funcName);
        }
        else if (strncmp(buffer, "hide:", 5) == 0) {
            char* fileName = buffer + 5;
            hidefile = fileName;
            print_my((char*)"hide func from dll\n");
            printf("!!!!!!ff %s\n", fileName);
            //print_my(hidefile);
            print_my(fileName);
            if (strcmp(hidefile, "nohide")) {
                printf("hide\n");
                p->patch(Shadowfile, (char*)"CreateFileW");
                p->patch(Shadowfile, (char*)"CreateFileA");
                p->patch(Shadowfile, (char*)"FindFirstFileA");
                p->patch(Shadowfile, (char*)"FindFirstFileW");
                p->patch(Shadowfile, (char*)"FindFirstFileExA");
                p->patch(Shadowfile, (char*)"FindFirstFileExW");
                //print_my((char*)"   Success patch\n");
            }
            print_my((char*)" Success unpatch\n");
            hook = 0;
        }
        break;

    case DLL_THREAD_ATTACH:
        printf("DLL_THREAD_ATTACH\n");
        //printf("pipe_buffer=%s\n", buffer);
        break;
    case DLL_THREAD_DETACH:
        //printf("DLL_THREAD_DETACH\n");
        break;

    case DLL_PROCESS_DETACH:
        printf("DLL_PROCESS_DETACH\n");
        break;
    }
    return TRUE;
}

