#include <stdio.h>
#ifndef UNICODE
#define UNICODE
#endif
#pragma comment(lib,"netapi32")

#include  <stdio.h>
#include  <windows.h>
#include  <lm.h>
#include<tchar.h>

int mainM()
{
	LPCWSTR servername = NULL;
	LPCWSTR localgroupname = _T("Administrators");
	DWORD LEVEL = 1;
	LPLOCALGROUP_MEMBERS_INFO_1 bufptr;
	DWORD entriesread;
	DWORD totalentries;
	NET_API_STATUS nStatus;
	do
	{
		nStatus = NetLocalGroupGetMembers(NULL, localgroupname,
			LEVEL, (LPBYTE*)&bufptr, MAX_PREFERRED_LENGTH, &entriesread, &totalentries, 0);
		if (nStatus == NERR_Success)
		{
			LPLOCALGROUP_MEMBERS_INFO_1 pTmpBuf;
			DWORD i;
			if ((pTmpBuf = bufptr) != NULL)
			{
				for (i = 0; i < entriesread; i++)
				{
					if (pTmpBuf == NULL)
					{
						fprintf(stderr, "An access violation has occurred\n");
						break;
					}
					wprintf(L"userName[%u]:%ws\n", i, pTmpBuf->lgrmi1_name);
					pTmpBuf++;
				}
			}
		}
	} while (false);
	if (bufptr != NULL)   // Õ∑≈ª∫≥Â  
		NetApiBufferFree(bufptr);
	return 0;
}