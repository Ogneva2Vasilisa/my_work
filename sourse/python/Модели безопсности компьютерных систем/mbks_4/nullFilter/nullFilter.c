
#include <fltKernel.h>
#include <dontuse.h>
#include <suppress.h>
#include <string.h>
#include <stdlib.h>

#include <ntifs.h>
#include <wdm.h>
#include <ntstrsafe.h>
NTSYSAPI PCHAR PsGetProcessImageFileName(PEPROCESS Process);

#pragma prefast(disable:__WARNING_ENCODE_MEMBER_FUNCTION_POINTER, "Not valid for kernel mode drivers")

ULONG_PTR OperationStatusCtx = 1;

#pragma prefast(disable:__WARNING_ENCODE_MEMBER_FUNCTION_POINTER, "Not valid for kernel mode drivers")

//---------------------------------------------------------------------------
//      Global variables
//---------------------------------------------------------------------------


typedef struct _NULL_FILTER_DATA {

    //
    //  The filter handle that results from a call to
    //  FltRegisterFilter.
    //

    PFLT_FILTER FilterHandle;

} NULL_FILTER_DATA, * PNULL_FILTER_DATA;

#define PTDBG_TRACE_ROUTINES            0x00000001
#define PTDBG_TRACE_OPERATION_STATUS    0x00000002

ULONG gTraceFlags = 0;

#define PT_DBG_PRINT( _dbgLevel, _string )          \
    (FlagOn(gTraceFlags,(_dbgLevel)) ?              \
        DbgPrint _string :                          \
        ((int)0))


/*************************************************************************
    Prototypes for the startup and unload routines used for
    this Filter.

    Implementation in nullFilter.c
*************************************************************************/

DRIVER_INITIALIZE DriverEntry;
NTSTATUS
DriverEntry(
    _In_ PDRIVER_OBJECT DriverObject,
    _In_ PUNICODE_STRING RegistryPath
);

NTSTATUS
NullUnload(
    _In_ FLT_FILTER_UNLOAD_FLAGS Flags
);

NTSTATUS
NullQueryTeardown(
    _In_ PCFLT_RELATED_OBJECTS FltObjects,
    _In_ FLT_INSTANCE_QUERY_TEARDOWN_FLAGS Flags
);

FLT_PREOP_CALLBACK_STATUS
PtPreOperationPassThrough(
    _Inout_ PFLT_CALLBACK_DATA Data,
    _In_ PCFLT_RELATED_OBJECTS FltObjects,
    _Flt_CompletionContext_Outptr_ PVOID* CompletionContext
);
BOOLEAN
PtDoRequestOperationStatus(
    _In_ PFLT_CALLBACK_DATA Data
);
VOID
PtOperationStatusCallback(
    _In_ PCFLT_RELATED_OBJECTS FltObjects,
    _In_ PFLT_IO_PARAMETER_BLOCK ParameterSnapshot,
    _In_ NTSTATUS OperationStatus,
    _In_ PVOID RequesterContext
);

FLT_POSTOP_CALLBACK_STATUS
PtPostOperationPassThrough(
    _Inout_ PFLT_CALLBACK_DATA Data,
    _In_ PCFLT_RELATED_OBJECTS FltObjects,
    _In_opt_ PVOID CompletionContext,
    _In_ FLT_POST_OPERATION_FLAGS Flags
);
VOID ReadConfigFile();
NTSTATUS CharToWchar(
    _In_ PCHAR AnsiString,
    _Out_ PWCHAR* UnicodeString,
    _Out_opt_ PULONG UnicodeSizeInBytes
);

//
//  Structure that contains all the global data structures
//  used throughout NullFilter.
//

NULL_FILTER_DATA NullFilterData;

//
//  Assign text sections for each routine.
//

#ifdef ALLOC_PRAGMA
#pragma alloc_text(INIT, DriverEntry)
#pragma alloc_text(PAGE, NullUnload)
#pragma alloc_text(PAGE, NullQueryTeardown)
#endif

ERESOURCE g_ConfigLock; // resource to protect global config arrays
volatile LONG g_ConfigWorkItemQueued = 0; // 0 - none queued, 1 - queued/running

//WCHAR* filedeny = L"Users\\WDKRemoteUser\\Documents\\test.txt";
WCHAR* configfile = L"\\??\\C:\\Users\\WDKRemoteUser\\Documents\\conf.txt";
WCHAR* configfile_short = L"Users\\WDKRemoteUser\\Documents\\conf.txt";
WCHAR** filesdeny[50] = { 0 };
WCHAR** fileswrite[50] = { 0 };
CHAR* processes[50] = { 0 };
INT colfiles; // TODO
INT proccol;




// Prototype (добавь в секцию прототипов)
VOID ConfigDeferredRoutine(
    _In_ PFLT_DEFERRED_IO_WORKITEM WorkItem,
    _In_ PFLT_CALLBACK_DATA Data,
    _In_opt_ PVOID Context
);

