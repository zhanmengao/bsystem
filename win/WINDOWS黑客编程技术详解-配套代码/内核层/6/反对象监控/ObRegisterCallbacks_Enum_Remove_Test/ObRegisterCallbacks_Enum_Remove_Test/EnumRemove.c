#include "EnumRemove.h"


VOID ShowError(PCHAR lpszText, NTSTATUS ntStatus)
{
	DbgPrint("%s Error[0x%X]\n", lpszText, ntStatus);
}


// ��ȡ���̶������ͻص�
BOOLEAN EnumProcessObCallback()
{
	POB_CALLBACK pObCallback = NULL;

	// ֱ�ӻ�ȡ CallbackList ����
	LIST_ENTRY CallbackList = ((POBJECT_TYPE)(*PsProcessType))->CallbackList;

	// ��ʼ����
	pObCallback = (POB_CALLBACK)CallbackList.Flink;
	do
	{
		if (FALSE == MmIsAddressValid(pObCallback))
		{
			break;
		}
		if (NULL != pObCallback->ObHandle)
		{
			// ��ʾ
			DbgPrint("[PsProcessType]pObCallback->ObHandle = 0x%p\n", pObCallback->ObHandle);
			DbgPrint("[PsProcessType]pObCallback->PreCall = 0x%p\n", pObCallback->PreCall);
			DbgPrint("[PsProcessType]pObCallback->PostCall = 0x%p\n", pObCallback->PostCall);
		}
		// ��ȡ��һ������Ϣ
		pObCallback = (POB_CALLBACK)pObCallback->ListEntry.Flink;
		
	} while (CallbackList.Flink != (PLIST_ENTRY)pObCallback);

	return TRUE;
}


// ��ȡ�̶߳������ͻص�
BOOLEAN EnumThreadObCallback()
{
	POB_CALLBACK pObCallback = NULL;

	// ֱ�ӻ�ȡ CallbackList ����
	LIST_ENTRY CallbackList = ((POBJECT_TYPE)(*PsThreadType))->CallbackList;

	// ��ʼ����
	pObCallback = (POB_CALLBACK)CallbackList.Flink;
	do
	{
		if (FALSE == MmIsAddressValid(pObCallback))
		{
			break;
		}
		if (NULL != pObCallback->ObHandle)
		{
			// ��ʾ
			DbgPrint("[PsThreadype]pObCallback->ObHandle = 0x%p\n", pObCallback->ObHandle);
			DbgPrint("[PsThreadType]pObCallback->PreCall = 0x%p\n", pObCallback->PreCall);
			DbgPrint("[PsThreadType]pObCallback->PostCall = 0x%p\n", pObCallback->PostCall);
		}
		// ��ȡ��һ������Ϣ
		pObCallback = (POB_CALLBACK)pObCallback->ListEntry.Flink;

	} while (CallbackList.Flink != (PLIST_ENTRY)pObCallback);

	return TRUE;
}


// �Ƴ��ص�
NTSTATUS RemoveObCallback(PVOID RegistrationHandle)
{
	ObUnRegisterCallbacks(RegistrationHandle);
	
	return STATUS_SUCCESS;
}