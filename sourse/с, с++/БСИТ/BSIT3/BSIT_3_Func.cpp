#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <iostream>
#include <string>
#include <time.h>
#include <lm.h>
#include <aclapi.h>
#include <sddl.h>
#pragma comment(lib, "netapi32.lib")
#define WIDTH 4

using namespace std;

bool GetWinMajorMinorVersion(DWORD& major, DWORD& minor)
{
    bool bRetCode = false;
    LPBYTE pinfoRawData = 0;
    if (NERR_Success == NetWkstaGetInfo(NULL, 100, &pinfoRawData))
    {
        WKSTA_INFO_100* pworkstationInfo = (WKSTA_INFO_100*)pinfoRawData;
        major = pworkstationInfo->wki100_ver_major;
        minor = pworkstationInfo->wki100_ver_minor;
        ::NetApiBufferFree(pinfoRawData);
        bRetCode = true;
    }
    return bRetCode;
}

std::string GetWindowsVersionString()
{
    std::string     winver;
    OSVERSIONINFOEX osver;
    SYSTEM_INFO     sysInfo;
    typedef void(__stdcall* GETSYSTEMINFO) (LPSYSTEM_INFO);

    __pragma(warning(push))
        __pragma(warning(disable:4996))
        memset(&osver, 0, sizeof(osver));
    osver.dwOSVersionInfoSize = sizeof(osver);
    GetVersionEx((LPOSVERSIONINFO)&osver);
    __pragma(warning(pop))
        DWORD major = 0;
    DWORD minor = 0;
    if (GetWinMajorMinorVersion(major, minor))
    {
        osver.dwMajorVersion = major;
        osver.dwMinorVersion = minor;
    }
    else if (osver.dwMajorVersion == 6 && osver.dwMinorVersion == 2)
    {
        OSVERSIONINFOEXW osvi;
        ULONGLONG cm = 0;
        cm = VerSetConditionMask(cm, VER_MINORVERSION, VER_EQUAL);
        ZeroMemory(&osvi, sizeof(osvi));
        osvi.dwOSVersionInfoSize = sizeof(osvi);
        osvi.dwMinorVersion = 3;
        if (VerifyVersionInfoW(&osvi, VER_MINORVERSION, cm))
        {
            osver.dwMinorVersion = 3;
        }
    }

    GETSYSTEMINFO getSysInfo = (GETSYSTEMINFO)GetProcAddress(GetModuleHandle(L"kernel32.dll"), "GetNativeSystemInfo");
    if (getSysInfo == NULL)  getSysInfo = ::GetSystemInfo;
    getSysInfo(&sysInfo);

    if (osver.dwMajorVersion == 10 && osver.dwMinorVersion >= 0 && osver.wProductType != VER_NT_WORKSTATION)  winver = "Windows 10 Server";
    if (osver.dwMajorVersion == 10 && osver.dwMinorVersion >= 0 && osver.wProductType == VER_NT_WORKSTATION)  winver = "Windows 10";
    if (osver.dwMajorVersion == 6 && osver.dwMinorVersion == 3 && osver.wProductType != VER_NT_WORKSTATION)  winver = "Windows Server 2012 R2";
    if (osver.dwMajorVersion == 6 && osver.dwMinorVersion == 3 && osver.wProductType == VER_NT_WORKSTATION)  winver = "Windows 8.1";
    if (osver.dwMajorVersion == 6 && osver.dwMinorVersion == 2 && osver.wProductType != VER_NT_WORKSTATION)  winver = "Windows Server 2012";
    if (osver.dwMajorVersion == 6 && osver.dwMinorVersion == 2 && osver.wProductType == VER_NT_WORKSTATION)  winver = "Windows 8";
    if (osver.dwMajorVersion == 6 && osver.dwMinorVersion == 1 && osver.wProductType != VER_NT_WORKSTATION)  winver = "Windows Server 2008 R2";
    if (osver.dwMajorVersion == 6 && osver.dwMinorVersion == 1 && osver.wProductType == VER_NT_WORKSTATION)  winver = "Windows 7";
    if (osver.dwMajorVersion == 6 && osver.dwMinorVersion == 0 && osver.wProductType != VER_NT_WORKSTATION)  winver = "Windows Server 2008";
    if (osver.dwMajorVersion == 6 && osver.dwMinorVersion == 0 && osver.wProductType == VER_NT_WORKSTATION)  winver = "Windows Vista";
    if (osver.dwMajorVersion == 5 && osver.dwMinorVersion == 2 && osver.wProductType == VER_NT_WORKSTATION
        && sysInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64)  winver = "Windows XP x64";
    if (osver.dwMajorVersion == 5 && osver.dwMinorVersion == 2)   winver = "Windows Server 2003";
    if (osver.dwMajorVersion == 5 && osver.dwMinorVersion == 1)   winver = "Windows XP";
    if (osver.dwMajorVersion == 5 && osver.dwMinorVersion == 0)   winver = "Windows 2000";
    if (osver.dwMajorVersion < 5)   winver = "unknown";

    if (osver.wServicePackMajor != 0)
    {
        std::string sp;
        char buf[128] = { 0 };
        sp = " Service Pack ";
        sprintf_s(buf, sizeof(buf), "%hd", osver.wServicePackMajor);
        sp.append(buf);
        winver += sp;
    }

    return winver;
}

