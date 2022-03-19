// DelSelf.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>

void CreateBat()
{
    HANDLE hFile = CreateFile("delself.cmd",
        GENERIC_WRITE,
        FILE_SHARE_READ,
        NULL,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL);
    if ( hFile == INVALID_HANDLE_VALUE )
    {
        return ;
    }
    
    char szBat[MAX_PATH] = {0};
    char szSelfName[MAX_PATH] = {0};
    
    GetModuleFileName(NULL, szSelfName, MAX_PATH);
    
    strcat(szBat, "del ");
    strcat(szBat, szSelfName);
    strcat(szBat, "\r\n");
    strcat(szBat, "del delself.cmd");
    
    DWORD dwNum = 0;
    
    WriteFile(hFile, szBat, strlen(szBat) + 1, &dwNum ,NULL);
    
    CloseHandle(hFile);
    
    WinExec("delself.cmd", SW_HIDE);
}

int main(int argc, char* argv[])
{
    CreateBat();

	return 0;
}
