#ifndef OSPROCESS_H
#define OSPROCESS_H
#include<string>

#ifdef _WIN32
#include<Windows.h>
#include"../../../../C++space/StringTools/StringTools/StringHead.h"
typedef DWORD pid_t;
#else
typedef unsigned long  DWORD;
typedef int HANDLE;
typedef bool BOOL;
#define stdstr std::string
#define INFINITE 0
#endif

class OSProcess
{
public:
    OSProcess();
    ~OSProcess();
public:
    bool Start();
    bool Alive();
    DWORD Wait(DWORD dwMilliseconds = INFINITE);
    void Exit();
public:
    //INIT
    void SetStartPath(const stdstr& processPath,const stdstr& command);
    void SetStartIO(HANDLE rHandle, HANDLE wHandle,HANDLE eHandle);
    DWORD ExitCode();
#ifdef _WIN32
    void SetStartShowWindow(bool bShow);
#else
#endif
public:
    pid_t GetPid();
    //Run
    BOOL SetRunPriority(DWORD fdwtPriority);
    DWORD GetRunPriority();

public:
    static pid_t GetSelf();
    static pid_t GetParentID();
#ifdef __linux
    static uid_t  GetLoginUserID();
    static uid_t  GetUserID();
#endif
private:

private:
#ifdef _WIN32
    PROCESS_INFORMATION mProcess = { 0 };
    STARTUPINFO mStart = { 0 };										//指定创建时进程的窗口工作站，桌面，标准句柄和主窗口的外观
#else
    __pid_t mPID = -1;
    int mRead;
    int mWrite;
    int mError;
    int status;
#endif
    stdstr mProcessPath;
    stdstr mCommand;
};

#endif
