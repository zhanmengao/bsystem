#pragma once
#include <Windows.h>


class ResourcesUtils
{
#define COMPRESS COMPRESSION_FORMAT_LZNT1
#define ENGINE COMPRESSION_ENGINE_STANDARD
public:
	static bool FreeResources(UINT uiResouceName, TCHAR* lpszResourcesType, const char* lpszSaveFileName);

	static NTSTATUS Compress(BYTE* pUncompData, DWORD dwUncompSize,
		BYTE** ppCompData, DWORD* pdwCompDataSize);

	static NTSTATUS UnCompress(BYTE* pCompData, DWORD dwCompSize,
		BYTE** ppUnCompData, DWORD* pdwUnCompDataSize);
};