#include "stdafx.h"
#include "WinUser.h"
#include "HGLoginMView.h"



CWinUser::CWinUser(void)
{
}


CWinUser::~CWinUser(void)
{
}

//NetLocalGroupEnum
NET_API_STATUS CWinUser::GetGroupList( CTreeCtrl* AddList )
{
	HTREEITEM hRoot = AddList->InsertItem(_T("LocalHost"), 0, 0);
	AddList->SetItemState(hRoot, TVIS_BOLD, TVIS_BOLD);
	HTREEITEM hSrc = AddList->InsertItem(_T("用户组"), 0, 0, hRoot);


	LPCWSTR servername = NULL;
	DWORD level = 0;
	BYTE *bufptr = NULL;
	DWORD prefmaxlen = MAX_PREFERRED_LENGTH;
	DWORD entriesread;
	DWORD totalentries;
	DWORD_PTR resumehandle = NULL;

	NET_API_STATUS nStatus;

	LOCALGROUP_INFO_0* gInfo;

	nStatus = NetLocalGroupEnum( NULL,0,&bufptr,prefmaxlen,&entriesread,&totalentries,&resumehandle );

	if ( nStatus == NO_ERROR  )
	{

		gInfo = (LOCALGROUP_INFO_0*)bufptr;

		for( int i =0 ;i < entriesread ;i++  )
		{

			AddList->InsertItem(  gInfo->lgrpi0_name , 2, 2, hSrc);
			gInfo++;

		}

	}


	NetApiBufferFree(  bufptr ); 


	AddList->Expand(hRoot, TVE_EXPAND);
	AddList->Expand(hSrc, TVE_EXPAND);

	return nStatus;
}

int CWinUser::GetGroupList( CStringList* GroupList )
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

	nStatus = NetLocalGroupEnum( NULL,0,&bufptr,prefmaxlen,&entriesread,&totalentries,&resumehandle );

	if ( nStatus == NO_ERROR  )
	{

		gInfo = (LOCALGROUP_INFO_0*)bufptr;

		for( int i =0 ;i < entriesread ;i++  )
		{

			//AddList->InsertItem(  gInfo->lgrpi0_name , 2, 2, hSrc);
			GroupList->AddTail( (CString)gInfo->lgrpi0_name );
			gInfo++;

		}

	}


	NetApiBufferFree(  bufptr ); 

	return entriesread;
}


NET_API_STATUS CWinUser::GetUserList( CListView* AddList ,CString GroupName )
{
	LPUSER_INFO_3 pBuf = NULL;
	LPUSER_INFO_3 pTmpBuf;
	DWORD dwLevel = 3;
	DWORD dwPrefMaxLen = MAX_PREFERRED_LENGTH;
	DWORD dwEntriesRead = 0;
	DWORD dwTotalEntries = 0;
	DWORD dwResumeHandle = 0;
	DWORD i;
	DWORD dwTotalCount = 0;
	NET_API_STATUS nStatus;
	LPTSTR pszServerName = NULL;

	do // begin do
	{
		nStatus = NetUserEnum((LPCWSTR) pszServerName,
			dwLevel,
			FILTER_NORMAL_ACCOUNT, // global users
			(LPBYTE*)&pBuf,
			dwPrefMaxLen,
			&dwEntriesRead,
			&dwTotalEntries,
			&dwResumeHandle);

		if ((nStatus == NERR_Success) || (nStatus == ERROR_MORE_DATA))
		{
			if ((pTmpBuf = pBuf) != NULL)
			{

				for (i = 0; (i < dwEntriesRead ); i++)
				{
					//assert(pTmpBuf != NULL);

					if (pTmpBuf == NULL)
					{
						break;
					}
					//

					if ( UserLSGroup( (CString)pTmpBuf->usri3_name ,GroupName  )  ||  GroupName == _T("Error:RootTree") )
					{
							int j = AddList->GetListCtrl().InsertItem( i, (LPCTSTR)pTmpBuf->usri3_name ); //用户名
							AddList->GetListCtrl().SetItemText(j, 1, (CString)pTmpBuf->usri3_home_dir_drive + L"  " + (CString)pTmpBuf->usri3_home_dir); //

							AddList->GetListCtrl().SetItemText(j, 2, _T(""));
							//GetMListView->ReadUserInfo2ini( (CString)pTmpBuf->usri3_name ,_T("HMDir") ));
							//AddList->GetListCtrl().SetItemText(j,2, (LPCTSTR)pTmpBuf->usri3_home_dir ); //主目录
							
							AddList->GetListCtrl().SetItemText(j,3, (LPCTSTR)pTmpBuf->usri3_script_path ); //脚本
							AddList->GetListCtrl().SetItemText(j,4, (LPCTSTR)pTmpBuf->usri3_profile ); //配置文件

							AddList->GetListCtrl().SetItemText(j,5, UserLSGroupListString( (CString)pTmpBuf->usri3_name ) );
/*
							switch (  pTmpBuf->usri3_priv  ) //用户组属性
							{

							case USER_PRIV_GUEST:
								//Guest
								AddList->GetListCtrl().SetItemText(j,6, _T("USER_PRIV_GUEST") );
								break;
							case USER_PRIV_USER:
								//User
								AddList->GetListCtrl().SetItemText(j,6, _T("USER_PRIV_USER") );
								break;
							case USER_PRIV_ADMIN:
								//Administrator
								AddList->GetListCtrl().SetItemText(j,6, _T("USER_PRIV_ADMIN") );
								break;
							default:
								break;
							}
*/

					}
					pTmpBuf++;
					dwTotalCount++;
				}
			}
		}

		if (pBuf != NULL)
		{
			NetApiBufferFree(pBuf);
			pBuf = NULL;
		}

	}while ( nStatus == ERROR_MORE_DATA ); // end do

	if ( pBuf != NULL )
		NetApiBufferFree(pBuf);

	//fprintf(stderr, "\nTotal of %d entries enumerated\n", dwTotalCount);
	//AfxMessageBox( _T("GetUserList") );
	return nStatus;
}


