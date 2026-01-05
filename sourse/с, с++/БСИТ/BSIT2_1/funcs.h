#include <windows.h>
#include <ntsecapi.h>
#include <lm.h>
#include <sddl.h>
#include <iostream>
#include <vector>
#include <set>
#include <locale>

#pragma comment(lib, "Netapi32.lib")
#pragma comment(lib, "Advapi32.lib")

// Объявления функций
void PrintError(const std::string& message);
void PrintSID(PSID sid);
std::set<std::wstring> GetPrivileges(PSID sid);
void PrintPrivileges(const std::set<std::wstring>& privileges, const std::wstring& label);
std::set<std::wstring> GetUserLocalGroups(const std::wstring& username);
void PrintUserInfo(const USER_INFO_1* userInfo);
void PrintGroupInfo(const GROUP_INFO_1* groupInfo);
void ListUsers();
void ListGroups();
void CreateGroup(const std::wstring& groupName);
void CreateUser(const std::wstring& username, const std::wstring& password);
void AddUserToGroup(const std::wstring& username, const std::wstring& groupName);
void RemoveUserFromGroup(const std::wstring& username, const std::wstring& groupName);
void DeleteUser(const std::wstring& username);
void DeleteGroup(const std::wstring& groupName);
void AddPrivilege(const std::wstring& username, const std::wstring& privilege);
void RemovePrivilege(const std::wstring& username, const std::wstring& privilege);
bool EnablePrivilege(const std::wstring& privilege, bool enable);
