#include <windows.h>
#include <taskschd.h>
#include <comdef.h>
#include <iostream>
#pragma comment(lib, "taskschd.lib")
#pragma comment(lib, "comsupp.lib")

int wmain(int argc, wchar_t* argv[]) {
    HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
    if (FAILED(hr)) {
        std::wcout << L"CoInitializeEx failed: " << hr << std::endl;
        return 1;
    }

    hr = CoInitializeSecurity(NULL, -1, NULL, NULL,
        RPC_C_AUTHN_LEVEL_PKT_PRIVACY, RPC_C_IMP_LEVEL_IMPERSONATE,
        NULL, 0, NULL);

    if (FAILED(hr)) {
        std::wcout << L"CoInitializeSecurity failed: " << hr << std::endl;
        CoUninitialize();
        return 1;
    }

    ITaskService* pService = NULL;
    hr = CoCreateInstance(CLSID_TaskScheduler, NULL, CLSCTX_INPROC_SERVER,
        IID_ITaskService, (void**)&pService);

    if (FAILED(hr)) {
        std::wcout << L"Failed to create ITaskService: " << hr << std::endl;
        CoUninitialize();
        return 1;
    }

    hr = pService->Connect(_variant_t(), _variant_t(), _variant_t(), _variant_t());
    if (FAILED(hr)) {
        std::wcout << L"ITaskService::Connect failed: " << hr << std::endl;
        pService->Release();
        CoUninitialize();
        return 1;
    }

    ITaskFolder* pRootFolder = NULL;
    hr = pService->GetFolder(_bstr_t(L"\\"), &pRootFolder);
    if (FAILED(hr)) {
        std::wcout << L"Cannot get Root Folder pointer: " << hr << std::endl;
        pService->Release();
        CoUninitialize();
        return 1;
    }

    // Удаляем задачу, если она уже существует
    pRootFolder->DeleteTask(_bstr_t(L"MySoftwareInventoryTask"), 0);

    ITaskDefinition* pTask = NULL;
    hr = pService->NewTask(0, &pTask);
    if (FAILED(hr)) {
        std::wcout << L"Failed to create task definition: " << hr << std::endl;
        pRootFolder->Release();
        pService->Release();
        CoUninitialize();
        return 1;
    }

    // Информация о задаче
    IRegistrationInfo* pRegInfo = NULL;
    pTask->get_RegistrationInfo(&pRegInfo);
    pRegInfo->put_Author(_bstr_t(L""));
    pRegInfo->put_Description(_bstr_t(L"Собирает список установленного ПО"));
    pRegInfo->Release();

    // Триггеры
    ITriggerCollection* pTriggerCollection = NULL;
    pTask->get_Triggers(&pTriggerCollection);

    // 1. Триггер по запуску системы
    ITrigger* pBootTrigger = NULL;
    pTriggerCollection->Create(TASK_TRIGGER_BOOT, &pBootTrigger);
    pBootTrigger->Release();

    // 2. Триггер по времени (каждый день в 10:00)
    ITrigger* pTimeTrigger = NULL;
    pTriggerCollection->Create(TASK_TRIGGER_DAILY, &pTimeTrigger);
    IDailyTrigger* pDaily = NULL;
    pTimeTrigger->QueryInterface(IID_IDailyTrigger, (void**)&pDaily);
    pDaily->put_StartBoundary(_bstr_t(L"2024-06-20T10:00:00"));
    pDaily->put_DaysInterval(1);
    pDaily->Release();
    pTimeTrigger->Release();
    pTriggerCollection->Release();

    // Действие — запуск вашей программы (например, SoftwareList.exe)
    IActionCollection* pActionCollection = NULL;
    pTask->get_Actions(&pActionCollection);
    IAction* pAction = NULL;
    pActionCollection->Create(TASK_ACTION_EXEC, &pAction);
    IExecAction* pExecAction = NULL;
    pAction->QueryInterface(IID_IExecAction, (void**)&pExecAction);
    pExecAction->put_Path(_bstr_t(L"C:\\Users\\uuu3\\Desktop\\SoftwareList.exe"));
    pExecAction->Release();
    pAction->Release();
    pActionCollection->Release();

    // Настройки задачи
    ITaskSettings* pSettings = NULL;
    pTask->get_Settings(&pSettings);
    pSettings->put_StartWhenAvailable(VARIANT_TRUE);
    pSettings->put_Enabled(VARIANT_TRUE);
    pSettings->Release();

    // Регистрируем задачу
    IRegisteredTask* pRegisteredTask = NULL;
    hr = pRootFolder->RegisterTaskDefinition(
        _bstr_t(L"MySoftwareInventoryTask"),
        pTask,
        TASK_CREATE_OR_UPDATE,
        _variant_t(L""), // Запускать от текущего пользователя
        _variant_t(L""),
        TASK_LOGON_INTERACTIVE_TOKEN,
        _variant_t(L""),
        &pRegisteredTask);

    if (FAILED(hr)) {
        std::wcout << L"RegisterTaskDefinition failed: " << hr << std::endl;
    }
    else {
        std::wcout << L"Task created successfully." << std::endl;
        pRegisteredTask->Release();
    }

    pTask->Release();
    pRootFolder->Release();
    pService->Release();
    CoUninitialize();
    return 0;
}
