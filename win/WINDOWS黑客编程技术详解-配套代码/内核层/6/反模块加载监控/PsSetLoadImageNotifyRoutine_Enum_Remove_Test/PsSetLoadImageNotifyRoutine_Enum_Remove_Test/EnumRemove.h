#ifndef _ENUM_REMOVE_H_
#define _ENUM_REMOVE_H_


#include <ntddk.h>


// �����ص�
BOOLEAN EnumNotifyRoutine();

// �Ƴ��ص�
NTSTATUS RemoveNotifyRoutine(PVOID pNotifyRoutineAddress);

// ��ȡ PspLoadImageNotifyRoutine �����ַ
PVOID GetPspLoadImageNotifyRoutine();

// �����������ȡ PspLoadImageNotifyRoutine �����ַ
PVOID SearchPspLoadImageNotifyRoutine(PUCHAR pSpecialData, ULONG ulSpecialDataSize);

// ָ���ڴ������������ɨ��
PVOID SearchMemory(PVOID pStartAddress, PVOID pEndAddress, PUCHAR pMemoryData, ULONG ulMemoryDataSize);


#endif