#include <stdio.h>
#include <string.h>
#include<sstream>
#include<vector>
#include<string>
#include<unistd.h>
#include<wait.h>
#include<fcntl.h>
#define CMD_RESULT_BUF_SIZE 1024

/*
  * cmd：待执行命令
  * result：命令输出结果
  * 函数返回：0 成功；-1 失败；
*/
int ExecuteCMD(const char *cmd, char *result)
{
    int iRet = -1;
    char buf_ps[CMD_RESULT_BUF_SIZE];
    char ps[CMD_RESULT_BUF_SIZE] = { 0 };
    FILE *ptr;

    strcpy(ps, cmd);

    if ((ptr = popen(ps, "r")) != NULL)
    {
        while (fgets(buf_ps, sizeof(buf_ps), ptr) != NULL)
        {
            strcat(result, buf_ps);
            if (strlen(result) > CMD_RESULT_BUF_SIZE)
            {
                break;
            }
        }
        pclose(ptr);
        ptr = NULL;
        iRet = 0;  // 处理成功
    }
    else
    {
        printf("popen %s error\n", ps);
        iRet = -1; // 处理失败
    }

    return iRet;
}
int ExecuteUSBIPTest(const char *cmd)
{
    int fd = open("/tmp/ExecuteUSBIP_TEST",O_RDWR | O_CREAT | O_TRUNC,0644);
    dup2(fd,STDOUT_FILENO);
    dup2(fd,STDERR_FILENO);
    execlp("usbip","usbip","bind","-b",cmd,NULL);
}

int ExecuteUSBIPTest2(const char* cmd,const std::vector<std::string>& bindlist)
{
    char filePath[256] = {0};
    sprintf(filePath,"/tmp/ExecuteUSBIP_%d",getpid());
    int fd = open(filePath,O_WRONLY | O_CREAT,0644);
    if(fd<0)
    {
        printf("uam service ExecuteBind open file error %s ",strerror(errno));
        exit(-1);
    }
    //dup2(fd,STDOUT_FILENO);
    //dup2(fd,STDERR_FILENO);
    size_t bl = bindlist.size() + 4;
    char** list = (char**)malloc(sizeof(char*) * bl);
    for(size_t i = 0;i<bl;i++)
    {
        list[i] = (char*)malloc(sizeof(char) * 32);
    }
    strcpy(list[0],"usbip");
    strcpy(list[1],cmd);
    strcpy(list[2],"-b");
    for(size_t i = 0;i<bindlist.size();i++)
    {
        strcpy(list[i+3],bindlist[i].c_str());
    }
    list[bl -1] = NULL;
    execvp("usbip",list);
}

int Executels(char *result,ssize_t bs)
{
    const char* filePath = "Executelss";

    __pid_t pid = fork();
    if(pid == 0)
    {
        int fd = open(filePath,O_RDWR | O_CREAT,0644);
        if(fd<0)
        {
            printf("uam service ExecuteBind open file error %s ",strerror(errno));
            exit(-1);
        }
        dup2(fd,STDOUT_FILENO);
        dup2(fd,STDERR_FILENO);
        execlp("ls","ls","-l",NULL);
        exit(0);
    }
    else
    {
        waitpid(pid,NULL,0);
        int fd = open(filePath,O_RDWR | O_CREAT,0644);
        if(fd<0)
        {
            printf("uam service ExecuteBind open file error %s ",strerror(errno));
            exit(-1);
        }
        ssize_t sz = read(fd,result,bs);
        perror("read");
        close(fd);
    }
}
//error
int ExecuteUSBIP(const char *cmd, char *result,ssize_t bs)
{
    const char* filePath = "/tmp/ExecuteUSBS";
    int fd = open(filePath,O_RDWR | O_CREAT | O_TRUNC,0644);
    if(fd<0)
    {
        printf("uam service ExecuteBind open file error %s ",strerror(errno));
        exit(-1);
    }
    __pid_t pid = fork();
    if(pid == 0)
    {
        dup2(fd,STDOUT_FILENO);
        dup2(fd,STDERR_FILENO);
        execlp("usbip","usbip","bind","-b",cmd,NULL);
        exit(0);
    }
    else
    {
        waitpid(pid,NULL,0);
        ssize_t sz = read(fd,result,bs);
        unlink(filePath);
        close(fd);
    }
}
int ExecuteBind(const char* cmd,const char *bindid, char *result,ssize_t bs)
{
    __pid_t pid = fork();
    if(pid == 0)
    {
        char filePath[256] = {0};
        sprintf(filePath,"/tmp/ExecuteUSBIP_%d",getpid());
        int fd = open(filePath,O_WRONLY | O_CREAT,0644);
        if(fd<0)
        {
            printf("uam service ExecuteBind open file error %s ",strerror(errno));
            exit(-1);
        }
        dup2(fd,STDOUT_FILENO);
        dup2(fd,STDERR_FILENO);
        execlp("usbip","usbip",cmd,"-b",bindid,NULL);
    }
    else
    {
        waitpid(pid,NULL,0);
        char filePath[256] = {0};
        sprintf(filePath,"/tmp/ExecuteUSBIP_%d",pid);
        int fd = open(filePath,O_RDONLY);
        if(fd<0)
        {
            printf("uam service ExecuteBind open file error %s ",strerror(errno));
            return -1;
        }
        else
        {
            read(fd,result,bs);
            unlink(filePath);
            close(fd);
        }
    }
    return 0;
}

