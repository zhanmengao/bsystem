#pragma once
#include <lmshare.h>
#include <LM.h>
#include <istream>
#include <atlstr.h>
#include <vector>
#include "GStringClass.h"
#pragma comment (lib,"Netapi32.lib")

using namespace std;

class NetFileShare
{
private:
	GStringClass gStringClass;

public:
	NetFileShare(void);
	~NetFileShare(void);
	void AddFileShare();
	//NET_API_STATUS EditNetShare( LPWSTR sharedFolderPath, LPWSTR shareName, LPWSTR remark,LPWSTR userName, UINT permissions );
	BOOL NetFileShare::GetAccountSid(LPCTSTR AccountName, PSID* Sid);
	NET_API_STATUS SetNetShare( CString sharedFolderPath, CString shareName,CString userName, UINT permissions);
	NET_API_STATUS RemoveNetShare( CString ShareName );
	NET_API_STATUS CheckNetShare( CString ShareName );
	CString ReturnInfo( NET_API_STATUS res );
	NET_API_STATUS GetSharePath( CString ShareName ,CString& PathStr );
	NET_API_STATUS GetShareUsers( CString ShareName , vector<CString> &NameStr );
};

