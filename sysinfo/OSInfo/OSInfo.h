#pragma once
#include<string.h>
#include<string>
#ifdef _WIN32
#include"../SystemInfo/WMI/WMI.h"

std::string GetOSFullName()
{
	std::vector<WMIQueryData> vec;
	CWMI::GetWin32_OS(vec);
	if (vec.size() > 0)
	{
		auto fit = vec[0].find(_T("Caption"));
		if (fit != vec[0].end())
		{
			return fit->second.data.data;
		}
	}
	return "";
}
#else
#include<unistd.h>
#include<stdio.h>
#include<wait.h>
#include<sys/utsname.h>
char* GetUser()
{
    return getlogin();
}
void GetHostName(char* buffer,size_t nsize)
{
    int fd[2];
    int ret = pipe(fd);
    if(ret == -1)
    {
        perror("pipe");
        return;
    }
    pid_t pid = fork();
    if(pid == 0)
    {
        close(fd[0]);
        dup2(fd[1],STDOUT_FILENO);
        execlp("hostname","hostname",NULL);
    }
    else
    {
        close(fd[1]);
        read(fd[0],buffer,nsize);
    }
    pid = waitpid(-1,NULL,WNOHANG);
    if(pid)
    {
        printf("gc child %d\n", pid);
    }
}

void GetLinuxVersion(char *buffer)
{
    FILE *fp = fopen("/proc/version", "r");
    if(NULL == fp)
        printf("failed to open version\n");
    char szTest[1000] = {0};
    while(!feof(fp))
    {
        memset(szTest, 0, sizeof(szTest));
        fgets(szTest, sizeof(szTest) - 1, fp); // leave out \n
        strcat(buffer,szTest);
    }
    fclose(fp);
}

struct OSInfo
{
    std::string sysName;
    std::string nodeName;
    std::string release;
    std::string version;
    std::string machine;
};

OSInfo GetOSInfo()
{
    struct utsname name;
    uname(&name);
    OSInfo info;
    info.machine = name.machine;
    info.release = name.release;
    info.sysName = name.sysname;
    info.version = name.version;
    info.nodeName = name.nodename;
    return info;
}

#endif
