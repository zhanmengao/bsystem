/*
 * Environ.c
 *
 *  Created on: 2018年5月25日
 *      Author: uzi
 */
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include<sys/utsname.h>



//获取主机名
int GetHostName(char* buf,unsigned bufSize)
{
	return gethostname(buf,bufSize);
}

int GetUTSInfo(struct utsname *pInfo)
{
	return uname(pInfo);
}

int GetSystemName(char* buf,unsigned bufSize)
{
	struct utsname info;
	int ret = GetUTSInfo(&info);
	//检查缓冲区大小
	if(bufSize < strlen(info.sysname) + 1)
	{
		return -1;
	}	
	strcpy(buf,info.sysname);
	return ret;
}

//当前设备在网络上的名称
int GetNodeName(char* buf,unsigned bufSize)
{
	struct utsname info;
	int ret = GetUTSInfo(&info);
	//检查缓冲区大小
	if(bufSize < strlen(info.nodename) + 1)
	{
		return -1;
	}
	strcpy(buf,info.nodename);
	return ret;
}

//系统发布级别
int GetReleaseLevel(char* buf,unsigned bufSize)
{
	struct utsname info;
	int ret = GetUTSInfo(&info);
	//检查缓冲区大小
	if(bufSize < strlen(info.release) + 1)
	{
		return -1;
	}
	strcpy(buf,info.release);
	return ret;
}

//系统发布版本
int GetVersion(char* buf,unsigned bufSize)
{
	struct utsname info;
	int ret = GetUTSInfo(&info);
	//检查缓冲区大小
	if(bufSize < strlen(info.version) + 1)
	{
		return -1;
	}
	strcpy(buf,info.version);
	return ret;
}

//获取硬件体系
int GetMachine(char* buf,unsigned bufSize)
{
	struct utsname info;
	int ret = GetUTSInfo(&info);
	//检查缓冲区大小
	if(bufSize < strlen(info.machine) + 1)
	{
		return -1;
	}
	strcpy(buf,info.machine);
	return ret;
}

int GetDomainName(char* buf,unsigned bufSize)
{
	struct utsname info;
	int ret = GetUTSInfo(&info);
	//检查缓冲区大小
	if(bufSize < strlen(info.__domainname) + 1)
	{
		return -1;
	}
	strcpy(buf,info.__domainname);
	return ret;
}

