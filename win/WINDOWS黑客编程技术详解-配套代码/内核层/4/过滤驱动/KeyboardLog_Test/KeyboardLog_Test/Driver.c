#include "Driver.h"


NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObject, PUNICODE_STRING pRegPath)
{
	DbgPrint("Enter DriverEntry\n");
	NTSTATUS status = STATUS_SUCCESS;
	ULONG i = 0;

	for (i = 0; i < IRP_MJ_MAXIMUM_FUNCTION; i++)
	{
		pDriverObject->MajorFunction[i] = DriverDefaultHandle;
	}

	pDriverObject->DriverUnload = DriverUnload;
	pDriverObject->MajorFunction[IRP_MJ_CREATE] = DriverDefaultHandle;
	pDriverObject->MajorFunction[IRP_MJ_CLOSE] = DriverDefaultHandle;
	pDriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = DriverControlHandle;

	pDriverObject->MajorFunction[IRP_MJ_READ] = DriverRead;
	pDriverObject->MajorFunction[IRP_MJ_POWER] = DriverPower;

	// ���������豸
	status = CreateDevice(pDriverObject);
	if (!NT_SUCCESS(status))
	{
		DbgPrint("CreateDevice Error[0x%X]\n", status);
		return status;
	}

	// �����豸�������豸ջ��
	status = AttachKdbClass(pDriverObject->DeviceObject);
	if (!NT_SUCCESS(status))
	{
		DbgPrint("AttachKdbClass Error[0x%X]\n", status);
		return status;
	}

	DbgPrint("Leave DriverEntry\n");
	return status;
}


VOID DriverUnload(PDRIVER_OBJECT pDriverObject)
{
	DbgPrint("Enter DriverUnload\n");
	PDEVICE_OBJECT pDevObj = pDriverObject->DeviceObject;
	LARGE_INTEGER liDelay = { 0 };

	if (NULL == pDevObj)
	{
		return;
	}
	if (NULL == pDevObj->DeviceExtension)
	{
		return;
	}

	// �Ȱѹ��������豸�ͼ����豸����, ��������µ�IRP_MJ_READ
	IoDetachDevice(((PDEVICE_EXTENSION)pDevObj->DeviceExtension)->pAttachDevObj);

	// ����Ϊ��ɵ�IRP����Ϊֻǰͨ��IoSetCompletionRoutine�Ѿ�����IO�������
	// ��ô����δ��ɵ�IRP �������֮������ �ò��豸�ĺ���
	// ��Ҫ�ֶ�����, ��pending״̬��IRP��ɷ���
	liDelay.QuadPart = -1000000;
	while (0 < ((PDEVICE_EXTENSION)pDevObj->DeviceExtension)->ulIrpInQuene)
	{
		KdPrint(("ʣ�����IRP:%d\n", ((PDEVICE_EXTENSION)pDevObj->DeviceExtension)->ulIrpInQuene));
		KeDelayExecutionThread(KernelMode, FALSE, &liDelay);
		/*
		// �÷�����Win10�ϲ�����
		if (1 == ((PDEVICE_EXTENSION)pDevObj->DeviceExtension)->ulIrpInQuene)
		{
			//ȡ�������һ��IRP
			IoCancelIrp(((PDEVICE_EXTENSION)pDevObj->DeviceExtension)->pLastIrp);
		}
		*/
	}

	UNICODE_STRING ustrSymName;
	RtlInitUnicodeString(&ustrSymName, SYM_NAME);
	IoDeleteSymbolicLink(&ustrSymName);

	if (pDriverObject->DeviceObject)
	{
		IoDeleteDevice(pDriverObject->DeviceObject);
	}

	DbgPrint("Leave DriverUnload\n");
}


NTSTATUS DriverDefaultHandle(PDEVICE_OBJECT pDevObj, PIRP pIrp)
{
	DbgPrint("Enter DriverDefaultHandle\n");
	
	NTSTATUS status = STATUS_SUCCESS;
	
	pIrp->IoStatus.Status = status;
	pIrp->IoStatus.Information = 0;
	IoCompleteRequest(pIrp, IO_NO_INCREMENT);

	DbgPrint("Leave DriverDefaultHandle\n");
	return status;
}


