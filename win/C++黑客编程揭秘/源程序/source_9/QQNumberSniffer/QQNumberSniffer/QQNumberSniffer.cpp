// QQNumberSniffer.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

#ifdef _MSC_VER
/*
 * we do not want the warnings about the old deprecated and unsecure CRT functions
 * since these examples can be compiled under *nix as well
 */
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <pcap.h>
#include <winsock2.h>


#pragma comment (lib, "wpcap")
#pragma comment (lib, "Packet")
#pragma comment (lib, "ws2_32")

/* 4 bytes IP address */
typedef struct ip_address
{
	u_char byte1;
	u_char byte2;
	u_char byte3;
	u_char byte4;
}ip_address;

/* IPv4 header */
typedef struct ip_header
{
	u_char	ver_ihl;		// Version (4 bits) + Internet header length (4 bits)
	u_char	tos;			// Type of service 
	u_short tlen;			// Total length 
	u_short identification; // Identification
	u_short flags_fo;		// Flags (3 bits) + Fragment offset (13 bits)
	u_char	ttl;			// Time to live
	u_char	proto;			// Protocol
	u_short crc;			// Header checksum
	ip_address	saddr;		// Source address
	ip_address	daddr;		// Destination address
	u_int	op_pad;			// Option + Padding
}ip_header;

/* UDP header*/
typedef struct udp_header
{
	u_short sport;			// Source port
	u_short dport;			// Destination port
	u_short len;			// Datagram length
	u_short crc;			// Checksum
}udp_header;

/* QQ header */
#pragma  pack(push,1)   
typedef struct qq_header
{
    u_char  flag;
    u_short version;
    u_short command;
    u_short seq;
    u_int   qq_number;
}qq_header;
#pragma  pack(pop,1) 

#define UDP_SIGN        17          // UDPЭ���ʶ
#define QQ_SER_PORT     8000        // QQʹ�õĶ˿ں�
#define QQ_SIGN         '\x02'      // QQЭ���ʶ

/* prototype of the packet handler */
void packet_handler(u_char *param, const struct pcap_pkthdr *header, const u_char *pkt_data);

