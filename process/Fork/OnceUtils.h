#ifndef ONCEUTILS_H
#define ONCEUTILS_H
#include<unistd.h>
#include<fcntl.h>
#include<string>
#include<string.h>
#define LOCKMODE (S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH)
class OnceUtils
{
public:
    bool Lock(const char* name)
    {
        int		fd;
        char	buf[16];

        fd = open(name, O_RDWR|O_CREAT, LOCKMODE);
        if (fd < 0)
        {
            exit(1);
        }
        if (lockfile(fd) < 0)
        {
            if (errno == EACCES || errno == EAGAIN)
            {
                close(fd);
                return false;
            }
            close(fd);
            return false;
        }
        ftruncate(fd, 0);
        sprintf(buf, "%ld", (long)getpid());
        write(fd, buf, strlen(buf)+1);
        return true;
    }
private:
    int lockfile(int fd)
    {
        struct flock lock;
        lock.l_type = F_WRLCK;		//独占锁
        lock.l_whence=SEEK_SET;		//从文件开始位置开始锁
        lock.l_start = 0;		//偏移
        lock.l_len = 0;			//锁到文件末尾
        lock.l_pid = getpid();
        return fcntl(fd,F_SETLK,&lock);		//设置文件锁
    }
};

#endif // ONCEUTILS_H
