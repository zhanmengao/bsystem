#include <iostream>
#include "PipeHelper.h"
#include"../../Process/WindowsProcessHelper/Fork/ChildProcess.h"
#include<fcntl.h>
#include<sys/stat.h>
#define SERVER_WRITE "/tmp/fctrlsw"
#define SERVER_READ "/tmp/fctrlsr"
int main1()
{
    IPCName mFifo;
    mFifo.InitClient(SERVER_READ, SERVER_WRITE);
    ByteArray fifoBuf;
    while(1)
    {
        if (mFifo.ReadData(fifoBuf))
        {
            std::string str;
            str.resize(fifoBuf.size());
            for(int i = 0;i<fifoBuf.size();i++)
            {
                str[i] = fifoBuf[i];
            }
            std::cout<<"read fifo"<<str<<std::endl;
        }
    }
}
#define FILE_MODEL (S_IRUSR | S_IWUSR | S_IRGRP | S_IWOTH)
int main33()
{
    if(mkfifo(SERVER_READ,FILE_MODEL)==-1)
    {
        if(errno!=EEXIST)
        {
            perror("mkpipe1 error");
        }
    }
    int r = open(SERVER_WRITE,O_RDONLY);
    char buffer[1024] = {0};
    while(1)
    {
        read(r,buffer,sizeof(buffer));
        printf("read %s \n",buffer);
    }

}
int main2()
{
    IPCAnon ipc;
    int nread,nwrite;
    ipc.Init(nread,nwrite);
    OSProcess process;
    const char* argv = "ls -l";
    process.SetStartIO(nread,nwrite,nwrite);
    process.SetStartPath("ls",argv);
    bool bRet = process.Start();
    printf("start : %d pid = %d \n" ,bRet,process.GetPid());
    close(nread);
    close(nwrite);
    ByteArray  bytearr;
    ipc.ReadData(bytearr,true);
    printf("recv %s \n" ,bytearr.data());
    process.Wait();
    return 0;
}

int main()
{
    return main1();
}
