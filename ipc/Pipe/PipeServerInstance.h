#pragma once
#include<windows.h>

class CPipeServerInstance
{
public:
	CPipeServerInstance::CPipeServerInstance()
	{
		m_hPipe = NULL;
	}

	CPipeServerInstance::~CPipeServerInstance()
	{

	}

	VOID StartService();

	BOOL RunServerInstance(LPOVERLAPPED lpoConnect, BOOL& bPendingIO);

	BOOL CreateAndConnectInstance(LPOVERLAPPED lpoOverlap);

	BOOL ConnectToNewClient(HANDLE hPipe, LPOVERLAPPED lpo);

	VOID WINAPI CompletedWriteRoutine(DWORD dwErr, DWORD cbWritten,
		LPOVERLAPPED lpOverLap);

	VOID WINAPI CompletedReadRoutine(DWORD dwErr, DWORD cbBytesRead,
		LPOVERLAPPED lpOverLap);

	VOID DealMsg(LPPIPEINST pipe);
private:
	HANDLE m_hPipe;
};