#include <sys/mman.h> /* for mmap and munmap */  
#include <sys/types.h> /* for open */  
#include <sys/stat.h> /* for open */  
#include <fcntl.h>     /* for open */  
#include <unistd.h>    /* for lseek and write */  
#include <stdio.h>  
#include <string.h> /* for memcpy */  
      
int main(int argc, char **argv)  
{  
	int fd;  
	char *mapped_mem, * p;  
	int flength = 1024;  
	void * start_addr = 0;  
      
	fd = open(argv[1], O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);  
	flength = lseek(fd, 1, SEEK_END);  
	write(fd, "\0", 1); //���ļ�������һ�����ַ����Ա�����printf�������� 
	lseek(fd, 0, SEEK_SET);  
	start_addr = (void*)0x80000;  
	mapped_mem = (char*)mmap(start_addr,
		flength,
		PROT_READ|PROT_WRITE,        //����д��  
		MAP_SHARED,       //�����������̷��ʴ��ڴ�����  
		fd,
		0);  
      
    // ʹ��ӳ������. 
	printf("%s\n", mapped_mem); //Ϊ�˱�֤���﹤���������������ݵ��ļ��������һ���ı���  
	while ((p = strstr(mapped_mem, "hello"))) { // �˴����޸��ļ� ���� 
		memcpy(p, "Linux", 5);  
		p += 5;  
	}  
          
	close(fd);  
	munmap(mapped_mem, flength);  
	return 0;  
}  
