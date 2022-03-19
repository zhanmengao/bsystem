#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
void myDaemon()
{
	// 1 所有工作在子进程中进行形式上脱离了控制终端 
	pid_t pid=fork();
	if(pid>0)
	{
		exit(0);
	}

	// 2  单独出去开一个回话
	setsid();

	// 3  防止占用可卸载的文件系统
	int ret=chdir("/home/");
	if(ret==-1)
	{
		perror("chdir error!");
		exit(-3);
	}

	//4  重设文件权限掩码  　　	防止继承的文件创建屏蔽字拒绝某些权限，增加守护进程灵活性
	umask(0002);

	//5   继承的打开文件不会用到，浪费系统资源，无法卸载
	close(STDIN_FILENO);
	ret=open("/tmp/null2",O_RDWR | O_CREAT);
	if(ret<0)
	{
		perror("open file error!");
		exit(-5);
	}
	dup2(ret,STDOUT_FILENO);
	dup2(ret,STDERR_FILENO);
}


int main(int argc, char const *argv[])
{
	myDaemon();
	while(1)
	{
		sleep(5);
	}
	return 0;
}
