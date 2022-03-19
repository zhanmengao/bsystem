#ifndef _ENUM_REMOVE_H_
#define _ENUM_REMOVE_H_


#include <ntddk.h>


typedef struct _CM_NOTIFY_ENTRY
{
	LIST_ENTRY  ListEntryHead;
	ULONG   UnKnown1;
	ULONG   UnKnown2;
	LARGE_INTEGER Cookie;
	PVOID   Context;
	PVOID   Function;
}CM_NOTIFY_ENTRY, *PCM_NOTIFY_ENTRY;


// �����ص�
BOOLEAN EnumCallback();

// �Ƴ��ص�
NTSTATUS RemoveCallback(LARGE_INTEGER Cookie);

// ��ȡ CallbackListHead �����ַ
PVOID GetCallbackListHead();

// �����������ȡ CallbackListHead �����ַ
PVOID SearchCallbackListHead(PUCHAR pSpecialData, ULONG ulSpecialDataSize, LONG lSpecialOffset);

// ָ���ڴ������������ɨ��
PVOID SearchMemory(PVOID pStartAddress, PVOID pEndAddress, PUCHAR pMemoryData, ULONG ulMemoryDataSize);


#endif