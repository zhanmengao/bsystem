#ifndef _GET_MIPROCESSLOADERENTRY_H_
#define _GET_MIPROCESSLOADERENTRY_H_


#include <ntddk.h>


#ifndef _WIN64
    // 32
    typedef NTSTATUS(__stdcall *typedef_MiProcessLoaderEntry)(PVOID, BOOLEAN);
#else 
    // 64
    typedef NTSTATUS(__fastcall *typedef_MiProcessLoaderEntry)(PVOID, BOOLEAN);
#endif


// �� MmLoadSystemImage �л�ȡ��Ӧ�� MiProcessLoaderEntry ������
// ����, 32��64λ�� Win7, Win8.1 ֱ�Ӵ� MmLoadSystemImage ������ MiProcessLoaderEntry
// 32��64λ�� Win10 ��Ҫ�� MmLoadSystemImage ������ MiConstructLoaderEntry, �ٴ� MiConstructLoaderEntry ������ MiProcessLoaderEntry
PVOID GetFuncAddr_MiProcessLoaderEntry();

// �� NtSetSystemInformation �л�ȡ MmLoadSystemImage ������ַ
PVOID GetFuncAddr_MmLoadSystemImage();

// ����������
PVOID SearchSpecialCode(PVOID pSearchBeginAddr, ULONG ulSearchLength, PUCHAR pSpecialCode, ULONG ulSpecialCodeLength);


#endif