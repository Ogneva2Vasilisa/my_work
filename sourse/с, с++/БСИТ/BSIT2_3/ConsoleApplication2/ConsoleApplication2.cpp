#include <windows.h>
#include <taskschd.h>
#include <comdef.h>
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <sstream>

using namespace std;
#pragma comment(lib, "taskschd.lib")
#pragma comment(lib, "comsuppw.lib")

struct TaskInfo {
    std::wstring name;
    std::wstring path;
    std::wstring state;
    std::wstring lastRunTime;
    std::wstring nextRunTime;
    std::wstring description;
};

// Преобразование DATE (COM дата) в строку
std::wstring DateToString(DATE date) {
    if (date == 0) return L"N/A";

    SYSTEMTIME st;
    VariantTimeToSystemTime(date, &st);

    std::wstringstream ss;
    ss << std::setw(2) << std::setfill(L'0') << st.wDay << L"/"
        << std::setw(2) << std::setfill(L'0') << st.wMonth << L"/"
        << st.wYear << L" "
        << std::setw(2) << std::setfill(L'0') << st.wHour << L":"
        << std::setw(2) << std::setfill(L'0') << st.wMinute << L":"
        << std::setw(2) << std::setfill(L'0') << st.wSecond;

    return ss.str();
}

// Преобразование SYSTEMTIME в time_t для удобства сравнения
time_t SystemTimeToTimeT(const SYSTEMTIME& st) {
    struct tm t = {};
    t.tm_year = st.wYear - 1900;
    t.tm_mon = st.wMonth - 1;
    t.tm_mday = st.wDay;
    t.tm_hour = st.wHour;
    t.tm_min = st.wMinute;
    t.tm_sec = st.wSecond;
    t.tm_isdst = -1;
    return mktime(&t);
}

// Рекурсивный обход папок планировщика задач с фильтрацией по дате
void GetTasksFromFolder(ITaskFolder* pFolder, const std::wstring& folderPath, std::vector<TaskInfo>& tasks, time_t startTime, time_t endTime) {
    IRegisteredTaskCollection* pTaskCollection = NULL;
    HRESULT hr = pFolder->GetTasks(TASK_ENUM_HIDDEN, &pTaskCollection);

    if (SUCCEEDED(hr)) {
        LONG numTasks = 0;
        pTaskCollection->get_Count(&numTasks);

        for (LONG i = 0; i < numTasks; i++) {
            IRegisteredTask* pRegisteredTask = NULL;
            hr = pTaskCollection->get_Item(_variant_t(i + 1), &pRegisteredTask);

            if (SUCCEEDED(hr)) {
                TaskInfo taskInfo;
                taskInfo.path = folderPath;

                BSTR taskName = NULL;
                if (SUCCEEDED(pRegisteredTask->get_Name(&taskName))) {
                    taskInfo.name = taskName;
                    SysFreeString(taskName);
                }

                bool includeTask = false;

                try {
                    TASK_STATE taskState;
                    if (SUCCEEDED(pRegisteredTask->get_State(&taskState))) {
                        switch (taskState) {
                        case TASK_STATE_DISABLED: taskInfo.state = L"Disabled"; break;
                        case TASK_STATE_QUEUED: taskInfo.state = L"Queued"; break;
                        case TASK_STATE_READY: taskInfo.state = L"Ready"; break;
                        case TASK_STATE_RUNNING: taskInfo.state = L"Running"; break;
                        default: taskInfo.state = L"Unknown";
                        }
                    }

                    DATE lastRunDate = 0;
                    if (SUCCEEDED(pRegisteredTask->get_LastRunTime(&lastRunDate))) {
                        if (lastRunDate != 0) {
                            SYSTEMTIME st;
                            VariantTimeToSystemTime(lastRunDate, &st);
                            time_t lastRun = SystemTimeToTimeT(st);
                            if (lastRun >= startTime && lastRun <= endTime) {
                                includeTask = true;
                                taskInfo.lastRunTime = DateToString(lastRunDate);
                            }
                        }
                    }

                    DATE nextRunDate;
                    if (SUCCEEDED(pRegisteredTask->get_NextRunTime(&nextRunDate))) {
                        taskInfo.nextRunTime = DateToString(nextRunDate);
                    }

                    ITaskDefinition* pTaskDefinition = NULL;
                    if (SUCCEEDED(pRegisteredTask->get_Definition(&pTaskDefinition))) {
                        IRegistrationInfo* pRegInfo = NULL;
                        if (SUCCEEDED(pTaskDefinition->get_RegistrationInfo(&pRegInfo))) {
                            BSTR description = NULL;
                            if (SUCCEEDED(pRegInfo->get_Description(&description))) {
                                taskInfo.description = description ? description : L"No description";
                                SysFreeString(description);
                            }
                            pRegInfo->Release();
                        }
                        pTaskDefinition->Release();
                    }
                }
                catch (...) {
                    // Игнорируем ошибки
                }

                if (includeTask) {
                    tasks.push_back(taskInfo);
                }

                pRegisteredTask->Release();
            }
        }
        pTaskCollection->Release();
    }

    // Рекурсивно обходим подпапки
    ITaskFolderCollection* pFolderCollection = NULL;
    hr = pFolder->GetFolders(0, &pFolderCollection);

    if (SUCCEEDED(hr)) {
        LONG numFolders = 0;
        pFolderCollection->get_Count(&numFolders);

        for (LONG i = 0; i < numFolders; i++) {
            ITaskFolder* pSubFolder = NULL;
            hr = pFolderCollection->get_Item(_variant_t(i + 1), &pSubFolder);

            if (SUCCEEDED(hr)) {
                BSTR folderName = NULL;
                pSubFolder->get_Name(&folderName);
                std::wstring subFolderPath = folderPath + L"\\" + folderName;
                SysFreeString(folderName);

                GetTasksFromFolder(pSubFolder, subFolderPath, tasks, startTime, endTime);
                pSubFolder->Release();
            }
        }
        pFolderCollection->Release();
    }
}

