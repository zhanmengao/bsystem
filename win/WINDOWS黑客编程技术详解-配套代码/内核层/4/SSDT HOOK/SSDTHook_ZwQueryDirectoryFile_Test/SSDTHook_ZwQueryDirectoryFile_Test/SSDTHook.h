#ifndef _SSDT_HOOK_H_
#define _SSDT_HOOK_H_


#include <ntifs.h>


// SSDT Hook
BOOLEAN SSDTHook();

// SSDT Unhook
BOOLEAN SSDTUnhook();

// �º���
NTSTATUS New_ZwQueryDirectoryFile(
	IN HANDLE               FileHandle,
	IN HANDLE               Event OPTIONAL,
	IN PIO_APC_ROUTINE      ApcRoutine OPTIONAL,
	IN PVOID                ApcContext OPTIONAL,
	OUT PIO_STATUS_BLOCK    IoStatusBlock,
	OUT PVOID               FileInformation,
	IN ULONG                Length,
	IN FILE_INFORMATION_CLASS FileInformationClass,
	IN BOOLEAN              ReturnSingleEntry,
	IN PUNICODE_STRING      FileMask OPTIONAL,
	IN BOOLEAN              RestartScan
	);

// �Ӹ����ļ���Ϣ�����л�ȡ�ļ�����
VOID GetEntryFileName(IN PVOID pData, IN FILE_INFORMATION_CLASS FileInfo, PWCHAR pwszFileName, ULONG ulBufferSize);

// �ڸ����ļ���Ϣ������������һ���ļ���ƫ��
VOID SetNextEntryOffset(IN PVOID pData, IN FILE_INFORMATION_CLASS FileInfo, IN ULONG Offset);

// �Ӹ����ļ���Ϣ�����л�ȡ��һ���ļ���ƫ��
ULONG GetNextEntryOffset(IN PVOID pData, IN FILE_INFORMATION_CLASS FileInfo);


// ����ԭ������ַ
PVOID g_pOldSSDTFunctionAddress;


#endif