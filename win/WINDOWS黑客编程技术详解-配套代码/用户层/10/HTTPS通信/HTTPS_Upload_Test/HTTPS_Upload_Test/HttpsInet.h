#ifndef _HTTPS_INET_H_
#define _HTTPS_INET_H_


#include <Windows.h>
#include <WinInet.h>
#pragma comment(lib, "WinInet.lib")


// URL�ֽ�
// ���룺URL������������������С
// �����szScheme��szHostName��szUserName��szPassword��szUrlPath��szExtraInfo
BOOL Https_UrlCrack(char *pszUrl, char *pszScheme, char *pszHostName, char *pszUserName, char *pszPassword, char *pszUrlPath, char *pszExtraInfo, DWORD dwBufferSize);


// ����Ӧ��Ϣͷ��Ϣ�л�ȡ�������ݳ��ȴ�С
// ���룺��Ӧ��Ϣͷ��Ϣ����
// ������������ݳ��ȴ�С
BOOL Https_GetContentLength(char *pResponseHeader, DWORD *pdwContentLength);

// ��������
// ���룺�������ݵ�URL·��
// ����������������ݡ������������ݳ���
BOOL Https_Download(char *pszDownloadUrl, BYTE **ppDownloadData, DWORD *pdwDownloadDataSize);


// �����ϴ�
// ���룺�ϴ����ݵ�URL·�����ϴ��������ݡ��ϴ��������ݳ���
BOOL Https_Upload(char *pszUploadUrl, BYTE *pUploadData, DWORD dwUploadDataSize);


// �����ݴ洢Ϊ�ļ�
// ���룺����ԭ�ļ�·������Ҫ������������ݡ���Ҫ������������ݳ���
BOOL Https_SaveToFile(char *pszFileName, BYTE *pData, DWORD dwDataSize);


#endif