// Функция вывода задач за период
void ListTasksInPeriod(const SYSTEMTIME& start, const SYSTEMTIME& end) {
    HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
    if (FAILED(hr)) {
        std::wcerr << L"CoInitializeEx failed: " << hr << std::endl;
        return;
    }

    hr = CoInitializeSecurity(
        NULL, -1, NULL, NULL,
        RPC_C_AUTHN_LEVEL_PKT_PRIVACY,
        RPC_C_IMP_LEVEL_IMPERSONATE,
        NULL, EOAC_NONE, NULL);
    if (FAILED(hr)) {
        std::wcerr << L"CoInitializeSecurity failed: " << hr << std::endl;
        CoUninitialize();
        return;
    }

    ITaskService* pService = NULL;
    hr = CoCreateInstance(
        CLSID_TaskScheduler,
        NULL,
        CLSCTX_INPROC_SERVER,
        IID_ITaskService,
        (void**)&pService);
    if (FAILED(hr)) {
        std::wcerr << L"CoCreateInstance failed: " << hr << std::endl;
        CoUninitialize();
        return;
    }

    hr = pService->Connect(_variant_t(), _variant_t(), _variant_t(), _variant_t());
    if (FAILED(hr)) {
        std::wcerr << L"ITaskService::Connect failed: " << hr << std::endl;
        pService->Release();
        CoUninitialize();
        return;
    }

    ITaskFolder* pRootFolder = NULL;
    hr = pService->GetFolder(_bstr_t(L"\\"), &pRootFolder);
    if (FAILED(hr)) {
        std::wcerr << L"GetFolder failed: " << hr << std::endl;
        pService->Release();
        CoUninitialize();
        return;
    }

    std::vector<TaskInfo> tasks;

    time_t startTime = SystemTimeToTimeT(start);
    time_t endTime = SystemTimeToTimeT(end);

    GetTasksFromFolder(pRootFolder, L"\\", tasks, startTime, endTime);

    pRootFolder->Release();
    pService->Release();
    CoUninitialize();

    if (tasks.empty()) {
        std::wcout << L"Нет задач, запущенных в указанный период." << std::endl;
        return;
    }

    std::wcout << L"Задачи, запущенные с "
        << std::setw(2) << std::setfill(L'0') << start.wDay << L"/"
        << std::setw(2) << std::setfill(L'0') << start.wMonth << L"/"
        << start.wYear << L" по "
        << std::setw(2) << std::setfill(L'0') << end.wDay << L"/"
        << std::setw(2) << std::setfill(L'0') << end.wMonth << L"/"
        << end.wYear << L":\n\n";

    for (const auto& task : tasks) {
        std::wcout << L"Name: " << task.name << std::endl;
        std::wcout << L"Path: " << task.path << std::endl;
        std::wcout << L"State: " << task.state << std::endl;
        std::wcout << L"Last Run: " << task.lastRunTime << std::endl;
        std::wcout << L"Next Run: " << task.nextRunTime << std::endl;
        std::wcout << L"Description: " << task.description << std::endl;
        std::wcout << L"----------------------------------------\n";
    }
}

