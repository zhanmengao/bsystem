#include"PrivilegeUp.h"
//��Ĺ��ܽ���:
//����������������������������������������-
//#define            SE_BACKUP_NAME               TEXT(��SeBackupPrivilege��)     //��������Ȩ��
//#define            SE_RESTORE_NAME              TEXT(��SeRestorePrivilege��)    //�ָ�����Ȩ��
//#define            SE_SHUTDOWN_NAME             TEXT(��SeShutdownPrivilege��)   //�ػ�Ȩ��
//#define            SE_DEBUG_NAME                TEXT(��SeDebugPrivilege��)      //����д����Ȩ��
//����������������������������������������-

//��Ĺ��ܽ���:
//����������������������������������������-
//ABOVE_NORMAL_PRIORITY_CLASS(0x00008000)   ���ڱ�׼
//BELOW_NORMAL_PRIORITY_CLASS(0x00004000)   ���ڱ�׼
//HIGH_PRIORITY_CLASS(0x00000080)           ��
//IDLE_PRIORITY_CLASS(0x00000040)           ��
//NORMAL_PRIORITY_CLASS(0x00000020)         ��׼
//REALTIME_PRIORITY_CLASS(0x00000100)       ʵʱ
//����������������������������������������-

//
//���õ�ǰ�������ȼ�Ϊ���(ʵʱ)
//
//����ֵ����false����ʧ�ܣ���true���ǳɹ���
bool CEnablePriv::SetRealTimePriority()
{
	if (!SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS))
	{
		return false;
	}

	return true;
}

//
//������ǰ����Ȩ�޺���(��SeShutdownPrivilege���ػ�Ȩ��)
//
//����ֵ����false����ʧ�ܣ���true���ǳɹ���
bool CEnablePriv::EnableShutdownPriv()
{
	HANDLE hToken;
	LUID sedebugnameValue;
	TOKEN_PRIVILEGES tkp;

	if (!OpenProcessToken(GetCurrentProcess(),
		TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
		return false;

	if (!LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &sedebugnameValue))
	{
		CloseHandle(hToken);
		return false;
	}

	tkp.PrivilegeCount = 1;
	tkp.Privileges[0].Luid = sedebugnameValue;
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	if (!AdjustTokenPrivileges(hToken, FALSE, &tkp, sizeof tkp, NULL, NULL))
		CloseHandle(hToken);

	return true;
}

//
//������ǰ����Ȩ�޺���(��SeDebugPrivilege������д����Ȩ��)
//
//����ֵ����false����ʧ�ܣ���true���ǳɹ���
bool CEnablePriv::EnableDebugPriv()
{
	HANDLE hToken;
	LUID sedebugnameValue;
	TOKEN_PRIVILEGES tkp;

	if (!OpenProcessToken(GetCurrentProcess(),
		TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
		return false;

	if (!LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &sedebugnameValue))
	{
		CloseHandle(hToken);
		return false;
	}

	tkp.PrivilegeCount = 1;
	tkp.Privileges[0].Luid = sedebugnameValue;
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	if (!AdjustTokenPrivileges(hToken, FALSE, &tkp, sizeof tkp, NULL, NULL))
		CloseHandle(hToken);
	printf("Privailege : %i", GetLastError());
	return true;
}

//
//������ǰ����Ȩ�޺���(��SeBackupPrivilege����������Ȩ��)
//
//����ֵ����false����ʧ�ܣ���true���ǳɹ���
bool CEnablePriv::EnableBackupPriv()
{
	HANDLE hToken;
	LUID sedebugnameValue;
	TOKEN_PRIVILEGES tkp;

	if (!OpenProcessToken(GetCurrentProcess(),
		TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
		return false;

	if (!LookupPrivilegeValue(NULL, SE_BACKUP_NAME, &sedebugnameValue))
	{
		CloseHandle(hToken);
		return false;
	}

	tkp.PrivilegeCount = 1;
	tkp.Privileges[0].Luid = sedebugnameValue;
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	if (!AdjustTokenPrivileges(hToken, FALSE, &tkp, sizeof tkp, NULL, NULL))
		CloseHandle(hToken);

	return true;
}

//
//������ǰ����Ȩ�޺���(��SeRestorePrivilege���ָ�����Ȩ��)
//
//����ֵ����false����ʧ�ܣ���true���ǳɹ���
bool CEnablePriv::EnableRestorePriv()
{
	HANDLE hToken;
	LUID sedebugnameValue;
	TOKEN_PRIVILEGES tkp;

	if (!OpenProcessToken(GetCurrentProcess(),
		TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
		return false;

	if (!LookupPrivilegeValue(NULL, SE_RESTORE_NAME, &sedebugnameValue))
	{
		CloseHandle(hToken);
		return false;
	}

	tkp.PrivilegeCount = 1;
	tkp.Privileges[0].Luid = sedebugnameValue;
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	if (!AdjustTokenPrivileges(hToken, FALSE, &tkp, sizeof tkp, NULL, NULL))
		CloseHandle(hToken);

	return true;
}