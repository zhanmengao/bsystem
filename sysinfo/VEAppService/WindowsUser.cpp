#include "stdafx.h"
#include "WindowsUser.h"

CWindowsUser::CWindowsUser(void)
{
}


CWindowsUser::~CWindowsUser(void)
{
}

NET_API_STATUS CWindowsUser::GetUserList( vector<CString> &AddList )
{

	return  GetUserList(AddList, "Error:RootTree" );
}

NET_API_STATUS CWindowsUser::GetUserList(vector<CString> &AddList, CString GroupName)
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

	do
	{
		nStatus = NetUserEnum((LPCWSTR)pszServerName,
			dwLevel,
			FILTER_NORMAL_ACCOUNT,
			(LPBYTE*)&pBuf,
			dwPrefMaxLen,
			&dwEntriesRead,
			&dwTotalEntries,
			&dwResumeHandle);

		if ((nStatus == NERR_Success) || (nStatus == ERROR_MORE_DATA))
		{
			if ((pTmpBuf = pBuf) != NULL)
			{

				for (i = 0; (i < dwEntriesRead); i++)
				{					
					if (pTmpBuf == NULL)
					{
						break;
					}

					CString name = (CString)pTmpBuf->usri3_name;
					
					if ( UserLSGroup(name, L"Remote Desktop Users") )
					{		
						/**
						switch (pTmpBuf->usri3_priv)
						{
						case USER_PRIV_GUEST:
						case USER_PRIV_USER:
							AddList.push_back(name);
							break;
						case USER_PRIV_ADMIN:
							//Administrator		
							break;
						default:							
							break;
						}**/
                   
						AddList.push_back(name);
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

	} while (nStatus == ERROR_MORE_DATA); // end do
		
	return nStatus;
}


NET_API_STATUS CWindowsUser::GetGroupList( vector<CString> &AddList )
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

	nStatus = NetLocalGroupEnum(NULL, 0, &bufptr, prefmaxlen, &entriesread, &totalentries, &resumehandle);

	if (nStatus == NO_ERROR)
	{

		gInfo = (LOCALGROUP_INFO_0*)bufptr;

		for (int i = 0; i < entriesread; i++)
		{			
			AddList.push_back( (CString)gInfo->lgrpi0_name );
			gInfo++;
		}
	}

	return entriesread;
}

NET_API_STATUS CWindowsUser::GetUserInfo( CString UserName, UserStruct_I1& UserInfo)
{
	LPUSER_INFO_3 tmpbuf;

	NetUserGetInfo(NULL, UserName, 3, (LPBYTE*)&tmpbuf);


	UserInfo.Comment = (CString)tmpbuf->usri3_comment;
	UserInfo.HomeDirPath = (CString)tmpbuf->usri3_home_dir;
	UserInfo.HDrive = (CString)tmpbuf->usri3_home_dir_drive;
	UserInfo.ProfilePath = (CString)tmpbuf->usri3_profile;
	UserInfo.ScriptPath = (CString)tmpbuf->usri3_script_path;


	if (tmpbuf != NULL) {
		NetApiBufferFree(tmpbuf);
	}

	return  0;
}

NET_API_STATUS CWindowsUser::DelSysUser(  CString UserNameStr)
{

	DWORD dwError = 0;
	NET_API_STATUS nStatus;

	nStatus = NetUserDel(NULL, UserNameStr);

	return nStatus;
}

NET_API_STATUS CWindowsUser::AddSysUser(  CString UserNameStr, CString PasswordStr)
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
	ui.usri1_flags = UF_SCRIPT | UF_DONT_EXPIRE_PASSWD;
	ui.usri1_script_path = NULL;

	nStatus = NetUserAdd(NULL, dwLevel, (LPBYTE)&ui, &dwError);



	return nStatus;
}

NET_API_STATUS CWindowsUser::AddUserToGroup( CString GroupNameStr, CString UserNameStr)
{

	DWORD level = 3;

	LOCALGROUP_MEMBERS_INFO_3 account;
	account.lgrmi3_domainandname = UserNameStr.GetBuffer();
	UserNameStr.ReleaseBuffer();

	DWORD  totalentries = 1;

	NET_API_STATUS nStatus = NetLocalGroupAddMembers(NULL, GroupNameStr, level, (LPBYTE)&account, totalentries);
	return nStatus;
}


NET_API_STATUS CWindowsUser::AddUserGroup(CString GroupName) {
	DWORD level = 0;

	LOCALGROUP_INFO_0 groupInfo;
	groupInfo.lgrpi0_name = GroupName.GetBuffer();

	NET_API_STATUS nStatus = NetLocalGroupAdd(NULL, level,(LPBYTE)&groupInfo,NULL);
	return nStatus;
}

NET_API_STATUS CWindowsUser::DelUserGroup(CString GroupName) {

	NET_API_STATUS nStatus = NetLocalGroupDel(NULL, GroupName);
	return nStatus;
}


NET_API_STATUS  CWindowsUser::DelUserToGroup( CString GroupNameStr, CString UserNameStr)
{

	DWORD level = 3;

	LOCALGROUP_MEMBERS_INFO_3 account;
	account.lgrmi3_domainandname = UserNameStr.GetBuffer();
	UserNameStr.ReleaseBuffer();

	DWORD  totalentries = 1;

	NET_API_STATUS nStatus = NetLocalGroupDelMembers(NULL, GroupNameStr, level, (LPBYTE)&account, totalentries);

	return nStatus;

}

NET_API_STATUS CWindowsUser::GetUserToGroups(CString UserNameStr, vector<CString> &GroupList)
{
	LPLOCALGROUP_USERS_INFO_0 pBuf = NULL;
	DWORD   dwLevel = 0;
	DWORD   dwFlags = LG_INCLUDE_INDIRECT;
	DWORD   dwPrefMaxLen = -1;
	DWORD   dwEntriesRead = 0;
	DWORD   dwTotalEntries = 0;
	NET_API_STATUS   nStatus;

	nStatus = NetUserGetLocalGroups(NULL,
		UserNameStr,
		dwLevel,
		dwFlags,
		(LPBYTE *)&pBuf,
		dwPrefMaxLen,
		&dwEntriesRead,
		&dwTotalEntries);

	if (nStatus == NERR_Success)
	{
		LPLOCALGROUP_USERS_INFO_0   pTmpBuf;
		DWORD   i;
		DWORD   dwTotalCount = 0;

		if ((pTmpBuf = pBuf) != NULL)
		{			

			for (i = 0; i < dwEntriesRead; i++)
			{
		
				if (pTmpBuf == NULL)
				{					
					break;
				}				
				GroupList.push_back((CString)pTmpBuf->lgrui0_name);

				pTmpBuf++;
				dwTotalCount++;
			}
		}		

		if (dwEntriesRead < dwTotalEntries)
		{
			//
		}
				 
		//printf("\nEntries   enumerated:   %d\n", dwTotalCount);
	}
	else
	{
		//fprintf(stderr, "A   system   error   has   occurred:   %d\n", nStatus);
	}
	
	if (pBuf != NULL)
	{
		NetApiBufferFree(pBuf);
    }
	return  nStatus;
}

BOOL CWindowsUser::UserLSGroup( CString UserName, CString GroupName)
{
	vector<CString> uGroups;

	GetUserToGroups(UserName, uGroups);
		
	for(int i = 0; i< uGroups.size(); i++ )
	{
		if ( uGroups[i] == GroupName)
		{			
			return TRUE;
		}
	}
	
	return FALSE;
}

CString CWindowsUser::UserLSGroupListCString( CString UserName)
{

	vector<CString> uGroups;

	GetUserToGroups(UserName, uGroups);

	CString GroupListCString;
		
	for (int i = 0; i< uGroups.size(); i++)
	{
		GroupListCString += uGroups[i] + " | ";
	}
	
	return GroupListCString;
}

NET_API_STATUS CWindowsUser::SetUserProfile( CString UserName, UserStruct_I1 UserInfo)
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

	ui.usri3_profile = UserInfo.ProfilePath.GetBuffer();
	UserInfo.ProfilePath.ReleaseBuffer();

	nStatus = NetUserSetInfo(NULL,
		UserName,
		dwLevel,
		(LPBYTE)&ui,
		NULL);

	return nStatus;
}

