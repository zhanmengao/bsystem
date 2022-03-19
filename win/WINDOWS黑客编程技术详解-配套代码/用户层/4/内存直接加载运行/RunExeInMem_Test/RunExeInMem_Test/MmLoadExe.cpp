#include "stdafx.h"
#include "MmLoadExe.h"


void ShowError(char *lpszText)
{
	char szErr[MAX_PATH] = { 0 };
	wsprintf(szErr, "%s Error!\nError Code Is:%d\n", lpszText, GetLastError());
#ifdef _DEBUG
	MessageBox(NULL, szErr, "ERROR", MB_OK | MB_ICONERROR);
#endif
}


// �ж��Ƿ����ض�λ�����
BOOL IsExistRelocationTable(LPVOID lpBaseAddress)
{
	PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)lpBaseAddress;
	PIMAGE_NT_HEADERS pNtHeaders = (PIMAGE_NT_HEADERS)((ULONG32)pDosHeader + pDosHeader->e_lfanew);
	PIMAGE_BASE_RELOCATION pLoc = (PIMAGE_BASE_RELOCATION)((unsigned long)pDosHeader + pNtHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress);

	// �ж��Ƿ��� �ض�λ��
	if ((PVOID)pLoc == (PVOID)pDosHeader)
	{
		// �ض�λ�� Ϊ��
		return FALSE;
	}

	return TRUE;
}


// ģ��PE�����������ڴ�EXE�ļ���������
// lpData: �ڴ�EXE�ļ����ݵĻ�ַ
// dwSize: �ڴ�EXE�ļ����ڴ��С
// ����ֵ: �ڴ�EXE���ص����̵ļ��ػ�ַ
LPVOID MmRunExe(LPVOID lpData, DWORD dwSize)
{
	LPVOID lpBaseAddress = NULL;

	// ��ȡ�����С
	DWORD dwSizeOfImage = GetSizeOfImage(lpData);

	// �ڽ����п���һ���ɶ�����д����ִ�е��ڴ��
	lpBaseAddress = ::VirtualAlloc(NULL, dwSizeOfImage, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
	if (NULL == lpBaseAddress)
	{
		ShowError("VirtualAlloc");
		return NULL;
	}
	::RtlZeroMemory(lpBaseAddress, dwSizeOfImage);

	// ���ڴ�PE���ݰ�SectionAlignment��С����ӳ�䵽�����ڴ���
	if (FALSE == MmMapFile(lpData, lpBaseAddress))
	{
		ShowError("MmMapFile");
		return NULL;
	}

	// �޸�PE�ļ��ض�λ����Ϣ
	if (FALSE == DoRelocationTable(lpBaseAddress))
	{
		ShowError("DoRelocationTable");
		return NULL;
	}

	// ��дPE�ļ��������Ϣ
	if (FALSE == DoImportTable(lpBaseAddress))
	{
		ShowError("DoImportTable");
		return NULL;
	}

	//�޸�ҳ���ԡ�Ӧ�ø���ÿ��ҳ�����Ե����������Ӧ�ڴ�ҳ�����ԡ�
	//ͳһ���ó�һ������PAGE_EXECUTE_READWRITE
	DWORD dwOldProtect = 0;
	if (FALSE == ::VirtualProtect(lpBaseAddress, dwSizeOfImage, PAGE_EXECUTE_READWRITE, &dwOldProtect))
	{
		ShowError("VirtualProtect");
		return NULL;
	}

	// �޸�PE�ļ����ػ�ַIMAGE_NT_HEADERS.OptionalHeader.ImageBase
	if (FALSE == SetImageBase(lpBaseAddress))
	{
		ShowError("SetImageBase");
		return NULL;
	}

	// ��ת��PE����ڵ㴦ִ��, ������ַ��ΪPE�ļ�����ڵ�IMAGE_NT_HEADERS.OptionalHeader.AddressOfEntryPoint
	if (FALSE == CallExeEntry(lpBaseAddress))
	{
		ShowError("CallExeEntry");
		return NULL;
	}

	return lpBaseAddress;
}


// ����PE�ṹ,��ȡPE�ļ����ص��ڴ��ľ����С
// lpData: �ڴ�DLL�ļ����ݵĻ�ַ
// ����ֵ: ����PE�ļ��ṹ��IMAGE_NT_HEADERS.OptionalHeader.SizeOfImageֵ�Ĵ�С
DWORD GetSizeOfImage(LPVOID lpData)
{
	DWORD dwSizeOfImage = 0;
	PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)lpData;
	PIMAGE_NT_HEADERS pNtHeaders = (PIMAGE_NT_HEADERS)((ULONG32)pDosHeader + pDosHeader->e_lfanew);
	dwSizeOfImage = pNtHeaders->OptionalHeader.SizeOfImage;

	return dwSizeOfImage;
}


