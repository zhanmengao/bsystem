#include <ntddk.h>

VOID DriverUnload(PDRIVER_OBJECT pDriverOjbect)
{
    KdPrint(("DriverUnload Routine!\r\n"));
}

NTSTATUS DriverEntry(
            PDRIVER_OBJECT pDriverObject,
            PUNICODE_STRING pRegistryPath)
{
    KdPrint(("%S\r\n", pDriverObject->DriverName.Buffer));
    pDriverObject->DriverUnload = DriverUnload;
    
    return STATUS_SUCCESS;
}