NET_API_STATUS CWinUser::GetUserList( CListView* AddList )
{
	
	return  GetUserList(  AddList ,_T("Error:RootTree") );
}


NET_API_STATUS CWinUser::GetUserInfo( CString UserName , UserStruct_I1& UserInfo )
{
	LPUSER_INFO_3 tmpbuf;

	NetUserGetInfo( NULL,UserName,3,(LPBYTE*)&tmpbuf);   


	UserInfo.Comment = (CString)tmpbuf->usri3_comment;
	UserInfo.HomeDirPath = (CString)tmpbuf->usri3_home_dir;
	UserInfo.HDrive = (CString)tmpbuf->usri3_home_dir_drive;
	UserInfo.ProfilePath = (CString)tmpbuf->usri3_profile;
	UserInfo.ScriptPath = (CString)tmpbuf->usri3_script_path;
	

	if ( tmpbuf != NULL )
		NetApiBufferFree(tmpbuf);

	return  0;
}



NET_API_STATUS CWinUser::DelSysUser( CString UserNameStr)
{

	DWORD dwError = 0;
	NET_API_STATUS nStatus;

	nStatus = NetUserDel(NULL,UserNameStr );

	return nStatus;
}

NET_API_STATUS CWinUser::AddSysUser( CString UserNameStr ,CString PasswordStr )
{
	USER_INFO_1 ui;
	DWORD dwLevel = 1;
	DWORD dwError = 0;
	NET_API_STATUS nStatus;

	ui.usri1_name = UserNameStr.GetBuffer();
	UserNameStr.ReleaseBuffer();
	ui.usri1_password = PasswordStr.GetBuffer();
	PasswordStr.ReleaseBuffer();

	ui.usri1_priv = USER_PRIV_USER;
	ui.usri1_home_dir = NULL;
	ui.usri1_comment = NULL;
	ui.usri1_flags = UF_SCRIPT;
	ui.usri1_script_path = NULL;

	nStatus = NetUserAdd(NULL, dwLevel,(LPBYTE)&ui,	&dwError);	

	//if ( &ui != NULL )
	//	NetApiBufferFree( &ui );

	return nStatus;
}


