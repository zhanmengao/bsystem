#include "FileManage.h"


VOID ShowError(CHAR *lpszText, NTSTATUS status)
{
	DbgPrint("%s Error! Error Code: 0x%08X\n", lpszText, status);
}


// �����ļ�
BOOLEAN MyCreateFile(UNICODE_STRING ustrFilePath)
{
	HANDLE hFile = NULL;
	OBJECT_ATTRIBUTES objectAttributes = { 0 };
	IO_STATUS_BLOCK iosb = { 0 };
	NTSTATUS status = STATUS_SUCCESS;
	// �����ļ�
	InitializeObjectAttributes(&objectAttributes, &ustrFilePath, OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE, NULL, NULL);
	status = ZwCreateFile(&hFile, GENERIC_READ, &objectAttributes, &iosb, NULL, FILE_ATTRIBUTE_NORMAL, 0, FILE_OPEN_IF, FILE_SYNCHRONOUS_IO_NONALERT, NULL, 0);
	if (!NT_SUCCESS(status))
	{
		ShowError("ZwCreateFile", status);
		return FALSE;
	}
	// �رվ��
	ZwClose(hFile);

	return TRUE;
}


// ����Ŀ¼
BOOLEAN MyCreateFileFolder(UNICODE_STRING ustrFileFolderPath)
{
	HANDLE hFile = NULL;
	OBJECT_ATTRIBUTES objectAttributes = { 0 };
	IO_STATUS_BLOCK iosb = { 0 };
	NTSTATUS status = STATUS_SUCCESS;
	// ����Ŀ¼
	InitializeObjectAttributes(&objectAttributes, &ustrFileFolderPath, OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE, NULL, NULL);
	status = ZwCreateFile(&hFile, GENERIC_READ, &objectAttributes, &iosb, NULL, FILE_ATTRIBUTE_NORMAL, 0, FILE_CREATE, FILE_DIRECTORY_FILE, NULL, 0);
	if (!NT_SUCCESS(status))
	{
		ShowError("ZwCreateFile", status);
		return FALSE;
	}
	// �رվ��
	ZwClose(hFile);

	return TRUE;
}


// ɾ���ļ����ǿ�Ŀ¼
BOOLEAN MyDeleteFileOrFileFolder(UNICODE_STRING ustrFileName)
{
	NTSTATUS status = STATUS_SUCCESS;
	OBJECT_ATTRIBUTES objectAttributes = { 0 };

	InitializeObjectAttributes(&objectAttributes, &ustrFileName, OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE, NULL, NULL);
	// ִ��ɾ������
	status = ZwDeleteFile(&objectAttributes);
	if (!NT_SUCCESS(status))
	{
		ShowError("ZwDeleteFile", status);
		return FALSE;
	}

	return TRUE;
}


// ��ȡ�ļ���С
ULONG64 MyGetFileSize(UNICODE_STRING ustrFileName)
{
	HANDLE hFile = NULL;
	OBJECT_ATTRIBUTES objectAttributes = { 0 };
	IO_STATUS_BLOCK iosb = { 0 };
	NTSTATUS status = STATUS_SUCCESS;
	FILE_STANDARD_INFORMATION fsi = { 0 };

	// ��ȡ�ļ����
	InitializeObjectAttributes(&objectAttributes, &ustrFileName, OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE, NULL, NULL);
	status = ZwCreateFile(&hFile, GENERIC_READ, &objectAttributes, &iosb, NULL, 0,
		FILE_SHARE_READ, FILE_OPEN, FILE_SYNCHRONOUS_IO_NONALERT, NULL, 0);
	if (!NT_SUCCESS(status))
	{
		ShowError("ZwCreateFile", status);
		return 0;
	}
	// ��ȡ�ļ���С��Ϣ
	status = ZwQueryInformationFile(hFile, &iosb, &fsi, sizeof(FILE_STANDARD_INFORMATION), FileStandardInformation);
	if (!NT_SUCCESS(status))
	{
		ZwClose(hFile);
		ShowError("ZwQueryInformationFile", status);
		return 0;
	}

	return fsi.EndOfFile.QuadPart;
}


