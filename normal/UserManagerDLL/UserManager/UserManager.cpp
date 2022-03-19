#ifndef UNICODE
#define UNICODE
#endif

#include <stdio.h>
#include <assert.h>
#include <windows.h> 
#include <lm.h>
#include<string>
#include<json\json.h>
#include<strsafe.h>
#include"../../../Error/WErrorUtils/ErrorInfo.h"
#pragma comment(lib, "Netapi32.lib")
#pragma comment(lib, "jsoncpp_static.lib")
#include"../../../../C++space/Convert/Convert/Convert.h"
#include"UserManager.h"
#define _DEBUG 0
#define MAX_NAME MAX_PATH

void SetBit(DWORD& flag, unsigned nBitsIdx, bool val)
{
	if (val)
	{
		flag |= 1 << nBitsIdx;
	}
	else
	{
		flag = flag & (~(1 << nBitsIdx));
	}
}
ULONG AddGroupPlus(const char* groupName, const char* user)
{
	//加组，组不存在则创建，若组为空则说明不想传,也算成功
	DWORD dwRet = AddUserToGroup(groupName, user);
	if (dwRet == NERR_Success)
	{
		return dwRet;
	}
	if (dwRet == NERR_GroupNotFound)
	{
		dwRet = AddGroup(groupName);
		if (dwRet == NERR_Success)
		{
			dwRet = AddUserToGroup(groupName, user);
			//如果加组失败，直接删除组
			if (dwRet != NERR_Success)
			{
				DelGroup(groupName);
			}
		}
	}
	return dwRet;
}
ULONG  AddUserPlus(const char* userName, const char* password, const char* groupName1, const char* groupName2)
{
	ULONG dwRet = AddUser(userName, password);
	if (dwRet != NERR_Success)
	{
		return dwRet;
	}
	bool bSec = true;
	do
	{
		dwRet = AddGroupPlus(groupName1, userName);
		if (dwRet != NERR_Success)
		{
			bSec = false;
			break;
		}
		dwRet = AddGroupPlus(groupName2, userName);
		if (dwRet != NERR_Success)
		{
			bSec = false;
			break;
		}
	} while (0);
	//如果添加组操作失败，直接删除用户
	if (!bSec)
	{
		DelUser(userName);
	}
	return dwRet;
}
ULONG AddUser(const char* userName, const char* password)
{
	{
		char log[1024] = { 0 };
		sprintf(log, "AddUser Begin:User %s\n", userName);
		OutputDebugStringA(log);
	}
	USER_INFO_1 ui;
	DWORD dwLevel = 1;
	DWORD dwError = 0;

	WCHAR name[MAX_NAME] = { 0 };
	lstrcpy(name, Convert::StringToWString(userName).c_str());
	ui.usri1_name = name;

	WCHAR pas[MAX_NAME] = { 0 };
	lstrcpy(pas, Convert::StringToWString(password).c_str());
	ui.usri1_password = pas;

	ui.usri1_priv = USER_PRIV_USER;
	ui.usri1_home_dir = NULL;
	ui.usri1_comment = NULL;
	ui.usri1_flags = UF_SCRIPT | UF_DONT_EXPIRE_PASSWD;
	ui.usri1_script_path = NULL;
	NET_API_STATUS nStatus = NetUserAdd(NULL, dwLevel, (LPBYTE)&ui, &dwError);
	{
		char log[1024] = { 0 };
		sprintf(log, "AddUser End:User %ws  nStatus %d\n", name, nStatus);
		OutputDebugStringA(log);
	}
	return nStatus;
}
ULONG DelUser(const char* userName)
{
	{
		char log[1024] = { 0 };
		sprintf(log, "DelUser Begin:User %s\n", userName);
		OutputDebugStringA(log);
	}
	DWORD dwError = 0;
	WCHAR name[MAX_NAME] = { 0 };
	lstrcpy(name, Convert::StringToWString(userName).c_str());
	NET_API_STATUS nStatus = NetUserDel(NULL, name);
	{
		char log[1024] = { 0 };
		sprintf(log, "DelUser End:%ws nStatus %d\n", name, nStatus);
		OutputDebugStringA(log);
	}
	return nStatus;
}

