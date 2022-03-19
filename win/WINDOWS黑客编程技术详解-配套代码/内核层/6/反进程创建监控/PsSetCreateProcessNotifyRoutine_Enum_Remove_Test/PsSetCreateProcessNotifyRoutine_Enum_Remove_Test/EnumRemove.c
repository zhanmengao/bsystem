#include "EnumRemove.h"


VOID ShowError(PCHAR lpszText, NTSTATUS ntStatus)
{
	DbgPrint("%s Error[0x%X]\n", lpszText, ntStatus);
}


// �����ص�
BOOLEAN EnumNotifyRoutine()
{
	ULONG i = 0;
	PVOID pPspCreateProcessNotifyRoutineAddress = NULL;
	PVOID pNotifyRoutineAddress = NULL;

	// ��ȡ PspCreateProcessNotifyRoutine �����ַ
	pPspCreateProcessNotifyRoutineAddress = GetPspCreateProcessNotifyRoutine();
	if (NULL == pPspCreateProcessNotifyRoutineAddress)
	{
		DbgPrint("GetPspCreateProcessNotifyRoutine Error!\n");
		return FALSE;
	}
	DbgPrint("pPspCreateProcessNotifyRoutineAddress=0x%p\n", pPspCreateProcessNotifyRoutineAddress);

	// ��ȡ�ص���ַ������
#ifdef _WIN64
	for (i = 0; i < 64; i++)
	{
		pNotifyRoutineAddress = *(PVOID *)((PUCHAR)pPspCreateProcessNotifyRoutineAddress + sizeof(PVOID) * i);
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
		pNotifyRoutineAddress = *(PVOID *)((PUCHAR)pPspCreateProcessNotifyRoutineAddress + sizeof(PVOID) * i);
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
	NTSTATUS status = PsSetCreateProcessNotifyRoutine((PCREATE_PROCESS_NOTIFY_ROUTINE)pNotifyRoutineAddress, TRUE);
	if (!NT_SUCCESS(status))
	{
		ShowError("PsSetCreateProcessNotifyRoutine", status);
	}
	return status;
}


// ��ȡ PspCreateProcessNotifyRoutine �����ַ
PVOID GetPspCreateProcessNotifyRoutine()
{
	PVOID pPspCreateProcessNotifyRoutineAddress = NULL;
	RTL_OSVERSIONINFOW osInfo = { 0 };
	UCHAR pFirstSpecialData[50] = { 0 };
	ULONG ulFirstSpecialDataSize = 0;
	UCHAR pSecondSpecialData[50] = { 0 };
	ULONG ulSecondSpecialDataSize = 0;

	// ��ȡϵͳ�汾��Ϣ, �ж�ϵͳ�汾
	RtlGetVersion(&osInfo);
	if (6 == osInfo.dwMajorVersion)
	{
		if (1 == osInfo.dwMinorVersion)
		{
			// Win7
#ifdef _WIN64
			// 64 λ
			// E9
			pFirstSpecialData[0] = 0xE9;
			ulFirstSpecialDataSize = 1;
			// 4C8D35
			pSecondSpecialData[0] = 0x4C;
			pSecondSpecialData[1] = 0x8D;
			pSecondSpecialData[2] = 0x35;
			ulSecondSpecialDataSize = 3;
#else
			// 32 λ
			// E8
			pFirstSpecialData[0] = 0xE8;
			ulFirstSpecialDataSize = 1;
			// C7450C
			pSecondSpecialData[0] = 0xC7;
			pSecondSpecialData[1] = 0x45;
			pSecondSpecialData[2] = 0x0C;
			ulSecondSpecialDataSize = 3;
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
			// E9
			pFirstSpecialData[0] = 0xE9;
			ulFirstSpecialDataSize = 1;
			// 4C8D3D
			pSecondSpecialData[0] = 0x4C;
			pSecondSpecialData[1] = 0x8D;
			pSecondSpecialData[2] = 0x3D;
			ulSecondSpecialDataSize = 3;
#else
			// 32 λ
			// E8
			pFirstSpecialData[0] = 0xE8;
			ulFirstSpecialDataSize = 1;
			// B8
			pSecondSpecialData[0] = 0xB8;
			ulSecondSpecialDataSize = 1;
#endif			
		}
	}
	else if (10 == osInfo.dwMajorVersion)
	{
		// Win10
#ifdef _WIN64
		// 64 λ
		// E9
		pFirstSpecialData[0] = 0xE9;
		ulFirstSpecialDataSize = 1;
		// 4C8D3D
		pSecondSpecialData[0] = 0x4C;
		pSecondSpecialData[1] = 0x8D;
		pSecondSpecialData[2] = 0x3D;
		ulSecondSpecialDataSize = 3;
#else
		// 32 λ
		// E8
		pFirstSpecialData[0] = 0xE8;
		ulFirstSpecialDataSize = 1;
		// BB
		pSecondSpecialData[0] = 0xBB;
		ulSecondSpecialDataSize = 1;
#endif
	}

	// �����������ȡ��ַ
	pPspCreateProcessNotifyRoutineAddress = SearchPspCreateProcessNotifyRoutine(pFirstSpecialData, ulFirstSpecialDataSize, pSecondSpecialData, ulSecondSpecialDataSize);
	return pPspCreateProcessNotifyRoutineAddress;
}


// �����������ȡ PspCreateProcessNotifyRoutine �����ַ
PVOID SearchPspCreateProcessNotifyRoutine(PUCHAR pFirstSpecialData, ULONG ulFirstSpecialDataSize, PUCHAR pSecondSpecialData, ULONG ulSecondSpecialDataSize)
{
	UNICODE_STRING ustrFuncName;
	PVOID pAddress = NULL;
	LONG lOffset = 0;
	PVOID pPsSetCteateProcessNotifyRoutine = NULL;
	PVOID pPspSetCreateProcessNotifyRoutineAddress = NULL;
	PVOID pPspCreateProcessNotifyRoutineAddress = NULL;

	// �Ȼ�ȡ PsSetCreateProcessNotifyRoutine ������ַ
	RtlInitUnicodeString(&ustrFuncName, L"PsSetCreateProcessNotifyRoutine");
	pPsSetCteateProcessNotifyRoutine = MmGetSystemRoutineAddress(&ustrFuncName);
	if (NULL == pPsSetCteateProcessNotifyRoutine)
	{
		ShowError("MmGetSystemRoutineAddress", 0);
		return pPspCreateProcessNotifyRoutineAddress;
	}

	// Ȼ��, ���� PspSetCreateProcessNotifyRoutine ������ַ
	pAddress = SearchMemory(pPsSetCteateProcessNotifyRoutine,
		(PVOID)((PUCHAR)pPsSetCteateProcessNotifyRoutine + 0xFF),
		pFirstSpecialData, ulFirstSpecialDataSize);
	if (NULL == pAddress)
	{
		ShowError("SearchMemory1", 0);
		return pPspCreateProcessNotifyRoutineAddress;
	}
	// ��ȡƫ������, �������ַ
	lOffset = *(PLONG)pAddress;
	pPspSetCreateProcessNotifyRoutineAddress = (PVOID)((PUCHAR)pAddress + sizeof(LONG) + lOffset);

	// ���, ���� PspCreateProcessNotifyRoutine ��ַ
	pAddress = SearchMemory(pPspSetCreateProcessNotifyRoutineAddress,
		(PVOID)((PUCHAR)pPspSetCreateProcessNotifyRoutineAddress + 0xFF),
		pSecondSpecialData, ulSecondSpecialDataSize);
	if (NULL == pAddress)
	{
		ShowError("SearchMemory2", 0);
		return pPspCreateProcessNotifyRoutineAddress;
	}
	// ��ȡ��ַ
#ifdef _WIN64
	// 64 λ�Ȼ�ȡƫ��, �ټ����ַ
	lOffset = *(PLONG)pAddress;
	pPspCreateProcessNotifyRoutineAddress = (PVOID)((PUCHAR)pAddress + sizeof(LONG) + lOffset);
#else
	// 32 λֱ�ӻ�ȡ��ַ
	pPspCreateProcessNotifyRoutineAddress = *(PVOID *)pAddress;
#endif

	return pPspCreateProcessNotifyRoutineAddress;
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