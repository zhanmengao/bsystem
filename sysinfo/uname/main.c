#include"GetSystemInfo.hpp"
#include<stdio.h>
int main()
{
	char buffer[1024] = {0};
	GetHostName(buffer,sizeof(buffer));
	printf("Host name = %s \n",buffer);
	
	GetSystemName(buffer,sizeof(buffer));
	printf("GetSystemName = %s \n",buffer);
	
	GetNodeName(buffer,sizeof(buffer));
	printf("GetNodeName = %s \n",buffer);	
	
	GetReleaseLevel(buffer,sizeof(buffer));
	printf("GetReleaseLevel = %s \n",buffer);	
	
	GetVersion(buffer,sizeof(buffer));
	printf("GetVersion = %s \n",buffer);	
	
	GetMachine(buffer,sizeof(buffer));
	printf("GetMachine = %s \n",buffer);	
	
	GetDomainName(buffer,sizeof(buffer));
	printf("GetDomainName = %s \n",buffer);	
	return 0;
}
