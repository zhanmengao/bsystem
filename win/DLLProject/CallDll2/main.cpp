#include<Windows.h>
#include<wchar.h>
#include<tchar.h>
#include<string>
#include<olectl.h>

typedef VOID(*PFUNMSG)(WCHAR *);
typedef void(*pMainLogic)();
typedef unsigned (*ReturnErrInfo)(ULONG info, char* buf, int& len);
void main()
{
	HMODULE hModule = LoadLibrary(L"Dll1.dll");

	if (hModule == NULL)
	{
		MessageBox(NULL, L"LoadLibrary Dll1.dll error!", L"error",MB_OK);
	}
	else
	{
		PFUNMSG pFunc = (PFUNMSG)GetProcAddress(hModule, "MsgBox");
		if (pFunc == NULL)
		{
			MessageBox(NULL, L"GetProcAddress MsgBox error!", std::to_wstring(GetLastError()).c_str(), MB_OK);
		}
		else
		{
			pFunc(L"Hello First Dll!");
		}
	}



	hModule = LoadLibrary(L"OpenService.dll");
	if (hModule == NULL)
	{
		MessageBox(NULL, L"LoadLibrary OpenService.dll error!", std::to_wstring(GetLastError()).c_str(), MB_OK);
	}
	else
	{
		pMainLogic pMain = (pMainLogic)GetProcAddress(hModule, "mainLogic");
		if (pMain == NULL)
		{
			MessageBox(NULL, L"GetProcAddress mainLogic error!", L"error", MB_OK);
		}
		pMain();
	}


	//hModule = LoadLibrary(L"SessionDLL.dll");
	hModule = LoadLibrary(L"RemoteAppDLL.dll");
	if (hModule == NULL)
	{
		MessageBox(NULL, L"LoadLibrary SessionDLL.dll error!", std::to_wstring(GetLastError()).c_str(), MB_OK);
	}
	else
	{
		ReturnErrInfo pError = (ReturnErrInfo)GetProcAddress(hModule, "ReturnErrInfo");
		if (pError == NULL)
		{
			MessageBox(NULL, L"GetProcAddress pError error!", L"error", MB_OK);
		}
		char buf[1024] = { 0 };
		int len = 0;
		pError(5, buf, len);
		printf(buf);
	}
}