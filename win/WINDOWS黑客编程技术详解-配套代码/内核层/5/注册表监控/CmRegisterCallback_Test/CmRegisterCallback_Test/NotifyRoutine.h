#ifndef _NOTIFY_ROUTINE_H_
#define _NOTIFY_ROUTINE_H_


#include <ntddk.h>


// δ������������
PUCHAR PsGetProcessImageFileName(PEPROCESS pEProcess);

NTSTATUS ObQueryNameString(
	_In_ PVOID Object,
	_Out_writes_bytes_opt_(Length) POBJECT_NAME_INFORMATION ObjectNameInfo,
	_In_ ULONG Length,
	_Out_ PULONG ReturnLength
);


// ���ûص�����
NTSTATUS SetRegisterCallback();

// ɾ���ص�����
VOID RemoveRegisterCallback();

// �ص�����
NTSTATUS RegisterMonCallback(
	_In_ PVOID CallbackContext,
	// �������ͣ�ֻ�ǲ�����ţ�����ָ�룩
	_In_opt_ PVOID Argument1,
	// ������ϸ��Ϣ�Ľṹ��ָ�� 
	_In_opt_ PVOID Argument2
	);

// ��ȡע�������·��
BOOLEAN GetRegisterObjectCompletePath(PUNICODE_STRING pRegistryPath, PVOID pRegistryObject);

// �ж��Ƿ��Ǳ���ע���·��
BOOLEAN IsProtectReg(UNICODE_STRING ustrRegPath);



// ע���ص�Cookie
LARGE_INTEGER g_liRegCookie;


#endif