// Реализация:
VOID ConfigDeferredRoutine(
    _In_ PFLT_DEFERRED_IO_WORKITEM WorkItem,
    _In_ PFLT_CALLBACK_DATA Data,
    _In_opt_ PVOID Context
)
{
    UNREFERENCED_PARAMETER(Data);
    UNREFERENCED_PARAMETER(Context);

    // mark not queued so new events can schedule another work item
    InterlockedExchange(&g_ConfigWorkItemQueued, 0);

    // optional debounce: wait briefly to coalesce quick successive saves
    // LARGE_INTEGER interval;
    // interval.QuadPart = -200 * 10000; // 200 ms
    // KeDelayExecutionThread(KernelMode, FALSE, &interval);

    // Call ReadConfigFile with the full path (this runs at PASSIVE_LEVEL)
    ReadConfigFile(configfile);

    // Free the work item allocated by FltAllocateDeferredIoWorkItem
    FltFreeDeferredIoWorkItem(WorkItem);
}



/*BOOLEAN wstrncmp(WCHAR* str1, WCHAR* str2, INT len) {
    if (len == 0 || str1[0] == 0) return 1;
    for (INT i = 0; i < len-1; i++) {
        if (str1[i] != str2[i]) return 1;
    }
    return 0;
}*/

CONST FLT_OPERATION_REGISTRATION Callbacks[] = {
    { IRP_MJ_CREATE,
      0,
      PtPreOperationPassThrough,
      PtPostOperationPassThrough },
    { IRP_MJ_WRITE,
      0,
      PtPreOperationPassThrough,
      PtPostOperationPassThrough },
    { IRP_MJ_SET_INFORMATION,
      0,
      PtPreOperationPassThrough,
      PtPostOperationPassThrough },
    { IRP_MJ_READ,
      0,
      PtPreOperationPassThrough,
      PtPostOperationPassThrough },

    { IRP_MJ_OPERATION_END }
};

//
//  This defines what we want to filter with FltMgr
//

CONST FLT_REGISTRATION FilterRegistration = {

    sizeof(FLT_REGISTRATION),         //  Size
    FLT_REGISTRATION_VERSION,           //  Version
    0,                                  //  Flags

    NULL,                               //  Context
    Callbacks,                               //  Operation callbacks

    NullUnload,                         //  FilterUnload

    NULL,                               //  InstanceSetup
    NullQueryTeardown,                  //  InstanceQueryTeardown
    NULL,                               //  InstanceTeardownStart
    NULL,                               //  InstanceTeardownComplete

    NULL,                               //  GenerateFileName
    NULL,                               //  GenerateDestinationFileName
    NULL                                //  NormalizeNameComponent

};

NTSTATUS CharToWchar(
    _In_ PCHAR AnsiString,
    _Out_ PWCHAR* UnicodeString,
    _Out_opt_ PULONG UnicodeSizeInBytes
)
{
    NTSTATUS status = STATUS_SUCCESS;
    ANSI_STRING ansiStr;
    UNICODE_STRING uniStr;
    PWCHAR buffer = NULL;

    *UnicodeString = NULL;
    if (UnicodeSizeInBytes) *UnicodeSizeInBytes = 0;

    // �������������� ANSI_STRING
    RtlInitAnsiString(&ansiStr, AnsiString);

    // ��������� ��������� ������ ������
    ULONG maxLength = 0;
    maxLength = RtlAnsiStringToUnicodeSize(&ansiStr);

    // �������� ������ ��� Unicode ������
    buffer = ExAllocatePool2(
        POOL_FLAG_NON_PAGED,
        maxLength,
        'wchr'
    );

    if (!buffer) {
        return STATUS_INSUFFICIENT_RESOURCES;
    }

    // �������������� UNICODE_STRING � ����� �������
    uniStr.Buffer = buffer;
    uniStr.MaximumLength = (USHORT)maxLength;

    // ��������������� ������
    status = RtlAnsiStringToUnicodeString(&uniStr, &ansiStr, FALSE);
    if (!NT_SUCCESS(status)) {
        KdPrint(("RtlAnsiStringToUnicodeString failed: 0x%08X\n", status));
        ExFreePool(buffer);
        return status;
    }

    *UnicodeString = buffer;
    if (UnicodeSizeInBytes) *UnicodeSizeInBytes = uniStr.Length;

    return status;
}


