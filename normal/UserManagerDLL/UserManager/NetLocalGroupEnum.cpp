#ifndef UNICODE
#define UNICODE
#endif
#pragma comment(lib,"netapi32")

#include  <stdio.h>
#include  <windows.h>
#include  <lm.h>

NET_API_STATUS GetGroupList(WCHAR** GroupList)
{
	LPCWSTR servername = NULL;
	DWORD level = 0;
	BYTE *bufptr = NULL;
	DWORD prefmaxlen = MAX_PREFERRED_LENGTH;
	DWORD entriesread;
	DWORD totalentries;
	DWORD_PTR resumehandle = NULL;

	NET_API_STATUS nStatus;

	LOCALGROUP_INFO_0* gInfo;

	nStatus = NetLocalGroupEnum(NULL, 0, &bufptr, prefmaxlen, &entriesread, &totalentries, &resumehandle);

	if (nStatus == NO_ERROR)
	{
		gInfo = (LOCALGROUP_INFO_0*)bufptr;
		for (DWORD i = 0; i < entriesread; i++)
		{
			WCHAR gName[260] = { 0 };
			lstrcpy(GroupList[i], gInfo->lgrpi0_name);
			wprintf(L"Group Name[%d] : %s\n", i, GroupList[i]);
			gInfo++;
		}
	}
	if (bufptr != nullptr)
	{
		NetApiBufferFree(bufptr);
		bufptr = nullptr;
	}
	return entriesread;
}
void main9()
{
	WCHAR *gName[30] = { 0 };
	for (int i = 0; i < 30; i++)
	{
		gName[i] = (WCHAR*)malloc(sizeof(WCHAR) * 260);
	}
	GetGroupList(gName);
	for (int i = 0; i < 30; i++)
	{
		free(gName[i]);
		gName[i] = NULL;
	}
}