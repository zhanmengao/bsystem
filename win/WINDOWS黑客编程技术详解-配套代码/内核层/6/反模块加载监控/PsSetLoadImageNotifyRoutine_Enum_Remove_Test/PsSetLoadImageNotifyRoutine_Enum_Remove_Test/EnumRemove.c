#include "EnumRemove.h"


VOID ShowError(PCHAR lpszText, NTSTATUS ntStatus)
{
	DbgPrint("%s Error[0x%X]\n", lpszText, ntStatus);
}


// �����ص�
BOOLEAN EnumNotifyRoutine()
{
	ULONG i = 0;
	PVOID pPspLoadImageNotifyRoutineAddress = NULL;
	PVOID pNotifyRoutineAddress = NULL;

	// ��ȡ PspLoadImageNotifyRoutine �����ַ
	pPspLoadImageNotifyRoutineAddress = GetPspLoadImageNotifyRoutine();
	if (NULL == pPspLoadImageNotifyRoutineAddress)
	{
		DbgPrint("GetPspLoadImageNotifyRoutine Error!\n");
		return FALSE;
	}
	DbgPrint("pPspLoadImageNotifyRoutineAddress=0x%p\n", pPspLoadImageNotifyRoutineAddress);

	// ��ȡ�ص���ַ������
#ifdef _WIN64
	for (i = 0; i < 64; i++)
	{
		pNotifyRoutineAddress = *(PVOID *)((PUCHAR)pPspLoadImageNotifyRoutineAddress + sizeof(PVOID) * i);
		pNotifyRoutineAddress = (PVOID)((ULONG64)pNotifyRoutineAddress & 0xfffffffffffffff8);
		if (MmIsAddressValid(pNotifyRoutineAddress))
		{
			pNotifyRoutineAddress = *(PVOID *)pNotifyRoutineAddress;
			DbgPrint("[%d]ullNotifyRoutine=0x%p\n", i, pNotifyRoutineAddress);
		}
	}
#else
	for (i = 0; i < 8; i++)
	{
		pNotifyRoutineAddress = *(PVOID *)((PUCHAR)pPspLoadImageNotifyRoutineAddress + sizeof(PVOID) * i);
		pNotifyRoutineAddress = (PVOID)((ULONG)pNotifyRoutineAddress & 0xfffffff8);
		if (MmIsAddressValid(pNotifyRoutineAddress))
		{
			pNotifyRoutineAddress = *(PVOID *)((PUCHAR)pNotifyRoutineAddress + 4);
			DbgPrint("[%d]ullNotifyRoutine=0x%p\n", i, pNotifyRoutineAddress);
		}
	}
#endif

	return TRUE;
}


// �Ƴ��ص�
NTSTATUS RemoveNotifyRoutine(PVOID pNotifyRoutineAddress)
{
	NTSTATUS status = PsRemoveLoadImageNotifyRoutine((PLOAD_IMAGE_NOTIFY_ROUTINE)pNotifyRoutineAddress);
	if (!NT_SUCCESS(status))
	{
		ShowError("PsRemoveLoadImageNotifyRoutine", status);
	}
	return status;
}


// ��ȡ PspLoadImageNotifyRoutine �����ַ
PVOID GetPspLoadImageNotifyRoutine()
{
	PVOID pPspLoadImageNotifyRoutineAddress = NULL;
	RTL_OSVERSIONINFOW osInfo = { 0 };
	UCHAR pSpecialData[50] = { 0 };
	ULONG ulSpecialDataSize = 0;

	// ��ȡϵͳ�汾��Ϣ, �ж�ϵͳ�汾
	RtlGetVersion(&osInfo);
	if (6 == osInfo.dwMajorVersion)
	{
		if (1 == osInfo.dwMinorVersion)
		{
			// Win7
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
			// BB
			pSpecialData[0] = 0xBB;
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
		// BF
		pSpecialData[0] = 0xBF;
		ulSpecialDataSize = 1;
#endif
	}

	// �����������ȡ��ַ
	pPspLoadImageNotifyRoutineAddress = SearchPspLoadImageNotifyRoutine(pSpecialData, ulSpecialDataSize);
	return pPspLoadImageNotifyRoutineAddress;
}


// �����������ȡ PspLoadImageNotifyRoutine �����ַ
PVOID SearchPspLoadImageNotifyRoutine(PUCHAR pSpecialData, ULONG ulSpecialDataSize)
{
	UNICODE_STRING ustrFuncName;
	PVOID pAddress = NULL;
	LONG lOffset = 0;
	PVOID pPsSetLoadImageNotifyRoutine = NULL;
	PVOID pPspLoadImageNotifyRoutine = NULL;

	// �Ȼ�ȡ PsSetLoadImageNotifyRoutine ������ַ
	RtlInitUnicodeString(&ustrFuncName, L"PsSetLoadImageNotifyRoutine");
	pPsSetLoadImageNotifyRoutine = MmGetSystemRoutineAddress(&ustrFuncName);
	if (NULL == pPsSetLoadImageNotifyRoutine)
	{
		ShowError("MmGetSystemRoutineAddress", 0);
		return pPspLoadImageNotifyRoutine;
	}

	// Ȼ��, ���� PspSetCreateProcessNotifyRoutine ������ַ
	pAddress = SearchMemory(pPsSetLoadImageNotifyRoutine,
		(PVOID)((PUCHAR)pPsSetLoadImageNotifyRoutine + 0xFF),
		pSpecialData, ulSpecialDataSize);
	if (NULL == pAddress)
	{
		ShowError("SearchMemory", 0);
		return pPspLoadImageNotifyRoutine;
	}

	// ��ȡ��ַ
#ifdef _WIN64
	// 64 λ�Ȼ�ȡƫ��, �ټ����ַ
	lOffset = *(PLONG)pAddress;
	pPspLoadImageNotifyRoutine = (PVOID)((PUCHAR)pAddress + sizeof(LONG) + lOffset);
#else
	// 32 λֱ�ӻ�ȡ��ַ
	pPspLoadImageNotifyRoutine = *(PVOID *)pAddress;
#endif

	return pPspLoadImageNotifyRoutine;
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