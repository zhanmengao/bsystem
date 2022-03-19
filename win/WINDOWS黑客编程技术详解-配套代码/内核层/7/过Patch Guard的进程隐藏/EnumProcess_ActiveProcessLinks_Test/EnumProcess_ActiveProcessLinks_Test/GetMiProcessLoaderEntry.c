#include "GetMiProcessLoaderEntry.h"
#include "SSDTFunction.h"


// �� MmLoadSystemImage �л�ȡ��Ӧ�� MiProcessLoaderEntry ������
// ����, 32��64λ�� Win7, Win8.1 ֱ�Ӵ� MmLoadSystemImage ������ MiProcessLoaderEntry
// 32��64λ�� Win10 ��Ҫ�� MmLoadSystemImage ������ MiConstructLoaderEntry, �ٴ� MiConstructLoaderEntry ������ MiProcessLoaderEntry
PVOID GetFuncAddr_MiProcessLoaderEntry()
{
	NTSTATUS status = STATUS_SUCCESS;
	RTL_OSVERSIONINFOW osVersionInfo = { 0 };
	PVOID pMmLoadSystemImage = NULL;
	PVOID pMiConstructLoaderEntry = NULL;
	PVOID pMiProcessLoaderEntry = NULL;
	UCHAR pSpecialCode[256] = { 0 };
	ULONG ulSpecialCodeLength = 256;
	ULONG ulSearchLength = 0x1000;
	PVOID pSearchResultAddr = NULL;
	LONG lOffset = 0;
	RtlZeroMemory(pSpecialCode, ulSpecialCodeLength);

	// �� NtSetSystemInformation �л�ȡ MmLoadSystemImage ������ַ
	pMmLoadSystemImage = GetFuncAddr_MmLoadSystemImage();
	if (NULL == pMmLoadSystemImage)
	{
		return pMiProcessLoaderEntry;
	}
	DbgPrint("pMmLoadSystemImage[0x%p]\n", pMmLoadSystemImage);

	// ��ȡϵͳ�汾��Ϣ
	RtlGetVersion(&osVersionInfo);
	if (6 == osVersionInfo.dwMajorVersion)
	{
		// Win7
		if (1 == osVersionInfo.dwMinorVersion)
		{
#ifndef _WIN64
			// 32 Bits
			pSpecialCode[0] = 0x6a;
			pSpecialCode[1] = 0x01;
			pSpecialCode[2] = 0x56;
			pSpecialCode[3] = 0xe8;
			ulSpecialCodeLength = 4;
#else
			// 64 Bits
			pSpecialCode[0] = 0xba;
			pSpecialCode[1] = 0x01;
			pSpecialCode[2] = 0x00;
			pSpecialCode[3] = 0x00;
			pSpecialCode[4] = 0x00;
			pSpecialCode[5] = 0x48;
			pSpecialCode[6] = 0x8b;
			pSpecialCode[7] = 0xcd;
			pSpecialCode[8] = 0xe8;
			ulSpecialCodeLength = 9;
#endif
		}
		// Win8
		else if (2 == osVersionInfo.dwMinorVersion)
		{
			
		}
		// Win8.1
		else if (3 == osVersionInfo.dwMinorVersion)
		{
#ifndef _WIN64
			// 32 Bits
			pSpecialCode[0] = 0x89;
			pSpecialCode[1] = 0x74;
			pSpecialCode[2] = 0x24;
			pSpecialCode[3] = 0x1c;
			pSpecialCode[4] = 0x8b;
			pSpecialCode[5] = 0xcf;
			pSpecialCode[6] = 0xe8;
			ulSpecialCodeLength = 7;
#else
			// 64 Bits
			pSpecialCode[0] = 0x41;
			pSpecialCode[1] = 0x83;
			pSpecialCode[2] = 0xcc;
			pSpecialCode[3] = 0x04;
			pSpecialCode[4] = 0xe8;
			ulSpecialCodeLength = 5;
#endif
		}
	}
	// Win10 
	else if (10 == osVersionInfo.dwMajorVersion)
	{
		// �Ȼ�ȡ MiConstructLoaderEntry, �ٻ�ȡ MiProcessLoaderEntry
#ifndef _WIN64
		// 32 Bits
		pSpecialCode[0] = 0x8d;
		pSpecialCode[1] = 0x54;
		pSpecialCode[2] = 0x24;
		pSpecialCode[3] = 0x4c;
		pSpecialCode[4] = 0x50;
		pSpecialCode[5] = 0xe8;
		ulSpecialCodeLength = 6;
		// ����������
		pSearchResultAddr = SearchSpecialCode(pMmLoadSystemImage, ulSearchLength, pSpecialCode, ulSpecialCodeLength);
		if (NULL == pSearchResultAddr)
		{
			return pMiProcessLoaderEntry;
		}
		// ��ȡƫ��ֵ
		lOffset = *(LONG *)((PUCHAR)pSearchResultAddr + ulSpecialCodeLength);
		// �����ַ(��ת��ַ = ��һ��ָ���ַ + ��תƫ��)
		pMiConstructLoaderEntry = (PVOID)(((PUCHAR)pSearchResultAddr + ulSpecialCodeLength + sizeof(LONG)) + lOffset);
        // ��������
		pSpecialCode[0] = 0x8b;
		pSpecialCode[1] = 0xcb;
		pSpecialCode[2] = 0x42;
		pSpecialCode[3] = 0xe8;
		ulSpecialCodeLength = 4;
		pMmLoadSystemImage = pMiConstructLoaderEntry;
#else
		// 64 Bits
		pSpecialCode[0] = 0x48;
		pSpecialCode[1] = 0x8b;
		pSpecialCode[2] = 0xcf;
		pSpecialCode[3] = 0x89;
		pSpecialCode[4] = 0x44;
		pSpecialCode[5] = 0x24;
		pSpecialCode[6] = 0x20;
		pSpecialCode[7] = 0xe8;
		ulSpecialCodeLength = 8;
		// ����������
		pSearchResultAddr = SearchSpecialCode(pMmLoadSystemImage, ulSearchLength, pSpecialCode, ulSpecialCodeLength);
		if (NULL == pSearchResultAddr)
		{
			return pMiProcessLoaderEntry;
		}
		// ��ȡƫ��ֵ
		lOffset = *(LONG *)((PUCHAR)pSearchResultAddr + ulSpecialCodeLength);
		// �����ַ(��ת��ַ = ��һ��ָ���ַ + ��תƫ��)
		pMiConstructLoaderEntry = (PVOID)(((PUCHAR)pSearchResultAddr + ulSpecialCodeLength + sizeof(LONG)) + lOffset);
		// ��������
		pSpecialCode[0] = 0xba;
		pSpecialCode[1] = 0x01;
		pSpecialCode[2] = 0x00;
		pSpecialCode[3] = 0x00;
		pSpecialCode[4] = 0x00;
		pSpecialCode[5] = 0x48;
		pSpecialCode[6] = 0x8b;
		pSpecialCode[7] = 0xcf;
		pSpecialCode[8] = 0xe8;
		ulSpecialCodeLength = 9;
		pMmLoadSystemImage = pMiConstructLoaderEntry;
#endif
	}
	// �����汾
	else
	{
		
	}

	// ����������
	pSearchResultAddr = SearchSpecialCode(pMmLoadSystemImage, ulSearchLength, pSpecialCode, ulSpecialCodeLength);
	if (NULL == pSearchResultAddr)
	{
		return pMiProcessLoaderEntry;
	}
	// ��ȡƫ��ֵ
	lOffset = *(LONG *)((PUCHAR)pSearchResultAddr + ulSpecialCodeLength);
	// �����ַ(��ת��ַ = ��һ��ָ���ַ + ��תƫ��)
	pMiProcessLoaderEntry = (PVOID)(((PUCHAR)pSearchResultAddr + ulSpecialCodeLength + sizeof(LONG)) + lOffset);

	return pMiProcessLoaderEntry;
}