char* curtime() {
    SYSTEMTIME sm;
    GetSystemTime(&sm);
    char y[1024] = { 0 }; int u = 0;
    char buf[16];    time_t ts = 0;    struct tm t;
    localtime_s(&t,&ts);
    strftime(buf, sizeof(buf), "%z", &t);
    u = sprintf(y, "%d-%d-%d %d:%d Current timezone (POSIX): ", sm.wDay, sm.wMonth, sm.wYear, sm.wHour, sm.wMinute, buf);
    return y;
}

char* time_OS() {
    int hour, min, sec, msec = GetTickCount64();
    hour = msec / (1000 * 60 * 60);
    min = msec / (1000 * 60) - hour * 60;
    sec = (msec / 1000) - (hour * 60 * 60) - min * 60;

    char y[1024] = { 0 }; int u = 0;
    u = sprintf(y, "%d:%d:%d", hour, min, sec); 
    return y;
}

char* inf_memory() {
    MEMORYSTATUSEX statex;
    statex.dwLength = sizeof(statex);
    GlobalMemoryStatusEx(&statex);

    char y[1024] = { 0 }; int u = 0;
    u = sprintf(y, "There is %d %% of memory in use.",statex.dwMemoryLoad);
    return y;
}

string disks1() {
    char disks[26][3] = { 0 };
    DWORD dr = GetLogicalDrives();
    int i = 0; int count = 0; int n = 0;
    for (i = 0; i < 26; i++)
    {
        n = ((dr >> i) & 0x00000001);
        if (n == 1)
        {
            disks[count][0] = char(65 + i);
            disks[count][1] = ':';
            count++;
        }
    }
    if (count == 0) {
        char y[1024] = "I can't find disks :(";
        return y;
    }
    char y[26][1024] = { 0 }; int u = 0;
    string buff;
    
    for (i = 0; i < count; i++)
    {
        if (GetDriveTypeA(disks[i]) == DRIVE_FIXED)
        {
            unsigned long s, b, f, c = 0;
            GetDiskFreeSpaceA(disks[i], &s, &b, &f, &c);
            int freeSpace = (double)f * (double)s * (double)b / 1024.0 / 1024.0 / 1024.0;
            u = sprintf(y[i], "%s %d Gigabyte free\n", disks[i], freeSpace);
        }
        buff.append(y[i]);
        //buff.append("\n");
    }
    

    return buff;
}

string free_memory() {
    char disks[26][3] = { 0 };
    DWORD dr = GetLogicalDrives();
    int i = 0; int count = 0; int n = 0;
    for (i = 0; i < 26; i++)
    {
        n = ((dr >> i) & 0x00000001);
        if (n == 1) {
            disks[count][0] = char(65 + i);
            disks[count][1] = ':';
            count++;
        }
    }
    if (count == 0) {
        char y[1024] = "I can't find disks :(";
        return y;
    }
    char y[26][1024] = { 0 }; int u = 0;
    string buff;

    for (i = 0; i < count; i++)
    {
        if (GetDriveTypeA(disks[i]) == DRIVE_FIXED) 
            u = sprintf(y[i], "%s \n", disks[i]);
        
        buff.append(y[i]);
        buff.append("\n");
    }
    return buff;
}

