#ifndef _TDI_TCP_H_
#define _TDI_TCP_H_


#include <ntddk.h>
#include <tdikrnl.h>

// TCP�����豸����
#define COMM_TCP_DEV_NAME L"\\Device\\Tcp"

// ��ַת���ĺ�
#define INETADDR(a, b, c, d) (a + (b<<8) + (c<<16) + (d<<24))
#define HTONL(a) (((a & 0xFF)<<24) + ((a & 0xFF00)<<8) + ((a & 0xFF0000)>>8) + (a&0xFF000000)>>24)
#define HTONS(a) (((a & 0xFF)<<8) + ((a & 0xFF00)>>8))


VOID ShowError(PCHAR lpszText, NTSTATUS ntStatus);
// ��ɻص�����
NTSTATUS TdiCompletionRoutine(PDEVICE_OBJECT pDevObj, PIRP pIrp, PVOID pContext);
// TDI��ʼ������
NTSTATUS TdiOpen(PDEVICE_OBJECT *ppTdiAddressDevObj, PFILE_OBJECT *ppTdiEndPointFileObject, HANDLE *phTdiAddress, HANDLE *phTdiEndPoint);
// TDI TCP���ӷ�����
NTSTATUS TdiConnection(PDEVICE_OBJECT pTdiAddressDevObj, PFILE_OBJECT pTdiEndPointFileObject, LONG *pServerIp, LONG lServerPort);
// TDI TCP������Ϣ
NTSTATUS TdiSend(PDEVICE_OBJECT pTdiAddressDevObj, PFILE_OBJECT pTdiEndPointFileObject, PUCHAR pSendData, ULONG ulSendDataLength);
// TDI TCP������Ϣ
ULONG_PTR TdiRecv(PDEVICE_OBJECT pTdiAddressDevObj, PFILE_OBJECT pTdiEndPointFileObject, PUCHAR pRecvData, ULONG ulRecvDataLength);
// TDI�ر��ͷ�
VOID TdiClose(PFILE_OBJECT pTdiEndPointFileObject, HANDLE hTdiAddress, HANDLE hTdiEndPoint);


#endif