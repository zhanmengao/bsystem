#ifndef _NETINFO_H
#define _NETINFO_H
//IP MAC
struct NetInfo
{
	char IP[64];
	char Mac[64];
};
#include<string>
#ifdef _WIN32

#include <ws2tcpip.h>
#include <iphlpapi.h>
#include"../../../C++space/Convert/Convert/CppConvert.h"
#include"../../SystemInfo/SystemInfo/SystemInfo/SysInfoUtils.h"
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "iphlpapi.lib")
static ULONG GetHostOrder(const char* ip)
{
	ULONG iIp = inet_addr(ip);
	iIp = ntohl(iIp);
	return iIp;
}
static void GetNetInfo(struct NetInfo* pInfo)
{
	ULONG buffer_size = 0;
	GetAdaptersInfo(NULL, &buffer_size);
	PIP_ADAPTER_INFO p_ip_info = (IP_ADAPTER_INFO *)malloc(buffer_size);
	GetAdaptersInfo(p_ip_info, &buffer_size);
	while (p_ip_info)
	{
		ULONG gate_way = GetHostOrder(p_ip_info->GatewayList.IpAddress.String);
		IP_ADDR_STRING* p_ip_item = &p_ip_info->IpAddressList;
		int is_right_ip = 0;
		while (p_ip_item)
		{
			ULONG tpIp = GetHostOrder(p_ip_item->IpAddress.String);
			strcpy(pInfo->IP, p_ip_item->IpAddress.String);

			ULONG net_mask = GetHostOrder(p_ip_item->IpMask.String);
			ULONG sub_net = gate_way & net_mask;
			ULONG sub_net_x = tpIp & net_mask;
			if (tpIp != 0)
			{
				if (sub_net == sub_net_x)
				{
					is_right_ip = 1;
					break;
				}
			}
			p_ip_item = p_ip_item->Next;
		}

		if (is_right_ip)
		{
			UCHAR mac[MAX_PATH] = { 0 };
			int iCount = 0;

			for (int i = 0; i<(int)p_ip_info->AddressLength; i++)
			{
				Byte2Hex(p_ip_info->Address[i], &mac[iCount]);
				iCount += 2;
				if (i<(int)p_ip_info->AddressLength - 1)
				{
					mac[iCount++] = ':';
				}
			}
			strcpy(pInfo->Mac, (char*)mac);
			break;
		}
		p_ip_info = p_ip_info->Next;
	}
	if (p_ip_info)
	{
		free(p_ip_info);
		p_ip_info = NULL;
	}
}


#else
#include <stdio.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <errno.h>
#include<unistd.h>
#ifdef __cplusplus
#include<string>
#include<vector>
#endif
static int GetHostOrder(char* ip,unsigned ipsize,char* mac,unsigned macsize)
{
    int fd;
    int interfaceNum = 0;
    struct ifreq buf[16];
    struct ifconf ifc;
    struct ifreq ifrcopy;

    if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("socket");
        close(fd);
        return -1;
    }

    ifc.ifc_len = sizeof(buf);
    ifc.ifc_buf = (caddr_t)buf;
    if (!ioctl(fd, SIOCGIFCONF, (char *)&ifc))
    {
        interfaceNum = ifc.ifc_len / sizeof(struct ifreq);
        while (interfaceNum-- > 0)
        {
            //ignore the interface that not up or not runing
            ifrcopy = buf[interfaceNum];
            if (ioctl(fd, SIOCGIFFLAGS, &ifrcopy))
            {
                printf("ioctl: %s [%s:%d]\n", strerror(errno), __FILE__, __LINE__);
                close(fd);
                return -1;
            }
            //get the IP of this interface
            if (!ioctl(fd, SIOCGIFADDR, (char *)&buf[interfaceNum]))
            {
                char tmp[64] = {0};
                snprintf(tmp, ipsize, "%s",
                         (char *)inet_ntoa(((struct sockaddr_in *)&(buf[interfaceNum].ifr_addr))->sin_addr));
                if(strcmp("127.0.0.1",tmp) == 0)
                {
                    continue;
                }
                strcpy(ip,tmp);
                printf("device ip: %s\n", ip);
            }
            else
            {
                printf("ioctl: %s [%s:%d]\n", strerror(errno), __FILE__, __LINE__);
                close(fd);
                return -1;
            }
            //get the mac of this interface
            if (!ioctl(fd, SIOCGIFHWADDR, (char *)(&buf[interfaceNum])))
            {
                memset(mac, 0, macsize);
                snprintf(mac, macsize, "%02x:%02x:%02x:%02x:%02x:%02x",
                         (unsigned char)buf[interfaceNum].ifr_hwaddr.sa_data[0],
                         (unsigned char)buf[interfaceNum].ifr_hwaddr.sa_data[1],
                         (unsigned char)buf[interfaceNum].ifr_hwaddr.sa_data[2],

                         (unsigned char)buf[interfaceNum].ifr_hwaddr.sa_data[3],
                         (unsigned char)buf[interfaceNum].ifr_hwaddr.sa_data[4],
                         (unsigned char)buf[interfaceNum].ifr_hwaddr.sa_data[5]);
                printf("device mac: %s\n", mac);
            }
            else
            {
                printf("ioctl: %s [%s:%d]\n", strerror(errno), __FILE__, __LINE__);
                close(fd);
                return -1;
            }
        }
    }
    else
    {
        printf("ioctl: %s [%s:%d]\n", strerror(errno), __FILE__, __LINE__);
        close(fd);
        return -1;
    }
    close(fd);
    return 0;
}
static void GetNetInfo(struct NetInfo* pInfo)
{
    GetHostOrder(pInfo->IP,sizeof(pInfo->IP),pInfo->Mac,sizeof(pInfo->Mac));
}

#ifdef __cplusplus
struct NetStat
{
    char state[32];
    char type[16];
    pid_t pid;
    char process[256];
};
static void GetNetStat(std::vector<NetStat>& vec)
{

}
#endif
#endif

std::string GetMac(const char* ipaddr);
#endif
