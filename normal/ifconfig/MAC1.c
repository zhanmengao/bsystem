#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <net/if.h>
#include <string.h>

int main(int argc, char *argv[])
{

        struct ifreq ifreq;
        int sock;

        if(argc!=2)
        {
                printf("Usage : ethname\n");
                return 1;
        }
        if((sock=socket(AF_INET,SOCK_STREAM,0))<0)
        {
                perror("socket");
                return 2;
        }
        strcpy(ifreq.ifr_name,argv[1]);
        if(ioctl(sock,SIOCGIFHWADDR,&ifreq)<0)
        {
                perror("ioctl");
                return 3;
        }
        printf("%02x:%02x:%02x:%02x:%02x:%02x\n",
                        (unsigned char)ifreq.ifr_hwaddr.sa_data[0],
                        (unsigned char)ifreq.ifr_hwaddr.sa_data[1],
                        (unsigned char)ifreq.ifr_hwaddr.sa_data[2],
                        (unsigned char)ifreq.ifr_hwaddr.sa_data[3],
                        (unsigned char)ifreq.ifr_hwaddr.sa_data[4],
                        (unsigned char)ifreq.ifr_hwaddr.sa_data[5]);
        return 0;
}