// --- Функция вывода установленного ПО ---
void ListInstalledSoftware(HKEY hRoot, const wchar_t* subkey) {
    HKEY hKey;
    if (RegOpenKeyExW(hRoot, subkey, 0, KEY_READ, &hKey) != ERROR_SUCCESS)
        return;

    DWORD index = 0;
    wchar_t name[256];
    DWORD nameSize = 256;
    while (RegEnumKeyExW(hKey, index, name, &nameSize, NULL, NULL, NULL, NULL) == ERROR_SUCCESS) {
        HKEY hAppKey;
        if (RegOpenKeyExW(hKey, name, 0, KEY_READ, &hAppKey) == ERROR_SUCCESS) {
            wchar_t displayName[256];
            DWORD size = sizeof(displayName);
            if (RegQueryValueExW(hAppKey, L"DisplayName", NULL, NULL, (LPBYTE)displayName, &size) == ERROR_SUCCESS) {
                std::wcout << displayName << std::endl;
            }
            RegCloseKey(hAppKey);
        }
        index++;
        nameSize = 256;
    }
    RegCloseKey(hKey);
}

bool ReadDateTime(const wchar_t* prompt, SYSTEMTIME& st) {
    std::wcout << prompt;
    std::wstring line;
    std::getline(std::wcin >> std::ws, line); // Считываем всю строку, пропуская начальные пробелы

    std::wistringstream iss(line);
    int y, m, d, h = 0, min = 0, s = 0;
    if (!(iss >> y >> m >> d)) {
        std::wcout << L"Ошибка ввода даты." << std::endl;
        return false;
    }
    // Если пользователь ввёл время — считать его, иначе оставить 0
    iss >> h >> min >> s;

    st.wYear = y;
    st.wMonth = m;
    st.wDay = d;
    st.wHour = h;
    st.wMinute = min;
    st.wSecond = s;
    st.wMilliseconds = 0;
    return true;
}


int main() {
    std::wcout.imbue(locale("rus_rus.866"));
    std::wcin.imbue(locale("rus_rus.866"));

    char* command = new char[200];
    wstring address;
    char ex = 0;
    HRESULT hr;
    while (1) {
        wcout << L"Введите команду:\n";
        wcout << L"1 - Вывести список установленного ПО\n";
        wcout << L"2 - Вывести задачи, запущенные в указанный период\n";
        wcout << L"Введите номер: ";

        int choice = 0;
        std::wcin >> choice;

        switch (choice) {
        case 1:
            ListInstalledSoftware(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall");
            ListInstalledSoftware(HKEY_CURRENT_USER, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall");
            ListInstalledSoftware(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Wow6432Node\\Microsoft\\Windows\\CurrentVersion\\Uninstall");
            break;
        case 2: {
            SYSTEMTIME start, end;
            if (!ReadDateTime(L"Введите дату и время начала (ГГГГ ММ ДД чч мм сс): ", start)) return 1;
            if (!ReadDateTime(L"Введите дату и время конца (ГГГГ ММ ДД чч мм сс): ", end)) return 1;
            ListTasksInPeriod(start, end);
            break;
        }
        default:
            return 0;
            break;
        }
    }

    return 0;
}
