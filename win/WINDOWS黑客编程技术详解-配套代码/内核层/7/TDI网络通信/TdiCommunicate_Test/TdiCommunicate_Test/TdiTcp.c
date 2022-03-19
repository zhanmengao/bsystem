#include "TdiTcp.h"


VOID ShowError(PCHAR lpszText, NTSTATUS ntStatus)
{
	KdPrint(("%s Error!\nError Code Is: 0x%8X\n", lpszText, ntStatus));
}


// ��ɻص�����
NTSTATUS TdiCompletionRoutine(PDEVICE_OBJECT pDevObj, PIRP pIrp, PVOID pContext)
{
	DbgPrint("Enter TDICompletionRoutine\n");

	if (NULL != pContext)
	{
		KeSetEvent((PKEVENT)pContext, IO_NO_INCREMENT, FALSE);
	}

	DbgPrint("Leave TDICompletionRoutine\n");
	return STATUS_MORE_PROCESSING_REQUIRED;
}


// TDI��ʼ������
NTSTATUS TdiOpen(PDEVICE_OBJECT *ppTdiAddressDevObj, PFILE_OBJECT *ppTdiEndPointFileObject, HANDLE *phTdiAddress, HANDLE *phTdiEndPoint)
{
	DbgPrint("Enter OpenTdi\n");

	NTSTATUS status = STATUS_UNSUCCESSFUL;
	PFILE_FULL_EA_INFORMATION pAddressEaBuffer = NULL;
	ULONG ulAddressEaBufferLength = 0;
	PTA_IP_ADDRESS pTaIpAddr = NULL;
	UNICODE_STRING ustrTDIDevName;
	OBJECT_ATTRIBUTES ObjectAttributes = { 0 };
	IO_STATUS_BLOCK iosb = { 0 };
	HANDLE hTdiAddress = NULL;
	PFILE_OBJECT pTdiAddressFileObject = NULL;
	PDEVICE_OBJECT pTdiAddressDevObj = NULL;
	PFILE_FULL_EA_INFORMATION pContextEaBuffer = NULL;
	ULONG ulContextEaBufferLength = 0;
	HANDLE hTdiEndPoint = NULL;
	PFILE_OBJECT pTdiEndPointFileObject = NULL;
	KEVENT irpCompleteEvent = { 0 };
	PIRP pIrp = NULL;	

	do
	{
		// Ϊ���ص�ַ��չ���Խṹ�����ڴ漰��ʼ��
		ulAddressEaBufferLength = sizeof(FILE_FULL_EA_INFORMATION) + TDI_TRANSPORT_ADDRESS_LENGTH + sizeof(TA_IP_ADDRESS);
		pAddressEaBuffer = (PFILE_FULL_EA_INFORMATION)ExAllocatePool(NonPagedPool, ulAddressEaBufferLength);
		if (NULL == pAddressEaBuffer)
		{
			ShowError("ExAllocatePool[Address]", 0);
			break;
		}
		RtlZeroMemory(pAddressEaBuffer, ulAddressEaBufferLength);
		RtlCopyMemory(pAddressEaBuffer->EaName, TdiTransportAddress, (1 + TDI_TRANSPORT_ADDRESS_LENGTH)); 
		pAddressEaBuffer->EaNameLength = TDI_TRANSPORT_ADDRESS_LENGTH;
		pAddressEaBuffer->EaValueLength = sizeof(TA_IP_ADDRESS);
		// ��ʼ������IP��ַ��˿�
		pTaIpAddr = (PTA_IP_ADDRESS)((PUCHAR)pAddressEaBuffer->EaName + pAddressEaBuffer->EaNameLength + 1);
		pTaIpAddr->TAAddressCount = 1;
		pTaIpAddr->Address[0].AddressLength = TDI_ADDRESS_LENGTH_IP;
		pTaIpAddr->Address[0].AddressType = TDI_ADDRESS_TYPE_IP;
		pTaIpAddr->Address[0].Address[0].sin_port = 0;     // 0��ʾ������������˿�
		pTaIpAddr->Address[0].Address[0].in_addr = 0;      // 0��ʾ��������IP��ַ
		RtlZeroMemory(pTaIpAddr->Address[0].Address[0].sin_zero, sizeof(pTaIpAddr->Address[0].Address[0].sin_zero));
		// ����TDI�����豸�ַ������ʼ���豸����
		RtlInitUnicodeString(&ustrTDIDevName, COMM_TCP_DEV_NAME);
		InitializeObjectAttributes(&ObjectAttributes, &ustrTDIDevName, OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE, NULL, NULL);
		// ���ݱ��ص�ַ��չ���Խṹ�������ص�ַ����
		status = ZwCreateFile(&hTdiAddress, GENERIC_READ | GENERIC_WRITE | SYNCHRONIZE,
			&ObjectAttributes, &iosb, NULL, FILE_ATTRIBUTE_NORMAL,
			FILE_SHARE_READ, FILE_OPEN, 0, pAddressEaBuffer, ulAddressEaBufferLength);
		if (!NT_SUCCESS(status))
		{
			ShowError("ZwCreateFile[Address]", status);
			break;
		}
		// ���ݱ��ص�ַ��������ȡ��Ӧ�ı��ص�ַ�ļ�����
		status = ObReferenceObjectByHandle(hTdiAddress,
			FILE_ANY_ACCESS, 0, KernelMode, &pTdiAddressFileObject, NULL);
		if (!NT_SUCCESS(status))
		{
			ShowError("ObReferenceObjectHandle[EndPoint]", status);
			break;
		}
		
		// ��ȡ���ص�ַ�ļ������Ӧ�������豸
		pTdiAddressDevObj = IoGetRelatedDeviceObject(pTdiAddressFileObject);
		if (NULL == pTdiAddressDevObj)
		{
			ShowError("IoGetRelatedDeviceObject", 0);
			break;
		}

		// Ϊ��������չ���������ڴ沢��ʼ��
		ulContextEaBufferLength = FIELD_OFFSET(FILE_FULL_EA_INFORMATION, EaName) + TDI_CONNECTION_CONTEXT_LENGTH + 1 + sizeof(CONNECTION_CONTEXT);
		pContextEaBuffer = (PFILE_FULL_EA_INFORMATION)ExAllocatePool(NonPagedPool, ulContextEaBufferLength);
		if (NULL == pContextEaBuffer)
		{
			ShowError("ExAllocatePool[EndPoint]", 0);
			break;
		}
		RtlZeroMemory(pContextEaBuffer, ulContextEaBufferLength);
		RtlCopyMemory(pContextEaBuffer->EaName, TdiConnectionContext, (1 + TDI_CONNECTION_CONTEXT_LENGTH));
		pContextEaBuffer->EaNameLength = TDI_CONNECTION_CONTEXT_LENGTH;
		pContextEaBuffer->EaValueLength = sizeof(CONNECTION_CONTEXT);
		// ���������Ĵ���TDI�˵����
		status = ZwCreateFile(&hTdiEndPoint, GENERIC_READ | GENERIC_WRITE | SYNCHRONIZE,
			&ObjectAttributes, &iosb, NULL, FILE_ATTRIBUTE_NORMAL, FILE_SHARE_READ,
			FILE_OPEN, 0, pContextEaBuffer, ulContextEaBufferLength);
		if (!NT_SUCCESS(status))
		{
			ShowError("ZwCreateFile[EndPoint]", status);
			break;
		}
		// ����TDI�˵��������ȡ��Ӧ�Ķ˵��ļ�����
		status = ObReferenceObjectByHandle(hTdiEndPoint,
			FILE_ANY_ACCESS, 0, KernelMode, &pTdiEndPointFileObject, NULL);
		if (!NT_SUCCESS(status))
		{
			ShowError("ObReferenceObjectHandle[EndPoint]", status);
			break;
		}

		// �����¼�
		KeInitializeEvent(&irpCompleteEvent, NotificationEvent, FALSE);

		// ��TDI�˵��뱾�ص�ַ�������, ����TDI��I/O�����:TDI_ASSOCIATE_ADDRESS
		pIrp = TdiBuildInternalDeviceControlIrp(TDI_ASSOCIATE_ADDRESS,
			pTdiAddressDevObj, pTdiEndPointFileObject, &irpCompleteEvent, &iosb);
		if (NULL == pIrp)
		{
			ShowError("TdiBuildInternalDeviceControlIrp", 0);
			break;
		}
		// ��չI/O�����
		TdiBuildAssociateAddress(pIrp, pTdiAddressDevObj, pTdiEndPointFileObject, NULL, NULL, hTdiAddress);

		// �������ʵ���Ļص�����
		IoSetCompletionRoutine(pIrp, TdiCompletionRoutine, &irpCompleteEvent, TRUE, TRUE, TRUE);

		// ����I/O��������ȴ�ִ��
		status = IoCallDriver(pTdiAddressDevObj, pIrp);
		if (STATUS_PENDING == status)
		{
			KeWaitForSingleObject(&irpCompleteEvent, Executive, KernelMode, FALSE, NULL);
		}

		// ��������
		*ppTdiAddressDevObj = pTdiAddressDevObj;
		*ppTdiEndPointFileObject = pTdiEndPointFileObject;
		*phTdiAddress = hTdiAddress;
		*phTdiEndPoint = hTdiEndPoint;

	}while (FALSE);

	// �ͷ��ڴ�
	if (pTdiAddressFileObject)
	{
        ObDereferenceObject(pTdiAddressFileObject);    // ����
	}
	if (pContextEaBuffer)
	{
		ExFreePool(pContextEaBuffer);
	}
	if (pAddressEaBuffer)
	{
		ExFreePool(pAddressEaBuffer);
	}

	DbgPrint("Leave OpenTdi\n");
	return status;
}


