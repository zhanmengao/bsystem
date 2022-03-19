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
	�������û���������������Ŀ¼��ϵͳĿ¼��
	*/
	WCHAR    lpUserName[50], lpGroupName[100];
	DWORD   nSize = sizeof(lpUserName);

	LPLOCALGROUP_USERS_INFO_0 pBuf = NULL;
	DWORD   dwEntriesRead = 0;
	DWORD   dwTotalEntries = 0;
	NET_API_STATUS   nStatus;

	ZeroMemory(lpUserName, sizeof(lpUserName));

	//��ȡϵͳ��Ŀ¼������long���͡����Ŀ¼�Ĵ�С����size��ֵ����ô������Ҫ��Ŀ¼��С��С�ڵĻ�������ʵ�ʵĴ�С��
	GetUserName(lpUserName, &nSize);
	/*
	��ȡ�û�����Ϣ���鿴MSDN
	Ӧ�ó���ʹ��NetUserGetLocalGroups���������������û�����������б�
	����ʹ�� Microsoft Windows Server 2003 �У����� Microsoft Windows XP ����NetUserGetLocalGroups������Ӧ�ó���ʱ����Ӧ�ó�����ܻ�й©�ڴ档
	Ӧ�ó���ʹ��NetUserGetLocalGroups���������������û�����������б�
	����û��������κα����飬 NetUserGetLocalGroups���������ͷ������ѷ�����ڴ�ռ䡣
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
	if (pBuf != NULL)   //�ͷŻ���  
		NetApiBufferFree(pBuf);
}