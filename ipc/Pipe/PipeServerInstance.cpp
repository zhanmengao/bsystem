#include "PipeServerInstance.h"
#include <string>
#include <strsafe.h>

CPipeServerInstance::CPipeServerInstance()
{
	m_hPipe = NULL;
}

CPipeServerInstance::~CPipeServerInstance()
{

}

VOID CPipeServerInstance::StartService()
{
	HANDLE hConnectEvent = NULL;

	// ����һ�����ӵȴ��¼�
	hConnectEvent = CreateEvent(NULL, TRUE, TRUE, NULL);

	if (NULL == hConnectEvent)
	{
		// ���������¼�ʧ��
		MYTRACE(L"CreateEvent failed with %d.\n", GetLastError());
		return;
	}

	OVERLAPPED oConnect;
	::ZeroMemory(&oConnect, sizeof(OVERLAPPED));
	oConnect.hEvent = hConnectEvent;

	BOOL bPendingIO = FALSE;
	// ����һ���ܵ�ʵ�����ȴ��ͻ��˽���
	bPendingIO = CreateAndConnectInstance(&oConnect);

	// �ȴ��¼��ķ���ֵ
	DWORD dwWait = WAIT_TIMEOUT;

	while (TRUE)
	{
		// �ȴ�һ���ͻ��˵Ľ��룬����Ϊ�˶�д����ִ��
		dwWait = WaitForSingleObjectEx(hConnectEvent, INFINITE, TRUE);

		switch (dwWait)
		{
		case WAIT_OBJECT_0:
			// һ���ͻ��˽���

			if (FALSE == RunServerInstance(&oConnect, bPendingIO))
			{
				return;
			}

			if (FALSE == bPendingIO)
			{
				return;
			}

			break;

		case WAIT_IO_COMPLETION:
			// �ȴ��¼����ɶ�д������̴�����
			break;

		default:
		{
			// ����
			MYTRACE(L"WaitForSingleObjectEx (%d)\n", GetLastError());
			return;
		}
		}
	}
	return;
}

BOOL CPipeServerInstance::RunServerInstance(LPOVERLAPPED lpoConnect, BOOL& bPendingIO)
{
	DWORD cbRet = 0;

	// ���һ���������������ȡ������ӵĽ��
	if (FALSE != bPendingIO)
	{
		if (FALSE == GetOverlappedResult(m_hPipe, lpoConnect, &cbRet, FALSE))
		{
			MYTRACE(L"ConnectNamedPipe (%d)\n", GetLastError());
			return FALSE;
		}
	}

	LPPIPEINST lpPipeInst;
	// ����ȫ�̶ֹ��ڴ�ռ����ڱ����д����
	lpPipeInst = (LPPIPEINST)GlobalAlloc(GPTR, sizeof(PIPEINST));
	if (NULL == lpPipeInst)
	{
		MYTRACE(L"GlobalAlloc failed (%d)\n", GetLastError());
		return FALSE;
	}

	lpPipeInst->hPipeInst = m_hPipe;
	lpPipeInst->lpPointer = this;

	DealMsg(lpPipeInst);
	WriteFileEx(
		lpPipeInst->hPipeInst,
		lpPipeInst->cbWrite,
		lpPipeInst->dwWrite,
		(LPOVERLAPPED)lpPipeInst,
		(LPOVERLAPPED_COMPLETION_ROUTINE)CompletedWriteRoutine);

	// ����һ���µĹܵ�ʵ��ȥ�ȴ���һ���ͻ��˽���
	bPendingIO = CreateAndConnectInstance(lpoConnect);

	return TRUE;
}

BOOL CPipeServerInstance::StopService()
{
	m_CriticalPipeHandle.Lock();
	for (VECHANDLEITER it = m_VecPipeHandle.begin(); it != m_VecPipeHandle.end(); it++)
	{
		DisconnectNamedPipe(*it);
	}
	m_CriticalPipeHandle.Unlock();
	return TRUE;
}

VOID CPipeServerInstance::DisconnectAndClose(LPPIPEINST lpPipeInst)
{
	if (NULL == lpPipeInst || NULL == lpPipeInst->hPipeInst)
	{
		return;
	}

	// �Ͽ��ܵ�����
	if (FALSE == DisconnectNamedPipe(lpPipeInst->hPipeInst))
	{
		MYTRACE(L"DisconnectNamedPipe failed with %d.\n", GetLastError());
	}

	// �رչܵ�
	CloseHandle(lpPipeInst->hPipeInst);

	// �ͷŵ�ȫ�ַ�����ڴ�
	if (NULL != lpPipeInst)
	{
		GlobalFree(lpPipeInst);
	}
}

BOOL CPipeServerInstance::CreateAndConnectInstance(LPOVERLAPPED lpoOverlap)
{
	m_hPipe = CreateNamedPipe(
		PIPENAME,
		PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED,
		PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT,
		PIPE_UNLIMITED_INSTANCES,
		sizeof(PipeMsgStruct),
		sizeof(PipeMsgStruct),
		PIPE_TIMEOUT,
		NULL);

	if (INVALID_HANDLE_VALUE == m_hPipe)
	{
		MYTRACE(L"CreateNamedPipe failed with %d.\n", GetLastError());
		return FALSE;
	}

	m_CriticalPipeHandle.Lock();
	m_VecPipeHandle.push_back(m_hPipe);
	m_CriticalPipeHandle.Unlock();

	// ����һ���µ����ӵȴ��ͻ��˽���
	return ConnectToNewClient(m_hPipe, lpoOverlap);
}

