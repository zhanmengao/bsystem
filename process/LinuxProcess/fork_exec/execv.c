#include <stdio.h>
#include <unistd.h>

int main()
{
	pid_t pid;
	pid=fork();
	if(pid==0)
	{
		char *agrs[]={"pwd",NULL};
		execv("/bin/pwd",agrs);
	}
	return 0;
}
