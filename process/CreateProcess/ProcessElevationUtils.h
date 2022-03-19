#pragma once
#include<Windows.h>
#include<Shlobj.h>
#include"ProcessHelper.h"
#include<WinNT.h>
#pragma comment (lib,"Shell32.lib")

class ProcessElevationUtils
{
public:
	static BOOL EnbalePrivileges(HANDLE hProcess, const TCHAR *pszPrivilegesName)
	{
		HANDLE hToken = NULL;
		LUID luidValue = { 0 };
		TOKEN_PRIVILEGES tokenPrivileges = { 0 };
		BOOL bRet = FALSE;
		DWORD dwRet = 0;
		// �򿪽������Ʋ���ȡ���� TOKEN_ADJUST_PRIVILEGES Ȩ�޵Ľ������ƾ��
		bRet = ::OpenProcessToken(hProcess, TOKEN_ADJUST_PRIVILEGES, &hToken);
		if (FALSE == bRet)
		{
			return FALSE;
		}
		// ��ȡ����ϵͳ�� pszPrivilegesName ��Ȩ��LUIDֵ
		bRet = ::LookupPrivilegeValue(NULL, pszPrivilegesName, &luidValue);
		if (FALSE == bRet)
		{
			return FALSE;
		}
		// ��������Ȩ����Ϣ
		tokenPrivileges.PrivilegeCount = 1;
		tokenPrivileges.Privileges[0].Luid = luidValue;
		tokenPrivileges.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;						//���ô���Ȩ
		// �����������Ʒ���Ȩ��
		bRet = ::AdjustTokenPrivileges(hToken, FALSE, &tokenPrivileges, 0, NULL, NULL);
		if (FALSE == bRet)
		{
			return FALSE;
		}
		else
		{
			// ���ݴ������ж��Ƿ���Ȩ�����óɹ�
			dwRet = ::GetLastError();
			if (ERROR_SUCCESS == dwRet)
			{
				return TRUE;
			}
			else if (ERROR_NOT_ALL_ASSIGNED == dwRet)
			{
				return FALSE;
			}
		}
		return FALSE;
	}
	//��ȡ��Ȩ����
	//TokenElevationTypeDefault:������Ĭ���û����У�����UAC������
	//TokenElevationTypeFull:���̵�Ȩ�ޱ��ɹ���������������û�б�ɸѡ��
	//TokenElevationTypeLimited:���������޵�Ȩ�����У�����Ӧ��һ��ɸѡ��������
	static BOOL GetProcessElevatin(HANDLE hProcess,TOKEN_ELEVATION_TYPE* pElevationType, BOOL* pIsAdmin)
	{
		HANDLE hToken = NULL;
		DWORD dwSize = 0;
		if (!OpenProcessToken(hProcess, TOKEN_QUERY, &hToken))
		{
			return FALSE;
		}
		BOOL bResult = FALSE;
		//��ȡ������Ϣ
		if (GetTokenInformation(hToken, TokenElevationType, pElevationType, sizeof(TOKEN_ELEVATION_TYPE), &dwSize))
		{
			BYTE adminSID[SECURITY_MAX_SID_SIZE] = { 0 };
			dwSize = sizeof(adminSID);
			CreateWellKnownSid(WELL_KNOWN_SID_TYPE::WinBuiltinAccessControlAssistanceOperatorsSid, NULL, &adminSID, &dwSize);
			//������Ʊ�ɸѡ��
			if (*pElevationType == TokenElevationTypeLimited)
			{
				HANDLE hUnfilteredToken = NULL;
				GetTokenInformation(hToken, TokenLinkedToken, &hUnfilteredToken, sizeof(HANDLE), &dwSize);
				//��������Ƿ����һ������ԱSID
				if (CheckTokenMembership(hUnfilteredToken, &adminSID, pIsAdmin))
				{
					bResult = TRUE;
				}
				CloseHandle(hUnfilteredToken);
			}
			//�������δ��ɸѡ��������ֱ�ӻ�ȡ��Ȩ��
			else
			{
				*pIsAdmin = IsUserAnAdmin();
				bResult = TRUE;
			}
		}
		CloseHandle(hToken);
		return bResult;
	}
	static bool IsRunasAdmin()
	{
		bool bElevated = false;
		HANDLE hToken = NULL;

		if (!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken))
		{
			return false;
		}
		TOKEN_ELEVATION tokenEle;
		DWORD dwRetLen = 0;

		if (GetTokenInformation(hToken, TokenElevation, &tokenEle, sizeof(tokenEle), &dwRetLen))
		{
			if (dwRetLen == sizeof(tokenEle))
			{
				bElevated = tokenEle.TokenIsElevated;
			}
		}

		CloseHandle(hToken);
		return bElevated;
	}
	static bool IsSystem()
	{
		stdstr str = ProcessHelper::GetProcessUser();
		for (auto it = str.begin(); it != str.end(); it++)
		{
			*it = toupper(*it);
		}
		if (str == _T("SYSTEM"))
		{
			return true;
		}
		return false;
	}
	static SECURITY_ATTRIBUTES GetPSA(bool bInheritHandle = false)
	{
		SECURITY_ATTRIBUTES psa = { 0 };
		psa.bInheritHandle = bInheritHandle;
	}

	//ͨ��������������Ȩ��������
	static BOOL UACWhiteListOpen(const TCHAR* path)
	{
		HKEY hKey = NULL;
		::RegCreateKeyEx(HKEY_CURRENT_USER, _T("Software\\Classes\\mscfile\\shell\\open\\command"), 0, NULL, 0,
			KEY_WOW64_64KEY | KEY_ALL_ACCESS, NULL, &hKey, NULL);
		if (hKey == NULL)
		{
			return FALSE;
		}
		
		//���ü�ֵ
		::RegSetValueEx(hKey, NULL, 0, REG_SZ, (BYTE*)path, (1 + _tcslen(path)) * sizeof(TCHAR));
		::RegCloseKey(hKey);
	}
};