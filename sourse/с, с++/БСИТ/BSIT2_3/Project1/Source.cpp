#include <windows.h>
#include <taskschd.h>
#include <comdef.h>
#include <vector>
#include <string>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <algorithm>

#pragma comment(lib, "taskschd.lib")
#pragma comment(lib, "comsupp.lib")
#pragma comment(lib, "wbemuuid.lib")
#pragma comment(lib, "advapi32.lib")

using namespace std;

struct TaskInfo {
    std::wstring name;
    std::wstring path;
    std::wstring state;
    std::wstring lastRunTime;
    std::wstring nextRunTime;
    std::wstring description;
};

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

void GetTasksFromFolder(ITaskFolder* pFolder, const std::wstring& folderPath, std::vector<TaskInfo>& tasks) {
    // Получаем список задач в текущей папке
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

                // Получаем базовую информацию о задаче
                BSTR taskName = NULL;
                if (SUCCEEDED(pRegisteredTask->get_Name(&taskName))) {
                    taskInfo.name = taskName;
                    SysFreeString(taskName);
                }

                // Пытаемся получить дополнительные свойства
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

                    DATE lastRunDate;
                    if (SUCCEEDED(pRegisteredTask->get_LastRunTime(&lastRunDate))) {
                        taskInfo.lastRunTime = DateToString(lastRunDate);
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
                    // Пропускаем ошибки при получении дополнительных свойств
                }

                tasks.push_back(taskInfo);
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

                GetTasksFromFolder(pSubFolder, subFolderPath, tasks);
                pSubFolder->Release();
            }
        }
        pFolderCollection->Release();
    }
}

std::vector<TaskInfo> GetAllTasks() {
    std::vector<TaskInfo> tasks;
    HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);

    if (FAILED(hr)) {
        std::wcerr << L"CoInitializeEx failed: " << hr << std::endl;
        return tasks;
    }

    hr = CoInitializeSecurity(
        NULL, -1, NULL, NULL,
        RPC_C_AUTHN_LEVEL_PKT_PRIVACY,
        RPC_C_IMP_LEVEL_IMPERSONATE,
        NULL, 0, NULL);

    if (FAILED(hr)) {
        std::wcerr << L"CoInitializeSecurity failed: " << hr << std::endl;
        CoUninitialize();
        return tasks;
    }

    ITaskService* pService = NULL;
    hr = CoCreateInstance(
        CLSID_TaskScheduler,
        NULL,
        CLSCTX_INPROC_SERVER,
        IID_ITaskService,
        (void**)&pService);

    if (SUCCEEDED(hr)) {
        hr = pService->Connect(_variant_t(), _variant_t(), _variant_t(), _variant_t());
        if (SUCCEEDED(hr)) {
            ITaskFolder* pRootFolder = NULL;
            hr = pService->GetFolder(_bstr_t(L"\\"), &pRootFolder);

            if (SUCCEEDED(hr)) {
                GetTasksFromFolder(pRootFolder, L"\\", tasks);
                pRootFolder->Release();
            }
        }
        pService->Release();
    }

    CoUninitialize();
    return tasks;
}

