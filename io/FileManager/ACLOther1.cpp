// ConsoleApplication1.cpp : Defines the entry point for the console application.
//
#include <windows.h>
#include <iostream>
#include <atlstr.h>
#include <AccCtrl.h>
#include <Aclapi.h>
#include <atlsecurity.h>
#pragma comment(lib,"Advapi32.lib")
#pragma comment(lib,"Netapi32.lib")

#include<LMShare.h>
#include <lm.h>
#include"../../../Process/WindowsProcessHelper/CreateProcess/SessionManager.h"
using namespace std;
BOOL EnableFileAccountPrivilege(LPTSTR pszPath, LPTSTR pszAccount)
{
	BOOL bSuccess = TRUE;
	PACL pNewDacl = NULL, pOldDacl = NULL;
	EXPLICIT_ACCESS ea;
	do
	{
		// 获取文件(夹)安全对象的DACL列表
		if (ERROR_SUCCESS != GetNamedSecurityInfo(pszPath, SE_FILE_OBJECT,
			DACL_SECURITY_INFORMATION, NULL, NULL, &pOldDacl, NULL, NULL))
		{
			bSuccess = FALSE;
			break;
		}
		// 此处不可直接用AddAccessAllowedAce函数,因为已有的DACL长度是固定,必须重新创建一个DACL对象
		// 生成指定用户帐户的访问控制信息(这里指定赋予全部的访问权限)
		::BuildExplicitAccessWithName(&ea, pszAccount, GENERIC_ALL, GRANT_ACCESS,
			SUB_CONTAINERS_AND_OBJECTS_INHERIT);
		
		// 创建新的ACL对象(合并已有的ACL对象和刚生成的用户帐户访问控制信息)
		if (ERROR_SUCCESS != ::SetEntriesInAcl(1, &ea, pOldDacl, &pNewDacl))
		{
			bSuccess = FALSE;
			break;
		}
		// 设置文件(夹)安全对象的DACL列表
		if (ERROR_SUCCESS != ::SetNamedSecurityInfo((LPTSTR)pszPath, SE_FILE_OBJECT,
			DACL_SECURITY_INFORMATION, NULL, NULL, pNewDacl, NULL))
		{
			bSuccess = FALSE;
		}
	} while (FALSE);
	// 释放资源
	if (pNewDacl != NULL)
		::LocalFree(pNewDacl);
	return bSuccess;
}
bool AtlEnableFileAccountPrivilege(PCTSTR pszPath, PCTSTR pszAccount) 
{
	CDacl dacl;
	CSid sid;
	// 获取用户帐户标志符
	if (!sid.LoadAccount(pszAccount))
	{
		return FALSE;
	}
	// 获取文件(夹)的DACL
	if (!AtlGetDacl(pszPath, SE_FILE_OBJECT, &dacl)) 
	{
		return FALSE;
	}
	// 在DACL中添加新的ACE项
	dacl.AddAllowedAce(sid, GENERIC_ALL);
	// 设置文件(夹)的DACL
	return AtlSetDacl(pszPath, SE_FILE_OBJECT, dacl);
}
void main3333(void)
{
	DWORD i, dwSize = 0, dwResult = 0;
	HANDLE hToken;
	PTOKEN_GROUPS pGroupInfo;
	SID_NAME_USE SidType;
	TCHAR lpName[MAX_PATH * 2];
	TCHAR lpDomain[MAX_PATH * 2];
	BYTE sidBuffer[MAX_PATH * 2];
	PSID pSID = (PSID)&sidBuffer;
	SID_IDENTIFIER_AUTHORITY SIDAuth = SECURITY_NT_AUTHORITY;
	DWORD dwLength = 0;

	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ALL_ACCESS, &hToken))
	{
		printf("OpenProcessToken Error %u\n", GetLastError());
	}
	if (!GetTokenInformation(hToken, TokenGroups, NULL, dwSize, &dwSize))
	{
		dwResult = GetLastError();
		if (dwResult != ERROR_INSUFFICIENT_BUFFER) 
		{
			printf("GetTokenInformation Error %u\n", dwResult);
		}
	}
	pGroupInfo = (PTOKEN_GROUPS)GlobalAlloc(GPTR, dwSize);
	if (!GetTokenInformation(hToken, TokenGroups, pGroupInfo, dwSize, &dwSize))
	{
		printf("GetTokenInformation Error %u\n", GetLastError());
	}

	if (!AllocateAndInitializeSid(&SIDAuth, 2, SECURITY_BUILTIN_DOMAIN_RID,
		DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0, 0, 0, 0, &pSID))
	{
		printf("AllocateAndInitializeSid Error %u\n", GetLastError());
	}

	for (i = 0; i < pGroupInfo->GroupCount; i++) 
	{
		dwSize = MAX_PATH;
		if (!LookupAccountSid(NULL, pGroupInfo->Groups[i].Sid, lpName, &dwSize, lpDomain, &dwSize, &SidType))
		{
			dwResult = GetLastError();
			if (dwResult == ERROR_NONE_MAPPED)
				wcscpy_s(lpName, dwSize, L"NONE_MAPPED");
			else 
			{
				printf("LookupAccountSid Error %u\n", GetLastError());
			}
		}
		else 
		{
			BOOL bRet = EnableFileAccountPrivilege(L"D:\\TestFolder", lpName);//创建一个文件夹
			if (!bRet)
			{
				printf("EnableFileAccountPrivilege Error: %d \n", GetLastError());
			}
			bRet = AtlEnableFileAccountPrivilege(L"D:\\TestFolder", lpName);
			if (!bRet)
			{
				printf("AtlEnableFileAccountPrivilege Error: %d \n", GetLastError());
			}
		}
	}

	if (pSID)
		FreeSid(pSID);
	if (pGroupInfo)
		GlobalFree(pGroupInfo);
}