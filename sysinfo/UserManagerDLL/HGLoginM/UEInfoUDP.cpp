// UEInfoUDP.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "UEInfoUDP.h"
#include "HGLoginMView.h"
#ifndef H_NETMESSAGE
#include "netmessage.h"
#endif

// CUEInfoUDP

CUEInfoUDP::CUEInfoUDP()
{
}

CUEInfoUDP::~CUEInfoUDP()
{
}


// CUEInfoUDP ��Ա����


void CUEInfoUDP::OnReceive(int nErrorCode)
{
	// TODO:  �ڴ����ר�ô����/����û���



	SOCKADDR_IN sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));
	int nSockAddrLen = sizeof(sockAddr);

	EUInfoStruct tPack;
	tPack.zClear();	

	int nResult = ReceiveFrom(&tPack, sizeof(EUInfoStruct), (SOCKADDR*)&sockAddr, &nSockAddrLen, 0);

	struct in_addr in;
	in = sockAddr.sin_addr;
	//AfxMessageBox( (CString)inet_ntoa( in ));
	CString ClientAddr = (CString)inet_ntoa(in);

	if (nResult != SOCKET_ERROR)
	{

		//CString  USERNAME = tPack.USERNAME;
		switch ( tPack.CODE )
		{
		case 1001:
			GetMListView->MsgCommand(tPack, ClientAddr, 1001);
			break;
		default:
			break;
		}

	}
	

	CSocket::OnReceive(nErrorCode);
}


void CUEInfoUDP::OnSend(int nErrorCode)
{
	// TODO:  �ڴ����ר�ô����/����û���

	CSocket::OnSend(nErrorCode);
}


void CUEInfoUDP::OnClose(int nErrorCode)
{
	// TODO:  �ڴ����ר�ô����/����û���

	CSocket::OnClose(nErrorCode);
}
