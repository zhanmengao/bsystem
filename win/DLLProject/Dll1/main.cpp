#include<Windows.h>

HINSTANCE hinstDLL = NULL;
//DllMain���Ǳ����
//DLL�ļ�������Ϊ�����Ŀ�ִ���ļ��ṩʹ��
BOOL WINAPI DllMain(HINSTANCE hinstDLL,DWORD fdwReason,LPVOID lpvREserved)
{
	switch (fdwReason)
	{
	//DLL��ĳ���̼���s
	case DLL_PROCESS_ATTACH:
		hinstDLL = hinstDLL;
		break;
	//DLL��ĳ����ж��
	case DLL_PROCESS_DETACH:
		break;
	//���������̱߳�����
	case DLL_THREAD_ATTACH:
		break;
	//���������߳̽���
	case DLL_THREAD_DETACH:
		break;
	}
	return TRUE;
}