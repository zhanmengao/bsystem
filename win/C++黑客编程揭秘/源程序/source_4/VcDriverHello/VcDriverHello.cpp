//////////////////////////////////////////////////
// VcDriverHello.cpp文件


extern "C"
{
	#include <ntddk.h>
}

VOID DriverUnload(PDRIVER_OBJECT pDriverOjbect)
{
    KdPrint(("DriverUnload Routine!\r\n"));
}


// 驱动程序加载时调用DriverEntry例程
NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObj, PUNICODE_STRING pRegistryString)
{
    KdPrint(("%S\r\n", pDriverObj->DriverName.Buffer));

    pDriverObj->DriverUnload = DriverUnload;

	// 请视情况返回DriverEntry例程执行结果
	return STATUS_SUCCESS;;
}
