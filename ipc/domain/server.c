#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <strings.h>
#include <string.h>
#include <ctype.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include <stddef.h>

#define SERVER_SOCK "domainserv.socket"
void main()
{
	int lfd = socket(AF_UNIX,SOCK_STREAM,0);
	if(lfd <0)
	{
		perror("socker error");
		return;
	}
	struct sockaddr_un sevrAddr;
	memset(&sevrAddr,0,sizeof(sevrAddr));
	sevrAddr.sun_family = AF_UNIX;
	strcpy(sevrAddr.sun_path,SERVER_SOCK);
	int addrSize = offsetof(struct sockaddr_un,sun_path) + strlen(sevrAddr.sun_path);			//拿到结构体实际大小
	unlink(SERVER_SOCK);															//删除sock文件，否则会bind失败
	if(bind(lfd,(struct sockaddr *)&sevrAddr,addrSize) < 0)
	{
		perror("bind error");
		return;
	}	
	listen(lfd,5);
	while(1)
	{
		struct sockaddr_un clieAddr;
		memset(&clieAddr,0,sizeof(clieAddr));
		int clieLen = sizeof(clieAddr);
		int cfd = accept(lfd,(struct sockaddr *)&clieAddr, (socklen_t *)&clieLen);
		{
			//打印对端文件名	
			clieLen -= offsetof(struct sockaddr_un, sun_path);
			clieAddr.sun_path[clieLen] = '\0';              
			printf("client bind filename %s\n", clieAddr.sun_path);			
		}
		int rSize = 0;
		char buf[1024] = {0};
        while ((rSize = read(cfd, buf, sizeof(buf))) > 0) 
		{
            for (int i = 0; i < rSize; i++)
			{
                buf[i] = toupper(buf[i]);				
			}
            write(cfd, buf, rSize);
        }
        close(cfd);		
	}
	close(lfd);
}
	
