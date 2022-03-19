#include"ZLIBUtils.h"
#include<stdio.h>

//数据压缩
int ZlibCompress(const BYTE *pUnCompressData, DWORD dwUnCompressDataSize,
	BYTE **ppCompressData, DWORD *pdwCompressDataSize)
{
	int ret = 0;
	do
	{
		DWORD dwBuffSize = 4096;
		BYTE* pDestData = NULL;
		while (1)
		{
			pDestData = new BYTE[dwBuffSize];
			memset(pDestData, 0, sizeof(BYTE) * dwBuffSize);
			ret = compress(pDestData, &dwBuffSize, pUnCompressData, dwUnCompressDataSize);
			if (ret == -5)
			{
				//缓冲区不够大
				if (pDestData)
				{
					delete[] pDestData;
					pDestData = NULL;
				}
				dwBuffSize += 100 * 1024;
			}
			else
			{
				break;
			}
		}
		*ppCompressData = pDestData;
		*pdwCompressDataSize = dwBuffSize;
	} while (false);
	return ret;
}

// 数据解压
int ZlibUnCompress(BYTE *pCompressData, DWORD dwCompressDataSize,
	BYTE **ppUncompressData, DWORD *pdwUncompressDataSize)
{
	int ret = 0;
	do
	{
		DWORD dwBuffSize = 4096;
		BYTE* pDestData = NULL;
		while (1)
		{
			pDestData = new BYTE[dwBuffSize];
			memset(pDestData, 0, sizeof(BYTE) * dwBuffSize);
			ret = uncompress(pDestData, &dwBuffSize, pCompressData, dwCompressDataSize);
			if (ret == -5)
			{
				//缓冲区不够大
				if (pDestData)
				{
					delete[] pDestData;
					pDestData = NULL;
				}
				dwBuffSize += 100 * 1024;
			}
			else
			{
				break;
			}
		}
		*ppUncompressData = pDestData;
		*pdwUncompressDataSize = dwBuffSize;
	} while (false);
	return ret;
}


#ifdef __cplusplus

#endif