NET_API_STATUS CWinUser::AddUserToGroup( CString GroupNameStr ,CString UserNameStr )
{

	DWORD level = 3;

	LOCALGROUP_MEMBERS_INFO_3 account; 
	account.lgrmi3_domainandname = UserNameStr.GetBuffer();
	UserNameStr.ReleaseBuffer();

	DWORD  totalentries = 1;

	NET_API_STATUS nStatus = NetLocalGroupAddMembers(NULL, GroupNameStr  ,level ,(LPBYTE)&account,totalentries );

	//if ( &account != NULL )
	//	NetApiBufferFree( &account);
    
	return nStatus;
}

NET_API_STATUS  CWinUser::DelUserToGroup( CString GroupNameStr ,CString UserNameStr )
{
	
	DWORD level = 3;

	LOCALGROUP_MEMBERS_INFO_3 account; 
	account.lgrmi3_domainandname = UserNameStr.GetBuffer();
	UserNameStr.ReleaseBuffer();

	DWORD  totalentries = 1;

	NET_API_STATUS nStatus = NetLocalGroupDelMembers( NULL, GroupNameStr  ,level ,(LPBYTE)&account,totalentries );

	if ( &account != NULL )
		NetApiBufferFree( &account);

	return nStatus;

}

NET_API_STATUS CWinUser::GetUserToGroups( CString UserNameStr ,CStringList* uGroupList )
{
	LPLOCALGROUP_USERS_INFO_0 pBuf = NULL;   
	DWORD   dwLevel   =   0;   
	DWORD   dwFlags   =   LG_INCLUDE_INDIRECT   ;   
	DWORD   dwPrefMaxLen   =   -1;   
	DWORD   dwEntriesRead   =   0;   
	DWORD   dwTotalEntries   =   0;   
	NET_API_STATUS   nStatus;  


	nStatus = NetUserGetLocalGroups(NULL,   
		UserNameStr,   
		dwLevel,   
		dwFlags,   
		(LPBYTE *)&pBuf,   
		dwPrefMaxLen,   
		&dwEntriesRead,   
		&dwTotalEntries);   

	if ( nStatus == NERR_Success )   
	{   
		LPLOCALGROUP_USERS_INFO_0   pTmpBuf;   
		DWORD   i;   
		DWORD   dwTotalCount   =   0;   

		if ( ( pTmpBuf = pBuf ) != NULL)   
		{   
			fprintf(stderr,   "\nLocal   group(s):\n");   
			//   
			//uGroupList->RemoveHead();	   

			for ( i = 0; i< dwEntriesRead; i++ )   
			{   
				//assert( pTmpBuf != NULL);   

				if ( pTmpBuf == NULL)   
				{   
					fprintf(stderr,   "An   access   violation   has   occurred\n");   
					break;   
				}   

				//wprintf(L"\t--   %s\n",   pTmpBuf->lgrui0_name);   
				uGroupList->AddTail( (CString)pTmpBuf->lgrui0_name );

				pTmpBuf++;   
				dwTotalCount++;   
			}   
		}   
		//   

		if   (dwEntriesRead   <   dwTotalEntries)   
			fprintf(stderr, "\nTotal   entries:   %d", dwTotalEntries);   
		// 
		printf("\nEntries   enumerated:   %d\n",   dwTotalCount);   
	}   
	else   
		fprintf(stderr,   "A   system   error   has   occurred:   %d\n",   nStatus);   
	//   

	if ( pBuf != NULL )   
		NetApiBufferFree(pBuf);   

	return  nStatus; 
}

