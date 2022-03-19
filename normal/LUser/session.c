#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
	pid_t pid;
	pid=fork();
	if(pid==0)
	{
		printf("child PID == %d\n", getpid());
        printf("child Group ID == %d\n", getpgid(0));
        printf("child Session ID == %d\n", getsid(getpid()));
        printf("\n");
        setsid();
 		printf("child PID == %d\n", getpid());
        printf("child Group ID == %d\n", getpgid(0));
        printf("child Session ID == %d\n", getsid(getpid()));               
	}
	else
	{
		printf("\n");
		printf("parent PID == %d\n", getpid());
        printf("parent Group ID == %d\n", getpgid(0));
        printf("parent Session ID == %d\n", getsid(getpid()));  
	}
	return 0;
}