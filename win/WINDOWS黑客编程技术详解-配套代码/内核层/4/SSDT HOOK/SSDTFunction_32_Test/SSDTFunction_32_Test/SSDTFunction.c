#include "SSDTFunction.h"


// ��ȡ SSDT ������ַ
PVOID GetSSDTFunction(PCHAR pszFunctionName)
{
	UNICODE_STRING ustrDllFileName;
	ULONG ulSSDTFunctionIndex = 0;
	PVOID pFunctionAddress = NULL;
	
	RtlInitUnicodeString(&ustrDllFileName, L"\\??\\C:\\Windows\\System32\\ntdll.dll");
	// �� ntdll.dll �л�ȡ SSDT ����������
	ulSSDTFunctionIndex = GetSSDTFunctionIndex(ustrDllFileName, pszFunctionName);

	// ����������, ��SSDT���л�ȡ��Ӧ������ַ
	pFunctionAddress = (PVOID)KeServiceDescriptorTable.ServiceTableBase[ulSSDTFunctionIndex];
	
	// ��ʾ
	DbgPrint("[%s][Index:%d][Address:0x%p]\n", pszFunctionName, ulSSDTFunctionIndex, pFunctionAddress);

	return pFunctionAddress;
}


// �� ntdll.dll �л�ȡ SSDT ����������
ULONG GetSSDTFunctionIndex(UNICODE_STRING ustrDllFileName, PCHAR pszFunctionName)
{
	ULONG ulFunctionIndex = 0;
	NTSTATUS status = STATUS_SUCCESS;
	HANDLE hFile = NULL;
	HANDLE hSection = NULL;
	PVOID pBaseAddress = NULL;

	// �ڴ�ӳ���ļ�
	status = DllFileMap(ustrDllFileName, &hFile, &hSection, &pBaseAddress);
	if (!NT_SUCCESS(status))
	{
		KdPrint(("DllFileMap Error!\n"));
		return ulFunctionIndex;
	}

	// ���ݵ������ȡ����������ַ, �Ӷ���ȡ SSDT ����������
	ulFunctionIndex = GetIndexFromExportTable(pBaseAddress, pszFunctionName);

	// �ͷ�
	ZwUnmapViewOfSection(NtCurrentProcess(), pBaseAddress);
	ZwClose(hSection);
	ZwClose(hFile);

	return ulFunctionIndex;
}

// �ڴ�ӳ���ļ�
NTSTATUS DllFileMap(UNICODE_STRING ustrDllFileName, HANDLE *phFile, HANDLE *phSection, PVOID *ppBaseAddress)
{
	NTSTATUS status = STATUS_SUCCESS;
	HANDLE hFile = NULL;
	HANDLE hSection = NULL;
	OBJECT_ATTRIBUTES objectAttributes = { 0 };
	IO_STATUS_BLOCK iosb = { 0 };
	PVOID pBaseAddress = NULL;
	SIZE_T viewSize = 0;
	// �� DLL �ļ�, ����ȡ�ļ����
	InitializeObjectAttributes(&objectAttributes, &ustrDllFileName, OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE, NULL, NULL);
	status = ZwOpenFile(&hFile, GENERIC_READ, &objectAttributes, &iosb,
		FILE_SHARE_READ, FILE_SYNCHRONOUS_IO_NONALERT);
	if (!NT_SUCCESS(status))
	{
		KdPrint(("ZwOpenFile Error! [error code: 0x%X]", status));
		return status;
	}
	// ����һ���ڶ���, �� PE �ṹ�е� SectionALignment ��С����ӳ���ļ�
	status = ZwCreateSection(&hSection, SECTION_MAP_READ | SECTION_MAP_WRITE, NULL, 0, PAGE_READWRITE, 0x1000000, hFile);
	if (!NT_SUCCESS(status))
	{
		ZwClose(hFile);
		KdPrint(("ZwCreateSection Error! [error code: 0x%X]", status));
		return status;
	}
	// ӳ�䵽�ڴ�
	status = ZwMapViewOfSection(hSection, NtCurrentProcess(), &pBaseAddress, 0, 1024, 0, &viewSize, ViewShare, MEM_TOP_DOWN, PAGE_READWRITE);
	if (!NT_SUCCESS(status))
	{
		ZwClose(hSection);
		ZwClose(hFile);
		KdPrint(("ZwMapViewOfSection Error! [error code: 0x%X]", status));
		return status;
	}

	// ��������
	*phFile = hFile;
	*phSection = hSection;
	*ppBaseAddress = pBaseAddress;

	return status;
}

// ���ݵ������ȡ����������ַ, �Ӷ���ȡ SSDT ����������
ULONG GetIndexFromExportTable(PVOID pBaseAddress, PCHAR pszFunctionName)
{
	ULONG ulFunctionIndex = 0;
	// Dos Header
	PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)pBaseAddress;
	// NT Header
	PIMAGE_NT_HEADERS pNtHeaders = (PIMAGE_NT_HEADERS)((PUCHAR)pDosHeader + pDosHeader->e_lfanew);
	// Export Table
	PIMAGE_EXPORT_DIRECTORY pExportTable = (PIMAGE_EXPORT_DIRECTORY)((PUCHAR)pDosHeader + pNtHeaders->OptionalHeader.DataDirectory[0].VirtualAddress);
	// �����Ƶĵ�����������
	ULONG ulNumberOfNames = pExportTable->NumberOfNames;
	// �����������Ƶ�ַ��
	PULONG lpNameArray = (PULONG)((PUCHAR)pDosHeader + pExportTable->AddressOfNames);
	PCHAR lpName = NULL;
	// ��ʼ����������
	for (ULONG i = 0; i < ulNumberOfNames; i++)
	{
		lpName = (PCHAR)((PUCHAR)pDosHeader + lpNameArray[i]);
		// �ж��Ƿ���ҵĺ���
		if (0 == _strnicmp(pszFunctionName, lpName, strlen(pszFunctionName)))
		{
			// ��ȡ����������ַ
			USHORT uHint = *(USHORT *)((PUCHAR)pDosHeader + pExportTable->AddressOfNameOrdinals + 2 * i);
			ULONG ulFuncAddr = *(PULONG)((PUCHAR)pDosHeader + pExportTable->AddressOfFunctions + 4 * uHint);
			PVOID lpFuncAddr = (PVOID)((PUCHAR)pDosHeader + ulFuncAddr);
			// ��ȡ SSDT ���� Index
#ifdef _WIN64
			ulFunctionIndex = *(ULONG *)((PUCHAR)lpFuncAddr + 4);
#else
			ulFunctionIndex = *(ULONG *)((PUCHAR)lpFuncAddr + 1);
#endif
			break;
		}
	}

	return ulFunctionIndex;
}