// TDI TCP���ӷ�����
NTSTATUS TdiConnection(PDEVICE_OBJECT pTdiAddressDevObj, PFILE_OBJECT pTdiEndPointFileObject, LONG *pServerIp, LONG lServerPort)
{
	DbgPrint("Enter TdiConnection\n");

	NTSTATUS status = STATUS_SUCCESS;
	IO_STATUS_BLOCK iosb = { 0 };
	PIRP pIrp = NULL;
	KEVENT connEvent = { 0 };
	TA_IP_ADDRESS serverTaIpAddr = { 0 };
	ULONG serverIpAddr = 0;
	USHORT serverPort = 0;
	TDI_CONNECTION_INFORMATION serverConnection = { 0 };

	// ���������¼�
	KeInitializeEvent(&connEvent, NotificationEvent, FALSE);

	// ����TDI����I/O�����:TDI_CONNECT
	pIrp = TdiBuildInternalDeviceControlIrp(TDI_CONNECT, pTdiAddressDevObj, pTdiEndPointFileObject, &connEvent, &iosb);
	if (NULL == pIrp)
	{
		ShowError("TdiBuildInternalDeviceControlIrp_TDI_CONNECT", 0);
		return STATUS_INSUFFICIENT_RESOURCES;
	}

	// ��ʼ��������IP��ַ��˿�
	serverIpAddr = INETADDR(pServerIp[0], pServerIp[1], pServerIp[2], pServerIp[3]);
	serverPort = HTONS(lServerPort);
	serverTaIpAddr.TAAddressCount = 1;
	serverTaIpAddr.Address[0].AddressLength = TDI_ADDRESS_LENGTH_IP;
	serverTaIpAddr.Address[0].AddressType = TDI_ADDRESS_TYPE_IP;
	serverTaIpAddr.Address[0].Address[0].sin_port = serverPort;
	serverTaIpAddr.Address[0].Address[0].in_addr = serverIpAddr;
	serverConnection.UserDataLength = 0;
	serverConnection.UserData = 0;
	serverConnection.OptionsLength = 0;
	serverConnection.Options = 0;
	serverConnection.RemoteAddressLength = sizeof(TA_IP_ADDRESS);
	serverConnection.RemoteAddress = &serverTaIpAddr;

	// �������ĵ�ַ��˿���Ϣ���ӵ�I/O�������,����������Ϣ
	TdiBuildConnect(pIrp, pTdiAddressDevObj, pTdiEndPointFileObject, NULL, NULL, NULL, &serverConnection, 0);

	// �������ʵ���ص�����
	IoSetCompletionRoutine(pIrp, TdiCompletionRoutine, &connEvent, TRUE, TRUE, TRUE);

	// ����I/O��������ȴ�ִ��
	status = IoCallDriver(pTdiAddressDevObj, pIrp);
	if (STATUS_PENDING == status)
	{
		KeWaitForSingleObject(&connEvent, Executive, KernelMode, FALSE, NULL);
	}

	DbgPrint("Leave TdiConnection\n");
	return status;
}


