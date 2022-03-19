#ifndef _REGISTRY_MANAGE_H_
#define _REGISTRY_MANAGE_H_


#include <ntddk.h>


// �������ߴ��Ѵ���ע����
BOOLEAN MyCreateRegistryKey(UNICODE_STRING ustrRegistry);

// ��ע����
BOOLEAN MyOpenRegistryKey(UNICODE_STRING ustrRegistry);

// ��ӻ����޸�ע����ֵ
BOOLEAN MySetRegistryKeyValue(UNICODE_STRING ustrRegistry, UNICODE_STRING ustrKeyValueName, ULONG ulKeyValueType, PVOID pKeyValueData, ULONG ulKeyValueDataSize);

// ɾ��ע����
BOOLEAN MyDeleteRegistryKey(UNICODE_STRING ustrRegistry);

// ɾ��ע����ֵ
BOOLEAN MyDeleteRegistryKeyValue(UNICODE_STRING ustrRegistry, UNICODE_STRING ustrKeyValueName);

// ��ѯע����ֵ
BOOLEAN MyQueryRegistryKeyValue(UNICODE_STRING ustrRegistry, UNICODE_STRING ustrKeyValueName);


#endif