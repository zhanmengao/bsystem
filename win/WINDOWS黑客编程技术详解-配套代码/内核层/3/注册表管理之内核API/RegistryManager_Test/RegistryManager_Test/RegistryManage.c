#include "RegistryManage.h"


VOID ShowError(PUCHAR pszText, NTSTATUS ntStatus)
{
	DbgPrint("%s Error[0x%X]\n", pszText, ntStatus);
}


// �������ߴ��Ѵ���ע����
BOOLEAN MyCreateRegistryKey(UNICODE_STRING ustrRegistry)
{
	HANDLE hRegister = NULL;
	OBJECT_ATTRIBUTES objectAttributes = {0};
	ULONG ulResult = 0;
	NTSTATUS status = STATUS_SUCCESS;

	// �������ߴ��Ѵ���ע����
	InitializeObjectAttributes(&objectAttributes, &ustrRegistry, OBJ_CASE_INSENSITIVE, NULL, NULL);
	status = ZwCreateKey(&hRegister,
		KEY_ALL_ACCESS,
		&objectAttributes,
		0,
		NULL,
		REG_OPTION_NON_VOLATILE,
		&ulResult);
	if (!NT_SUCCESS(status))
	{
		ShowError("ZwCreateKey", status);
		return FALSE;
	}
	if (REG_CREATED_NEW_KEY == ulResult)
	{
		DbgPrint("The register item is createed!\n");
	}
	else if (REG_OPENED_EXISTING_KEY == ulResult)
	{
		DbgPrint("The register item has been created, and now is opened!\n");
	}

	// �ر�ע�������
	ZwClose(hRegister);
	return TRUE;
}


// ��ע����
BOOLEAN MyOpenRegistryKey(UNICODE_STRING ustrRegistry)
{
	OBJECT_ATTRIBUTES objectAttributes = {0};
	HANDLE hRegister = NULL;
	NTSTATUS status = STATUS_SUCCESS;

	// ��ע����
	InitializeObjectAttributes(&objectAttributes, &ustrRegistry, OBJ_CASE_INSENSITIVE, NULL, NULL);
	status = ZwOpenKey(&hRegister, KEY_ALL_ACCESS, &objectAttributes);
	if (!NT_SUCCESS(status))
	{
		ShowError("ZwOpenKey", status);
		return FALSE;
	}
	DbgPrint("Open register successfully!\n");

	// �ر�ע�������
	ZwClose(hRegister);
	return TRUE;
}


// ��ӻ����޸�ע����ֵ
BOOLEAN MySetRegistryKeyValue(UNICODE_STRING ustrRegistry, UNICODE_STRING ustrKeyValueName, ULONG ulKeyValueType, PVOID pKeyValueData, ULONG ulKeyValueDataSize)
{
	HANDLE hRegister = NULL;
	OBJECT_ATTRIBUTES objectAttributes = {0};
	NTSTATUS status = STATUS_SUCCESS;
	// ��ע����
	InitializeObjectAttributes(&objectAttributes, &ustrRegistry, OBJ_CASE_INSENSITIVE, NULL, NULL);
	status = ZwOpenKey(&hRegister, KEY_ALL_ACCESS, &objectAttributes);
	if (!NT_SUCCESS(status))
	{
		ShowError("ZwOpenKey", status);
		return FALSE;
	}
	// ��ӻ����޸ļ�ֵ
	status = ZwSetValueKey(hRegister, &ustrKeyValueName, 0, ulKeyValueType, pKeyValueData, ulKeyValueDataSize);
	if (!NT_SUCCESS(status))
	{
		ZwClose(hRegister);
		ShowError("ZwSetValueKey", status);
		return FALSE;
	}
	// �ر�ע�������
	ZwClose(hRegister);
	return TRUE;
}


