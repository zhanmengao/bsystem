#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <linux/if_packet.h>
#include <netinet/if_ether.h>
#include <netinet/in.h>
#include <unistd.h> //for close


typedef struct _iphdr //����IP�ײ� 
{ 
	unsigned char h_verlen; //4λ�ײ�����+4λIP�汾�� 
	unsigned char tos; //8λ��������TOS 
	unsigned short total_len; //16λ�ܳ��ȣ��ֽڣ� 
	unsigned short ident; //16λ��ʶ 
	unsigned short frag_and_flags; //3λ��־λ 
	unsigned char ttl; //8λ����ʱ�� TTL 
	unsigned char proto; //8λЭ�� (TCP, UDP ������) 
	unsigned short checksum; //16λIP�ײ�У��� 
	unsigned int sourceIP; //32λԴIP��ַ 
	unsigned int destIP; //32λĿ��IP��ַ 
}IP_HEADER; 

typedef struct _udphdr //����UDP�ײ�
{
	unsigned short uh_sport;    //16λԴ�˿�
	unsigned short uh_dport;    //16λĿ�Ķ˿�
	unsigned int uh_len;//16λUDP������
	unsigned int uh_sum;//16λУ���
}UDP_HEADER;

typedef struct _tcphdr //����TCP�ײ� 
{ 
	unsigned short th_sport; //16λԴ�˿� 
	unsigned short th_dport; //16λĿ�Ķ˿� 
	unsigned int th_seq; //32λ���к� 
	unsigned int th_ack; //32λȷ�Ϻ� 
	unsigned char th_lenres;//4λ�ײ�����/6λ������ 
	unsigned char th_flag; //6λ��־λ
	unsigned short th_win; //16λ���ڴ�С
	unsigned short th_sum; //16λУ���
	unsigned short th_urp; //16λ��������ƫ����
}TCP_HEADER; 

typedef struct _icmphdr {  
	unsigned char  icmp_type;  
	unsigned char icmp_code; /* type sub code */  
	unsigned short icmp_cksum;  
	unsigned short icmp_id;  
	unsigned short icmp_seq;  
	/* This is not the std header, but we reserve space for time */  
	unsigned short icmp_timestamp;  
}ICMP_HEADER;

void analyseIP(IP_HEADER *ip);
void analyseTCP(TCP_HEADER *tcp);
void analyseUDP(UDP_HEADER *udp);
void analyseICMP(ICMP_HEADER *icmp);


int main(void)
{
	int sockfd;
	IP_HEADER *ip;
	char buf[10240];
	ssize_t n;
	/* capture ip datagram without ethernet header */
	if ((sockfd = socket(PF_PACKET, SOCK_DGRAM, htons(ETH_P_IP))) == -1)
	{    
		printf("socket error!\n");
		return 1;
	}
	while (1)
	{
		n = recv(sockfd, buf, sizeof(buf), 0);
		if (n == -1)
		{
			printf("recv error!\n");
			break;
		}
		else if (n == 0)
			continue;
		//�������ݲ�����������·֡ͷ
		ip = (IP_HEADER *)(buf);
		analyseIP(ip);
		size_t iplen =  (ip->h_verlen & 0x0f) * 4;
		TCP_HEADER *tcp = (TCP_HEADER *)(buf + iplen);
		if (ip->proto == IPPROTO_TCP)
		{
			TCP_HEADER *tcp = (TCP_HEADER *)(buf + iplen);
			analyseTCP(tcp);
		}
		else if (ip->proto == IPPROTO_UDP)
		{
			UDP_HEADER *udp = (UDP_HEADER *)(buf + iplen);
			analyseUDP(udp);
		}
		else if (ip->proto == IPPROTO_ICMP)
		{
			ICMP_HEADER *icmp = (ICMP_HEADER *)(buf + iplen);
			analyseICMP(icmp);
		}
		else if (ip->proto == IPPROTO_IGMP)
		{
			printf("IGMP----\n");
		}
		else
		{
			printf("other protocol!\n");
		}        
		printf("\n\n");
	}
	close(sockfd);
	return 0;
}

void analyseIP(IP_HEADER *ip)
{
	unsigned char* p = (unsigned char*)&ip->sourceIP;
	printf("Source IP\t: %u.%u.%u.%u\n", p[0], p[1], p[2], p[3]);
	p = (unsigned char*)&ip->destIP;
	printf("Destination IP\t: %u.%u.%u.%u\n", p[0], p[1], p[2], p[3]);

}

void analyseTCP(TCP_HEADER *tcp)
{
	printf("TCP -----\n");
	printf("Source port: %u\n", ntohs(tcp->th_sport));
	printf("Dest port: %u\n", ntohs(tcp->th_dport));
}

void analyseUDP(UDP_HEADER *udp)
{
	printf("UDP -----\n");
	printf("Source port: %u\n", ntohs(udp->uh_sport));
	printf("Dest port: %u\n", ntohs(udp->uh_dport));
}

void analyseICMP(ICMP_HEADER *icmp)
{
	printf("ICMP -----\n");
	printf("type: %u\n", icmp->icmp_type);
	printf("sub code: %u\n", icmp->icmp_code);
}