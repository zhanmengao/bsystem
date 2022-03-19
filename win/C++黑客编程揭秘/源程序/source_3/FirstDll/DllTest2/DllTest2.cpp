// DllTest2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <windows.h>

typedef VOID (*PFUNMSG)(char *);

int main(int argc, char* argv[])
{
    HMODULE hModule = LoadLibrary("FirstDll.dll");

    if ( hModule == NULL )
    {
        MessageBox(NULL, "FirstDll.dll�ļ�������", 
                   "DLL�ļ�����ʧ��", MB_OK);

        return -1;
    }

    PFUNMSG pFunMsg = (PFUNMSG)GetProcAddress(hModule, "MsgBox");
    pFunMsg("Hello First Dll !");

	return 0;
}
