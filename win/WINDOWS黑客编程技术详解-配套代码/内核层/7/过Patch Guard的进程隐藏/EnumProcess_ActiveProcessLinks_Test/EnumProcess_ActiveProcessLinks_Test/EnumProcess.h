#ifndef _ENUM_PROCESS_H_
#define _ENUM_PROCESS_H_


#include <ntddk.h>


// ����δ��������
PUCHAR PsGetProcessImageFileName(PEPROCESS pEprocess);


// ��������
BOOLEAN EnumProcess();

// ����ָ������
BOOLEAN HideProcess(PUCHAR pszHideProcessName);

// ����ָ������(Bypass Patch Guard)
BOOLEAN HideProcess_Bypass_PatchGuard(PUCHAR pszHideProcessName);

// ���ݲ�ͬϵͳ, ��ȡ��Ӧƫ�ƴ�С
ULONG GetActiveProcessLinksOffset();




#endif