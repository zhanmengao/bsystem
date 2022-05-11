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
	//���飬�鲻�����򴴽�������Ϊ����˵�����봫,Ҳ��ɹ�
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
			//�������ʧ�ܣ�ֱ��ɾ����
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
	//�����������ʧ�ܣ�ֱ��ɾ���û�
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
					//д��
					char uName[MAX_PATH] = { 0 };
					strcpy(uName, Convert::WStringToString(pTmpBuf->lgrmi1_name).c_str());
					jsData["Data"].append(Convert::PChar2GolangBuf(uName));
					pTmpBuf++;
				}
			}
		}
	} while (false);
	if (bufptr != NULL)   //�ͷŻ���  
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
		//����
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
		//����
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
		strcpy(buf, "ִ�гɹ�");
		break;
	case ERROR_ACCESS_DENIED:
		//The user does not have access to the requested information.
		//�û�û�з������������Ϣ
		strcpy(buf, "�û�Ȩ�޲���");
		break;
	case NERR_InvalidComputer:
		//The computer name is invalid.
		//���������Ч
		strcpy(buf, "���������Ч"); break;
	case NERR_NotPrimary:
		//The operation is allowed only on the primary domain controller of the domain.
		//�ò������ڸ�����������������
		strcpy(buf, "�ò������ڸ�����������������");
		break;

	case NERR_GroupExists:
		//The group already exists.
		//���Ѿ�����
		strcpy(buf, "���Ѿ�����");
		break;

	case NERR_UserExists:
		//The user account already exists.
		//�û��ʻ��Ѵ��ڡ�
		strcpy(buf, "�û��ʻ��Ѵ���");
		break;

	case NERR_PasswordTooShort:
		//The password is shorter than required. (The password could also be too long, be too recent in its change history, not have enough unique characters, or not meet another password policy requirement.)
		//���벻�����ϸ���Ҫ��
		strcpy(buf, "���벻�����ϸ���Ҫ��");
		break;

	case NERR_UserNotFound:
		//The user name could not be found.
		//�û�û���ҵ�
		strcpy(buf, "�û�û���ҵ�");
		break;

	case ERROR_MORE_DATA:
		//More entries are available. Specify a large enough buffer to receive all entries.
		//�������Ŀ�ǿ��õġ�ָ��һ���㹻��Ļ���������������Ŀ��
		strcpy(buf, "����������");
		break;

	case NERR_BufTooSmall:
		//The return buffer is too small.
		//���صĻ�����̫С��
		strcpy(buf, "����������");
		break;

	case NERR_GroupNotFound:
		//The local group specified by the groupname parameter does not exist.
		//ָ���ı����鲻����
		strcpy(buf, "ָ���ı����鲻����");
		break;

	case ERROR_NO_SUCH_MEMBER:
		//One or more of the members specified do not exist. Therefore, no new members were added.
		//һ����һ�����ϵĳ�Աָ�������ڡ���ˣ�û������³�Ա��
		strcpy(buf, "һ����һ�����ϵĳ�Աָ�������ڡ���ˣ�û������³�Ա");
		break;

	case ERROR_MEMBER_IN_ALIAS:
		//One or more of the members specified were already members of the local group. No new members were added.
		//һ����һ�����ϵĳ�Աָ���ı�����ĳ�Ա��û������³�Ա��
		strcpy(buf, "һ����һ�����ϵĳ�Աָ���ı�����ĳ�Ա��û������³�Ա");
		break;

	case ERROR_INVALID_MEMBER:
		//One or more of the members cannot be added because their account type is invalid. No new members were added.
		//һ����һ�����ϵĳ�Ա����˵��Ϊ���ǵ��ʻ�������Ч��û������³�Ա��
		strcpy(buf, "һ����һ�����ϵĳ�Ա����˵��Ϊ���ǵ��ʻ�������Ч��û������³�Ա");
		break;


	case ERROR_MEMBER_NOT_IN_ALIAS:
		//One or more of the members specified were not members of the local group. No members were deleted.
		//һ������ָ���ĳ�Ա���Ǳ�����ĳ�Ա����Աû�б�ɾ����
		strcpy(buf, "һ������ָ���ĳ�Ա���Ǳ�����ĳ�Ա����Աû�б�ɾ��");
		break;

	case ERROR_INVALID_LEVEL:
		//The system call level is not correct. This error is returned if the level parameter was not specified as 0.
		//ϵͳ���õĲ���ǲ���ȷ�ġ�������������ˮƽ����û�б�ָ��Ϊ0���ء�
		strcpy(buf, "ϵͳ���õĲ���ǲ���ȷ�ġ�������������ˮƽ����û�б�ָ��Ϊ0����");
		break;

	case ERROR_INVALID_PARAMETER:
		//A parameter is incorrect. This error is returned if the flags parameter contains a value other than LG_INCLUDE_INDIRECT.
		//һ�������ǲ���ȷ�ġ�������������Flags�������������� LG_INCLUDE_INDIRECT ����һ��ֵ��
		strcpy(buf, "һ�������ǲ���ȷ�ġ�������������Flags�������������� LG_INCLUDE_INDIRECT ����һ��ֵ");
		break;
	case ERROR_NOT_ENOUGH_MEMORY:
		//Insufficient memory was available to complete the operation.
		//�ڴ治���޷���ɲ���
		strcpy(buf, "�ڴ治���޷���ɲ���");
		break;

	case NERR_DCNotFound:
		//The domain controller could not be found.
		//��������޷��ҵ���
		strcpy(buf, "��������޷��ҵ�");
		break;

	case RPC_S_SERVER_UNAVAILABLE:
		//The RPC server is unavailable. This error is returned if the servername parameter could not be found.
		//RPC�����������á�������������ServerName�����޷��ҵ����ء�
		strcpy(buf, "RPC�����������á�������������ServerName�����޷��ҵ�����");
		break;
	default:
	{
		return GetErrInfo(info, buf, len);
	}
	}
	//תutf8
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