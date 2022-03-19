#pragma once
#include "stdafx.h"
#include <psapi.h>

void FreeNTDLLFunctions();
HMODULE LoadNTDLLFunctions();
int GetProcessPath(DWORD idProcess, wchar_t* ParentProcessPath);
DWORD GetParentProcessID(DWORD dwProcessId);

bool processGuard(LPWSTR lpCommandLine);
bool LefProcess();

int QuitProcess(DWORD processId ,wchar_t* exitFunName );
