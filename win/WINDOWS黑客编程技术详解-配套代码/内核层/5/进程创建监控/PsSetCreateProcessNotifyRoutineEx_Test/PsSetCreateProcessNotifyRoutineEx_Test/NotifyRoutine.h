#ifndef _NOTIFY_ROUTINE_H_
#define _NOTIFY_ROUTINE_H_


#include <ntddk.h>


// δ������������
PCHAR PsGetProcessImageFileName(PEPROCESS pEProcess);


// ��̷�ʽ�ƹ�ǩ�����
BOOLEAN BypassCheckSign(PDRIVER_OBJECT pDriverObject);

// ���ûص�����
NTSTATUS SetProcessNotifyRoutine();

// ɾ���ص�����
NTSTATUS RemoveProcessNotifyRoutine();

// �ص�����
VOID ProcessNotifyExRoutine(PEPROCESS pEProcess, HANDLE hProcessId, PPS_CREATE_NOTIFY_INFO CreateInfo);


#endif