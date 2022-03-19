#include "EnumRemove.h"


VOID ShowError(PCHAR lpszText, NTSTATUS ntStatus)
{
	DbgPrint("%s Error[0x%X]\n", lpszText, ntStatus);
}


// �����ص�
BOOLEAN EnumCallback()
{
	NTSTATUS status = STATUS_SUCCESS;
	ULONG ulFilterListSize = 0;
	PFLT_FILTER *ppFilterList = NULL;
	ULONG i = 0;
	LONG lOperationsOffset = 0;
	PFLT_OPERATION_REGISTRATION pFltOperationRegistration = NULL;

	// ��ȡ Minifilter ������Filter ������
	FltEnumerateFilters(NULL, 0, &ulFilterListSize);
	// �����ڴ�
	ppFilterList = (PFLT_FILTER *)ExAllocatePool(NonPagedPool, ulFilterListSize *sizeof(PFLT_FILTER));
	if (NULL == ppFilterList)
	{
		DbgPrint("ExAllocatePool Error!\n");
		return FALSE;
	}
	// ��ȡ Minifilter �����й�����Filter ����Ϣ
	status = FltEnumerateFilters(ppFilterList, ulFilterListSize, &ulFilterListSize);
	if (!NT_SUCCESS(status))
	{
		DbgPrint("FltEnumerateFilters Error![0x%X]\n", status);
		return FALSE;
	}

	DbgPrint("ulFilterListSize=%d\n", ulFilterListSize);

	// ��ȡ PFLT_FILTER �� Operations ƫ��
	lOperationsOffset = GetOperationsOffset();
	if (0 == lOperationsOffset)
	{
		DbgPrint("GetOperationsOffset Error\n");
		return FALSE;
	}
	
	// ��ʼ���� Minifilter �и���������Filter ����Ϣ
	__try
	{
		for (i = 0; i < ulFilterListSize; i++)
		{
			// ��ȡ PFLT_FILTER �� Operations ��Ա��ַ
			pFltOperationRegistration = (PFLT_OPERATION_REGISTRATION)(*(PVOID *)((PUCHAR)ppFilterList[i] + lOperationsOffset));

			__try
			{
				// ͬһ�������µĻص���Ϣ
				DbgPrint("-------------------------------------------------------------------------------\n");
				while (IRP_MJ_OPERATION_END != pFltOperationRegistration->MajorFunction)   
				{
					if (IRP_MJ_MAXIMUM_FUNCTION > pFltOperationRegistration->MajorFunction)     // MajorFunction ID Is: 0~27
					{
						// ��ʾ
						DbgPrint("[Filter=%p]IRP=%d, PreFunc=0x%p, PostFunc=0x%p\n", ppFilterList[i], pFltOperationRegistration->MajorFunction,
							pFltOperationRegistration->PreOperation, pFltOperationRegistration->PostOperation);
					}
					// ��ȡ��һ����Ϣ�ص���Ϣ
					pFltOperationRegistration = (PFLT_OPERATION_REGISTRATION)((PUCHAR)pFltOperationRegistration + sizeof(FLT_OPERATION_REGISTRATION));
				}
				DbgPrint("-------------------------------------------------------------------------------\n");
			}
			__except (EXCEPTION_EXECUTE_HANDLER)
			{
				DbgPrint("[2_EXCEPTION_EXECUTE_HANDLER]\n");
			}
		}
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		DbgPrint("[1_EXCEPTION_EXECUTE_HANDLER]\n");
	}

	// �ͷ��ڴ�
	ExFreePool(ppFilterList);
	ppFilterList = NULL;

	return TRUE;
}


