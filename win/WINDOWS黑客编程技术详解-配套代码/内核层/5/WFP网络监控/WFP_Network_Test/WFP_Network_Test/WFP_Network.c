#include "WFP_Network.h"


GUID GUID_ALE_AUTH_CONNECT_CALLOUT_V4 = { 0x6812fc83, 0x7d3e, 0x499a, 0xa0, 0x12, 0x55, 0xe0, 0xd8, 0x5f, 0x34, 0x8b };


VOID ShowError(PCHAR lpszText, NTSTATUS ntStatus)
{
	KdPrint(("%s[Error:0x%X]\n", lpszText, ntStatus));
}


// ����WFP
NTSTATUS WfpLoad(PDEVICE_OBJECT pDevObj)
{
	NTSTATUS status = STATUS_SUCCESS;

	// ע��Callout�����ù��˵�
	status = RegisterCalloutForLayer(
		pDevObj,
		&FWPM_LAYER_ALE_AUTH_CONNECT_V4,
		&GUID_ALE_AUTH_CONNECT_CALLOUT_V4,
		classifyFn,
		notifyFn,
		flowDeleteFn,
		&g_AleConnectCalloutId,
		&g_AleConnectFilterId,
		&g_hEngine);
	if (!NT_SUCCESS(status))
	{
		ShowError("RegisterCalloutForLayer", status);
		return status;
	}

	return status;
}


// ж��WFP
NTSTATUS WfpUnload()
{
	if (NULL != g_hEngine)
	{
		// ɾ��FilterId
		FwpmFilterDeleteById(g_hEngine, g_AleConnectFilterId);
		// ɾ��CalloutId
		FwpmCalloutDeleteById(g_hEngine, g_AleConnectCalloutId);
		// ���Filter
		g_AleConnectFilterId = 0;
		// ��ע��CalloutId
		FwpsCalloutUnregisterById(g_AleConnectCalloutId);
		// ���CalloutId
		g_AleConnectCalloutId = 0;
		// �ر�����
		FwpmEngineClose(g_hEngine);
		g_hEngine = NULL;
	}

	return STATUS_SUCCESS;
}


// ע��Callout�����ù��˵�
NTSTATUS RegisterCalloutForLayer(
	IN PDEVICE_OBJECT pDevObj,
	IN const GUID *layerKey,
	IN const GUID *calloutKey,
	IN FWPS_CALLOUT_CLASSIFY_FN classifyFn,
	IN FWPS_CALLOUT_NOTIFY_FN notifyFn,
	IN FWPS_CALLOUT_FLOW_DELETE_NOTIFY_FN flowDeleteNotifyFn,
	OUT ULONG32 *calloutId,
	OUT ULONG64 *filterId,
	OUT HANDLE *engine)
{
	NTSTATUS status = STATUS_SUCCESS;

	// ע��Callout
	status = RegisterCallout(
		pDevObj,
		calloutKey,
		classifyFn,
		notifyFn,
		flowDeleteNotifyFn,
		calloutId);
	if (!NT_SUCCESS(status))
	{
		return status;
	}

	// ���ù��˵�
	status = SetFilter(
		layerKey,
		calloutKey,
		filterId,
		engine);
	if (!NT_SUCCESS(status))
	{
		return status;
	}

	return status;
}


// ע��Callout
NTSTATUS RegisterCallout(
	PDEVICE_OBJECT pDevObj,
	IN const GUID *calloutKey,
	IN FWPS_CALLOUT_CLASSIFY_FN classifyFn,
	IN FWPS_CALLOUT_NOTIFY_FN notifyFn,
	IN FWPS_CALLOUT_FLOW_DELETE_NOTIFY_FN flowDeleteNotifyFn,
	OUT ULONG32 *calloutId)
{
	NTSTATUS status = STATUS_SUCCESS;
	FWPS_CALLOUT sCallout = { 0 };
	
	// ����Callout
	sCallout.calloutKey = *calloutKey;
	sCallout.classifyFn = classifyFn;
	sCallout.flowDeleteFn = flowDeleteNotifyFn;
	sCallout.notifyFn = notifyFn;

	// ע��Callout
	status = FwpsCalloutRegister(pDevObj, &sCallout, calloutId);
	if (!NT_SUCCESS(status))
	{
		ShowError("FwpsCalloutRegister", status);
		return status;
	}

	return status;
}


