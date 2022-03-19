#ifndef ABSTRACTIPC_H
#define ABSTRACTIPC_H

#include<vector>
#include<string.h>
#ifdef _WIN32
#include<Windows.h>
#else
#include<unistd.h>
#include<fcntl.h>
#include<stdio.h>

typedef int HANDLE;

#endif

typedef std::vector<char> ByteArray;
class AbstractIPC
{
public:
    AbstractIPC() : mWrite(NULL), mRead(NULL)
    {
    }

    virtual int WriteData(const char* rData,size_t len)
    {
#ifdef _WIN32
        //---------写管道---------
        DWORD wn = 0;
        if (!WriteFile(mWrite, rData, len, &wn, NULL))
        {
            return -1;
        }
        else
        {
            return wn;
        }
#else
        return write(mWrite,rData,len);
#endif
    }
    virtual int WriteData(const ByteArray& data)
    {
#ifdef _WIN32
        //---------写管道---------
        DWORD wn = 0;
        if (!WriteFile(mWrite, data.data(), data.size(), &wn, NULL))
        {
            return -1;
        }
        else
        {
            return wn;
        }
#else
        if(data.size()<=0)
        {
            return 0;
        }
        return write(mWrite,data.data(),data.size());
#endif
    }
    virtual int ReadData(ByteArray& data, bool block = false)
    {
        if (block)
        {
            return ReadBlock(data);
        }
        else
        {
            return ReadNonBlock(data);
        }
    }
    virtual int ReadNonBlock(ByteArray& data)
    {
        int total = 0;
#ifdef _WIN32
        if (mRead == NULL)
        {
            return -1;
        }
        if (CheckPipe() > 0)
        {
            return ReadBlock(data);
        }
        return 0;
#else
        int flag = fcntl(mRead,F_GETFL);
        fcntl(mRead,F_SETFL,flag | O_NONBLOCK);
        return Read(data);
#endif
    }
    virtual int ReadBlock(ByteArray& data)
    {
        data.clear();
#ifdef __linux
        int flag = fcntl(mRead,F_GETFL);
        flag &= (~O_NONBLOCK);
        fcntl(mRead,F_SETFL,flag);
#endif
        return Read(data);
    }
    virtual int Read(ByteArray& data)
    {
        int total = 0;
        int rn = 0;
        //将管道读完
        do
        {
            memset(rbuf, 0, sizeof(rbuf));
#ifdef _WIN32
            rn = CheckPipe();
            if (!ReadFile(mRead, rbuf, rn,NULL, NULL))
            {
                break;
            }
#else
            rn = read(mRead,rbuf,sizeof(rbuf));
            if(rn <= 0)
            {
                perror("pipe read");
                break;
            }
#endif
            else
            {
                data.resize(total + rn);
                for (int i = 0; i < rn; i++)
                {
                    data[total + i] = rbuf[i];
                }
                total +=rn;
#ifdef _WIN32
                rn = CheckPipe();
#else
                if(rn < sizeof(rbuf))
                {
                    rn = 0;
                }
#endif
            }
        } while (rn > 0);
        return total;
    }
#ifdef _WIN32
    int CheckPipe()
    {
        DWORD r = 0;
        if (!PeekNamedPipe(mRead, NULL, NULL, NULL, &r, 0))	//管道是否有数据可读
        {
        }
        return r;
    }
#endif
protected:
    HANDLE mWrite;
    HANDLE mRead;
    char rbuf[4096] = { 0 };
};

#endif