// ɾ��ע����
BOOLEAN MyDeleteRegistryKey(UNICODE_STRING ustrRegistry)
{
	HANDLE hRegister = NULL;
	OBJECT_ATTRIBUTES objectAttributes = {0};
	NTSTATUS status = STATUS_SUCCESS;
	// ��ע����
	InitializeObjectAttributes(&objectAttributes, &ustrRegistry, OBJ_CASE_INSENSITIVE, NULL, NULL);
	status = ZwOpenKey(&hRegister, KEY_ALL_ACCESS, &objectAttributes);
	if (!NT_SUCCESS(status))
	{
		ShowError("ZwOpenKey", status);
		return FALSE;
	}
	// ɾ��ע����
	status = ZwDeleteKey(hRegister);
	if (!NT_SUCCESS(status))
	{
		ZwClose(hRegister);
		ShowError("ZwDeleteKey", status);
		return FALSE;
	}
	// �ر�ע�������
	ZwClose(hRegister);
	return TRUE;
}


// ɾ��ע����ֵ
BOOLEAN MyDeleteRegistryKeyValue(UNICODE_STRING ustrRegistry, UNICODE_STRING ustrKeyValueName)
{
	HANDLE hRegister = NULL;
	OBJECT_ATTRIBUTES objectAttributes = { 0 };
	NTSTATUS status = STATUS_SUCCESS;
	// ��ע����
	InitializeObjectAttributes(&objectAttributes, &ustrRegistry, OBJ_CASE_INSENSITIVE, NULL, NULL);
	status = ZwOpenKey(&hRegister, KEY_ALL_ACCESS, &objectAttributes);
	if (!NT_SUCCESS(status))
	{
		ShowError("ZwOpenKey", status);
		return FALSE;
	}
	// ɾ��ע����
	status = ZwDeleteValueKey(hRegister, &ustrKeyValueName);
	if (!NT_SUCCESS(status))
	{
		ZwClose(hRegister);
		ShowError("ZwDeleteValueKey", status);
		return FALSE;
	}
	// �ر�ע�������
	ZwClose(hRegister);
	return TRUE;
}


// ��ѯע����ֵ
BOOLEAN MyQueryRegistryKeyValue(UNICODE_STRING ustrRegistry, UNICODE_STRING ustrKeyValueName)
{
	HANDLE hRegister = NULL;
	OBJECT_ATTRIBUTES objectAttributes = {0};
	NTSTATUS status = STATUS_SUCCESS;
	ULONG ulBufferSize = 0;
	PKEY_VALUE_PARTIAL_INFORMATION pKeyValuePartialInfo = NULL;

	// ��ע����
	InitializeObjectAttributes(&objectAttributes, &ustrRegistry, OBJ_CASE_INSENSITIVE, NULL, NULL);
	status = ZwOpenKey(&hRegister, KEY_ALL_ACCESS, &objectAttributes);
	if (!NT_SUCCESS(status))
	{
		ShowError("ZwOpenKey", status);
		return FALSE;
	}
	// �Ȼ�ȡ��ѯע����ֵ���軺�����Ĵ�С
	status = ZwQueryValueKey(hRegister, &ustrKeyValueName, KeyValuePartialInformation, NULL, 0, &ulBufferSize);
	if (0 == ulBufferSize)
	{
		ZwClose(hRegister);
		ShowError("ZwQueryValueKey", status);
		return FALSE;
	}
	// ���뻺����
	pKeyValuePartialInfo = (PKEY_VALUE_PARTIAL_INFORMATION)ExAllocatePool(NonPagedPool, ulBufferSize);
	// ��ѯע����ֵ����ȡ��ѯ���
	status = ZwQueryValueKey(hRegister, &ustrKeyValueName, KeyValuePartialInformation, pKeyValuePartialInfo, ulBufferSize, &ulBufferSize);
	if (!NT_SUCCESS(status))
	{
		ExFreePool(pKeyValuePartialInfo);
		ZwClose(hRegister);
		ShowError("ZwQueryValueKey", status);
		return FALSE;
	}
	// ��ʾ��ѯ���
	DbgPrint("KeyValueName=%wZ, KeyValueType=%d, KeyValueData=%S\n",
		&ustrKeyValueName, pKeyValuePartialInfo->Type, pKeyValuePartialInfo->Data);

	// �ͷ��ڴ�, �رվ��
	ExFreePool(pKeyValuePartialInfo);
	ZwClose(hRegister);
	return TRUE;
}