VOID ReadConfigFile(WCHAR* config) {
    NTSTATUS status = STATUS_SUCCESS;
    OBJECT_ATTRIBUTES objectAttributes;
    IO_STATUS_BLOCK ioStatusBlock;
    HANDLE fileHandle = NULL;
    FILE_STANDARD_INFORMATION fileInfo;
    PCHAR buffer = NULL;
    UNICODE_STRING filePath;

    RtlInitUnicodeString(&filePath, config);

    InitializeObjectAttributes(
        &objectAttributes,
        &filePath,
        OBJ_KERNEL_HANDLE | OBJ_CASE_INSENSITIVE,
        NULL,
        NULL
    );

    status = ZwCreateFile(
        &fileHandle,
        FILE_READ_DATA | SYNCHRONIZE,
        &objectAttributes,
        &ioStatusBlock,
        NULL,
        FILE_ATTRIBUTE_NORMAL,
        FILE_SHARE_READ,
        FILE_OPEN,
        FILE_SYNCHRONOUS_IO_NONALERT,
        NULL,
        0
    );

    if (!NT_SUCCESS(status)) {
        if (fileHandle) ZwClose(fileHandle);
        return;
    }

    status = ZwQueryInformationFile(
        fileHandle,
        &ioStatusBlock,
        &fileInfo,
        sizeof(fileInfo),
        FileStandardInformation
    );

    if (!NT_SUCCESS(status) || fileInfo.EndOfFile.QuadPart == 0) {
        ZwClose(fileHandle);
        return;
    }

    if (fileInfo.EndOfFile.QuadPart > MAXULONG) {
        ZwClose(fileHandle);
        return;
    }

    buffer = ExAllocatePool2(
        POOL_FLAG_NON_PAGED,
        (ULONG)fileInfo.EndOfFile.QuadPart + 1,
        'cnfZ'
    );

    if (!buffer) {
        ZwClose(fileHandle);
        return;
    }

    status = ZwReadFile(
        fileHandle,
        NULL,
        NULL,
        NULL,
        &ioStatusBlock,
        buffer,
        (ULONG)fileInfo.EndOfFile.QuadPart,
        NULL,
        NULL
    );

    ZwClose(fileHandle);

    if (!NT_SUCCESS(status)) {
        ExFreePoolWithTag(buffer, 'cnfZ');
        return;
    }

    //
    // Захватываем ресурс один раз — весь обмен глобальными структурами в этой секции
    //
    KeEnterCriticalRegion();
    ExAcquireResourceExclusiveLite(&g_ConfigLock, TRUE);

    // Очистка старой конфигурации
    for (INT _i = 0; _i < proccol; _i++) {
        if (processes[_i]) {
            ExFreePoolWithTag(processes[_i], 'cnfZ');
            processes[_i] = NULL;
        }
    }
    for (INT _i = 0; _i < 50; _i++) {
        if (filesdeny[_i]) {
            INT _j = 0;
            while (filesdeny[_i][_j] != NULL) {
                ExFreePoolWithTag(filesdeny[_i][_j], 'wchr');
                _j++;
            }
            ExFreePoolWithTag(filesdeny[_i], 'arrW');
            filesdeny[_i] = NULL;
        }
        if (fileswrite[_i]) {
            INT _j = 0;
            while (fileswrite[_i][_j] != NULL) {
                ExFreePoolWithTag(fileswrite[_i][_j], 'wchr');
                _j++;
            }
            ExFreePoolWithTag(fileswrite[_i], 'arrW');
            fileswrite[_i] = NULL;
        }
    }
    proccol = 0;

    //
    // Парсинг: осторожно, с проверками
    //
    PCHAR pbuffer = buffer;
    CHAR flag = 0;

    while (1) {
        PCHAR nextline = strchr(pbuffer, '\n');
        if (nextline == NULL) {
            // некорректный формат или конец — выйти
            break;
        }

        SIZE_T linelen = (SIZE_T)(nextline - pbuffer);
        if (linelen == 0) {
            // пустая строка — сместиться и продолжить
            pbuffer = nextline + 1;
            continue;
        }

        // аллоцируем procname
        PCHAR procname = ExAllocatePool2(POOL_FLAG_NON_PAGED, (ULONG)(linelen + 1), 'cnfZ');
        if (!procname) {
            KdPrint(("ReadConfigFile: ExAllocatePool2(procname) failed\n"));
            ExReleaseResourceLite(&g_ConfigLock);
            KeLeaveCriticalRegion();

            ExFreePoolWithTag(buffer, 'cnfZ');
            return;
        }

        // копируем имя процесса и NUL-терминируем
        RtlZeroMemory(procname, linelen + 1);
        RtlCopyMemory(procname, pbuffer, linelen);
        // очистим возможные \r\t
        for (SIZE_T k = 0; k < linelen; k++) {
            if (procname[k] == '\r' || procname[k] == '\t') procname[k] = 0;
        }

        // сохраняем имя
        if (proccol >= 50) {
            // слишком много записей — откажемся
            ExFreePoolWithTag(procname, 'cnfZ');
            KdPrint(("ReadConfigFile: too many processes in config, skipping rest\n"));
            break;
        }
        processes[proccol] = procname;

        // двигаемся к следующей строке после имени
        pbuffer = nextline + 1;

        // теперь парсим блок до '---' для текущего процесса
        while (1) {
            if (strncmp(pbuffer, "---", 3) == 0) {
                // если следуют только --- (и возможно \n), то конец блока
                if (pbuffer[3] == 0 || pbuffer[3] == '\n' || pbuffer[3] == '\r') {
                    // завершаем запись процесса
                    proccol++;
                    // сдвигаем pbuffer за разделитель
                    // если есть дополнительный разделитель формата "---\r\n" или "---\n "
                    // постараемся сдвинуть на минимум 1
                    pbuffer += 3;
                    if (*pbuffer == '\r') pbuffer++;
                    if (*pbuffer == '\n') pbuffer++;
                    break;
                }
                else {
                    // если есть дополнителные символы после --- (неожиданно), просто пропускаем
                    pbuffer += 3;
                    continue;
                }
            }

            // ожидаем шаблон "ID:..."
            PCHAR colon = strchr(pbuffer, ':');
            if (colon == NULL) {
                KdPrint(("ReadConfigFile: malformed ID: line\n"));
                ExReleaseResourceLite(&g_ConfigLock);
                KeLeaveCriticalRegion();

                ExFreePoolWithTag(buffer, 'cnfZ');
                return;
            }

            SIZE_T idlen = (SIZE_T)(colon - pbuffer);
            if (idlen == 0 || idlen >= sizeof(char[10])) {
                KdPrint(("ReadConfigFile: invalid ID length\n"));
                ExReleaseResourceLite(&g_ConfigLock);
                KeLeaveCriticalRegion();

                ExFreePoolWithTag(buffer, 'cnfZ');
                return;
            }
            CHAR ID[16] = { 0 };
            RtlCopyMemory(ID, pbuffer, idlen);
            ID[idlen] = 0;

            // advance to data after colon
            PCHAR dataStart = colon + 1;
            // find end of data (newline)
            PCHAR dataEnd = strchr(dataStart, '\n');
            if (dataEnd == NULL) {
                KdPrint(("ReadConfigFile: malformed data line (no newline)\n"));
                ExReleaseResourceLite(&g_ConfigLock);
                KeLeaveCriticalRegion();

                ExFreePoolWithTag(buffer, 'cnfZ');
                return;
            }

            // process ID
            if (strncmp(ID, "Read", 4) == 0) {
                // кол-во файлов
                CHAR numfiles_s[16] = { 0 };
                SIZE_T numlen = (SIZE_T)(dataEnd - dataStart);
                if (numlen >= sizeof(numfiles_s)) { KdPrint(("ReadConfigFile: numfiles too long\n")); ExReleaseResourceLite(&g_ConfigLock);
                KeLeaveCriticalRegion();

                ExFreePoolWithTag(buffer, 'cnfZ');
                return;
                }
                RtlCopyMemory(numfiles_s, dataStart, (SIZE_T)numlen);
                INT numfiles = atoi(numfiles_s);
                if (numfiles < 0) numfiles = 0;
                if (numfiles > 200) { KdPrint(("ReadConfigFile: numfiles unreasonable\n")); ExReleaseResourceLite(&g_ConfigLock);
                KeLeaveCriticalRegion();

                ExFreePoolWithTag(buffer, 'cnfZ');
                return;
                }

                // выделяем массив указателей (+1 для NULL)
                filesdeny[proccol] = ExAllocatePool2(POOL_FLAG_NON_PAGED, (numfiles + 1) * sizeof(WCHAR*), 'arrW');
                if (!filesdeny[proccol]) { KdPrint(("ReadConfigFile: alloc filesdeny failed\n")); ExReleaseResourceLite(&g_ConfigLock);
                KeLeaveCriticalRegion();

                ExFreePoolWithTag(buffer, 'cnfZ');
                return;
                }
                RtlZeroMemory(filesdeny[proccol], (numfiles + 1) * sizeof(WCHAR*));

                // далее переходим за newline и читаем имена (зависит от формата: пробел/новая строка)
                pbuffer = dataEnd + 1;
                for (INT j = 0; j < numfiles; j++) {
                    PCHAR fnameEnd = strchr(pbuffer, (j == numfiles - 1) ? '\n' : ' ');
                    if (fnameEnd == NULL) { KdPrint(("ReadConfigFile: filename parse fail\n")); ExReleaseResourceLite(&g_ConfigLock);
                    KeLeaveCriticalRegion();

                    ExFreePoolWithTag(buffer, 'cnfZ');
                    return;
                    }
                    SIZE_T fnlen = (SIZE_T)(fnameEnd - pbuffer);
                    CHAR fname[256] = { 0 };
                    if (fnlen >= sizeof(fname)) { KdPrint(("ReadConfigFile: filename too long\n")); ExReleaseResourceLite(&g_ConfigLock);
                    KeLeaveCriticalRegion();

                    ExFreePoolWithTag(buffer, 'cnfZ');
                    return;
                    }
                    RtlCopyMemory(fname, pbuffer, fnlen);
                    // trim
                    for (SIZE_T kk = 0; kk < fnlen; kk++) if (fname[kk] == '\r' || fname[kk] == '\t') fname[kk] = 0;

                    // конвертируем
                    ULONG wsz = 0;
                    filesdeny[proccol][j] = NULL;
                    if (!NT_SUCCESS(CharToWchar(fname, &filesdeny[proccol][j], &wsz))) {
                        KdPrint(("ReadConfigFile: CharToWchar failed for deny file\n"));
                        ExReleaseResourceLite(&g_ConfigLock);
                        KeLeaveCriticalRegion();

                        ExFreePoolWithTag(buffer, 'cnfZ');
                        return;
                    }

                    // advance pbuffer to next token
                    if (j < numfiles - 1) pbuffer = fnameEnd + 1;
                    else pbuffer = fnameEnd + 1;
                }
                // NULL-terminate array
                filesdeny[proccol][numfiles] = NULL;
            }
            else if (strncmp(ID, "Write", 5) == 0) {
                // аналогично для Write
                CHAR numfiles_s[16] = { 0 };
                SIZE_T numlen = (SIZE_T)(dataEnd - dataStart);
                if (numlen >= sizeof(numfiles_s)) { KdPrint(("ReadConfigFile: numfiles too long\n")); ExReleaseResourceLite(&g_ConfigLock);
                KeLeaveCriticalRegion();

                ExFreePoolWithTag(buffer, 'cnfZ');
                return;
                }
                RtlCopyMemory(numfiles_s, dataStart, (SIZE_T)numlen);
                INT numfiles = atoi(numfiles_s);
                if (numfiles < 0) numfiles = 0;
                if (numfiles > 200) { KdPrint(("ReadConfigFile: numfiles unreasonable\n")); ExReleaseResourceLite(&g_ConfigLock);
                KeLeaveCriticalRegion();

                ExFreePoolWithTag(buffer, 'cnfZ');
                return;
                }

                fileswrite[proccol] = ExAllocatePool2(POOL_FLAG_NON_PAGED, (numfiles + 1) * sizeof(WCHAR*), 'arrW');
                if (!fileswrite[proccol]) { KdPrint(("ReadConfigFile: alloc fileswrite failed\n")); ExReleaseResourceLite(&g_ConfigLock);
                KeLeaveCriticalRegion();

                ExFreePoolWithTag(buffer, 'cnfZ');
                return;
                }
                RtlZeroMemory(fileswrite[proccol], (numfiles + 1) * sizeof(WCHAR*));

                pbuffer = dataEnd + 1;
                for (INT j = 0; j < numfiles; j++) {
                    PCHAR fnameEnd = strchr(pbuffer, (j == numfiles - 1) ? '\n' : ' ');
                    if (fnameEnd == NULL) { KdPrint(("ReadConfigFile: filename parse fail\n")); ExReleaseResourceLite(&g_ConfigLock);
                    KeLeaveCriticalRegion();

                    ExFreePoolWithTag(buffer, 'cnfZ');
                    return;
                    }
                    SIZE_T fnlen = (SIZE_T)(fnameEnd - pbuffer);
                    CHAR fname[256] = { 0 };
                    if (fnlen >= sizeof(fname)) { KdPrint(("ReadConfigFile: filename too long\n")); ExReleaseResourceLite(&g_ConfigLock);
                    KeLeaveCriticalRegion();

                    ExFreePoolWithTag(buffer, 'cnfZ');
                    return;
                    }
                    RtlCopyMemory(fname, pbuffer, fnlen);
                    for (SIZE_T kk = 0; kk < fnlen; kk++) if (fname[kk] == '\r' || fname[kk] == '\t') fname[kk] = 0;

                    ULONG wsz = 0;
                    fileswrite[proccol][j] = NULL;
                    if (!NT_SUCCESS(CharToWchar(fname, &fileswrite[proccol][j], &wsz))) {
                        KdPrint(("ReadConfigFile: CharToWchar failed for write file\n"));
                        ExReleaseResourceLite(&g_ConfigLock);
                        KeLeaveCriticalRegion();

                        ExFreePoolWithTag(buffer, 'cnfZ');
                        return;
                    }

                    if (j < numfiles - 1) pbuffer = fnameEnd + 1;
                    else pbuffer = fnameEnd + 1;
                }
                fileswrite[proccol][numfiles] = NULL;
            }
            else {
                // неизвестный ID -> пропустим строку
                pbuffer = dataEnd + 1;
            }
        } // конец внутреннего while для блока процесса

        if (flag) break;
    } // конец внешнего while

    //
    // Успешно распарсили — выходим корректно
    //
    // В случае ошибки части массивов могут быть выделены — мы не пытаемся освободить всё тут,
    // потому что мы уже очищали старую конфигурацию в начале. При ошибке можно выбрать стратегию:
    //  - если парсинг прервался, можно оставить частично заполненные новые массивы (опасно),
    //  - или освободить все, что было выделено в этой сессии и восстановить старую конфигурацию (сложнее).
    // Для простоты: если дошли до cleanup, считаем, что текущая попытка загрузки завершилась — но
    // мы всё равно должны корректно отпустить ресурс и очистить временный буфер.
    ExReleaseResourceLite(&g_ConfigLock);
    KeLeaveCriticalRegion();

    ExFreePoolWithTag(buffer, 'cnfZ');
    return;
}



