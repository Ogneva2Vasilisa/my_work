/*++

Copyright (c) 1999 - 2002  Microsoft Corporation

Module Name:

    nullFilter.c

Abstract:

    This is the main module of the nullFilter mini filter driver.
    It is a simple minifilter that registers itself with the main filter
    for no callback operations.

Environment:

    Kernel mode

--*/

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

} NULL_FILTER_DATA, *PNULL_FILTER_DATA;

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
DriverEntry (
    _In_ PDRIVER_OBJECT DriverObject,
    _In_ PUNICODE_STRING RegistryPath
    );

NTSTATUS
NullUnload (
    _In_ FLT_FILTER_UNLOAD_FLAGS Flags
    );

NTSTATUS
NullQueryTeardown (
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

WCHAR* filedeny = L"Users\\WDKRemoteUser\\Documents\\test.txt";
WCHAR* configfile = L"\\??\\C:\\Users\\WDKRemoteUser\\Documents\\conf.txt";
WCHAR** filesdeny[50] = { 0 };
WCHAR** fileswrite[50] = { 0 };
CHAR* processes[50] = { 0 };
INT colfiles; // TODO
INT proccol;

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

    sizeof( FLT_REGISTRATION ),         //  Size
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


VOID ReadConfigFile() {
    NTSTATUS status = STATUS_SUCCESS;
    OBJECT_ATTRIBUTES objectAttributes;
    IO_STATUS_BLOCK ioStatusBlock;
    HANDLE fileHandle = NULL;
    FILE_STANDARD_INFORMATION fileInfo;
    PCHAR buffer = NULL;
    UNICODE_STRING filePath;

    // ������������� UNICODE_STRING �� ������� ������
    RtlInitUnicodeString(&filePath, configfile);

    KdPrintEx((DPFLTR_IHVDRIVER_ID, DPFLTR_INFO_LEVEL, "NullFilter: Unicode String\n"));

    // ��������� ��������� �����
    InitializeObjectAttributes(
        &objectAttributes,
        &filePath,
        OBJ_KERNEL_HANDLE | OBJ_CASE_INSENSITIVE,
        NULL,
        NULL
    );

    // �������� �����
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

    KdPrintEx((DPFLTR_IHVDRIVER_ID, DPFLTR_INFO_LEVEL, "NullFilter: Create File\n"));

    if (!NT_SUCCESS(status)) {
        KdPrint(("ZwCreateFile failed: 0x%08X\n", status));
        if (fileHandle != NULL) {
            ZwClose(fileHandle);
        }

        return;
    }

    status = ZwQueryInformationFile(
        fileHandle,
        &ioStatusBlock,
        &fileInfo,
        sizeof(fileInfo),
        FileStandardInformation
    );

    KdPrintEx((DPFLTR_IHVDRIVER_ID, DPFLTR_INFO_LEVEL, "NullFilter: File Info\n"));

    if (!NT_SUCCESS(status) || fileInfo.EndOfFile.QuadPart == 0) {
        KdPrint(("ZwQueryInformationFile failed: 0x%08X\n", status));
        if (fileHandle != NULL) {
            ZwClose(fileHandle);
        }

        return;
    }

    // �������� �� ������� ������� ����
    if (fileInfo.EndOfFile.QuadPart > MAXULONG) {
        status = STATUS_FILE_TOO_LARGE;
        KdPrint(("File too large\n"));
        if (fileHandle != NULL) {
            ZwClose(fileHandle);
        }

        return;
    }

    // ��������� ������
    buffer = ExAllocatePool2(
        POOL_FLAG_NON_PAGED,              // Non-paged pool
        (ULONG)fileInfo.EndOfFile.QuadPart + sizeof(CHAR), // +1 ��� ����-�����������
        'cnfZ'                            // Pool tag
    );

    KdPrintEx((DPFLTR_IHVDRIVER_ID, DPFLTR_INFO_LEVEL, "NullFilter: Allocate memory\n"));

    if (buffer == NULL) {
        status = STATUS_INSUFFICIENT_RESOURCES;
        KdPrint(("Memory allocation failed\n"));
        if (fileHandle != NULL) {
            ZwClose(fileHandle);
        }

        return;
    }

    // ������ �����
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

    KdPrintEx((DPFLTR_IHVDRIVER_ID, DPFLTR_INFO_LEVEL, "NullFilter: Read File\n"));

    if (!NT_SUCCESS(status)) {
        KdPrint(("ZwReadFile failed: 0x%08X\n", status));
        ExFreePoolWithTag(buffer, 'cnfZ');
        if (fileHandle != NULL) {
            ZwClose(fileHandle);
        }

        return;
    }

    ZwClose(fileHandle);

    PCHAR pbuffer = buffer;
    proccol = 0;
    CHAR flag = 0;
    while (1) {
        //CHAR procname[50] = { 0 };
        PCHAR nextline = strchr(pbuffer, '\n');
        PCHAR procname = ExAllocatePool2(
            POOL_FLAG_NON_PAGED,              // Non-paged pool
            nextline - pbuffer + 1, // +1 ��� ����-�����������
            'cnfZ'                            // Pool tag
        );
        for (INT i = 0; i < nextline - pbuffer; i++) {
            procname[i] = *(CHAR*)(pbuffer + i);
            if (procname[i] == '\n' || procname[i] == '\r' || procname[i] == '\t') {
                procname[i] = 0;
            }
        }

        processes[proccol] = procname;

        nextline++;

        while (1) {
            if (strncmp(nextline, "---", 3)==0) {
            //if ((nextline[0] == '-') & ((nextline[1] == '-') & (nextline[2] == '-'))) {
                if (nextline[3] == 0) {
                    flag = 1;
                }
                else {
                    nextline += 3+2;
                    pbuffer = nextline;
                }
                proccol++;
                break;
            }
            PCHAR nextnextline = strchr(nextline, ':');
            CHAR ID[10] = { 0 };
            for (INT i = 0; i < nextnextline - nextline; i++) {
                ID[i] = *(CHAR*)(nextline + i);
            }

            PCHAR t = nextline;
            nextline = nextnextline;
            nextnextline = t;

            if (strncmp(ID, "Read", 4) == 0) {
                KdPrintEx((DPFLTR_IHVDRIVER_ID, DPFLTR_INFO_LEVEL, "NullFilter: Success Files\n"));
                nextline++;
                nextnextline = strchr(nextline, '\n');
                CHAR numfiles[4] = { 0 };
                for (INT i = 0; i < nextnextline - nextline; i++) {
                    numfiles[i] = *(CHAR*)(nextline + i);
                }
                colfiles = atoi(numfiles);
                filesdeny[proccol] = ExAllocatePool2(
                    POOL_FLAG_NON_PAGED,
                    (colfiles + 1) * sizeof(WCHAR*),
                    'arrW'                           // Pool tag
                );
                nextnextline++;
                if (colfiles == 1 || colfiles == 0) {
                    nextline = strchr(nextnextline, '\n');
                }
                else nextline = strchr(nextnextline, ' ');


                for (INT j = 0; j < colfiles; j++) {
                    CHAR filename[255] = { 0 };
                    for (INT i = 0; i < nextline - nextnextline; i++) {
                        filename[i] = *(CHAR*)(nextnextline + i);
                        if (filename[i] == '\n' || filename[i] == '\r' || filename[i] == '\t') {
                            filename[i] = 0;
                        }
                    }
                    if (j < colfiles - 1) {
                        nextline++;
                        nextnextline = strchr(nextline, ' ');
                        PCHAR check = strchr(nextline, '\n');
                        if (nextnextline == NULL || (nextnextline != NULL) && check < nextnextline) {
                            nextnextline = strchr(nextline, '\n');
                        }
                        t = nextline;
                        nextline = nextnextline;
                        nextnextline = t;
                    }
                    
                    ULONG size = 0;
                    filesdeny[proccol][j] = NULL;
                    CharToWchar(filename, &filesdeny[proccol][j], &size);
                }
                nextline++;
            }
            else if (strncmp(ID, "Write", 5) == 0) {
                KdPrintEx((DPFLTR_IHVDRIVER_ID, DPFLTR_INFO_LEVEL, "NullFilter: Success Files\n"));
                nextline++;
                nextnextline = strchr(nextline, '\n');
                CHAR numfiles[4] = { 0 };
                for (INT i = 0; i < nextnextline - nextline; i++) {
                    numfiles[i] = *(CHAR*)(nextline + i);
                }
                colfiles = atoi(numfiles);
                fileswrite[proccol] = ExAllocatePool2(
                    POOL_FLAG_NON_PAGED,
                    (colfiles + 1) * sizeof(WCHAR*),
                    'arrW'                           // Pool tag
                );
                nextnextline++;
                if (colfiles == 1 || colfiles == 0) {
                    nextline = strchr(nextnextline, '\n');
                }
                else nextline = strchr(nextnextline, ' ');


                for (INT j = 0; j < colfiles; j++) {
                    CHAR filename[255] = { 0 };
                    for (INT i = 0; i < nextline - nextnextline; i++) {
                        filename[i] = *(CHAR*)(nextnextline + i);
                        if (filename[i] == '\n' || filename[i] == '\r' || filename[i] == '\t') {
                            filename[i] = 0;
                        }
                    }
                    if (j < colfiles - 1) {
                        nextline++;
                        nextnextline = strchr(nextline, ' ');
                        PCHAR check = strchr(nextline, '\n');
                        if (nextnextline == NULL || (nextnextline != NULL) && check < nextnextline) {
                            nextnextline = strchr(nextline, '\n');
                        }
                        t = nextline;
                        nextline = nextnextline;
                        nextnextline = t;
                    }
                   
                    ULONG size = 0;
                    fileswrite[proccol][j] = NULL;
                    CharToWchar(filename, &fileswrite[proccol][j], &size);
                }
                nextline++;
            }
        }
        if (flag) break;
    }



    ExFreePoolWithTag(buffer, 'cnfZ');
    
    return;
}


/*************************************************************************
    Filter initialization and unload routines.
*************************************************************************/

NTSTATUS
DriverEntry (
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

    UNREFERENCED_PARAMETER( RegistryPath );

    //
    //  Register with FltMgr
    //

    status = FltRegisterFilter( DriverObject,
                                &FilterRegistration,
                                &NullFilterData.FilterHandle );

    FLT_ASSERT( NT_SUCCESS( status ) );

    if (NT_SUCCESS( status )) {

        //
        //  Start filtering i/o
        //

        ReadConfigFile();

        status = FltStartFiltering( NullFilterData.FilterHandle );

        if (!NT_SUCCESS( status )) {
            FltUnregisterFilter( NullFilterData.FilterHandle );
        }
    }
    return status;
}

NTSTATUS
NullUnload (
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
    UNREFERENCED_PARAMETER( Flags );

    PAGED_CODE();

    FltUnregisterFilter( NullFilterData.FilterHandle );

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

    while (fileswrite[i] != NULL) {
        INT j = 0;
        while (fileswrite[i][j] != NULL) {
            ExFreePoolWithTag(fileswrite[i][j], 'wchr');
            j++;
        }
        ExFreePoolWithTag(fileswrite[i], 'arrW');
        i++;
    }

    for (i = 0; i < proccol; i++) {
        ExFreePoolWithTag(processes[i], 'cnfZ');
    }

    return STATUS_SUCCESS;
}

NTSTATUS
NullQueryTeardown (
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
    UNREFERENCED_PARAMETER( FltObjects );
    UNREFERENCED_PARAMETER( Flags );

    PAGED_CODE();

    return STATUS_SUCCESS;
}


FLT_PREOP_CALLBACK_STATUS
PtPreOperationPassThrough(
    _Inout_ PFLT_CALLBACK_DATA Data,
    _In_ PCFLT_RELATED_OBJECTS FltObjects,
    _Flt_CompletionContext_Outptr_ PVOID* CompletionContext
)
/*++

Routine Description:

    This routine is the main pre-operation dispatch routine for this
    miniFilter. Since this is just a simple passThrough miniFilter it
    does not do anything with the callbackData but rather return
    FLT_PREOP_SUCCESS_WITH_CALLBACK thereby passing it down to the next
    miniFilter in the chain.

    This is non-pageable because it could be called on the paging path

Arguments:

    Data - Pointer to the filter callbackData that is passed to us.

    FltObjects - Pointer to the FLT_RELATED_OBJECTS data structure containing
        opaque handles to this filter, instance, its associated volume and
        file object.

    CompletionContext - The context for the completion routine for this
        operation.

Return Value:

    The return value is the status of the operation.

--*/
{
    NTSTATUS status;

    UNREFERENCED_PARAMETER(FltObjects);
    UNREFERENCED_PARAMETER(CompletionContext);

    PT_DBG_PRINT(PTDBG_TRACE_ROUTINES,
        ("PassThrough!PtPreOperationPassThrough: Entered\n"));

    //
    //  See if this is an operation we would like the operation status
    //  for.  If so request it.
    //
    //  NOTE: most filters do NOT need to do this.  You only need to make
    //        this call if, for example, you need to know if the oplock was
    //        actually granted.
    //

    if (PtDoRequestOperationStatus(Data)) {
        //KdPrintEx((DPFLTR_IHVDRIVER_ID, DPFLTR_INFO_LEVEL, "NullFilter Worked\n"));

        PFLT_FILE_NAME_INFORMATION fileNameInfo;
        status = FltGetFileNameInformation(Data, FLT_FILE_NAME_NORMALIZED, &fileNameInfo);
        
        //KdPrintEx((DPFLTR_IHVDRIVER_ID, DPFLTR_INFO_LEVEL, "%ws\n", Data->Iopb->TargetFileObject->FileName.Buffer));
        if (!NT_SUCCESS(status)) {

            PT_DBG_PRINT(PTDBG_TRACE_OPERATION_STATUS,
                ("PassThrough!PtPreOperationPassThrough: FltRequestOperationStatusCallback Failed, status=%08x\n",
                    status));
            return FLT_PREOP_SUCCESS_NO_CALLBACK;
        }

        else {
            PCHAR ProcessName = PsGetProcessImageFileName(PsGetCurrentProcess());
            KdPrintEx((DPFLTR_IHVDRIVER_ID, DPFLTR_INFO_LEVEL, "Process Name: %s\n", ProcessName));
            for (INT i = 0; i < proccol; i++) {
                if (strstr(ProcessName, processes[i]) != NULL) {
                    BOOLEAN isWriteAccess = 0;
                    BOOLEAN isReadAccess = 0;
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
                                    FltReleaseFileNameInformation(fileNameInfo);
                                    Data->IoStatus.Status = STATUS_ACCESS_DENIED;
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
                                    return FLT_PREOP_COMPLETE;
                                }
                                j++;
                            }
                        }
                    }
                }
            }
            FltReleaseFileNameInformation(fileNameInfo);
        }
    }

    return FLT_PREOP_SUCCESS_WITH_CALLBACK;
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
    UNREFERENCED_PARAMETER(Data);
    UNREFERENCED_PARAMETER(FltObjects);
    UNREFERENCED_PARAMETER(CompletionContext);
    UNREFERENCED_PARAMETER(Flags);

    PT_DBG_PRINT(PTDBG_TRACE_ROUTINES,
        ("PassThrough!PtPostOperationPassThrough: Entered\n"));

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
            //&&
            //((iopb->Parameters.FileSystemControl.Common.FsControlCode == FSCTL_REQUEST_FILTER_OPLOCK) ||
             //   (iopb->Parameters.FileSystemControl.Common.FsControlCode == FSCTL_REQUEST_BATCH_OPLOCK) ||
             //   (iopb->Parameters.FileSystemControl.Common.FsControlCode == FSCTL_REQUEST_OPLOCK_LEVEL_1) ||
             //   (iopb->Parameters.FileSystemControl.Common.FsControlCode == FSCTL_REQUEST_OPLOCK_LEVEL_2)))

            //||

            //
            //    Check for directy change notification
            //

            //((iopb->MajorFunction == IRP_MJ_DIRECTORY_CONTROL) &&
            //    (iopb->MinorFunction == IRP_MN_NOTIFY_CHANGE_DIRECTORY))
            );
}