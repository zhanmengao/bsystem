#include "stdafx.h"
#include "ServiceOperate.h"


void ShowError(char *lpszText)
{
	char szErr[MAX_PATH] = { 0 };
	::wsprintf(szErr, "%s Error!\nError Code Is:%d\n", lpszText, ::GetLastError());
#ifdef _DEBUG
	::MessageBox(NULL, szErr, "ERROR", MB_OK | MB_ICONERROR);
#endif
}


// 0 ���ط���    1 ��������    2 ֹͣ����    3 ɾ������
BOOL SystemServiceOperate(char *lpszExePath, int iOperateType)
{
	BOOL bRet = TRUE;
	char szName[MAX_PATH] = { 0 };

	::lstrcpy(szName, lpszExePath);
	// ���˵��ļ�Ŀ¼����ȡ�ļ���
	::PathStripPath(szName);                   

	SC_HANDLE shOSCM = NULL, shCS = NULL;
	SERVICE_STATUS ss;
	DWORD dwErrorCode = 0;
	BOOL bSuccess = FALSE;
	// �򿪷�����ƹ��������ݿ�
	shOSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (!shOSCM)
	{
		ShowError("OpenSCManager");
		return FALSE;
	}

	if (0 != iOperateType)
	{
		// ��һ���Ѿ����ڵķ���
		shCS = OpenService(shOSCM, szName, SERVICE_ALL_ACCESS);
		if (!shCS)
		{
			ShowError("OpenService");
			::CloseServiceHandle(shOSCM);
			shOSCM = NULL;
			return FALSE;
		}
	}

	switch (iOperateType)
	{
	case 0:
	{
		// ��������
		// SERVICE_AUTO_START   ��ϵͳ�Զ�����
		// SERVICE_DEMAND_START �ֶ�����
		shCS = ::CreateService(shOSCM, szName, szName,
			SERVICE_ALL_ACCESS,
			SERVICE_WIN32_OWN_PROCESS | SERVICE_INTERACTIVE_PROCESS,
			SERVICE_AUTO_START,
			SERVICE_ERROR_NORMAL,
			lpszExePath, NULL, NULL, NULL, NULL, NULL);
		if (!shCS)
		{
			ShowError("CreateService");
			bRet = FALSE;
		}
		break;
	}
	case 1:
	{
		// ��������
		if (!::StartService(shCS, 0, NULL))
		{
			ShowError("StartService");
			bRet = FALSE;
		}
		break;
	}
	case 2:
	{
		// ֹͣ����
		if (!::ControlService(shCS, SERVICE_CONTROL_STOP, &ss))
		{
			ShowError("ControlService");
			bRet = FALSE;
		}
		break;
	}
	case 3:
	{
		// ɾ������
		if (!::DeleteService(shCS))
		{
			ShowError("DeleteService");
			bRet = FALSE;
		}
		break;
	}
	default:
		break;
	}
	// �رվ��
	if (shCS)
	{
		::CloseServiceHandle(shCS);
		shCS = NULL;
	}
	if (shOSCM)
	{
		::CloseServiceHandle(shOSCM);
		shOSCM = NULL;
	}

	return bRet;
}