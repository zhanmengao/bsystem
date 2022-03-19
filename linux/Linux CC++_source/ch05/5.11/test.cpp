#include <stdio.h>
#include <stdlib.h>
 #include <string.h>
 #include <fcntl.h>
 #include <sys/types.h>
 #include <unistd.h>
 #include <sys/wait.h>
 #include <signal.h>
#include <sys/stat.h>

 #define MAXFILE 65535

volatile sig_atomic_t _running = 1;

void sigterm_handler(int arg)
{
	_running = 0;
}

int main()
{
	pid_t pc;
	int i, fd, len;
	char *buf = "this is a Dameon\n";
	len = strlen(buf);
	pc = fork(); //��һ��
	if(pc < 0)                                                   
	{
		printf("error fork\n");
		exit(1);
	}
	else if(pc > 0)
		exit(0);

	setsid(); //�ڶ���
	chdir("/"); //������
	umask(0); //���Ĳ�
	for(i = 0 ; i < MAXFILE ; i++) //���岽
		close(i);
	signal(SIGTERM, sigterm_handler);
	while(_running) 
	{
		if((fd = open("/tmp/dameon.log", O_CREAT | O_WRONLY | O_APPEND, 0600)) < 0) \
		{
			perror("open");
			exit(1);
		}
		write(fd, buf, len + 1);
		close(fd);
		usleep(10 * 1000); //10����
	}

}

