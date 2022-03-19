#ifndef _SSDT_FUNCTION_H_
#define _SSDT_FUNCTION_H_


#include <ntddk.h>
#include <ntimage.h>


#ifndef _WIN64
    // 32 Bits
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

#else
    // 64 Bits
    #pragma pack(1)
    typedef struct _SERVICE_DESCIPTOR_TABLE
    {
	    PULONG ServiceTableBase;		  // SSDT��ַ
	    PVOID ServiceCounterTableBase; // SSDT�з��񱻵��ô���������
	    ULONGLONG NumberOfService;     // SSDT�������
	    PVOID ParamTableBase;		  // ϵͳ����������ַ
    }SSDTEntry, *PSSDTEntry;
    #pragma pack()

#endif


// ��ȡ SSDT ������ַ
PVOID GetSSDTFunction(PCHAR pszFunctionName);

// 32λ, ֱ�ӻ�ȡ������ַ; 64λ, ����������, �� KiSystemCall64 �л�ȡ SSDT ��ַ
PVOID GetSSDTAddress();

// �� ntdll.dll �л�ȡ SSDT ����������
ULONG GetSSDTFunctionIndex(UNICODE_STRING ustrDllFileName, PCHAR pszFunctionName);
// �ڴ�ӳ���ļ�
NTSTATUS DllFileMap(UNICODE_STRING ustrDllFileName, HANDLE *phFile, HANDLE *phSection, PVOID *ppBaseAddress);
// ���ݵ������ȡ����������ַ, �Ӷ���ȡ SSDT ����������
ULONG GetIndexFromExportTable(PVOID pBaseAddress, PCHAR pszFunctionName);


#endif