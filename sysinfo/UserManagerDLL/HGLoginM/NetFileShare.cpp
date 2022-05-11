#include "stdafx.h"
#include "NetFileShare.h"
#include <lmshare.h>
#include <LM.h>
#include <istream>
#pragma comment (lib,"Netapi32.lib")

////////////////////////////////////////////////////////////////////////////

char* CSting2PChar( CString Str )
{
	char* tmpChar = new char[Str.GetLength()];
	memset( tmpChar,0,Str.GetLength() );

	int i;
	for(  i=0;i<Str.GetLength();i++ )
	{
		tmpChar[i] = (char)Str.GetAt(i);
	}
	tmpChar[i] = '\0';

	return tmpChar;
}

LPWSTR Str2Unicode( char* pStrA )
{
	int widesize = MultiByteToWideChar (CP_ACP, 0, pStrA , -1, NULL, 0);

	if (widesize == ERROR_NO_UNICODE_TRANSLATION)
	{
		throw std::exception("Invalid UTF-8 sequence.");
	}
	if (widesize == 0)
	{
		throw std::exception("Error in conversion.");
	}
	//std::vector<wchar_t> resultstring(widesize);
	WCHAR* resultstring = new WCHAR[widesize];

	int convresult = MultiByteToWideChar (CP_ACP, 0, pStrA , -1, resultstring, widesize);

	if (convresult != widesize)
	{
		throw std::exception("La falla!");
	}

	//return std::wstring(&resultstring[0]);
	return resultstring;

}

/////////////////////////////////////////////////////////////////////////////

NetFileShare::NetFileShare(void)
{
}


NetFileShare::~NetFileShare(void)
{
}

void NetFileShare::AddFileShare()
{

}


//修改文件夹共享属性，带用户权限控制
NET_API_STATUS NetFileShare::SetNetShare( CString sharedFolderPath, CString shareName,CString userName,	UINT permissions)
{

	LPTSTR Username   =  Str2Unicode( CSting2PChar(  userName ) ); 

	LPWSTR pwShareName = Str2Unicode( CSting2PChar( shareName ) ); 
	LPWSTR pwSharePath = Str2Unicode( CSting2PChar( sharedFolderPath ) );


	DWORD level = 502;
	SHARE_INFO_502 si502;   
	DWORD parmErr = 0;

	si502.shi502_netname = pwShareName;//shareName;                                 //共享名
	si502.shi502_type = STYPE_DISKTREE;                               //资源类型 这里是磁盘
	si502.shi502_remark = NULL;                                     //注释
	si502.shi502_path =  pwSharePath;//sharedFolderPath;                             //文件夹路径
	si502.shi502_permissions = ACCESS_ALL ;                           //访问权限，这个不用管
	si502.shi502_passwd = NULL;                                       //访问密码
	si502.shi502_max_uses = -1;                                       //最大用户连接
	si502.shi502_current_uses = 0;                                    //当前连接用户
	si502.shi502_reserved   =   0;                                    //保留字段


	//下面将填充SHARE_INFO_502里用到的SD结构
	SECURITY_DESCRIPTOR sd;   //首先声明一个SD

	BYTE aclBuffer[1024];
	PACL pacl=(PACL)&aclBuffer;  //声明一个ACL，长度是1024

	BYTE sidBuffer[100];
	PSID psid=(PSID) &sidBuffer;   //声明一个SID，长度是100

	//初始化一个SD
	InitializeSecurityDescriptor(&sd, SECURITY_DESCRIPTOR_REVISION);

	//初始化一个ACL
	InitializeAcl( pacl, 1024, ACL_REVISION );

	DWORD sidBufferSize = 100;
	TCHAR domainBuffer[80];
	DWORD domainBufferSize = 80;
	SID_NAME_USE snu;

	//查找一个用户UserA，并取该用户的SID
	LookupAccountName(0, Username, psid, &sidBufferSize, domainBuffer, &domainBufferSize, &snu);

	//设置该用户的Access-Allowed的ACE，第三个参数是设置权限，我们将用到上面定义的permissions
	if( 0 == permissions )  //只读（GENERIC_WRITE 在XP下好像不能把文件夹设置为只读，所以用了0x001200a9代替）
		AddAccessAllowedAce(pacl, ACL_REVISION, 0x001200a9, psid);
	else if( 1 == permissions )
		AddAccessAllowedAce(pacl, ACL_REVISION, GENERIC_ALL, psid);

	//把ACL设置到SD中
	SetSecurityDescriptorDacl(&sd, TRUE, pacl, FALSE);

	si502.shi502_security_descriptor   =   &sd;   //   将完成的SD结构赋值到502结构里  


	NET_API_STATUS res;
	/*
    if( CheckNetShare( shareName ) == NERR_DeviceNotShared )
	{
		res = NetShareAdd( NULL, 502,(LPBYTE)&si502, NULL ); 
	}
	else
	{
        res = NetShareSetInfo(NULL,pwShareName, level,(LPBYTE)&si502, &parmErr);
	}
	*/
	RemoveNetShare( shareName );
	res = NetShareAdd( NULL, 502,(LPBYTE)&si502, NULL ); 

	if ( &si502 != NULL )
		NetApiBufferFree( &si502 );


	delete Username;
	delete pwShareName;
	delete pwSharePath;

	return res;
}

