//////////////////////////////////////////////////
// VcDriverHello.cpp�ļ�


extern "C"
{
	#include <ntddk.h>
}

VOID DriverUnload(PDRIVER_OBJECT pDriverOjbect)
{
    KdPrint(("DriverUnload Routine!\r\n"));
}


// �����������ʱ����DriverEntry����
NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObj, PUNICODE_STRING pRegistryString)
{
    KdPrint(("%S\r\n", pDriverObj->DriverName.Buffer));

    pDriverObj->DriverUnload = DriverUnload;

	// �����������DriverEntry����ִ�н��
	return STATUS_SUCCESS;;
}
