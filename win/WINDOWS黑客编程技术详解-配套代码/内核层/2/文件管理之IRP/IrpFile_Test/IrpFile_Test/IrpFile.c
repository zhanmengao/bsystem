#include "IrpFile.h"


// ���ʵ��, �����¼��ź���, ���ͷ�IRP
NTSTATUS MyCompleteRoutine(
	IN PDEVICE_OBJECT DeviceObject,
	IN PIRP pIrp,
	IN PVOID Context)
{
	*pIrp->UserIosb = pIrp->IoStatus;

	// �����¼��ź�
	if (pIrp->UserEvent)
	{
		KeSetEvent(pIrp->UserEvent, IO_NO_INCREMENT, FALSE);
	}

	// �ͷ�MDL
	if (pIrp->MdlAddress)
	{
		IoFreeMdl(pIrp->MdlAddress);
		pIrp->MdlAddress = NULL;
	}

	// �ͷ�IRP
	IoFreeIrp(pIrp);
	pIrp = NULL;

	return STATUS_MORE_PROCESSING_REQUIRED;
}


// �������ߴ��ļ�
// ZwCreateFile
NTSTATUS IrpCreateFile(
	OUT PFILE_OBJECT *ppFileObject,
	IN ACCESS_MASK DesiredAccess,
	IN PUNICODE_STRING pustrFilePath,
	OUT PIO_STATUS_BLOCK IoStatusBlock,
	IN PLARGE_INTEGER AllocationSize OPTIONAL,
	IN ULONG FileAttributes,
	IN ULONG ShareAccess,
	IN ULONG CreateDisposition,
	IN ULONG CreateOptions,
	IN PVOID EaBuffer OPTIONAL,
	IN ULONG EaLength)
{
	NTSTATUS status = STATUS_SUCCESS;
	ULONG ulFileNameMaxSize = 512;
	WCHAR wszName[100] = { 0 };
	UNICODE_STRING ustrRootPath;
	OBJECT_ATTRIBUTES objectAttributes = { 0 };
	HANDLE hRootFile = NULL;
	PFILE_OBJECT pRootFileObject = NULL, pFileObject = NULL;
	PDEVICE_OBJECT RootDeviceObject = NULL, RootRealDevice = NULL;
	PIRP pIrp = NULL;
	KEVENT kEvent = { 0 };
	ACCESS_STATE accessData = { 0 };
	AUX_ACCESS_DATA auxAccessData = { 0 };
	IO_SECURITY_CONTEXT ioSecurityContext = { 0 };
	PIO_STACK_LOCATION pIoStackLocation = NULL;

	// �򿪴��̸�Ŀ¼����ȡ���
	wcscpy(wszName, L"\\??\\A:\\");
	wszName[4] = pustrFilePath->Buffer[0];
	RtlInitUnicodeString(&ustrRootPath, wszName);
	DbgPrint("RootPath:%wZ\n", &ustrRootPath);
	InitializeObjectAttributes(&objectAttributes, &ustrRootPath, OBJ_KERNEL_HANDLE, NULL, NULL);
	status = IoCreateFile(&hRootFile, GENERIC_READ | SYNCHRONIZE,
		&objectAttributes, IoStatusBlock, NULL, FILE_ATTRIBUTE_NORMAL,
		FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
		FILE_OPEN, FILE_SYNCHRONOUS_IO_NONALERT, NULL, 0, CreateFileTypeNone, 
		NULL, IO_NO_PARAMETER_CHECKING);
	if (!NT_SUCCESS(status))
	{
		DbgPrint("IoCreateFile Error[0x%X]", status);
		return status;
	}
	
	// ��ȡ���̸�Ŀ¼�ļ�����
	status = ObReferenceObjectByHandle(hRootFile, FILE_READ_ACCESS, *IoFileObjectType, KernelMode, &pRootFileObject, NULL);
	if (!NT_SUCCESS(status))
	{
		ZwClose(hRootFile);
		DbgPrint("ObReferenceObjectByHandle Error[0x%X]\n", status);
		return status;
	}

	// ��ȡ���̸�Ŀ¼�豸����
	RootDeviceObject = pRootFileObject->Vpb->DeviceObject;
	RootRealDevice = pRootFileObject->Vpb->RealDevice;

	// �رմ��̸�Ŀ¼����Ͷ���
	ObDereferenceObject(pRootFileObject);
	ZwClose(hRootFile);

	// ����IRP
	pIrp = IoAllocateIrp(RootDeviceObject->StackSize, FALSE);
	if (NULL == pIrp)
	{
		ObDereferenceObject(pFileObject);
		DbgPrint("IoAllocateIrp Error!\n");
		return STATUS_UNSUCCESSFUL;
	}

	// �����¼�
	KeInitializeEvent(&kEvent, SynchronizationEvent, FALSE);

	// �������ļ�����
	InitializeObjectAttributes(&objectAttributes, NULL, OBJ_CASE_INSENSITIVE, NULL, NULL);
	status = ObCreateObject(KernelMode, *IoFileObjectType, &objectAttributes, KernelMode, NULL, sizeof(FILE_OBJECT), 0, 0, &pFileObject);
	if (!NT_SUCCESS(status))
	{
		DbgPrint("ObCreateObject Error[0x%X]\n", status);
		return status;
	}

	// ���ô������ļ����� FILE_OBJECT
	RtlZeroMemory(pFileObject, sizeof(FILE_OBJECT));
	pFileObject->Type = IO_TYPE_FILE;
	pFileObject->Size = sizeof(FILE_OBJECT);
	pFileObject->DeviceObject = RootRealDevice;
	pFileObject->Flags = FO_SYNCHRONOUS_IO;

	// FILE_OBJECT�е�FileName��ö�̬����, ����ObDereferenceObject�ļ������ʱ�������
	pFileObject->FileName.Buffer = (PWCHAR)ExAllocatePool(NonPagedPool, ulFileNameMaxSize);
	pFileObject->FileName.MaximumLength = (USHORT)ulFileNameMaxSize;
	pFileObject->FileName.Length = pustrFilePath->Length - 4;
	RtlZeroMemory(pFileObject->FileName.Buffer, ulFileNameMaxSize);
	RtlCopyMemory(pFileObject->FileName.Buffer, &pustrFilePath->Buffer[2], pFileObject->FileName.Length);
	DbgPrint("pFileObject->FileName:%wZ\n", &pFileObject->FileName);
	KeInitializeEvent(&pFileObject->Lock, SynchronizationEvent, FALSE);
	KeInitializeEvent(&pFileObject->Event, NotificationEvent, FALSE);

	// ����Ȩ��״̬
	RtlZeroMemory(&auxAccessData, sizeof(auxAccessData));
	status = SeCreateAccessState(&accessData, &auxAccessData, DesiredAccess, IoGetFileObjectGenericMapping());
	if (!NT_SUCCESS(status))
	{
		IoFreeIrp(pIrp);
		ObDereferenceObject(pFileObject);
		DbgPrint("SeCreateAccessState Error[0x%X]\n", status);
		return status;
	}

	// ���ð�ȫ���� IO_SECURITY_CONTEXT
	ioSecurityContext.SecurityQos = NULL;
	ioSecurityContext.AccessState = &accessData;
	ioSecurityContext.DesiredAccess = DesiredAccess;
	ioSecurityContext.FullCreateOptions = 0;

	// ����IRP
	RtlZeroMemory(IoStatusBlock, sizeof(IO_STATUS_BLOCK));
	pIrp->MdlAddress = NULL;
	pIrp->AssociatedIrp.SystemBuffer = EaBuffer;
	pIrp->Flags = IRP_CREATE_OPERATION | IRP_SYNCHRONOUS_API;
	pIrp->RequestorMode = KernelMode;
	pIrp->UserIosb = IoStatusBlock;
	pIrp->UserEvent = &kEvent;
	pIrp->PendingReturned = FALSE;
	pIrp->Cancel = FALSE;
	pIrp->CancelRoutine = NULL;
	pIrp->Tail.Overlay.Thread = PsGetCurrentThread();
	pIrp->Tail.Overlay.AuxiliaryBuffer = NULL;
	pIrp->Tail.Overlay.OriginalFileObject = pFileObject;

	// ��ȡ��һ��IRP��IO_STACK_LOCATION������
	pIoStackLocation = IoGetNextIrpStackLocation(pIrp);
	pIoStackLocation->MajorFunction = IRP_MJ_CREATE;
	pIoStackLocation->DeviceObject = RootDeviceObject;
	pIoStackLocation->FileObject = pFileObject;
	pIoStackLocation->Parameters.Create.SecurityContext = &ioSecurityContext;
	pIoStackLocation->Parameters.Create.Options = (CreateDisposition << 24) | CreateOptions;
	pIoStackLocation->Parameters.Create.FileAttributes = (USHORT)FileAttributes;
	pIoStackLocation->Parameters.Create.ShareAccess = (USHORT)ShareAccess;
	pIoStackLocation->Parameters.Create.EaLength = EaLength;

	// �������ʵ��, �Ա�֪ͨIRP�������, �ͷ���Դ
	IoSetCompletionRoutine(pIrp, MyCompleteRoutine, NULL, TRUE, TRUE, TRUE);

	// ����IRP
	status = IoCallDriver(RootDeviceObject, pIrp);
	
	// �ȴ�IRP�Ĵ���
	if (STATUS_PENDING == status)
	{
		KeWaitForSingleObject(&kEvent, Executive, KernelMode, TRUE, NULL);
	}

	// �ж�IRP������
	status = IoStatusBlock->Status;
	if (!NT_SUCCESS(status))
	{
		ObDereferenceObject(pFileObject);
		DbgPrint("IRP FAILED!\n");
		return status;
	}

	InterlockedIncrement(&pFileObject->DeviceObject->ReferenceCount);
	if (pFileObject->Vpb)
	{
		InterlockedIncrement(&pFileObject->Vpb->ReferenceCount);
	}
	
	// �����ļ�����
	*ppFileObject = pFileObject;
	
	return status;
}