// ���ڴ�DLL���ݰ�SectionAlignment��С����ӳ�䵽�����ڴ���
// lpData: �ڴ�DLL�ļ����ݵĻ�ַ
// lpBaseAddress: �ڴ�DLL���ݰ�SectionAlignment��С����ӳ�䵽�����ڴ��е��ڴ��ַ
// ����ֵ: �ɹ�����TRUE�����򷵻�FALSE
BOOL MmMapFile(LPVOID lpData, LPVOID lpBaseAddress)
{
	PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)lpData;
	PIMAGE_NT_HEADERS pNtHeaders = (PIMAGE_NT_HEADERS)((ULONG32)pDosHeader + pDosHeader->e_lfanew);
	// ��ȡSizeOfHeaders��ֵ: ����ͷ+�ڱ�ͷ�Ĵ�С
	DWORD dwSizeOfHeaders = pNtHeaders->OptionalHeader.SizeOfHeaders;
	// ��ȡ�ڱ������
	WORD wNumberOfSections = pNtHeaders->FileHeader.NumberOfSections;
	// ��ȡ��һ���ڱ�ͷ�ĵ�ַ
	PIMAGE_SECTION_HEADER pSectionHeader = (PIMAGE_SECTION_HEADER)((DWORD)pNtHeaders + sizeof(IMAGE_NT_HEADERS));

	// ���� ����ͷ+�ڱ�ͷ�Ĵ�С
	::RtlCopyMemory(lpBaseAddress, lpData, dwSizeOfHeaders);
	// ����SectionAlignmentѭ�����ؽڱ�
	WORD i = 0;
	LPVOID lpSrcMem = NULL;
	LPVOID lpDestMem = NULL;
	DWORD dwSizeOfRawData = 0;
	for (i = 0; i < wNumberOfSections; i++)
	{
		if ((0 == pSectionHeader->VirtualAddress) ||
			(0 == pSectionHeader->SizeOfRawData))
		{
			pSectionHeader++;
			continue;
		}

		lpSrcMem = (LPVOID)((DWORD)lpData + pSectionHeader->PointerToRawData);
		lpDestMem = (LPVOID)((DWORD)lpBaseAddress + pSectionHeader->VirtualAddress);
		dwSizeOfRawData = pSectionHeader->SizeOfRawData;
		::RtlCopyMemory(lpDestMem, lpSrcMem, dwSizeOfRawData);

		pSectionHeader++;
	}

	return TRUE;
}


// ����SectionAlignment
// dwSize: ��ʾδ����ǰ�ڴ�Ĵ�С
// dwAlignment: �����Сֵ
// ����ֵ: �����ڴ����֮���ֵ
DWORD Align(DWORD dwSize, DWORD dwAlignment)
{
	DWORD dwRet = 0;
	DWORD i = 0, j = 0;
	i = dwSize / dwAlignment;
	j = dwSize % dwAlignment;
	if (0 != j)
	{
		i++;
	}

	dwRet = i * dwAlignment;

	return dwRet;
}


