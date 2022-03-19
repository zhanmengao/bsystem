#ifndef OSUTILS_H
#define OSUTILS_H

#include<unistd.h>
#include<fcntl.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<sys/types.h>
#include"../../../C++space/C语言基础/C_指针训练/BitUtils.h"

//O_RDONLY	O_WRONLY	O_RDWR
//O_CREAT创建，O_EXCL创建时检测文件是否存在
//O_APPEND:追加写	O_TRUNC覆盖写
//O_NONBLOCK非阻塞
static int Open(const char* filePath, int flags, mode_t mode)
{
    return open(filePath, O_RDWR | flags, mode);
}
static int CreatLink(const char* src,const char* sym)
{
    return symlink(src,sym);
}

static int Write(int fd,const void* rData,ssize_t rSize)
{
    return write(fd,rData,rSize);
}
static int Read(int fd,void* rData,ssize_t rSize)
{
    return read(fd,rData,rSize);
}
static int ReadLink(const char* path,char* buffer,ssize_t size)
{
    return readlink(path,buffer,size);
}

static int Getfl(int fd)
{
    return fcntl(fd,F_GETFL,0);
}
static int Setfl(int fd,int flags)
{
    unsigned fl = fcntl(fd,F_GETFL,0);
    AddBits(&fl,flags);
    return fcntl(fd,F_SETFL,fl);
}
static int Clrfl(int fd,int flags)
{
    unsigned fl = fcntl(fd,F_GETFL,0);
    RemoveBits(&fl,flags);
    return fcntl(fd,F_SETFL,fl);
}
static int SetFileNonBlock(int fd)
{
    return Setfl(fd,O_NONBLOCK);
}
static int SetFileBlock(int fd)
{
    return Clrfl(fd,O_NONBLOCK);
}
#endif // OSUTILS_H
