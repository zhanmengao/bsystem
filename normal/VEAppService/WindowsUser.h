#pragma once

#include <stdio.h>
#include <vector>
#include <atlstr.h>
#include <lmaccess.h>
#include "lmerr.h"
#include "lmapibuf.h"

#include "CommonFiles.h"

#pragma comment( lib,"Netapi32.lib")

using namespace std;


class CWindowsUser
{
public:
	CWindowsUser(void);
	~CWindowsUser(void);

	NET_API_STATUS GetUserList( vector<CString> &AddList );
	NET_API_STATUS GetUserList( vector<CString> &AddList ,CString GroupName );
	NET_API_STATUS GetGroupList( vector<CString> &AddList );
	

	NET_API_STATUS AddSysUser(CString UserNameStr, CString PasswordStr);
	NET_API_STATUS DelSysUser(CString UserNameStr);

	NET_API_STATUS AddUserGroup(CString GroupName);
	NET_API_STATUS DelUserGroup(CString GroupName);

	NET_API_STATUS AddUserToGroup(CString GroupNameStr, CString UserNameStr);
	NET_API_STATUS DelUserToGroup(CString GroupNameStr, CString UserNameStr);


	//NET_API_STATUS SetUserInfo( CString UserName , UserStruct_I1 UserInfo );
	NET_API_STATUS GetUserInfo( CString UserName, UserStruct_I1 &UserInfo);

	NET_API_STATUS GetUserToGroups( CString UserNameStr, vector<CString> &GroupList);
	CString UserLSGroupListCString( CString UserName);
	BOOL UserLSGroup( CString UserName, CString GroupName);
	

	NET_API_STATUS SetUserProfile( CString UserName, UserStruct_I1 UserInfo);
	NET_API_STATUS SetUserHomeDir( CString UserName, UserStruct_I1 UserInfo);

	NET_API_STATUS SetUserStatus(CString UserName, int status);

};