// �ļ�����
// ZwQueryDirectoryFile
NTSTATUS IrpQueryDirectoryFile(
	IN PFILE_OBJECT pFileObject,
	OUT PIO_STATUS_BLOCK IoStatusBlock,
	OUT PVOID FileInformation,
	IN ULONG Length,
	IN FILE_INFORMATION_CLASS FileInformationClass,
	IN PUNICODE_STRING FileName OPTIONAL)
{
	NTSTATUS status = STATUS_SUCCESS;
	PIRP pIrp = NULL;
	KEVENT kEvent = { 0 };
	PIO_STACK_LOCATION pIoStackLocation = NULL;
	PDEVICE_OBJECT pDevObj = NULL;

	// �жϲ����Ƿ���Ч
	if ((NULL == pFileObject) ||
		(NULL == pFileObject->Vpb) ||
		(NULL == pFileObject->Vpb->DeviceObject))
	{
		return STATUS_UNSUCCESSFUL;
	}

	// ��ȡ�豸����
	pDevObj = pFileObject->Vpb->DeviceObject;

	// ����IRP
	pIrp = IoAllocateIrp(pDevObj->StackSize, FALSE);
	if (NULL == pIrp)
	{
		return STATUS_UNSUCCESSFUL;
	}

	// �����¼�
	KeInitializeEvent(&kEvent, SynchronizationEvent, FALSE);

	// ����IRP
	RtlZeroMemory(FileInformation, Length);
	pIrp->UserEvent = &kEvent;
	pIrp->UserIosb = IoStatusBlock;
	pIrp->UserBuffer = FileInformation;
	pIrp->Tail.Overlay.Thread = PsGetCurrentThread();
	pIrp->Tail.Overlay.OriginalFileObject = pFileObject;
	pIrp->Overlay.AsynchronousParameters.UserApcRoutine = NULL;

	// ��ȡ��һ��IRP��IO_STACK_LOCATION������
	pIoStackLocation = IoGetNextIrpStackLocation(pIrp);
	pIoStackLocation->MajorFunction = IRP_MJ_DIRECTORY_CONTROL;
	pIoStackLocation->MinorFunction = IRP_MN_QUERY_DIRECTORY;
	pIoStackLocation->FileObject = pFileObject;
	pIoStackLocation->Flags = SL_RESTART_SCAN;
	pIoStackLocation->Parameters.QueryDirectory.Length = Length;
	pIoStackLocation->Parameters.QueryDirectory.FileName = FileName;
	pIoStackLocation->Parameters.QueryDirectory.FileInformationClass = FileInformationClass;

	// �������ʵ��, �Ա�֪ͨIRP�������, �ͷ���Դ
	IoSetCompletionRoutine(pIrp, MyCompleteRoutine, NULL, TRUE, TRUE, TRUE);

	// ����IRP
	status = IoCallDriver(pDevObj, pIrp);

	// �ȴ�IRP�Ĵ���
	if (STATUS_PENDING == status)
	{
		KeWaitForSingleObject(&kEvent, Executive, KernelMode, FALSE, NULL);
	}

	status = IoStatusBlock->Status;
	if (!NT_SUCCESS(status))
	{
		return status;
	}
	return status;
}


