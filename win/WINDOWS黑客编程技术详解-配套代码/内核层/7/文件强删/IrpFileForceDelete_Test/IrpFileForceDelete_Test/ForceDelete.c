#include "ForceDelete.h"


// ǿ��ɾ���ļ�
NTSTATUS ForceDeleteFile(UNICODE_STRING ustrFileName)
{
	NTSTATUS status = STATUS_SUCCESS;
	PFILE_OBJECT pFileObject = NULL;
	IO_STATUS_BLOCK iosb = { 0 };
	FILE_BASIC_INFORMATION fileBaseInfo = { 0 };
	FILE_DISPOSITION_INFORMATION fileDispositionInfo = { 0 };
	PVOID pImageSectionObject = NULL;
	PVOID pDataSectionObject = NULL;
	PVOID pSharedCacheMap = NULL;

	// ����IRP���ļ�
	status = IrpCreateFile(&pFileObject, GENERIC_READ | GENERIC_WRITE, &ustrFileName,
		&iosb, NULL, FILE_ATTRIBUTE_NORMAL, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
		FILE_OPEN, FILE_SYNCHRONOUS_IO_NONALERT, NULL, 0);
	if (!NT_SUCCESS(status))
	{
		DbgPrint("IrpCreateFile Error[0x%X]\n", status);
		return FALSE;
	}

	// ����IRP�����ļ�����, ȥ��ֻ������, �޸�Ϊ FILE_ATTRIBUTE_NORMAL
	RtlZeroMemory(&fileBaseInfo, sizeof(fileBaseInfo));
	fileBaseInfo.FileAttributes = FILE_ATTRIBUTE_NORMAL;
	status = IrpSetInformationFile(pFileObject, &iosb, &fileBaseInfo, sizeof(fileBaseInfo), FileBasicInformation);
	if (!NT_SUCCESS(status))
	{
		DbgPrint("IrpSetInformationFile[SetInformation] Error[0x%X]\n", status);
		return status;
	}

	// ���PSECTION_OBJECT_POINTERS�ṹ
	if (pFileObject->SectionObjectPointer)
	{
		// �����ֵ
		pImageSectionObject = pFileObject->SectionObjectPointer->ImageSectionObject;
		pDataSectionObject = pFileObject->SectionObjectPointer->DataSectionObject;
		pSharedCacheMap = pFileObject->SectionObjectPointer->SharedCacheMap;
		// ��Ϊ��
		pFileObject->SectionObjectPointer->ImageSectionObject = NULL;
		pFileObject->SectionObjectPointer->DataSectionObject = NULL;
		pFileObject->SectionObjectPointer->SharedCacheMap = NULL;
	}

	// ����IRP�����ļ�����, ����ɾ���ļ�����
	RtlZeroMemory(&fileDispositionInfo, sizeof(fileDispositionInfo));
	fileDispositionInfo.DeleteFile = TRUE;
	status = IrpSetInformationFile(pFileObject, &iosb, &fileDispositionInfo, sizeof(fileDispositionInfo), FileDispositionInformation);
	if (!NT_SUCCESS(status))
	{
		DbgPrint("IrpSetInformationFile[DeleteFile] Error[0x%X]\n", status);
		return status;
	}

	//��ԭ��ֵ  
	if (pFileObject->SectionObjectPointer)
	{
		pFileObject->SectionObjectPointer->ImageSectionObject = pImageSectionObject;
		pFileObject->SectionObjectPointer->DataSectionObject = pDataSectionObject;
		pFileObject->SectionObjectPointer->SharedCacheMap = pSharedCacheMap;
	}

	// �ر��ļ�����
	ObDereferenceObject(pFileObject);

	return status;
}