// ���ù��˵�
NTSTATUS SetFilter(
	IN const GUID *layerKey,
	IN const GUID *calloutKey,
	OUT ULONG64 *filterId,
	OUT HANDLE *engine)
{
	HANDLE hEngine = NULL;
	NTSTATUS status = STATUS_SUCCESS;
	FWPM_SESSION session = { 0 };
	FWPM_FILTER mFilter = { 0 };
	FWPM_CALLOUT mCallout = { 0 };
	FWPM_DISPLAY_DATA mDispData = { 0 };

	// ����Session
	session.flags = FWPM_SESSION_FLAG_DYNAMIC;
	status = FwpmEngineOpen(NULL,
		RPC_C_AUTHN_WINNT,
		NULL,
		&session,
		&hEngine);
	if (!NT_SUCCESS(status))
	{
		ShowError("FwpmEngineOpen", status);
		return status;
	}

	// ��ʼ����
	status = FwpmTransactionBegin(hEngine, 0);
	if (!NT_SUCCESS(status))
	{
		ShowError("FwpmTransactionBegin", status);
		return status;
	}

	// ����Callout����
	mDispData.name = L"MY WFP TEST";
	mDispData.description = L"WORLD OF DEMON";
	mCallout.applicableLayer = *layerKey;
	mCallout.calloutKey = *calloutKey;
	mCallout.displayData = mDispData;
	// ���Callout��Session��
	status = FwpmCalloutAdd(hEngine, &mCallout, NULL, NULL);
	if (!NT_SUCCESS(status))
	{
		ShowError("FwpmCalloutAdd", status);
		return status;
	}

	// ���ù���������
	mFilter.action.calloutKey = *calloutKey;
	mFilter.action.type = FWP_ACTION_CALLOUT_TERMINATING;
	mFilter.displayData.name = L"MY WFP TEST";
	mFilter.displayData.description = L"WORLD OF DEMON";
	mFilter.layerKey = *layerKey;
	mFilter.subLayerKey = FWPM_SUBLAYER_UNIVERSAL;
	mFilter.weight.type = FWP_EMPTY;
	// ��ӹ�����
	status = FwpmFilterAdd(hEngine, &mFilter, NULL, filterId);
	if (!NT_SUCCESS(status))
	{
		ShowError("FwpmFilterAdd", status);
		return status;
	}

	// �ύ����
	status = FwpmTransactionCommit(hEngine);
	if (!NT_SUCCESS(status))
	{
		ShowError("FwpmTransactionCommit", status);
		return status;
	}

	*engine = hEngine;

	return status;
}


// Callout���� classifyFn
#if (NTDDI_VERSION >= NTDDI_WIN8)
    VOID NTAPI classifyFn(
		_In_ const FWPS_INCOMING_VALUES0* inFixedValues,
		_In_ const FWPS_INCOMING_METADATA_VALUES0* inMetaValues,
		_Inout_opt_ void* layerData,
		_In_opt_ const void* classifyContext,
		_In_ const FWPS_FILTER2* filter,
		_In_ UINT64 flowContext,
		_Inout_ FWPS_CLASSIFY_OUT0* classifyOut
		)
#elif (NTDDI_VERSION >= NTDDI_WIN7)                       
    VOID NTAPI classifyFn(
		_In_ const FWPS_INCOMING_VALUES0* inFixedValues,
		_In_ const FWPS_INCOMING_METADATA_VALUES0* inMetaValues,
		_Inout_opt_ void* layerData,
		_In_opt_ const void* classifyContext,
		_In_ const FWPS_FILTER1* filter,
		_In_ UINT64 flowContext,
		_Inout_ FWPS_CLASSIFY_OUT0* classifyOut
		)
#else
    VOID NTAPI classifyFn(
		_In_ const FWPS_INCOMING_VALUES0* inFixedValues,
		_In_ const FWPS_INCOMING_METADATA_VALUES0* inMetaValues,
		_Inout_opt_ void* layerData,
		_In_ const FWPS_FILTER0* filter,
		_In_ UINT64 flowContext,
		_Inout_ FWPS_CLASSIFY_OUT0* classifyOut
		)