// ��ȡ�ļ�����Ϣ
// ZwQueryInformationFile
NTSTATUS IrpQueryInformationFile(
	IN PFILE_OBJECT pFileObject,
	OUT PIO_STATUS_BLOCK IoStatusBlock,
	OUT PVOID FileInformation,
	IN ULONG Length,
	IN FILE_INFORMATION_CLASS FileInformationClass
	)
{
	NTSTATUS status = STATUS_SUCCESS;
	PIRP pIrp = NULL;
	KEVENT kEvent = { 0 };
	PIO_STACK_LOCATION pIoStackLocation = NULL;
	PDEVICE_OBJECT pDevObj = NULL;

	// �жϲ����Ƿ���Ч
	if ((NULL == pFileObject) ||
		(NULL == pFileObject->Vpb) ||
		(NULL == pFileObject->Vpb->DeviceObject))
	{
		return STATUS_UNSUCCESSFUL;
	}

	// ��ȡ�豸����
	pDevObj = pFileObject->Vpb->DeviceObject;

	// ����IRP
	pIrp = IoAllocateIrp(pDevObj->StackSize, FALSE);
	if (NULL == pIrp)
	{
		return STATUS_UNSUCCESSFUL;
	}

	// �����¼�
	KeInitializeEvent(&kEvent, SynchronizationEvent, FALSE);

	// ����IRP
	RtlZeroMemory(FileInformation, Length);
	pIrp->UserEvent = &kEvent;
	pIrp->UserIosb = IoStatusBlock;
	pIrp->AssociatedIrp.SystemBuffer = FileInformation;
	pIrp->RequestorMode = KernelMode;
	pIrp->Tail.Overlay.Thread = PsGetCurrentThread();
	pIrp->Tail.Overlay.OriginalFileObject = pFileObject;

	// ��ȡ��һ��IRP��IO_STACK_LOCATION������
	pIoStackLocation = IoGetNextIrpStackLocation(pIrp);
	pIoStackLocation->MajorFunction = IRP_MJ_QUERY_INFORMATION;
	pIoStackLocation->DeviceObject = pDevObj;
	pIoStackLocation->FileObject = pFileObject;
	pIoStackLocation->Parameters.QueryFile.Length = Length;
	pIoStackLocation->Parameters.QueryFile.FileInformationClass = FileInformationClass;

	// �������ʵ��, �Ա�֪ͨIRP�������, �ͷ���Դ
	IoSetCompletionRoutine(pIrp, MyCompleteRoutine, NULL, TRUE, TRUE, TRUE);

	// ����IRP
	status = IoCallDriver(pDevObj, pIrp);

	// �ȴ�IRP�Ĵ���
	if (STATUS_PENDING == status)
	{
		KeWaitForSingleObject(&kEvent, Executive, KernelMode, FALSE, NULL);
	}

	status = IoStatusBlock->Status;
	if (!NT_SUCCESS(status))
	{
		return status;
	}
	return status;
}