string owner(LPCWSTR file_name) {
    string buff = "";
    char y[MAX_PATH] = { 0 };
    PSECURITY_DESCRIPTOR pSD;
    PACL pDACL;
    PSID powner;
    char* powner_s = NULL;
    ACL_SIZE_INFORMATION sizeInfo;
    GetNamedSecurityInfo(file_name, SE_FILE_OBJECT, DACL_SECURITY_INFORMATION, NULL, NULL, &pDACL, NULL, &pSD);
    GetNamedSecurityInfo(file_name, SE_FILE_OBJECT, OWNER_SECURITY_INFORMATION, &powner, NULL, NULL, NULL, &pSD);
    GetAclInformation(pDACL, &sizeInfo, sizeof(sizeInfo), AclSizeInformation);
    
    TCHAR bufName[MAX_PATH] = { 0 };
    TCHAR bufDomain[MAX_PATH] = { 0 };
    SID** pNewAceSid = NULL;
    SID_NAME_USE SidNameUse;
    LPSTR StringSid = { 0 };


    DWORD dwCbName = _countof(bufName);
    DWORD dwCbDomainName = _countof(bufDomain);
    bool bSuccess = LookupAccountSid(NULL, powner, bufName, &dwCbName, bufDomain, &dwCbDomainName, &SidNameUse);
    ConvertSidToStringSidA(powner, &StringSid);
    //printf("%ls\\%ls SID: %s", bufDomain, bufName, StringSid);
    sprintf(y, "Owner %ls      is %ls\\%ls  \n(SID: %s)", file_name, bufDomain, bufName, StringSid);
    //printf("%ls\\%ls ACE: %d, SID: %s", bufDomain, bufName, mask, StringSid);
    buff.append(y);
    buff.append("\n");
    buff.append("\0");
    return buff;
}


