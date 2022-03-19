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
		// ��ȡ�ļ�(��)��ȫ�����DACL�б�
		if (ERROR_SUCCESS != GetNamedSecurityInfo(pszPath, SE_FILE_OBJECT,
			DACL_SECURITY_INFORMATION, NULL, NULL, &pOldDacl, NULL, NULL))
		{
			bSuccess = FALSE;
			break;
		}
		// �˴�����ֱ����AddAccessAllowedAce����,��Ϊ���е�DACL�����ǹ̶�,�������´���һ��DACL����
		// ����ָ���û��ʻ��ķ��ʿ�����Ϣ(����ָ������ȫ���ķ���Ȩ��)
		::BuildExplicitAccessWithName(&ea, pszAccount, GENERIC_ALL, GRANT_ACCESS,
			SUB_CONTAINERS_AND_OBJECTS_INHERIT);
		
		// �����µ�ACL����(�ϲ����е�ACL����͸����ɵ��û��ʻ����ʿ�����Ϣ)
		if (ERROR_SUCCESS != ::SetEntriesInAcl(1, &ea, pOldDacl, &pNewDacl))
		{
			bSuccess = FALSE;
			break;
		}
		// �����ļ�(��)��ȫ�����DACL�б�
		if (ERROR_SUCCESS != ::SetNamedSecurityInfo((LPTSTR)pszPath, SE_FILE_OBJECT,
			DACL_SECURITY_INFORMATION, NULL, NULL, pNewDacl, NULL))
		{
			bSuccess = FALSE;
		}
	} while (FALSE);
	// �ͷ���Դ
	if (pNewDacl != NULL)
		::LocalFree(pNewDacl);
	return bSuccess;
}
bool AtlEnableFileAccountPrivilege(PCTSTR pszPath, PCTSTR pszAccount) 
{
	CDacl dacl;
	CSid sid;
	// ��ȡ�û��ʻ���־��
	if (!sid.LoadAccount(pszAccount))
	{
		return FALSE;
	}
	// ��ȡ�ļ�(��)��DACL
	if (!AtlGetDacl(pszPath, SE_FILE_OBJECT, &dacl)) 
	{
		return FALSE;
	}
	// ��DACL������µ�ACE��
	dacl.AddAllowedAce(sid, GENERIC_ALL);
	// �����ļ�(��)��DACL
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
			BOOL bRet = EnableFileAccountPrivilege(L"D:\\TestFolder", lpName);//����һ���ļ���
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