// �������ļ����ļ���
BOOLEAN MyRenameFileOrFileFolder(UNICODE_STRING ustrSrcFileName, UNICODE_STRING ustrDestFileName)
{
	HANDLE hFile = NULL;
	OBJECT_ATTRIBUTES objectAttributes = { 0 };
	IO_STATUS_BLOCK iosb = { 0 };
	NTSTATUS status = STATUS_SUCCESS;
	PFILE_RENAME_INFORMATION pRenameInfo = NULL;
	ULONG ulLength = (1024 + sizeof(FILE_RENAME_INFORMATION));

	// �����ڴ�
	pRenameInfo = (PFILE_RENAME_INFORMATION)ExAllocatePool(NonPagedPool, ulLength);
	if (NULL == pRenameInfo)
	{
		ShowError("ExAllocatePool", 0);
		return FALSE;
	}
	// ������������Ϣ
	RtlZeroMemory(pRenameInfo, ulLength);
	pRenameInfo->FileNameLength = ustrDestFileName.Length;
	wcscpy(pRenameInfo->FileName, ustrDestFileName.Buffer);
	pRenameInfo->ReplaceIfExists = 0;
	pRenameInfo->RootDirectory = NULL;
	// ����Դ�ļ���Ϣ����ȡ���
	InitializeObjectAttributes(&objectAttributes, &ustrSrcFileName, OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE, NULL, NULL);
	status = ZwCreateFile(&hFile, SYNCHRONIZE | DELETE, &objectAttributes,
		&iosb, NULL, 0, FILE_SHARE_READ, FILE_OPEN,
		FILE_SYNCHRONOUS_IO_NONALERT | FILE_NO_INTERMEDIATE_BUFFERING, NULL, 0);
	if (!NT_SUCCESS(status))
	{
		ExFreePool(pRenameInfo);
		ShowError("ZwCreateFile", status);
		return FALSE;
	}
	// ����ZwSetInformationFile�������ļ���Ϣ
	status = ZwSetInformationFile(hFile, &iosb, pRenameInfo, ulLength, FileRenameInformation);
	if (!NT_SUCCESS(status))
	{
		ZwClose(hFile);
		ExFreePool(pRenameInfo);
		ShowError("ZwSetInformationFile", status);
		return FALSE;
	}
	// �ͷ��ڴ�, �رվ��
	ExFreePool(pRenameInfo);
	ZwClose(hFile);

	return TRUE;
}


// �����ļ��к��ļ�
BOOLEAN MyQueryFileAndFileFolder(UNICODE_STRING ustrPath)
{
	HANDLE hFile = NULL;
	OBJECT_ATTRIBUTES objectAttributes = { 0 };
	IO_STATUS_BLOCK iosb = { 0 };
	NTSTATUS status = STATUS_SUCCESS;

	// ��ȡ�ļ����
	InitializeObjectAttributes(&objectAttributes, &ustrPath, OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE, NULL, NULL);
	status = ZwCreateFile(&hFile, FILE_LIST_DIRECTORY | SYNCHRONIZE | FILE_ANY_ACCESS,
		&objectAttributes, &iosb, NULL, FILE_ATTRIBUTE_NORMAL, FILE_SHARE_READ | FILE_SHARE_WRITE,
		FILE_OPEN, FILE_DIRECTORY_FILE | FILE_SYNCHRONOUS_IO_NONALERT | FILE_OPEN_FOR_BACKUP_INTENT,
		NULL, 0);
	if (!NT_SUCCESS(status))
	{
		ShowError("ZwCreateFile", status);
		return FALSE;
	}

	// �����ļ�
	// ע��˴��Ĵ�С!!!һ��Ҫ�����㹻�ڴ棬�������ExFreePool������
	ULONG ulLength = (2 * 4096 + sizeof(FILE_BOTH_DIR_INFORMATION)) * 0x2000;              
	PFILE_BOTH_DIR_INFORMATION pDir = ExAllocatePool(PagedPool, ulLength);
	// ����pDir���׵�ַ�������ͷ��ڴ�ʹ��!!!
	PFILE_BOTH_DIR_INFORMATION pBeginAddr = pDir;                                      
	// ��ȡ��Ϣ
	status = ZwQueryDirectoryFile(hFile, NULL, NULL, NULL, &iosb, pDir, ulLength,
		FileBothDirectoryInformation, FALSE, NULL, FALSE);
	if (!NT_SUCCESS(status))
	{
		ExFreePool(pDir);
		ZwClose(hFile);
		ShowError("ZwQueryDirectoryFile", status);
		return FALSE;
	}
	// ����
	UNICODE_STRING ustrTemp;
	UNICODE_STRING ustrOne;
	UNICODE_STRING ustrTwo;
	RtlInitUnicodeString(&ustrOne, L".");
	RtlInitUnicodeString(&ustrTwo, L"..");
	WCHAR wcFileName[1024] = { 0 };
	while (TRUE)
	{
		// �ж��Ƿ����ϼ�Ŀ¼���Ǳ�Ŀ¼
		RtlZeroMemory(wcFileName, 1024);
		RtlCopyMemory(wcFileName, pDir->FileName, pDir->FileNameLength);
		RtlInitUnicodeString(&ustrTemp, wcFileName);
		if ((0 != RtlCompareUnicodeString(&ustrTemp, &ustrOne, TRUE)) &&
			(0 != RtlCompareUnicodeString(&ustrTemp, &ustrTwo, TRUE)))
		{
			if (pDir->FileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				// Ŀ¼
				DbgPrint("[DIRECTORY]\t%wZ\n", &ustrTemp);
			}
			else
			{
				// �ļ�
				DbgPrint("[FILE]\t\t%wZ\n", &ustrTemp);
			}
		}
		// �������
		if (0 == pDir->NextEntryOffset)
		{
			DbgPrint("\n[QUERY OVER]\n\n");
			break;
		}
		// pDirָ��ĵ�ַ�ı��ˣ���������ExFreePool(pDir)������������ԣ����뱣���׵�ַ
		pDir = (PFILE_BOTH_DIR_INFORMATION)((PUCHAR)pDir + pDir->NextEntryOffset);
	}
	// �ͷ��ڴ�, �ر��ļ����
	ExFreePool(pBeginAddr);
	ZwClose(hFile);

	return TRUE;
}


