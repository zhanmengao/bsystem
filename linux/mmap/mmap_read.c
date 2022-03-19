#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <fcntl.h>

#define MMAPFILE "/tmp/data.1"

struct User
{
    int id;
    char name[32];
};
int main()
{
    struct User* add;
    struct User user;
    int fd;
    fd=open(MMAPFILE,O_RDWR | O_CREAT,0775);
    if(fd<0)
    {
        perror("open error");
        exit(-1);
    }
    write(fd,&user,sizeof(user));
    add = mmap(NULL,sizeof(user),PROT_READ | PROT_WRITE,MAP_SHARED,fd,0);// creatt 4b mapp
    if(add==MAP_FAILED)
    {
        perror("mmap error");
        exit(-3);
    }

    /*
    Bus error解决方案

     mmap过后，在进行内存拷贝时，先使用write()函数向已经打开的文件描述符中写东西。
     */
    close(fd);
    pid_t pid = fork();
    if(pid == 0)
    {
        while(1)
        {
            user.id = getpid();
            memcpy(add,&user,sizeof(user));
            sleep(5);
        }
    }
    else if(pid >0)
    {
        while(1)
        {
            sleep(2);
            memcpy(&user,add,sizeof(user));
            printf("userID=%u,username=%s\n",user.id,user.name);
        }
    }
    unlink(MMAPFILE);
    return 0;
}
