#define _CRT_SECURE_NO_WARNINGS
#define MAX_DIRS    10 
#define MAX_FILES   255 
#define MAX_BUFFER  4096 

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



int check_zip();
int readfile() {
    ifstream in("config.txt"); // окрываем файл для чтения
    if (GetLastError() != 0) {
        return -1;
    }
    if (in.is_open())
    {
        getline(in, target_dir);
        //cout << target_dir << std::endl;

        getline(in, zip_dir);
        //cout << zip_dir << std::endl;
        int i = 0;
        masks.push_back("");
        while (std::getline(in, masks[i]))
        {
            //cout << masks[i] << std::endl;
            i++;
            masks.push_back("");
        }
        
    }
    in.close();     // закрываем файл
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

void removeFile( const char* zipPath) {
    int err;
    struct zip* zip = zip_open(zipPath, ZIP_CREATE | ZIP_CHECKCONS, &err);
    zip_close(zip);
    zip = zip_open(zip_dir.c_str(), ZIP_CREATE | ZIP_CHECKCONS, &err);
    if (!zip) {
        //fprintf(stderr, "addFileToZip : Error: can't open file %s with err %d\n", zip_dir.c_str(), err);
        zip_close(zip);
        return ;
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
    for (const auto& entry : directory_iterator(target))
    {
        if (is_directory(entry.path()))
        {
            addDirectoryToZip(entry.path().string().c_str(), entry.path().filename().string().c_str());
            add_my_dir(entry.path().string());
        }
        else {
            for (int i = 0; i < masks.size()-1; i++)
            {
                if (check((char*)entry.path().filename().string().c_str(), (char*)masks[i].c_str()))
                {
                    string buffer = entry.path().string();
                    int n = 0;
                    if ((n = buffer.find(target_dir)) != buffer.npos)
                        buffer.erase(n, target_dir.size());
                    buffer.erase(0,1);
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
                    size_t len = wcstombs(nullptr, wstring(fni->FileName, fni->FileNameLength / sizeof(WCHAR)).c_str(), 0) + 1;
                    char* buffer = new char[len]; 
                    wcstombs(buffer, wstring(fni->FileName, fni->FileNameLength / sizeof(WCHAR)).c_str(), len);
                    string str(buffer); 
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

                    delete[] buffer;
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

int check_zip() {
    struct zip* zip_file; // дескриптор zip файла
    int err; // переменая для возврата кодов ошибок
    int files_total; // количество файлов в архиве

    // открываем файл zip с именем переданным в качестве параметра
    zip_file = zip_open(zip_dir.c_str(), ZIP_CREATE , &err);
    if (!zip_file) {
        //fprintf(stderr, "Error: can't open file %s\n", zip_dir.c_str());
        return -1;
    };

    files_total = zip_get_num_files(zip_file); // количество файлов в архиве
    //printf("Files in ZIP: %d\n", files_total);
    int i = 0;
    struct zip_stat file_info; // информация о файле
    struct tm* file_time;
    for (i = 0; i < files_total; i++) {
        // получаем информацию о файле с номером i в структуру file_info
        zip_stat_index(zip_file, i, 0, &file_info);

        //printf("index=%ud ", file_info.index); // номер файла

        //printf("name=\"%s\" ", file_info.name); // имя файла

        //printf("size=%ud ", file_info.size); // размер файла

        file_time = localtime(&file_info.mtime); // дата модификации файла
        /*printf("date=\"%02d-%02d-%04d %02d:%02d\"",
            file_time->tm_mday, file_time->tm_mon + 1,
            file_time->tm_year + 1900, file_time->tm_hour,
            file_time->tm_min);

        printf("\n");*/
    };
    zip_close(zip_file);

    return 0;
}

int main_test_console()
{

    readfile(); //config file
    add_my_dir(target_dir); // init zip
    check_dir(); // monitoring target dir
    check_zip(); // What is in zip?
    return 0;
}