/*************************************************************************
    Filter initialization and unload routines.
*************************************************************************/

NTSTATUS
DriverEntry(
    _In_ PDRIVER_OBJECT DriverObject,
    _In_ PUNICODE_STRING RegistryPath
)
/*++

Routine Description:

    This is the initialization routine for this miniFilter driver. This
    registers the miniFilter with FltMgr and initializes all
    its global data structures.

Arguments:

    DriverObject - Pointer to driver object created by the system to
        represent this driver.
    RegistryPath - Unicode string identifying where the parameters for this
        driver are located in the registry.

Return Value:

    Returns STATUS_SUCCESS.

--*/
{
    NTSTATUS status;

    UNREFERENCED_PARAMETER(RegistryPath);

    //
    //  Register with FltMgr
    //

    status = FltRegisterFilter(DriverObject,
        &FilterRegistration,
        &NullFilterData.FilterHandle);

    FLT_ASSERT(NT_SUCCESS(status));

    if (NT_SUCCESS(status)) {

        //
        //  Start filtering i/o
        //

        ExInitializeResourceLite(&g_ConfigLock);

        ReadConfigFile(configfile);

        status = FltStartFiltering(NullFilterData.FilterHandle);

        if (!NT_SUCCESS(status)) {
            FltUnregisterFilter(NullFilterData.FilterHandle);
        }
    }
    return status;
}

