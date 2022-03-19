#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>         
#include <sys/socket.h>
#include <strings.h>
#include <string.h>
#include <ctype.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include <stddef.h>

#define SERVER_SOCK "domainserv.socket"
#define CLIENT_SOCK "domainclie.socket"
void main()
{
	int cfd = socket(AF_UNIX,SOCK_STREAM,0);
	struct sockaddr_un clieAddr;
	memset(&clieAddr,0,sizeof(clieAddr));
	clieAddr.sun_family = AF_UNIX;
	strcpy(clieAddr.sun_path,CLIENT_SOCK);
	int addrSize = offsetof(struct sockaddr_un,sun_path) + strlen(clieAddr.sun_path);			//拿到结构体实际大小
	unlink(CLIENT_SOCK);														//删除sock文件，否则会bind失败
	bind(cfd,(struct sockaddr *)&clieAddr,addrSize);						//客户端不能依赖隐式绑定，因为需要一个文件名
	
	while(1)
	{
		struct sockaddr_un sevrAddr;
		memset(&sevrAddr,0,sizeof(sevrAddr));
		sevrAddr.sun_family = AF_UNIX;
		strcpy(sevrAddr.sun_path,SERVER_SOCK);
		int sevrLen = offsetof(struct sockaddr_un, sun_path) + strlen(sevrAddr.sun_path);   /* 计算服务器端地址结构有效长度 */
		connect(cfd, (struct sockaddr *)&sevrAddr,sevrLen);
		char buf[1024] = {0};
		while (fgets(buf, sizeof(buf), stdin) != NULL) 
		{
			write(cfd, buf, strlen(buf));
			sevrLen = read(cfd, buf, sizeof(buf));
			write(STDOUT_FILENO, buf, sevrLen);
		}
		
	}
	close(cfd);
}
