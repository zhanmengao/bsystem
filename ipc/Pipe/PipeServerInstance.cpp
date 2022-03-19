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

	// 创建一个连接等待事件
	hConnectEvent = CreateEvent(NULL, TRUE, TRUE, NULL);

	if (NULL == hConnectEvent)
	{
		// 创建连接事件失败
		MYTRACE(L"CreateEvent failed with %d.\n", GetLastError());
		return;
	}

	OVERLAPPED oConnect;
	::ZeroMemory(&oConnect, sizeof(OVERLAPPED));
	oConnect.hEvent = hConnectEvent;

	BOOL bPendingIO = FALSE;
	// 创建一个管道实例并等待客户端接入
	bPendingIO = CreateAndConnectInstance(&oConnect);

	// 等待事件的返回值
	DWORD dwWait = WAIT_TIMEOUT;

	while (TRUE)
	{
		// 等待一个客户端的接入，或者为了读写例程执行
		dwWait = WaitForSingleObjectEx(hConnectEvent, INFINITE, TRUE);

		switch (dwWait)
		{
		case WAIT_OBJECT_0:
			// 一个客户端接入

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
			// 等待事件是由读写完成例程触发的
			break;

		default:
		{
			// 错误
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

	// 如果一个操作被挂起，则获取这个连接的结果
	if (FALSE != bPendingIO)
	{
		if (FALSE == GetOverlappedResult(m_hPipe, lpoConnect, &cbRet, FALSE))
		{
			MYTRACE(L"ConnectNamedPipe (%d)\n", GetLastError());
			return FALSE;
		}
	}

	LPPIPEINST lpPipeInst;
	// 分配全局固定内存空间用于保存读写数据
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

	// 创建一个新的管道实例去等待下一个客户端接入
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

	// 断开管道连接
	if (FALSE == DisconnectNamedPipe(lpPipeInst->hPipeInst))
	{
		MYTRACE(L"DisconnectNamedPipe failed with %d.\n", GetLastError());
	}

	// 关闭管道
	CloseHandle(lpPipeInst->hPipeInst);

	// 释放掉全局分配的内存
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

	// 启动一个新的连接等待客户端接入
	return ConnectToNewClient(m_hPipe, lpoOverlap);
}

BOOL CPipeServerInstance::ConnectToNewClient(HANDLE hPipe, LPOVERLAPPED lpo)
{
	BOOL fPendingIO = FALSE;
	DWORD dwLastError = ERROR_SUCCESS;

	// 异步命名管道连接应该失败
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
		// 正在连接
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

		// 如果客户端已经连接上，则设置事件
		if (SetEvent(lpo->hEvent))
		{
			break;
		}
	}// 这个地方故意不break的，因为SetEvent失败了

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

	// 因为lpOverLap的内存是固定的，而其又是LPPIPEINST类型的第一个元素
	// 于是，这样就可以获得之前分配的LPPIPEINST类型的对象
	lpPipeInst = (LPPIPEINST)lpOverLap;

	// 已经异步写完，于是再异步读
	if ((0 == dwErr) && (cbWritten == lpPipeInst->dwWrite))
		fRead = ReadFileEx(
			lpPipeInst->hPipeInst,
			lpPipeInst->cbRead,
			PIPEMSGLENGTH,
			(LPOVERLAPPED)lpPipeInst,
			(LPOVERLAPPED_COMPLETION_ROUTINE)CompletedReadRoutine);

	// 如果写失败了，就断开连接
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

	// 因为lpOverLap的内存是固定的，而其又是LPPIPEINST类型的第一个元素
	// 于是，这样就可以获得之前分配的LPPIPEINST类型的对象
	lpPipeInst = (LPPIPEINST)lpOverLap;

	CPipeServerInstance* pThis = static_cast<CPipeServerInstance*> (lpPipeInst->lpPointer);

	// 已经异步读完，于是再异步写
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

	// 如果读失败了，就断开连接
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