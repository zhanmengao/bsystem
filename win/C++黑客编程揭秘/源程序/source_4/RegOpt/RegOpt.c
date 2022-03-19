#include <ntddk.h>

#define REG_PATH L"\\Registry\\Machine\\Software\\Microsoft\\Windows\\CurrentVersion\\run\\"

VOID DriverUnload(PDRIVER_OBJECT pDriverObject)
{
}

VOID CreateKey()
{
    UNICODE_STRING uniRegPath;
    OBJECT_ATTRIBUTES objAttributes;
    NTSTATUS nStatus;
    HANDLE hRegistry;
    ULONG ulResult;
    
    RtlInitUnicodeString(&uniRegPath, REG_PATH);
    InitializeObjectAttributes(&objAttributes,
                        &uniRegPath,
                        OBJ_CASE_INSENSITIVE,
                        NULL,
                        NULL);
    
    // 创建注册表项
    nStatus = ZwCreateKey(&hRegistry,
                    KEY_ALL_ACCESS,
                    &objAttributes,
                    0,
                    NULL,
                    REG_OPTION_NON_VOLATILE,
                    &ulResult);
                    
    if ( NT_SUCCESS(nStatus) )
    {
        KdPrint(("ZwCreateKey Successfully ! \r\n"));
    }
    else
    {
        KdPrint(("ZwCreateKey Unsuccessfully ! \r\n"));
    }

    // 关闭注册表句柄
    ZwClose(hRegistry);
}

VOID QueryAndSetKey(HANDLE hRegistry)
{
    UNICODE_STRING uniValueName;
    NTSTATUS nStatus;
    PWCHAR pValue = L"test";
    PKEY_VALUE_PARTIAL_INFORMATION  pKeyValuePartialClass;
    ULONG ulResult;
    
    RtlInitUnicodeString(&uniValueName, L"test");
    
    // 添加注册表键值
    nStatus = ZwSetValueKey(hRegistry,
                        &uniValueName,
                        0,
                        REG_SZ,
                        pValue,
                        wcslen(pValue) * 2 + sizeof(WCHAR));
    
    if ( NT_SUCCESS(nStatus) )
    {
        KdPrint(("ZwSetValueKey Successfully ! \r\n"));
    }
    else
    {
        KdPrint(("ZwSetValueKey Unsuccessfully ! \r\n"));
    }
    
    // 查询注册表项
    nStatus = ZwQueryValueKey(hRegistry,
                            &uniValueName,
                            KeyValuePartialInformation,
                            NULL,
                            NULL,
                            &ulResult);
    
    // STATUS_BUFFER_TOO_SMALL表示缓冲区太小
    if ( nStatus == STATUS_BUFFER_TOO_SMALL || ulResult != 0 )
    {
        pKeyValuePartialClass = ExAllocatePool(PagedPool, ulResult);
        nStatus = ZwQueryValueKey(hRegistry,
                            &uniValueName,
                            KeyValuePartialInformation,
                            pKeyValuePartialClass,
                            ulResult,
                            &ulResult);
        
        KdPrint(("%S \r\n", pKeyValuePartialClass->Data));
        
        ExFreePool(pKeyValuePartialClass);
    }
    else
    {
        KdPrint(("ZwQueryValueKey Unsuccessfully ! \r\n"));
    }
}

VOID OpenKey()
{
    UNICODE_STRING uniRegPath;
    OBJECT_ATTRIBUTES objAttributes;
    HANDLE hRegistry;
    NTSTATUS nStatus;
    
    RtlInitUnicodeString(&uniRegPath, REG_PATH);
    InitializeObjectAttributes(&objAttributes,
                        &uniRegPath,
                        OBJ_CASE_INSENSITIVE,
                        NULL,
                        NULL);
     
    // 打开注册表
    nStatus = ZwOpenKey(&hRegistry, KEY_ALL_ACCESS, &objAttributes);
    
    if ( NT_SUCCESS(nStatus) )
    {
        KdPrint(("ZwOpenKey Successfully ! \r\n"));
    }
    else
    {
        KdPrint(("ZwOpenKey Unsuccessfully ! \r\n"));
        return ;
    }
    
    // 查询并设置
    QueryAndSetKey(hRegistry);
    
    // 关闭注册表句柄
    ZwClose(hRegistry);
}

NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObject, PUNICODE_STRING pRegistryPath)
{
    pDriverObject->DriverUnload = DriverUnload;
    
    CreateKey();
    OpenKey();

    return STATUS_SUCCESS;
}