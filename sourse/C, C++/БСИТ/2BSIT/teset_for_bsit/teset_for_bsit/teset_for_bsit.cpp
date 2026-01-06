#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <Windows.h>
#include <string>// для std::getline
#include <vector>
#include <zip.h>
#include <time.h>
#include <filesystem>


using namespace std;

namespace fs = std::filesystem;
using namespace fs;
string zip_dir;
string target_dir;
vector <string> masks;

#define MAX_DIRS    10 
#define MAX_FILES   255 
#define MAX_BUFFER  4096 

int readfile() {
    FILE* in = fopen("C:\\Users\\vas\\source\\repos\\2BSIT\\x64\\Release\\config.txt","r"); // окрываем файл для чтения
    if (GetLastError() != 0) {
        return -1;
    }
    char buf[512] = { 0 };
    if (!feof(in))
    {
        fgets(buf, 1024, in);
        buf[strlen(buf) - 1] = 0;
        target_dir = string(buf);
        fgets(buf, 1024, in);
        buf[strlen(buf) - 1] = 0;
        zip_dir = string(buf);
        int i = 0;
        while (!feof(in))
        {
            fgets(buf, 1024, in);
            buf[strlen(buf) - 1] = 0;
            masks.push_back(string(buf));
            i++;
        }
    }
    fclose(in);     // закрываем файл
    return 0;
}


bool check(char* s, char* p)// название файла, маска
{
    char* rs = 0, * rp = 0;
    while (1)
        if (*p == '*')
            rs = s, rp = ++p;
        else if (!*s)
            return !*p;
        else if (*s == *p || *p == '?')
            ++s, ++p;
        else if (rs)
            s = ++rs, p = rp;
        else
            return false;
}

void removeFile(const char* zipPath) {
    int err;
    struct zip* zip = zip_open(zipPath, ZIP_CREATE | ZIP_CHECKCONS, &err);
    zip_close(zip);
    zip = zip_open(zip_dir.c_str(), ZIP_CREATE | ZIP_CHECKCONS, &err);
    if (!zip) {
        //fprintf(stderr, "addFileToZip : Error: can't open file %s with err %d\n", zip_dir.c_str(), err);
        zip_close(zip);
        return;
    };
    zip_int64_t index = zip_name_locate(zip, zipPath, 0);
    if (index >= 0) {
        zip_delete(zip, index);
    }
    else {
        //std::cerr << "File not found in the zip archive: " << zipPath << std::endl;
    }
    zip_close(zip);
}

int addFileToZip(const char* filePath, const char* zipPath) {
    int err;
    struct zip* zip = zip_open(zipPath, ZIP_CREATE | ZIP_CHECKCONS, &err);
    zip_close(zip);
    zip = zip_open(zip_dir.c_str(), ZIP_CREATE | ZIP_CHECKCONS, &err);
    if (!zip) {
        //fprintf(stderr, "addFileToZip : Error: can't open file %s with err %d\n", zip_dir.c_str(), err);
        zip_close(zip);
        return -1;
    };
    zip_source_t* source = zip_source_file(zip, filePath, 0, 0);
    if (source != nullptr) {
        zip_file_add(zip, zipPath, source, ZIP_FL_OVERWRITE);
    }
    else {
        //cerr << "Error adding file to zip: " << filePath << endl;
    }
    zip_close(zip);
    return 0;
}

int addDirectoryToZip(const char* directoryPath, const char* zipPath) {
    int err;
    struct zip* zip = zip_open(zip_dir.c_str(), ZIP_CREATE | ZIP_CHECKCONS, &err);
    zip_close(zip);
    zip = zip_open(zip_dir.c_str(), ZIP_CREATE | ZIP_CHECKCONS, &err);
    if (!zip) {
        //fprintf(stderr, "addDirectoryToZip : Error: can't open file %s with err %d\n", zip_dir.c_str(), err);
        zip_close(zip);
        return -1;
    };
    zip_source_t* source = zip_source_file(zip, directoryPath, ZIP_FL_RECOMPRESS, 4096);
    if (source != nullptr) {
        zip_dir_add(zip, zipPath, ZIP_FL_OVERWRITE);
    }
    else {
        //cerr << "Error adding directory to zip: " << directoryPath << endl;
    }
    zip_close(zip);
    return 0;
}

int add_my_dir(string target) {
    //cout << target << endl;
    for (const auto& entry : directory_iterator(target))
    {
        if (is_directory(entry.path()))
        {
            string buffer = entry.path().string();
            int n = 0;
            if ((n = buffer.find(target_dir)) != buffer.npos)
                buffer.erase(n, target_dir.size());
            buffer.erase(0, 1);

            addDirectoryToZip(entry.path().string().c_str(), buffer.c_str());
            add_my_dir(entry.path().string());
        }
        else {
            for (int i = 0; i < masks.size(); i++)
            {
                if (check((char*)entry.path().filename().string().c_str(), (char*)masks[i].c_str()))
                {
                    string buffer = entry.path().string();
                    int n = 0;
                    if ((n = buffer.find(target_dir)) != buffer.npos)
                        buffer.erase(n, target_dir.size());
                    buffer.erase(0, 1);
                    addFileToZip(entry.path().string().c_str(), buffer.c_str());
                }
            }
        }
    }
    return 0;
}

