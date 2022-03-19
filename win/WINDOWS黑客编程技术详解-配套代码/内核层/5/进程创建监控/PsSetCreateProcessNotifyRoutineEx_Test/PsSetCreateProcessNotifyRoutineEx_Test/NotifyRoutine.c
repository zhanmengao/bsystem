#include "NotifyRoutine.h"


VOID ShowError(PCHAR pszText, NTSTATUS ntStatus)
{
	DbgPrint("%s Error[0x%X]\n", pszText, ntStatus);
}


// ��̷�ʽ�ƹ�ǩ�����
BOOLEAN BypassCheckSign(PDRIVER_OBJECT pDriverObject)
{
#ifdef _WIN64
	typedef struct _KLDR_DATA_TABLE_ENTRY
	{
		LIST_ENTRY listEntry;
		ULONG64 __Undefined1;
		ULONG64 __Undefined2;
		ULONG64 __Undefined3;
		ULONG64 NonPagedDebugInfo;
		ULONG64 DllBase;
		ULONG64 EntryPoint;
		ULONG SizeOfImage;
		UNICODE_STRING path;
		UNICODE_STRING name;
		ULONG   Flags;
		USHORT  LoadCount;
		USHORT  __Undefined5;
		ULONG64 __Undefined6;
		ULONG   CheckSum;
		ULONG   __padding1;
		ULONG   TimeDateStamp;
		ULONG   __padding2;
	} KLDR_DATA_TABLE_ENTRY, *PKLDR_DATA_TABLE_ENTRY;
#else
	typedef struct _KLDR_DATA_TABLE_ENTRY
	{
		LIST_ENTRY listEntry;
		ULONG unknown1;
		ULONG unknown2;
		ULONG unknown3;
		ULONG unknown4;
		ULONG unknown5;
		ULONG unknown6;
		ULONG unknown7;
		UNICODE_STRING path;
		UNICODE_STRING name;
		ULONG   Flags;
	} KLDR_DATA_TABLE_ENTRY, *PKLDR_DATA_TABLE_ENTRY;
#endif

	PKLDR_DATA_TABLE_ENTRY pLdrData = (PKLDR_DATA_TABLE_ENTRY)pDriverObject->DriverSection;
	pLdrData->Flags = pLdrData->Flags | 0x20;

	return TRUE;
}


// ���ûص�����
NTSTATUS SetProcessNotifyRoutine()
{
	NTSTATUS status = PsSetCreateProcessNotifyRoutineEx((PCREATE_PROCESS_NOTIFY_ROUTINE_EX)ProcessNotifyExRoutine, FALSE);
	if (!NT_SUCCESS(status))
	{
		ShowError("PsSetCreateProcessNotifyRoutineEx", status);
	}
	return status;
}


// ɾ���ص�����
NTSTATUS RemoveProcessNotifyRoutine()
{
	NTSTATUS status = PsSetCreateProcessNotifyRoutineEx((PCREATE_PROCESS_NOTIFY_ROUTINE_EX)ProcessNotifyExRoutine, TRUE);
	if (!NT_SUCCESS(status))
	{
		ShowError("PsSetCreateProcessNotifyRoutineEx", status);
	}
	return status;
}


// �ص�����
VOID ProcessNotifyExRoutine(PEPROCESS pEProcess, HANDLE hProcessId, PPS_CREATE_NOTIFY_INFO CreateInfo)
{
	// CreateInfo Ϊ NULL ʱ����ʾ�����˳�����Ϊ NULL ʱ����ʾ���̴���
	if (NULL == CreateInfo)
	{
		return;
	}
	// ��ȡ��������
	PCHAR pszImageFileName = PsGetProcessImageFileName(pEProcess);
	// ��ʾ����������Ϣ
	DbgPrint("[%s][%d][%wZ]\n", pszImageFileName, hProcessId, CreateInfo->ImageFileName);
	// ��ָֹ������(520.exe)���� 
	if (0 == _stricmp(pszImageFileName, "520.exe"))
	{
		// ��ֹ����
		CreateInfo->CreationStatus = STATUS_UNSUCCESSFUL;

		DbgPrint("[��ֹ����]\n");
	}
}