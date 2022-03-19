#include <stdio.h>
#include <unistd.h>

int main()
{
	pid_t pid;
	pid=fork();
	if(pid==0)
	{
		char *agrs[]={"pwd",NULL};
		execvp("pwd",agrs);
	}
	return 0;
}
