/*
this is QQ Client
*/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>

#define ONLINE 0
#define DOWMLINE 1
#define CHAT 2

#define SERVER_ADDR "/home/uzi/桌面/gitSPace/系统编程/QQ/ServerFifo"
#define CLIENT_ADDR_ROOT "/tmp/fifo."
#define FILE_MODEL (S_IRUSR | S_IWUSR | S_IRGRP | S_IWOTH)

struct MSG
{
	int send;
	int receive;
	int msgCode;
	char data[1024];
};

int fd_write;
int fd_read;
void SendRequest(struct MSG* pMsg);
void Response(const struct MSG* const pMsg);
void Lock(int fd);
void UNLock(int fd);
int main(int argc, char const *argv[])
{
	pid_t pid=getpid();
	char buffer[1024];
	memset(buffer,0,sizeof(buffer));
	strcpy(buffer,CLIENT_ADDR_ROOT);
	strcat(buffer,(char*)&pid);
	printf("addr= %s\n",buffer);
	//receive
	if(mkfifo(buffer,FILE_MODEL)==-1)
	{
		perror("server mkpipe1 error\n");
		exit(-1);
	}

	fd_write=open(SERVER_ADDR,O_WRONLY);
	fd_read=open(buffer,O_RDWR);

	if(fd_write>0 && fd_read>0)
	{

		struct MSG msg;
		//send online package
		memset(&msg,0,sizeof(msg));
		msg.send=pid;
		msg.receive=-1;
		strcpy(msg.data,buffer);        //此时buffer存储的是fifo文件路径
		SendRequest(&msg);
		int ret;                       //读取到的长度
		char isBeginInputData=0;  //是否正在接收输入data
		UNLock(fd_write);
		UNLock(fd_read);
		UNLock(STDIN_FILENO);
		int receive=0;                  //接收方ID
		while(1)
		{
			//ReSetState
			memset(&msg,0,sizeof(msg));
			memset(buffer,0,1024);		
			//read input
			//printf("\n %s","您要发给哪个进程？ ");
			if(!isBeginInputData)
			{
				scanf("%u",&receive);
				if(receive>0)
				{
					printf("receive=%u\n",receive);
			        isBeginInputData=1;		
				}

			}
			else
			{
				Lock(STDIN_FILENO);
				gets(buffer);
				printf("\n %s","您要发送的内容： ");
				//cin>>buffer;
				if(strlen(buffer)>0)
				{
					msg.send=pid;
					msg.receive=receive;
					msg.msgCode=CHAT;
					strcpy(msg.data,buffer);
					SendRequest(&msg);
			        receive=0;
					isBeginInputData=0;	
					UNLock(STDIN_FILENO);				
				}

			}


			//receive from server
			ret=read(fd_read,&msg,1024);;
			if(ret>0)
			{
				Response(&msg);
			}
			sleep(0.5f);
		}
	}
	return 0;
}
//向服务器发出消息
void SendRequest(struct MSG* pMsg)
{
	write(fd_write,pMsg,sizeof(*pMsg));
	memset(pMsg,0,sizeof(*pMsg));
}
//处理服务器发过来的消息
void Response(const struct MSG* const pMsg)
{
	switch(pMsg->msgCode)
	{
		case CHAT:
		    printf("%u said:%s \n",pMsg->send,pMsg->data);
		break;
	}
}
void Lock(int fd)
{
        /* 设置为阻塞*/
	if (fcntl(fd, F_SETFL,F_SETFL) < 0)
	{
		perror("fcntl fd_write 失败");
		exit(-1);
	}

}
void UNLock(int fd)
{
    int flags = fcntl(fd, F_GETFL, 0);               //获取当前flag	
        /* 设置为非阻塞*/
    if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) < 0)
    {
    	perror("fcntl fd_write 失败");
    	exit(-1);
    }
}