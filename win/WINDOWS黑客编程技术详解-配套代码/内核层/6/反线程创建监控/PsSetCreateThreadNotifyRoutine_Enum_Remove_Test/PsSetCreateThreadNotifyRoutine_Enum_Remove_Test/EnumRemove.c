#include "EnumRemove.h"


VOID ShowError(PCHAR lpszText, NTSTATUS ntStatus)
{
	DbgPrint("%s Error[0x%X]\n", lpszText, ntStatus);
}


// �����ص�
BOOLEAN EnumNotifyRoutine()
{
	ULONG i = 0;
	PVOID pPspCreateThreadNotifyRoutineAddress = NULL;
	PVOID pNotifyRoutineAddress = NULL;

	// ��ȡ PspCreateThreadNotifyRoutine �����ַ
	pPspCreateThreadNotifyRoutineAddress = GetPspCreateThreadNotifyRoutine();
	if (NULL == pPspCreateThreadNotifyRoutineAddress)
	{
		DbgPrint("GetPspCreateThreadNotifyRoutine Error!\n");
		return FALSE;
	}
	DbgPrint("pPspCreateThreadNotifyRoutineAddress=0x%p\n", pPspCreateThreadNotifyRoutineAddress);

	// ��ȡ�ص���ַ������
#ifdef _WIN64
	for (i = 0; i < 64; i++)
	{
		pNotifyRoutineAddress = *(PVOID *)((PUCHAR)pPspCreateThreadNotifyRoutineAddress + sizeof(PVOID) * i);
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
		pNotifyRoutineAddress = *(PVOID *)((PUCHAR)pPspCreateThreadNotifyRoutineAddress + sizeof(PVOID) * i);
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
	NTSTATUS status = PsRemoveCreateThreadNotifyRoutine((PCREATE_THREAD_NOTIFY_ROUTINE)pNotifyRoutineAddress);
	if (!NT_SUCCESS(status))
	{
		ShowError("PsRemoveCreateThreadNotifyRoutine", status);
	}
	return status;
}


// ��ȡ PspCreateThreadNotifyRoutine �����ַ
PVOID GetPspCreateThreadNotifyRoutine()
{
	PVOID pPspCreateThreadNotifyRoutineAddress = NULL;
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
			// 488D0D
			pFirstSpecialData[0] = 0x48;
			pFirstSpecialData[1] = 0x8D;
			pFirstSpecialData[2] = 0x0D;
			ulFirstSpecialDataSize = 3;
#else
			// 32 λ
			// BE
			pFirstSpecialData[0] = 0xBE;
			ulFirstSpecialDataSize = 1;
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
			pFirstSpecialData[0] = 0x48;
			pFirstSpecialData[1] = 0x8D;
			pFirstSpecialData[2] = 0x0D;
			ulFirstSpecialDataSize = 3;
#else
			// 32 λ
			// BB
			pFirstSpecialData[0] = 0xBB;
			ulFirstSpecialDataSize = 1;
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
		// 488D0D
		pSecondSpecialData[0] = 0x48;
		pSecondSpecialData[1] = 0x8D;
		pSecondSpecialData[2] = 0x0D;
		ulSecondSpecialDataSize = 3;
#else
		// 32 λ
		// E8
		pFirstSpecialData[0] = 0xE8;
		ulFirstSpecialDataSize = 1;
		// BF
		pSecondSpecialData[0] = 0xBF;
		ulSecondSpecialDataSize = 1;
#endif
	}

	// �����������ȡ��ַ
	pPspCreateThreadNotifyRoutineAddress = SearchPspCreateThreadNotifyRoutine(pFirstSpecialData, ulFirstSpecialDataSize, pSecondSpecialData, ulSecondSpecialDataSize);
	return pPspCreateThreadNotifyRoutineAddress;
}


