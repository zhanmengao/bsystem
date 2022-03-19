#ifndef _SSDT_FUNCTION_H_
#define _SSDT_FUNCTION_H_


#include <ntddk.h>
#include <ntimage.h>


#pragma pack(1)
typedef struct _SERVICE_DESCIPTOR_TABLE
{
	PULONG ServiceTableBase;		  // SSDT��ַ
	PULONG ServiceCounterTableBase;   // SSDT�з��񱻵��ô���������
	ULONG NumberOfService;            // SSDT�������
	PUCHAR ParamTableBase;		      // ϵͳ����������ַ
}SSDTEntry, *PSSDTEntry;
#pragma pack()

// ֱ�ӻ�ȡ SSDT 
extern SSDTEntry __declspec(dllimport) KeServiceDescriptorTable;


// ��ȡ SSDT ������ַ
PVOID GetSSDTFunction(PCHAR pszFunctionName);

// �� ntdll.dll �л�ȡ SSDT ����������
ULONG GetSSDTFunctionIndex(UNICODE_STRING ustrDllFileName, PCHAR pszFunctionName);
// �ڴ�ӳ���ļ�
NTSTATUS DllFileMap(UNICODE_STRING ustrDllFileName, HANDLE *phFile, HANDLE *phSection, PVOID *ppBaseAddress);
// ���ݵ������ȡ����������ַ, �Ӷ���ȡ SSDT ����������
ULONG GetIndexFromExportTable(PVOID pBaseAddress, PCHAR pszFunctionName);


#endif