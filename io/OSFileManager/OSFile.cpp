#include"OSFile.h"


//创建新的文件，有则打开
bool OSFile::Create(const char *filePath, bool bOnCreate, bool bAppend, bool bBlock)
{
    int flags = 0;
    flags |= O_CREAT;
    if (bOnCreate)
    {
        flags |= O_EXCL;
    }
    if (bAppend)
    {
        flags |= O_APPEND;
    }
    if (!bBlock)
    {
        flags |= O_NONBLOCK;
    }
    fd = ::Open(filePath, flags, 0644);
    return fd > 0;
}


bool OSFile::Open(const char *filePath, bool bAppend, bool bBlock)
{
    int flags = 0;
    if (bAppend)
    {
        flags |= O_APPEND;
    }
    if (!bBlock)
    {
        flags |= O_NONBLOCK;
    }
    fd = ::Open(filePath, flags, 0644);
    return fd > 0;
}

int OSFile::Write(void *rData, ssize_t rSize)
{
    return ::Write(fd, rData, rSize);
}

int OSFile::Read(void *rBuffer, ssize_t rSize)
{
    return ::Read(fd, rBuffer, rSize);
}

int OSFile::Futimens(const struct timespec times[2])
{
    return futimens(fd,times);
}

int OSFile::WriteSeek(const void* rData,ssize_t rSize,off_t offset)
{
    return pwrite(fd,rData,rSize,offset);
}
int OSFile::ReadSeek(void* rData,ssize_t rSize,off_t offset)
{
    ssize_t nread = pread(fd,rData,rSize,offset);
    lseek(fd,nread,SEEK_CUR);
    return nread;
}
int OSFile::IsCanSeek()
{
    if(lseek(fd,0,SEEK_CUR) == -1)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

int OSFile::SetSeek(off_t offset,int where)
{
    return lseek(fd,offset,where);
}

int OSFile::Resizefd(off_t  length)
{
    return ftruncate(fd, length);
}
__off_t OSFile::ResizeBiger(ssize_t bigger)
{
    if(bigger == 0)
    {
        return 0;
    }
    __off_t pos = lseek(fd,0,SEEK_CUR);
    lseek(fd,bigger - 1,SEEK_END);
    write(fd,"\0",1);                   //io make filesize change
    return ::lseek(fd,pos,SEEK_CUR);
}
int OSFile::ResizeFile(const char* path, off_t  length)
{
    return truncate(path, length);
}
FILE* OSFile::GetFILE(const char* mode)
{
    return  fdopen(fd,mode);
}
int OSFile::GetFILENO(FILE* pFile)
{
    return fileno(pFile);
}
int OSFile::Getfl()
{
    return fcntl(fd,F_GETFL,0);
}
int OSFile::Setfl(int flags)
{
    unsigned fl = fcntl(fd,F_GETFL,0);
    AddBits(&fl,flags);
    return fcntl(fd,F_SETFL,fl);
}
int OSFile::Clrfl(int flags)
{
    unsigned fl = fcntl(fd,F_GETFL,0);
    RemoveBits(&fl,flags);
    return fcntl(fd,F_SETFL,fl);
}
int OSFile::SetFileNonBlock()
{
    return Setfl(O_NONBLOCK);
}
int OSFile::SetFileBlock()
{
    return Clrfl(O_NONBLOCK);
}
int OSFile::SetSync()
{
    return Setfl(O_SYNC);
}

int OSFile::IsReadOnly()
{
    int flag = Getfl();
    flag &=O_ACCMODE;
    return flag & O_RDONLY;
}
int OSFile::IsWriteOnly()
{
    int flag = Getfl();
    flag &=O_ACCMODE;
    return flag == O_WRONLY;
}
int OSFile::IsReadWrite()
{
    int flag = Getfl();
    flag &=O_ACCMODE;
    return flag == O_RDWR;
}
int OSFile::IsCanRead()
{
    int flag = Getfl();
    flag &=O_ACCMODE;
    return flag == O_RDWR || flag == O_RDONLY;
}
int OSFile::IsCanWrite()
{
    int flag = Getfl();
    flag &=O_ACCMODE;
    return flag == O_RDWR || flag == O_WRONLY;
}
int OSFile::IsSync()
{
    int flag = Getfl();
    return flag & O_SYNC;
}
int OSFile::IsAppend()
{
    int flag = Getfl();
    return flag & O_APPEND;
}

int OSFile::ReSTDIN()
{
    return dup2(fd,STDIN_FILENO);
}
int OSFile::ReSTDOUT()
{
    return dup2(fd,STDOUT_FILENO);
}
int OSFile::Copyfd()
{
    return dup(fd);
    fcntl(fd,F_DUPFD,0);        //==
}
int OSFile::Copyfd2(int fd2)
{
    return dup2(fd,fd2);
    close(fd2);
    fcntl(fd,F_DUPFD,fd2);          //==
}
