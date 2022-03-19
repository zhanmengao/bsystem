#include <winsock2.h>
#include <stdio.h>
#include <mstcpip.h>

#pragma comment(lib, "Advapi32.lib")
#pragma comment (lib, "ws2_32")

// Э��
#define PROTO_ICMP    1
#define PROTO_IGMP    2
#define PROTO_TCP     6
#define PROTO_UDP     17

typedef struct _IPHeader		// 20�ֽڵ�IPͷ
{
    UCHAR     iphVerLen;      // �汾�ź�ͷ���ȣ���ռ4λ��
    UCHAR     ipTOS;          // ��������
    USHORT    ipLength;       // ����ܳ��ȣ�������IP���ĳ���
    USHORT    ipID;			  // �����ʶ��Ωһ��ʶ���͵�ÿһ�����ݱ�
    USHORT    ipFlags;	      // ��־
    UCHAR     ipTTL;	      // ����ʱ�䣬����TTL
    UCHAR     ipProtocol;     // Э�飬������TCP��UDP��ICMP��
    USHORT    ipChecksum;     // У���
    ULONG     ipSource;       // ԴIP��ַ
    ULONG     ipDestination;  // Ŀ��IP��ַ
} IPHeader, *PIPHeader; 

// ����TCP��־
#define   TCP_FIN   0x01
#define   TCP_SYN   0x02
#define   TCP_RST   0x04
#define   TCP_PSH   0x08
#define   TCP_ACK   0x10
#define   TCP_URG   0x20
#define   TCP_ACE   0x40
#define   TCP_CWR   0x80

typedef struct _TCPHeader		// 20�ֽڵ�TCPͷ
{
    USHORT	sourcePort;			// 16λԴ�˿ں�
    USHORT	destinationPort;	// 16λĿ�Ķ˿ں�
    ULONG	sequenceNumber;		// 32λ���к�
    ULONG	acknowledgeNumber;	// 32λȷ�Ϻ�
    UCHAR	dataoffset;			// ��4λ��ʾ����ƫ��
    UCHAR	flags;				// 6λ��־λ    
    USHORT	windows;			// 16λ���ڴ�С
    USHORT	checksum;			// 16λУ���
    USHORT	urgentPointer;		// 16λ��������ƫ���� 
} TCPHeader, *PTCPHeader;

typedef struct _UDPHeader
{
    USHORT			sourcePort;		// Դ�˿ں�		
    USHORT			destinationPort;// Ŀ�Ķ˿ں�		
    USHORT			len;			// �������
    USHORT			checksum;		// У���
} UDPHeader, *PUDPHeader;

void DecodeTCPPacket(char *pData, char *szSrcIP, char *szDestIp)
{
	TCPHeader *pTCPHdr = (TCPHeader *)pData;

	printf("%s:%d -> %s:%d\r\n", 
            szSrcIP, 
            ntohs(pTCPHdr->sourcePort), 
            szDestIp, 
            ntohs(pTCPHdr->destinationPort));
	
	// ���滹���Ը���Ŀ�Ķ˿ںŽ�һ������Ӧ�ò�Э��
	switch(::ntohs(pTCPHdr->destinationPort))
	{
	case 21:
        // ����FTP���û���������
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
        // ֱ������������ȡ��������
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

    // ��IPͷ��ȡ��ԴIP��ַ��Ŀ��IP��ַ
    source.S_un.S_addr = pIPHdr->ipSource;
    dest.S_un.S_addr = pIPHdr->ipDestination;
    strcpy(szSourceIp, inet_ntoa(source));
    strcpy(szDestIp, inet_ntoa(dest));

    // IPͷ����
	int nHeaderLen = (pIPHdr->iphVerLen & 0xf) * sizeof(ULONG);

	switch( pIPHdr->ipProtocol )
	{
	case IPPROTO_TCP: // TCPЭ��
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

	// ����ԭʼ�׽���
	SOCKET sRaw = socket(AF_INET, SOCK_RAW, IPPROTO_IP);

	// ��ȡ����IP��ַ
	char szHostName[56];
	SOCKADDR_IN addr_in;
	struct hostent *pHost;
	gethostname(szHostName, 56);
	if( (pHost = gethostbyname((char*)szHostName)) == NULL )	
    {
        return -1;
    }

	// �ڵ���ioctl֮ǰ���׽��ֱ����
	addr_in.sin_family  = AF_INET;
	addr_in.sin_port    = htons(0);
	memcpy(&addr_in.sin_addr.S_un.S_addr, pHost->h_addr_list[0], pHost->h_length);

	printf("Binding to interface : %s \r\n", ::inet_ntoa(addr_in.sin_addr));
	if( bind(sRaw, (PSOCKADDR)&addr_in, sizeof(addr_in)) == SOCKET_ERROR )
    {
		return -1;
    }

	// ����SIO_RCVALL���ƴ��룬�Ա�������е�IP��	
	DWORD dwValue = 1;
	if( ioctlsocket(sRaw, SIO_RCVALL, &dwValue) != 0 )
    {
		return -1;
    }

	// ��ʼ���շ��
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


