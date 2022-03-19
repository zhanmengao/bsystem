#include<stdio.h>
#include"LogManager.hpp"
#include"PathHelper.hpp"
#include<Windows.h>
#include"../DLLע��/DLLUtils.h"

//�䵱regsvr32�Ĺ���
void main(int argc, _TCHAR* argv[])
{
	// ����DLL����ȡ���
	HMODULE hDll = ::LoadLibrary(_T("HookWTSSend.dll"));
	if (NULL == hDll)
	{
		printf("%s error[%d]\n", "LoadLibrary", ::GetLastError());
		return;
	}
	printf("Load Library OK.\n");

	// ����ȫ�ֹ���
	typedef HHOOK(*typedef_SetHook)();
	typedef_SetHook SetHook = (typedef_SetHook)::GetProcAddress(hDll, "SetHook");
	if (NULL == SetHook)
	{
		printf("GetProcAddress Error[%d]\n", ::GetLastError());
	}
	HHOOK hHook = SetHook();
	if (NULL == hHook)
	{
		printf("%s error[%d]\n", "SetWindowsHookEx", ::GetLastError());
	}
	printf("Set Windows Hook OK.\n");
	system("pause");
	// ж��ȫ�ֹ���
	typedef BOOL(*typedef_UnsetHook)(HHOOK);
	typedef_UnsetHook UnsetHook = (typedef_UnsetHook)::GetProcAddress(hDll, "UnsetHook");
	if (NULL == UnsetHook)
	{
		printf("GetProcAddress Error[%d]\n", ::GetLastError());
	}
	if (FALSE == UnsetHook(hHook))
	{
		printf("%s error[%d]\n", "UnhookWindowsHookE", ::GetLastError());
	}
	printf("Unhook Windows Hook OK.\n");
	// ж��DLL
	::FreeLibrary(hDll);

	system("pause");
}

#define PID 15436
#ifdef _WIN64
#define DLL_PATH "D:\\Space\\System\\Windows\\DLLProject\\HookMessage\\x64\\Release\\HookWTSSend.dll"
#else
#define DLL_PATH "D:\\Space\\System\\Windows\\DLLProject\\HookMessage\\Release\\HookWTSSend.dll"
#endif
//���� Զ�߳�ע��
void main2(int argc, _TCHAR* argv[])
{
	// ����DLL����ȡ���
	HMODULE hDll = ::LoadLibrary(_T(DLL_PATH));
	if (NULL == hDll)
	{
		printf("%s error[%d]\n", "LoadLibrary", ::GetLastError());
		return;
	}
	printf("Load Library OK.\n");
	DLLUtils::InjectDLLThread(PID, DLL_PATH);
	printf("Hook OK.\n");
	system("pause");
	// ж��DLL
	DLLUtils::FreeDLL(PID, TEXT(DLL_PATH));
	::FreeLibrary(hDll);

	system("pause");
}