// �����ļ���Ϣ
// ZwSetInformationFile
NTSTATUS IrpSetInformationFile(
	IN PFILE_OBJECT pFileObject,
	OUT PIO_STATUS_BLOCK IoStatusBlock,
	IN PVOID FileInformation,
	IN ULONG Length,
	IN FILE_INFORMATION_CLASS FileInformationClass
	)
{
	NTSTATUS status = STATUS_SUCCESS;
	PIRP pIrp = NULL;
	KEVENT kEvent = { 0 };
	PIO_STACK_LOCATION pIoStackLocation = NULL;
	PDEVICE_OBJECT pDevObj = NULL;

	// �жϲ����Ƿ���Ч
	if ((NULL == pFileObject) ||
		(NULL == pFileObject->Vpb) ||
		(NULL == pFileObject->Vpb->DeviceObject))
	{
		return STATUS_UNSUCCESSFUL;
	}

	// ��ȡ�豸����
	pDevObj = pFileObject->Vpb->DeviceObject;

	// ����IRP
	pIrp = IoAllocateIrp(pDevObj->StackSize, FALSE);
	if (NULL == pIrp)
	{
		return STATUS_UNSUCCESSFUL;
	}

	// �����¼�
	KeInitializeEvent(&kEvent, SynchronizationEvent, FALSE);

	// ����IRP
	pIrp->UserEvent = &kEvent;
	pIrp->UserIosb = IoStatusBlock;
	pIrp->AssociatedIrp.SystemBuffer = FileInformation;
	pIrp->RequestorMode = KernelMode;
	pIrp->Tail.Overlay.Thread = PsGetCurrentThread();
	pIrp->Tail.Overlay.OriginalFileObject = pFileObject;

	// ��ȡ��һ��IRP��IO_STACK_LOCATION������
	pIoStackLocation = IoGetNextIrpStackLocation(pIrp);
	pIoStackLocation->MajorFunction = IRP_MJ_SET_INFORMATION;
	pIoStackLocation->DeviceObject = pDevObj;
	pIoStackLocation->FileObject = pFileObject;
	pIoStackLocation->Parameters.SetFile.Length = Length;
	pIoStackLocation->Parameters.SetFile.FileInformationClass = FileInformationClass;
	
	// �������ʵ��, �Ա�֪ͨIRP�������, �ͷ���Դ
	IoSetCompletionRoutine(pIrp, MyCompleteRoutine, NULL, TRUE, TRUE, TRUE);

	// ����IRP
	status = IoCallDriver(pDevObj, pIrp);

	// �ȴ�IRP�Ĵ���
	if (STATUS_PENDING == status)
	{
		KeWaitForSingleObject(&kEvent, Executive, KernelMode, FALSE, NULL);
	}

	status = IoStatusBlock->Status;
	if (!NT_SUCCESS(status))
	{
		return status;
	}
	return status;
}


