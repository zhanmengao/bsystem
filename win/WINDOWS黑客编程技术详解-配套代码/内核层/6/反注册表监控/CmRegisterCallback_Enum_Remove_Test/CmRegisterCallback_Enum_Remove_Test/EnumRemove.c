#include "EnumRemove.h"


VOID ShowError(PCHAR lpszText, NTSTATUS ntStatus)
{
	DbgPrint("%s Error[0x%X]\n", lpszText, ntStatus);
}


// �����ص�
BOOLEAN EnumCallback()
{
	ULONG i = 0;
	PVOID pCallbackListHeadAddress = NULL;
	PCM_NOTIFY_ENTRY pNotifyEntry = NULL;

	// ��ȡ CallbackListHead �����ַ
	pCallbackListHeadAddress = GetCallbackListHead();
	if (NULL == pCallbackListHeadAddress)
	{
		DbgPrint("GetCallbackListHead Error!\n");
		return FALSE;
	}
	DbgPrint("pCallbackListHeadAddress=0x%p\n", pCallbackListHeadAddress);

	// ��ʼ����˫������
	pNotifyEntry = (PCM_NOTIFY_ENTRY)pCallbackListHeadAddress;
	do
	{
		// �ж� pNotifyEntry ��ַ�Ƿ���Ч
		if (FALSE == MmIsAddressValid(pNotifyEntry))
		{
			break;
		}
		// �ж� �ص����� ��ַ�Ƿ���Ч
		if (MmIsAddressValid(pNotifyEntry->Function))
		{
			// ��ʾ
			DbgPrint("CallbackFunction=0x%p, Cookie=0x%I64X\n", pNotifyEntry->Function, pNotifyEntry->Cookie.QuadPart);
		}
		// ��ȡ��һ����
		pNotifyEntry = (PCM_NOTIFY_ENTRY)pNotifyEntry->ListEntryHead.Flink;

	} while (pCallbackListHeadAddress != (PVOID)pNotifyEntry);


	return TRUE;
}


// �Ƴ��ص�
NTSTATUS RemoveCallback(LARGE_INTEGER Cookie)
{
	NTSTATUS status = CmUnRegisterCallback(Cookie);
	if (!NT_SUCCESS(status))
	{
		ShowError("CmUnRegisterCallback", status);
	}
	return status;
}


// ��ȡ CallbackListHead �����ַ
PVOID GetCallbackListHead()
{
	PVOID pCallbackListHeadAddress = NULL;
	RTL_OSVERSIONINFOW osInfo = { 0 };
	UCHAR pSpecialData[50] = { 0 };
	ULONG ulSpecialDataSize = 0;
	LONG lSpecialOffset = 0;

	// ��ȡϵͳ�汾��Ϣ, �ж�ϵͳ�汾
	RtlGetVersion(&osInfo);
	if (6 == osInfo.dwMajorVersion)
	{
		if (1 == osInfo.dwMinorVersion)
		{
			// Win7
#ifdef _WIN64
			// 64 λ
			// 488D54
			pSpecialData[0] = 0x48;
			pSpecialData[1] = 0x8D;
			pSpecialData[2] = 0x54;
			ulSpecialDataSize = 3;
			lSpecialOffset = 5;
#else
			// 32 λ
			// BF
			pSpecialData[0] = 0xBF;
			ulSpecialDataSize = 1;
#endif	
		}
		else if (2 == osInfo.dwMinorVersion)
		{
			// Win8
#ifdef _WIN64
			// 64 λ
			
#else
			// 32 λ
			
#endif
		}
		else if (3 == osInfo.dwMinorVersion)
		{
			// Win8.1
#ifdef _WIN64
			// 64 λ
			// 488D0D
			pSpecialData[0] = 0x48;
			pSpecialData[1] = 0x8D;
			pSpecialData[2] = 0x0D;
			ulSpecialDataSize = 3;
#else
			// 32 λ
			// BE
			pSpecialData[0] = 0xBE;
			ulSpecialDataSize = 1;
#endif			
		}
	}
	else if (10 == osInfo.dwMajorVersion)
	{
		// Win10
#ifdef _WIN64
		// 64 λ
		// 488D0D
		pSpecialData[0] = 0x48;
		pSpecialData[1] = 0x8D;
		pSpecialData[2] = 0x0D;
		ulSpecialDataSize = 3;
#else
		// 32 λ
		// B9
		pSpecialData[0] = 0xB9;
		ulSpecialDataSize = 1;
#endif
	}

	// �����������ȡ��ַ
	pCallbackListHeadAddress = SearchCallbackListHead(pSpecialData, ulSpecialDataSize, lSpecialOffset);
	return pCallbackListHeadAddress;
}


// �����������ȡ CallbackListHead �����ַ
PVOID SearchCallbackListHead(PUCHAR pSpecialData, ULONG ulSpecialDataSize, LONG lSpecialOffset)
{
	UNICODE_STRING ustrFuncName;
	PVOID pAddress = NULL;
	LONG lOffset = 0;
	PVOID pCmUnRegisterCallback = NULL;
	PVOID pCallbackListHead = NULL;

	// �Ȼ�ȡ CmUnRegisterCallback ������ַ
	RtlInitUnicodeString(&ustrFuncName, L"CmUnRegisterCallback");
	pCmUnRegisterCallback = MmGetSystemRoutineAddress(&ustrFuncName);
	if (NULL == pCmUnRegisterCallback)
	{
		ShowError("MmGetSystemRoutineAddress", 0);
		return pCallbackListHead;
	}

	// Ȼ��, ���� PspSetCreateProcessNotifyRoutine ������ַ
	pAddress = SearchMemory(pCmUnRegisterCallback,
		(PVOID)((PUCHAR)pCmUnRegisterCallback + 0xFF),
		pSpecialData, ulSpecialDataSize);
	if (NULL == pAddress)
	{
		ShowError("SearchMemory", 0);
		return pCallbackListHead;
	}

	// ��ȡ��ַ
#ifdef _WIN64
	// 64 λ�Ȼ�ȡƫ��, �ټ����ַ
	lOffset = *(PLONG)((PUCHAR)pAddress + lSpecialOffset);
	pCallbackListHead = (PVOID)((PUCHAR)pAddress + lSpecialOffset + sizeof(LONG) + lOffset);
#else
	// 32 λֱ�ӻ�ȡ��ַ
	pCallbackListHead = *(PVOID *)((PUCHAR)pAddress + lSpecialOffset);
#endif

	return pCallbackListHead;
}


// ָ���ڴ������������ɨ��
PVOID SearchMemory(PVOID pStartAddress, PVOID pEndAddress, PUCHAR pMemoryData, ULONG ulMemoryDataSize)
{
	PVOID pAddress = NULL;
	PUCHAR i = NULL;
	ULONG m = 0;

	// ɨ���ڴ�
	for (i = (PUCHAR)pStartAddress; i < (PUCHAR)pEndAddress; i++)
	{
		// �ж�������
		for (m = 0; m < ulMemoryDataSize; m++)
		{
			if (*(PUCHAR)(i + m) != pMemoryData[m])
			{
				break;
			}
		}
		// �ж��Ƿ��ҵ�����������ĵ�ַ
		if (m >= ulMemoryDataSize)
		{
			// �ҵ�������λ��, ��ȡ���������������һ��ַ
			pAddress = (PVOID)(i + ulMemoryDataSize);
			break;
		}
	}

	return pAddress;
}