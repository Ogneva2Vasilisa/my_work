#include <iostream>
#include <locale>

#include "funcs.h"

// Функция для вывода ошибок


// Функция для получения привилегий
std::set<std::wstring> GetPrivileges(PSID sid) {
    std::set<std::wstring> privileges;
    LSA_HANDLE policyHandle;
    LSA_OBJECT_ATTRIBUTES attributes = { 0 };

    // Открываем политику LSA
    if (LsaOpenPolicy(NULL, &attributes, POLICY_ALL_ACCESS, &policyHandle) != 0) {
        PrintError("Failed to open LSA policy");
        return privileges;
    }

    // Получаем привилегии для SID
    PLSA_UNICODE_STRING lsaPrivileges = nullptr;
    ULONG privilegeCount = 0;
    if (LsaEnumerateAccountRights(policyHandle, sid, &lsaPrivileges, &privilegeCount) == 0) {
        for (ULONG i = 0; i < privilegeCount; i++) {
            privileges.insert(lsaPrivileges[i].Buffer);
        }
        LsaFreeMemory(lsaPrivileges);
    }
    

    // Закрываем политику LSA
    LsaClose(policyHandle);
    return privileges;
}

// Функция для вывода привилегий
void PrintPrivileges(const std::set<std::wstring>& privileges, const std::wstring& label) {
    std::wcout << label << std::endl;
    for (const auto& privilege : privileges) {
        std::wcout << L"  " << privilege << std::endl;
    }
}

// Функция для получения списка локальных групп пользователя
std::set<std::wstring> GetUserLocalGroups(const std::wstring& username) {
    std::set<std::wstring> groups;
    LPLOCALGROUP_USERS_INFO_0 pBuf = nullptr;
    DWORD dwLevel = 0;
    DWORD dwPrefMaxLen = MAX_PREFERRED_LENGTH;
    DWORD dwEntriesRead = 0;
    DWORD dwTotalEntries = 0;
    NET_API_STATUS nStatus;

    // Получаем список локальных групп пользователя
    nStatus = NetUserGetLocalGroups(NULL, username.c_str(), dwLevel, LG_INCLUDE_INDIRECT, (LPBYTE*)&pBuf, dwPrefMaxLen, &dwEntriesRead, &dwTotalEntries);

    if (nStatus == NERR_Success) {
        for (DWORD i = 0; i < dwEntriesRead; i++) {
            groups.insert(pBuf[i].lgrui0_name);
        }
    }
    else {
        PrintError("Failed to get local groups for user");
    }

    if (pBuf != nullptr) {
        NetApiBufferFree(pBuf);
    }
    return groups;
}