BOOL CPipeServerInstance::ConnectToNewClient(HANDLE hPipe, LPOVERLAPPED lpo)
{
	BOOL fPendingIO = FALSE;
	DWORD dwLastError = ERROR_SUCCESS;

	// �첽�����ܵ�����Ӧ��ʧ��
	if (FALSE != ConnectNamedPipe(hPipe, lpo))
	{
		MYTRACE(L"ConnectNamedPipe failed with %d.\n", GetLastError());
		return FALSE;
	}

	switch (GetLastError())
	{
		// If the function fails, the return value is zero 
		// and GetLastError returns a value other than ERROR_IO_PENDING or ERROR_PIPE_CONNECTED.
	case ERROR_IO_PENDING:
	{
		// ��������
		fPendingIO = TRUE;
	}break;

	case ERROR_PIPE_CONNECTED:
	{
		// If a client connects before the function is called, 
		// the function returns zero and GetLastError returns ERROR_PIPE_CONNECTED. 
		// This can happen if a client connects in the interval 
		// between the call to CreateNamedPipe and the call to ConnectNamedPipe.
		// In this situation, there is a good connection between client and server, 
		// even though the function returns zero.

		// ����ͻ����Ѿ������ϣ��������¼�
		if (SetEvent(lpo->hEvent))
		{
			break;
		}
	}// ����ط����ⲻbreak�ģ���ΪSetEventʧ����

	default:
	{
		MYTRACE(L"ConnectNamedPipe failed with %d.\n", GetLastError());
	}
	}

	return fPendingIO;
}

VOID WINAPI CPipeServerInstance::CompletedWriteRoutine(DWORD dwErr, DWORD cbWritten,
	LPOVERLAPPED lpOverLap)
{
	LPPIPEINST lpPipeInst = NULL;
	BOOL fRead = FALSE;

	// ��ΪlpOverLap���ڴ��ǹ̶��ģ���������LPPIPEINST���͵ĵ�һ��Ԫ��
	// ���ǣ������Ϳ��Ի��֮ǰ�����LPPIPEINST���͵Ķ���
	lpPipeInst = (LPPIPEINST)lpOverLap;

	// �Ѿ��첽д�꣬�������첽��
	if ((0 == dwErr) && (cbWritten == lpPipeInst->dwWrite))
		fRead = ReadFileEx(
			lpPipeInst->hPipeInst,
			lpPipeInst->cbRead,
			PIPEMSGLENGTH,
			(LPOVERLAPPED)lpPipeInst,
			(LPOVERLAPPED_COMPLETION_ROUTINE)CompletedReadRoutine);

	// ���дʧ���ˣ��ͶϿ�����
	CPipeServerInstance* pThis = static_cast<CPipeServerInstance*> (lpPipeInst->lpPointer);
	if (FALSE == fRead && NULL != pThis)
	{
		pThis->DisconnectAndClose(lpPipeInst);
	}
}

VOID WINAPI CPipeServerInstance::CompletedReadRoutine(DWORD dwErr, DWORD cbBytesRead,
	LPOVERLAPPED lpOverLap)
{
	LPPIPEINST lpPipeInst = NULL;
	BOOL fWrite = FALSE;

	// ��ΪlpOverLap���ڴ��ǹ̶��ģ���������LPPIPEINST���͵ĵ�һ��Ԫ��
	// ���ǣ������Ϳ��Ի��֮ǰ�����LPPIPEINST���͵Ķ���
	lpPipeInst = (LPPIPEINST)lpOverLap;

	CPipeServerInstance* pThis = static_cast<CPipeServerInstance*> (lpPipeInst->lpPointer);

	// �Ѿ��첽���꣬�������첽д
	if (0 == dwErr && 0 != cbBytesRead && NULL != pThis)
	{
		pThis->DealMsg(lpPipeInst);

		fWrite = WriteFileEx(
			lpPipeInst->hPipeInst,
			lpPipeInst->cbWrite,
			lpPipeInst->dwWrite,
			(LPOVERLAPPED)lpPipeInst,
			(LPOVERLAPPED_COMPLETION_ROUTINE)CompletedWriteRoutine);
	}

	// �����ʧ���ˣ��ͶϿ�����
	if (FALSE == fWrite && NULL != pThis)
	{
		pThis->DisconnectAndClose(lpPipeInst);
	}
}

VOID CPipeServerInstance::DealMsg(LPPIPEINST pipe)
{
	MYTRACE(L"[%p] %s\n", pipe->hPipeInst, pipe->cbRead);
	std::wstring strCmd = L"Default answer from server";
	StringCchCopy((WCHAR*)pipe->cbWrite, PIPEMSGLENGTH, strCmd.c_str());
	pipe->dwWrite = (lstrlen(strCmd.c_str()) + 1) * sizeof(TCHAR);
}