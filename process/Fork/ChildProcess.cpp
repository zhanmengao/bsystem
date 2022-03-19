#include"ChildProcess.h"
#ifdef _WIN32
#include<tchar.h>
#include<iostream>
#include"ForkUtils.h"
#include"../CreateProcess/ProcessHelper.h"
#else
#include<unistd.h>
#include<vector>
#include<wait.h>
#include<string.h>
#include<sys/resource.h>
#endif

OSProcess::OSProcess()
{
#ifdef _WIN32
    GetStartupInfo(&mStart);
    mStart.cb = sizeof(STARTUPINFO);
    mStart.lpReserved = NULL;
#endif
}
void OSProcess::SetStartPath(const stdstr& processPath, const stdstr& command)
{
    mProcessPath = processPath;
    mCommand = command;
}
void OSProcess::SetStartIO(HANDLE rHandle, HANDLE wHandle, HANDLE eHandle)
{
#ifdef _WIN32
    ForkUtils::SetStartIO(mStart, rHandle, wHandle, eHandle);
#else
    mRead = rHandle;
    mWrite = wHandle;
    mError = eHandle;
#endif
}

bool OSProcess::Start()
{
#ifdef _WIN32
    //启动信息
    BOOL bRet = FALSE;
    TCHAR *processPath = nullptr;
    TCHAR *cmd = nullptr;
    do
    {
        if (!mCommand.empty())
        {
            size_t buflen = mCommand.size() * 1.2f + sizeof(TCHAR);
            cmd = new TCHAR[buflen];
            memset(cmd, 0, buflen);
            _tcscpy_s(cmd, buflen, mCommand.c_str());
        }

        if (!mProcessPath.empty())
        {
            size_t buflen = mProcessPath.size() * 1.2f + sizeof(TCHAR);
            processPath = new TCHAR[buflen];
            memset(processPath, 0, buflen);
            _tcscpy_s(processPath, buflen, mProcessPath.c_str());
        }

        bRet = CreateProcess(processPath, cmd, NULL, NULL, TRUE, NULL, NULL, NULL, &mStart, &mProcess);
    } while (false);
    if (mProcess.hThread)
    {
        CloseHandle(mProcess.hThread);
        mProcess.hThread = NULL;
    }
    if (processPath != nullptr)
    {
        delete[] processPath;
        processPath = nullptr;
    }
    if (cmd != nullptr)
    {
        delete[] cmd;
        cmd = nullptr;
    }
    return bRet;
#else
    mPID = fork();
    if (mPID == 0)
    {
        if (mRead >= 0)
        {
            dup2(mRead, STDIN_FILENO);
        }
        if (mWrite >= 0)
        {
            dup2(mWrite, STDOUT_FILENO);
        }
        if (mError >= 0)
        {
            dup2(mError, STDERR_FILENO);
        }
        //printf("I am child");
        std::vector<std::string> parameters;
        size_t nPos = 0;
        while (nPos < mCommand.size())
        {
            size_t current = mCommand.find(' ', nPos);
            std::string s = mCommand.substr(nPos, current - nPos);
            nPos = current + 1;
            parameters.push_back(s);
            if (current == std::string::npos)
            {
                break;
            }
        }
        int argc = parameters.size();
        char** argv = new char*[argc + 1];
        for (size_t i = 0; i < parameters.size(); i++)
        {
            argv[i] = new char[parameters[i].size() + 1];
            strcpy(argv[i], parameters[i].data());
            printf("%s \n", argv[i]);
        }
        argv[parameters.size()] = NULL;
        execvp(mProcessPath.c_str(), argv);
        for (int i = 0; i < argc; i++)
        {
            delete[]argv[i];
            argv[i] = NULL;
        }
        delete[]argv;
        _exit(0);
    }
    return mPID >= 0;
#endif
}
bool OSProcess::Alive()
{
#ifdef _WIN32
    DWORD ret = WaitForSingleObject(mProcess.hProcess, 10);
    if (ret != WAIT_OBJECT_0 && ret != WAIT_TIMEOUT)
    {
        return false;
    }
    return true;
#else
    return kill(mPID, 0) == 0;
#endif
}
DWORD OSProcess::Wait(DWORD dwMilliseconds)
{
#ifdef _WIN32
    DWORD ret = WaitForSingleObject(mProcess.hProcess, dwMilliseconds);
    return ret;
#else
    __pid_t retPid = waitpid(mPID, &status, dwMilliseconds);
    return retPid;
#endif
}

