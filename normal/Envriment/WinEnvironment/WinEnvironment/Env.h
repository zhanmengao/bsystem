#pragma once
#include<Windows.h>
#include<stdio.h>
#include<strsafe.h>
#include<tchar.h>
#pragma comment (lib,"Psapi.lib")
using namespace std;
#pragma   comment(lib,   "shell32.lib")


/*
%WINDIR%                 {系统目录 - C:\WINDOWS}
%SYSTEMROOT%             {系统目录 - C:\WINDOWS}
%SYSTEMDRIVE%            {系统根目录 - C:}
%HOMEDRIVE%              {当前用户根目录 - C:}
%USERPROFILE%            {当前用户目录 - C:\Documents and Settings\wy}
%HOMEPATH%               {当前用户路径 - \Documents and Settings\wy}
%TMP%                    {当前用户临时文件夹 - C:\DOCUME~1\wy\LOCALS~1\Temp}
%TEMP%                   {当前用户临时文件夹 - C:\DOCUME~1\wy\LOCALS~1\Temp}
%APPDATA%                {当前用户数据文件夹 - C:\Documents and Settings\wy\Application Data}
%PROGRAMFILES%           {程序默认安装目录 - C:\Program Files}
%COMMONPROGRAMFILES%     {文件通用目录 - C:\Program Files\Common Files}

%ALLUSERSPROFILE%        {所有用户文件目录 - C:\Documents and Settings\All Users}
%USERNAME%               {当前用户名 - wy}
%OS%                     {操作系统名 - Windows_NT}
%COMPUTERNAME%           {计算机名 - IBM-B63851E95C9}
%NUMBER_OF_PROCESSORS%   {处理器个数 - 1}
%PROCESSOR_ARCHITECTURE% {处理器芯片架构 - x86}
%PROCESSOR_LEVEL%        {处理器型号 - 6}
%PROCESSOR_REVISION%     {处理器修订号 - 0905}
%USERDOMAIN%             {包含用户帐号的域 - IBM-B63851E95C9}
%COMSPEC%                {C:\WINDOWS\system32\cmd.exe}
*/
static VOID GetEnvironmentQS()
{
	WCHAR szChar[MAX_PATH] = { 0 };
	// 使用SHGetSpecialFolderPath获取特殊目录路径
	GetEnvironmentVariable(L"WINDIR", szChar, MAX_PATH);
	printf("WINDIR:\t %ws\n", szChar);
	GetEnvironmentVariable(L"SYSTEMROOT", szChar, MAX_PATH);
	printf("SYSTEMROOT:\t %ws\n", szChar);
	GetEnvironmentVariable(L"SYSTEMDRIVE", szChar, MAX_PATH);
	printf("SYSTEMDRIVE:\t %ws\n", szChar);
	GetEnvironmentVariable(L"HOMEDRIVE", szChar, MAX_PATH);
	printf("HOMEDRIVE:\t %ws\n", szChar);
	GetEnvironmentVariable(L"USERPROFILE", szChar, MAX_PATH);
	printf("USERPROFILE:\t %ws\n", szChar);
	GetEnvironmentVariable(L"HOMEPATH", szChar, MAX_PATH);
	printf("HOMEPATH:\t %ws\n", szChar);
	GetEnvironmentVariable(L"TMP", szChar, MAX_PATH);
	printf("TMP:\t %ws\n", szChar);
	GetEnvironmentVariable(L"TEMP", szChar, MAX_PATH);
	printf("TEMP:\t %ws\n", szChar);
	GetEnvironmentVariable(L"APPDATA", szChar, MAX_PATH);
	printf("APPDATA:\t %ws\n", szChar);
	GetEnvironmentVariable(L"PROGRAMFILES", szChar, MAX_PATH);
	printf("PROGRAMFILES:\t %ws\n", szChar);
	GetEnvironmentVariable(L"COMMONPROGRAMFILES", szChar, MAX_PATH);
	printf("COMMONPROGRAMFILES:\t %ws\n", szChar);
	GetEnvironmentVariable(L"USERNAME", szChar, MAX_PATH);
	printf("USERNAME:\t %ws\n", szChar);
	GetEnvironmentVariable(L"ALLUSERSPROFILE", szChar, MAX_PATH);
	printf("ALLUSERSPROFILE:\t %ws\n", szChar);
	GetEnvironmentVariable(L"OS", szChar, MAX_PATH);
	printf("OS:\t %ws\n", szChar);
	GetEnvironmentVariable(L"COMPUTERNAME", szChar, MAX_PATH);
	printf("COMPUTERNAME:\t %ws\n", szChar);
	GetEnvironmentVariable(L"NUMBER_OF_PROCESSORS", szChar, MAX_PATH);
	printf("NUMBER_OF_PROCESSORS:\t %ws\n", szChar);
	GetEnvironmentVariable(L"PROCESSOR_ARCHITECTURE", szChar, MAX_PATH);
	printf("PROCESSOR_ARCHITECTURE:\t %ws\n", szChar);
	GetEnvironmentVariable(L"PROCESSOR_LEVEL", szChar, MAX_PATH);
	printf("PROCESSOR_LEVEL:\t %ws\n", szChar);
	GetEnvironmentVariable(L"PROCESSOR_REVISION", szChar, MAX_PATH);
	printf("PROCESSOR_REVISION:\t %ws\n", szChar);
	GetEnvironmentVariable(L"USERDOMAIN", szChar, MAX_PATH);
	printf("USERDOMAIN:\t %ws\n", szChar);
	GetEnvironmentVariable(L"COMSPEC", szChar, MAX_PATH);
	printf("COMSPEC:\t %ws\n", szChar);
}


