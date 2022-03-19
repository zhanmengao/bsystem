#ifndef _OSUTILS_H
#define _OSUTILS_H
#pragma once
#include <stdio.h>
#ifdef _WIN32
#include<Windows.h>
#include<tchar.h>
#include"../FileManager/FileManager/FileAttrHelper.h"

class OSFile
{
public:
	//创建新的文件，有则打开
	bool Create(const TCHAR* filePath,DWORD dwOpen= OPEN_ALWAYS,
		DWORD dwShare = FILE_SHARE_READ,
		SECURITY_ATTRIBUTES *psa = nullptr)
	{
		hFile =  CreateFile(filePath, GENERIC_ALL, dwShare, psa, dwOpen, FILE_ATTRIBUTE_NORMAL, NULL);
		return hFile != INVALID_HANDLE_VALUE;
	}
	//打开已有的文件
	bool Open(const TCHAR* filePath)
	{
		return Create(filePath, OPEN_EXISTING);
	}
	static DWORD Write(HANDLE hFile,void* rData,DWORD dwSize)
	{
		DWORD dwWriten = 0;
		BOOL bRet  = WriteFile(hFile, rData, dwSize,&dwWriten,NULL);
		return dwWriten;
	}
	static DWORD Read(HANDLE hFile, void* rBuffer, DWORD dwSize)
	{
		DWORD dwRead = 0;
		BOOL bRet = ReadFile(hFile, rBuffer, dwSize, &dwRead, NULL);
		return dwRead;
	}
	DWORD Size()
	{
		return GetFileSize(hFile, 0);
	}
	HANDLE GetFileHandle()
	{
		return hFile;
	}
	DWORD GetACL(PSID* psid,PSID* pgid,PACL* dacl,PACL* sacl, PSECURITY_DESCRIPTOR* sd)
	{
		DWORD dwret = GetSecurityInfo(hFile, SE_OBJECT_TYPE::SE_FILE_OBJECT,
			OWNER_SECURITY_INFORMATION | GROUP_SECURITY_INFORMATION |
			DACL_SECURITY_INFORMATION | SACL_SECURITY_INFORMATION,
			psid, pgid, dacl, sacl, sd);
		if (dwret != ERROR_SUCCESS)
		{
			perror("GetSecurityInfo");
		}
		return dwret;
	}
	PSID GetOwner()
	{
		PSID sid;
		PSID gid;
		PACL dacl;
		PACL sacl;
		PSECURITY_DESCRIPTOR sd;
		DWORD dwret = GetSecurityInfo(hFile, SE_OBJECT_TYPE::SE_FILE_OBJECT, 
			OWNER_SECURITY_INFORMATION | GROUP_SECURITY_INFORMATION | 
			DACL_SECURITY_INFORMATION,
			&sid, &gid, &dacl, NULL, &sd);
		if (dwret != ERROR_SUCCESS)
		{
			perror("GetSecurityInfo");
		}
		return sid;
	}
	PSID GetGroup()
	{
		PSID gid;
		DWORD dwret = GetSecurityInfo(hFile, SE_OBJECT_TYPE::SE_FILE_OBJECT,
			GROUP_SECURITY_INFORMATION,
			NULL, &gid, NULL, NULL, NULL);
		if (dwret != ERROR_SUCCESS)
		{
			perror("GetSecurityInfo");
		}
		return gid;
	}
	PACL GetDACL()
	{
		PACL dacl;
		DWORD dwret = GetSecurityInfo(hFile, SE_OBJECT_TYPE::SE_FILE_OBJECT,
			DACL_SECURITY_INFORMATION,
			NULL,NULL, &dacl,NULL,NULL);
		if (dwret != ERROR_SUCCESS)
		{
			perror("GetSecurityInfo");
		}
		return dacl;
	}
	PACL GetSACL()
	{
		PACL sacl;
		DWORD dwret = GetSecurityInfo(hFile, SE_OBJECT_TYPE::SE_FILE_OBJECT,
			SACL_SECURITY_INFORMATION,
			NULL, NULL, NULL, &sacl, NULL);
		if (dwret != ERROR_SUCCESS)
		{
			perror("GetSecurityInfo");
		}
		return sacl;
	}
	PSECURITY_DESCRIPTOR GetSecurityDesc()
	{
		PSECURITY_DESCRIPTOR sd;
		DWORD dwret = GetSecurityInfo(hFile, SE_OBJECT_TYPE::SE_FILE_OBJECT,
			NULL,
			NULL, NULL, NULL, NULL, &sd);
		if (dwret != ERROR_SUCCESS)
		{
			perror("GetSecurityInfo");
		}
		return sd;
	}
private:
	HANDLE hFile = INVALID_HANDLE_VALUE;
	//#define CREATE_NEW          只有在不存在的情况下才创建一个新文件。如果指定的文件存在，则该函数失败。最后的错误代码被设置为ERROR_FILE_EXISTS（80）。
	//#define CREATE_ALWAYS       始终创建一个新文件。如果指定的文件存在并且是可写的，则该函数覆盖文件，函数成功，并且最后的错误代码设置为ERROR_ALREADY_EXISTS（183）。
	//#define OPEN_EXISTING       只有存在文件或设备时才打开。如果指定的文件或设备不存在，则该函数将失败
	//#define OPEN_ALWAYS      总是打开一个文件。如果指定的文件存在，则函数成功并且最后的错误代码被设置为ERROR_ALREADY_EXISTS(183).
	//如果指定的文件不存在并且是可写位置的有效路径，则该函数将创建一个文件，并将最后一个错误代码设置为零。
	//#define TRUNCATE_EXISTING   打开一个文件并截断它，以便它的大小为零字节，只要它存在。如果指定的文件不存在，则该函数失败。


