#ifndef _ENUM_REMOVE_H_
#define _ENUM_REMOVE_H_


#include <ntddk.h>


// �����ص�
BOOLEAN EnumNotifyRoutine();

// �Ƴ��ص�
NTSTATUS RemoveNotifyRoutine(PVOID pNotifyRoutineAddress);

// ��ȡ PspCreateProcessNotifyRoutine �����ַ
PVOID GetPspCreateProcessNotifyRoutine();

// �����������ȡ PspCreateProcessNotifyRoutine �����ַ
PVOID SearchPspCreateProcessNotifyRoutine(PUCHAR pFirstSpecialData, ULONG ulFirstSpecialDataSize, PUCHAR pSecondSpecialData, ULONG ulSecondSpecialDataSize);

// ָ���ڴ������������ɨ��
PVOID SearchMemory(PVOID pStartAddress, PVOID pEndAddress, PUCHAR pMemoryData, ULONG ulMemoryDataSize);


#endif