NET_API_STATUS NetFileShare::RemoveNetShare( CString ShareName )
{
	LPWSTR pwShareName = Str2Unicode( CSting2PChar( ShareName ) ); 
	NET_API_STATUS res =  NetShareDel(NULL ,pwShareName,0);

	delete pwShareName;

	return res;
}

NET_API_STATUS NetFileShare::CheckNetShare( CString ShareName )
{
	LPWSTR pwShareName = Str2Unicode( CSting2PChar( ShareName ) );
	DWORD DEVTYPE = 0;
	NET_API_STATUS res =  NetShareCheck( NULL ,pwShareName,&DEVTYPE );

	delete pwShareName;
	return res;
}


NET_API_STATUS NetFileShare::GetShareUsers( CString ShareName ,CStringList* NameStr )
{
	
	LPWSTR pwShareName = Str2Unicode( CSting2PChar( ShareName ) );

	PSHARE_INFO_502 pShareInfo;

	NET_API_STATUS res = NetShareGetInfo(NULL, pwShareName , 502, (LPBYTE*)&pShareInfo);


	BOOL bPresent, bDefaulted;
	PACL pDACL = NULL;
	PSECURITY_DESCRIPTOR pTempSd = pShareInfo->shi502_security_descriptor;
	if(IsValidSecurityDescriptor(pTempSd))
	{
		BOOL bRes = GetSecurityDescriptorDacl(pTempSd, &bPresent, &pDACL, &bDefaulted);
		if(!(bRes && (&bPresent)))
		{
			//AfxMessageBox(IDS_UNKNOWNERR);
			return -1;
		}

		ACL_SIZE_INFORMATION aclInfo;
		GetAclInformation( pDACL, (LPVOID)&aclInfo, sizeof(ACL_SIZE_INFORMATION), AclSizeInformation);
		

		if(IsValidAcl( pDACL ))
		{
			PACCESS_ALLOWED_ACE pTempAce = NULL;
			for(DWORD index=0; index<aclInfo.AceCount; index++)
			{
				GetAce(pDACL, index, (LPVOID*)&pTempAce);
				if(IsValidSid(&pTempAce->SidStart))
				{
                 
				//	LPTSTR lpAccountName= NULL;
				//	LPTSTR lpDomainName= NULL;
				///	DWORD AccountSize = 0;
				//	DWORD DomainSize = 0;
                
					
					WCHAR lpAccountName[MAX_PATH] ;//= NULL;
					WCHAR lpDomainName[MAX_PATH] ;// = NULL;
					DWORD AccountSize = MAX_PATH;
					DWORD DomainSize = MAX_PATH;
					ZeroMemory( lpAccountName,MAX_PATH );
					ZeroMemory( lpDomainName,MAX_PATH );
					
					SID_NAME_USE snu;
				//	LookupAccountSid( NULL, &pTempAce->SidStart, lpAccountName, &AccountSize, \
				//		lpDomainName, &DomainSize, &snu);

				//	lpAccountName = new TCHAR[AccountSize];
				//	lpDomainName = new TCHAR[DomainSize];
				//	DWORD AccountSize = MAX_PATH;
				//	DWORD DomainSize = MAX_PATH;
					LookupAccountSid( NULL, &pTempAce->SidStart, lpAccountName, &AccountSize, \
						lpDomainName, &DomainSize, &snu);

					if( ! NameStr->Find( (CString)lpAccountName ) )
					     NameStr->AddTail( (CString)lpAccountName );
					//delete[] lpAccountName;
					//delete[] lpDomainName;
				}
			}
		}
	}
	

	return res;

}

NET_API_STATUS NetFileShare::GetSharePath( CString ShareName ,CString& PathStr )
{
	PSHARE_INFO_502 BufPtr;
	NET_API_STATUS res;
	//LPTSTR   lpszServer = NULL, lpszShare;
	//
	PathStr = _T("Error:Not001");
	LPWSTR pwShareName = Str2Unicode( CSting2PChar( ShareName ) );

	if( (res = NetShareGetInfo ( NULL, pwShareName ,502,(LPBYTE *)&BufPtr)) == ERROR_SUCCESS )
	{

		PathStr = (CString) BufPtr->shi502_path ;		
	}

	NetApiBufferFree(BufPtr);

	return res;
}

CString NetFileShare::ReturnInfo(  NET_API_STATUS res )  
{

	CString tmpStr;

	switch ( res  )
	{

	case NERR_Success:    
		tmpStr = _T("设置成功");
		break;
	case ERROR_ACCESS_DENIED:    
		tmpStr = L"用户没有访问所请求的信息";    
		break;
	case ERROR_INVALID_LEVEL:
		tmpStr = L"对于水平参数指定的值无效";    
		break;
	case ERROR_INVALID_NAME:
		tmpStr = L"字符或文件名无效";    
		break;
	case ERROR_INVALID_PARAMETER:
		tmpStr = L"指定的参数无效。";
		break;
	case NERR_DuplicateShare:
		tmpStr = L"共享名称已在使用此服务器上的";
		break;
	case NERR_RedirectedPath:
		tmpStr = L"操作重定向的资源是无效的";
		break;
	case NERR_UnknownDevDir:
		tmpStr = L"设备或目录不存在";
		break;
	default:
		tmpStr = L"未知错误";
		break;
	}

	return tmpStr;
}
