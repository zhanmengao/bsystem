#ifndef _ZLIB_COMPRESS_H_
#define _ZLIB_COMPRESS_H_

#include <Windows.h>
#include <Shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")
//*************************************************
//         zlibѹ�����ͷ�ļ��;�̬��
//*************************************************
#include "zlib\\zconf.h"
#include "zlib\\zlib.h"
#ifdef _DEBUG
	#ifdef _WIN64
		#pragma comment(lib, "zlib\\x64\\debug\\zlibstat.lib")
	#else
		#pragma comment(lib, "zlib\\x86\\debug\\zlibstat.lib")
	#endif
#else
	#ifdef _WIN64
		#pragma comment(lib, "zlib\\x64\\release\\zlibstat.lib")
	#else
		#pragma comment(lib, "zlib\\x86\\release\\zlibstat.lib")
	#endif
#endif
//*************************************************


#define MAX_SRC_FILE_SIZE (100*1024*1024)			// �����ļ����ƴ�СΪ 100M 


// ����ѹ��
// ���룺��Ҫѹ���ļ���·��
// ���������ѹ�����ѹ���������ݡ�����ѹ�����ѹ���������ݳ���
BOOL Zlib_CompressData(char *pszCompressFileName, BYTE **ppCompressData, DWORD *pdwCompressDataSize);


// ���ݽ�ѹ
// ���룺��Ҫ��ѹ���ļ���·��
// ��������ݽ�ѹ����������ݡ����ݽ�ѹ������ݳ���
BOOL Zlib_UncompressData(char *pszUncompressFileName, BYTE **ppUncompressData, DWORD *pdwUncompressDataSize);


// ���ݽ�ѹ
// ���룺��Ҫ��ѹ�����������ݡ���Ҫ��ѹ�����������ݳ���
// ��������ݽ�ѹ����������ݡ����ݽ�ѹ������ݳ���
BOOL Zlib_UncompressData(BYTE *pCompressData, DWORD dwCompressDataSize, BYTE **ppUncompressData, DWORD *pdwUncompressDataSize);


// �����ݴ洢Ϊ�ļ�
// ���룺����ԭ�ļ�·������Ҫ������������ݡ���Ҫ������������ݳ���
BOOL SaveToFile(char *pszFileName, BYTE *pData, DWORD dwDataSize);


// �����ݴ洢Ϊ�ļ�
// ���룺����ԭ�ļ�·������Ҫ������������ݡ���Ҫ������������ݳ���
BOOL SaveToOriginalFile(char *pszFileName, BYTE *pData, DWORD dwDataSize);

#endif