#ifndef BLOG_H
#define BLOG_H

#pragma once
#include<string>
#include<fstream>
#include<iostream>

#include<set>
#include<time.h>
#ifdef __linux
#include<unistd.h>
#include<fcntl.h>
#include<dirent.h>
#include<sys/stat.h>
#else

#endif

using namespace std;

class LogManager
{
public:
    LogManager()
    {
        umask(002);
        //SetPath("");
    }
    LogManager(const string& logPath)
    {
        SetPath(logPath);
    }
    ~LogManager()
    {
        Close();
    }
    void SetPath(const string& logFolder)
    {
        Close();
        mLogFolder = logFolder;
        if(mkdir(logFolder.c_str(),0775)!=0)
        {
           perror(logFolder.c_str());
        }
        if(chmod(logFolder.c_str(),0775)!=0)
        {
           perror(logFolder.c_str());
        }
        CheckLogPath();
    }
    void SetTag(const std::string& tag)
    {
        mTag = tag;
    }
    void SetWriteStdout(bool bSend)
    {
        bSend2STDOUT = bSend;
    }
    void SetDebug(bool bSend)
    {
        bSendDebug = bSend;
    }

    void Log(const string& log)
    {
        CheckLogPath();
        Log(log, mfs);
    }
    void Log(const string& log, fstream& fs)
    {
        string str = BuildLogHead();
        if (fs.is_open())
        {
            fs << str << log << endl;
        }
        if (bSend2STDOUT)
        {
            cout << str << log << endl;
        }
        if (bSendDebug)
        {
#ifdef _WIN32
            OutputDebugStringA(log.c_str());
#endif
        }
    }
    void Close()
    {
        if (mfs.is_open())
        {
            mfs.flush();
            mfs.close();
        }
        if (mfsDaily.is_open())
        {
            mfsDaily.flush();
            mfsDaily.close();
        }
    }
    //记录到当天的日志
    void LogDaily(const string& log)
    {
        CheckLogPath();
        Log(log, mfsDaily);
    }
private:
    string BuildLogHead()
    {
        string str = GetLogTime();
        char buf[1024] = { 0 };
#ifdef _WIN32
        sprintf_s(buf, sizeof(buf), "%s %s:    ",BuildTag().c_str(), str.c_str());
#elif __linux
        sprintf(buf, "[%s]:    ", str.c_str());
#endif
        return buf;
    }
    string BuildTag()
    {
        return "[" + mTag + "]";
    }
    string GetMonthFolder()
    {
        return mLogFolder + "/"+ GetLogFolder() + "/";
    }
    string GetDailyPath()
    {
        return GetMonthFolder() + GetLogPath() + ".log";
    }
    string GetMonthLogPath()
    {
        return GetMonthFolder() + "log.txt";
    }
    string GetLogFolder()
    {
        char buffer[64] = {0};
        auto t = time(NULL);
        tm* tm = localtime(&t);
        strftime(buffer, sizeof(buffer), "%Y%m", tm);
        return buffer;
    }
    string GetLogPath()
    {
        char buffer[64] = {0};
        auto t = time(NULL);
        tm* tm = localtime(&t);
        strftime(buffer, sizeof(buffer), "%Y%m%d", tm);
        return buffer;
    }
    string GetLogTime()
    {
        char buffer[64] = {0};
        auto tm = GetCurrenttm();
        strftime(buffer, sizeof(buffer), "%Y_%m_%d %H:%M:%S", &tm);
        return buffer;
    }
    tm GetCurrenttm()
    {
        auto t = time(NULL);
        tm* tm = localtime(&t);
        return *tm;
    }
    void CheckLogPath()
    {
        tm current = GetCurrenttm();
        if(current.tm_year!=mLastTime.tm_year || current.tm_yday!=mLastTime.tm_yday)
        {
            if(mfs.is_open())
            {
                mfs.close();
            }
            if(mfsDaily.is_open())
            {
                mfsDaily.close();
            }
            std::string month = GetMonthFolder();
            if(mkdir(month.c_str(),0775)!=0)
            {
               perror(month.c_str());
            }
            if(chmod(month.c_str(),0775)!=0)
            {
               perror(month.c_str());
            }

            mfs.open(GetMonthLogPath().c_str(),ios::app | ios::in);
            if(chmod(GetMonthLogPath().c_str(),0775)!=0)
            {
               perror(GetMonthLogPath().c_str());
            }
            mfsDaily.open(GetDailyPath().c_str(),ios::app | ios::in);
            if(chmod(GetDailyPath().c_str(),0775)!=0)
            {
               perror(GetDailyPath().c_str());
            }
            mLastTime = current;
        }
    }
private:
    std::string mLogFolder;
    std::string mTag = "Default";
    fstream mfs;
    fstream mfsDaily;
    tm mLastTime = {0};
    bool bSend2STDOUT = false;
    bool bSendDebug = true;
};

#endif BLOG_H
