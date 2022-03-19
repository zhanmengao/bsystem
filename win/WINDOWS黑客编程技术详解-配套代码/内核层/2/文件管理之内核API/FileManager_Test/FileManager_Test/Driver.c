#include "FileManage.h"
#include "Driver.h"


NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObject, PUNICODE_STRING pRegPath)
{
	DbgPrint("Enter DriverEntry\n");

	NTSTATUS status = STATUS_SUCCESS;
	pDriverObject->DriverUnload = DriverUnload;
	for (ULONG i = 0; i < IRP_MJ_MAXIMUM_FUNCTION; i++)
	{
		pDriverObject->MajorFunction[i] = DriverDefaultHandle;
	}

	// ����Ŀ¼
	UNICODE_STRING ustrDirectory;
	RtlInitUnicodeString(&ustrDirectory, L"\\??\\C:\\MyCreateFolder");
	MyCreateFileFolder(ustrDirectory);

	// ����Ŀ¼
	UNICODE_STRING ustrCreateFile;
	RtlInitUnicodeString(&ustrCreateFile, L"\\??\\C:\\MyCreateFolder\\MyCreateFile.txt");
	MyCreateFile(ustrCreateFile);

	// ɾ���ļ�
	UNICODE_STRING ustrDeleteFile;
	RtlInitUnicodeString(&ustrDeleteFile, L"\\??\\C:\\MyCreateFolder\\520\\520.exe");
	MyDeleteFileOrFileFolder(ustrDeleteFile);

	// ɾ����Ŀ¼
	UNICODE_STRING ustrDeleteFilder;
	RtlInitUnicodeString(&ustrDeleteFilder, L"\\??\\C:\\MyCreateFolder\\520");
	MyDeleteFileOrFileFolder(ustrDeleteFilder);

	// ��ȡ�ļ���С
	UNICODE_STRING ustrFileSize;
	RtlInitUnicodeString(&ustrFileSize, L"\\??\\C:\\MyCreateFolder\\520.exe");
	ULONG64 ullFileSize = MyGetFileSize(ustrFileSize);
	DbgPrint("FileSize = %I64d Bytes\n", ullFileSize);

	// ������Ŀ¼
	UNICODE_STRING ustrOldFilder, ustrNewFilder;
	RtlInitUnicodeString(&ustrOldFilder, L"\\??\\C:\\MyCreateFolder\\520");
	RtlInitUnicodeString(&ustrNewFilder, L"\\??\\C:\\MyCreateFolder\\5222220");
	MyRenameFileOrFileFolder(ustrOldFilder, ustrNewFilder);

	// �������ļ�
	UNICODE_STRING ustrOldFile, ustrNewFile;
	RtlInitUnicodeString(&ustrOldFile, L"\\??\\C:\\MyCreateFolder\\MyCreateFile.txt");
	RtlInitUnicodeString(&ustrNewFile, L"\\??\\C:\\MyCreateFolder\\MyNNNNCreateFile.txt");
	MyRenameFileOrFileFolder(ustrOldFile, ustrNewFile);

	// �����ļ��к��ļ�
	UNICODE_STRING ustrQueryFile;
	RtlInitUnicodeString(&ustrQueryFile, L"\\??\\C:\\MyCreateFolder");
	MyQueryFileAndFileFolder(ustrQueryFile);

	// �ļ�����
	UNICODE_STRING ustrScrFile, ustrDestFile;
	RtlInitUnicodeString(&ustrScrFile, L"\\??\\C:\\MyCreateFolder\\1.exe");
	RtlInitUnicodeString(&ustrDestFile, L"\\??\\C:\\MyCreateFolder\\2.exe");
	MyCopyFile(ustrScrFile, ustrDestFile);

	DbgPrint("Leave DriverEntry\n");
	return status;
}



VOID DriverUnload(PDRIVER_OBJECT pDriverObject)
{
}


NTSTATUS DriverDefaultHandle(PDEVICE_OBJECT pDevObj, PIRP pIrp)
{
	NTSTATUS status = STATUS_SUCCESS;
	pIrp->IoStatus.Status = status;
	pIrp->IoStatus.Information = 0;
	IoCompleteRequest(pIrp, IO_NO_INCREMENT);

	return status;
}