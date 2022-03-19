// IsDebugTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include <stdio.h>

extern "C" BOOL WINAPI IsDebuggerPresent(VOID);

DWORD WINAPI ThreadProc(LPVOID lpParam)
{
    while ( TRUE )
    {
        // 用来检测用ActiveDebugProcess()来创建调试关系
        if ( IsDebuggerPresent() == TRUE )
        {
            printf("thread func checked the debuggee \r\n");
            break;
        }
        Sleep(1000);
    }

    return 0;
}

int main(int argc, char* argv[])
{
    BOOL bRet = FALSE;

    // 用来检测CreateProcess()创建调试关系
    bRet = IsDebuggerPresent();

    if ( bRet == TRUE )
    {
        printf("main func checked the debuggee \r\n");
        getchar();
        return 1;
    }

    HANDLE hThread = CreateThread(NULL, 0, ThreadProc, NULL, 0, NULL);
    if ( hThread == NULL )
    {
        return -1;
    }

    WaitForSingleObject(hThread, INFINITE);
    CloseHandle(hThread);

    getchar();

    return 0;
}