NET_API_STATUS CWindowsUser::SetUserHomeDir( CString UserName, UserStruct_I1 UserInfo)
{
	DWORD dwLevel = 3;
	NET_API_STATUS nStatus;

	LPUSER_INFO_3 tmpbuf;
	nStatus = NetUserGetInfo(NULL, UserName, 3, (LPBYTE*)&tmpbuf);

	if (nStatus != NERR_Success)
	{
		if (tmpbuf != NULL)
		{
			NetApiBufferFree(tmpbuf);
		}
		return nStatus;
	}

	WCHAR homeDir[MAX_PATH] = { 0 };
	wcscpy(homeDir, UserInfo.HomeDirPath);

	//tmpbuf->usri3_home_dir_drive = L"Z:";
	tmpbuf->usri3_home_dir = homeDir;

	nStatus =
		NetUserSetInfo(NULL,
			UserName,
			dwLevel,
			(LPBYTE)tmpbuf,
			NULL);

	if (tmpbuf != NULL)
	{
		NetApiBufferFree(tmpbuf);
	}

	return nStatus;
}

NET_API_STATUS CWindowsUser::SetUserStatus(CString UserName, int status)
{
	DWORD dwLevel = 3;
	NET_API_STATUS nStatus;

	LPUSER_INFO_3 tmpbuf;
	nStatus = NetUserGetInfo(NULL, UserName, 3, (LPBYTE*)&tmpbuf);

	if (nStatus != NERR_Success)
	{
		if (tmpbuf != NULL)
		{
			NetApiBufferFree(tmpbuf);
		}
		return nStatus;
	}

	if ( status == 1 )
	{
		tmpbuf->usri3_flags = UF_ACCOUNTDISABLE;
	}
	else
	{
		tmpbuf->usri3_flags = UF_DONT_EXPIRE_PASSWD;
	}
	
	nStatus =
		NetUserSetInfo(NULL,
			UserName,
			dwLevel,
			(LPBYTE)tmpbuf,
			NULL);

	if (tmpbuf != NULL)
	{
		NetApiBufferFree(tmpbuf);
	}

	return nStatus;
}

