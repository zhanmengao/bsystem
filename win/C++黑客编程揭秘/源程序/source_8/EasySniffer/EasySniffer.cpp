#include <winsock2.h>
#include <stdio.h>
#include <mstcpip.h>

#pragma comment(lib, "Advapi32.lib")
#pragma comment (lib, "ws2_32")

// 协议
#define PROTO_ICMP    1
#define PROTO_IGMP    2
#define PROTO_TCP     6
#define PROTO_UDP     17

typedef struct _IPHeader		// 20字节的IP头
{
    UCHAR     iphVerLen;      // 版本号和头长度（各占4位）
    UCHAR     ipTOS;          // 服务类型
    USHORT    ipLength;       // 封包总长度，即整个IP报的长度
    USHORT    ipID;			  // 封包标识，惟一标识发送的每一个数据报
    USHORT    ipFlags;	      // 标志
    UCHAR     ipTTL;	      // 生存时间，就是TTL
    UCHAR     ipProtocol;     // 协议，可能是TCP、UDP、ICMP等
    USHORT    ipChecksum;     // 校验和
    ULONG     ipSource;       // 源IP地址
    ULONG     ipDestination;  // 目标IP地址
} IPHeader, *PIPHeader; 

// 定义TCP标志
#define   TCP_FIN   0x01
#define   TCP_SYN   0x02
#define   TCP_RST   0x04
#define   TCP_PSH   0x08
#define   TCP_ACK   0x10
#define   TCP_URG   0x20
#define   TCP_ACE   0x40
#define   TCP_CWR   0x80

typedef struct _TCPHeader		// 20字节的TCP头
{
    USHORT	sourcePort;			// 16位源端口号
    USHORT	destinationPort;	// 16位目的端口号
    ULONG	sequenceNumber;		// 32位序列号
    ULONG	acknowledgeNumber;	// 32位确认号
    UCHAR	dataoffset;			// 高4位表示数据偏移
    UCHAR	flags;				// 6位标志位    
    USHORT	windows;			// 16位窗口大小
    USHORT	checksum;			// 16位校验和
    USHORT	urgentPointer;		// 16位紧急数据偏移量 
} TCPHeader, *PTCPHeader;

typedef struct _UDPHeader
{
    USHORT			sourcePort;		// 源端口号		
    USHORT			destinationPort;// 目的端口号		
    USHORT			len;			// 封包长度
    USHORT			checksum;		// 校验和
} UDPHeader, *PUDPHeader;

void DecodeTCPPacket(char *pData, char *szSrcIP, char *szDestIp)
{
	TCPHeader *pTCPHdr = (TCPHeader *)pData;

	printf("%s:%d -> %s:%d\r\n", 
            szSrcIP, 
            ntohs(pTCPHdr->sourcePort), 
            szDestIp, 
            ntohs(pTCPHdr->destinationPort));
	
	// 下面还可以根据目的端口号进一步解析应用层协议
	switch(::ntohs(pTCPHdr->destinationPort))
	{
	case 21:
        // 解析FTP的用户名和密码
        printf("FTP========================================\r\n");
        pData = pData + sizeof(TCPHeader);
        if ( strncmp(pData, "USER ", 5) == 0 )
        {
            printf("Ftp UserName : %s \r\n", pData + 4);
        }
        if ( strncmp(pData, "PASS ", 5) == 0 )
        {
            printf("Ftp Password : %s \r\n", pData + 4);
        }
        printf("FTP========================================\r\n");
		break;
	case 80:
    case 8080:
        // 直接输出浏览器获取到的内容
        printf("WEB========================================\r\n");
        printf("%s\r\n", pData + sizeof(TCPHeader));
        printf("WEB========================================\r\n");
		break;
	}
}

void DecodeUDPPacket(char *pData, char *szSrcIP, char *szDestIp)
{
    UDPHeader *pUDPHdr = (UDPHeader *)pData;
    
    printf("%s:%d -> %s:%d\r\n", 
            szSrcIP, 
            ntohs(pUDPHdr->sourcePort), 
            szDestIp, 
            ntohs(pUDPHdr->destinationPort));
}

void DecodeIPPacket(char *pData)
{	
    IPHeader *pIPHdr = (IPHeader*)pData;

    in_addr source, dest;
    char szSourceIp[32], szDestIp[32]; 

    printf("-------------------------------\r\n");

    // 从IP头中取出源IP地址和目的IP地址
    source.S_un.S_addr = pIPHdr->ipSource;
    dest.S_un.S_addr = pIPHdr->ipDestination;
    strcpy(szSourceIp, inet_ntoa(source));
    strcpy(szDestIp, inet_ntoa(dest));

    // IP头长度
	int nHeaderLen = (pIPHdr->iphVerLen & 0xf) * sizeof(ULONG);

	switch( pIPHdr->ipProtocol )
	{
	case IPPROTO_TCP: // TCP协议
		DecodeTCPPacket(pData + nHeaderLen, szSourceIp, szDestIp);
		break;
	case IPPROTO_UDP:
        DecodeUDPPacket(pData + nHeaderLen, szSourceIp, szDestIp);
		break;
	case IPPROTO_ICMP:
		break; 
	}
}


int main()
{
    WSADATA wsa;

    WSAStartup(MAKEWORD(2, 2), &wsa);

	// 创建原始套节字
	SOCKET sRaw = socket(AF_INET, SOCK_RAW, IPPROTO_IP);

	// 获取本地IP地址
	char szHostName[56];
	SOCKADDR_IN addr_in;
	struct hostent *pHost;
	gethostname(szHostName, 56);
	if( (pHost = gethostbyname((char*)szHostName)) == NULL )	
    {
        return -1;
    }

	// 在调用ioctl之前，套节字必须绑定
	addr_in.sin_family  = AF_INET;
	addr_in.sin_port    = htons(0);
	memcpy(&addr_in.sin_addr.S_un.S_addr, pHost->h_addr_list[0], pHost->h_length);

	printf("Binding to interface : %s \r\n", ::inet_ntoa(addr_in.sin_addr));
	if( bind(sRaw, (PSOCKADDR)&addr_in, sizeof(addr_in)) == SOCKET_ERROR )
    {
		return -1;
    }

	// 设置SIO_RCVALL控制代码，以便接收所有的IP包	
	DWORD dwValue = 1;
	if( ioctlsocket(sRaw, SIO_RCVALL, &dwValue) != 0 )
    {
		return -1;
    }

	// 开始接收封包
	char buff[1024];
	int nRet;
	while(TRUE)
	{
		nRet = recv(sRaw, buff, 1024, 0);
		if( nRet > 0 )
		{
			DecodeIPPacket(buff);
		}
	}
	closesocket(sRaw);

    WSACleanup();

    return 0;
}


