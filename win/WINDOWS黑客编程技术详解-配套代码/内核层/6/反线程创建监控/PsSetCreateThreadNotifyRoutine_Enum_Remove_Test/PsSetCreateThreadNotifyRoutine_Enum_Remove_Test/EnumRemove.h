#ifndef _ENUM_REMOVE_H_
#define _ENUM_REMOVE_H_


#include <ntddk.h>


// �����ص�
BOOLEAN EnumNotifyRoutine();

// �Ƴ��ص�
NTSTATUS RemoveNotifyRoutine(PVOID pNotifyRoutineAddress);

// ��ȡ PspCreateThreadNotifyRoutine �����ַ
PVOID GetPspCreateThreadNotifyRoutine();

// �����������ȡ PspCreateThreadNotifyRoutine �����ַ
PVOID SearchPspCreateThreadNotifyRoutine(PUCHAR pFirstSpecialData, ULONG ulFirstSpecialDataSize, PUCHAR pSecondSpecialData, ULONG ulSecondSpecialDataSize);

// ָ���ڴ������������ɨ��
PVOID SearchMemory(PVOID pStartAddress, PVOID pEndAddress, PUCHAR pMemoryData, ULONG ulMemoryDataSize);


#endif