// �޸�PE�ļ��ض�λ����Ϣ
// lpBaseAddress: �ڴ�DLL���ݰ�SectionAlignment��С����ӳ�䵽�����ڴ��е��ڴ��ַ
// ����ֵ: �ɹ�����TRUE�����򷵻�FALSE
BOOL DoRelocationTable(LPVOID lpBaseAddress)
{
	/* �ض�λ��Ľṹ��
	// DWORD sectionAddress, DWORD size (����������Ҫ�ض�λ������)
	// ���� 1000����Ҫ����5���ض�λ���ݵĻ����ض�λ���������
	// 00 10 00 00   14 00 00 00      xxxx xxxx xxxx xxxx xxxx 0000
	// -----------   -----------      ----
	// �����ڵ�ƫ��  �ܳߴ�=8+6*2     ��Ҫ�����ĵ�ַ           ���ڶ���4�ֽ�
	// �ض�λ�������ɸ����������address �� size����0 ��ʾ����
	// ��Ҫ�����ĵ�ַ��12λ�ģ���4λ����̬�֣�intel cpu����3
	*/
	//����NewBase��0x600000,���ļ������õ�ȱʡImageBase��0x400000,������ƫ��������0x200000
	//ע���ض�λ���λ�ÿ��ܺ�Ӳ���ļ��е�ƫ�Ƶ�ַ��ͬ��Ӧ��ʹ�ü��غ�ĵ�ַ

	PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)lpBaseAddress;
	PIMAGE_NT_HEADERS pNtHeaders = (PIMAGE_NT_HEADERS)((ULONG32)pDosHeader + pDosHeader->e_lfanew);
	PIMAGE_BASE_RELOCATION pLoc = (PIMAGE_BASE_RELOCATION)((unsigned long)pDosHeader + pNtHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress);

	// �ж��Ƿ��� �ض�λ��
	if ((PVOID)pLoc == (PVOID)pDosHeader)
	{
		// �ض�λ�� Ϊ��
		return TRUE;
	}

	while ((pLoc->VirtualAddress + pLoc->SizeOfBlock) != 0) //��ʼɨ���ض�λ��
	{
		WORD *pLocData = (WORD *)((PBYTE)pLoc + sizeof(IMAGE_BASE_RELOCATION));
		//���㱾����Ҫ�������ض�λ���ַ������Ŀ
		int nNumberOfReloc = (pLoc->SizeOfBlock - sizeof(IMAGE_BASE_RELOCATION)) / sizeof(WORD);

		for (int i = 0; i < nNumberOfReloc; i++)
		{
			// ÿ��WORD����������ɡ���4λָ�����ض�λ�����ͣ�WINNT.H�е�һϵ��IMAGE_REL_BASED_xxx�������ض�λ���͵�ȡֵ��
			// ��12λ�������VirtualAddress���ƫ�ƣ�ָ���˱�������ض�λ��λ�á�
/*
			#ifdef _WIN64
			if ((DWORD)(pLocData[i] & 0x0000F000) == 0x0000A000)
			{
			// 64λdll�ض�λ��IMAGE_REL_BASED_DIR64
			// ����IA-64�Ŀ�ִ���ļ����ض�λ�ƺ�����IMAGE_REL_BASED_DIR64���͵ġ�

			ULONGLONG* pAddress = (ULONGLONG *)((PBYTE)pNewBase + pLoc->VirtualAddress + (pLocData[i] & 0x0FFF));
			ULONGLONG ullDelta = (ULONGLONG)pNewBase - m_pNTHeader->OptionalHeader.ImageBase;
			*pAddress += ullDelta;

			}
			#endif
*/
			if ((DWORD)(pLocData[i] & 0x0000F000) == 0x00003000) //����һ����Ҫ�����ĵ�ַ
			{
				// 32λdll�ض�λ��IMAGE_REL_BASED_HIGHLOW
				// ����x86�Ŀ�ִ���ļ������еĻ�ַ�ض�λ����IMAGE_REL_BASED_HIGHLOW���͵ġ�
				DWORD* pAddress = (DWORD *)((PBYTE)pDosHeader + pLoc->VirtualAddress + (pLocData[i] & 0x0FFF));
				DWORD dwDelta = (DWORD)pDosHeader - pNtHeaders->OptionalHeader.ImageBase;
				*pAddress += dwDelta;

			}
		}

		//ת�Ƶ���һ���ڽ��д���
		pLoc = (PIMAGE_BASE_RELOCATION)((PBYTE)pLoc + pLoc->SizeOfBlock);
	}

	return TRUE;
}


