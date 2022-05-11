#include  <stdio.h>
#include  <lm.h>
#include <afxcoll.h>
#include<cstring>
#ifndef UNICODE
#define UNICODE
#endif
#pragma comment(lib,"netapi32")



//获取所有用户
void GetOSUserName(CStringArray &userarray)
{
	LPUSER_INFO_1 pBuf = NULL;
	LPUSER_INFO_1 pTmpBuf;
	DWORD dwLevel = 1;
	DWORD dwPrefMaxLen = -1;
	DWORD dwEntriesRead = 0;
	DWORD dwTotalEntries = 0;
	DWORD dwResumeHandle = 0;
	NET_API_STATUS nStatus;
	LPTSTR pszServerName = NULL;
	do
	{
		nStatus = NetUserEnum((LPCWSTR)pszServerName, dwLevel, FILTER_NORMAL_ACCOUNT,
			(LPBYTE*)&pBuf, dwPrefMaxLen, &dwEntriesRead, &dwTotalEntries, &dwResumeHandle);

		if ((nStatus == NERR_Success) || (nStatus == ERROR_MORE_DATA))
		{
			if ((pTmpBuf = pBuf) != NULL)
			{
				for (DWORD i = 0; i < dwEntriesRead; ++i)
				{
					CString csFlag;
					csFlag.Format(_T("%s,%ld"), pTmpBuf->usri1_name, pBuf->usri1_priv);
					if (pTmpBuf->usri1_priv == USER_PRIV_ADMIN ||
						pTmpBuf->usri1_priv == USER_PRIV_USER &&
						(CString(pTmpBuf->usri1_name) != "ASPNET"))
					{
						userarray.Add((CString)pTmpBuf->usri1_name);
					}
					pTmpBuf++;
				}
			}
		}
		else
		{
			MessageBox(NULL, _T("A system error has occurred"), _T("ERROR"), MB_OK);
		}
	}while (nStatus == ERROR_MORE_DATA);
	if (pBuf != NULL)
	{
		NetApiBufferFree(pBuf);
		pBuf = nullptr;
	}
}