#ifndef UNICODE
#define UNICODE
#endif
#pragma comment(lib,"netapi32")

#include  <stdio.h>
#include  <windows.h>
#include  <lm.h>
#define USER_NAME  L"qaq"
#define OLD_PASS  L"P@ssword233"
#define NEW_PASS  L"qaq1234567"


int main1()
{
	// ����USER_INFO_1�ṹ��
	USER_INFO_1 UserInfo;
	DWORD dwLevel = 1;
	DWORD dwError = 0;

	UserInfo.usri1_name = USER_NAME;            // �˻�    
	UserInfo.usri1_password = OLD_PASS;      // ����
	UserInfo.usri1_priv = USER_PRIV_USER;
	UserInfo.usri1_home_dir = NULL;
	UserInfo.usri1_comment = NULL;
	UserInfo.usri1_flags = UF_SCRIPT;
	UserInfo.usri1_script_path = NULL;

	//�����ΪAdmins���û�,����ΪP@ssword233 
	auto nState = NetUserAdd(NULL, dwLevel, (LPBYTE)&UserInfo, &dwError);

	// ����û���administrators��
	LOCALGROUP_MEMBERS_INFO_3 account;
	account.lgrmi3_domainandname = UserInfo.usri1_name;
	nState = NetLocalGroupAddMembers(NULL, L"Administrators", 3, (LPBYTE)&account, 1);

	//������ɾ���û�
	nState = NetLocalGroupDelMembers(NULL, L"Administrators", 3, (LPBYTE)&account, 1);

	//����û�������
	nState = NetLocalGroupAddMember(NULL, USER_NAME, NULL);
	//������ɾ���û�
	nState = NetLocalGroupDelMember(NULL, USER_NAME, NULL);
	//�޸�����
	nState = NetUserChangePassword(NULL, USER_NAME, OLD_PASS, NEW_PASS);
	//�޸���Ϣ
	nState = NetUserSetInfo(NULL,USER_NAME, dwLevel, (LPBYTE)&UserInfo,NULL);
	{
		LPUSER_INFO_1 uInfo;
		nState = NetUserGetInfo(NULL, USER_NAME, dwLevel, (LPBYTE*)&uInfo);
		printf("flag:%u", uInfo->usri1_flags);
		if (uInfo != nullptr)
		{
			NetApiBufferFree(uInfo);
			uInfo = nullptr;
		}
	}

	//ɾ���û�
	nState = NetUserDel(NULL, USER_NAME);
	return 0;
}