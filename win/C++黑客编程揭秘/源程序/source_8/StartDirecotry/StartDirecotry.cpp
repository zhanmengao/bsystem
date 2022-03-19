// StartDirecotry.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>

int main(int argc, char* argv[])
{
    char szSysPath[MAX_PATH] = { 0 };
    char szStartDirectory[MAX_PATH] = { 0 };
    char szFileName[MAX_PATH] = { 0 };

    GetSystemDirectory(szSysPath, MAX_PATH);
    strncpy(szStartDirectory, szSysPath, 3);

    strcat(szStartDirectory,
        "Documents and Settings\\All Users\\「开始」菜单\\程序\\启动\\test.exe");

    GetModuleFileName(NULL, szFileName, MAX_PATH);

    CopyFile(szFileName, szStartDirectory, FALSE);

	return 0;
}
