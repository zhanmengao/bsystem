
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/mman.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
 
#define MMAP_DATA_SIZE 1024
 
int main(int argc,char * argv[])
{
        char * data;
        int fd = shm_open("shm-file0001", O_CREAT|O_RDWR, 0777);
 
        if (fd < 0) {
                printf("shm_open failed!\n");
                return -1;
        }
 
        ftruncate(fd, MMAP_DATA_SIZE);
        if (1) {
                data = (char*)mmap(NULL, 1024, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
                if (!data) {
                        printf("mmap failed\n");
                        close(fd);
                }
 
 
                sprintf(data,"This is a share memory! %d\n",fd);
 
                munmap(data,MMAP_DATA_SIZE);
        }
        else {
            char buf[1024];
            int len = sprintf(buf,"This is a share memory by write! ! %d\n",fd);
            if (write(fd,buf,len) <= 0) {
                printf("write file %d failed!%d\n",len,errno);
             }  
        }
 
        close(fd);
        getchar();
 
        shm_unlink("shm-file0001");
 
        return 0;
}