#endif
{
	/*
	��WFP �Ļص��������ṩ�˷ḻ����Ϣ������ WFP �����ŵ㣬 ��������Ϊ��ø��������Ϣ���ʾ���֭��
	������ FWPM_LAYER_ALE_AUTH_CONNECT_V4 �Ļص�����������ܻ�ý��� ID������·�������ء�Զ
	�̵� IP ��ַ/�˿ں��Լ�Э����롣 ����ˬ���Ǵ˻ص����������һ����������������ָ��һ��ֵ��������
	���л�������.
	*/
	ULONG ulLocalIp = inFixedValues->incomingValue[FWPS_FIELD_ALE_AUTH_CONNECT_V4_IP_LOCAL_ADDRESS].value.uint32;
	UINT16 uLocalPort = inFixedValues->incomingValue[FWPS_FIELD_ALE_AUTH_CONNECT_V4_IP_LOCAL_PORT].value.uint16;
	ULONG ulRemoteIp = inFixedValues->incomingValue[FWPS_FIELD_ALE_AUTH_CONNECT_V4_IP_REMOTE_ADDRESS].value.uint32;
	UINT16 uRemotePort = inFixedValues->incomingValue[FWPS_FIELD_ALE_AUTH_CONNECT_V4_IP_REMOTE_PORT].value.uint16;
	KIRQL kCurrentIrql = KeGetCurrentIrql();
	ULONG64 processId = inMetaValues->processId;
	UCHAR szProcessPath[256] = { 0 };
	CHAR szProtocalName[256] = { 0 };
	RtlZeroMemory(szProcessPath, 256);
	ULONG i = 0;
	// ��ȡ����·��
	for (i = 0; i < inMetaValues->processPath->size; i++)
	{
		// �����ǿ��ַ��洢��
		szProcessPath[i] = inMetaValues->processPath->data[i];
	}
	
	// ��������
	classifyOut->actionType = FWP_ACTION_PERMIT;    

	// ��ָֹ��������������
	if (NULL != wcsstr((PWCHAR)szProcessPath, L"tcpclient.exe"))
	{
		KdPrint(("TCPClient.exe[FWP_ACTION_BLOCK]\n"));
		// �ܾ�����
		classifyOut->actionType = FWP_ACTION_BLOCK;
		classifyOut->rights = classifyOut->rights & (~FWPS_RIGHT_ACTION_WRITE);
		classifyOut->flags = classifyOut->flags | FWPS_CLASSIFY_OUT_FLAG_ABSORB;
	}
	// Э���ж�
	ProtocalIdToName(inFixedValues->incomingValue[FWPS_FIELD_ALE_AUTH_CONNECT_V4_IP_PROTOCOL].value.uint16, szProtocalName);
	// ��ʾ
	DbgPrint("Protocal=%s, LocalIp=%u.%u.%u.%u:%d, RemoteIp=%u.%u.%u.%u:%d, IRQL=%d, PID=%I64d, Path=%S\n",
		szProtocalName,
		(ulLocalIp >> 24) & 0xFF,
		(ulLocalIp >> 16) & 0xFF,
		(ulLocalIp >> 8) & 0xFF, 
		(ulLocalIp)& 0xFF,
		uLocalPort,
		(ulRemoteIp >> 24) & 0xFF, 
		(ulRemoteIp >> 16) & 0xFF, 
		(ulRemoteIp >> 8) & 0xFF, 
		(ulRemoteIp)& 0xFF,
		uRemotePort,
		kCurrentIrql,
		processId, 
		(PWCHAR)szProcessPath);
}

// Callout���� notifyFn
#if (NTDDI_VERSION >= NTDDI_WIN8)
	NTSTATUS NTAPI notifyFn(
		_In_ FWPS_CALLOUT_NOTIFY_TYPE notifyType,
		_In_ const GUID* filterKey,
		_Inout_ FWPS_FILTER2* filter
		)
#elif (NTDDI_VERSION >= NTDDI_WIN7)
	NTSTATUS NTAPI notifyFn(
		_In_ FWPS_CALLOUT_NOTIFY_TYPE notifyType,
		_In_ const GUID* filterKey,
		_Inout_ FWPS_FILTER1* filter
		)
#else
	NTSTATUS NTAPI notifyFn(
		_In_ FWPS_CALLOUT_NOTIFY_TYPE notifyType,
		_In_ const GUID* filterKey,
		_Inout_ FWPS_FILTER0* filter
		)
#endif
{
	NTSTATUS status = STATUS_SUCCESS;


	return status;
}


// Callout���� flowDeleteFn
VOID NTAPI flowDeleteFn(
	_In_ UINT16 layerId,
	_In_ UINT32 calloutId,
	_In_ UINT64 flowContext
	)
{
	return;
}


// Э���ж�
NTSTATUS ProtocalIdToName(UINT16 protocalId, PCHAR lpszProtocalName)
{
	NTSTATUS status = STATUS_SUCCESS;

	switch (protocalId)
	{
	case 1:
	{
		// ICMP
		RtlCopyMemory(lpszProtocalName, "ICMP", 5);
		break;
	}
	case 2:
	{
		// IGMP
		RtlCopyMemory(lpszProtocalName, "IGMP", 5);
		break;
	}
	case 6:
	{
		// TCP
		RtlCopyMemory(lpszProtocalName, "TCP", 4);
		break;
	}
	case 17:
	{
		// UDP
		RtlCopyMemory(lpszProtocalName, "UDP", 4);
		break;
	}
	case 27:
	{
		// RDP
		RtlCopyMemory(lpszProtocalName, "RDP", 6);
		break;
	}
	default:
	{
		// UNKNOW
		RtlCopyMemory(lpszProtocalName, "UNKNOWN", 8);
		break;
	}
	}

	return status;
}