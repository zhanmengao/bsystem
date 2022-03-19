#include <stdio.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>


int GetLocalIp(char *ip)
{
	struct ifaddrs *ifAddrStruct;
	struct sockaddr_in *sin = NULL;
	void *tmpAddrPtr=NULL;
	getifaddrs(&ifAddrStruct);
	while (ifAddrStruct != NULL)
     {
		if (ifAddrStruct->ifa_addr->sa_family==AF_INET)
	     {
			tmpAddrPtr=&((struct sockaddr_in *)ifAddrStruct->ifa_addr)->sin_addr;
			inet_ntop(AF_INET, tmpAddrPtr, ip, INET_ADDRSTRLEN);
			printf("%s IP Address:%s\n", ifAddrStruct->ifa_name, ip);
			
			sin = (struct sockaddr_in *)ifAddrStruct->ifa_dstaddr;
			printf(">>> broadcast: %s\n", inet_ntoa(sin->sin_addr));
			
			sin = (struct sockaddr_in *)ifAddrStruct->ifa_netmask;
            printf(">>> subnetMask: %s\n", inet_ntoa(sin->sin_addr));
		}
		ifAddrStruct=ifAddrStruct->ifa_next;
	}
	//free ifaddrs
	freeifaddrs(ifAddrStruct);
	return 0;
}

int main()
{
	char ip[16];
	memset(ip, 0, sizeof(ip));
	GetLocalIp(ip);
	return 0;
}
