#ifndef _ENUM_REMOVE_H_
#define _ENUM_REMOVE_H_


#include <fltKernel.h>


// �����ص�
BOOLEAN EnumCallback();

// �Ƴ��ص�
NTSTATUS RemoveCallback(PFLT_FILTER pFilter);

// ��ȡ Operations ƫ��
LONG GetOperationsOffset();

// �µĻص����� ��Ϣ����ǰ
FLT_PREOP_CALLBACK_STATUS
New_MiniFilterPreOperation(
_Inout_ PFLT_CALLBACK_DATA Data,
_In_ PCFLT_RELATED_OBJECTS FltObjects,
_Flt_CompletionContext_Outptr_ PVOID *CompletionContext
);
// �µĻص����� ��Ϣ�����
FLT_POSTOP_CALLBACK_STATUS
New_MiniFilterPostOperation(
_Inout_ PFLT_CALLBACK_DATA Data,
_In_ PCFLT_RELATED_OBJECTS FltObjects,
_In_opt_ PVOID CompletionContext,
_In_ FLT_POST_OPERATION_FLAGS Flags
);


#endif