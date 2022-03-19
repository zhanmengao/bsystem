//
// Created by uzi on 11/24/20.
//
#include"PipeHelper.h"
#include<fcntl.h>
#include<sys/stat.h>
IPCAnon::IPCAnon()
{
}
IPCAnon::~IPCAnon()
{
    Destroy();
}
bool IPCAnon::Init(int &nRead, int& nWrite)
{
    int fd[2];
    int ret = -1;
    do
    {
        ret = pipe(fd);
        if(ret == -1)
        {
            perror("pipe");
            break;
        }
        mRead = fd[0];
        this->nWrite = fd[1];
        ret = pipe(fd);
        if(ret == -1)
        {
            perror("pipe");
            break;
        }
        this->nRead = fd[0];
        mWrite = fd[1];
    }while(0);
    if(ret == -1)
    {
        Destroy();
    }
    else
    {
        nRead = this->nRead;
        nWrite = this->nWrite;
    }
    return ret >0;
}
int IPCAnon::ReadData(ByteArray& data, bool block)
{
    return AbstractIPC::ReadData(data,block);
}
void IPCAnon::Destroy()
{
    close(nRead);
    close(mRead);
    close(nWrite);
    close(mWrite);
}


IPCName::IPCName()
{

}
IPCName::~IPCName()
{

}
bool IPCName::InitServer(const std::string& wFifo,const std::string& rFifo)
{
#define FILE_MODEL (S_IRUSR | S_IWUSR | S_IRGRP | S_IWOTH)
    bool bRet = true;
    printf("InitServer w = %s r = %s \n",wFifo.c_str(),rFifo.c_str());
    do
    {
        if(mkfifo(rFifo.c_str(),FILE_MODEL)==-1)
        {
            if(errno!=EEXIST)
            {
                perror("mkpipe1 error");
                bRet = false;
                break;
            }
        }
        mRead = open(rFifo.c_str(),O_RDONLY);
        if(mRead <0)
        {
            bRet = false;
            break;
        }
        unlink(rFifo.c_str());
        mWrite = open(wFifo.c_str(),O_WRONLY);
        if(mWrite <0)
        {
            bRet = false;
            break;
        }

    }while(0);
    if(!bRet)
    {
        close(mRead);
        close(mWrite);
    }
    return bRet;
}
bool IPCName::InitClient(const std::string& wFifo,const std::string& rFifo)
{
#define FILE_MODEL (S_IRUSR | S_IWUSR | S_IRGRP | S_IWOTH)
    printf("InitClient w = %s r = %s \n",wFifo.c_str(),rFifo.c_str());
    bool bRet = true;
    do
    {
        if(mkfifo(rFifo.c_str(),FILE_MODEL)==-1)
        {
            if(errno!=EEXIST)
            {
                perror("mkpipe1 error");
                bRet = false;
                break;
            }
        }
        mWrite = open(wFifo.c_str(),O_WRONLY);
        if(mWrite <0)
        {
            bRet = false;
            break;
        }
        mRead = open(rFifo.c_str(),O_RDONLY);
        if(mRead <0)
        {
            bRet = false;
            break;
        }
        unlink(rFifo.c_str());
    }while(0);
    if(!bRet)
    {
        close(mRead);
        close(mWrite);
    }
    return bRet;
}

void IPCName::Close()
{
    close(mRead);
    close(mWrite);
}
int IPCName::ReadData(ByteArray& data, bool block)
{
    return AbstractIPC::ReadData(data,block);
}
