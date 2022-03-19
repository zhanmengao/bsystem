#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>//for ENOENT
#include <string.h>//for memset
int main() 
{
	struct stat st;
	memset(&st, 0, sizeof(st));
	if (!stat("/zww/test/myfile1.txt", &st)) //���myfil.txt�����ڣ�stat�ͻ᷵�طǣ�
	{
		if (st.st_size >= 0) //����һ�㱣֤
		{
			printf("/zww/test/myfile1.txt exists.\n");
		}
	}
	else 	if(ENOENT == errno)
		printf("/zww/test/myfile1.txt does NOT exist:%d\n",errno);
}