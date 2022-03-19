#include<Windows.h>

HINSTANCE hinstDLL = NULL;
//DllMain不是必须的
//DLL文件本质是为其他的可执行文件提供使用
BOOL WINAPI DllMain(HINSTANCE hinstDLL,DWORD fdwReason,LPVOID lpvREserved)
{
	switch (fdwReason)
	{
	//DLL被某进程加载s
	case DLL_PROCESS_ATTACH:
		hinstDLL = hinstDLL;
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