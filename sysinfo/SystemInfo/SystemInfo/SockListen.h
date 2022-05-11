#pragma once
#include <stdio.h>
#include <windows.h>
#include <setupapi.h>
#include <devguid.h>
#include <regstr.h>
#include <locale.h>

#pragma comment( lib,"setupapi.lib")

#include <iphlpapi.h>
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "iphlpapi.lib")


int GetTCPportlist()
{
	int iErrno;
	PMIB_TCPTABLE_OWNER_PID pMibTcpTableOwnerPid;
	DWORD dwSize = 0;
	//TCHAR szBuffer[1024];
	int i;
	HMODULE hModule;

	//https://docs.microsoft.com/zh-cn/windows/win32/api/iphlpapi/nf-iphlpapi-getextendedtcptable?redirectedfrom=MSDN

	if ((iErrno = GetExtendedTcpTable(NULL, &dwSize, TRUE, AF_INET, TCP_TABLE_OWNER_PID_ALL, 0)) != NO_ERROR)
	{
		if (iErrno != ERROR_INSUFFICIENT_BUFFER)
		{
			printf("GetExtendedTcpTable Error: %d\n", iErrno);
			return FALSE;
		}
	}
	pMibTcpTableOwnerPid = (PMIB_TCPTABLE_OWNER_PID)malloc(dwSize);
	if (pMibTcpTableOwnerPid == NULL)
	{
		return FALSE;
	}
	if ((iErrno = GetExtendedTcpTable(pMibTcpTableOwnerPid, &dwSize, TRUE, AF_INET, TCP_TABLE_OWNER_PID_ALL, 0)) != NO_ERROR)
	{
		printf("GetExtendedTcpTable Error: %d\n", iErrno);
		return FALSE;
	}

	for (i = 0; i < (int)pMibTcpTableOwnerPid->dwNumEntries; i++)
	{

		IN_ADDR localAddr;
		IN_ADDR remoteAddr;
		CHAR szLocalAddr[24];
		CHAR szRemoteAddr[24];
		USHORT usRemotePort;
		CHAR szState[24];

		localAddr.S_un.S_addr = pMibTcpTableOwnerPid->table[i].dwLocalAddr;
		remoteAddr.S_un.S_addr = pMibTcpTableOwnerPid->table[i].dwRemoteAddr;

		sprintf(szLocalAddr, "%s", inet_ntoa(localAddr));
		sprintf(szRemoteAddr, "%s", inet_ntoa(remoteAddr));

		switch (pMibTcpTableOwnerPid->table[i].dwState)
		{
		case MIB_TCP_STATE_CLOSED:
			sprintf(szState, "%s", "CLOSED");
			break;
		case MIB_TCP_STATE_LISTEN:
			sprintf(szState, "%s", "LISTENING");
			break;
		case MIB_TCP_STATE_SYN_SENT:
			sprintf(szState, "%s", "SYN_SENT");
			break;
		case MIB_TCP_STATE_SYN_RCVD:
			sprintf(szState, "%s", "SYN_RCVD");
			break;
		case MIB_TCP_STATE_ESTAB:
			sprintf(szState, "%s", "ESTABLISHED");
			break;
		case MIB_TCP_STATE_FIN_WAIT1:
			sprintf(szState, "%s", "FIN_WAIT_1");
			break;
		case MIB_TCP_STATE_FIN_WAIT2:
			sprintf(szState, "%s", "FIN_WAIT_2");
			break;
		case MIB_TCP_STATE_CLOSE_WAIT:
			sprintf(szState, "%s", "CLOSE_WAIT");
			break;
		case MIB_TCP_STATE_CLOSING:
			sprintf(szState, "%s", "CLOSING");
			break;
		case MIB_TCP_STATE_LAST_ACK:
			sprintf(szState, "%s", "LAST_ACK");
			break;
		case MIB_TCP_STATE_TIME_WAIT:
			sprintf(szState, "%s", "TIME_WAIT");
			break;
		case MIB_TCP_STATE_DELETE_TCB:
			sprintf(szState, "%s", "DELETE_TCB");
			break;
		}

		usRemotePort = strcmp(szState, "LISTENING") == 0 ? 0 : ntohs((USHORT)pMibTcpTableOwnerPid->table[i].dwRemotePort);

		char usLocalPortSendBuf[32] = { '\0' };
		char usRemotePortSendBuf[32] = { '\0' };
		char usPidSendBuf[6] = { '\0' };


		sprintf(usLocalPortSendBuf, "%s:%d", szLocalAddr, ntohs((USHORT)pMibTcpTableOwnerPid->table[i].dwLocalPort));
		sprintf(usRemotePortSendBuf, "%s:%d", szRemoteAddr, usRemotePort);
		sprintf(usPidSendBuf, "%d", pMibTcpTableOwnerPid->table[i].dwOwningPid);


		printf("%s\t%s\t%s\t%s\n", usLocalPortSendBuf, usRemotePortSendBuf, usPidSendBuf, szState);

	}

	free(pMibTcpTableOwnerPid);

	return 0;
}

int GetUDPportlist()
{
	int iErrno;
	PMIB_UDPTABLE_OWNER_PID pMibUdpTableOwnerPid;
	DWORD dwSize = 0;
	int i;
	HMODULE hModule;

	if ((iErrno = GetExtendedUdpTable(NULL, &dwSize, TRUE, AF_INET, UDP_TABLE_OWNER_PID, 0)) != NO_ERROR)
	{
		if (iErrno != ERROR_INSUFFICIENT_BUFFER)
		{
			printf("GetExtendedUdpTable Error: %d\n", iErrno);
			return FALSE;
		}
	}
	pMibUdpTableOwnerPid = (PMIB_UDPTABLE_OWNER_PID)malloc(dwSize);
	if (pMibUdpTableOwnerPid == NULL)
	{
		return FALSE;
	}
	if ((iErrno = GetExtendedUdpTable(pMibUdpTableOwnerPid, &dwSize, TRUE, AF_INET, UDP_TABLE_OWNER_PID, 0)) != NO_ERROR)
	{
		printf("GetExtendedUdpTable Error: %d\n", iErrno);
		return FALSE;
	}

	for (i = 0; i < (int)pMibUdpTableOwnerPid->dwNumEntries; i++)
	{
		IN_ADDR localAddr;
		CHAR szLocalAddr[24];

		localAddr.S_un.S_addr = pMibUdpTableOwnerPid->table[i].dwLocalAddr;
		sprintf(szLocalAddr, "%s", inet_ntoa(localAddr));

		char usLocalPortSendBuf[32] = { '\0' };
		char usPidSendBuf[6] = { '\0' };

		sprintf(usLocalPortSendBuf, "%s:%d", szLocalAddr, ntohs((USHORT)pMibUdpTableOwnerPid->table[i].dwLocalPort));
		sprintf(usPidSendBuf, "%d", pMibUdpTableOwnerPid->table[i].dwOwningPid);

		printf("%s\t%s\n", usLocalPortSendBuf, usPidSendBuf);

	}

	free(pMibUdpTableOwnerPid);

	return -1;
}