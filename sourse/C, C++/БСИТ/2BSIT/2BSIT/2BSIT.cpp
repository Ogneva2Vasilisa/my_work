#include <windows.h>
#include <tchar.h>
#include <iostream>
#include <string.h>

using namespace std;

#define servicePath _T("C:\\Users\\vas\\source\\repos\\2BSIT\\x64\\Release\\MyServ.exe") 
#define serviceName _T("MyServ")

SERVICE_STATUS serviceStatus;
SERVICE_STATUS_HANDLE serviceStatusHandle;


//-------------------------------
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
namespace fs = std::filesystem;
using namespace fs;
string zip_dir;
string target_dir;
vector <string> masks;

int addLogMessage(char* str);

int readfile() {
    FILE* in;
    fopen_s(&in, "C:\\Users\\vas\\source\\repos\\2BSIT\\x64\\Release\\config.txt", "r"); // окрываем файл для чтения
    if (GetLastError() != 0) {
        return -1;
    }
    char buf[512] = { 0 };
    if (!feof(in))
    {
        fgets(buf, 1024, in);
        buf[strlen(buf) - 1] = 0;
        addLogMessage(buf);
        target_dir = string(buf);
        fgets(buf, 1024, in);
        buf[strlen(buf) - 1] = 0;
        addLogMessage(buf);
        zip_dir = string(buf);
        int i = 0;
        while (!feof(in))
        {
            fgets(buf, 1024, in);
            buf[strlen(buf) - 1] = 0;
            addLogMessage(buf);
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
    addLogMessage((char*)"addFileToZip");
    int err;
    struct zip* zip = zip_open(zipPath, ZIP_CREATE | ZIP_CHECKCONS, &err);
    zip_close(zip);
    zip = zip_open(zip_dir.c_str(), ZIP_CREATE | ZIP_CHECKCONS, &err);
    if (!zip) {
        addLogMessage((char*)"addFileToZip : Error: can't open file");
        //fprintf(stderr, "addFileToZip : Error: can't open file %s with err %d\n", zip_dir.c_str(), err);
        zip_close(zip);
        return -1;
    };
    zip_source_t* source = zip_source_file(zip, filePath, 0, 0);
    if (source != nullptr) {
        zip_file_add(zip, zipPath, source, ZIP_FL_OVERWRITE);
    }
    else {
        addLogMessage((char*)"addFileToZip : Error adding file to zip");
        //cerr << "Error adding file to zip: " << filePath << endl;
    }
    zip_close(zip);
    return 0;
}

int addDirectoryToZip(const char* directoryPath, const char* zipPath) {
    addLogMessage((char*)"addDirectoryToZip");
    int err;
    struct zip* zip = zip_open(zip_dir.c_str(), ZIP_CREATE | ZIP_CHECKCONS, &err);
    zip_close(zip);
    zip = zip_open(zip_dir.c_str(), ZIP_CREATE | ZIP_CHECKCONS, &err);
    if (!zip) {
        addLogMessage((char*)"addDirectoryToZip : Error: can't open file");
        //fprintf(stderr, "addDirectoryToZip : Error: can't open file %s with err %d\n", zip_dir.c_str(), err);
        zip_close(zip);
        return -1;
    };
    zip_source_t* source = zip_source_file(zip, directoryPath, ZIP_FL_RECOMPRESS, 4096);
    if (source != nullptr) {
        zip_dir_add(zip, zipPath, ZIP_FL_OVERWRITE);
    }
    else {
        addLogMessage((char*)"addDirectoryToZip : Error adding directory to zip");
        //cerr << "Error adding directory to zip: " << directoryPath << endl;
    }
    zip_close(zip);
    return 0;
}

int add_my_dir(string target) {
    addLogMessage((char*)"TARGET");
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
                    
                    char buffer[512];
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

//-------------------------------

int addLogMessage(char* str) {
    errno_t err;
    FILE* log;

    if ((err = fopen_s(&log, "C:\\Users\\vas\\source\\repos\\2BSIT\\x64\\Release\\MyLog.txt", "a+")) != 0) {
        return -1;
    }

    fprintf(log, "[code: 0] %s\n", str);
    fclose(log);
    return 0;
}

void ControlHandler(DWORD request) { 
  switch(request) 
  { 
    case SERVICE_CONTROL_STOP: 
      addLogMessage((char*)"Stopped.");

      serviceStatus.dwWin32ExitCode = 0; 
      serviceStatus.dwCurrentState = SERVICE_STOPPED; 
      SetServiceStatus (serviceStatusHandle, &serviceStatus);
      return; 

    case SERVICE_CONTROL_SHUTDOWN: 
      addLogMessage((char*)"Shutdown.");

      serviceStatus.dwWin32ExitCode = 0; 
      serviceStatus.dwCurrentState = SERVICE_STOPPED; 
      SetServiceStatus (serviceStatusHandle, &serviceStatus);
      return; 
    
    default:
      break;
  } 

  SetServiceStatus (serviceStatusHandle, &serviceStatus);

  return; 
} 



void ServiceMain(int argc, char** argv) {
    int error;
    int i = 0;

    serviceStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
    serviceStatus.dwCurrentState = SERVICE_START_PENDING;
    serviceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN;
    serviceStatus.dwWin32ExitCode = 0;
    serviceStatus.dwServiceSpecificExitCode = 0;
    serviceStatus.dwCheckPoint = 0;
    serviceStatus.dwWaitHint = 0;

    serviceStatusHandle = RegisterServiceCtrlHandler(serviceName, (LPHANDLER_FUNCTION)ControlHandler);
    if (serviceStatusHandle == (SERVICE_STATUS_HANDLE)0) {
        return;
    }

    /*error = InitService();
    if (error) {
        serviceStatus.dwCurrentState = SERVICE_STOPPED;
        serviceStatus.dwWin32ExitCode = -1;
        SetServiceStatus(serviceStatusHandle, &serviceStatus);
        return;
    }*/

    serviceStatus.dwCurrentState = SERVICE_RUNNING;
    SetServiceStatus(serviceStatusHandle, &serviceStatus);

    while (serviceStatus.dwCurrentState == SERVICE_RUNNING)
    {
        char buffer[255];
        sprintf_s(buffer, "%u", i);
        int result = addLogMessage(buffer);
        
        i++;
        readfile(); //config file
        addLogMessage((char*)"Readed config file");
        addLogMessage((char*)target_dir.c_str());
        addLogMessage((char*)zip_dir.c_str());
        add_my_dir(target_dir); // init zip
        addLogMessage((char*)"Init zip complite ");
        check_dir(); // monitoring target dir
    }

    return;
}


int InstallService() {
    SC_HANDLE hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_CREATE_SERVICE);
    if (!hSCManager) {
        addLogMessage((char*)"Error: Can't open Service Control Manager");
        return -1;
    }

    SC_HANDLE hService = CreateService(
        hSCManager,
        serviceName,
        serviceName,
        SERVICE_ALL_ACCESS,
        SERVICE_WIN32_OWN_PROCESS,
        SERVICE_DEMAND_START,
        SERVICE_ERROR_NORMAL,
        servicePath,
        NULL, NULL, NULL, NULL, NULL
    );

    if (!hService) {
        int err = GetLastError();
        switch (err) {
        case ERROR_ACCESS_DENIED:
            addLogMessage((char*)"Error: ERROR_ACCESS_DENIED");
            break;
        case ERROR_CIRCULAR_DEPENDENCY:
            addLogMessage((char*)"Error: ERROR_CIRCULAR_DEPENDENCY");
            break;
        case ERROR_DUPLICATE_SERVICE_NAME:
            addLogMessage((char*)"Error: ERROR_DUPLICATE_SERVICE_NAME");
            break;
        case ERROR_INVALID_HANDLE:
            addLogMessage((char*)"Error: ERROR_INVALID_HANDLE");
            break;
        case ERROR_INVALID_NAME:
            addLogMessage((char*)"Error: ERROR_INVALID_NAME");
            break;
        case ERROR_INVALID_PARAMETER:
            addLogMessage((char*)"Error: ERROR_INVALID_PARAMETER");
            break;
        case ERROR_INVALID_SERVICE_ACCOUNT:
            addLogMessage((char*)"Error: ERROR_INVALID_SERVICE_ACCOUNT");
            break;
        case ERROR_SERVICE_EXISTS:
            addLogMessage((char*)"Error: ERROR_SERVICE_EXISTS");
            break;
        default:
            addLogMessage((char*)"Error: Undefined");
        }
        CloseServiceHandle(hSCManager);
        return -1;
    }
    CloseServiceHandle(hService);

    CloseServiceHandle(hSCManager);
    addLogMessage((char*)"Success install service!");
    return 0;
}

int RemoveService() {
    SC_HANDLE hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if (!hSCManager) {
        addLogMessage((char*)"Error: Can't open Service Control Manager");
        return -1;
    }
    SC_HANDLE hService = OpenService(hSCManager, serviceName, SERVICE_STOP | DELETE);
    if (!hService) {
        addLogMessage((char*)"Error: Can't remove service");
        CloseServiceHandle(hSCManager);
        return -1;
    }

    DeleteService(hService);
    CloseServiceHandle(hService);
    CloseServiceHandle(hSCManager);
    addLogMessage((char*)"Success remove service!");
    return 0;
}

int StartService() {
    SC_HANDLE hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_CREATE_SERVICE);
    SC_HANDLE hService = OpenService(hSCManager, serviceName, SERVICE_START);
    if (!StartService(hService, 0, NULL)) {
        CloseServiceHandle(hSCManager);
        addLogMessage((char*)"Error: Can't start service");
        return -1;
    }

    CloseServiceHandle(hService);
    CloseServiceHandle(hSCManager);
    return 0;
}

int StopService() {
    SC_HANDLE hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    SC_HANDLE hService = OpenServiceW(hSCManager, serviceName, SERVICE_STOP);
    bool ok = ControlService(hService, SERVICE_CONTROL_STOP, &serviceStatus);
    if (ok)
        addLogMessage((char*)"Stop service");
    CloseServiceHandle(hService);
    CloseServiceHandle(hSCManager);
    //addLogMessage((char*)"Error: Can't stop service");
    return 0;
}




int _tmain(int argc, _TCHAR* argv[]) {

    //servicePath = LPTSTR(argv[0]);

    if (argc - 1 == 0) {
        SERVICE_TABLE_ENTRY ServiceTable[1];
        ServiceTable[0].lpServiceName = (LPWSTR)serviceName;
        ServiceTable[0].lpServiceProc = (LPSERVICE_MAIN_FUNCTION)ServiceMain;
        
        if (!StartServiceCtrlDispatcher(ServiceTable)) {
            DWORD a = GetLastError();
            addLogMessage((char*)"Error: StartServiceCtrlDispatcher");
        }
    }
    else if (wcscmp(argv[argc - 1], _T("install")) == 0) {
        InstallService();
    }
    else if (wcscmp(argv[argc - 1], _T("remove")) == 0) {
        RemoveService();
    }
    else if (wcscmp(argv[argc - 1], _T("start")) == 0) {
        StartService();

    }
    else if (wcscmp(argv[argc - 1], _T("stop")) == 0) {
        StopService();

    }
}