ULONG AddGroup(const char* groupName)
{
	{
		char log[1024] = { 0 };
		sprintf(log, "AddGroup Begin:%s\n", groupName);
		OutputDebugStringA(log);
	}
	if (groupName == nullptr || strlen(groupName) == 0)
	{
		return 0;
	}
	_LOCALGROUP_INFO_0 info;
	WCHAR name[MAX_NAME] = { 0 };
	lstrcpy(name, Convert::StringToWString(groupName).c_str());
	info.lgrpi0_name = name;
	NET_API_STATUS nStatus = NetLocalGroupAdd(NULL, 0, (LPBYTE)&info, NULL);
	{
		char log[1024] = { 0 };
		sprintf(log, "AddGroup End:%ws nStatus %d\n", name, nStatus);
		OutputDebugStringA(log);
	}
	return nStatus;
}

ULONG DelGroup(const char* groupName)
{
	{
		char log[1024] = { 0 };
		sprintf(log, "DelGroup Begin:%s\n", groupName);
		OutputDebugStringA(log);
	}
	if (groupName == NULL || strlen(groupName) == 0)
	{
		return 0;
	}
	WCHAR name[MAX_NAME] = { 0 };
	lstrcpy(name, Convert::StringToWString(groupName).c_str());
	NET_API_STATUS nStatus = NetLocalGroupDel(NULL, name);
	{
		char log[1024] = { 0 };
		sprintf(log, "DelGroup End:%ws nStatus %d\n", name, nStatus);
		OutputDebugStringA(log);
	}
	return nStatus;
}

ULONG AddUserToGroup(const char* group, const char* user)
{
	{
		char log[1024] = { 0 };
		sprintf(log, "AddUserToGroup Begin:Group:%s User:%s\n", group, user);
		OutputDebugStringA(log);
	}
	if (group == nullptr || user == nullptr || strlen(group) == 0 || strlen(user) == 0)
	{
		return 0;
	}
	DWORD level = 3;
	LOCALGROUP_MEMBERS_INFO_3 account;
	WCHAR uName[MAX_NAME] = { 0 };
	lstrcpy(uName, Convert::StringToWString(user).c_str());
	account.lgrmi3_domainandname = uName;

	DWORD  totalentries = 1;
	WCHAR gName[MAX_NAME] = { 0 };
	lstrcpy(gName, Convert::StringToWString(group).c_str());

	NET_API_STATUS nStatus = NetLocalGroupAddMembers(NULL, gName,
		level, (LPBYTE)&account, totalentries);
	{
		char log[1024] = { 0 };
		sprintf(log, "AddUserToGroup End:Group:%ws User:%ws nStatus:%d\n", gName, uName, nStatus);
		OutputDebugStringA(log);
	}
	return nStatus;
}

ULONG DelUserToGroup(const char* group, const char* user)
{
	{
		char log[1024] = { 0 };
		sprintf(log, "DelUserToGroup Begin:Group:%s User:%s\n", group, user);
		OutputDebugStringA(log);
	}
	DWORD level = 3;

	LOCALGROUP_MEMBERS_INFO_3 account;
	WCHAR uName[MAX_NAME] = { 0 };
	lstrcpy(uName, Convert::StringToWString(user).c_str());
	account.lgrmi3_domainandname = uName;

	DWORD  totalentries = 1;
	WCHAR gName[MAX_NAME] = { 0 };
	lstrcpy(gName, Convert::StringToWString(group).c_str());
	NET_API_STATUS nStatus =
		NetLocalGroupDelMembers(NULL, gName, level, (LPBYTE)&account, totalentries);
	{
		char log[1024] = { 0 };
		sprintf(log, "DelUserToGroup End:Group:%ws User:%ws nStatus:%d\n", gName, uName, nStatus);
		OutputDebugStringA(log);
	}
	return nStatus;
}
ULONG GetGroupList(char* GroupList, int& bufSize)
{
	Json::Value jsData;
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

	if (nStatus == NERR_Success)
	{
		gInfo = (LOCALGROUP_INFO_0*)bufptr;
		for (DWORD i = 0; i < entriesread; i++)
		{
			char gName[MAX_PATH] = { 0 };
			strcpy(gName, Convert::WStringToString(gInfo->lgrpi0_name).c_str());
			jsData["Data"].append(Convert::PChar2GolangBuf(gName));
			gInfo++;
		}
	}
	if (bufptr != nullptr)
	{
		NetApiBufferFree(bufptr);
		bufptr = nullptr;
	}
	Json::FastWriter writer;
	string strWrite = writer.write(jsData);
	StringCchCopyA(GroupList, strlen(strWrite.c_str()), strWrite.c_str());
	bufSize = strlen(GroupList);
	return nStatus;
}