int _tmain(int argc, _TCHAR* argv[])
{
	pcap_if_t *alldevs = NULL;
	pcap_if_t *d = NULL;
	int       inum = 0;
	int       i = 0;
	pcap_t    *adhandle = NULL;
    char      errbuf[PCAP_ERRBUF_SIZE] = { 0 };
	u_int     netmask;
	char      packet_filter[] = "ip and udp";
	struct bpf_program fcode;
	
	/* ���һ��������п���ʹ�õ�����ӿ� */
	if( pcap_findalldevs(&alldevs, errbuf) == -1 )
	{
		fprintf(stderr,"Error in pcap_findalldevs: %s\n", errbuf);
		exit(1);
	}
	
	/* �������ӿ��б� */
	for( d = alldevs; d; d = d->next)
	{
		printf("%d. %s", ++i, d->name);
		if (d->description)
        {
			printf(" (%s)\n", d->description);
        }
        else
		{
            printf(" (No description available)\n");
	
        }
    }
	
    if( i == 0 )
	{
		printf("\nNo interfaces found! Make sure WinPcap is installed.\n");
		return -1;
	}
	
    // ѡ��Ҫ����������ӿ�
	printf("Enter the interface number (1-%d):",i);
	scanf("%d", &inum);
	
	/* Check if the user specified a valid adapter */
	if( inum < 1 || inum > i )
	{
		printf("\nAdapter number out of range.\n");
		
		/* Free the device list */
		pcap_freealldevs(alldevs);
		return -1;
	}

	/* ���ѡ�������ӿ� */
	for( d = alldevs, i = 0; i < inum - 1; d = d->next, i++ );
	
	/* ������������ */
    adhandle = pcap_open_live(d->name,	        // �豸����
                              65536,			// 65535��֤�ܲ��񵽲�ͬ������·���ϵ�ÿ�����ݰ���ȫ������
                              1,				// ����ģʽ
                              1000,			    // ��ȡ��ʱʱ��
                              errbuf);			// ���󻺳���
                            

	if ( adhandle == NULL )
	{
		fprintf(stderr, "\nUnable to open the adapter. %s is not supported by WinPcap\n");
		/* �ͷ��豸�б� */
		pcap_freealldevs(alldevs);
		return -1;
	}
	
	/* ���������·�㣬Ϊ�˼򵥣�����ֻ������̫�� */
	if(pcap_datalink(adhandle) != DLT_EN10MB)
	{
		fprintf(stderr,"\nThis program works only on Ethernet networks.\n");
		/* �ͷ��豸�б� */
		pcap_freealldevs(alldevs);
		return -1;
	}
	
	if( d->addresses != NULL )
    {
		/* ��ýӿڵ�һ����ַ������ */
		netmask=((struct sockaddr_in *)(d->addresses->netmask))->sin_addr.S_un.S_addr;
    }
    else
    {
		/* ����ӿ�û�е�ַ����ô���Ǽ���һ��C������� */
		netmask=0xff0000; 
    }

	// ������˹���
	if (pcap_compile(adhandle, &fcode, packet_filter, 1, netmask) <0 )
	{
		fprintf(stderr,"\nUnable to compile the packet filter. Check the syntax.\n");
		/* �ͷ��豸�б� */
		pcap_freealldevs(alldevs);
		return -1;
	}
	
	// ���ù��˹���
	if (pcap_setfilter(adhandle, &fcode)<0)
	{
		fprintf(stderr,"\nError setting the filter.\n");
		/* �ͷ��豸�б� */
		pcap_freealldevs(alldevs);
		return -1;
	}
	
	printf("\nlistening on %s...\n", d->description);
	
	/* �ͷ��豸�б� */
	pcap_freealldevs(alldevs);
	
    /* ��ʼ���� */
    printf("��ʼ�������ݣ�\r\n");
    // packet_handler�ǻص�����
	pcap_loop(adhandle, 0, packet_handler, NULL);
	
	return 0;
}

/* ÿ�β������ݰ�ʱ��libpcap�����Զ���������ص����� */
void packet_handler(u_char *param, const struct pcap_pkthdr *header, const u_char *pkt_data)
{
	ip_header  *ih = NULL;
	udp_header *uh = NULL;
	u_int      ip_len = 0;
	u_short    sport = 0, dport = 0;
    qq_header  *qh = NULL;

    PBYTE pByte = NULL;
    
	/* ���IP���ݰ�ͷ����λ�� */
    ih = (ip_header *) (pkt_data + 14); // 14����̫��ͷ������

	/* ���UDP�ײ���λ�� */
	ip_len = (ih->ver_ihl & 0xf) * 4;

    // �ж��Ƿ�ΪUDPЭ��
    if ( ih->proto == UDP_SIGN )
    {
        uh = (udp_header *) ((u_char*)ih + ip_len);
        /* �������ֽ�����ת���������ֽ����� */
        sport = ntohs( uh->sport );
        dport = ntohs( uh->dport );

        // �ж�Դ�˿ڻ�Ŀ�Ķ˿��Ƿ�Ϊ8000
        if ( sport == QQ_SER_PORT || dport == QQ_SER_PORT )
        {
            pByte = (PBYTE)ih + ip_len + sizeof(udp_header);

            if ( *pByte == QQ_SIGN )
            {
                qh = (qq_header *)pByte;
                int n = ntohl(qh->qq_number);
                printf("QQ = %u, %d.%d.%d.%d:%d - > %d.%d.%d.%d:%d\r\n",
                    n,
                    ih->saddr.byte1,
                    ih->saddr.byte2,
                    ih->saddr.byte3,
                    ih->saddr.byte4,
                    sport,
                    ih->daddr.byte1,
                    ih->daddr.byte2,
                    ih->daddr.byte3,
                    ih->daddr.byte4,
                    dport);
            }
        }
    }
}