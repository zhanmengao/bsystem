#ifndef _SSDT_FUNCTION_H_
#define _SSDT_FUNCTION_H_


#include <ntddk.h>
#include <ntimage.h>


#pragma pack(1)
typedef struct _SERVICE_DESCIPTOR_TABLE
{
	PULONG ServiceTableBase;		  // SSDT��ַ
	PVOID ServiceCounterTableBase; // SSDT�з��񱻵��ô���������
	ULONGLONG NumberOfService;     // SSDT�������
	PVOID ParamTableBase;		  // ϵͳ����������ַ
}SSDTEntry, *PSSDTEntry;
#pragma pack()


// ��ȡ SSDT ������ַ
PVOID GetSSDTFunction(PCHAR pszFunctionName);

// ����������, �� KiSystemCall64 �л�ȡ SSDT ��ַ
PVOID GetSSDTAddress();

// �� ntdll.dll �л�ȡ SSDT ����������
ULONG GetSSDTFunctionIndex(UNICODE_STRING ustrDllFileName, PCHAR pszFunctionName);
// �ڴ�ӳ���ļ�
NTSTATUS DllFileMap(UNICODE_STRING ustrDllFileName, HANDLE *phFile, HANDLE *phSection, PVOID *ppBaseAddress);
// ���ݵ������ȡ����������ַ, �Ӷ���ȡ SSDT ����������
ULONG GetIndexFromExportTable(PVOID pBaseAddress, PCHAR pszFunctionName);


#endif