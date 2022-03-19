#include <stdio.h>
#include <unistd.h>

int main()
{
	pid_t pid;
	pid=fork();
	if(pid==0)
	{
		execlp("ls","ls","-l",NULL);
	}
	return 0;
}


