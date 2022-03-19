// DllTest1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>

#pragma comment (lib, "FirstDll")

extern "C" VOID MsgBox(char *szMsg);

int main(int argc, char* argv[])
{
    MsgBox("Hello First Dll !");

	return 0;
}