NTSTATUS
NullUnload(
    _In_ FLT_FILTER_UNLOAD_FLAGS Flags
)
/*++

Routine Description:

    This is the unload routine for this miniFilter driver. This is called
    when the minifilter is about to be unloaded. We can fail this unload
    request if this is not a mandatory unloaded indicated by the Flags
    parameter.

Arguments:

    Flags - Indicating if this is a mandatory unload.

Return Value:

    Returns the final status of this operation.

--*/
{
    UNREFERENCED_PARAMETER(Flags);

    PAGED_CODE();

    FltUnregisterFilter(NullFilterData.FilterHandle);

    INT i = 0;
    while (filesdeny[i] != NULL) {
        INT j = 0;
        while (filesdeny[i][j] != NULL) {
            ExFreePoolWithTag(filesdeny[i][j], 'wchr');
            j++;
        }
        ExFreePoolWithTag(filesdeny[i], 'arrW');
        i++;
    }
    i = 0;
    while (fileswrite[i] != NULL) {
        INT j = 0;
        while (fileswrite[i][j] != NULL) {
            ExFreePoolWithTag(fileswrite[i][j], 'wchr');
            j++;
        }
        ExFreePoolWithTag(fileswrite[i], 'arrW');
        i++;
    }
    i = 0;
    for (i = 0; i < proccol; i++) {
        ExFreePoolWithTag(processes[i], 'cnfZ');
    }

    while(InterlockedCompareExchange(&g_ConfigWorkItemQueued, 0, 0) == 1) {
        LARGE_INTEGER interval;
        interval.QuadPart = -10 * 1000 * 100; // 100 ms
        KeDelayExecutionThread(KernelMode, FALSE, &interval);
    }

    ExDeleteResourceLite(&g_ConfigLock);

    return STATUS_SUCCESS;
}

