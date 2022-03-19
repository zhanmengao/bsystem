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
		// 打开进程令牌并获取具有 TOKEN_ADJUST_PRIVILEGES 权限的进程令牌句柄
		bRet = ::OpenProcessToken(hProcess, TOKEN_ADJUST_PRIVILEGES, &hToken);
		if (FALSE == bRet)
		{
			return FALSE;
		}
		// 获取本地系统的 pszPrivilegesName 特权的LUID值
		bRet = ::LookupPrivilegeValue(NULL, pszPrivilegesName, &luidValue);
		if (FALSE == bRet)
		{
			return FALSE;
		}
		// 设置提升权限信息
		tokenPrivileges.PrivilegeCount = 1;
		tokenPrivileges.Privileges[0].Luid = luidValue;
		tokenPrivileges.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;						//启用此特权
		// 提升进程令牌访问权限
		bRet = ::AdjustTokenPrivileges(hToken, FALSE, &tokenPrivileges, 0, NULL, NULL);
		if (FALSE == bRet)
		{
			return FALSE;
		}
		else
		{
			// 根据错误码判断是否特权都设置成功
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
	//获取提权类型
	//TokenElevationTypeDefault:进程以默认用户运行，或者UAC被禁用
	//TokenElevationTypeFull:进程的权限被成功提升，并且令牌没有被筛选过
	//TokenElevationTypeLimited:进程以受限的权限运行，它对应于一个筛选过的令牌
	static BOOL GetProcessElevatin(HANDLE hProcess,TOKEN_ELEVATION_TYPE* pElevationType, BOOL* pIsAdmin)
	{
		HANDLE hToken = NULL;
		DWORD dwSize = 0;
		if (!OpenProcessToken(hProcess, TOKEN_QUERY, &hToken))
		{
			return FALSE;
		}
		BOOL bResult = FALSE;
		//获取令牌信息
		if (GetTokenInformation(hToken, TokenElevationType, pElevationType, sizeof(TOKEN_ELEVATION_TYPE), &dwSize))
		{
			BYTE adminSID[SECURITY_MAX_SID_SIZE] = { 0 };
			dwSize = sizeof(adminSID);
			CreateWellKnownSid(WELL_KNOWN_SID_TYPE::WinBuiltinAccessControlAssistanceOperatorsSid, NULL, &adminSID, &dwSize);
			//如果令牌被筛选过
			if (*pElevationType == TokenElevationTypeLimited)
			{
				HANDLE hUnfilteredToken = NULL;
				GetTokenInformation(hToken, TokenLinkedToken, &hUnfilteredToken, sizeof(HANDLE), &dwSize);
				//检查令牌是否包含一个管理员SID
				if (CheckTokenMembership(hUnfilteredToken, &adminSID, pIsAdmin))
				{
					bResult = TRUE;
				}
				CloseHandle(hUnfilteredToken);
			}
			//如果令牌未被筛选过，可以直接获取其权限
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

	//通过白名单程序提权启动程序
	static BOOL UACWhiteListOpen(const TCHAR* path)
	{
		HKEY hKey = NULL;
		::RegCreateKeyEx(HKEY_CURRENT_USER, _T("Software\\Classes\\mscfile\\shell\\open\\command"), 0, NULL, 0,
			KEY_WOW64_64KEY | KEY_ALL_ACCESS, NULL, &hKey, NULL);
		if (hKey == NULL)
		{
			return FALSE;
		}
		
		//设置键值
		::RegSetValueEx(hKey, NULL, 0, REG_SZ, (BYTE*)path, (1 + _tcslen(path)) * sizeof(TCHAR));
		::RegCloseKey(hKey);
	}
};