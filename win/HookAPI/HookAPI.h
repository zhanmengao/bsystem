#pragma once
#include<Windows.h>
#include <Winternl.h>
#include"../DLL×¢Èë/ExpDefine.h"
#define EXPORT extern "C" __declspec(dllexport)


NTSTATUS New_ZwQuerySystemInformation(
	SYSTEM_INFORMATION_CLASS SystemInformationClass,
	PVOID SystemInformation,
	ULONG SystemInformationLength,
	PULONG ReturnLength);

EXPORT HHOOK SetHook();
EXPORT BOOL UnsetHook(HHOOK hHook);