// �Ƴ��ص�
NTSTATUS RemoveCallback(PFLT_FILTER pFilter)
{
	LONG lOperationsOffset = 0;
	PFLT_OPERATION_REGISTRATION pFltOperationRegistration = NULL;

	// ��ʼ���� ������Filter ����Ϣ
	// ��ȡ PFLT_FILTER �� Operations ��Ա��ַ
	pFltOperationRegistration = (PFLT_OPERATION_REGISTRATION)(*(PVOID *)((PUCHAR)pFilter + lOperationsOffset));
	__try
	{
		// ͬһ�������µĻص���Ϣ
		while (IRP_MJ_OPERATION_END != pFltOperationRegistration->MajorFunction)
		{
			if (IRP_MJ_MAXIMUM_FUNCTION > pFltOperationRegistration->MajorFunction)     // MajorFunction ID Is: 0~27
			{
				// �滻�ص�����
				pFltOperationRegistration->PreOperation = New_MiniFilterPreOperation;
				pFltOperationRegistration->PostOperation = New_MiniFilterPostOperation;

				// ��ʾ
				DbgPrint("[Filter=%p]IRP=%d, PreFunc=0x%p, PostFunc=0x%p\n", pFilter, pFltOperationRegistration->MajorFunction,
					pFltOperationRegistration->PreOperation, pFltOperationRegistration->PostOperation);
			}
			// ��ȡ��һ����Ϣ�ص���Ϣ
			pFltOperationRegistration = (PFLT_OPERATION_REGISTRATION)((PUCHAR)pFltOperationRegistration + sizeof(FLT_OPERATION_REGISTRATION));
		}
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		DbgPrint("[EXCEPTION_EXECUTE_HANDLER]\n");
	}
	
	return STATUS_SUCCESS;
}


// ��ȡ Operations ƫ��
LONG GetOperationsOffset()
{
	RTL_OSVERSIONINFOW osInfo = { 0 };
	LONG lOperationsOffset = 0;

	// ��ȡϵͳ�汾��Ϣ, �ж�ϵͳ�汾
	RtlGetVersion(&osInfo);
	if (6 == osInfo.dwMajorVersion)
	{
		if (1 == osInfo.dwMinorVersion)
		{
			// Win7
#ifdef _WIN64
			// 64 λ
			// 0x188
			lOperationsOffset = 0x188;
#else
			// 32 λ
			// 0xCC
			lOperationsOffset = 0xCC;
#endif	
		}
		else if (2 == osInfo.dwMinorVersion)
		{
			// Win8
#ifdef _WIN64
			// 64 λ
			
#else
			// 32 λ
			
#endif
		}
		else if (3 == osInfo.dwMinorVersion)
		{
			// Win8.1
#ifdef _WIN64
			// 64 λ
			// 0x198
			lOperationsOffset = 0x198;
#else
			// 32 λ
			// 0xD4
			lOperationsOffset = 0xD4;
#endif			
		}
	}
	else if (10 == osInfo.dwMajorVersion)
	{
		// Win10
#ifdef _WIN64
		// 64 λ
		// 0x1A8
		lOperationsOffset = 0x1A8;
#else
		// 32 λ
		// 0xE4
		lOperationsOffset = 0xE4;
#endif
	}

	return lOperationsOffset;
}


// �µĻص����� ��Ϣ����ǰ
FLT_PREOP_CALLBACK_STATUS New_MiniFilterPreOperation(
_Inout_ PFLT_CALLBACK_DATA Data,
_In_ PCFLT_RELATED_OBJECTS FltObjects,
_Flt_CompletionContext_Outptr_ PVOID *CompletionContext
)
{
	UNREFERENCED_PARAMETER(FltObjects);
	UNREFERENCED_PARAMETER(CompletionContext);

	DbgPrint("[New_MiniFilterPreOperation]\n");

	return FLT_PREOP_SUCCESS_WITH_CALLBACK;
}


// �µĻص����� ��Ϣ�����
FLT_POSTOP_CALLBACK_STATUS New_MiniFilterPostOperation(
_Inout_ PFLT_CALLBACK_DATA Data,
_In_ PCFLT_RELATED_OBJECTS FltObjects,
_In_opt_ PVOID CompletionContext,
_In_ FLT_POST_OPERATION_FLAGS Flags
)
{
	UNREFERENCED_PARAMETER(Data);
	UNREFERENCED_PARAMETER(FltObjects);
	UNREFERENCED_PARAMETER(CompletionContext);
	UNREFERENCED_PARAMETER(Flags);

	DbgPrint("[New_MiniFilterPostOperation]\n");

	return FLT_POSTOP_FINISHED_PROCESSING;
}