// TDI TCP������Ϣ
NTSTATUS TdiSend(PDEVICE_OBJECT pTdiAddressDevObj, PFILE_OBJECT pTdiEndPointFileObject, PUCHAR pSendData, ULONG ulSendDataLength)
{
	DbgPrint("Enter TdiSend\n");

	NTSTATUS status = STATUS_SUCCESS;
	KEVENT sendEvent;
	PIRP pIrp = NULL;
	IO_STATUS_BLOCK iosb = {0};
	PMDL pSendMdl = NULL;

	// ��ʼ���¼�
	KeInitializeEvent(&sendEvent, NotificationEvent, FALSE);

	// ����I/O�����:TDI_SEND
	pIrp = TdiBuildInternalDeviceControlIrp(TDI_SEND, pTdiAddressDevObj, pTdiEndPointFileObject, &sendEvent, &iosb);
	if (NULL == pIrp)
	{
		ShowError("TdiBuildInternalDeviceControlIrp", 0);
		return STATUS_INSUFFICIENT_RESOURCES;
	}

	// ����MDL
	pSendMdl = IoAllocateMdl(pSendData, ulSendDataLength, FALSE, FALSE, pIrp);
	if (NULL == pSendMdl)
	{
		ShowError("IoAllocateMdl", 0);
		return STATUS_INSUFFICIENT_RESOURCES;
	}
	MmProbeAndLockPages(pSendMdl, KernelMode, IoModifyAccess);

	// ��չI/O�����,��ӷ�����Ϣ
	TdiBuildSend(pIrp, pTdiAddressDevObj, pTdiEndPointFileObject, NULL, NULL, pSendMdl, 0, ulSendDataLength);

	// �������ʵ���ص�����
	IoSetCompletionRoutine(pIrp, TdiCompletionRoutine, &sendEvent, TRUE, TRUE, TRUE);

	// ����I/O��������ȴ�ִ��
	status = IoCallDriver(pTdiAddressDevObj, pIrp);
	if (STATUS_PENDING == status)
	{
		KeWaitForSingleObject(&sendEvent, Executive, KernelMode, FALSE, NULL);
	}

	// �ͷ�MDL
	if (pSendMdl)
	{
		IoFreeMdl(pSendMdl);
	}

	DbgPrint("Leave TdiSend\n");
	return status;
}


