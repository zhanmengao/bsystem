#ifndef _WININET_FTP_H_
#define _WININET_FTP_H_


#include <Windows.h>
#include <WinInet.h>
#pragma comment(lib, "WinInet.lib")


/*
    FTP��URL��ʽ��������ʾ��

	    FTP://�˺�:����@����/��Ŀ¼���ļ�     // ����: ftp://admin:123456@192.168.0.1/mycode/520.zip
    
	ע��:��Ҫʹ������
*/

/*
	��ǰ��򿪻Ự�ͽ������Ӻ�HTTP�Լ�HTTPSһ��;
	֮���FTP�ļ�����, ��ʹ��FTPOpenFile��ȡ�ļ����֮��, ���ļ��Ĳ�������ͬ���ز���һ����.
*/



// �����ݴ洢Ϊ�ļ�
// ���룺����ԭ�ļ�·������Ҫ������������ݡ���Ҫ������������ݳ���
BOOL Ftp_SaveToFile(char *pszFileName, BYTE *pData, DWORD dwDataSize);

// URL�ֽ�
// ���룺URL
// �����szScheme��szHostName��szUserName��szPassword��szUrlPath��szExtraInfo
BOOL Ftp_UrlCrack(char *pszUrl, char *pszScheme, char *pszHostName, char *pszUserName, char *pszPassword, char *pszUrlPath, char *pszExtraInfo, DWORD dwBufferSize);

// ��������
// ���룺�������ݵ�URL·��
// ����������������ݡ������������ݳ���
BOOL FTPDownload(char *pszDownloadUrl, BYTE **ppDownloadData, DWORD *pdwDownloadDataSize);

// �����ϴ�
// ���룺�ϴ����ݵ�URL·�����ϴ��������ݡ��ϴ��������ݳ���
BOOL FTPUpload(char *pszUploadUrl, BYTE *pUploadData, DWORD dwUploadDataSize);


#endif