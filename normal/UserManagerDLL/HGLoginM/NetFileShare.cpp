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


//�޸��ļ��й������ԣ����û�Ȩ�޿���
NET_API_STATUS NetFileShare::SetNetShare( CString sharedFolderPath, CString shareName,CString userName,	UINT permissions)
{

	LPTSTR Username   =  Str2Unicode( CSting2PChar(  userName ) ); 

	LPWSTR pwShareName = Str2Unicode( CSting2PChar( shareName ) ); 
	LPWSTR pwSharePath = Str2Unicode( CSting2PChar( sharedFolderPath ) );


	DWORD level = 502;
	SHARE_INFO_502 si502;   
	DWORD parmErr = 0;

	si502.shi502_netname = pwShareName;//shareName;                                 //������
	si502.shi502_type = STYPE_DISKTREE;                               //��Դ���� �����Ǵ���
	si502.shi502_remark = NULL;                                     //ע��
	si502.shi502_path =  pwSharePath;//sharedFolderPath;                             //�ļ���·��
	si502.shi502_permissions = ACCESS_ALL ;                           //����Ȩ�ޣ�������ù�
	si502.shi502_passwd = NULL;                                       //��������
	si502.shi502_max_uses = -1;                                       //����û�����
	si502.shi502_current_uses = 0;                                    //��ǰ�����û�
	si502.shi502_reserved   =   0;                                    //�����ֶ�


	//���潫���SHARE_INFO_502���õ���SD�ṹ
	SECURITY_DESCRIPTOR sd;   //��������һ��SD

	BYTE aclBuffer[1024];
	PACL pacl=(PACL)&aclBuffer;  //����һ��ACL��������1024

	BYTE sidBuffer[100];
	PSID psid=(PSID) &sidBuffer;   //����һ��SID��������100

	//��ʼ��һ��SD
	InitializeSecurityDescriptor(&sd, SECURITY_DESCRIPTOR_REVISION);

	//��ʼ��һ��ACL
	InitializeAcl( pacl, 1024, ACL_REVISION );

	DWORD sidBufferSize = 100;
	TCHAR domainBuffer[80];
	DWORD domainBufferSize = 80;
	SID_NAME_USE snu;

	//����һ���û�UserA����ȡ���û���SID
	LookupAccountName(0, Username, psid, &sidBufferSize, domainBuffer, &domainBufferSize, &snu);

	//���ø��û���Access-Allowed��ACE������������������Ȩ�ޣ����ǽ��õ����涨���permissions
	if( 0 == permissions )  //ֻ����GENERIC_WRITE ��XP�º����ܰ��ļ�������Ϊֻ������������0x001200a9���棩
		AddAccessAllowedAce(pacl, ACL_REVISION, 0x001200a9, psid);
	else if( 1 == permissions )
		AddAccessAllowedAce(pacl, ACL_REVISION, GENERIC_ALL, psid);

	//��ACL���õ�SD��
	SetSecurityDescriptorDacl(&sd, TRUE, pacl, FALSE);

	si502.shi502_security_descriptor   =   &sd;   //   ����ɵ�SD�ṹ��ֵ��502�ṹ��  


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
		tmpStr = _T("���óɹ�");
		break;
	case ERROR_ACCESS_DENIED:    
		tmpStr = L"�û�û�з������������Ϣ";    
		break;
	case ERROR_INVALID_LEVEL:
		tmpStr = L"����ˮƽ����ָ����ֵ��Ч";    
		break;
	case ERROR_INVALID_NAME:
		tmpStr = L"�ַ����ļ�����Ч";    
		break;
	case ERROR_INVALID_PARAMETER:
		tmpStr = L"ָ���Ĳ�����Ч��";
		break;
	case NERR_DuplicateShare:
		tmpStr = L"������������ʹ�ô˷������ϵ�";
		break;
	case NERR_RedirectedPath:
		tmpStr = L"�����ض������Դ����Ч��";
		break;
	case NERR_UnknownDevDir:
		tmpStr = L"�豸��Ŀ¼������";
		break;
	default:
		tmpStr = L"δ֪����";
		break;
	}

	return tmpStr;
}
