#pragma once
class NetFileShare
{
public:
	NetFileShare(void);
	~NetFileShare(void);
	void AddFileShare();
	//NET_API_STATUS EditNetShare( LPWSTR sharedFolderPath, LPWSTR shareName, LPWSTR remark,LPWSTR userName, UINT permissions );
	NET_API_STATUS SetNetShare( CString sharedFolderPath, CString shareName,CString userName, UINT permissions);
	NET_API_STATUS RemoveNetShare( CString ShareName );
	NET_API_STATUS CheckNetShare( CString ShareName );
	CString ReturnInfo( NET_API_STATUS res );
	NET_API_STATUS GetSharePath( CString ShareName ,CString& PathStr );
	NET_API_STATUS GetShareUsers( CString ShareName ,CStringList* NameStr );
};

