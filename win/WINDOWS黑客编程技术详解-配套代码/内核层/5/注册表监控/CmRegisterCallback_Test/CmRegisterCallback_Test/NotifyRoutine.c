#include "NotifyRoutine.h"


VOID ShowError(PCHAR lpszText, NTSTATUS ntStatus)
{
	DbgPrint("%s Error[0x%x]\n", lpszText, ntStatus);
}


// ���ûص�����
NTSTATUS SetRegisterCallback()
{
	NTSTATUS status = CmRegisterCallback(RegisterMonCallback, NULL, &g_liRegCookie);
	if (!NT_SUCCESS(status))
	{
		ShowError("CmRegisterCallback", status);
		g_liRegCookie.QuadPart = 0;
		return status;
	}

	return status;
}


// ɾ���ص�����
VOID RemoveRegisterCallback()
{
	if (0 < g_liRegCookie.QuadPart)
	{
		CmUnRegisterCallback(g_liRegCookie);
	}
}


// �ص�����
NTSTATUS RegisterMonCallback(
	_In_ PVOID CallbackContext,
	// �������ͣ�ֻ�ǲ�����ţ�����ָ�룩
	_In_opt_ PVOID Argument1,		
	// ������ϸ��Ϣ�Ľṹ��ָ�� 
	_In_opt_ PVOID Argument2		
	)
{
	NTSTATUS status = STATUS_SUCCESS;
	UNICODE_STRING ustrRegPath;

	// ��ȡ��������
	LONG lOperateType = (REG_NOTIFY_CLASS)Argument1;
	// �����ڴ�
	ustrRegPath.Length = 0;
	ustrRegPath.MaximumLength = 1024 * sizeof(WCHAR);
	ustrRegPath.Buffer = ExAllocatePool(NonPagedPool, ustrRegPath.MaximumLength);
	if (NULL == ustrRegPath.Buffer)
	{
		ShowError("ExAllocatePool", 0);
		return status;
	}
	RtlZeroMemory(ustrRegPath.Buffer, ustrRegPath.MaximumLength);
	// �жϲ���
	switch (lOperateType)
	{
	// ����ע���֮ǰ
	case RegNtPreCreateKey:
	{
		// ��ȡע���·��
		GetRegisterObjectCompletePath(&ustrRegPath, ((PREG_CREATE_KEY_INFORMATION)Argument2)->RootObject);
		
		// ��ʾ
		DbgPrint("[RegNtPreCreateKey][%wZ][%wZ]\n", &ustrRegPath, ((PREG_CREATE_KEY_INFORMATION)Argument2)->CompleteName);
		break;
	}
	// ��ע���֮ǰ
	case RegNtPreOpenKey:
	{
		// ��ȡע���·��
		GetRegisterObjectCompletePath(&ustrRegPath, ((PREG_CREATE_KEY_INFORMATION)Argument2)->RootObject);
	
		// ��ʾ
		DbgPrint("[RegNtPreOpenKey][%wZ][%wZ]\n", &ustrRegPath, ((PREG_CREATE_KEY_INFORMATION)Argument2)->CompleteName);
		break;
	}
	// ɾ����֮ǰ
	case RegNtPreDeleteKey:
	{
		// ��ȡע���·��
		GetRegisterObjectCompletePath(&ustrRegPath, ((PREG_DELETE_KEY_INFORMATION)Argument2)->Object);
	
		// ��ʾ
		DbgPrint("[RegNtPreDeleteKey][%wZ]\n", &ustrRegPath);
		break;
	}
	// ɾ����ֵ֮ǰ
	case RegNtPreDeleteValueKey:
	{
		// ��ȡע���·��
		GetRegisterObjectCompletePath(&ustrRegPath, ((PREG_DELETE_VALUE_KEY_INFORMATION)Argument2)->Object);
		
		// ��ʾ
		DbgPrint("[RegNtPreDeleteValueKey][%wZ][%wZ]\n", &ustrRegPath, ((PREG_DELETE_VALUE_KEY_INFORMATION)Argument2)->ValueName);
		break;
	}
	// �޸ļ�ֵ֮ǰ
	case RegNtPreSetValueKey:
	{
		// ��ȡע���·��
		GetRegisterObjectCompletePath(&ustrRegPath, ((PREG_SET_VALUE_KEY_INFORMATION)Argument2)->Object);
		
		// ��ʾ
		DbgPrint("[RegNtPreSetValueKey][%wZ][%wZ]\n", &ustrRegPath, ((PREG_SET_VALUE_KEY_INFORMATION)Argument2)->ValueName);
		break;
	}
	default:
		break;
	}
	// �ж��Ƿ��Ǳ�������ע���
	if (IsProtectReg(ustrRegPath))
	{
		// �ܾ�����
		status = STATUS_ACCESS_DENIED;
	}
	// �ͷ��ڴ�
	if (NULL != ustrRegPath.Buffer)
	{
		ExFreePool(ustrRegPath.Buffer);
		ustrRegPath.Buffer = NULL;
	}

	// ��ȡ��ǰ����, ������ע���Ľ���
	PEPROCESS pEProcess = PsGetCurrentProcess();
	if (NULL != pEProcess)
	{
		UCHAR *lpszProcessName = PsGetProcessImageFileName(pEProcess);
		if (NULL != lpszProcessName)
		{
			DbgPrint("Current Process[%s]\n", lpszProcessName);
		}
	}

	return status;
}


// ��ȡע�������·��
BOOLEAN GetRegisterObjectCompletePath(PUNICODE_STRING pRegistryPath, PVOID pRegistryObject)
{
	// �ж����ݵ�ַ�Ƿ���Ч
	if ((FALSE == MmIsAddressValid(pRegistryObject)) ||
		(NULL == pRegistryObject))
	{
		return FALSE;
	}
	// �����ڴ�
	ULONG ulSize = 512;
	PVOID lpObjectNameInfo = ExAllocatePool(NonPagedPool, ulSize);
	if (NULL == lpObjectNameInfo)
	{
		ShowError("ExAllocatePool", 0);
		return FALSE;
	}
	// ��ȡע���·��
	ULONG ulRetLen = 0;
	NTSTATUS status = ObQueryNameString(pRegistryObject, (POBJECT_NAME_INFORMATION)lpObjectNameInfo, ulSize, &ulRetLen);
	if (!NT_SUCCESS(status))
	{
		ExFreePool(lpObjectNameInfo);
		ShowError("ObQueryNameString", status);
		return FALSE;
	}
	// ����
	RtlCopyUnicodeString(pRegistryPath, (PUNICODE_STRING)lpObjectNameInfo);
	// �ͷ��ڴ�
	ExFreePool(lpObjectNameInfo);
	return TRUE;
}


// �ж��Ƿ��Ǳ���ע���·��
BOOLEAN IsProtectReg(UNICODE_STRING ustrRegPath)
{
	if (NULL != wcsstr(ustrRegPath.Buffer, L"DemonGan"))
	{
		return TRUE;
	}

	return FALSE;
}