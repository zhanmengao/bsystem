#pragma once

#include<zconf.h>
#include<zlib.h>
#include <Windows.h>

//#pragma comment(lib,"legacy_stdio_definitions.lib")
#ifdef _DEBUG
	#ifdef _WIN64
			#pragma comment(lib,"zlibstaticd.lib")
	#else
			#pragma comment(lib,"zlibstaticd.lib")
	#endif

#else
	#ifdef _WIN64
			#pragma comment(lib,"zlibstatic.lib")
	#else
			#pragma comment(lib,"zlibstatic.lib")
	#endif
#endif

//数据压缩
 int ZlibCompress(const BYTE *pUnCompressData, DWORD dwUnCompressDataSize,
	BYTE **ppCompressData, DWORD *pdwCompressDataSize);

// 数据解压
int ZlibUnCompress(BYTE *pCompressData, DWORD dwCompressDataSize,
	BYTE **ppUncompressData, DWORD *pdwUncompressDataSize);


