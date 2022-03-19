#pragma once
#pragma once
#include<Windows.h>
#include <Aclapi.h>
#include <lmshare.h>
#include <LM.h>
#include<string>
#include"../../../../C++space/StringTools/StringTools/StringHead.h"
#pragma comment(lib,"Netapi32.lib")
class FileAttrHelperEx
{
public:
	static BOOL EnableFileAccountPrivilege(TCHAR *lpszFileName, TCHAR *lpszAccountName,
		DWORD dwAccessMask = GENERIC_ALL);
	static DWORD SetFolderShare(const WCHAR* strFolderPath, const WCHAR* strShareName,
		const TCHAR* strUserName, DWORD dwAccessMask = GENERIC_ALL);
	static DWORD SetFolderPrivate(const WCHAR* strShareName);
private:
	static BOOL  SetAce(const stdstr& strAccountName, PACL pacl, DWORD accessMask);
	static BOOL GetAccountRights(TCHAR *lpszFileName, TCHAR *lpszAccountName, int(&arrRights)[32]);
	static BOOL AltEnableFileAccountPrivilege(PCTSTR pszPath, PCTSTR pszAccount,
		DWORD dwAccessMask = GENERIC_ALL);
};
/*
#define GENERIC_READ                     (0x80000000L)
#define GENERIC_WRITE                    (0x40000000L)
#define GENERIC_EXECUTE                  (0x20000000L)
#define GENERIC_ALL                      (0x10000000L)
*/