int ExecuteBind(const char* cmd,const std::vector<std::string>& bindlist, char *result,ssize_t bs)
{
    __pid_t pid = fork();
    if(pid == 0)
    {
        char filePath[256] = {0};
        sprintf(filePath,"/tmp/ExecuteUSBIP_%d",getpid());
        int fd = open(filePath,O_WRONLY | O_CREAT,0644);
        if(fd<0)
        {
            printf("uam service ExecuteBind open file error %s ",strerror(errno));
            exit(-1);
        }
        dup2(fd,STDOUT_FILENO);
        dup2(fd,STDERR_FILENO);
        size_t bl = bindlist.size() + 4;
        char** list = (char**)malloc(sizeof(char*) * bl);
        for(size_t i = 0;i<bl;i++)
        {
            list[i] = (char*)malloc(sizeof(char) * 32);
        }
        strcpy(list[0],"usbip");
        strcpy(list[1],cmd);
        strcpy(list[2],"-b");
        for(size_t i = 0;i<bindlist.size();i++)
        {
            strcpy(list[i+3],bindlist[i].c_str());
        }
        list[bl -1] = NULL;
        execvp("usbip",list);
        exit(0);
    }
    else
    {
        waitpid(pid,NULL,0);
        char filePath[256] = {0};
        sprintf(filePath,"/tmp/ExecuteUSBIP_%d",pid);
        int fd = open(filePath,O_RDONLY);
        if(fd<0)
        {
            printf("uam service ExecuteBind open file error %s ",strerror(errno));
            return -1;
        }
        else
        {
            read(fd,result,bs);
            unlink(filePath);
            close(fd);
        }
    }
    return 0;
}

int main()
{
    char result[CMD_RESULT_BUF_SIZE] = { 0 };

    //ExecuteCMD("ls -l", result);
    //printf("%s", result);

    ExecuteCMD("usbip list -l", result);
    printf("ExecuteCMD %s", result);

    memset(result,0,sizeof(result));
    Executels(result,sizeof(result));
    printf("ls \n %s \n", result);

    memset(result,0,sizeof(result));
    //ExecuteUSBIP("1-3",result,sizeof(result));
    //printf("ExecuteUSBIP \n %s \n", result);

    memset(result,0,sizeof(result));
    ExecuteBind("bind","1-3",result,sizeof(result));
    //printf("%s", result);

    memset(result,0,sizeof(result));
    std::vector<std::string> pivlist;
    pivlist.push_back("1-3");
    pivlist.push_back("2-3");
    ExecuteBind("bind",pivlist,result,sizeof(result));
    //printf("%s", result);

    //ExecuteUSBIPTest("1-3");
    ExecuteUSBIPTest2("bind",pivlist);
    return 0;
}