// �����������ȡ PspCreateThreadNotifyRoutine �����ַ
PVOID SearchPspCreateThreadNotifyRoutine(PUCHAR pFirstSpecialData, ULONG ulFirstSpecialDataSize, PUCHAR pSecondSpecialData, ULONG ulSecondSpecialDataSize)
{
	UNICODE_STRING ustrFuncName;
	PVOID pAddress = NULL;
	LONG lOffset = 0;
	PVOID pPsSetCreateThreadNotifyRoutine = NULL;
	PVOID pPspSetCreateThreadNotifyRoutineAddress = NULL;
	PVOID pPspCreateThreadNotifyRoutineAddress = NULL;

	// �Ȼ�ȡ PsSetCreateThreadNotifyRoutine ������ַ
	RtlInitUnicodeString(&ustrFuncName, L"PsSetCreateThreadNotifyRoutine");
	pPsSetCreateThreadNotifyRoutine = MmGetSystemRoutineAddress(&ustrFuncName);
	if (NULL == pPsSetCreateThreadNotifyRoutine)
	{
		ShowError("MmGetSystemRoutineAddress", 0);
		return pPspCreateThreadNotifyRoutineAddress;
	}

	// Ȼ��, ���ڷ� Win10 ϵͳ, ����ݵ�һ���������ȡ PspCreateThreadNotifyRoutine ��ַ;
	// ���� Win10 ϵͳ, �����ȸ��ݵ�һ���������ȡ��ȡ PspSetCreateThreadNotifyRoutine ��ַ, 
	// �ٸ��ݵڶ����������ȡ PspCreateThreadNotifyRoutine ��ַ.
	pAddress = SearchMemory(pPsSetCreateThreadNotifyRoutine,
		(PVOID)((PUCHAR)pPsSetCreateThreadNotifyRoutine + 0xFF),
		pFirstSpecialData, ulFirstSpecialDataSize);
	if (NULL == pAddress)
	{
		ShowError("SearchMemory1", 0);
		return pPspCreateThreadNotifyRoutineAddress;
	}

	// �޵ڶ���������, ��� Win10 ϵͳ
	if (0 == ulSecondSpecialDataSize)
	{
		// ��ȡ PspCreateThreadNotifyRoutine ��ַ
#ifdef _WIN64
		// 64 λ
		// ��ȡƫ������, �������ַ
		lOffset = *(PLONG)pAddress;
		pPspCreateThreadNotifyRoutineAddress = (PVOID)((PUCHAR)pAddress + sizeof(LONG) + lOffset);
#else
		// 32 λ
		pPspCreateThreadNotifyRoutineAddress = *(PVOID *)pAddress;
#endif 
		
		// ֱ�ӷ���
		return pPspCreateThreadNotifyRoutineAddress;
	}

	// ���ڵڶ���������, �� Win10 ϵͳ
	// ��ȡƫ������, �������ַ
	lOffset = *(PLONG)pAddress;
	pPspSetCreateThreadNotifyRoutineAddress = (PVOID)((PUCHAR)pAddress + sizeof(LONG) + lOffset);
	// ���, ���� PspCreateThreadNotifyRoutine ��ַ
	pAddress = SearchMemory(pPspSetCreateThreadNotifyRoutineAddress,
		(PVOID)((PUCHAR)pPspSetCreateThreadNotifyRoutineAddress + 0xFF),
		pSecondSpecialData, ulSecondSpecialDataSize);
	if (NULL == pAddress)
	{
		ShowError("SearchMemory2", 0);
		return pPspCreateThreadNotifyRoutineAddress;
	}
	// ��ȡ PspCreateThreadNotifyRoutine ��ַ
#ifdef _WIN64
	// 64 λ�Ȼ�ȡƫ��, �ټ����ַ
	lOffset = *(PLONG)pAddress;
	pPspCreateThreadNotifyRoutineAddress = (PVOID)((PUCHAR)pAddress + sizeof(LONG) + lOffset);
#else
	// 32 λֱ�ӻ�ȡ��ַ
	pPspCreateThreadNotifyRoutineAddress = *(PVOID *)pAddress;
#endif

	return pPspCreateThreadNotifyRoutineAddress;
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