// TDI TCP������Ϣ
ULONG_PTR TdiRecv(PDEVICE_OBJECT pTdiAddressDevObj, PFILE_OBJECT pTdiEndPointFileObject, PUCHAR pRecvData, ULONG ulRecvDataLength)
{
	DbgPrint("Enter TdiRecv\n");

	NTSTATUS status = STATUS_SUCCESS;
	KEVENT recvEvent;
	PIRP pIrp = NULL;
	IO_STATUS_BLOCK iosb = { 0 };
	PMDL pRecvMdl = NULL;
	ULONG_PTR ulRecvSize = 0;

	// ��ʼ���¼�
	KeInitializeEvent(&recvEvent, NotificationEvent, FALSE);

	// ����I/O�����:TDI_SEND
	pIrp = TdiBuildInternalDeviceControlIrp(TDI_RECV, pTdiAddressDevObj, pTdiEndPointFileObject, &recvEvent, &iosb);
	if (NULL == pIrp)
	{
		ShowError("TdiBuildInternalDeviceControlIrp", 0);
		return STATUS_INSUFFICIENT_RESOURCES;
	}

	// ����MDL
	pRecvMdl = IoAllocateMdl(pRecvData, ulRecvDataLength, FALSE, FALSE, pIrp);
	if (NULL == pRecvMdl)
	{
		ShowError("IoAllocateMdl", 0);
		return STATUS_INSUFFICIENT_RESOURCES;
	}
	MmProbeAndLockPages(pRecvMdl, KernelMode, IoModifyAccess);

	// ��չI/O�����,��ӷ�����Ϣ
	TdiBuildReceive(pIrp, pTdiAddressDevObj, pTdiEndPointFileObject, NULL, NULL, pRecvMdl, TDI_RECEIVE_NORMAL, ulRecvDataLength);

	// �������ʵ���ص�����
	IoSetCompletionRoutine(pIrp, TdiCompletionRoutine, &recvEvent, TRUE, TRUE, TRUE);

	// ����I/O��������ȴ�ִ��
	status = IoCallDriver(pTdiAddressDevObj, pIrp);
	if (STATUS_PENDING == status)
	{
		KeWaitForSingleObject(&recvEvent, Executive, KernelMode, FALSE, NULL);
	}

	// ��ȡʵ�ʽ��յ����ݴ�С
	ulRecvSize = pIrp->IoStatus.Information;

	// �ͷ�MDL
	if (pRecvMdl)
	{
		IoFreeMdl(pRecvMdl);
	}

	DbgPrint("Leave TdiRecv\n");
	return status;
}


// TDI�ر��ͷ�
VOID TdiClose(PFILE_OBJECT pTdiEndPointFileObject, HANDLE hTdiAddress, HANDLE hTdiEndPoint)
{
	DbgPrint("Enter TdiClose\n");

	if (pTdiEndPointFileObject)
	{
		ObDereferenceObject(pTdiEndPointFileObject);
	}
	if (hTdiEndPoint)
	{
		ZwClose(hTdiEndPoint);
	}
	if (hTdiAddress)
	{
		ZwClose(hTdiAddress);
	}

	DbgPrint("Leave TdiClose\n");
}
