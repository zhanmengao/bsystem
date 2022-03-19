// RegRun.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
    char szFileName[MAX_PATH] = { 0 };

    GetModuleFileName(NULL, szFileName, MAX_PATH);

    HKEY hKey = NULL;
    RegOpenKey(HKEY_LOCAL_MACHINE,
            "Software\\Microsoft\\Windows\\CurrentVersion\\Run",
            &hKey);
    RegSetValueEx(hKey,
            "test",
            0,
            REG_SZ,
            (const unsigned char *)szFileName,
            strlen(szFileName) + sizeof(char));
    RegCloseKey(hKey);

	return 0;
}
