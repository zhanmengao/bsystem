#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>

int main(void)
{
    int fd = -1,i;
    ssize_t size =-1;
    char buf[10];   
    char filename[] = "/root/test.txt"; //Ҫ��ȡ���ļ�

    fd = open(filename,O_RDONLY); //ֻ����ʽ���ļ�
    if(-1==fd)
    {   
            printf("Open file %s failuer,fd:%d\n",filename,fd);
		       return -1;
    }
    else  printf("Open file %s success,fd:%d\n",filename,fd);
  
    //ѭ����ȡ���ݣ�ֱ���ļ�ĩβ���߳���
    while(size)
    {   
       //��ȡ�ļ��е����ݣ�10����˼��ϣ����10���ֽڣ��������������ֽ����Ǻ�������ֵ
        size = read(fd,buf,10);       
        if(-1==size)
        {   
            close(fd);
            printf("Read file %s error occurs\n",filename);
            return -1; 
        }else{
            if(size>0)
            {   
                printf("read %d bytes:",size);
                printf("\"");
                for(i =0;i<size;i++) //ѭ����ӡ�ļ�����������
                    printf("%c",*(buf+i));
                printf("\"\n");
            }else{
                printf("reach the end of file \n");
            }

        }
    }
    return 0;
}
