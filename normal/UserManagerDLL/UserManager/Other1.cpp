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
	// 定义USER_INFO_1结构体
	USER_INFO_1 UserInfo;
	DWORD dwLevel = 1;
	DWORD dwError = 0;

	UserInfo.usri1_name = USER_NAME;            // 账户    
	UserInfo.usri1_password = OLD_PASS;      // 密码
	UserInfo.usri1_priv = USER_PRIV_USER;
	UserInfo.usri1_home_dir = NULL;
	UserInfo.usri1_comment = NULL;
	UserInfo.usri1_flags = UF_SCRIPT;
	UserInfo.usri1_script_path = NULL;

	//添加名为Admins的用户,密码为P@ssword233 
	auto nState = NetUserAdd(NULL, dwLevel, (LPBYTE)&UserInfo, &dwError);

	// 添加用户到administrators组
	LOCALGROUP_MEMBERS_INFO_3 account;
	account.lgrmi3_domainandname = UserInfo.usri1_name;
	nState = NetLocalGroupAddMembers(NULL, L"Administrators", 3, (LPBYTE)&account, 1);

	//从组里删除用户
	nState = NetLocalGroupDelMembers(NULL, L"Administrators", 3, (LPBYTE)&account, 1);

	//添加用户到组里
	nState = NetLocalGroupAddMember(NULL, USER_NAME, NULL);
	//从组里删除用户
	nState = NetLocalGroupDelMember(NULL, USER_NAME, NULL);
	//修改密码
	nState = NetUserChangePassword(NULL, USER_NAME, OLD_PASS, NEW_PASS);
	//修改信息
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

	//删除用户
	nState = NetUserDel(NULL, USER_NAME);
	return 0;
}