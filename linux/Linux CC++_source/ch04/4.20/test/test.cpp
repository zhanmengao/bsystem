
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
int main()
{
	int fd;
	struct stat buf;
	stat("test.txt", &buf);//1
	printf("1.link =% d\n", buf.st_nlink);//δ���ļ�֮ǰ����������

	fd = open("test.txt", O_RDONLY);//2.���Ѵ����ļ�test.txt
	stat("test.txt", &buf);
	printf("2.link =% d\n", buf.st_nlink);//����������

	close(fd);//3.�ر��ļ�test.txt
	stat("test.txt", &buf);
	printf("3.link =% d\n", buf.st_nlink);//����������

	link("test.txt", "test2.txt");//4.����Ӳ����test2.txt
	stat("test.txt", &buf);
	printf("4.link =% d\n", buf.st_nlink);//����������

	unlink("test2.txt");//5.ɾ��test2.txt
	stat("test.txt", &buf);
	printf("5.link =% d\n", buf.st_nlink);//����������

	//6.�ظ�����2  //���´�test.txt
	fd = open("test.txt", O_RDONLY);//���Ѵ����ļ�test.txt
	stat("test.txt", &buf);
	printf("6.link =% d\n", buf.st_nlink);//����������
	
	
	unlink("test.txt");//7.ɾ��test.txt
	fstat(fd, &buf);
	printf("7.link =% d\n", buf.st_nlink);//����������

	close(fd);//8.�˲�����Բ���ʾд������Ϊ���̽���ʱ���򿪵��ļ��Զ����رա�
	
}