#pragma once

#include <lmaccess.h>
#include "lmerr.h"
#include "lmapibuf.h"

#pragma comment( lib,"Netapi32.lib")


typedef struct _UserStruct
{

	CString Name;
	CString Comment;
	CString HDrive;
	CString HomeDirPath;
	CString ScriptPath;
	CString ProfilePath;

}UserStruct_I1 ,*PUserStruct_I1;



class CWinUser
{
public:
	CWinUser(void);
	~CWinUser(void);

	NET_API_STATUS GetUserList( CListView* AddList );
	NET_API_STATUS GetUserList( CListView* AddList ,CString GroupName );
//	NET_API_STATUS GetUserList( CHGLoginMView* AddList );
	NET_API_STATUS GetGroupList( CTreeCtrl* AddList );
	int GetGroupList( CStringList* GroupList );

	NET_API_STATUS AddSysUser( CString UserNameStr ,CString PasswordStr );
	NET_API_STATUS DelSysUser( CString UserNameStr );

	NET_API_STATUS AddUserToGroup( CString GroupNameStr ,CString UserNameStr );
	NET_API_STATUS DelUserToGroup( CString GroupNameStr ,CString UserNameStr );

	
	//NET_API_STATUS SetUserInfo( CString UserName , UserStruct_I1 UserInfo );
	NET_API_STATUS GetUserInfo( CString UserName , UserStruct_I1& UserInfo );

	NET_API_STATUS GetUserToGroups( CString UserNameStr ,CStringList* uGroupList );
	CString UserLSGroupListString( CString UserName );
	BOOL UserLSGroup( CString UserName ,CString GroupName );
	
	CString ReturnErrInfo( NET_API_STATUS info );

	NET_API_STATUS SetUserProfile(CString UserName, UserStruct_I1 UserInfo);
	NET_API_STATUS SetUserHomeDir(CString UserName, UserStruct_I1 UserInfo);
	NET_API_STATUS SetUserPassword(CString UserName, CString oldpassword, CString newpassword);
	NET_API_STATUS SetUserPassword(CString UserName, CString UserPassword);
};