int check_dir() {
    HANDLE hDir = CreateFileA(
        (LPCSTR)target_dir.c_str(),            // Directory to monitor
        FILE_LIST_DIRECTORY,          // Access (read/write)
        FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, // Share mode
        nullptr,                      // Security attributes
        OPEN_EXISTING,                // Creation disposition
        FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED, // Flags
        nullptr                       // Template file
    );

    if (hDir == INVALID_HANDLE_VALUE) {
        //cout <<"Failed to get handle to directory: " << GetLastError() << std::endl;
        return -1;
    }

    char buffer[1024];
    DWORD bytesReturned;
    OVERLAPPED overlapped = { 0 };


    while (true) {
        // Start monitoring
        if (ReadDirectoryChangesW(
            hDir,                // Handle to directory
            buffer,              // Buffer to store results
            sizeof(buffer),      // Size of buffer
            TRUE,                // Monitor subdirectories
            FILE_NOTIFY_CHANGE_FILE_NAME |
            FILE_NOTIFY_CHANGE_DIR_NAME |
            FILE_NOTIFY_CHANGE_ATTRIBUTES |
            FILE_NOTIFY_CHANGE_SIZE |
            FILE_NOTIFY_CHANGE_LAST_WRITE |
            FILE_NOTIFY_CHANGE_LAST_ACCESS |
            FILE_NOTIFY_CHANGE_CREATION |
            FILE_NOTIFY_CHANGE_SECURITY, // Notify on these changes
            &bytesReturned,      // Number of bytes returned
            &overlapped,         // Overlapped structure
            nullptr              // Completion routine (NULL for blocking)
        )) {
            // Wait for the changes
            overlapped.hEvent = hDir;
            DWORD dwWaitStatus = WaitForSingleObject(overlapped.hEvent, INFINITE);
            overlapped = { 0 };

            struct zip* zip_file; // дескриптор zip файла
            int err; // переменая для возврата кодов ошибок

            if (dwWaitStatus == WAIT_OBJECT_0) {
                FILE_NOTIFY_INFORMATION* fni = reinterpret_cast<FILE_NOTIFY_INFORMATION*>(buffer);

                do {
                    //cout << "Change detected: " << endl;

                    //------------------------------------------------------------------------       
                    string buffer1;
                    buffer1.append(target_dir.c_str());
                    buffer1.append("\\");

                    char buffer [512];
                    //cout << fni->FileNameLength << ' ' << fni->FileNameLength / sizeof(WCHAR) << endl;
                    WideCharToMultiByte(CP_UTF8, 0, fni->FileName, fni->FileNameLength, buffer, fni->FileNameLength / sizeof(WCHAR), NULL, 0);
                    buffer[fni->FileNameLength / sizeof(WCHAR)] = 0;
                    string str = buffer;
                    //cout << buffer << ' ' << str<< endl;
                    bool flag = FALSE;
                    buffer1.append(str);
                    if (buffer1.find(".") != string::npos)
                        switch (fni->Action) {
                        case FILE_ACTION_ADDED:
                            //cout << "File added: " << endl;
                            //wcout << wstring(fni->FileName, fni->FileNameLength / sizeof(WCHAR)) << endl;
                            flag = TRUE;
                            break;
                        case FILE_ACTION_MODIFIED:
                            //cout << "File modified: " << endl; 
                            //wcout << wstring(fni->FileName, fni->FileNameLength / sizeof(WCHAR)) << endl;
                            flag = TRUE;
                            break;
                        }

                    if (flag)
                    {
                        addFileToZip(buffer1.c_str(), buffer);
                        //check_zip();   //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
                    }

                    
                    if (fni->NextEntryOffset == 0) break;
                    fni = reinterpret_cast<FILE_NOTIFY_INFORMATION*>(reinterpret_cast<BYTE*>(fni) + fni->NextEntryOffset);
                } while (true);
            }
            else {
                //cout <<"Wait failed: " << GetLastError() << endl;
                break;
            }
        }
        else {
            //cout <<"ReadDirectoryChangesW failed: " << GetLastError() << endl;
            break;
        }
    }
    CloseHandle(hDir);

}

int main()
{

    readfile(); //config file
    //cout << "sfg" << endl;
    add_my_dir(target_dir); // init zip
    check_dir(); // monitoring target dir
    //check_zip(); // What is in zip?
    return 0;
}