ULONG GetGroupUsers(const char* group, char* UserList, int& bufSize)
{
	Json::Value jsData;
	LPCWSTR servername = NULL;
	DWORD LEVEL = 1;
	LPLOCALGROUP_MEMBERS_INFO_1 bufptr;
	DWORD entriesread;
	DWORD totalentries;
	NET_API_STATUS nStatus;

	WCHAR GroupName[MAX_NAME] = { 0 };
	lstrcpy(GroupName, Convert::StringToWString(group).c_str());
	do
	{
		nStatus = NetLocalGroupGetMembers(NULL, GroupName,
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
						break;
					}
					//写入
					char uName[MAX_PATH] = { 0 };
					strcpy(uName, Convert::WStringToString(pTmpBuf->lgrmi1_name).c_str());
					jsData["Data"].append(Convert::PChar2GolangBuf(uName));
					pTmpBuf++;
				}
			}
		}
	} while (false);
	if (bufptr != NULL)   //释放缓冲  
	{
		NetApiBufferFree(bufptr);
		bufptr = nullptr;
	}
	Json::FastWriter writer;
	string strWrite = writer.write(jsData);
	StringCchCopyA(UserList, strlen(strWrite.c_str()), strWrite.c_str());
	bufSize = strlen(UserList);
	return nStatus;
}

ULONG GetUserGroups(const char* user, char* uGroupList, int& bufSize)
{
	Json::Value jsData;
	LPLOCALGROUP_USERS_INFO_0 pBuf = NULL;
	DWORD   dwLevel = 0;
	DWORD   dwFlags = LG_INCLUDE_INDIRECT;
	DWORD   dwPrefMaxLen = -1;
	DWORD   dwEntriesRead = 0;
	DWORD   dwTotalEntries = 0;
	NET_API_STATUS   nStatus;

	WCHAR UserNameStr[MAX_NAME] = { 0 };
	lstrcpy(UserNameStr, Convert::StringToWString(user).c_str());
	do
	{
		nStatus = NetUserGetLocalGroups(NULL, UserNameStr, dwLevel, dwFlags,
			(LPBYTE *)&pBuf, dwPrefMaxLen,
			&dwEntriesRead, &dwTotalEntries);
		if (nStatus == NERR_Success)
		{
			LPLOCALGROUP_USERS_INFO_0   pTmpBuf;
			if ((pTmpBuf = pBuf) != NULL)
			{
				for (DWORD i = 0; i < dwEntriesRead; i++)
				{
					if (pTmpBuf == NULL)
					{
						break;
					}
					//strcpy(uGroupList[i], WStringToString(pTmpBuf->lgrui0_name).c_str());
					char ugName[MAX_PATH] = { 0 };
					strcpy(ugName, Convert::WStringToString(pTmpBuf->lgrui0_name).c_str());
					jsData["Data"].append(Convert::PChar2GolangBuf(ugName));
					pTmpBuf++;
				}
			}
		}
	} while (false);

	if (pBuf != NULL)
	{
		NetApiBufferFree(pBuf);
		pBuf = nullptr;
	}
	Json::FastWriter writer;
	string strWrite = writer.write(jsData);
	StringCchCopyA(uGroupList, strlen(strWrite.c_str()), strWrite.c_str());
	bufSize = strlen(uGroupList);
	return  nStatus;
}

