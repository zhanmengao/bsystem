//
// Created by uzi on 11/24/20.
//

#ifndef PIPEHELPER_PIPEHELPER_H
#define PIPEHELPER_PIPEHELPER_H
#include <unistd.h>
#include<string>
#include<vector>
#include"AbstractIPC.h"


class IPCAnon:public AbstractIPC
{
public:
    IPCAnon();
    ~IPCAnon();
    bool Init(int &nRead, int& nWrite);
    void Destroy();
    virtual int ReadData(ByteArray& data, bool block = true);
private:
    int nWrite;
    int nRead;
};

class IPCName:public AbstractIPC
{
public:
    IPCName();
    ~IPCName();
    bool InitServer(const std::string& wFifo,const std::string& rFifo);
    bool InitClient(const std::string& wFifo,const std::string& rFifo);
    virtual int ReadData(ByteArray& data, bool block = true);
    void Close();
};
#endif //PIPEHELPER_PIPEHELPER_H