string rights2(LPCWSTR file_name) {
    string buff = "";
    char y[50][MAX_PATH] = { 0 };
    PSECURITY_DESCRIPTOR pSD;
    PACL pDACL = NULL;
    bool fileflag = false;
    PSID* powner = NULL;
    char* powner_s = NULL;
    ACL_SIZE_INFORMATION sizeInfo;
    
    char filename [MAX_PATH] = {0};
    WideCharToMultiByte(CP_UTF8, 0, &file_name[0], sizeof(file_name), &filename[0], sizeof(file_name), NULL, NULL);
    if (PathFileExistsA(filename)) {
        GetNamedSecurityInfo(file_name, SE_FILE_OBJECT, DACL_SECURITY_INFORMATION, NULL, NULL, &pDACL, NULL, &pSD);
    }
    else {
        HKEY mainkey = NULL;
        HKEY rKey;
        char* regname = new char[1000];
        memset(regname, 0, 1000);
        
        if (!strcmp("HKEY_LOCAL_MACHINE", filename)) {
            mainkey = HKEY_LOCAL_MACHINE;
        }
        else if (!strcmp("HKEY_CLASSES_ROOT", filename)) {
            mainkey = HKEY_CLASSES_ROOT;
        }
        else if (!strcmp("HKEY_CURRENT_USER", filename)) {
            mainkey = HKEY_CURRENT_USER;
        }
        else if (!strcmp("HKEY_USERS", filename)) {
            mainkey = HKEY_USERS;
        }
        else if (!strcmp("HKEY_CURRENT_CONFIG", filename)) {
            mainkey = HKEY_CURRENT_CONFIG;
        }

        if (RegOpenKeyExA(mainkey, filename, 0, KEY_READ, &rKey) == ERROR_SUCCESS) {
            if (filename != NULL) strcat(regname, filename);
            DWORD dwSize = 0;
            RegGetKeySecurity(rKey, DACL_SECURITY_INFORMATION, nullptr, &dwSize);
            pSD = (PSECURITY_DESCRIPTOR)LocalAlloc(LPTR, dwSize);
            RegGetKeySecurity(rKey, DACL_SECURITY_INFORMATION, pSD, &dwSize);
            BOOL bDaclPresent = FALSE;
            BOOL bDaclDefaulted = FALSE;
            GetSecurityDescriptorDacl(pSD, &bDaclPresent, &pDACL, &bDaclDefaulted);
            fileflag = true;
        }
    }
    if (pDACL != NULL) {
        GetAclInformation(pDACL, &sizeInfo, sizeof(sizeInfo), AclSizeInformation);

        for (int i = 0; i < pDACL->AceCount; i++) {
            ACE_HEADER* pAceHeader;
            GetAce(pDACL, i, (void**)&pAceHeader);
            ACCESS_ALLOWED_ACE* pACE;
            GetAce(pDACL, i, (void**)&pACE);
            SYSTEM_AUDIT_ACE* SA_Ace = (SYSTEM_AUDIT_ACE*)pACE;
            ACCESS_MASK mask = SA_Ace->Mask;

            TCHAR bufName[MAX_PATH] = { 0 };
            TCHAR bufDomain[MAX_PATH] = { 0 };
            SID* pAceSid = NULL;
            SID_NAME_USE SidNameUse;
            ACCESS_ALLOWED_ACE* pAce = (ACCESS_ALLOWED_ACE*)pAceHeader;
            pAceSid = (SID*)&pAce->SidStart;

            DWORD dwCbName = _countof(bufName);
            DWORD dwCbDomainName = _countof(bufDomain);
            bool bSuccess = LookupAccountSid(NULL, pAceSid, bufName, &dwCbName, bufDomain, &dwCbDomainName, &SidNameUse);

            if (!bSuccess) {
                printf("Failed to grab SID [%d]\n", GetLastError());
                return "";
            }
            LPSTR StringSid = nullptr;
            ConvertSidToStringSidA(pAceSid, &StringSid);

            buff += "SID:           ";
            buff += StringSid;
            buff += "\n";
            char y[MAX_PATH] = { 0 };
            sprintf(y, "Name:          %ls\\%ls \n", bufDomain, bufName);
            buff.append(y);

            buff += "ACE type       ";
            switch (pAceHeader->AceType) {
            case ACCESS_ALLOWED_ACE_TYPE:
            {
                buff += "ACCESS_ALLOWED ACE\n";
                break;
            }
            case ACCESS_DENIED_ACE_TYPE:
            {
                buff += "ACCESS_DENIED ACE\n";
                break;
            }
            case SYSTEM_AUDIT_ACE_TYPE:
            {
                buff += "SYSTEM_AUDIT ACE\n";
                break;
            }
            case SYSTEM_ALARM_ACE_TYPE:
            {
                buff += "SYSTEM_ALARM ACE\n";
                break;
            }
            default:
                buff += "Unknown ACE type\n";
                continue;
            }

            buff += "Ace Flags:     ";
            if (mask & CONTAINER_INHERIT_ACE) buff += "CONTAINER_INHERIT_ACE ";
            if (mask & OBJECT_INHERIT_ACE) buff += "OBJECT_INHERIT_ACE ";
            if (mask & INHERIT_ONLY_ACE) buff += "INHERIT_ONLY_ACE ";
            if (mask & NO_PROPAGATE_INHERIT_ACE) buff += "NO_PROPAGATE_INHERIT_ACE ";
            if (mask & INHERITED_ACE) buff += "INHERITED_ACE ";
            buff += "\n";

            buff += "Access Mask:   ";
            mask = mask & 0x0000FFFF;
            if (mask & FILE_READ_DATA) buff += "FILE_READ_DATA ";
            if (mask & FILE_LIST_DIRECTORY) buff += "FILE_LIST_DIRECTORY ";
            if (mask & FILE_WRITE_DATA) buff += "FILE_WRITE_DATA ";
            if (mask & FILE_APPEND_DATA) buff += "FILE_APPEND_DATA ";
            if (mask & FILE_READ_EA) buff += "FILE_READ_EA ";
            if (mask & FILE_WRITE_EA) buff += "FILE_WRITE_EA ";
            if (mask & FILE_EXECUTE) buff += "FILE_EXECUTE ";
            if (mask & FILE_DELETE_CHILD) buff += "FILE_DELETE_CHILD ";
            if (mask & FILE_READ_ATTRIBUTES) buff += "FILE_READ_ATTRIBUTES ";
            if (mask & FILE_WRITE_ATTRIBUTES) buff += "FILE_WRITE_ATTRIBUTES ";
            if (mask & DELETE) buff += "DELETE ";
            if (mask & READ_CONTROL) buff += "READ_CONTROL ";
            if (mask & WRITE_DAC) buff += "WRITE_DAC ";
            if (mask & WRITE_OWNER) buff += "WRITE_OWNER ";
            if (mask & SYNCHRONIZE) buff += "SYNCHRONIZE ";
            buff += "\n\n";

            LocalFree(StringSid);
        }
    }
    return buff;
}