void PrintTasksList() {
    auto tasks = GetAllTasks();
    std::wcout << L"Found " << tasks.size() << L" tasks:\n\n";

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

HRESULT CreateSecurityMonitoringTask() {
    HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
    if (FAILED(hr)) {
        std::wcerr << L"CoInitializeEx failed: " << hr << std::endl;
        return hr;
    }

    // Инициализация безопасности COM
    hr = CoInitializeSecurity(
        NULL,
        -1,
        NULL,
        NULL,
        RPC_C_AUTHN_LEVEL_PKT_PRIVACY,
        RPC_C_IMP_LEVEL_IMPERSONATE,
        NULL,
        0,
        NULL);

    if (FAILED(hr)) {
        std::wcerr << L"CoInitializeSecurity failed: " << hr << std::endl;
        CoUninitialize();
        return hr;
    }

    // Создание экземпляра службы планировщика задач
    ITaskService* pService = NULL;
    hr = CoCreateInstance(
        CLSID_TaskScheduler,
        NULL,
        CLSCTX_INPROC_SERVER,
        IID_ITaskService,
        (void**)&pService);

    if (FAILED(hr)) {
        std::wcerr << L"Failed to create ITaskService instance: " << hr << std::endl;
        CoUninitialize();
        return hr;
    }

    // Подключение к службе планировщика
    hr = pService->Connect(_variant_t(), _variant_t(), _variant_t(), _variant_t());
    if (FAILED(hr)) {
        std::wcerr << L"ITaskService::Connect failed: " << hr << std::endl;
        pService->Release();
        CoUninitialize();
        return hr;
    }

    // Получение корневой папки задач
    ITaskFolder* pRootFolder = NULL;
    hr = pService->GetFolder(_bstr_t(L"\\"), &pRootFolder);
    if (FAILED(hr)) {
        std::wcerr << L"Cannot get Root Folder pointer: " << hr << std::endl;
        pService->Release();
        CoUninitialize();
        return hr;
    }

    // Удаление задачи, если она уже существует
    pRootFolder->DeleteTask(_bstr_t(L"SecurityChangesMonitor"), 0);

    // Создание определения задачи
    ITaskDefinition* pTask = NULL;
    hr = pService->NewTask(0, &pTask);
    if (FAILED(hr)) {
        std::wcerr << L"Failed to create task definition: " << hr << std::endl;
        pRootFolder->Release();
        pService->Release();
        CoUninitialize();
        return hr;
    }

    // Настройка информации о регистрации
    IRegistrationInfo* pRegInfo = NULL;
    hr = pTask->get_RegistrationInfo(&pRegInfo);
    if (SUCCEEDED(hr)) {
        pRegInfo->put_Author(_bstr_t(L"Windows Security Monitor"));
        pRegInfo->put_Description(_bstr_t(
            L"Monitors changes in Windows Defender and Firewall settings. "
            L"Notifies user when security settings are modified."));
        pRegInfo->Release();
    }

    // Настройка триггеров событий
    ITriggerCollection* pTriggerCollection = NULL;
    hr = pTask->get_Triggers(&pTriggerCollection);
    if (FAILED(hr)) {
        std::wcerr << L"Cannot get trigger collection: " << hr << std::endl;
        pTask->Release();
        pRootFolder->Release();
        pService->Release();
        CoUninitialize();
        return hr;
    }

    // 1. Триггер для событий Windows Defender
    ITrigger* pDefenderTrigger = NULL;
    hr = pTriggerCollection->Create(TASK_TRIGGER_EVENT, &pDefenderTrigger);
    if (SUCCEEDED(hr)) {
        IEventTrigger* pEventTrigger = NULL;
        hr = pDefenderTrigger->QueryInterface(IID_IEventTrigger, (void**)&pEventTrigger);
        if (SUCCEEDED(hr)) {
            pEventTrigger->put_Id(_bstr_t(L"DefenderSettingsChangeTrigger"));
            pEventTrigger->put_Subscription(_bstr_t(
                L"<QueryList>"
                L"  <Query Id='0' Path='Microsoft-Windows-Windows Defender/Operational'>"
                L"    <Select Path='Microsoft-Windows-Windows Defender/Operational'>"
                L"      *[System[Provider[@Name='Microsoft-Windows-Windows Defender'] and "
                L"       (EventID=5000 or EventID=5001 or EventID=5004 or EventID=5007 or "
                L"        EventID=5010 or EventID=5012)]]"
                L"    </Select>"
                L"  </Query>"
                L"</QueryList>"));
            pEventTrigger->Release();
        }
        pDefenderTrigger->Release();
    }

    // 2. Триггер для событий брандмауэра
    ITrigger* pFirewallTrigger = NULL;
    hr = pTriggerCollection->Create(TASK_TRIGGER_EVENT, &pFirewallTrigger);
    if (SUCCEEDED(hr)) {
        IEventTrigger* pEventTrigger = NULL;
        hr = pFirewallTrigger->QueryInterface(IID_IEventTrigger, (void**)&pEventTrigger);
        if (SUCCEEDED(hr)) {
            pEventTrigger->put_Id(_bstr_t(L"FirewallSettingsChangeTrigger"));
            pEventTrigger->put_Subscription(_bstr_t(
                L"<QueryList>"
                L"  <Query Id='0' Path='Microsoft-Windows-Windows Firewall With Advanced Security/Firewall'>"
                L"    <Select Path='Microsoft-Windows-Windows Firewall With Advanced Security/Firewall'>"
                L"      *[System[Provider[@Name='Microsoft-Windows-Windows Firewall With Advanced Security'] and "
                L"       (EventID=2004 or EventID=2005 or EventID=2006 or EventID=2008 or "
                L"        EventID=2009 or EventID=2010 or EventID=2031)]]"
                L"    </Select>"
                L"  </Query>"
                L"</QueryList>"));
            pEventTrigger->Release();
        }
        pFirewallTrigger->Release();
    }

    pTriggerCollection->Release();

    // Настройка действия (показ сообщения)
    IActionCollection* pActionCollection = NULL;
    hr = pTask->get_Actions(&pActionCollection);
    if (FAILED(hr)) {
        std::wcerr << L"Cannot get action collection: " << hr << std::endl;
        pTask->Release();
        pRootFolder->Release();
        pService->Release();
        CoUninitialize();
        return hr;
    }

    IAction* pAction = NULL;
    hr = pActionCollection->Create(TASK_ACTION_EXEC, &pAction);
    if (SUCCEEDED(hr)) {
        IExecAction* pExecAction = NULL;
        hr = pAction->QueryInterface(IID_IExecAction, (void**)&pExecAction);
        if (SUCCEEDED(hr)) {
            pExecAction->put_Path(_bstr_t(L"powershell.exe"));
            pExecAction->put_Arguments(_bstr_t(
                L"-WindowStyle Hidden -Command \""
                L"Add-Type -AssemblyName PresentationFramework; "
                L"[System.Windows.MessageBox]::Show("
                L"'Обнаружены изменения в настройках безопасности Windows!\n\n' + "
                L"'Проверьте: Windows Defender', "
                L"'Security Alert', 'OK', 'Warning')\""));
            pExecAction->Release();
        }
        pAction->Release();
    }

    pActionCollection->Release();

    // Настройка параметров задачи
    ITaskSettings* pSettings = NULL;
    hr = pTask->get_Settings(&pSettings);
    if (SUCCEEDED(hr)) {
        pSettings->put_StartWhenAvailable(VARIANT_TRUE);
        pSettings->put_DisallowStartIfOnBatteries(VARIANT_FALSE);
        pSettings->put_StopIfGoingOnBatteries(VARIANT_FALSE);
        pSettings->put_ExecutionTimeLimit(_bstr_t(L"PT0H0M0S")); // Без ограничения времени
        pSettings->put_Enabled(VARIANT_TRUE);
        pSettings->Release();
    }

    // Регистрация задачи
    IRegisteredTask* pRegisteredTask = NULL;
    hr = pRootFolder->RegisterTaskDefinition(
        _bstr_t(L"SecurityChangesMonitor"),
        pTask,
        TASK_CREATE_OR_UPDATE,
        _variant_t(L"S-1-5-32-544"), // Группа администраторов
        _variant_t(),
        TASK_LOGON_GROUP,
        _variant_t(L""),
        &pRegisteredTask);

    if (FAILED(hr)) {
        std::wcerr << L"Error registering the task: " << hr << std::endl;
    }
    else {
        std::wcout << L"Successfully created security monitoring task" << std::endl;
        pRegisteredTask->Release();
    }

    // Освобождение ресурсов
    pTask->Release();
    pRootFolder->Release();
    pService->Release();
    CoUninitialize();

    return hr;
}

HRESULT CreateFirewallMonitoringTask() {
    HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
    if (FAILED(hr)) return hr;

    hr = CoInitializeSecurity(
        NULL, -1, NULL, NULL,
        RPC_C_AUTHN_LEVEL_PKT_PRIVACY,
        RPC_C_IMP_LEVEL_IMPERSONATE,
        NULL, 0, NULL);

    if (FAILED(hr)) {
        CoUninitialize();
        return hr;
    }

    ITaskService* pService = NULL;
    hr = CoCreateInstance(
        CLSID_TaskScheduler,
        NULL,
        CLSCTX_INPROC_SERVER,
        IID_ITaskService,
        (void**)&pService);

    if (FAILED(hr)) {
        CoUninitialize();
        return hr;
    }

    hr = pService->Connect(_variant_t(), _variant_t(), _variant_t(), _variant_t());
    if (FAILED(hr)) {
        pService->Release();
        CoUninitialize();
        return hr;
    }

    ITaskFolder* pRootFolder = NULL;
    hr = pService->GetFolder(_bstr_t(L"\\"), &pRootFolder);
    if (FAILED(hr)) {
        pService->Release();
        CoUninitialize();
        return hr;
    }

    // Удаляем существующую задачу, если есть
    pRootFolder->DeleteTask(_bstr_t(L"FirewallChangesMonitor"), 0);

    ITaskDefinition* pTask = NULL;
    hr = pService->NewTask(0, &pTask);
    if (FAILED(hr)) {
        pRootFolder->Release();
        pService->Release();
        CoUninitialize();
        return hr;
    }

    // Настройка информации о задаче
    IRegistrationInfo* pRegInfo = NULL;
    hr = pTask->get_RegistrationInfo(&pRegInfo);
    if (SUCCEEDED(hr)) {
        pRegInfo->put_Author(_bstr_t(L"System Monitor"));
        pRegInfo->put_Description(_bstr_t(L"Monitors firewall configuration changes"));
        pRegInfo->Release();
    }

    // Настройка триггеров
    ITriggerCollection* pTriggerCollection = NULL;
    hr = pTask->get_Triggers(&pTriggerCollection);
    if (FAILED(hr)) {
        pTask->Release();
        pRootFolder->Release();
        pService->Release();
        CoUninitialize();
        return hr;
    }

    // Триггер для аудита реестра (настройки брандмауэра хранятся здесь)
    ITrigger* pRegTrigger = NULL;
    hr = pTriggerCollection->Create(TASK_TRIGGER_EVENT, &pRegTrigger);
    if (SUCCEEDED(hr)) {
        IEventTrigger* pEventTrigger = NULL;
        hr = pRegTrigger->QueryInterface(IID_IEventTrigger, (void**)&pEventTrigger);
        if (SUCCEEDED(hr)) {
            pEventTrigger->put_Id(_bstr_t(L"RegistryChangeTrigger"));
            pEventTrigger->put_Subscription(_bstr_t(
                L"<QueryList>"
                L"  <Query Id='0'>"
                L"    <Select Path='Security'>"
                L"      *[System[Provider[@Name='Microsoft-Windows-Security-Auditing'] and "
                L"       (EventID=4657)]]"
                L"      and "
                L"      *[EventData[Data[@Name='ObjectName'] and "
                L"       (Data='HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Services\\SharedAccess\\Parameters\\FirewallPolicy')]]"
                L"    </Select>"
                L"  </Query>"
                L"</QueryList>"));
            pEventTrigger->Release();
        }
        pRegTrigger->Release();
    }

    // Триггер для событий брандмауэра
    ITrigger* pFirewallTrigger = NULL;
    hr = pTriggerCollection->Create(TASK_TRIGGER_EVENT, &pFirewallTrigger);
    if (SUCCEEDED(hr)) {
        IEventTrigger* pEventTrigger = NULL;
        hr = pFirewallTrigger->QueryInterface(IID_IEventTrigger, (void**)&pEventTrigger);
        if (SUCCEEDED(hr)) {
            pEventTrigger->put_Id(_bstr_t(L"FirewallEventTrigger"));
            pEventTrigger->put_Subscription(_bstr_t(
                L"<QueryList>"
                L"  <Query Id = '0' Path = 'Microsoft-Windows-Windows Firewall With Advanced Security/ConnectionSecurity'>"
                L"      <Select Path = 'Microsoft-Windows-Windows Firewall With Advanced Security/ConnectionSecurity'>*[System[Provider[@Name='Microsoft-Windows-Firewall' or @Name='Microsoft-Windows-Windows Firewall With Advanced Security']]]</Select>"
                L"      <Select Path = 'Microsoft-Windows-Windows Firewall With Advanced Security/ConnectionSecurityVerbose'>*[System[Provider[@Name='Microsoft-Windows-Firewall' or @Name='Microsoft-Windows-Windows Firewall With Advanced Security']]]</Select>"
                L"      <Select Path = 'Microsoft-Windows-Windows Firewall With Advanced Security/Firewall'>*[System[Provider[@Name='Microsoft-Windows-Firewall' or @Name='Microsoft-Windows-Windows Firewall With Advanced Security']]]</Select>"
                L"      <Select Path = 'Microsoft-Windows-Windows Firewall With Advanced Security/FirewallDiagnostics'>*[System[Provider[@Name='Microsoft-Windows-Firewall' or @Name='Microsoft-Windows-Windows Firewall With Advanced Security']]]</Select>"
                L"      <Select Path = 'Microsoft-Windows-Windows Firewall With Advanced Security/FirewallVerbose'>*[System[Provider[@Name='Microsoft-Windows-Firewall' or @Name='Microsoft-Windows-Windows Firewall With Advanced Security']]]</Select>"
                L"      <Select Path = 'Network Isolation Operational'>*[System[Provider[@Name='Microsoft-Windows-Firewall' or @Name='Microsoft-Windows-Windows Firewall With Advanced Security']]]</Select>"
                L"      <Select Path = 'System'>*[System[Provider[@Name='Microsoft-Windows-Firewall' or @Name='Microsoft-Windows-Windows Firewall With Advanced Security']]]</Select>"
                L"  </Query>"
                L"</QueryList>"));
            pEventTrigger->Release();
        }
        pFirewallTrigger->Release();
    }

    pTriggerCollection->Release();

    // Настройка действия
    IActionCollection* pActionCollection = NULL;
    hr = pTask->get_Actions(&pActionCollection);
    if (FAILED(hr)) {
        pTask->Release();
        pRootFolder->Release();
        pService->Release();
        CoUninitialize();
        return hr;
    }

    IAction* pAction = NULL;
    hr = pActionCollection->Create(TASK_ACTION_EXEC, &pAction);
    if (SUCCEEDED(hr)) {
        IExecAction* pExecAction = NULL;
        hr = pAction->QueryInterface(IID_IExecAction, (void**)&pExecAction);
        if (SUCCEEDED(hr)) {
            pExecAction->put_Path(_bstr_t(L"powershell.exe"));
            pExecAction->put_Arguments(_bstr_t(
                L"-WindowStyle Hidden -Command \""
                L"Add-Type -AssemblyName PresentationFramework; "
                L"[System.Windows.MessageBox]::Show("
                L"'Обнаружены изменения в настройках безопасности Windows!\n\n' + "
                L"'Проверьте: Брандмауэр Windows', "
                L"'Security Alert', 'OK', 'Warning')\""));
            pExecAction->Release();
        }
        pAction->Release();
    }

    pActionCollection->Release();

    // Настройка параметров задачи
    ITaskSettings* pSettings = NULL;
    hr = pTask->get_Settings(&pSettings);
    if (SUCCEEDED(hr)) {
        pSettings->put_StartWhenAvailable(VARIANT_TRUE);
        pSettings->put_DisallowStartIfOnBatteries(VARIANT_FALSE);
        pSettings->put_StopIfGoingOnBatteries(VARIANT_FALSE);
        pSettings->put_ExecutionTimeLimit(_bstr_t(L"PT0H0M0S"));
        pSettings->put_Enabled(VARIANT_TRUE);
        pSettings->Release();
    }

    // Регистрация задачи
    IRegisteredTask* pRegisteredTask = NULL;
    hr = pRootFolder->RegisterTaskDefinition(
        _bstr_t(L"FirewallChangesMonitor"),
        pTask,
        TASK_CREATE_OR_UPDATE,
        _variant_t(L"S-1-5-32-544"), // Администраторы
        _variant_t(),
        TASK_LOGON_GROUP,
        _variant_t(L""),
        &pRegisteredTask);

    if (SUCCEEDED(hr)) {
        pRegisteredTask->Release();
    }

    pTask->Release();
    pRootFolder->Release();
    pService->Release();
    CoUninitialize();

    return hr;
}

// Функция для включения аудита реестра
bool EnableRegistryAuditing() {
    HKEY hKey;
    LONG lResult = RegOpenKeyEx(
        HKEY_LOCAL_MACHINE,
        L"SYSTEM\\CurrentControlSet\\Control\\Lsa",
        0,
        KEY_READ | KEY_WRITE,
        &hKey);

    if (lResult != ERROR_SUCCESS) {
        return false;
    }

    DWORD value = 1;
    lResult = RegSetValueEx(
        hKey,
        L"auditbaseobjects",
        0,
        REG_DWORD,
        (const BYTE*)&value,
        sizeof(DWORD));

    RegCloseKey(hKey);

    return (lResult == ERROR_SUCCESS);
}


std::wstring GetErrorMessage(HRESULT hr) {
    _com_error err(hr);
    return err.ErrorMessage();
}

HRESULT CreateICMPBlockNotificationTask(const std::wstring& targetIP) {
    HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
    if (FAILED(hr)) {
        std::wcerr << L"CoInitializeEx failed: " << GetErrorMessage(hr) << std::endl;
        return hr;
    }

    hr = CoInitializeSecurity(
        NULL, -1, NULL, NULL,
        RPC_C_AUTHN_LEVEL_PKT_PRIVACY,
        RPC_C_IMP_LEVEL_IMPERSONATE,
        NULL, 0, NULL);

    if (FAILED(hr)) {
        std::wcerr << L"CoInitializeSecurity failed: " << GetErrorMessage(hr) << std::endl;
        CoUninitialize();
        return hr;
    }

    ITaskService* pService = NULL;
    hr = CoCreateInstance(
        CLSID_TaskScheduler,
        NULL,
        CLSCTX_INPROC_SERVER,
        IID_ITaskService,
        (void**)&pService);

    if (FAILED(hr)) {
        std::wcerr << L"Failed to create ITaskService: " << GetErrorMessage(hr) << std::endl;
        CoUninitialize();
        return hr;
    }

    hr = pService->Connect(_variant_t(), _variant_t(), _variant_t(), _variant_t());
    if (FAILED(hr)) {
        std::wcerr << L"ITaskService::Connect failed: " << GetErrorMessage(hr) << std::endl;
        pService->Release();
        CoUninitialize();
        return hr;
    }

    ITaskFolder* pRootFolder = NULL;
    hr = pService->GetFolder(_bstr_t(L"\\"), &pRootFolder);
    if (FAILED(hr)) {
        std::wcerr << L"Cannot get Root Folder: " << GetErrorMessage(hr) << std::endl;
        pService->Release();
        CoUninitialize();
        return hr;
    }

    // Удаление существующей задачи
    pRootFolder->DeleteTask(_bstr_t(L"ICMPBlockMonitor"), 0);

    ITaskDefinition* pTask = NULL;
    hr = pService->NewTask(0, &pTask);
    if (FAILED(hr)) {
        std::wcerr << L"Failed to create task definition: " << GetErrorMessage(hr) << std::endl;
        pRootFolder->Release();
        pService->Release();
        CoUninitialize();
        return hr;
    }

    // 1. Настройка информации о регистрации
    IRegistrationInfo* pRegInfo = NULL;
    hr = pTask->get_RegistrationInfo(&pRegInfo);
    if (SUCCEEDED(hr)) {
        pRegInfo->put_Author(_bstr_t(L"System Monitor"));
        pRegInfo->put_Description(_bstr_t(L"Monitors blocked ICMP requests"));
        pRegInfo->Release();
    }

    // 2. Настройка параметров безопасности
    IPrincipal* pPrincipal = NULL;
    hr = pTask->get_Principal(&pPrincipal);
    if (SUCCEEDED(hr)) {
        // Используем системную учетную запись
        pPrincipal->put_UserId(_bstr_t(L"NT AUTHORITY\\SYSTEM"));
        pPrincipal->put_LogonType(TASK_LOGON_SERVICE_ACCOUNT);
        pPrincipal->put_RunLevel(TASK_RUNLEVEL_LUA);
        pPrincipal->Release();
    }

    // 3. Настройка триггера события
    ITriggerCollection* pTriggerCollection = NULL;
    hr = pTask->get_Triggers(&pTriggerCollection);
    if (FAILED(hr)) {
        std::wcerr << L"Cannot get trigger collection: " << GetErrorMessage(hr) << std::endl;
        pTask->Release();
        pRootFolder->Release();
        pService->Release();
        CoUninitialize();
        return hr;
    }

    ITrigger* pTrigger = NULL;
    hr = pTriggerCollection->Create(TASK_TRIGGER_EVENT, &pTrigger);
    if (SUCCEEDED(hr)) {
        IEventTrigger* pEventTrigger = NULL;
        hr = pTrigger->QueryInterface(IID_IEventTrigger, (void**)&pEventTrigger);
        if (SUCCEEDED(hr)) {
            pEventTrigger->put_Id(_bstr_t(L"ICMPBlockTrigger"));

            std::wstring query = L"<QueryList>\n"
                L"  <Query Id='0' Path='Microsoft-Windows-Windows Firewall With Advanced Security/Firewall'>\n"
                L"      <Select Path='Microsoft-Windows-Windows Firewall With Advanced Security/Firewall'>"
                L"*[System[Provider[@Name='Microsoft-Windows-Windows Firewall With Advanced Security'] and EventID=5152]"
                L" and *[EventData[Data[@Name='Protocol']='1']";

            if (!targetIP.empty()) {
                query += L" and *[EventData[Data[@Name='SourceAddress']='" + targetIP + L"']";
            }

            query += L"</Select>\n </Query>\n</QueryList>";
            printf("%ws\n", query.c_str());
            pEventTrigger->put_Subscription(_bstr_t(query.c_str()));
            pEventTrigger->Release();
        }
        pTrigger->Release();
    }
    pTriggerCollection->Release();

    // 4. Настройка действия
    IActionCollection* pActionCollection = NULL;
    hr = pTask->get_Actions(&pActionCollection);
    if (FAILED(hr)) {
        std::wcerr << L"Cannot get action collection: " << GetErrorMessage(hr) << std::endl;
        pTask->Release();
        pRootFolder->Release();
        pService->Release();
        CoUninitialize();
        return hr;
    }

    IAction* pAction = NULL;
    hr = pActionCollection->Create(TASK_ACTION_SHOW_MESSAGE, &pAction);
    if (SUCCEEDED(hr)) {
        IShowMessageAction* pShowAction = NULL;
        hr = pAction->QueryInterface(IID_IShowMessageAction, (void**)&pShowAction);
        if (SUCCEEDED(hr)) {
            std::wstring message = L"Брандмауэр заблокировал ICMP-запрос";
            if (!targetIP.empty()) {
                message += L" с IP: " + targetIP;
            }

            pShowAction->put_Title(_bstr_t(L"Security Alert"));
            pShowAction->put_MessageBody(_bstr_t(message.c_str()));
            pShowAction->Release();
        }
        pAction->Release();
    }
    pActionCollection->Release();

    // 5. Настройка параметров задачи
    ITaskSettings* pSettings = NULL;
    hr = pTask->get_Settings(&pSettings);
    if (SUCCEEDED(hr)) {
        pSettings->put_StartWhenAvailable(VARIANT_TRUE);
        pSettings->put_DisallowStartIfOnBatteries(VARIANT_FALSE);
        pSettings->put_StopIfGoingOnBatteries(VARIANT_FALSE);
        pSettings->put_ExecutionTimeLimit(_bstr_t(L"PT0H0M0S"));
        pSettings->put_Enabled(VARIANT_TRUE);
        pSettings->Release();
    }

    // 6. Регистрация задачи
    IRegisteredTask* pRegisteredTask = NULL;
    hr = pRootFolder->RegisterTaskDefinition(
        _bstr_t(L"ICMPBlockMonitor"),
        pTask,
        TASK_CREATE_OR_UPDATE,
        _variant_t(L"NT AUTHORITY\\SYSTEM"), // SYSTEM account
        _variant_t(),
        TASK_LOGON_SERVICE_ACCOUNT,
        _variant_t(L""),
        &pRegisteredTask);

    if (FAILED(hr)) {
        std::wcerr << L"Error registering task (0x" << std::hex << hr << "): " << GetErrorMessage(hr) << std::endl;
    }
    else {
        std::wcout << L"Task created successfully!" << std::endl;
        pRegisteredTask->Release();
    }

    pTask->Release();
    pRootFolder->Release();
    pService->Release();
    CoUninitialize();

    return hr;
}


HRESULT CreateICMPMonitoringTask(const std::wstring& targetIP) {
    HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
    if (FAILED(hr)) {
        std::wcerr << L"CoInitializeEx failed: " << hr << std::endl;
        return hr;
    }

    // Инициализация безопасности COM
    hr = CoInitializeSecurity(
        NULL,
        -1,
        NULL,
        NULL,
        RPC_C_AUTHN_LEVEL_PKT_PRIVACY,
        RPC_C_IMP_LEVEL_IMPERSONATE,
        NULL,
        0,
        NULL);

    if (FAILED(hr)) {
        std::wcerr << L"CoInitializeSecurity failed: " << hr << std::endl;
        CoUninitialize();
        return hr;
    }

    // Создание экземпляра службы планировщика задач
    ITaskService* pService = NULL;
    hr = CoCreateInstance(
        CLSID_TaskScheduler,
        NULL,
        CLSCTX_INPROC_SERVER,
        IID_ITaskService,
        (void**)&pService);

    if (FAILED(hr)) {
        std::wcerr << L"Failed to create ITaskService instance: " << hr << std::endl;
        CoUninitialize();
        return hr;
    }

    // Подключение к службе планировщика
    hr = pService->Connect(_variant_t(), _variant_t(), _variant_t(), _variant_t());
    if (FAILED(hr)) {
        std::wcerr << L"ITaskService::Connect failed: " << hr << std::endl;
        pService->Release();
        CoUninitialize();
        return hr;
    }

    // Получение корневой папки задач
    ITaskFolder* pRootFolder = NULL;
    hr = pService->GetFolder(_bstr_t(L"\\"), &pRootFolder);
    if (FAILED(hr)) {
        std::wcerr << L"Cannot get Root Folder pointer: " << hr << std::endl;
        pService->Release();
        CoUninitialize();
        return hr;
    }

    // Удаление задачи, если она уже существует
    pRootFolder->DeleteTask(_bstr_t(L"SecurityChangesMonitor"), 0);

    // Создание определения задачи
    ITaskDefinition* pTask = NULL;
    hr = pService->NewTask(0, &pTask);
    if (FAILED(hr)) {
        std::wcerr << L"Failed to create task definition: " << hr << std::endl;
        pRootFolder->Release();
        pService->Release();
        CoUninitialize();
        return hr;
    }

    // Настройка информации о регистрации
    IRegistrationInfo* pRegInfo = NULL;
    hr = pTask->get_RegistrationInfo(&pRegInfo);
    if (SUCCEEDED(hr)) {
        pRegInfo->put_Author(_bstr_t(L"Windows Security Monitor"));
        pRegInfo->put_Description(_bstr_t(
            L"Monitors changes in Windows Defender and Firewall settings. "
            L"Notifies user when security settings are modified."));
        pRegInfo->Release();
    }

    // Настройка триггеров событий
    ITriggerCollection* pTriggerCollection = NULL;
    hr = pTask->get_Triggers(&pTriggerCollection);
    if (FAILED(hr)) {
        std::wcerr << L"Cannot get trigger collection: " << hr << std::endl;
        pTask->Release();
        pRootFolder->Release();
        pService->Release();
        CoUninitialize();
        return hr;
    }

    // Триггер для событий брандмауэра
    ITrigger* pFirewallTrigger = NULL;
    hr = pTriggerCollection->Create(TASK_TRIGGER_EVENT, &pFirewallTrigger);
    if (SUCCEEDED(hr)) {
        IEventTrigger* pEventTrigger = NULL;
        hr = pFirewallTrigger->QueryInterface(IID_IEventTrigger, (void**)&pEventTrigger);
        if (SUCCEEDED(hr)) {
            pEventTrigger->put_Id(_bstr_t(L"FirewallSettingsChangeTrigger"));
            wstring str = L"<QueryList>"
                L"  <Query Id = '0' Path = 'Security'>"
                L"    <Select Path = 'Security'>*[System[(EventID = 5152)]] and *[EventData[Data[@Name = 'SourceAddress'] = '" + targetIP + L"']]</Select>"
                L"  </Query>"
                L"</QueryList>";
            pEventTrigger->put_Subscription(_bstr_t(str.c_str()));
            pEventTrigger->Release();
        }
        pFirewallTrigger->Release();
    }

    pTriggerCollection->Release();

    // Настройка действия (показ сообщения)
    IActionCollection* pActionCollection = NULL;
    hr = pTask->get_Actions(&pActionCollection);
    if (FAILED(hr)) {
        std::wcerr << L"Cannot get action collection: " << hr << std::endl;
        pTask->Release();
        pRootFolder->Release();
        pService->Release();
        CoUninitialize();
        return hr;
    }

    IAction* pAction = NULL;
    hr = pActionCollection->Create(TASK_ACTION_EXEC, &pAction);
    if (SUCCEEDED(hr)) {
        IExecAction* pExecAction = NULL;
        hr = pAction->QueryInterface(IID_IExecAction, (void**)&pExecAction);
        if (SUCCEEDED(hr)) {
            pExecAction->put_Path(_bstr_t(L"powershell.exe"));
            pExecAction->put_Arguments(_bstr_t(
                L"-WindowStyle Hidden -Command \""
                L"Add-Type -AssemblyName PresentationFramework; "
                L"[System.Windows.MessageBox]::Show("
                L"'Обнаружены попытки подключения!\n\n' + "
                L"'Сброшен пакет', "
                L"'Security Alert', 'OK', 'Warning')\""));
            pExecAction->Release();
        }
        pAction->Release();
    }

    pActionCollection->Release();

    // Настройка параметров задачи
    ITaskSettings* pSettings = NULL;
    hr = pTask->get_Settings(&pSettings);
    if (SUCCEEDED(hr)) {
        pSettings->put_StartWhenAvailable(VARIANT_TRUE);
        pSettings->put_DisallowStartIfOnBatteries(VARIANT_FALSE);
        pSettings->put_StopIfGoingOnBatteries(VARIANT_FALSE);
        pSettings->put_ExecutionTimeLimit(_bstr_t(L"PT0H0M0S")); // Без ограничения времени
        pSettings->put_Enabled(VARIANT_TRUE);
        pSettings->Release();
    }

    // Регистрация задачи
    IRegisteredTask* pRegisteredTask = NULL;
    hr = pRootFolder->RegisterTaskDefinition(
        _bstr_t(L"ICMPChangesMonitor"),
        pTask,
        TASK_CREATE_OR_UPDATE,
        _variant_t(L"S-1-5-32-544"), // Группа администраторов
        _variant_t(),
        TASK_LOGON_GROUP,
        _variant_t(L""),
        &pRegisteredTask);

    if (FAILED(hr)) {
        std::wcerr << L"Error registering the task: " << hr << std::endl;
    }
    else {
        std::wcout << L"Successfully created security monitoring task" << std::endl;
        pRegisteredTask->Release();
    }

    // Освобождение ресурсов
    pTask->Release();
    pRootFolder->Release();
    pService->Release();
    CoUninitialize();

    return hr;
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
        wcout << L"1 - Список активных задач\n";
        wcout << L"2 - Задача Defender\n";
        wcout << L"3 - Задача брандмауэр\n";
        wcout << L"4 - Задача ICMP\n";
        fgets(command, 200, stdin);
        int comm = strtol(command, 0, 10);
        switch (comm)
        {
        case 1:
            PrintTasksList();
            break;
        case 2:
            hr = CreateSecurityMonitoringTask();
            if (SUCCEEDED(hr)) {
                std::wcout << L"Security monitoring task created successfully!" << std::endl;
            }
            else {
                std::wcout << L"Failed to create security monitoring task. Error: " << hr << std::endl;
            }
            break;
        case 3:
            hr = CreateFirewallMonitoringTask();
            if (SUCCEEDED(hr)) {
                std::wcout << L"Security monitoring task created successfully!" << std::endl;
            }
            else {
                std::wcout << L"Failed to create security monitoring task. Error: " << hr << std::endl;
            }
            break;
        case 4:
            wcout << L"Введите адрес:\n";
            wcin >> address;
            hr = CreateICMPMonitoringTask(address);

            if (SUCCEEDED(hr)) {
                std::wcout << L"ICMP block monitoring task created successfully!" << std::endl;
            }
            else {
                std::wcerr << L"Failed to create task. Error: " << hr << std::endl;
            }
            break;
        default:
            ex = 1;
            break;
        }
        if (ex) break;
    }
    delete[] command;
    return 0;
}