#ifndef _LOAD_IMAGE_NOTIFY_H_
#define _LOAD_IMAGE_NOTIFY_H_


#include <ntifs.h>
#include <ntimage.h>


typedef struct _MY_DATA
{
	HANDLE ProcessId;
	PVOID pImageBase;
}MY_DATA, *PMY_DATA;


NTSTATUS MmUnmapViewOfSection(PEPROCESS Process, PVOID BaseAddress);

NTSTATUS SetNotifyRoutine();
NTSTATUS RemoveNotifyRoutine();
VOID LoadImageNotifyRoutine(PUNICODE_STRING FullImageName, HANDLE ProcessId, PIMAGE_INFO ImageInfo);

NTSTATUS U2C(PUNICODE_STRING pustrSrc, PCHAR pszDest, ULONG ulDestLength);
VOID ThreadProc(_In_ PVOID StartContext);

// �ܾ���������
NTSTATUS DenyLoadDriver(PVOID pImageBase);
// �ܾ�����DLLģ��
NTSTATUS DenyLoadDll(HANDLE ProcessId, PVOID pImageBase);

#endif