CString CWinUser::ReturnErrInfo( NET_API_STATUS info )
{

	CString infoStr = _T("");

	switch ( info  )
	{

	case NERR_Success:  
		infoStr = _T("执行成功");
		break;

	case ERROR_ACCESS_DENIED:
		//The user does not have access to the requested information.
		//用户没有访问所请求的信息
		infoStr = _T("ERROR_ACCESS_DENIED");
		break;

	case NERR_InvalidComputer:
		//The computer name is invalid.
		//计算机名无效
		infoStr = _T("计算机名无效");
		break;

	case NERR_NotPrimary:
		//The operation is allowed only on the primary domain controller of the domain.
		//该操作仅在该域的主域控制器允许。
		infoStr = _T("该操作仅在该域的主域控制器允许");
		break;

	case NERR_GroupExists:
		//The group already exists.
		//组已经存在
		infoStr = _T("组已经存在");
		break;

	case NERR_UserExists:
		//The user account already exists.
		//用户帐户已存在。
		infoStr = _T("用户帐户已存在");
		break;

	case NERR_PasswordTooShort:
		//The password is shorter than required. (The password could also be too long, be too recent in its change history, not have enough unique characters, or not meet another password policy requirement.)
		//密码不符合严格性要求
		infoStr = _T("密码不符合严格性要求");
		break;

	case NERR_UserNotFound:
		//The user name could not be found.
		//用户没有找到
		infoStr = _T("用户没有找到");
		break;

	case ERROR_MORE_DATA:
		//More entries are available. Specify a large enough buffer to receive all entries.
		//更多的条目是可用的。指定一个足够大的缓冲区接收所有条目。
		infoStr = _T("缓冲区不足");
		break;

	case NERR_BufTooSmall:
		//The return buffer is too small.
		//返回的缓冲区太小。
		infoStr = _T("缓冲区不足");
		break;

	case NERR_GroupNotFound:
		//The local group specified by the groupname parameter does not exist.
		//指定的本地组不存在
		infoStr = _T("指定的本地组不存在");
		break;

	case ERROR_NO_SUCH_MEMBER:
		//One or more of the members specified do not exist. Therefore, no new members were added.
		//一个或一个以上的成员指定不存在。因此，没有添加新成员。
		infoStr = _T("一个或一个以上的成员指定不存在");
		break;

	case ERROR_MEMBER_IN_ALIAS:
		//One or more of the members specified were already members of the local group. No new members were added.
		//一个或一个以上的成员指定的本地组的成员。没有添加新成员。
		infoStr = _T("一个或一个以上的成员指定的本地组的成员。没有添加新成员");
		break;

	case ERROR_INVALID_MEMBER:
		//One or more of the members cannot be added because their account type is invalid. No new members were added.
		//一个或一个以上的成员不能说因为他们的帐户类型无效。没有添加新成员。
		infoStr = _T("一个或一个以上的成员不能说因为他们的帐户类型无效。没有添加新成员");
		break;
	

	case ERROR_MEMBER_NOT_IN_ALIAS:
		//One or more of the members specified were not members of the local group. No members were deleted.
		//一个或多个指定的成员不是本地组的成员。成员没有被删除。
		infoStr = _T("一个或多个指定的成员不是本地组的成员。成员没有被删除");
		break;

	case ERROR_INVALID_LEVEL:
		//The system call level is not correct. This error is returned if the level parameter was not specified as 0.
		//系统调用的层次是不正确的。这个错误是如果水平参数没有被指定为0返回。
		infoStr = _T("一个或多个指定的成员不是本地组的成员。成员没有被删除");
		break;

	case ERROR_INVALID_PARAMETER:
		//A parameter is incorrect. This error is returned if the flags parameter contains a value other than LG_INCLUDE_INDIRECT.
		//一个参数是不正确的。这个错误是如果Flags参数包含比其他 LG_INCLUDE_INDIRECT 返回一个值。
		infoStr = _T("一个参数是不正确的。这个错误是如果Flags参数包含比其他 LG_INCLUDE_INDIRECT 返回一个值");
		break;

	case ERROR_NOT_ENOUGH_MEMORY:
		//Insufficient memory was available to complete the operation.
		//内存不足无法完成操作
		infoStr = _T("内存不足无法完成操作");
		break;

	case NERR_DCNotFound:
		//The domain controller could not be found.
		//域控制器无法找到。
		infoStr = _T("域控制器无法找到");
		break;

	case RPC_S_SERVER_UNAVAILABLE:
		//The RPC server is unavailable. This error is returned if the servername parameter could not be found.
		//RPC服务器不可用。这个错误是如果ServerName参数无法找到返回。
		infoStr = _T("RPC服务器不可用。这个错误是如果ServerName参数无法找到返回");
		break;

	default:
		break;

	}


	return infoStr;

}

BOOL CWinUser::UserLSGroup( CString UserName ,CString GroupName  )
{
	CStringList* uGroups = new CStringList;
	GetUserToGroups( UserName , uGroups );

	POSITION rPos;

	rPos = uGroups->GetHeadPosition();
	while (rPos != NULL)
	{
		if( uGroups->GetNext(rPos) == GroupName )
		{
			delete uGroups;
			return TRUE;
		}
	}

	delete uGroups;

	return FALSE;
}

