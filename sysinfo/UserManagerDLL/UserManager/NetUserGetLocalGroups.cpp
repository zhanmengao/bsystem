#ifndef UNICODE
#define UNICODE
#endif
#pragma comment(lib,"netapi32")

#include  <stdio.h>
#include  <windows.h>
#include  <lm.h>

void main7()
{
	/*
	先声明用户名，组名，驱动目录，系统目录。
	*/
	WCHAR    lpUserName[50], lpGroupName[100];
	DWORD   nSize = sizeof(lpUserName);

	LPLOCALGROUP_USERS_INFO_0 pBuf = NULL;
	DWORD   dwEntriesRead = 0;
	DWORD   dwTotalEntries = 0;
	NET_API_STATUS   nStatus;

	ZeroMemory(lpUserName, sizeof(lpUserName));

	//获取系统的目录；返回long类型。如果目录的大小大于size的值，那么返回需要的目录大小。小于的话，返回实际的大小。
	GetUserName(lpUserName, &nSize);
	/*
	提取用户的信息，查看MSDN
	应用程序使用NetUserGetLocalGroups函数来检索本地用户所属的组的列表
	当您使用 Microsoft Windows Server 2003 中，或在 Microsoft Windows XP 调用NetUserGetLocalGroups函数的应用程序时，该应用程序可能会泄漏内存。
	应用程序使用NetUserGetLocalGroups函数来检索本地用户所属的组的列表。
	如果用户不属于任何本地组， NetUserGetLocalGroups函数不会释放所有已分配的内存空间。
	*/
	nStatus = NetUserGetLocalGroups(NULL,
		lpUserName,
		0,
		LG_INCLUDE_INDIRECT,
		(LPBYTE   *)&pBuf,
		MAX_PREFERRED_LENGTH,
		&dwEntriesRead,
		&dwTotalEntries);

	if (nStatus == NERR_Success)   //If the function succeeds, the return value is NERR_Success.  
	{
		LPLOCALGROUP_USERS_INFO_0 pTmpBuf;
		DWORD i;

		if ((pTmpBuf = pBuf) != NULL)
		{
			for (i = 0; i < dwEntriesRead; i++)
			{
				if (pTmpBuf == NULL)
					break;
				lstrcpy(lpGroupName, pTmpBuf->lgrui0_name);
				wprintf(L"group[%u]:%s \n", i, pTmpBuf->lgrui0_name);
				pTmpBuf++;
			}
		}
	}
	if (pBuf != NULL)   //释放缓冲  
		NetApiBufferFree(pBuf);
}