NTSTATUS
NullQueryTeardown(
    _In_ PCFLT_RELATED_OBJECTS FltObjects,
    _In_ FLT_INSTANCE_QUERY_TEARDOWN_FLAGS Flags
)
/*++

Routine Description:

    This is the instance detach routine for this miniFilter driver.
    This is called when an instance is being manually deleted by a
    call to FltDetachVolume or FilterDetach thereby giving us a
    chance to fail that detach request.

Arguments:

    FltObjects - Pointer to the FLT_RELATED_OBJECTS data structure containing
        opaque handles to this filter, instance and its associated volume.

    Flags - Indicating where this detach request came from.

Return Value:

    Returns the status of this operation.

--*/
{
    UNREFERENCED_PARAMETER(FltObjects);
    UNREFERENCED_PARAMETER(Flags);

    PAGED_CODE();

    return STATUS_SUCCESS;
}



FLT_PREOP_CALLBACK_STATUS
PtPreOperationPassThrough(
    _Inout_ PFLT_CALLBACK_DATA Data,
    _In_ PCFLT_RELATED_OBJECTS FltObjects,
    _Flt_CompletionContext_Outptr_ PVOID* CompletionContext
)
{
    NTSTATUS status;
    BOOLEAN used_shared_lock = FALSE;
    KIRQL currIrql = PASSIVE_LEVEL; // инициализация на всякий случай
    FLT_PREOP_CALLBACK_STATUS ret = FLT_PREOP_SUCCESS_WITH_CALLBACK;

    UNREFERENCED_PARAMETER(FltObjects);
    UNREFERENCED_PARAMETER(CompletionContext);

    PT_DBG_PRINT(PTDBG_TRACE_ROUTINES,
        ("PassThrough!PtPreOperationPassThrough: Entered\n"));

    if (!PtDoRequestOperationStatus(Data)) {
        return FLT_PREOP_SUCCESS_WITH_CALLBACK;
    }
    // above line is placeholder; we need real variable. Rewrite properly:

    PFLT_FILE_NAME_INFORMATION fileNameInfo = NULL;
    status = FltGetFileNameInformation(Data, FLT_FILE_NAME_NORMALIZED, &fileNameInfo);
    if (!NT_SUCCESS(status) || fileNameInfo == NULL) {
        PT_DBG_PRINT(PTDBG_TRACE_OPERATION_STATUS,
            ("PassThrough!PtPreOperationPassThrough: FltGetFileNameInformation Failed, status=%08x\n",
                status));
        return FLT_PREOP_SUCCESS_NO_CALLBACK;
    }

    // Получим имя процесса и текущий IRQL
    PCHAR ProcessName = PsGetProcessImageFileName(PsGetCurrentProcess());
    KdPrintEx((DPFLTR_IHVDRIVER_ID, DPFLTR_INFO_LEVEL, "Process Name: %s\n", ProcessName));

    currIrql = KeGetCurrentIrql();

    // Только если мы на низком IRQL — безопасно брать ресурсы с ожиданием
    if (currIrql <= APC_LEVEL) {
        KeEnterCriticalRegion();
        ExAcquireResourceSharedLite(&g_ConfigLock, TRUE);
        used_shared_lock = TRUE;
    }
    else {
        // HIGH IRQL (например DISPATCH_LEVEL): НЕ блокируем ресурс (не вызываем ExAcquire с wait=true)
        // В этом режиме мы просто читаем "как есть" — риск race есть, но это безопаснее, чем вызывать
        // ожидание/вход в критическую секцию при высоком IRQL.
        used_shared_lock = FALSE;
    }

    // Основная проверка по конфигу (чтение global arrays)
    for (INT i = 0; i < proccol; i++) {
        if (processes[i] == NULL) continue; // защитимся от NULL
        if (strstr(ProcessName, processes[i]) != NULL) {
            BOOLEAN isWriteAccess = FALSE;
            BOOLEAN isReadAccess = FALSE;
            if (Data->Iopb->MajorFunction == IRP_MJ_CREATE) {
                ACCESS_MASK desiredAccess = Data->Iopb->Parameters.Create.SecurityContext->DesiredAccess;
                isWriteAccess = (desiredAccess & (FILE_WRITE_DATA | FILE_APPEND_DATA | GENERIC_WRITE)) != 0;
                isReadAccess = (desiredAccess & (FILE_READ_DATA | GENERIC_READ)) != 0;
            }
            INT j = 0;
            if (Data->Iopb->MajorFunction == IRP_MJ_READ || isReadAccess) {
                if (filesdeny[i] != NULL) {
                    while (filesdeny[i][j] != NULL) {
                        if (wcsstr(fileNameInfo->Name.Buffer, filesdeny[i][j]) != NULL) {
                            // найдено совпадение — подготовим ответ и выйдем
                            FltReleaseFileNameInformation(fileNameInfo);
                            Data->IoStatus.Status = STATUS_ACCESS_DENIED;

                            if (used_shared_lock) {
                                ExReleaseResourceLite(&g_ConfigLock);
                                KeLeaveCriticalRegion();
                                used_shared_lock = 0;
                            }
                            return FLT_PREOP_COMPLETE;
                        }
                        j++;
                    }
                }
            }
            else if (Data->Iopb->MajorFunction == IRP_MJ_WRITE || isWriteAccess) {
                if (fileswrite[i] != NULL) {
                    while (fileswrite[i][j] != NULL) {
                        if (wcsstr(fileNameInfo->Name.Buffer, fileswrite[i][j]) != NULL) {
                            FltReleaseFileNameInformation(fileNameInfo);
                            Data->IoStatus.Status = STATUS_ACCESS_DENIED;

                            if (used_shared_lock) {
                                ExReleaseResourceLite(&g_ConfigLock);
                                KeLeaveCriticalRegion();
                                used_shared_lock = 0;
                            }
                            return FLT_PREOP_COMPLETE;
                        }
                        j++;
                    }
                }
            }
        }
    }

    // освобождаем имя файла
    FltReleaseFileNameInformation(fileNameInfo);

    // Освобождаем ресурс если брали
    if (used_shared_lock) {
        ExReleaseResourceLite(&g_ConfigLock);
        KeLeaveCriticalRegion();
        used_shared_lock = 0;
    }

    return ret;
}




