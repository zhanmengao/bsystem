// MemPatch.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
    // VA = 004024D8
    DWORD dwVAddress = 0x00401EA8;
    BYTE  bCode = 0;
    DWORD dwReadNum = 0;

    // 判断参数数量
    if ( argc != 2 )
    {
        printf("Please input two argument \r\n");
        return -1;
    }

    STARTUPINFO si = { 0 };
    si.cb = sizeof(STARTUPINFO);
    si.wShowWindow = SW_SHOW;
    si.dwFlags = STARTF_USESHOWWINDOW;
    PROCESS_INFORMATION pi = { 0 };

    BOOL bRet = CreateProcess(argv[1],
                    NULL,
                    NULL,
                    NULL,
                    FALSE,
                    CREATE_SUSPENDED,   // 将子进程暂停
                    NULL,
                    NULL,
                    &si,
                    &pi);

    if ( bRet == FALSE )
    {
        printf("CreateProcess Error ! \r\n");
        return -1;
    }

    ReadProcessMemory(pi.hProcess,
                     (LPVOID)dwVAddress,
                     (LPVOID)&bCode,
                     sizeof(BYTE),
                     &dwReadNum);

    // 判断是否为JNZ
    if ( bCode != '\x75' )
    {
        printf("%02X \r\n", bCode);
        CloseHandle(pi.hThread);
        CloseHandle(pi.hProcess);
        return -1;
    }

    // 将JNZ修改为JZ
    bCode = '\x74';
    WriteProcessMemory(pi.hProcess,
                     (LPVOID)dwVAddress,
                     (LPVOID)&bCode,
                     sizeof(BYTE),
                     &dwReadNum);

    ResumeThread(pi.hThread);

    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);
    
    printf("Write JZ is Successfully ! \r\n");

    getchar();

	return 0;
}