ULONG SetUserActive(const char* user, bool bActive)
{
	{
		char log[1024] = { 0 };
		sprintf(log, "SetUserActive Begin:%s\n", user);
		OutputDebugStringA(log);
	}
	LPUSER_INFO_1 uInfo;
	NET_API_STATUS   nStatus;
	WCHAR    lpUserName[MAX_NAME] = { 0 };
	do
	{
		DWORD dwLevel = 1;
		DWORD dwError = 0;
		lstrcpy(lpUserName, Convert::StringToWString(user).c_str());
		DWORD   nSize = sizeof(lpUserName);
		nStatus = NetUserGetInfo(NULL, lpUserName, dwLevel, (LPBYTE*)&uInfo);
		if (nStatus != NERR_Success)
		{
			break;
		}
		//设置
		SetBit(uInfo->usri1_flags, UF_ACCOUNTDISABLE - 1, !bActive);
		nStatus = NetUserSetInfo(NULL, lpUserName, dwLevel, (LPBYTE)uInfo, NULL);
	} while (false);
	if (uInfo != nullptr)
	{
		NetApiBufferFree(uInfo);
		uInfo = nullptr;
	}
	{
		char log[1024] = { 0 };
		sprintf(log, "SetUserActive End:%ws nStatus:%d\n", lpUserName, nStatus);
		OutputDebugStringA(log);
	}
	return nStatus;
}
ULONG SetUserPassword(const char* user, const char* oldpassword, const char* newpassword)
{
	{
		char log[1024] = { 0 };
		sprintf(log, "SetUserPassword Begin:%s\n", user);
		OutputDebugStringA(log);
	}
	WCHAR UserName[MAX_NAME] = { 0 };
	lstrcpy(UserName, Convert::StringToWString(user).c_str());
	WCHAR Old[MAX_NAME] = { 0 };
	lstrcpy(Old, Convert::StringToWString(oldpassword).c_str());
	WCHAR New[MAX_NAME] = { 0 };
	lstrcpy(New, Convert::StringToWString(newpassword).c_str());

	NET_API_STATUS nStatus = NetUserChangePassword(NULL, UserName, Old, New);
	{
		char log[1024] = { 0 };
		sprintf(log, "SetUserPassword End:%ws nStatus:%d\n", UserName, nStatus);
		OutputDebugStringA(log);
	}
	return nStatus;
}
ULONG SetUserPasswordAdmin(const char* user, const char* pass)
{
	{
		char log[1024] = { 0 };
		sprintf(log, "SetUserPassword Begin:%s\n", user);
		OutputDebugStringA(log);
	}
	LPUSER_INFO_1 uInfo;
	NET_API_STATUS   nStatus;
	WCHAR    lpUserName[MAX_NAME] = { 0 };
	do
	{
		DWORD dwLevel = 1;
		DWORD dwError = 0;
		lstrcpy(lpUserName, Convert::StringToWString(user).c_str());
		nStatus = NetUserGetInfo(NULL, lpUserName, dwLevel, (LPBYTE*)&uInfo);
		if (nStatus != NERR_Success)
		{
			break;
		}
		WCHAR    lpPassword[MAX_NAME] = { 0 };
		lstrcpy(lpPassword, Convert::StringToWString(pass).c_str());
		uInfo->usri1_password = lpPassword;
		//设置
		nStatus = NetUserSetInfo(NULL, lpUserName, dwLevel, (LPBYTE)uInfo, NULL);
	} while (false);
	if (uInfo != nullptr)
	{
		NetApiBufferFree(uInfo);
		uInfo = nullptr;
	}
	{
		char log[1024] = { 0 };
		sprintf(log, "SetUserPassword End:%ws nStatus:%d\n", lpUserName, nStatus);
		OutputDebugStringA(log);
	}
	return nStatus;
}
unsigned ReturnErrInfo(ULONG info, char* buf, int& len)
{
	len = 0;
	switch (info)
	{
	case NERR_Success:
		strcpy(buf, "执行成功");
		break;
	case ERROR_ACCESS_DENIED:
		//The user does not have access to the requested information.
		//用户没有访问所请求的信息
		strcpy(buf, "用户权限不足");
		break;
	case NERR_InvalidComputer:
		//The computer name is invalid.
		//计算机名无效
		strcpy(buf, "计算机名无效"); break;
	case NERR_NotPrimary:
		//The operation is allowed only on the primary domain controller of the domain.
		//该操作仅在该域的主域控制器允许。
		strcpy(buf, "该操作仅在该域的主域控制器允许");
		break;

	case NERR_GroupExists:
		//The group already exists.
		//组已经存在
		strcpy(buf, "组已经存在");
		break;

	case NERR_UserExists:
		//The user account already exists.
		//用户帐户已存在。
		strcpy(buf, "用户帐户已存在");
		break;

	case NERR_PasswordTooShort:
		//The password is shorter than required. (The password could also be too long, be too recent in its change history, not have enough unique characters, or not meet another password policy requirement.)
		//密码不符合严格性要求
		strcpy(buf, "密码不符合严格性要求");
		break;

	case NERR_UserNotFound:
		//The user name could not be found.
		//用户没有找到
		strcpy(buf, "用户没有找到");
		break;

	case ERROR_MORE_DATA:
		//More entries are available. Specify a large enough buffer to receive all entries.
		//更多的条目是可用的。指定一个足够大的缓冲区接收所有条目。
		strcpy(buf, "缓冲区不足");
		break;

	case NERR_BufTooSmall:
		//The return buffer is too small.
		//返回的缓冲区太小。
		strcpy(buf, "缓冲区不足");
		break;

	case NERR_GroupNotFound:
		//The local group specified by the groupname parameter does not exist.
		//指定的本地组不存在
		strcpy(buf, "指定的本地组不存在");
		break;

	case ERROR_NO_SUCH_MEMBER:
		//One or more of the members specified do not exist. Therefore, no new members were added.
		//一个或一个以上的成员指定不存在。因此，没有添加新成员。
		strcpy(buf, "一个或一个以上的成员指定不存在。因此，没有添加新成员");
		break;

	case ERROR_MEMBER_IN_ALIAS:
		//One or more of the members specified were already members of the local group. No new members were added.
		//一个或一个以上的成员指定的本地组的成员。没有添加新成员。
		strcpy(buf, "一个或一个以上的成员指定的本地组的成员。没有添加新成员");
		break;

	case ERROR_INVALID_MEMBER:
		//One or more of the members cannot be added because their account type is invalid. No new members were added.
		//一个或一个以上的成员不能说因为他们的帐户类型无效。没有添加新成员。
		strcpy(buf, "一个或一个以上的成员不能说因为他们的帐户类型无效。没有添加新成员");
		break;


	case ERROR_MEMBER_NOT_IN_ALIAS:
		//One or more of the members specified were not members of the local group. No members were deleted.
		//一个或多个指定的成员不是本地组的成员。成员没有被删除。
		strcpy(buf, "一个或多个指定的成员不是本地组的成员。成员没有被删除");
		break;

	case ERROR_INVALID_LEVEL:
		//The system call level is not correct. This error is returned if the level parameter was not specified as 0.
		//系统调用的层次是不正确的。这个错误是如果水平参数没有被指定为0返回。
		strcpy(buf, "系统调用的层次是不正确的。这个错误是如果水平参数没有被指定为0返回");
		break;

	case ERROR_INVALID_PARAMETER:
		//A parameter is incorrect. This error is returned if the flags parameter contains a value other than LG_INCLUDE_INDIRECT.
		//一个参数是不正确的。这个错误是如果Flags参数包含比其他 LG_INCLUDE_INDIRECT 返回一个值。
		strcpy(buf, "一个参数是不正确的。这个错误是如果Flags参数包含比其他 LG_INCLUDE_INDIRECT 返回一个值");
		break;
	case ERROR_NOT_ENOUGH_MEMORY:
		//Insufficient memory was available to complete the operation.
		//内存不足无法完成操作
		strcpy(buf, "内存不足无法完成操作");
		break;

	case NERR_DCNotFound:
		//The domain controller could not be found.
		//域控制器无法找到。
		strcpy(buf, "域控制器无法找到");
		break;

	case RPC_S_SERVER_UNAVAILABLE:
		//The RPC server is unavailable. This error is returned if the servername parameter could not be found.
		//RPC服务器不可用。这个错误是如果ServerName参数无法找到返回。
		strcpy(buf, "RPC服务器不可用。这个错误是如果ServerName参数无法找到返回");
		break;
	default:
	{
		return GetErrInfo(info, buf, len);
	}
	}
	//转utf8
	std::string utf8Code = Convert::PChar2GolangBuf(buf);
	strcpy(buf, utf8Code.c_str());
	len = strlen(buf);
	return 0;
}

void main()
{
	DWORD ret = AddUserPlus("qaq", "pasqaq", "gqaq","gqaq2");
	ret = AddUserPlus("qaq2", "pasqaq", "gqaq", nullptr);
	char buf[1024] = { 0 };
	int len = 1024;
	GetErrInfo(ret, buf, len);
	printf("%s", Convert::Utf8ToASCII(string(buf)));
	system("pause");
}