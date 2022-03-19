#include "Driver.h"
#include "RegistryManage.h"


NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObject, PUNICODE_STRING pRegPath)
{
	DbgPrint("Enter DriverEntry\n");

	NTSTATUS status = STATUS_SUCCESS;
	pDriverObject->DriverUnload = DriverUnload;
	for (ULONG i = 0; i < IRP_MJ_MAXIMUM_FUNCTION; i++)
	{
		pDriverObject->MajorFunction[i] = DriverDefaultHandle;
	}

	// ����ע����
	UNICODE_STRING ustrRegistry;
	RtlInitUnicodeString(&ustrRegistry, L"\\Registry\\Machine\\Software\\DemonGan");
	MyCreateRegistryKey(ustrRegistry);

	// ��ע����
	MyOpenRegistryKey(ustrRegistry);

	// ���ע����ֵ
	UNICODE_STRING ustrKeyValueName;
	WCHAR wstrKeyValueData[] = L"I am DemonGan";
	RtlInitUnicodeString(&ustrKeyValueName, L"Name");
	MySetRegistryKeyValue(ustrRegistry, ustrKeyValueName, REG_SZ, wstrKeyValueData, sizeof(wstrKeyValueData));

	// ��ѯע����ֵ
	MyQueryRegistryKeyValue(ustrRegistry, ustrKeyValueName);

	// ɾ��ע����ֵ
	MyDeleteRegistryKeyValue(ustrRegistry, ustrKeyValueName);

	// ɾ��ע����
	MyDeleteRegistryKey(ustrRegistry);

	DbgPrint("Leave DriverEntry\n");
	return status;
}


VOID DriverUnload(PDRIVER_OBJECT pDriverObject)
{
}


NTSTATUS DriverDefaultHandle(PDEVICE_OBJECT pDevObj, PIRP pIrp)
{
	NTSTATUS status = STATUS_SUCCESS;
	pIrp->IoStatus.Status = status;
	pIrp->IoStatus.Information = 0;
	IoCompleteRequest(pIrp, IO_NO_INCREMENT);

	return status;
}