CString CWinUser::UserLSGroupListString( CString UserName )
{
	CStringList* uGroups = new CStringList;
	GetUserToGroups( UserName , uGroups );

	CString GroupListString;

	POSITION rPos;

	rPos = uGroups->GetHeadPosition();
	while (rPos != NULL)
	{
	   GroupListString += uGroups->GetNext(rPos) + _T(" | ");	
	}

	delete uGroups;

	return GroupListString;
}

NET_API_STATUS CWinUser::SetUserProfile(CString UserName, UserStruct_I1 UserInfo)
{
	DWORD dwLevel = 3;
	USER_INFO_3 ui;

	NET_API_STATUS nStatus;

	LPUSER_INFO_3 tmpbuf;
	nStatus = NetUserGetInfo( NULL,UserName,3,(LPBYTE*)&tmpbuf);  

	if ( nStatus != NERR_Success  )
	{
		NetApiBufferFree( tmpbuf ); 
		return nStatus;
	}

	ui = *tmpbuf;

	//ui.usri3_profile = UserInfo.ProfilePath.GetBuffer();
	//UserInfo.ProfilePath.ReleaseBuffer();

	ui.usri3_flags = UF_DONT_EXPIRE_PASSWD;

	nStatus = NetUserSetInfo(  NULL,
							   UserName,
							   dwLevel,
							   (LPBYTE)&ui,
							   NULL);
	if( &ui != NULL  )
		NetApiBufferFree( &ui ); 

	return nStatus;
}

NET_API_STATUS CWinUser::SetUserPassword(CString UserName, CString UserPassword)
{
	DWORD dwLevel = 3;
	USER_INFO_3 ui;

	NET_API_STATUS nStatus;

	LPUSER_INFO_3 tmpbuf;
	nStatus = NetUserGetInfo(NULL, UserName, 3, (LPBYTE*)&tmpbuf);

	if (nStatus != NERR_Success)
	{
		NetApiBufferFree(tmpbuf);
		return nStatus;
	}

	ui = *tmpbuf;

	//ui.usri3_profile = UserInfo.ProfilePath.GetBuffer();
	//UserInfo.ProfilePath.ReleaseBuffer();

	ui.usri3_password = UserPassword.GetBuffer();
	UserPassword.ReleaseBuffer();
	ui.usri3_flags = UF_DONT_EXPIRE_PASSWD;

	nStatus = NetUserSetInfo(NULL,
		UserName,
		dwLevel,
		(LPBYTE)&ui,
		NULL);

	if (&ui != NULL)
		NetApiBufferFree(&ui);

	return nStatus;
}



NET_API_STATUS CWinUser::SetUserHomeDir(CString UserName, UserStruct_I1 UserInfo)
{
	DWORD dwLevel = 3;
	USER_INFO_3 ui;

	NET_API_STATUS nStatus;

	LPUSER_INFO_3 tmpbuf;
	nStatus = NetUserGetInfo(NULL, UserName, 3, (LPBYTE*)&tmpbuf);

	if (nStatus != NERR_Success)
	{
		NetApiBufferFree(tmpbuf);
		return nStatus;
	}

	ui = *tmpbuf;

	ui.usri3_home_dir_drive = _T("Z:");
	ui.usri3_home_dir = UserInfo.HomeDirPath.GetBuffer();
	UserInfo.HomeDirPath.ReleaseBuffer();
	

	nStatus = NetUserSetInfo(NULL,
		UserName,
		dwLevel,
		(LPBYTE)&ui,
		NULL);

	if (&ui != NULL)
		NetApiBufferFree(&ui);

	return nStatus;
}


NET_API_STATUS CWinUser::SetUserPassword( CString UserName, CString oldpassword, CString newpassword )
{
	DWORD dwError = 0;
	NET_API_STATUS nStatus;
	//

	//
	// Call the NetUserChangePassword function.
	//12685565
	nStatus = NetUserChangePassword(NULL, UserName, oldpassword, newpassword);

	//if (nStatus == NERR_Success)
	//	fwprintf(stderr, L"User password has been changed successfully\n");

	//else
	//	fprintf(stderr, "A system error has occurred: %d\n", nStatus);

	return nStatus;
}