BOOL IsWow64()
{
	BOOL bIsWow64 = FALSE;

	typedef BOOL(WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);

	LPFN_ISWOW64PROCESS fnIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress(
		GetModuleHandle(TEXT("kernel32")), "IsWow64Process");

	if (NULL != fnIsWow64Process)
	{
		if (!fnIsWow64Process(GetCurrentProcess(), &bIsWow64))
		{
			//handle error
		}
	}
	return bIsWow64;
}

int EnvExpand()
{
	char l_szSrcA[256] = { "%ProgramFiles%\\WindowsPowerShell\\;%SystemRoot%\\system32\\;%SystemDrive%\\" };
	char l_szDstA[256] = { 0 };
	printf("源字符串：%s\n", l_szSrcA);
	//扩充环境字符串：将由百分号封闭起来的环境变量名转换成那个变量的内容。
	ExpandEnvironmentStringsA(l_szSrcA, l_szDstA, 256);
	printf("目的字符串：%s\n", l_szDstA);

	char l_szEnvironmentStringA[260] = { 0 };
	GetEnvironmentVariableA("ComSpec", l_szEnvironmentStringA, 260);
	printf("ComSpec:%s\n", l_szEnvironmentStringA);
	strcat_s(l_szEnvironmentStringA, _countof(l_szEnvironmentStringA), "QAQQQQQ");
	SetEnvironmentVariableA("MyEnv", l_szEnvironmentStringA);									//添加一个新环境变量

	GetEnvironmentVariableA("MyEnv", l_szEnvironmentStringA, 260);
	printf("MyEnv:%s\n", l_szEnvironmentStringA);
	return 0;
}

void DumpEnvStrings()
{
	TCHAR* pEnvBlock = GetEnvironmentStrings();
	TCHAR szName[MAX_PATH] = { 0 };
	TCHAR szValue[MAX_PATH] = { 0 };
	const TCHAR* pszCurrent = pEnvBlock;
	HRESULT hr = S_OK;
	const TCHAR* pszPos = NULL;
	int current = 0;
	while (pszCurrent != NULL)
	{
		//如果当前不是字符
		if (*pszCurrent != TEXT('='))
		{
			//找到下一个字符
			pszPos = _tcschr(pszCurrent, TEXT('='));
			pszPos++;
			
			size_t cbNameLength = (size_t)(pszPos)-(size_t)pszCurrent - sizeof(TCHAR);
			hr = StringCbCopyN(szName, MAX_PATH, pszCurrent, cbNameLength);
			if (FAILED(hr))
			{
				break;
			}
			hr = StringCchCopyN(szValue, MAX_PATH, pszPos, _tcslen(pszPos) + 1);
			if (SUCCEEDED(hr))
			{
				_tprintf(TEXT(" [%u]  %s=%s\r\n"), current, szName, szValue);
			}
			else if (hr == STRSAFE_E_INSUFFICIENT_BUFFER)
			{
				_tprintf(TEXT(" [%u]  %s=%s...\r\n"),current,szName,szValue);
			}
			else
			{
				_tprintf(TEXT(" [%u]  %s=???...\r\n"),current,szName);
			}
		}
		else
		{
			_tprintf(TEXT(" [%u]  %s\r\n"), current, szName);
		}
		current++;
		while (*pszCurrent != TEXT('\0'))
		{
			pszCurrent++;
		}
		pszCurrent++;
		if (*pszCurrent == TEXT('\0'))
		{
			break;
		}
	}
	FreeEnvironmentStrings(pEnvBlock);
}

void DumpEnvVariables(TCHAR** pEnvBlock)
{
	int current = 0;
	TCHAR** pElement = pEnvBlock;
	TCHAR* pCurrent = NULL;
	while (pElement != NULL)
	{
		pCurrent = (TCHAR*)(*pElement);
		//没有更多的环境变量了
		if (pCurrent == NULL)
		{
			pElement = NULL;
		}
		else
		{
			_tprintf(TEXT("  [%u] %s \r\n"), current, pCurrent);
			current++;
			pElement++;
		}
	}
}