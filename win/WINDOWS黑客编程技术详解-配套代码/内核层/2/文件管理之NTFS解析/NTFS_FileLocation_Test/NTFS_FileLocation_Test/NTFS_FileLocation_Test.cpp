// NTFS_FileLocation_Test.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "NtfsFileLocation.h"


int _tmain(int argc, _TCHAR* argv[])
{
	// ����·��
	printf("Input The File Path:\n");
	char szFilePath[MAX_PATH] = { 0 };
	gets(szFilePath);
	// �򿪴���
	HANDLE hFile = NULL;
	if (!OpenDisk(hFile, szFilePath))
	{
		return 1;
	}
	// ��ȡ������С(2)���ش�С(1)��$MFT��ʼ�غ�(8)
	WORD wSizeOfSector = 0;
	BYTE bSizeOfCluster = 0;
	LARGE_INTEGER liClusterNumberOfMFT;
	GetDataFromDBR(hFile, wSizeOfSector, bSizeOfCluster, liClusterNumberOfMFT);

	// ����$MFTԪ�ļ����ֽ�ƫ��
	LARGE_INTEGER liMFTOffset;
	liMFTOffset.QuadPart = liClusterNumberOfMFT.QuadPart * bSizeOfCluster * wSizeOfSector;

	// �����Ŀ¼����$MFT���5��Ŀ¼��ÿ��Ŀ¼��С�̶�Ϊ1KB(0x400)
	LARGE_INTEGER liRootOffset;
	liRootOffset.QuadPart = liMFTOffset.QuadPart + 5 * 0x400;

	// �ļ���λ
	LocationFile(hFile, szFilePath, wSizeOfSector, bSizeOfCluster, liMFTOffset, liRootOffset);
	// ��ʾ�߼��ֽ�ƫ�ƺ��ļ���
	printf("Location File:0x%llx\n", liRootOffset.QuadPart);

	// 80H���� ��ȡ�ļ���������ƫ��
	FileContentOffset(hFile, wSizeOfSector, bSizeOfCluster, liMFTOffset, liRootOffset);

	printf("\n");
	// �ر��ļ����˳�
	::CloseHandle(hFile);
	system("pause");
	return 0;
}