// ��ȡ�ļ�����
BOOLEAN MyReadFile(UNICODE_STRING ustrFileName, LARGE_INTEGER liOffset, PUCHAR pReadData, PULONG pulReadDataSize)
{
	HANDLE hFile = NULL;
	IO_STATUS_BLOCK iosb = { 0 };
	OBJECT_ATTRIBUTES objectAttributes = { 0 };
	NTSTATUS status = STATUS_SUCCESS;

	// ���ļ�
	InitializeObjectAttributes(&objectAttributes, &ustrFileName, OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE, NULL, NULL);
	status = ZwCreateFile(&hFile, GENERIC_READ, &objectAttributes, &iosb, NULL,
		FILE_ATTRIBUTE_NORMAL, FILE_SHARE_READ | FILE_SHARE_WRITE, FILE_OPEN,
		FILE_NON_DIRECTORY_FILE | FILE_SYNCHRONOUS_IO_NONALERT, NULL, 0);
	if (!NT_SUCCESS(status))
	{
		ShowError("ZwCreateFile", status);
		return FALSE;
	}
	// ��ȡ�ļ�����
	RtlZeroMemory(&iosb, sizeof(iosb));
	status = ZwReadFile(hFile, NULL, NULL, NULL, &iosb,
		pReadData, *pulReadDataSize, &liOffset, NULL);
	if (!NT_SUCCESS(status))
	{
		*pulReadDataSize = iosb.Information;
		ZwClose(hFile);
		ShowError("ZwCreateFile", status);
		return FALSE;
	}
	// ��ȡʵ�ʶ�ȡ������
	*pulReadDataSize = iosb.Information;
	// �رվ��
	ZwClose(hFile);

	return TRUE;
}


// ���ļ�д������
BOOLEAN MyWriteFile(UNICODE_STRING ustrFileName, LARGE_INTEGER liOffset, PUCHAR pWriteData, PULONG pulWriteDataSize)
{
	HANDLE hFile = NULL;
	IO_STATUS_BLOCK iosb = { 0 };
	OBJECT_ATTRIBUTES objectAttributes = { 0 };
	NTSTATUS status = STATUS_SUCCESS;

	// ���ļ�
	InitializeObjectAttributes(&objectAttributes, &ustrFileName, OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE, NULL, NULL);
	status = ZwCreateFile(&hFile, GENERIC_WRITE, &objectAttributes, &iosb, NULL,
		FILE_ATTRIBUTE_NORMAL, FILE_SHARE_READ | FILE_SHARE_WRITE, FILE_OPEN_IF,
		FILE_NON_DIRECTORY_FILE | FILE_SYNCHRONOUS_IO_NONALERT, NULL, 0);
	if (!NT_SUCCESS(status))
	{
		ShowError("ZwCreateFile", status);
		return FALSE;
	}
	// ��ȡ�ļ�����
	RtlZeroMemory(&iosb, sizeof(iosb));
	status = ZwWriteFile(hFile, NULL, NULL, NULL, &iosb,
		pWriteData, *pulWriteDataSize, &liOffset, NULL);
	if (!NT_SUCCESS(status))
	{
		*pulWriteDataSize = iosb.Information;
		ZwClose(hFile);
		ShowError("ZwCreateFile", status);
		return FALSE;
	}
	// ��ȡʵ��д�������
	*pulWriteDataSize = iosb.Information;
	// �رվ��
	ZwClose(hFile);

	return TRUE;
}


// �ļ�����
BOOLEAN MyCopyFile(UNICODE_STRING ustrScrFile, UNICODE_STRING ustrDestFile)
{
	ULONG ulBufferSize = 40960;
	ULONG ulReadDataSize = ulBufferSize;
	LARGE_INTEGER liOffset = { 0 };
	PUCHAR pBuffer = ExAllocatePool(NonPagedPool, ulBufferSize);
	
	// һ�߶�ȡ, һ��д��, ʵ���ļ�����
	do
	{
		// ��ȡ�ļ�
		ulReadDataSize = ulBufferSize;
		MyReadFile(ustrScrFile, liOffset, pBuffer, &ulReadDataSize);
		// ����ȡ������Ϊ�յ�ʱ��, �������Ʋ���
		if (0 >= ulReadDataSize)
		{
			break;
		}

		// д���ļ�
		MyWriteFile(ustrDestFile, liOffset, pBuffer, &ulReadDataSize);
		
		// ����ƫ��
		liOffset.QuadPart = liOffset.QuadPart + ulReadDataSize;

	} while (TRUE);

	// �ͷ��ڴ�
	ExFreePool(pBuffer);

	return TRUE;
}