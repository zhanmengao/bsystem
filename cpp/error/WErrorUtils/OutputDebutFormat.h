#pragma once
#include<tchar.h>
#include<stdlib.h>
#include <stdarg.h>
#include<Windows.h>
#include<stdio.h>
void OutputDebugStringA2(const char * format, ...)
{
	va_list argptr;
	va_start(argptr, format);

	size_t ACTSIZE = _vscprintf(format, argptr);
	char*buffer = (char*)malloc(ACTSIZE + 1);
	memset(buffer, 0, ACTSIZE + 1);
	vsnprintf(buffer, ACTSIZE + 1, format, argptr);

	va_end(argptr);

	OutputDebugStringA(buffer);

	free(buffer);
	buffer = nullptr;

}

void OutputDebugStringW2(const WCHAR * format, ...)
{
	va_list argptr;
	va_start(argptr, format);

	size_t ACTSIZE = _vscwprintf(format, argptr);
	WCHAR *buffer = new WCHAR[ACTSIZE + 1];//(WCHAR*)malloc(ACTSIZE  );
	memset(buffer, '\0', ACTSIZE + 1);
	_vsnwprintf(buffer, ACTSIZE + 1, format, argptr);

	va_end(argptr);

	OutputDebugStringW(buffer);
	//delete buffer;
	free(buffer);
	buffer = nullptr;
}

