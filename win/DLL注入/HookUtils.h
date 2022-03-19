#pragma once
#include<Windows.h>
#include"../../../lib/MinHook/include/MinHook.h"

class HookUtils
{
public:
	//��Ϣ���ӣ���DLL
	static HHOOK RegMessageHook(HOOKPROC proc, HINSTANCE dllModule);
	static BOOL DisMessageHook(HHOOK hook);

	//Hookĳ������
	static MH_STATUS HookFunction(void* pOld, void* pNew, void** pOldFunc);
	static MH_STATUS UnHookFunction(void* pOld);
	static MH_STATUS UnInitialize();
};