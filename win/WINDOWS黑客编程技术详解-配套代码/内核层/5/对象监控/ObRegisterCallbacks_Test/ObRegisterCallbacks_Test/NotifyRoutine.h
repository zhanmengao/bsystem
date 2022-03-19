#ifndef _NOTIFY_ROUTINE_H_
#define _NOTIFY_ROUTINE_H_


#include <ntifs.h>
#include <ntddk.h>


// δ������������
PUCHAR PsGetProcessImageFileName(PEPROCESS pEProcess);


// ��̷�ʽ�ƹ�ǩ�����
BOOLEAN BypassCheckSign(PDRIVER_OBJECT pDriverObject);

// ���ý��̻ص�����
NTSTATUS SetProcessCallbacks();

// �����̻߳ص�����
NTSTATUS SetThreadCallbacks();

// ɾ�����̻ص�����
VOID RemoveProcessCallbacks();

// ɾ���̻߳ص�����
VOID RemoveThreadCallbacks();

// ���̻ص�����
OB_PREOP_CALLBACK_STATUS ProcessPreCall(PVOID RegistrationContext, POB_PRE_OPERATION_INFORMATION pObPreOperationInfo);

// �̻߳ص�����
OB_PREOP_CALLBACK_STATUS ThreadPreCall(PVOID RegistrationContext, POB_PRE_OPERATION_INFORMATION pObPreOperationInfo);

// �ж��Ƿ�Ϊ��������
BOOLEAN IsProtectProcess(PEPROCESS pEProcess);


// ���̻ص�������
HANDLE g_obProcessHandle;

// �̻߳ص�������
HANDLE g_obThreadHandle;


#endif