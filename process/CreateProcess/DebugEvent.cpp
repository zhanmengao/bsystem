#include <stddef.h>  
#include <stdio.h>  
#include <string.h>  
#include <stdlib.h>  
#include <stdint.h>  
#include <assert.h>  
#include <windows.h>  
#include <wchar.h>
#include <iostream>
#include <tchar.h>
using namespace std;

#define MAX_PARAM_LEN       4096  

int main111()
{
	int i, j = 0, len;
	WCHAR command_buf[MAX_PARAM_LEN];

	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	DEBUG_EVENT de;
	BOOL stop = FALSE;

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));


	WCHAR cmdpath[1024] = _T("\n");
	GetSystemDirectory(cmdpath, sizeof(cmdpath));
	wcsncat_s(cmdpath, _T("\\cmd.exe"), 9);	//¿í×Ö·û´®Á¬½Ó¡£¡£

	wcout << "path:" << cmdpath << endl;

	if (!CreateProcess(cmdpath, NULL, NULL, NULL, FALSE,
		DEBUG_ONLY_THIS_PROCESS, NULL, NULL, &si, &pi)) {
		printf("CreateProcess failed (%d).\n", GetLastError());
		exit(-1);
	}

	while (TRUE)
	{
		WaitForDebugEvent(&de, INFINITE);
		printf("\nRecv Event Code = %u", de.dwDebugEventCode);
		switch (de.dwDebugEventCode)
		{
		/* exception */
		case EXCEPTION_DEBUG_EVENT:
			switch (de.u.Exception.ExceptionRecord.ExceptionCode)
			{
			case   EXCEPTION_INT_DIVIDE_BY_ZERO:    /* #DE */
				// Do what the parent process want to do when the child process gets #DE interrupt.  
				TerminateProcess(pi.hProcess, 1);
				break;
			case   EXCEPTION_BREAKPOINT:            /* #BP */
				// Do what the parent process want to do when the child process gets #BP interrupt.  
				break;
			default:
				printf("Unknown Exception\n");
				break;
			}
			ContinueDebugEvent(de.dwProcessId, de.dwThreadId, DBG_EXCEPTION_HANDLED);
			continue;

		case CREATE_PROCESS_DEBUG_EVENT:        /* child process created */
			// Do what the parent process want to do when the child process was created.  
			break;
		case EXIT_PROCESS_DEBUG_EVENT:          /* child process exits */
			stop = TRUE;
			// Do what the parent process want to do when the child process exits.  
			break;

		default:
			//printf("Unknown Event!\n");
			break;
		}
		if (TRUE == stop)
		{
			//printf("Process exit\n");  
			break;
		}
		ContinueDebugEvent(de.dwProcessId, de.dwThreadId, DBG_CONTINUE);
	}

	assert(stop);

	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	return 0;
}