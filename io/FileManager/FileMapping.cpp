#include <windows.h>

INT64 Countos(void)
{
	SYSTEM_INFO sinf;
	GetSystemInfo(&sinf);
	HANDLE hFile = CreateFile(TEXT("d:/test.txt"), GENERIC_READ,
		FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, NULL);
	HANDLE hFileMapping = CreateFileMapping(hFile, NULL,
		PAGE_READONLY, 0, 0, NULL);
	DWORD dwFileSizeHigh;
	INT64 qwFileSize = GetFileSize(hFile, &dwFileSizeHigh);
	qwFileSize += (((_int64)dwFileSizeHigh) << 32);
	CloseHandle(hFile);
	INT64 qwFileoffset = 0, qwNumofos = 0;
	while (qwFileSize>0)
	{
		DWORD dwBytesInBlock = sinf.dwAllocationGranularity;
		if (qwFileSize<sinf.dwAllocationGranularity)
			dwBytesInBlock = (DWORD)qwFileSize;
		/////////////////////////////////////////////////////////////////////////
		/*
		�����ǰ�һ���ļ�ӳ�䵽���̵ĵ�ַ�ռ��е�ʱ�򣬲���һ����ӳ�������ļ�������
		ÿ��ֻ���ļ���һС����ӳ�䵽��ַ�ռ��С��ļ��б�ӳ�䵽���̵�ַ�ռ��еĲ���
		����Ϊ��ͼ
		*/
		/////////////////////////////////////////////////////////////////////////
		PBYTE pbFile = (PBYTE)MapViewOfFile(hFileMapping, FILE_MAP_READ,
			(DWORD)(qwFileoffset >> 32),
			(DWORD)(qwFileoffset & 0xFFFFFFFF),
			dwBytesInBlock);
		for (DWORD dwByte = 0; dwByte<dwBytesInBlock; dwByte++)
		{
			if (pbFile[dwByte] == '0')
				qwNumofos++;
		}
		UnmapViewOfFile(pbFile);
		qwFileoffset += dwBytesInBlock;
		qwFileSize -= dwBytesInBlock;
	}
	CloseHandle(hFileMapping);
	return (qwNumofos);
}