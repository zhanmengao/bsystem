#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

int main(int argc, char const *argv[])
{
	int ret=kill(getpid(),SIGKILL);
	if(ret==-1)
	{
		perror("kill error");
		exit(ret);
	}
	sleep(10);
	return 0;
}