NTSTATUS DriverControlHandle(PDEVICE_OBJECT pDevObj, PIRP pIrp)
{
	DbgPrint("Enter DriverControlHandle\n");
	NTSTATUS status = STATUS_SUCCESS;
	PIO_STACK_LOCATION pIoStackLocation = IoGetCurrentIrpStackLocation(pIrp);
	ULONG ulInputLength = pIoStackLocation->Parameters.DeviceIoControl.InputBufferLength;
	ULONG ulOutputLength = pIoStackLocation->Parameters.DeviceIoControl.OutputBufferLength;
	ULONG ulControlCode = pIoStackLocation->Parameters.DeviceIoControl.IoControlCode;
	ULONG ulInfo = 0;
	PVOID pBuf = pIrp->AssociatedIrp.SystemBuffer;

	switch (ulControlCode)
	{
	case IOCTL_TEST:
	{
		break;
	}
	default:
		break;
	}

	pIrp->IoStatus.Status = status;
	pIrp->IoStatus.Information = ulInfo;
	IoCompleteRequest(pIrp, IO_NO_INCREMENT);

	DbgPrint("Enter DriverControlHandle\n");
	return status;
}


NTSTATUS DriverPower(PDEVICE_OBJECT pDevObj, PIRP pIrp)
{
	DbgPrint("Enter DriverPower\n");
	NTSTATUS status = STATUS_SUCCESS;

	PoStartNextPowerIrp(pIrp);
	IoSkipCurrentIrpStackLocation(pIrp);
	status = PoCallDriver(((PDEVICE_EXTENSION)pDevObj->DeviceExtension)->pAttachDevObj, pIrp);

	DbgPrint("Leave DriverPower\n");
	return status;
}


NTSTATUS DriverRead(PDEVICE_OBJECT pDevObj, PIRP pIrp)
{
	DbgPrint("\nEnter DriverRead\n");
	NTSTATUS status = STATUS_SUCCESS;

	// ���� pIrp �� IO_STACK_LOCATION ����һ�豸
	IoCopyCurrentIrpStackLocationToNext(pIrp);

	// �������ʵ��
	IoSetCompletionRoutine(pIrp, ReadCompleteRoutine, pDevObj, TRUE, TRUE, TRUE);

	// ��¼IRP����
	((PDEVICE_EXTENSION)pDevObj->DeviceExtension)->ulIrpInQuene++;

	// ����IRP����һ�豸��
	status = IoCallDriver(((PDEVICE_EXTENSION)pDevObj->DeviceExtension)->pAttachDevObj, pIrp);

	DbgPrint("Leave DriverRead\n");
	return status;
}


NTSTATUS ReadCompleteRoutine(PDEVICE_OBJECT pDevObj, PIRP pIrp, PVOID pContext)
{
	NTSTATUS status = pIrp->IoStatus.Status;
	PKEYBOARD_INPUT_DATA pKeyboardInputData = NULL;
	ULONG ulKeyCount = 0, i = 0;

	if (NT_SUCCESS(status))
	{
		pKeyboardInputData = (PKEYBOARD_INPUT_DATA)pIrp->AssociatedIrp.SystemBuffer;
		ulKeyCount = (ULONG)pIrp->IoStatus.Information / sizeof(KEYBOARD_INPUT_DATA);

		// ��ȡ��������
		for (i = 0; i < ulKeyCount; i++)
		{
			// Key Press
			if (KEY_MAKE == pKeyboardInputData[i].Flags)
			{
				// ����ɨ����
				DbgPrint("[Down][0x%X]\n", pKeyboardInputData[i].MakeCode);
			}
			// Key Release
			else if (KEY_BREAK == pKeyboardInputData[i].Flags)
			{
				// ����ɨ����
				DbgPrint("[Up][0x%X]\n", pKeyboardInputData[i].MakeCode);
			}
			//�������������һ�䣬Ȼ�󰴼�ȫ������Ϊ�� ���� A
//			pKeyboardInputData[i].MakeCode = 0x1e;
		}
	}

	if (pIrp->PendingReturned)
	{
		IoMarkIrpPending(pIrp);
	}

	// ����IRP�ڶ��е�����
	((PDEVICE_EXTENSION)pDevObj->DeviceExtension)->ulIrpInQuene--;

	status = pIrp->IoStatus.Status;
	return status;
}


