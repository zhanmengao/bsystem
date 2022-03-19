#pragma once
#include<Windows.h>

class HandleUtils
{
public:
	//���ƾ������������
	static HANDLE CopyHandle(HANDLE hSrcProcess, HANDLE hTargetProcess, HANDLE handle,
		DWORD access = 0, BOOL bInherit = FALSE, DWORD option = DUPLICATE_SAME_ACCESS);
	static DWORD GetHandleFlags(HANDLE handle);																		//��ȡ����ĵ�ǰ��־
	static BOOL SetHandleInherit(HANDLE handle, bool bInherit);													//���þ���Ƿ�ɼ̳�
	static bool GetHandleInherit(HANDLE handle);
	static BOOL SetHandleProtectFromClose(HANDLE handle, bool bInherit);								//���þ���Ƿ�ɹر�
	static SECURITY_ATTRIBUTES GetInheritSa();																				//��ȡ��ȫ�ṹ_��־�ɼ̳�
	static void MakeSa(SECURITY_ATTRIBUTES& sa,bool bInheritHandle);										//���ð�ȫ������

	static HANDLE dup2(HANDLE hSrcProcess, HANDLE hSrcHandle, HANDLE hTargetProcess);				//����һ�����
};