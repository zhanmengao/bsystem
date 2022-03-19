#include<unistd.h>
#include<stdio.h>
#include<fcntl.h>
#include<stdlib.h>

int main()
{
	char buffer[1024] = {0};
	while(1)
	{

		int rn = read(STDIN_FILENO,buffer,sizeof(buffer));
		if(rn <0)
		{
			perror("read STDIO_FILENO");
			exit(-1);
		}
		write(STDOUT_FILENO,buffer,rn);
	}
	return 0;
}

