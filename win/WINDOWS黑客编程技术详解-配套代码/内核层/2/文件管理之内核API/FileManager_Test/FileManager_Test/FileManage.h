#ifndef _FILE_MANAGE_H_
#define _FILE_MANAGE_H


#include <ntifs.h>


// �����ļ�
BOOLEAN MyCreateFile(UNICODE_STRING ustrFilePath);

// ����Ŀ¼
BOOLEAN MyCreateFileFolder(UNICODE_STRING ustrFileFolderPath);

// ɾ���ļ����ǿ�Ŀ¼
BOOLEAN MyDeleteFileOrFileFolder(UNICODE_STRING ustrFileName);

// ��ȡ�ļ���С
ULONG64 MyGetFileSize(UNICODE_STRING ustrFileName);

// �������ļ����ļ���
BOOLEAN MyRenameFileOrFileFolder(UNICODE_STRING ustrSrcFileName, UNICODE_STRING ustrDestFileName);

// �����ļ��к��ļ�
BOOLEAN MyQueryFileAndFileFolder(UNICODE_STRING ustrPath);

// ��ȡ�ļ�����
BOOLEAN MyReadFile(UNICODE_STRING ustrFileName, LARGE_INTEGER liOffset, PUCHAR pReadData, PULONG pulReadDataSize);

// ���ļ�д������
BOOLEAN MyWriteFile(UNICODE_STRING ustrFileName, LARGE_INTEGER liOffset, PUCHAR pWriteData, PULONG pulWriteDataSize);

// �ļ�����
BOOLEAN MyCopyFile(UNICODE_STRING ustrScrFile, UNICODE_STRING ustrDestFile);


#endif