// AutoStartProcessGuardHook.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <Windows.h>


#if defined _M_X64
#pragma comment(lib, "../ProcessGuard/x64/Debug/ProcessGuard.lib")
#elif defined _M_IX86
#pragma comment(lib, "../ProcessGuard/Release/ProcessGuard_Win32.lib")
#endif

EXTERN_C void StartMonitor();
EXTERN_C void StopMonitor();

#pragma comment(linker,"/subsystem:\"windows\" /entry:\"mainCRTStartup\"")

int main()
{
	StartMonitor();

	while ( true )
	{
		/**
		* ά�ֽ��̲��˳�
		*/
		Sleep( 1000 * 60 * 1 );
	}
	
	StopMonitor();

    return 0;
}

