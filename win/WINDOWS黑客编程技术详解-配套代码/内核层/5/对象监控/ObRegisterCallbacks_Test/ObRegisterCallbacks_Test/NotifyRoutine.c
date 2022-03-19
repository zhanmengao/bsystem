#include "NotifyRoutine.h"


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


// ���ý��̻ص�����
NTSTATUS SetProcessCallbacks()
{
	NTSTATUS status = STATUS_SUCCESS;
	OB_CALLBACK_REGISTRATION obCallbackReg = { 0 };
	OB_OPERATION_REGISTRATION obOperationReg = { 0 };

	RtlZeroMemory(&obCallbackReg, sizeof(OB_CALLBACK_REGISTRATION));
	RtlZeroMemory(&obOperationReg, sizeof(OB_OPERATION_REGISTRATION));

	// ���� OB_CALLBACK_REGISTRATION
	obCallbackReg.Version = ObGetFilterVersion();
	obCallbackReg.OperationRegistrationCount = 1;
	obCallbackReg.RegistrationContext = NULL;
	RtlInitUnicodeString(&obCallbackReg.Altitude, L"321000");
	obCallbackReg.OperationRegistration = &obOperationReg;

	// ���� OB_OPERATION_REGISTRATION
	// Thread �� Process ����������
	obOperationReg.ObjectType = PsProcessType;                                   
	obOperationReg.Operations = OB_OPERATION_HANDLE_CREATE | OB_OPERATION_HANDLE_DUPLICATE;
	// Thread �� Process ����������
	obOperationReg.PreOperation = (POB_PRE_OPERATION_CALLBACK)(&ProcessPreCall); 

	// ע��ص�����
	status = ObRegisterCallbacks(&obCallbackReg, &g_obProcessHandle);
	if (!NT_SUCCESS(status))
	{
		DbgPrint("ObRegisterCallbacks Error[0x%X]\n", status);
		return status;
	}

	return status;
}


// �����̻߳ص�����
NTSTATUS SetThreadCallbacks()
{
	NTSTATUS status = STATUS_SUCCESS;
	OB_CALLBACK_REGISTRATION obCallbackReg = { 0 };
	OB_OPERATION_REGISTRATION obOperationReg = { 0 };

	RtlZeroMemory(&obCallbackReg, sizeof(OB_CALLBACK_REGISTRATION));
	RtlZeroMemory(&obOperationReg, sizeof(OB_OPERATION_REGISTRATION));

	// ���� OB_CALLBACK_REGISTRATION
	obCallbackReg.Version = ObGetFilterVersion();
	obCallbackReg.OperationRegistrationCount = 1;
	obCallbackReg.RegistrationContext = NULL;
	RtlInitUnicodeString(&obCallbackReg.Altitude, L"321001");
	obCallbackReg.OperationRegistration = &obOperationReg;

	// ���� OB_OPERATION_REGISTRATION
	// Thread �� Process ����������
	obOperationReg.ObjectType = PsThreadType;                                   
	obOperationReg.Operations = OB_OPERATION_HANDLE_CREATE | OB_OPERATION_HANDLE_DUPLICATE;
	// Thread �� Process ����������
	obOperationReg.PreOperation = (POB_PRE_OPERATION_CALLBACK)(&ThreadPreCall); 

	// ע��ص�����
	status = ObRegisterCallbacks(&obCallbackReg, &g_obThreadHandle);
	if (!NT_SUCCESS(status))
	{
		DbgPrint("ObRegisterCallbacks Error[0x%X]\n", status);
		return status;
	}

	return status;
}


// ɾ�����̻ص�����
VOID RemoveProcessCallbacks()
{
	// ж�ػص�
	if (NULL != g_obProcessHandle)
	{
		ObUnRegisterCallbacks(g_obProcessHandle);
		g_obProcessHandle = NULL;
	}
}


// ɾ���̻߳ص�����
VOID RemoveThreadCallbacks()
{
	// ж�ػص�
	if (NULL != g_obThreadHandle)
	{
		ObUnRegisterCallbacks(g_obThreadHandle);
		g_obThreadHandle = NULL;
	}
}


// ���̻ص�����
OB_PREOP_CALLBACK_STATUS ProcessPreCall(PVOID RegistrationContext, POB_PRE_OPERATION_INFORMATION pObPreOperationInfo)
{
	PEPROCESS pEProcess = NULL;

	// �ж϶�������
	if (*PsProcessType != pObPreOperationInfo->ObjectType)
	{
		return OB_PREOP_SUCCESS;
	}
	// ��ȡ���̽ṹ����
	pEProcess = (PEPROCESS)pObPreOperationInfo->Object;
	// �ж��Ƿ��б���PID, ����, ��ܾ���������
	if (IsProtectProcess(pEProcess))
	{
		// ��������: �������
		if (OB_OPERATION_HANDLE_CREATE == pObPreOperationInfo->Operation)
		{
			if (1 == (1 & pObPreOperationInfo->Parameters->CreateHandleInformation.OriginalDesiredAccess))
			{
				pObPreOperationInfo->Parameters->CreateHandleInformation.DesiredAccess = 0;
			}
		}
		// ��������: ���ƾ��
		else if (OB_OPERATION_HANDLE_DUPLICATE == pObPreOperationInfo->Operation)
		{
			if (1 == (1 & pObPreOperationInfo->Parameters->DuplicateHandleInformation.OriginalDesiredAccess))
			{
				pObPreOperationInfo->Parameters->DuplicateHandleInformation.DesiredAccess = 0;
			}
		}
	}

	return OB_PREOP_SUCCESS;
}


// �̻߳ص�����
OB_PREOP_CALLBACK_STATUS ThreadPreCall(PVOID RegistrationContext, POB_PRE_OPERATION_INFORMATION pObPreOperationInfo)
{
	PEPROCESS pEProcess = NULL;

	// �ж϶�������
	if (*PsThreadType != pObPreOperationInfo->ObjectType)
	{
		return OB_PREOP_SUCCESS;
	}
	// ��ȡ�̶߳�Ӧ�Ľ��� PEPROCESS
	pEProcess = IoThreadToProcess((PETHREAD)pObPreOperationInfo->Object);
	// �ж��Ƿ��б���PID, ����, ��ܾ������߳�
	if (IsProtectProcess(pEProcess))
	{
		// ��������: �������
		if (OB_OPERATION_HANDLE_CREATE == pObPreOperationInfo->Operation)
		{
			if (1 == (1 & pObPreOperationInfo->Parameters->CreateHandleInformation.OriginalDesiredAccess))
			{
				pObPreOperationInfo->Parameters->CreateHandleInformation.DesiredAccess = 0;
			}
		}
		// ��������: ���ƾ��
		else if (OB_OPERATION_HANDLE_DUPLICATE == pObPreOperationInfo->Operation)
		{
			if (1 == (1 & pObPreOperationInfo->Parameters->DuplicateHandleInformation.OriginalDesiredAccess))
			{
				pObPreOperationInfo->Parameters->DuplicateHandleInformation.DesiredAccess = 0;
			}
		}
	}

	return OB_PREOP_SUCCESS;
}


// �ж��Ƿ�Ϊ��������
BOOLEAN IsProtectProcess(PEPROCESS pEProcess)
{
	PUCHAR pProcessName = PsGetProcessImageFileName(pEProcess);
	if (NULL != pProcessName)
	{
		if (0 == _stricmp(pProcessName, "520.exe"))
		{
			DbgPrint("[Protect]");
			return TRUE;
		}

		DbgPrint("ProcessName=%s\n", pProcessName);
	}
	return FALSE;
}