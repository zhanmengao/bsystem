#include "stdafx.h"
#include "NetFileShare.h"

////////////////////////////////////////////////////////////////////////////
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

BOOL NetFileShare::GetAccountSid(LPCTSTR AccountName, PSID* Sid)
{
	PSID pSID = NULL;
	DWORD cbSid = 0;
	LPTSTR DomainName = NULL;
	DWORD cbDomainName = 0;
	SID_NAME_USE SIDNameUse;
	BOOL  bDone = FALSE;

	__try
	{
		if (!LookupAccountName(NULL,  // we only concertrate local machine.
			AccountName,
			pSID,
			&cbSid,
			DomainName,
			&cbDomainName,
			&SIDNameUse))
		{
			pSID = (PSID)malloc(cbSid);
			DomainName = (LPTSTR)malloc(cbDomainName * sizeof(TCHAR));
			if (!pSID || !DomainName)
			{
				__leave;
			}
			if (!LookupAccountName(NULL,
				AccountName,
				pSID,
				&cbSid,
				DomainName,
				&cbDomainName,
				&SIDNameUse))
			{
				__leave;
			}
			bDone = TRUE;
		}
	}
	__finally
	{
		if (DomainName)
			free(DomainName);
		if (!bDone && pSID)
			free(pSID);
	}
	if (bDone)
		*Sid = pSID;

	return bDone;
}

//�޸��ļ��й������ԣ����û�Ȩ�޿���
NET_API_STATUS NetFileShare::SetNetShare( CString sharedFolderPath, CString shareName,CString userName,	UINT permissions)
{
	LPTSTR Username   = gStringClass.Str2Unicode(gStringClass.CSting2PChar(  userName ) );

	LPWSTR pwShareName = gStringClass.Str2Unicode(gStringClass.CSting2PChar( shareName ) );
	LPWSTR pwSharePath = gStringClass.Str2Unicode(gStringClass.CSting2PChar( sharedFolderPath ) );


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

	PSID psid;   //����һ��SID��������100

				 //��ʼ��һ��SD
	InitializeSecurityDescriptor(&sd, SECURITY_DESCRIPTOR_REVISION);

	//��ʼ��һ��ACL
	InitializeAcl(pacl, 1024, ACL_REVISION);
	//userName = "everyone";
	if (!GetAccountSid(userName, &psid))
	{
		printf("Get user:%s sid failed!\r\n", userName);
		return FALSE;
	}

	//���ø��û���Access-Allowed��ACE������������������Ȩ�ޣ����ǽ��õ����涨���permissions
	if (0 == permissions)  //ֻ����GENERIC_WRITE ��XP�º����ܰ��ļ�������Ϊֻ������������0x001200a9���棩
		AddAccessAllowedAce(pacl, ACL_REVISION, 0x001200a9, psid);
	else if (1 == permissions)
		AddAccessAllowedAce(pacl, ACL_REVISION, GENERIC_ALL, psid);

	//��ACL���õ�SD��
	SetSecurityDescriptorDacl(&sd, TRUE, pacl, FALSE);

	si502.shi502_security_descriptor = &sd;   //   ����ɵ�SD�ṹ��ֵ��502�ṹ��  

	NET_API_STATUS status;
	DWORD paraerr = 0;
	status = NetShareAdd(NULL, 502, (LPBYTE)&si502, &paraerr);

	//if (status == ERROR_ACCESS_DENIED)
	//{
	//}
	//else if (status == ERROR_INVALID_LEVEL) {
	//}
	//else if (status == ERROR_INVALID_NAME)
	//{
	//}
	//else if (status == ERROR_INVALID_PARAMETER)
	//{
	//}
	//else if (status == NERR_RedirectedPath)
	//{
	//}
	//else if (status == NERR_UnknownDevDir)
	//{
	//}
	//else if (status == NERR_DuplicateShare)
	//{
	//}
	//else if (status == NERR_UnknownServer)
	//{
	//}

	if (status != NERR_Success)
	{
		if (status == NERR_DuplicateShare)
		{
			std::cout << "duplicate share.  " << std::endl;
			RemoveNetShare(pwShareName);

			if (NetShareAdd(NULL, 502, (LPBYTE)&si502, &paraerr) == NERR_Success)
			{
				std::cout << "NetShareAdd successfully! " << std::endl;
				return TRUE;
			}
		}

		std::cout << "Failed to modify net share.  " << std::endl;
		return FALSE;
	}

	return TRUE;
}

NET_API_STATUS NetFileShare::RemoveNetShare( CString ShareName )
{
	LPWSTR pwShareName = gStringClass.Str2Unicode(gStringClass.CSting2PChar( ShareName ) );
	NET_API_STATUS res =  NetShareDel(NULL ,pwShareName,0);
	//delete pwShareName;
	return res;
}

NET_API_STATUS NetFileShare::CheckNetShare( CString ShareName )
{
	LPWSTR pwShareName = gStringClass.Str2Unicode(gStringClass.CSting2PChar( ShareName ) );
	DWORD DEVTYPE = 0;
	NET_API_STATUS res =  NetShareCheck( NULL ,pwShareName,&DEVTYPE );

	delete pwShareName;
	return res;
}


NET_API_STATUS NetFileShare::GetShareUsers( CString ShareName, vector<CString> &NameStr )
{
	
	LPWSTR pwShareName = gStringClass.Str2Unicode(gStringClass.CSting2PChar( ShareName ) );

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
                		
					
					WCHAR lpAccountName[MAX_PATH] ;//= NULL;
					WCHAR lpDomainName[MAX_PATH] ;// = NULL;
					DWORD AccountSize = MAX_PATH;
					DWORD DomainSize = MAX_PATH;
					ZeroMemory( lpAccountName,MAX_PATH );
					ZeroMemory( lpDomainName,MAX_PATH );
					
					SID_NAME_USE snu;
				
					LookupAccountSid( NULL, &pTempAce->SidStart, lpAccountName, &AccountSize, lpDomainName, &DomainSize, &snu );
										
					vector<CString>::iterator result = find(NameStr.begin(), NameStr.end(), (CString)lpAccountName );
					
					if (result != NameStr.end()) {
						NameStr.push_back((CString)lpAccountName);
					}						 
						 
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
	PathStr = _T("Error:Not001");
	LPWSTR pwShareName = gStringClass.Str2Unicode(gStringClass.CSting2PChar( ShareName ) );

	if( (res = NetShareGetInfo ( NULL, pwShareName ,502,(LPBYTE *)&BufPtr)) == ERROR_SUCCESS )
	{

		PathStr = (CString) BufPtr->shi502_path ;		
	}

	if (BufPtr != NULL)
	{
		NetApiBufferFree(BufPtr);
	}

	return res;
}