// ���ļ�
// ZwReadFile
NTSTATUS IrpReadFile(
	IN PFILE_OBJECT pFileObject,
	OUT PIO_STATUS_BLOCK IoStatusBlock,
	OUT PVOID Buffer,
	IN ULONG Length,
	IN PLARGE_INTEGER ByteOffset OPTIONAL
	)
{
	NTSTATUS status = STATUS_SUCCESS;
	PIRP pIrp = NULL;
	KEVENT kEvent = { 0 };
	PIO_STACK_LOCATION pIoStackLocation = NULL;
	PDEVICE_OBJECT pDevObj = NULL;

	// �жϲ����Ƿ���Ч
	if ((NULL == pFileObject) ||
		(NULL == pFileObject->Vpb) ||
		(NULL == pFileObject->Vpb->DeviceObject))
	{
		return STATUS_UNSUCCESSFUL;
	}

	// ��������
	if (NULL == ByteOffset)
	{
		if (0 == (FO_SYNCHRONOUS_IO & pFileObject->Flags))
		{
			return STATUS_INVALID_PARAMETER;
		}
		ByteOffset = &pFileObject->CurrentByteOffset;
	}

	// ��ȡ�豸����
	pDevObj = pFileObject->Vpb->DeviceObject;

	// ����IRP
	pIrp = IoAllocateIrp(pDevObj->StackSize, FALSE);
	if (NULL == pIrp)
	{
		return STATUS_UNSUCCESSFUL;
	}

	// �����¼�
	KeInitializeEvent(&kEvent, SynchronizationEvent, FALSE);

	// ����IRP
	RtlZeroMemory(Buffer, Length);
	pIrp->MdlAddress = MmCreateMdl(NULL, Buffer, Length);
	if (NULL == pIrp->MdlAddress)
	{
		IoFreeIrp(pIrp);
		return STATUS_INSUFFICIENT_RESOURCES;
	}
	MmBuildMdlForNonPagedPool(pIrp->MdlAddress);

	pIrp->UserEvent = &kEvent;
	pIrp->UserIosb = IoStatusBlock;
	pIrp->Flags = IRP_READ_OPERATION;
	pIrp->RequestorMode = KernelMode;
	pIrp->Tail.Overlay.Thread = PsGetCurrentThread();
	pIrp->Tail.Overlay.OriginalFileObject = pFileObject;

	// ��ȡ��һ��IRP��IO_STACK_LOCATION������
	pIoStackLocation = IoGetNextIrpStackLocation(pIrp);
	pIoStackLocation->MajorFunction = IRP_MJ_READ;
	pIoStackLocation->MinorFunction = IRP_MN_NORMAL;
	pIoStackLocation->DeviceObject = pDevObj;
	pIoStackLocation->FileObject = pFileObject;
	pIoStackLocation->Parameters.Read.Length = Length;
	pIoStackLocation->Parameters.Read.ByteOffset = *ByteOffset;
	
	// �������ʵ��, �Ա�֪ͨIRP�������, �ͷ���Դ
	IoSetCompletionRoutine(pIrp, MyCompleteRoutine, NULL, TRUE, TRUE, TRUE);

	// ����IRP
	status = IoCallDriver(pDevObj, pIrp);

	// �ȴ�IRP�Ĵ���
	if (STATUS_PENDING == status)
	{
		KeWaitForSingleObject(&kEvent, Executive, KernelMode, FALSE, NULL);
	}

	status = IoStatusBlock->Status;
	if (!NT_SUCCESS(status))
	{
		return status;
	}
	return status;
}


