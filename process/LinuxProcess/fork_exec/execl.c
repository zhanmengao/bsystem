#include <stdio.h>
#include <unistd.h>

int main()
{
	pid_t pid;
	pid=fork();
	if(pid==0)
	{
		execl("/bin/pwd","pwd",NULL);
	}
	printf("YYYYYYYYYYYYYYYYYYYYY \n");
	return 0;
}
