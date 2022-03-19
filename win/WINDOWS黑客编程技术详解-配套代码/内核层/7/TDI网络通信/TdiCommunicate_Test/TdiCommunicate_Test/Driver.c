#include "Driver.h"
#include "TdiTcp.h"
#include <ntstatus.h>


typedef struct _MY_DATA
{
	PDEVICE_OBJECT pTdiAddressDevObj;
	PFILE_OBJECT pTdiEndPointFileObject;
	HANDLE hTdiAddress;
	HANDLE hTdiEndPoint;
}MY_DATA, *PMY_DATA;


// ������Ϣ���߳�
VOID RecvThreadProc(_In_ PVOID StartContext)
{
	PMY_DATA pMyData = (PMY_DATA)StartContext;
	NTSTATUS status = STATUS_SUCCESS;
	char szRecvData[1024] = { 0 };
	ULONG ulRecvDataLenngth = 1024;
	RtlZeroMemory(szRecvData, ulRecvDataLenngth);

	// TDI TCP������Ϣ
	do
	{
		ulRecvDataLenngth = TdiRecv(pMyData->pTdiAddressDevObj, pMyData->pTdiEndPointFileObject, szRecvData, ulRecvDataLenngth);
		if (0 < ulRecvDataLenngth)
		{
			DbgPrint("[TDI Recv]%s\n", szRecvData);
			break;;
		}

	} while (TRUE);

	// �ͷ�
	TdiClose(pMyData->pTdiEndPointFileObject, pMyData->hTdiAddress, pMyData->hTdiEndPoint);
	ExFreePool(pMyData);
}


// TDI ͨ�Ų���
VOID TdiTest()
{
	NTSTATUS status = STATUS_SUCCESS;
	HANDLE hTdiAddress = NULL;
	HANDLE hTdiEndPoint = NULL;
	PDEVICE_OBJECT pTdiAddressDevObj = NULL;
	PFILE_OBJECT pTdiEndPointFileObject = NULL;
	LONG pServerIp[4] = {127, 0, 0, 1};
	LONG lServerPort = 12345;
	UCHAR szSendData[] = "I am Demon`Gan--->From TDI";
	ULONG ulSendDataLength = 1 + strlen(szSendData);
	HANDLE hThread = NULL;

	// TDI��ʼ��
	status = TdiOpen(&pTdiAddressDevObj, &pTdiEndPointFileObject, &hTdiAddress, &hTdiEndPoint);
	if (!NT_SUCCESS(status))
	{
		DbgPrint("TdiOpen Error[0x%X]\n", status);
		return;
	}
	// TDI TCP���ӷ�����
	status = TdiConnection(pTdiAddressDevObj, pTdiEndPointFileObject, pServerIp, lServerPort);
	if (!NT_SUCCESS(status))
	{
		DbgPrint("TdiConnection Error[0x%X]\n", status);
		return;
	}
	// TDI TCP������Ϣ
	status = TdiSend(pTdiAddressDevObj, pTdiEndPointFileObject, szSendData, ulSendDataLength);
	if (!NT_SUCCESS(status))
	{
		DbgPrint("TdiSend Error[0x%X]\n", status);
		return;
	}
	DbgPrint("[TDI Send]%s\n", szSendData);
	// ����������Ϣ���߳�, ѭ��������Ϣ
	PMY_DATA pMyData = ExAllocatePool(NonPagedPool, sizeof(MY_DATA));
	pMyData->pTdiAddressDevObj = pTdiAddressDevObj;
	pMyData->pTdiEndPointFileObject = pTdiEndPointFileObject;
	pMyData->hTdiAddress = hTdiAddress;
	pMyData->hTdiEndPoint = hTdiEndPoint;
	PsCreateSystemThread(&hThread, 0, NULL, NtCurrentProcess(), NULL, RecvThreadProc, pMyData);
}


NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObject, PUNICODE_STRING pRegPath)
{
	DbgPrint("Enter DriverEntry\n");
	NTSTATUS status = STATUS_SUCCESS;

	pDriverObject->DriverUnload = DriverUnload;
	pDriverObject->MajorFunction[IRP_MJ_CREATE] = DriverDefaultHandle;
	pDriverObject->MajorFunction[IRP_MJ_CLOSE] = DriverDefaultHandle;

	// TDI ͨ�Ų���
	TdiTest();

	DbgPrint("Leave DriverEntry\n");
	return STATUS_SUCCESS;
}

VOID DriverUnload(PDRIVER_OBJECT pDriverObject)
{
	DbgPrint("Enter DriverUnload\n");

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

NTSTATUS CreateDevice(PDRIVER_OBJECT pDriverObject)
{
	NTSTATUS status = STATUS_SUCCESS;
	PDEVICE_OBJECT pDevObj = NULL;
	UNICODE_STRING ustrDevName, ustrSymName;

	RtlInitUnicodeString(&ustrDevName, DEV_NAME);
	RtlInitUnicodeString(&ustrSymName, SYM_NAME);
	status = IoCreateDevice(pDriverObject, 0, &ustrDevName, FILE_DEVICE_UNKNOWN, 0, FALSE, &pDevObj);
	if (!NT_SUCCESS(status))
	{
		ShowError("IoCreateDevice", status);
		return status;
	}
	status = IoCreateSymbolicLink(&ustrSymName, &ustrDevName);
	if (!NT_SUCCESS(status))
	{
		ShowError("IoCreateSymbolicLink", status);
		IoDeleteDevice(pDevObj);
		return status;
	}

	return status;
}