// �� NtSetSystemInformation �л�ȡ MmLoadSystemImage ������ַ
PVOID GetFuncAddr_MmLoadSystemImage()
{
	NTSTATUS status = STATUS_SUCCESS;
	RTL_OSVERSIONINFOW osVersionInfo = { 0 };
	PVOID pNtSetSystemInformation = NULL;
	PVOID pMmLoadSystemImage = NULL;
	UCHAR pSpecialCode[256] = { 0 };
	ULONG ulSpecialCodeLength = 256;
	ULONG ulSearchLength = 0x1000;
	PVOID pSearchResultAddr = NULL;
	LONG lOffset = 0;
	RtlZeroMemory(pSpecialCode, ulSpecialCodeLength);

	// �� SSDT �л�ȡ NtSetSystemInformation ������ַ
	pNtSetSystemInformation = GetSSDTFunction("NtSetSystemInformation");
	if (NULL == pNtSetSystemInformation)
	{
		return pMmLoadSystemImage;
	}

	// ��ȡϵͳ�汾��Ϣ
	RtlGetVersion(&osVersionInfo);
	if (6 == osVersionInfo.dwMajorVersion)
	{
		// Win7
		if (1 == osVersionInfo.dwMinorVersion)
		{
#ifndef _WIN64
			// 32 Bits
			pSpecialCode[0] = 0xd8;
			pSpecialCode[1] = 0x50;
			pSpecialCode[2] = 0xe8;
			ulSpecialCodeLength = 3;
#else
			// 64 Bits
			pSpecialCode[0] = 0x48;
			pSpecialCode[1] = 0x8d;
			pSpecialCode[2] = 0x4c;
			pSpecialCode[3] = 0x24;
			pSpecialCode[4] = 0x38;
			pSpecialCode[5] = 0xe8;
			ulSpecialCodeLength = 6;
#endif
		}
		// Win8
		else if (2 == osVersionInfo.dwMinorVersion)
		{
			
		}
		// Win8.1
		else if (3 == osVersionInfo.dwMinorVersion)
		{
#ifndef _WIN64
			// 32 Bits
			pSpecialCode[0] = 0x8d;
			pSpecialCode[1] = 0x85;
			pSpecialCode[2] = 0x10;
			pSpecialCode[3] = 0xff;
			pSpecialCode[4] = 0xff;
			pSpecialCode[5] = 0xff;
			pSpecialCode[6] = 0x50;
			pSpecialCode[7] = 0xe8;
			ulSpecialCodeLength = 8;
#else
			// 64 Bits
			pSpecialCode[0] = 0x48;
			pSpecialCode[1] = 0x8d;
			pSpecialCode[2] = 0x8c;
			pSpecialCode[3] = 0x24;
			pSpecialCode[4] = 0x00;
			pSpecialCode[5] = 0x02;
			pSpecialCode[6] = 0x00;
			pSpecialCode[7] = 0x00;
			pSpecialCode[8] = 0xe8;
			ulSpecialCodeLength = 9;
#endif
		}
	}
	// Win10 
	else if (10 == osVersionInfo.dwMajorVersion)
	{
#ifndef _WIN64
		// 32 Bits
		pSpecialCode[0] = 0x8d;
		pSpecialCode[1] = 0x85;
		pSpecialCode[2] = 0x04;
		pSpecialCode[3] = 0xff;
		pSpecialCode[4] = 0xff;
		pSpecialCode[5] = 0xff;
		pSpecialCode[6] = 0x50;
		pSpecialCode[7] = 0xe8;
		ulSpecialCodeLength = 8;
#else
		// 64 Bits
		pSpecialCode[0] = 0x48;
		pSpecialCode[1] = 0x8d;
		pSpecialCode[2] = 0x8c;
		pSpecialCode[3] = 0x24;
		pSpecialCode[4] = 0x48;
		pSpecialCode[5] = 0x02;
		pSpecialCode[6] = 0x00;
		pSpecialCode[7] = 0x00;
		pSpecialCode[8] = 0xe8;
		ulSpecialCodeLength = 9;
#endif
	}
	// �����汾
	else
	{
		
	}

	// ����������
	pSearchResultAddr = SearchSpecialCode(pNtSetSystemInformation, ulSearchLength, pSpecialCode, ulSpecialCodeLength);
	if (NULL == pSearchResultAddr)
	{
		return pMmLoadSystemImage;
	}
	// ��ȡƫ��ֵ
	lOffset = *(LONG *)((PUCHAR)pSearchResultAddr + ulSpecialCodeLength);
	// �����ַ(��ת��ַ = ��һ��ָ���ַ + ��תƫ��)
	pMmLoadSystemImage = (PVOID)(((PUCHAR)pSearchResultAddr + ulSpecialCodeLength + sizeof(LONG)) + lOffset);

	return pMmLoadSystemImage;
}


// ����������
PVOID SearchSpecialCode(PVOID pSearchBeginAddr, ULONG ulSearchLength, PUCHAR pSpecialCode, ULONG ulSpecialCodeLength)
{
	PVOID pDestAddr = NULL;
	PUCHAR pBeginAddr = (PUCHAR)pSearchBeginAddr;
	PUCHAR pEndAddr = pBeginAddr + ulSearchLength;
	PUCHAR i = NULL;
	ULONG j = 0;

	for (i = pBeginAddr; i <= pEndAddr; i++)
	{
		// ����������
		for (j = 0; j < ulSpecialCodeLength; j++)
		{
			// �жϵ�ַ�Ƿ���Ч
			if (FALSE == MmIsAddressValid((PVOID)(i + j)))
			{
				break;;
			}
			// ƥ��������
			if (*(PUCHAR)(i + j) != pSpecialCode[j])
			{
				break;
			}
		}
		// ƥ��ɹ�
		if (j >= ulSpecialCodeLength)
		{
			pDestAddr = (PVOID)i;
			break;
		}
	}

	return pDestAddr;
}