// ��дPE�ļ��������Ϣ
// lpBaseAddress: �ڴ�DLL���ݰ�SectionAlignment��С����ӳ�䵽�����ڴ��е��ڴ��ַ
// ����ֵ: �ɹ�����TRUE�����򷵻�FALSE
BOOL DoImportTable(LPVOID lpBaseAddress)
{
	PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)lpBaseAddress;
	PIMAGE_NT_HEADERS pNtHeaders = (PIMAGE_NT_HEADERS)((ULONG32)pDosHeader + pDosHeader->e_lfanew);
	PIMAGE_IMPORT_DESCRIPTOR pImportTable = (PIMAGE_IMPORT_DESCRIPTOR)((DWORD)pDosHeader + 
		pNtHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);

	// ѭ������DLL������е�DLL����ȡ������еĺ�����ַ
	char *lpDllName = NULL;
	HMODULE hDll = NULL;
	PIMAGE_THUNK_DATA lpImportNameArray = NULL;
	PIMAGE_IMPORT_BY_NAME lpImportByName = NULL;
	PIMAGE_THUNK_DATA lpImportFuncAddrArray = NULL;
	FARPROC lpFuncAddress = NULL;
	DWORD i = 0;

	while (TRUE)
	{
		if (0 == pImportTable->OriginalFirstThunk)
		{
			break;
		}

		// ��ȡ�������DLL�����Ʋ�����DLL
		lpDllName = (char *)((DWORD)pDosHeader + pImportTable->Name);
		hDll = ::GetModuleHandle(lpDllName);
		if (NULL == hDll)
		{
			hDll = ::LoadLibrary(lpDllName);
			if (NULL == hDll)
			{
				pImportTable++;
				continue;
			}
		}

		i = 0;
		// ��ȡOriginalFirstThunk�Լ���Ӧ�ĵ��뺯�����Ʊ��׵�ַ
		lpImportNameArray = (PIMAGE_THUNK_DATA)((DWORD)pDosHeader + pImportTable->OriginalFirstThunk);
		// ��ȡFirstThunk�Լ���Ӧ�ĵ��뺯����ַ���׵�ַ
		lpImportFuncAddrArray = (PIMAGE_THUNK_DATA)((DWORD)pDosHeader + pImportTable->FirstThunk);
		while (TRUE)
		{
			if (0 == lpImportNameArray[i].u1.AddressOfData)
			{
				break;
			}

			// ��ȡIMAGE_IMPORT_BY_NAME�ṹ
			lpImportByName = (PIMAGE_IMPORT_BY_NAME)((DWORD)pDosHeader + lpImportNameArray[i].u1.AddressOfData);

			// �жϵ�����������ŵ������Ǻ������Ƶ���
			if (0x80000000 & lpImportNameArray[i].u1.Ordinal)
			{
				// ��ŵ���
				// ��IMAGE_THUNK_DATAֵ�����λΪ1ʱ����ʾ��������ŷ�ʽ���룬��ʱ����λ��������һ���������
				lpFuncAddress = GetProcAddress(hDll, (LPCSTR)(lpImportNameArray[i].u1.Ordinal & 0x0000FFFF));
			}
			else
			{
				// ���Ƶ���
				lpFuncAddress = GetProcAddress(hDll, (LPCSTR)lpImportByName->Name);
			}
			// ע��˴��ĺ�����ַ��ĸ�ֵ��Ҫ����PE��ʽ����װ�أ���Ҫ�����ˣ�����
			lpImportFuncAddrArray[i].u1.Function = (DWORD)lpFuncAddress;                      
			i++;
		}

		pImportTable++;
	}

	return TRUE;
}


// �޸�PE�ļ����ػ�ַIMAGE_NT_HEADERS.OptionalHeader.ImageBase
// lpBaseAddress: �ڴ�EXE���ݰ�SectionAlignment��С����ӳ�䵽�����ڴ��е��ڴ��ַ
// ����ֵ: �ɹ�����TRUE�����򷵻�FALSE
BOOL SetImageBase(LPVOID lpBaseAddress)
{
	PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)lpBaseAddress;
	PIMAGE_NT_HEADERS pNtHeaders = (PIMAGE_NT_HEADERS)((ULONG32)pDosHeader + pDosHeader->e_lfanew);
	pNtHeaders->OptionalHeader.ImageBase = (ULONG32)lpBaseAddress;

	return TRUE;
}


// ��ת��PE����ڵ㴦ִ��, ������ַ��ΪPE�ļ�����ڵ�IMAGE_NT_HEADERS.OptionalHeader.AddressOfEntryPoint
// lpBaseAddress: �ڴ�PE���ݰ�SectionAlignment��С����ӳ�䵽�����ڴ��е��ڴ��ַ
// ����ֵ: �ɹ�����TRUE�����򷵻�FALSE
BOOL CallExeEntry(LPVOID lpBaseAddress)
{
	PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)lpBaseAddress;
	PIMAGE_NT_HEADERS pNtHeaders = (PIMAGE_NT_HEADERS)((ULONG32)pDosHeader + pDosHeader->e_lfanew);
	LPVOID lpExeEntry = (LPVOID)((ULONG32)pDosHeader + pNtHeaders->OptionalHeader.AddressOfEntryPoint);
	// ��ת����ڵ㴦ִ��
	__asm
	{
		mov eax, lpExeEntry
		jmp eax
	}

	return TRUE;
}