//
// Created by zhanmengao on 11/13/20.
//

#ifndef LDESKTOP_AUTOSTARTUTILS_H
#define LDESKTOP_AUTOSTARTUTILS_H
#include"DesktopUtils.h"
int SetAutoStart(const char* appName,const char* exe,const char* icon,const char* GenericName,
                      const char* Comment,int onlycreate)
{
    char buffer[1024] = {0};
    char filePath[256] = {0};
    GetAutoStartFolder(filePath);
    strcat(filePath,appName);
    strcat(filePath,".desktop");

    int flag = O_TRUNC | O_CREAT | O_WRONLY;
    if(!onlycreate)
    {
        flag |= O_EXCL;
    }
    int fd = open(filePath,flag);
    if(fd == -1)
    {
        perror(filePath);
        return -1;
    }
    sprintfDesktop(buffer,appName,exe,icon,GenericName,Comment);
    int wn = write(fd,buffer,strlen(buffer));
    close(fd);
    SetFileExec(filePath);
    return wn;
}
#endif //LDESKTOP_AUTOSTARTUTILS_H
