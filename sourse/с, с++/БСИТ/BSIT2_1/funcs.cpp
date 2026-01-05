#include "funcs.h"

void PrintError(const std::string& message) {
    std::cerr << message << " Error code: " << GetLastError() << std::endl;
}

// Функция для вывода SID
void PrintSID(PSID sid) {
    LPWSTR sidString = nullptr;
    if (ConvertSidToStringSid(sid, &sidString)) {
        std::wcout << L"SID: " << sidString << std::endl;
        LocalFree(sidString);
    }
    else {
        PrintError("Failed to convert SID to string");
    }
}

// Функция для создания группы
void CreateGroup(const std::wstring& groupName) {
    GROUP_INFO_1 groupInfo;
    groupInfo.grpi1_name = (LPWSTR)groupName.c_str();
    groupInfo.grpi1_comment = NULL;

    NET_API_STATUS nStatus = NetLocalGroupAdd(NULL, 1, (LPBYTE)&groupInfo, NULL);

    if (nStatus == NERR_Success) {
        std::wcout << L"Group " << groupName << L" created successfully." << std::endl;
    }
    else {
        PrintError("Failed to create group");
    }
}

// Функция для создания пользователя
void CreateUser(const std::wstring& username, const std::wstring& password) {
    USER_INFO_1 userInfo;
    userInfo.usri1_name = (LPWSTR)username.c_str();
    userInfo.usri1_password = (LPWSTR)password.c_str();
    userInfo.usri1_priv = USER_PRIV_USER;
    userInfo.usri1_home_dir = NULL;
    userInfo.usri1_comment = NULL;
    userInfo.usri1_flags = UF_SCRIPT;
    userInfo.usri1_script_path = NULL;

    NET_API_STATUS nStatus = NetUserAdd(NULL, 1, (LPBYTE)&userInfo, NULL);

    if (nStatus == NERR_Success) {
        std::wcout << L"User " << username << L" created successfully." << std::endl;
    }
    else {
        PrintError("Failed to create user");
    }
}

// Функция для добавления пользователя в группу
void AddUserToGroup(const std::wstring& username, const std::wstring& groupName) {
    LOCALGROUP_MEMBERS_INFO_3 lgmi;
    lgmi.lgrmi3_domainandname = (LPWSTR)username.c_str();

    NET_API_STATUS nStatus = NetLocalGroupAddMembers(NULL, groupName.c_str(), 3, (LPBYTE)&lgmi, 1);

    if (nStatus == NERR_Success) {
        std::wcout << L"User " << username << L" added to group " << groupName << L" successfully." << std::endl;
    }
    else {
        PrintError("Failed to add user to group");
    }
}

// Функция для удаления пользователя из группы
void RemoveUserFromGroup(const std::wstring& username, const std::wstring& groupName) {
    LOCALGROUP_MEMBERS_INFO_3 lgmi;
    lgmi.lgrmi3_domainandname = (LPWSTR)username.c_str();

    NET_API_STATUS nStatus = NetLocalGroupDelMembers(NULL, groupName.c_str(), 3, (LPBYTE)&lgmi, 1);

    if (nStatus == NERR_Success) {
        std::wcout << L"User " << username << L" removed from group " << groupName << L" successfully." << std::endl;
    }
    else {
        PrintError("Failed to remove user from group");
    }
}

// Функция для удаления пользователя
void DeleteUser(const std::wstring& username) {
    NET_API_STATUS nStatus = NetUserDel(NULL, username.c_str());

    if (nStatus == NERR_Success) {
        std::wcout << L"User " << username << L" deleted successfully." << std::endl;
    }
    else {
        PrintError("Failed to delete user");
    }
}

// Функция для удаления группы
void DeleteGroup(const std::wstring& groupName) {
    NET_API_STATUS nStatus = NetLocalGroupDel(NULL, groupName.c_str());

    if (nStatus == NERR_Success) {
        std::wcout << L"Group " << groupName << L" deleted successfully." << std::endl;
    }
    else {
        PrintError("Failed to delete group");
    }
}

