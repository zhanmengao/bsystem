#ifndef _SSDT_FUNCTION_INDEX_H_
#define _SSDT_FUNCTION_INDEX_H_


#include <ntddk.h>
#include <ntimage.h>


// �� ntdll.dll �л�ȡ SSDT ����������
ULONG GetSSDTFunctionIndex(UNICODE_STRING ustrDllFileName, PCHAR pszFunctionName);

// �ڴ�ӳ���ļ�
NTSTATUS DllFileMap(UNICODE_STRING ustrDllFileName, HANDLE *phFile, HANDLE *phSection, PVOID *ppBaseAddress);

// ���ݵ������ȡ����������ַ, �Ӷ���ȡ SSDT ����������
ULONG GetIndexFromExportTable(PVOID pBaseAddress, PCHAR pszFunctionName);


#endif