VOID
PtOperationStatusCallback(
    _In_ PCFLT_RELATED_OBJECTS FltObjects,
    _In_ PFLT_IO_PARAMETER_BLOCK ParameterSnapshot,
    _In_ NTSTATUS OperationStatus,
    _In_ PVOID RequesterContext
)
/*++

Routine Description:

    This routine is called when the given operation returns from the call
    to IoCallDriver.  This is useful for operations where STATUS_PENDING
    means the operation was successfully queued.  This is useful for OpLocks
    and directory change notification operations.

    This callback is called in the context of the originating thread and will
    never be called at DPC level.  The file object has been correctly
    referenced so that you can access it.  It will be automatically
    dereferenced upon return.

    This is non-pageable because it could be called on the paging path

Arguments:

    FltObjects - Pointer to the FLT_RELATED_OBJECTS data structure containing
        opaque handles to this filter, instance, its associated volume and
        file object.

    RequesterContext - The context for the completion routine for this
        operation.

    OperationStatus -

Return Value:

    The return value is the status of the operation.

--*/
{
    UNREFERENCED_PARAMETER(FltObjects);

    PT_DBG_PRINT(PTDBG_TRACE_ROUTINES,
        ("PassThrough!PtOperationStatusCallback: Entered\n"));

    PT_DBG_PRINT(PTDBG_TRACE_OPERATION_STATUS,
        ("PassThrough!PtOperationStatusCallback: Status=%08x ctx=%p IrpMj=%02x.%02x \"%s\"\n",
            OperationStatus,
            RequesterContext,
            ParameterSnapshot->MajorFunction,
            ParameterSnapshot->MinorFunction,
            FltGetIrpName(ParameterSnapshot->MajorFunction)));
}