DWORD OSProcess::ExitCode()
{
#ifdef _WIN32
    DWORD dwExit = 0;
    GetExitCodeProcess(mProcess.hProcess, &dwExit);
    return dwExit;
#else
    int exitCode = 0;
    __pid_t retPid = waitpid(mPID, &status, WNOHANG);
    //0 running
    /*  1.  WIFEXITED(status) 为非0	→ 进程正常结束
       WEXITSTATUS(status) 如上宏为真，使用此宏 → 获取进程退出状态 (exit的参数)
    2. 	WIFSIGNALED(status) 为非0 → 进程异常终止
       WTERMSIG(status) 如上宏为真，使用此宏 → 取得使进程终止的那个信号的编号。
    *3. 	WIFSTOPPED(status) 为非0 → 进程处于暂停状态
       WSTOPSIG(status) 如上宏为真，使用此宏 → 取得使进程暂停的那个信号的编号。
       WIFCONTINUED(status) 为真 → 进程暂停后已经继续运行*/
    //子进程正在运行
    if (retPid == 0)
    {
        return 3;
    }
    //normal exit
    if (WIFEXITED(status) != 0)
    {
        exitCode = WEXITSTATUS(status);
    }
    //异常终止(信号)
    else if (WIFSIGNALED(status) != 0)
    {
        exitCode = WTERMSIG(status);
    }
    else if (WIFSTOPPED(status) != 0)
    {
        exitCode = WSTOPSIG(status);
    }
    else if (WIFCONTINUED(status) != 0)
    {
        return 3;
    }
    return exitCode;
#endif
}
void OSProcess::Exit()
{
#ifdef _WIN32
    if (mProcess.hProcess != NULL)
    {
        TerminateProcess(mProcess.hProcess, 0);
        CloseHandle(mProcess.hProcess);
        mProcess.hProcess = NULL;
    }
    if (mStart.hStdInput != NULL)
    {
        CloseHandle(mStart.hStdInput);
        mStart.hStdInput = NULL;
    }
    if (mStart.hStdOutput != NULL)
    {
        CloseHandle(mStart.hStdOutput);
        mStart.hStdOutput = NULL;
    }
    if (mStart.hStdError != NULL)
    {
        CloseHandle(mStart.hStdError);
        mStart.hStdError = NULL;
    }
#else
    kill(mPID, 9);
#endif
}
OSProcess::~OSProcess()
{
    Exit();
}
pid_t OSProcess::GetPid()
{
#ifdef __linux
    return mPID;
#else
    return mProcess.dwProcessId;
#endif
}
pid_t OSProcess::GetSelf()
{
#ifdef __linux
    return getpid();
#else
    return GetCurrentProcessId();
#endif
}
pid_t OSProcess::GetParentID()
{
#ifdef __linux
    return getppid();
#else
    return 0;
#endif
}
BOOL OSProcess::SetRunPriority(DWORD fdwtPriority)
{
#ifdef _WIN32
    return ProcessHelper::SetPriority(mProcess.hProcess, fdwtPriority);
#else
    return setpriority(PRIO_PROCESS,mPID,fdwtPriority);
#endif
}
DWORD OSProcess::GetRunPriority()
{
#ifdef _WIN32
    return ProcessHelper::GetPriority(mProcess.hProcess);
#else
    return getpriority(PRIO_PROCESS,mPID);
#endif
}
#ifdef _WIN32
void OSProcess::SetStartShowWindow(bool bShow)
{
    ForkUtils::SetStartShowWindow(mStart, bShow);
}
#else
uid_t  OSProcess::GetLoginUserID()
{
    return getuid();
}
uid_t  OSProcess::GetUserID()
{
    return geteuid();
}
#endif
