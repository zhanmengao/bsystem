#ifndef _NTFS_FILE_LOCATION_H_
#define _NTFS_FILE_LOCATION_H_


#include <Windows.h>


// �򿪴���
BOOL OpenDisk(HANDLE &hFile, char *lpszDrive);

// ��DBR�л�ȡ���ݣ�ÿ�������ֽ�����ÿ���ص���������ԭ�ļ�$MFT����ʼ�غ�
BOOL GetDataFromDBR(HANDLE hFile, WORD &wSizeOfSector, BYTE &bSizeOfCluster, LARGE_INTEGER &liClusterNumberOfMFT);

// ��λ�ļ�
BOOL LocationFile(HANDLE hFile, char *lpszFileName, WORD wSizeOfSector, BYTE bSizeOfCluster, LARGE_INTEGER liMFTOffset, LARGE_INTEGER &liRootOffset);

// 0x90���ԵĴ���
BOOL HandleAttribute_90(BYTE *lpBuffer, WORD wAttributeOffset, BYTE *lpUnicode, DWORD dwLen, LARGE_INTEGER liMFTOffset, LARGE_INTEGER &liRootOffset);

// 0xA0���ԵĴ���
BOOL HandleAttribute_A0(HANDLE hFile, BYTE *lpBuffer, WORD wSizeOfSector, BYTE bSizeOfCluster, WORD wAttributeOffset, BYTE *lpUnicode, DWORD dwLen, LARGE_INTEGER liMFTOffset, LARGE_INTEGER &liRootOffset);

// �ڴ�Ƚ�
BOOL CompareMemory(BYTE *lpSrc, BYTE *lpDst, DWORD dwLen);

// ��ȡ�ļ�����ƫ��
BOOL FileContentOffset(HANDLE hFile, WORD wSizeOfSector, BYTE bSizeOfCluster, LARGE_INTEGER liMFTOffset, LARGE_INTEGER liRootOffset);


#endif