//
// Created by zhanmengao on 11/13/20.
//

#ifndef LDESKTOP_DESKTOPUTILS_H
#define LDESKTOP_DESKTOPUTILS_H
#include<fcntl.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#include"../../File/FileManager/LFileManager/PathHelper.h"
#include"../../File/FileManager/LFileManager/FileAttrHelper.h"

void sprintfDesktop(char* buffer,const char* appName,const char* exe,const char* icon,const char* GenericName,const char* Comment)
{
    sprintf(buffer,"%s\n%s%s\n%s%s\n%s%s\n%s%s\n%s\n%s\n%s\n%s\n",
            "[Desktop Entry]",
            "Name=",appName,
            "GenericName=",GenericName,
            "Comment=",Comment,
            "Exec=",exe,
            "Icon=",icon,
            "Terminal=false",
            "Type=Application",
            "StartupNotify=true"
            "Actions=new-window;");
}
int CreateDesktop(const char* appName,const char* exe,const char* icon,const char* GenericName,
                  const char* Comment,int onlycreate)
{
    /**
 * 创建桌面图标（快捷方式）
 * ====================================== */
    char buffer[1024] = {0};
    char filePath[256] = {0};
    GetDesktopFolder(filePath);
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

int CreateApplication(const char* appName,const char* exe,const char* icon,const char* GenericName,
                      const char* Comment,int onlycreate)
{
    char buffer[1024] = {0};
    char filePath[256] = {0};
    const char* appfolder = GetAppFolder();
    sprintf(filePath,"%s%s%s",appfolder,appName,".desktop");

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
#endif //LDESKTOP_DESKTOPUTILS_H
