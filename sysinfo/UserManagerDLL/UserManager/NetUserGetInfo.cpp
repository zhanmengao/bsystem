#ifndef UNICODE
#define UNICODE
#endif
#pragma comment(lib,"netapi32")

#include  <stdio.h>
#include  <windows.h>
#include  <lm.h>


int main333()
{
	// 定义USER_INFO_1结构体
	DWORD dwLevel = 1;
	DWORD dwError = 0;
	WCHAR    lpUserName[50] = { 0 };
	DWORD   nSize = sizeof(lpUserName);
	GetUserName(lpUserName, &nSize);
	LPUSER_INFO_1 uInfo;
	auto nState = NetUserGetInfo(NULL, lpUserName, dwLevel, (LPBYTE*)&uInfo);
	printf("flag:%u", uInfo->usri1_flags);
	if (uInfo != nullptr)
	{
		NetApiBufferFree(uInfo);
		uInfo = nullptr;
	}
	return 0;
}