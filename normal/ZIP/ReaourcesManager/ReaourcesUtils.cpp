#include"ResourcesUtils.h"
#include<stdio.h>
#include<tchar.h>
#include <shlobj.h>


#define COMPRESS COMPRESSION_FORMAT_LZNT1
#define ENGINE COMPRESSION_ENGINE_STANDARD
typedef NTSTATUS(WINAPI* typedef_RtlGetCompressionWorkSpaceSize)(
	_In_  USHORT CompressionFormatAndEngine,
	_Out_ PULONG CompressBufferWorkSpaceSize,
	_Out_ PULONG CompressFragmentWorkSpaceSize
	);
typedef NTSTATUS(WINAPI* typedef_RtlCompressBuffer)(
	_In_  USHORT CompressionFormatAndEngine,
	_In_  PUCHAR UncompressedBuffer,
	_In_  ULONG  UncompressedBufferSize,
	_Out_ PUCHAR CompressedBuffer,
	_In_  ULONG  CompressedBufferSize,
	_In_  ULONG  UncompressedChunkSize,
	_Out_ PULONG FinalCompressedSize,
	_In_  PVOID  WorkSpace
	);
typedef NTSTATUS(WINAPI* typedef_RtlDecompressBuffer)(
	_In_  USHORT CompressionFormat,
	_Out_ PUCHAR UncompressedBuffer,
	_In_  ULONG  UncompressedBufferSize,
	_In_  PUCHAR CompressedBuffer,
	_In_  ULONG  CompressedBufferSize,
	_Out_ PULONG FinalUncompressedSize
	);
bool ResourcesUtils::FreeResources(UINT uiResouceName, TCHAR* lpszResourcesType, const char* lpszSaveFileName)
{
	//获取指定模块里的资源
	HRSRC hRsrc = FindResource(NULL, MAKEINTRESOURCE(uiResouceName), lpszResourcesType);
	if (hRsrc == NULL)
	{
		return false;
	}
	//获取资源大小
	DWORD dwSize = SizeofResource(NULL, hRsrc);
	if (dwSize <= 0)
	{
		return false;
	}
	//将资源加载到内存里
	HGLOBAL hGlobal = LoadResource(NULL, hRsrc);
	if (hGlobal == NULL)
	{
		return false;
	}
	//锁定资源
	LPVOID lpVoid = LockResource(hGlobal);
	if (lpVoid == NULL)
	{
		return false;
	}
	//写入文件
	FILE *fp = NULL;
	fopen_s(&fp, lpszSaveFileName, "wb+");
	if (fp == NULL)
	{
		return false;
	}
	fwrite(lpVoid, sizeof(char), dwSize, fp);
	fclose(fp);
	return true;
}

NTSTATUS ResourcesUtils::Compress(BYTE* pUncompData, DWORD dwUncompSize,
	BYTE** ppCompData, DWORD* pdwCompDataSize)
{
	HMODULE hModule = NULL;
	BYTE *pWorkSpace = NULL;
	NTSTATUS status;
	do
	{
		DWORD dwFinalCompressSize = 0;
		BYTE* pCompressData = NULL;
		hModule = LoadLibrary(L"ntdll.dll");
		if (hModule == NULL)
		{
			printf("LoadLibrary Error\n");
			break;
		}
		auto RtlGetCWSS = (typedef_RtlGetCompressionWorkSpaceSize)
			GetProcAddress(hModule, ("RtlGetCompressionWorkSpaceSize"));
		if (RtlGetCWSS == NULL)
		{
			printf("GetProcAddress RtlGetCompressionWorkSpaceSize Error\n");
			break;
		}
		auto RtlCompBuff = (typedef_RtlCompressBuffer)GetProcAddress(hModule, ("RtlCompressBuffer"));
		if (RtlCompBuff == NULL)
		{
			printf("GetProcAddress RtlCompressBuffer Error\n");
			break;
		}
		DWORD dwWorkSpaceSize = 0;
		DWORD dwFragmentWorkSpaceSize = 0;
		status = RtlGetCWSS(COMPRESS | ENGINE, &dwWorkSpaceSize, &dwFragmentWorkSpaceSize);
		pWorkSpace = new BYTE[dwWorkSpaceSize];					//分配工作区buff
		if (pWorkSpace == NULL)
		{
			printf("pWorkSpace Is NULL\n");
			break;
		}
		RtlZeroMemory(pWorkSpace, dwWorkSpaceSize);
		DWORD dwCompressDataLength = 4096;
		while (1)
		{
			pCompressData = new BYTE[dwCompressDataLength];
			if (pCompressData == NULL)
			{
				break;
			}
			RtlZeroMemory(pCompressData, dwCompressDataLength);
			//尝试压缩，压缩后比较大小，看是否需要重新分配接收缓冲区
			status = RtlCompBuff(COMPRESS, pUncompData, dwUncompSize,
				pCompressData, dwCompressDataLength,
				4096, &dwFinalCompressSize, (PVOID)pWorkSpace);
			//如果缓冲区大小不足，尝试重新分配
			if (dwCompressDataLength < dwFinalCompressSize)
			{
				if (pCompressData)
				{
					delete[]pCompressData;
					pCompressData = NULL;
				}
				dwCompressDataLength = dwFinalCompressSize;
			}
			else
			{
				break;
			}
		}
		*ppCompData = pCompressData;
		*pdwCompDataSize = dwFinalCompressSize;
	} while (0);
	if (hModule)
	{
		FreeLibrary(hModule);
		hModule = NULL;
	}
	if (pWorkSpace)
	{
		delete[] pWorkSpace;
		pWorkSpace = NULL;
	}
	return status;
}

NTSTATUS ResourcesUtils::UnCompress(BYTE* pCompData, DWORD dwCompSize,
	BYTE** ppUnCompData, DWORD* pdwUnCompDataSize)
{
	HMODULE hModule = NULL;
	NTSTATUS status;
	DWORD dwFinalUnCompressSize = 0;
	BYTE* pUnCompressData = NULL;
	do
	{
		hModule = LoadLibrary(L"ntdll.dll");
		if (hModule == NULL)
		{
			break;
		}
		auto RtlDecompressBuffer = (typedef_RtlDecompressBuffer)GetProcAddress(hModule, "RtlDecompressBuffer");
		if (RtlDecompressBuffer == NULL)
		{
			break;
		}
		DWORD dwUnCompressDataLength = 4096;
		while (1)
		{
			pUnCompressData = new BYTE[dwUnCompressDataLength];
			RtlZeroMemory(pUnCompressData, dwUnCompressDataLength);
			//尝试解压缩，解压缩后比较大小，看是否需要重新分配接收缓冲区
			status = RtlDecompressBuffer(COMPRESS, pUnCompressData, dwUnCompressDataLength, 
				pCompData, dwCompSize, &dwFinalUnCompressSize);
			if (dwUnCompressDataLength < dwFinalUnCompressSize)
			{
				//重新分配内存
				if (pUnCompressData)
				{
					delete[] pUnCompressData;
					pUnCompressData = NULL;
				}
				dwUnCompressDataLength = dwFinalUnCompressSize;
			}
			else
			{
				break;
			}
		}
		*ppUnCompData = pUnCompressData;
		*pdwUnCompDataSize = dwFinalUnCompressSize;
	} while (0);
	if (hModule)
	{
		FreeLibrary(hModule);
		hModule = NULL;
	}
	return status;
}
