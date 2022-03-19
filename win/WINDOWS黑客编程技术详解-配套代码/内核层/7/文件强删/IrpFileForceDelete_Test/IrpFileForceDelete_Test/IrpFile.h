#ifndef _IRP_FILE_H_
#define _IRP_FILE_H_


#include <ntifs.h>


typedef struct _AUX_ACCESS_DATA {
	PPRIVILEGE_SET PrivilegesUsed;
	GENERIC_MAPPING GenericMapping;
	ACCESS_MASK AccessesToAudit;
	ACCESS_MASK MaximumAuditMask;
	ULONG Unknown[256];
} AUX_ACCESS_DATA, *PAUX_ACCESS_DATA;


NTSTATUS SeCreateAccessState(
	PACCESS_STATE AccessState,
	PVOID AuxData,
	ACCESS_MASK DesiredAccess,
	PGENERIC_MAPPING GenericMapping
	);

NTSTATUS ObCreateObject(
	__in KPROCESSOR_MODE ProbeMode,            // �����Ƿ�Ҫ��֤����
	__in POBJECT_TYPE ObjectType,              // ��������ָ��
	__in POBJECT_ATTRIBUTES ObjectAttributes,  // ���������, ���ջ�ת����ObAllocateObject��Ҫ��OBJECT_CREATE_INFORMATION�ṹ
	__in KPROCESSOR_MODE OwnershipMode,        // �ں˶���?�û�����? ͬ��
	__inout_opt PVOID ParseContext,            // �����û��
	__in ULONG ObjectBodySize,                 // �������С
	__in ULONG PagedPoolCharge,                // ...
	__in ULONG NonPagedPoolCharge,             // ...
	__out PVOID *Object                        // ���ն������ָ��
	);




// ���ʵ��, �����¼��ź���, ���ͷ�IRP
NTSTATUS MyCompleteRoutine(
	IN PDEVICE_OBJECT DeviceObject,
	IN PIRP pIrp,
	IN PVOID Context);


// �������ߴ��ļ�
// ZwCreateFile
NTSTATUS IrpCreateFile(
	OUT PFILE_OBJECT *ppFileObject,
	IN ACCESS_MASK DesiredAccess,
	IN PUNICODE_STRING pustrFilePath,
	OUT PIO_STATUS_BLOCK IoStatusBlock,
	IN PLARGE_INTEGER AllocationSize OPTIONAL,
	IN ULONG FileAttributes,
	IN ULONG ShareAccess,
	IN ULONG CreateDisposition,
	IN ULONG CreateOptions,
	IN PVOID EaBuffer OPTIONAL,
	IN ULONG EaLength);

// �ļ�����
// ZwQueryDirectoryFile
NTSTATUS IrpQueryDirectoryFile(
	IN PFILE_OBJECT pFileObject,
	OUT PIO_STATUS_BLOCK IoStatusBlock,
	OUT PVOID FileInformation,
	IN ULONG Length,
	IN FILE_INFORMATION_CLASS FileInformationClass,
	IN PUNICODE_STRING FileName OPTIONAL);

// ��ȡ�ļ�����Ϣ
// ZwQueryInformationFile
NTSTATUS IrpQueryInformationFile(
	IN PFILE_OBJECT pFileObject,
	OUT PIO_STATUS_BLOCK IoStatusBlock,
	OUT PVOID FileInformation,
	IN ULONG Length,
	IN FILE_INFORMATION_CLASS FileInformationClass
	);


// �����ļ���Ϣ
// ZwSetInformationFile
NTSTATUS IrpSetInformationFile(
	IN PFILE_OBJECT pFileObject,
	OUT PIO_STATUS_BLOCK IoStatusBlock,
	IN PVOID FileInformation,
	IN ULONG Length,
	IN FILE_INFORMATION_CLASS FileInformationClass
	);

// ���ļ�
// ZwReadFile
NTSTATUS IrpReadFile(
	IN PFILE_OBJECT pFileObject,
	OUT PIO_STATUS_BLOCK IoStatusBlock,
	OUT PVOID Buffer,
	IN ULONG Length,
	IN PLARGE_INTEGER ByteOffset OPTIONAL
	);

// д�ļ�
// ZwWriteFile
NTSTATUS IrpWriteFile(
	IN PFILE_OBJECT pFileObject,
	OUT PIO_STATUS_BLOCK IoStatusBlock,
	IN PVOID Buffer,
	IN ULONG Length,
	IN PLARGE_INTEGER ByteOffset OPTIONAL
	);


#endif