// д�ļ�
// ZwWriteFile
NTSTATUS IrpWriteFile(
	IN PFILE_OBJECT pFileObject,
	OUT PIO_STATUS_BLOCK IoStatusBlock,
	IN PVOID Buffer,
	IN ULONG Length,
	IN PLARGE_INTEGER ByteOffset OPTIONAL
	)
{
	NTSTATUS status = STATUS_SUCCESS;
	PIRP pIrp = NULL;
	KEVENT kEvent = { 0 };
	PIO_STACK_LOCATION pIoStackLocation = NULL;
	PDEVICE_OBJECT pDevObj = NULL;

	// �жϲ����Ƿ���Ч
	if ((NULL == pFileObject) ||
		(NULL == pFileObject->Vpb) ||
		(NULL == pFileObject->Vpb->DeviceObject))
	{
		return STATUS_UNSUCCESSFUL;
	}

	// ��������
	if (NULL == ByteOffset)
	{
		if (0 == (FO_SYNCHRONOUS_IO & pFileObject->Flags))
		{
			return STATUS_INVALID_PARAMETER;
		}
		ByteOffset = &pFileObject->CurrentByteOffset;
	}

	// ��ȡ�豸����
	pDevObj = pFileObject->Vpb->DeviceObject;

	// ����IRP
	pIrp = IoAllocateIrp(pDevObj->StackSize, FALSE);
	if (NULL == pIrp)
	{
		return STATUS_UNSUCCESSFUL;
	}

	// �����¼�
	KeInitializeEvent(&kEvent, SynchronizationEvent, FALSE);

	// ����IRP
	pIrp->MdlAddress = MmCreateMdl(NULL, Buffer, Length);
	if (NULL == pIrp->MdlAddress)
	{
		IoFreeIrp(pIrp);
		return STATUS_INSUFFICIENT_RESOURCES;
	}
	MmBuildMdlForNonPagedPool(pIrp->MdlAddress);
	
	pIrp->UserEvent = &kEvent;
	pIrp->UserIosb = IoStatusBlock;
	pIrp->Flags = IRP_WRITE_OPERATION;
	pIrp->RequestorMode = KernelMode;
	pIrp->Tail.Overlay.Thread = PsGetCurrentThread();
	pIrp->Tail.Overlay.OriginalFileObject = pFileObject;

	// ��ȡ��һ��IRP��IO_STACK_LOCATION������
	pIoStackLocation = IoGetNextIrpStackLocation(pIrp);
	pIoStackLocation->MajorFunction = IRP_MJ_WRITE;
	pIoStackLocation->MinorFunction = IRP_MN_NORMAL;
	pIoStackLocation->DeviceObject = pDevObj;
	pIoStackLocation->FileObject = pFileObject;
	pIoStackLocation->Parameters.Write.Length = Length;
	pIoStackLocation->Parameters.Write.ByteOffset = *ByteOffset;

	// �������ʵ��, �Ա�֪ͨIRP�������, �ͷ���Դ
	IoSetCompletionRoutine(pIrp, MyCompleteRoutine, NULL, TRUE, TRUE, TRUE);

	// ����IRP
	status = IoCallDriver(pDevObj, pIrp);

	// �ȴ�IRP�Ĵ���
	if (STATUS_PENDING == status)
	{
		KeWaitForSingleObject(&kEvent, Executive, KernelMode, FALSE, NULL);
	}

	status = IoStatusBlock->Status;
	if (!NT_SUCCESS(status))
	{
		return status;
	}
	return status;
}

