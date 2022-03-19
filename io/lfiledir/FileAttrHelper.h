//
// Created by zhanmengao on 10/12/20.
//

#ifndef LFILEMANAGER_FILEATTRHELPER_H
#define LFILEMANAGER_FILEATTRHELPER_H
#include"../../../Linux/apue.h"

#include<dirent.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/types.h>
#include"../../FileManager/CFileManager.h"
enum FILE_TYPE
{
    FILET_INVALID = -1,
    FILET_SOCK = DT_SOCK,
    FILET_LNK = DT_LNK,
    FILET_REG = DT_REG,
    FILET_BLK = DT_BLK,
    FILET_DIR = DT_DIR,
    FILET_CHR = DT_CHR,
    FILET_FIFO = DT_FIFO,
};
#ifdef __cplusplus
class FileAttrHelper
{
public:
    static int IsFolderByType(char d_type)
    {
        return d_type == DT_DIR;
    }
    static int IsNormalFileByType(char d_type)
    {
        return d_type == DT_REG;
    }
    static int IsLinkFileByType(char d_type)
    {
        return d_type == DT_LNK;
    }
    static FILE_TYPE fgettype(mode_t mode)
    {
        mode &= S_IFMT;
        switch(mode)
        {
            case S_IFDIR:return FILE_TYPE::FILET_DIR;break;
            case S_IFBLK:return FILE_TYPE::FILET_BLK;break;
            case S_IFREG:return FILE_TYPE::FILET_REG;break;
            case S_IFIFO:return FILE_TYPE::FILET_FIFO;break;
            case S_IFLNK:return FILE_TYPE::FILET_LNK;break;
            case S_IFSOCK:return FILE_TYPE::FILET_SOCK;break;
            case S_IFCHR:return FILE_TYPE::FILET_CHR;break;
        }
        return FILE_TYPE::FILET_INVALID;
    }
    static FILE_TYPE GetType(const char* filePath)
    {
        struct stat fileStat;
        lstat(filePath,&fileStat);
        mode_t mode = fileStat.st_mode;
        mode&= S_IFMT;
        return fgettype(mode);
    }

    static int IsFolder(const char* filePath)
    {
        struct stat fileStat;
        lstat(filePath,&fileStat);
        mode_t mode = fileStat.st_mode;
        mode&= S_IFMT;
        return mode == S_IFDIR;
    }
    static int IsNormalFile(const char* filePath)
    {
        struct stat fileStat;
        int ret = lstat(filePath,&fileStat);
        if(ret == -1)
        {
            perror(filePath);
            return -1;
        }
        return S_ISREG(fileStat.st_mode);
    }
//lstat BU Chuan tou link file
//stat will find real file
    static int IsLinkFile(const char* filePath)
    {
        struct stat fileStat;
        int ret = lstat(filePath,&fileStat);
        if(ret == -1)
        {
            perror(filePath);
            return -1;
        }
        return S_ISLNK(fileStat.st_mode);
    }
    static int isux(const char* path)
    {
        struct stat st;
        int ret = stat(path,&st);
        if(ret == -1)
        {
            return ret;
        }
        return st.st_mode & S_IXUSR;
    }
    static int isur(const char* path)
    {
        struct stat st;
        int ret = stat(path,&st);
        if(ret == -1)
        {
            return ret;
        }
        return st.st_mode & S_IRUSR;
    }
    static int SetFileExec(const char* Xfilename)
    {
        struct stat fileStat;
        int iret = stat( Xfilename ,&fileStat );
        if(iret == -1)
        {
            return -1;
        }
        return chmod( Xfilename, fileStat.st_mode | S_IXUSR  );
    }
    static struct stat GetAttr(const char* filePath)
    {
        struct stat fileStat;
        lstat(filePath,&fileStat);
        return fileStat;
    }
    static int GetFileSize(int fd)
    {
        int pos = lseek(fd,0,SEEK_CUR);
        if(pos == -1)
        {
            perror("lseek");
            return -1;
        }
        int size = lseek(fd,0,SEEK_END);
        lseek(fd,pos,SEEK_SET);
        return pos;
    }
    static int GetFilesize(const char* filePath)
    {
        struct stat fileStat;
        stat(filePath,&fileStat);
        return fileStat.st_size;
    }
    static int Chmod(const char* filePath,mode_t mode)
    {
        return chmod(filePath,mode);
    }
    static int chmod755(const char *path)
    {
        return chmod(path,S_IRWXU | S_IRGRP |S_IXGRP | S_IROTH | S_IXOTH);
    }
    static int chmodux(const char* path)
    {
        struct stat st;
        int ret = stat(path,&st);
        if(ret == -1)
        {
            return ret;
        }
        ret = chmod(path,st.st_mode & S_IXUSR);
        return ret;
    }
    static int Chown(const char* filePath,uid_t owner,uid_t gid)
    {
        return chown(filePath,owner,gid);
    }
    static mode_t uMask(mode_t mask)
    {
        return umask(mask);
    }
    static mode_t UMask775()
    {
        return umask(002);
    }
    static mode_t  uMask755()
    {
        return umask(022);
    }
    static mode_t uMask750()
    {
        return umask(027);
    }
};
#endif

#endif //LFILEMANAGER_FILEATTRHELPER_H