FLT_POSTOP_CALLBACK_STATUS
PtPostOperationPassThrough(
    _Inout_ PFLT_CALLBACK_DATA Data,
    _In_ PCFLT_RELATED_OBJECTS FltObjects,
    _In_opt_ PVOID CompletionContext,
    _In_ FLT_POST_OPERATION_FLAGS Flags
)
/*++

Routine Description:

    This routine is the post-operation completion routine for this
    miniFilter.

    This is non-pageable because it may be called at DPC level.

Arguments:

    Data - Pointer to the filter callbackData that is passed to us.

    FltObjects - Pointer to the FLT_RELATED_OBJECTS data structure containing
        opaque handles to this filter, instance, its associated volume and
        file object.

    CompletionContext - The completion context set in the pre-operation routine.

    Flags - Denotes whether the completion is successful or is being drained.

Return Value:

    The return value is the status of the operation.

--*/
{

    UNREFERENCED_PARAMETER(FltObjects);
    UNREFERENCED_PARAMETER(CompletionContext);
    UNREFERENCED_PARAMETER(Flags);

    PT_DBG_PRINT(PTDBG_TRACE_ROUTINES,
        ("PassThrough!PtPostOperationPassThrough: Entered\n"));

    NTSTATUS status;
    if (PtDoRequestOperationStatus(Data)) {
        PFLT_FILE_NAME_INFORMATION fileNameInfo;
        status = FltGetFileNameInformation(Data, FLT_FILE_NAME_NORMALIZED, &fileNameInfo);

        //KdPrintEx((DPFLTR_IHVDRIVER_ID, DPFLTR_INFO_LEVEL, "%ws\n", Data->Iopb->TargetFileObject->FileName.Buffer));
        if (!NT_SUCCESS(status)) {

            return FLT_POSTOP_FINISHED_PROCESSING;
        }

        else {
            if (Data->Iopb->MajorFunction == IRP_MJ_WRITE) {
                if (wcsstr(fileNameInfo->Name.Buffer, configfile_short) != NULL) {

                    // Try to coalesce: if already queued or running, skip scheduling new item
                    if (InterlockedCompareExchange(&g_ConfigWorkItemQueued, 1, 0) == 0) {
                        // allocate a deferred work item and queue it
                        PFLT_DEFERRED_IO_WORKITEM workItem = FltAllocateDeferredIoWorkItem();
                        if (workItem != NULL) {
                            NTSTATUS qstatus = FltQueueDeferredIoWorkItem(
                                workItem,
                                Data,                // CallbackData
                                ConfigDeferredRoutine,
                                DelayedWorkQueue,    // очередь (можно CriticalWorkQueue)
                                NULL                 // Context
                            );
                            if (!NT_SUCCESS(qstatus)) {
                                FltFreeDeferredIoWorkItem(workItem);
                                InterlockedExchange(&g_ConfigWorkItemQueued, 0);
                                KdPrint(("Config: FltQueueDeferredIoWorkItem failed: 0x%08X\n", qstatus));
                            }
                            else {
                                KdPrint(("Config: queued deferred reload work item\n"));
                            }
                        }
                        else {
                            InterlockedExchange(&g_ConfigWorkItemQueued, 0);
                            KdPrint(("Config: FltAllocateDeferredIoWorkItem failed\n"));
                        }

                    } // else already queued/running
                }
            }

        }
        FltReleaseFileNameInformation(fileNameInfo);

    }
    return FLT_POSTOP_FINISHED_PROCESSING;
}


BOOLEAN
PtDoRequestOperationStatus(
    _In_ PFLT_CALLBACK_DATA Data
)
/*++

Routine Description:

    This identifies those operations we want the operation status for.  These
    are typically operations that return STATUS_PENDING as a normal completion
    status.

Arguments:

Return Value:

    TRUE - If we want the operation status
    FALSE - If we don't

--*/
{
    PFLT_IO_PARAMETER_BLOCK iopb = Data->Iopb;

    //
    //  return boolean state based on which operations we are interested in
    //

    //if (iopb->MajorFunction != 0) KdPrintEx((DPFLTR_IHVDRIVER_ID, DPFLTR_INFO_LEVEL, "NullFilter Worked - %x\n", iopb->MajorFunction));


    return (BOOLEAN)

        //
        //  Check for oplock operations
        //

        ((iopb->MajorFunction == IRP_MJ_FILE_SYSTEM_CONTROL) || (iopb->MajorFunction == IRP_MJ_WRITE) || (iopb->MajorFunction == IRP_MJ_READ) ||
            (iopb->MajorFunction == IRP_MJ_SET_INFORMATION) ||
            //
            // �������� �������� ����� (����������� ����)
            //
            (
                (iopb->MajorFunction == IRP_MJ_CREATE) &&
                !(iopb->Parameters.Create.Options & FILE_DIRECTORY_FILE) &&

                // ��������� ����� �������, ����������� �� ��������
                (
                    (iopb->Parameters.Create.SecurityContext->DesiredAccess & FILE_CREATE) ||
                    (iopb->Parameters.Create.ShareAccess & FILE_SHARE_VALID_FLAGS)))

            );
}