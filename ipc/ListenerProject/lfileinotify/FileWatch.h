//
// Created by uzi on 12/8/20.
//

#ifndef LFILEINOTIFY_FILEWATCH_H
#define LFILEINOTIFY_FILEWATCH_H
#include <sys/inotify.h>
#include<stdio.h>
#include<fcntl.h>
#include"../../../File/OSFileManager/OSUtils.h"
#ifdef __cplusplus
#include<vector>
#include<string>
#endif

/*
int inotify_init()//初始化inotify，每个inotify实例对应一个排队的序列
int inotify_add_watch(int fd,const char *path,uint32_t mask)//通过文件名和事件掩码添加一个watch对象，返回值是watch对象的描述符
//fd：inotify_init的返回值
//path：要监控的文件路径
//mask：监听文件的哪些事件

//返回值：表示对那个文件的监视

int inotify_rm_watch(int fd,uint32_t wd)//删除监视对象
//fd：inotify_init的返回值
//wd：inotify_add_watch的返回值

//文件事件用struct inotify_event表示，通过fd由read函数来读取
//buf是inotify_event结构的数组的指针，BUF_LEN 指定要读取的总长度，buf的长度不能小于BUF_LEN<br>//read读取到的事件数取决于BUF_LEN以及事件中文件名的长度，返回实际读取的长度
size_t len = read (fd, buf, BUF_LEN);
*/
//struct inotify_event
//    __s32           wd;             /* watch descriptor */
//    __u32           mask;           /* watch mask */
//    __u32           cookie;         /* cookie to synchronize two events */
//    __u32           len;            /* length (including nulls) of name */
//    char            name[0];        /* stub for possible name */
//文件名是变长的，实际紧跟在该结构的后面，文件名被0填充以保证下一个事件结构能够4字节对齐。<br>//len字段也把name的填充字段统计在内
static int addListenPath(int inotifyFd, const char* filePath,uint32_t mask)
{
    return inotify_add_watch(inotifyFd, filePath, mask);
}

struct notifyInfo
{
public:
    std::string name;
    notifyInfo(const inotify_event* const info)
    {
        mask = info->mask;
        name = info->name;
    }
    notifyInfo& operator=(const inotify_event* const info)
    {
        mask = info->mask;
        name = info->name;
    }
public:
/*IN_ATTRIB，文件属性被修改
IN_CLOSE_WRITE，可写文件被关闭
IN_CLOSE_NOWRITE，不可写文件被关闭
IN_CREATE，文件/文件夹被创建
IN_DELETE，文件/文件夹被删除
IN_DELETE_SELF，被监控的对象本身被删除
IN_MODIFY，文件被修改
IN_MOVE_SELF，被监控的对象本身被移动
IN_MOVED_FROM，文件被移出被监控目录
IN_MOVED_TO，文件被移入被监控目录
IN_OPEN，文件被打开*/
    bool IsAttrB()
    {
        return mask & IN_ATTRIB;
    }
    bool IsModify()
    {
        return mask & IN_MODIFY;
    }
    bool IsOpen()
    {
        return mask & IN_OPEN;
    }
    bool IsClose()
    {
        return mask & IN_CLOSE_WRITE ||mask & IN_CLOSE_NOWRITE;
    }
    bool IsCreat()
    {
        return mask & IN_CREATE;
    }
    bool IsDelete()
    {
        return mask & IN_DELETE;
    }
    bool IsDeleteSelf()
    {
        return mask & IN_DELETE_SELF;
    }
    bool IsMoveSelf()
    {
        return mask & IN_MOVE_SELF;
    }
    bool IsMoveFrom()
    {
        return mask & IN_MOVED_FROM;
    }
    bool IsMoveTo()
    {
        return mask & IN_MOVED_TO;
    }

private:
    uint32_t mask;	/* Watch mask.  */
};
struct listenInfo
{
    int fd;
    std::string filePath;
};
class FileListener
{
public:
    FileListener()
    {
        inotifyFd = inotify_init();
    }
    ~FileListener()
    {
        for(auto it = mListen.begin();it!=mListen.end();it++)
        {
            inotify_rm_watch(inotifyFd, it->fd);
        }
    }
    bool AddListen(const char* filePath,uint32_t mask = IN_ATTRIB | IN_CLOSE_WRITE | IN_CLOSE_NOWRITE | IN_CREATE | IN_DELETE | IN_DELETE_SELF
                                                        |IN_MODIFY | IN_MOVE_SELF | IN_MOVED_FROM | IN_MOVED_TO | IN_OPEN | IN_ACCESS)
    {
        int fd = ::addListenPath(inotifyFd,filePath,mask);
        if( fd > 0)
        {
            listenInfo info;
            info.fd = fd;
            info.filePath = filePath;
            mListen.push_back(info);
        }
        return fd>0;
    }
    int DelListen(const char* filePath)
    {
        int ret = -1;
        for(auto it = mListen.begin();it!=mListen.end();it++)
        {
            if(it->filePath == filePath)
            {
                ret = inotify_rm_watch(inotifyFd,it->fd);
                mListen.erase(it);
                break;
            }
        }
        return ret;
    }
    void SetBlock(bool bBlock)
    {
        if(bBlock)
        {
            ::SetFileBlock(inotifyFd);
        }
        else
        {
            ::SetFileNonBlock(inotifyFd);
        }
    }
    void Read(std::vector<notifyInfo>& vec)
    {
        int len, pos, index;
        char nameBuffer[64];
        char eventBuff[256];
        struct inotify_event* inotifyEvent;
        do
        {
            len = read(inotifyFd, eventBuff, 256);
            if (len <= 0)
                break;
            pos = 0;
            while (pos < len)
            {
                inotifyEvent = (struct inotify_event*)(eventBuff + pos);
                vec.push_back(inotifyEvent);
                pos += sizeof(struct inotify_event) + inotifyEvent->len;
            }
        }while(false);
    }

protected:
    int inotifyFd;
    std::vector<listenInfo> mListen;
};
#endif //LFILEINOTIFY_FILEWATCH_H