// Функция для добавления привилегии пользователю
void AddPrivilege(const std::wstring& username, const std::wstring& privilege) {
    LSA_HANDLE policyHandle;
    LSA_OBJECT_ATTRIBUTES attributes = { 0 };

    if (LsaOpenPolicy(NULL, &attributes, POLICY_ALL_ACCESS, &policyHandle) != 0) {
        PrintError("Failed to open LSA policy");
        return;
    }

    PSID sid = nullptr;
    DWORD sidSize = 0;
    WCHAR domainName[256];
    DWORD domainNameSize = sizeof(domainName) / sizeof(domainName[0]);
    SID_NAME_USE sidType;

    if (!LookupAccountName(NULL, username.c_str(), sid, &sidSize, domainName, &domainNameSize, &sidType)) {
        if (GetLastError() == ERROR_INSUFFICIENT_BUFFER) {
            sid = (PSID)malloc(sidSize);
            if (sid) {
                if (LookupAccountName(NULL, username.c_str(), sid, &sidSize, domainName, &domainNameSize, &sidType)) {
                    LSA_UNICODE_STRING privilegeString;
                    privilegeString.Buffer = (LPWSTR)privilege.c_str();
                    privilegeString.Length = (USHORT)(privilege.size() * sizeof(WCHAR));
                    privilegeString.MaximumLength = (USHORT)((privilege.size() + 1) * sizeof(WCHAR));

                    if (LsaAddAccountRights(policyHandle, sid, &privilegeString, 1) != 0) {
                        PrintError("Failed to add privilege");
                    }
                    else {
                        std::wcout << L"Privilege " << privilege << L" added to user " << username << L" successfully." << std::endl;
                    }
                }
                else {
                    PrintError("Failed to lookup account name");
                }
                free(sid);
            }
        }
        else {
            PrintError("Failed to lookup account name");
        }
    }

    LsaClose(policyHandle);
}

// Функция для удаления привилегии пользователя
void RemovePrivilege(const std::wstring& username, const std::wstring& privilege) {
    LSA_HANDLE policyHandle;
    LSA_OBJECT_ATTRIBUTES attributes = { 0 };

    if (LsaOpenPolicy(NULL, &attributes, POLICY_ALL_ACCESS, &policyHandle) != 0) {
        PrintError("Failed to open LSA policy");
        return;
    }

    PSID sid = nullptr;
    DWORD sidSize = 0;
    WCHAR domainName[256];
    DWORD domainNameSize = sizeof(domainName) / sizeof(domainName[0]);
    SID_NAME_USE sidType;

    if (!LookupAccountName(NULL, username.c_str(), sid, &sidSize, domainName, &domainNameSize, &sidType)) {
        if (GetLastError() == ERROR_INSUFFICIENT_BUFFER) {
            sid = (PSID)malloc(sidSize);
            if (sid) {
                if (LookupAccountName(NULL, username.c_str(), sid, &sidSize, domainName, &domainNameSize, &sidType)) {
                    LSA_UNICODE_STRING privilegeString;
                    privilegeString.Buffer = (LPWSTR)privilege.c_str();
                    privilegeString.Length = (USHORT)(privilege.size() * sizeof(WCHAR));
                    privilegeString.MaximumLength = (USHORT)((privilege.size() + 1) * sizeof(WCHAR));

                    if (LsaRemoveAccountRights(policyHandle, sid, FALSE, &privilegeString, 1) != 0) {
                        PrintError("Failed to remove privilege");
                    }
                    else {
                        std::wcout << L"Privilege " << privilege << L" removed from user " << username << L" successfully." << std::endl;
                    }
                }
                else {
                    PrintError("Failed to lookup account name");
                }
                free(sid);
            }
        }
        else {
            PrintError("Failed to lookup account name");
        }
    }

    LsaClose(policyHandle);
}

// Функция для включения привилегии пользователя
void EnablePrivilege(const std::wstring& username, const std::wstring& privilege) {
    // Включение привилегии требует работы с токеном пользователя, что сложнее и требует дополнительных прав.
    // Этот пример не реализует включение/выключение привилегий в токене.
    std::wcout << L"Enabling privileges is not implemented in this example." << std::endl;
}

// Функция для выключения привилегии пользователя
void DisablePrivilege(const std::wstring& username, const std::wstring& privilege) {
    // Аналогично включению, выключение привилегий требует работы с токеном пользователя.
    std::wcout << L"Disabling privileges is not implemented in this example." << std::endl;
};

// Функция для включения или выключения привилегии
bool SetPrivilege(HANDLE hToken, const std::wstring& privilege, bool enable) {
    TOKEN_PRIVILEGES tp;
    LUID luid;

    // Получаем LUID для привилегии
    if (!LookupPrivilegeValue(NULL, privilege.c_str(), &luid)) {
        PrintError("LookupPrivilegeValue error");
        return false;
    }

    tp.PrivilegeCount = 1;
    tp.Privileges[0].Luid = luid;
    tp.Privileges[0].Attributes = enable ? SE_PRIVILEGE_ENABLED : 0;

    // Изменяем привилегии токена
    if (!AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(TOKEN_PRIVILEGES), NULL, NULL)) {
        PrintError("AdjustTokenPrivileges error");
        return false;
    }

    if (GetLastError() == ERROR_NOT_ALL_ASSIGNED) {
        PrintError("The token does not have the specified privilege.");
        return false;
    }

    return true;
}

// Функция для включения или выключения привилегии в текущем процессе
bool EnablePrivilege(const std::wstring& privilege, bool enable) {
    HANDLE hToken;
    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) {
        PrintError("OpenProcessToken error");
        return false;
    }

    bool result = SetPrivilege(hToken, privilege, enable);
    CloseHandle(hToken);
    return result;
}