NTSTATUS CreateDevice(PDRIVER_OBJECT pDriverObject)
{
	DbgPrint("Enter CreateDevice\n");
	NTSTATUS status = STATUS_SUCCESS;
	UNICODE_STRING ustrDevName, ustrSymName;
	PDEVICE_OBJECT pDevObj = NULL;

	RtlInitUnicodeString(&ustrDevName, DEV_NAME);
	RtlInitUnicodeString(&ustrSymName, SYM_NAME);

	// ���������豸
	status = IoCreateDevice(pDriverObject, sizeof(DEVICE_EXTENSION), &ustrDevName, FILE_DEVICE_KEYBOARD, 0, FALSE, &pDevObj);
	if (!NT_SUCCESS(status))
	{
		DbgPrint("IoCreateDevice Error[0x%X]\n", status);
		return status;
	}

	// ������������
	status = IoCreateSymbolicLink(&ustrSymName, &ustrDevName);
	if (!NT_SUCCESS(status))
	{
		DbgPrint("IoCreateSymbolicLink Error[0x%X]\n", status);
		return status;
	}

	DbgPrint("Enter CreateDevice\n");
	return status;
}


NTSTATUS AttachKdbClass(PDEVICE_OBJECT pDevObj)
{
	DbgPrint("Enter HookKdbClass\n");
	NTSTATUS status = STATUS_SUCCESS;
	UNICODE_STRING ustrObjectName;
	PFILE_OBJECT pFileObj = NULL;
	PDEVICE_OBJECT pKeyboardClassDeviceObject = NULL, pAttachDevObj = NULL;

	RtlInitUnicodeString(&ustrObjectName, L"\\Device\\KeyboardClass0");

	// ��ȡ�����豸�����ָ��
	status = IoGetDeviceObjectPointer(&ustrObjectName, GENERIC_READ | GENERIC_WRITE, &pFileObj, &pKeyboardClassDeviceObject);
	if (!NT_SUCCESS(status))
	{
		DbgPrint("IoGetDeviceObjectPointer Error[0x%X]\n", status);
		return status;
	}
	// ��������
	ObReferenceObject(pFileObj);

	// ����ǰ�豸���ӵ������豸���豸ջ����, ���ص���ԭ���豸ջ�ϵ�ջ���豸, ����ǰ��ջ�ϸ����豸����һ���豸
	pAttachDevObj = IoAttachDeviceToDeviceStack(pDevObj, pKeyboardClassDeviceObject);
	if (NULL == pAttachDevObj)
	{
		DbgPrint("IoAttachDeviceToDeviceStack Error\n");
		return STATUS_UNSUCCESSFUL;
	}

	// ���ô��豸�ı�־��Ҫ�븽�ӵ����豸ջ�ϵ��豸��־����һ��
	pDevObj->Flags = pDevObj->Flags | DO_BUFFERED_IO | DO_POWER_PAGABLE;
	pDevObj->ActiveThreadCount = pDevObj->ActiveThreadCount & (~DO_DEVICE_INITIALIZING);


	// ������һ�豸��DeviceExtension
	((PDEVICE_EXTENSION)pDevObj->DeviceExtension)->pAttachDevObj = pAttachDevObj;
	((PDEVICE_EXTENSION)pDevObj->DeviceExtension)->ulIrpInQuene = 0;

	DbgPrint("Leave HookKdbClass\n");
	return status;
}







