#ifndef _WFP_NETWORK_H_
#define _WFP_NETWORK_H_


#include <ntddk.h>
#include <fwpsk.h>
#include <fwpmk.h>


VOID ShowError(PCHAR lpszText, NTSTATUS ntStatus);

// ����WFP
NTSTATUS WfpLoad(PDEVICE_OBJECT pDevObj);

// ж��WFP
NTSTATUS WfpUnload();

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
	OUT HANDLE *engine);

// ע��Callout
NTSTATUS RegisterCallout(
	PDEVICE_OBJECT pDevObj,
	IN const GUID *calloutKey,
	IN FWPS_CALLOUT_CLASSIFY_FN classifyFn,
	IN FWPS_CALLOUT_NOTIFY_FN notifyFn,
	IN FWPS_CALLOUT_FLOW_DELETE_NOTIFY_FN flowDeleteNotifyFn,
	OUT ULONG32 *calloutId);

// ���ù��˵�
NTSTATUS SetFilter(
	IN const GUID *layerKey,
	IN const GUID *calloutKey,
	OUT ULONG64 *filterId,
	OUT HANDLE *engine);

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
		);
#elif (NTDDI_VERSION >= NTDDI_WIN7)                       
    VOID NTAPI classifyFn(
		_In_ const FWPS_INCOMING_VALUES0* inFixedValues,
		_In_ const FWPS_INCOMING_METADATA_VALUES0* inMetaValues,
		_Inout_opt_ void* layerData,
		_In_opt_ const void* classifyContext,
		_In_ const FWPS_FILTER1* filter,
		_In_ UINT64 flowContext,
		_Inout_ FWPS_CLASSIFY_OUT0* classifyOut
		);
#else
    VOID NTAPI classifyFn(
		_In_ const FWPS_INCOMING_VALUES0* inFixedValues,
		_In_ const FWPS_INCOMING_METADATA_VALUES0* inMetaValues,
		_Inout_opt_ void* layerData,
		_In_ const FWPS_FILTER0* filter,
		_In_ UINT64 flowContext,
		_Inout_ FWPS_CLASSIFY_OUT0* classifyOut
		);
#endif

// Callout���� notifyFn
#if (NTDDI_VERSION >= NTDDI_WIN8)
	NTSTATUS NTAPI notifyFn(
		_In_ FWPS_CALLOUT_NOTIFY_TYPE notifyType,
		_In_ const GUID* filterKey,
		_Inout_ FWPS_FILTER2* filter
		);
#elif (NTDDI_VERSION >= NTDDI_WIN7)
	NTSTATUS NTAPI notifyFn(
		_In_ FWPS_CALLOUT_NOTIFY_TYPE notifyType,
		_In_ const GUID* filterKey,
		_Inout_ FWPS_FILTER1* filter
		);
#else
	NTSTATUS NTAPI notifyFn(
		_In_ FWPS_CALLOUT_NOTIFY_TYPE notifyType,
		_In_ const GUID* filterKey,
		_Inout_ FWPS_FILTER0* filter
		);
#endif

// Callout���� flowDeleteFn
VOID NTAPI flowDeleteFn(
	_In_ UINT16 layerId,
	_In_ UINT32 calloutId,
	_In_ UINT64 flowContext
	);

// Э���ж�
NTSTATUS ProtocalIdToName(UINT16 protocalId, PCHAR lpszProtocalName);


// ������������
HANDLE g_hEngine;
// �����������е�callout������ʱ��ʶ��
ULONG32 g_AleConnectCalloutId;
// ������������ʱ��ʶ��
ULONG64 g_AleConnectFilterId;


#endif