	//0x00000000 	防止其他进程在请求删除，读取或写入访问时打开文件或设备。
	//FILE_SHARE_DELETE	0x00000004
	//启用文件或设备共享删除访问，否则，如果其他进程请求删除访问，则无法打开该文件或设备。如果未指定此标志，
	//但文件或设备已被打开以进行删除访问，则该功能失败。注意：除访问权限允许删除和重命名操作
	//FILE_SHARE_READ	0x00000001
	//启用文件或设备共享读访问，否则，如果其他进程请求读取访问权限，则无法打开文件或设备。
	//如果未指定此标志，但文件或设备已被打开以进行读取访问，则该功能失败。
	//FILE_SHARE_WRITE	0x00000002
	//启用文件或设备共享写访问，否则，如果其他进程请求写访问权限，则无法打开该文件或设备。
	//如果未指定此标志，但文件或设备已打开以进行写入访问或具有写入访问的文件映射，则该功能将失败。
};
#else
#include"OSUtils.h"

class OSFile
{
public:
    //创建新的文件，有则打开
    bool Create(const char *filePath, bool bOnCreate = false, bool bAppend = false, bool bBlock = true);
    //打开已有的文件
    bool Open(const char *filePath, bool bAppend = false, bool bBlock = true);
    int Write(void *rData, ssize_t rSize);

    int Read(void *rBuffer, ssize_t rSize);
    int Futimens(const struct timespec times[2]);
    int WriteSeek(const void* rData,ssize_t rSize,off_t offset);
    int ReadSeek(void* rData,ssize_t rSize,off_t offset);
    int IsCanSeek();
    int SetSeek(off_t offset,int where);
    __off_t ResizeBiger(ssize_t bigger);
    int Resizefd(off_t  length);
    //file must exist
    int ResizeFile(const char* path, off_t  length);
    FILE* GetFILE(const char* mode);
    int GetFILENO(FILE* pFile);
    int Getfl();
    int Setfl(int flags);
    int Clrfl(int flags);
    int SetFileNonBlock();
    int SetFileBlock();
    int SetSync();
    int IsReadOnly();
    int IsWriteOnly();
    int IsReadWrite();
    int IsCanRead();
    int IsCanWrite();
    int IsSync();
    int IsAppend();

    int ReSTDIN();
    int ReSTDOUT();
    int Copyfd();
    int Copyfd2(int fd2);
private:
    int fd = -1;
};
#endif
#endif