// Функция для вывода информации о пользователе
void PrintUserInfo(const USER_INFO_1* userInfo) {
    std::wcout << L"Username: " << userInfo->usri1_name << std::endl;

    // Получаем SID пользователя
    PSID sid = nullptr;
    DWORD sidSize = 0;
    WCHAR domainName[256];
    DWORD domainNameSize = sizeof(domainName) / sizeof(domainName[0]);
    SID_NAME_USE sidType;

    // Первый вызов для определения размера SID
    if (!LookupAccountName(NULL, userInfo->usri1_name, sid, &sidSize, domainName, &domainNameSize, &sidType)) {
        if (GetLastError() == ERROR_INSUFFICIENT_BUFFER) {
            sid = (PSID)malloc(sidSize);
            if (sid) {
                // Второй вызов для получения SID
                if (LookupAccountName(NULL, userInfo->usri1_name, sid, &sidSize, domainName, &domainNameSize, &sidType)) {
                    PrintSID(sid);

                    // Получаем привилегии пользователя
                    auto userPrivileges = GetPrivileges(sid);
                    PrintPrivileges(userPrivileges, L"User privileges:");

                    // Получаем группы пользователя
                    auto userGroups = GetUserLocalGroups(userInfo->usri1_name);
                    std::wcout << L"User is a member of the following local groups:" << std::endl;
                    for (const auto& group : userGroups) {
                        std::wcout << L"  " << group << std::endl;
                    }

                    // Получаем привилегии групп
                    std::set<std::wstring> inheritedPrivileges;
                    for (const auto& group : userGroups) {
                        PSID groupSid = nullptr;
                        DWORD groupSidSize = 0;
                        if (!LookupAccountName(NULL, group.c_str(), groupSid, &groupSidSize, domainName, &domainNameSize, &sidType)) {
                            if (GetLastError() == ERROR_INSUFFICIENT_BUFFER) {
                                groupSid = (PSID)malloc(groupSidSize);
                                if (groupSid) {
                                    if (LookupAccountName(NULL, group.c_str(), groupSid, &groupSidSize, domainName, &domainNameSize, &sidType)) {
                                        auto groupPrivileges = GetPrivileges(groupSid);
                                        inheritedPrivileges.insert(groupPrivileges.begin(), groupPrivileges.end());
                                    }
                                    free(groupSid);
                                }
                            }
                        }
                    }

                    // Выводим унаследованные привилегии
                    PrintPrivileges(inheritedPrivileges, L"Inherited privileges from groups:");
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
    std::wcout << std::endl;
}

// Функция для вывода информации о группе
void PrintGroupInfo(const GROUP_INFO_1* groupInfo) {
    std::wcout << L"Group Name: " << groupInfo->grpi1_name << std::endl;

    // Получаем SID группы
    PSID sid = nullptr;
    DWORD sidSize = 0;
    WCHAR domainName[256];
    DWORD domainNameSize = sizeof(domainName) / sizeof(domainName[0]);
    SID_NAME_USE sidType;

    // Первый вызов для определения размера SID
    if (!LookupAccountName(NULL, groupInfo->grpi1_name, sid, &sidSize, domainName, &domainNameSize, &sidType)) {
        if (GetLastError() == ERROR_INSUFFICIENT_BUFFER) {
            sid = (PSID)malloc(sidSize);
            if (sid) {
                // Второй вызов для получения SID
                if (LookupAccountName(NULL, groupInfo->grpi1_name, sid, &sidSize, domainName, &domainNameSize, &sidType)) {
                    PrintSID(sid);
                    auto groupPrivileges = GetPrivileges(sid);
                    PrintPrivileges(groupPrivileges, L"Group privileges:");
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
    std::wcout << std::endl;
}

// Функция для вывода списка пользователей
void ListUsers() {
    LPUSER_INFO_1 pBuf = NULL;
    DWORD dwLevel = 1;
    DWORD dwPrefMaxLen = MAX_PREFERRED_LENGTH;
    DWORD dwEntriesRead = 0;
    DWORD dwTotalEntries = 0;
    NET_API_STATUS nStatus;

    // Получаем список пользователей
    nStatus = NetUserEnum(NULL, dwLevel, FILTER_NORMAL_ACCOUNT, (LPBYTE*)&pBuf, dwPrefMaxLen, &dwEntriesRead, &dwTotalEntries, NULL);

    if (nStatus == NERR_Success) {
        for (DWORD i = 0; i < dwEntriesRead; i++) {
            PrintUserInfo(&pBuf[i]);
        }
    }
    else {
        PrintError("Failed to enumerate users");
    }

    if (pBuf != NULL) {
        NetApiBufferFree(pBuf);
    }
}

// Функция для вывода списка групп
void ListGroups() {
    LPGROUP_INFO_1 pBuf;
    DWORD dwLevel = 1;
    DWORD dwPrefMaxLen = MAX_PREFERRED_LENGTH;
    DWORD dwEntriesRead = 0;
    DWORD dwTotalEntries = 0;
    NET_API_STATUS nStatus;

    // Получаем список групп
    nStatus = NetLocalGroupEnum(
        NULL,           // Имя сервера (NULL для локального компьютера)
        dwLevel,        // Уровень информации (1 для GROUP_INFO_1)
        (LPBYTE*)&pBuf, // Буфер для данных
        dwPrefMaxLen,   // Предпочтительная длина буфера
        &dwEntriesRead, // Количество прочитанных записей
        &dwTotalEntries,// Общее количество записей
        NULL            // Resume handle (не используется)
    );

    if (nStatus == NERR_Success) {
        for (DWORD i = 0; i < dwEntriesRead; i++) {
            PrintGroupInfo(&pBuf[i]);
        }
    }
    else {
        PrintError("Failed to enumerate groups");
    }

    if (pBuf != NULL) {
        NetApiBufferFree(pBuf);
    }
}

void help() {
    std::wcout << L"1 name          - создать группу name" << std::endl;
    std::wcout << L"2 name pass     - создать пользователя name с паролем pass" << std::endl;
    std::wcout << L"3 name1 name2   - включить пользователя name1 в группу name2" << std::endl;
    std::wcout << L"4 name1 name2   - исключить пользователя name1 из группы name2" << std::endl;
    std::wcout << L"5 name          - удалить пользователя name" << std::endl;
    std::wcout << L"6 name          - удалить группу name" << std::endl;
    std::wcout << L"7 name priv     - добавить пользователю name привелегию priv" << std::endl;
    std::wcout << L"8 name priv     - удалить у name привелегию priv" << std::endl;
    std::wcout << L"9 name priv     - включить у name привелегию priv" << std::endl;
    std::wcout << L"10 name priv    - выключить у name привелегию priv" << std::endl;
    std::wcout << L"11              - вывести список пользователей" << std::endl;
    std::wcout << L"12              - вывести список групп" << std::endl;
    std::wcout << L"13              - эта справка" << std::endl;
}

int main() {
    setlocale(LC_ALL, "Russian");
    // Вывод списка пользователей
    std::wcout << L"Listing users:" << std::endl;
    ListUsers();
    std::wcout <<std::endl;
    std::wcout <<std::endl;
    // Вывод списка групп
    std::wcout << L"Listing groups:" << std::endl;
    ListGroups();
    help();

    // Обработка команд
    int command;
    std::wstring name1, name2, priv, pass;
    while (true) {
        std::wcout << L"Введите команду: ";
        std::wcin >> command;

        switch (command) {
        case 1:
            std::wcin >> name1;
            CreateGroup(name1);
            break;
        case 2:
            std::wcin >> name1 >> pass;
            CreateUser(name1, pass);
            break;
        case 3:
            std::wcin >> name1 >> name2;
            AddUserToGroup(name1, name2);
            break;
        case 4:
            std::wcin >> name1 >> name2;
            RemoveUserFromGroup(name1, name2);
            break;
        case 5:
            std::wcin >> name1;
            DeleteUser(name1);
            break;
        case 6:
            std::wcin >> name1;
            DeleteGroup(name1);
            break;
        case 7:
            std::wcin >> name1 >> priv;
            AddPrivilege(name1, priv);
            break;
        case 8:
            std::wcin >> name1 >> priv;
            RemovePrivilege(name1, priv);
            break;
        case 9:
            std::wcin >> priv;
            if (EnablePrivilege(priv, true)) {
                std::wcout << L"Привилегия " << priv << L" включена." << std::endl;
            }
            else {
                std::wcout << L"Не удалось включить привилегию " << priv << L"." << std::endl;
            }
            break;
        case 10:
            std::wcin >> priv;
            if (EnablePrivilege(priv, false)) {
                std::wcout << L"Привилегия " << priv << L" выключена." << std::endl;
            }
            else {
                std::wcout << L"Не удалось выключить привилегию " << priv << L"." << std::endl;
            }
            break;
        case 11:
            ListUsers();
            break;
        case 12:
            ListGroups();
            break;
        case 13:
            help();
        default:
            std::wcout << L"Неизвестная команда." << std::endl;
            break;
        }
    }
    return 0;
}