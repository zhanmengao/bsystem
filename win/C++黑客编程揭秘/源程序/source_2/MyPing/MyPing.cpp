#include <stdio.h>
#include <winsock2.h>

#pragma comment (lib, "ws2_32")

struct icmp_header
{
    unsigned char icmp_type;    // 消息类型
    unsigned char icmp_code;    // 代码
    unsigned short icmp_checksum;    // 校验和
    unsigned short icmp_id;     // 用来惟一标识此请求的ID号，通常设置为进程ID
    unsigned short icmp_sequence;   // 序列号
    unsigned long icmp_timestamp;   // 时间戳
};

#define ICMP_HEADER_SIZE sizeof(icmp_header)
#define ICMP_ECHO_REQUEST 0x08
#define ICMP_ECHO_REPLY 0x00

// 计算校验和
unsigned short chsum(struct icmp_header *picmp, int len)
{
    long sum = 0;
    unsigned short *pusicmp = (unsigned short *)picmp;

    while ( len > 1 )
    {
        sum += *(pusicmp++);
        if ( sum & 0x80000000 )
        {
            sum = (sum & 0xffff) + (sum >> 16);
        }
        len -= 2;
    }

    if ( len )
    {
        sum += (unsigned short)*(unsigned char *)pusicmp;
    }

    while ( sum >> 16 )
    {
        sum = (sum & 0xffff) + (sum >> 16);
    }

    return (unsigned short)~sum;
}

BOOL MyPing(char *szDestIp)
{
    BOOL bRet = TRUE;
    WSADATA wsaData;
    int nTimeOut = 1000;
    char szBuff[ICMP_HEADER_SIZE + 32] = { 0 };
    icmp_header *pIcmp = (icmp_header *)szBuff;
    char icmp_data[32] = { 0 };

    WSAStartup(MAKEWORD(2, 2), &wsaData);
    // 创建原始套接字
    SOCKET s = socket(PF_INET, SOCK_RAW, IPPROTO_ICMP);
    
    // 设置接收超时
    setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (char const*)&nTimeOut, sizeof(nTimeOut));

    // 设置目的地址
    sockaddr_in dest_addr;
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_addr.S_un.S_addr = inet_addr(szDestIp);
    dest_addr.sin_port = htons(0);

    // 构造ICMP封包
    pIcmp->icmp_type = ICMP_ECHO_REQUEST;
    pIcmp->icmp_code = 0;
    pIcmp->icmp_id = (USHORT)::GetCurrentProcessId();
    pIcmp->icmp_sequence = 0;
    pIcmp->icmp_timestamp = 0;
    pIcmp->icmp_checksum = 0;

    // 拷贝数据
    // 这里的数据可以是任意的
    // 这里使用abc是为了和系统提供的看起来一样
    memcpy((szBuff + ICMP_HEADER_SIZE), "abcdefghijklmnopqrstuvwabcdefghi", 32);
    
    // 计算校验和
    pIcmp->icmp_checksum = chsum((struct icmp_header *)szBuff, sizeof(szBuff));

    sockaddr_in from_addr;
    char szRecvBuff[1024];
    int nLen = sizeof(from_addr);
    sendto(s, szBuff, sizeof(szBuff), 0, (SOCKADDR *)&dest_addr, sizeof(SOCKADDR));
    recvfrom(s, szRecvBuff, MAXBYTE, 0, (SOCKADDR *)&from_addr, &nLen);

    // 判断接收到的是否是自己请求的地址
    if ( lstrcmp(inet_ntoa(from_addr.sin_addr), szDestIp) )
    {
        bRet = FALSE;
    }
    else
    {
        struct icmp_header *pIcmp1 = (icmp_header *)(szRecvBuff + 20);
        printf("%s\r\n", inet_ntoa(from_addr.sin_addr));
    }

    return bRet;
}

int main(int argc, char **argv)
{
    int i = 0;
    while ( i < 1000 )
    {
        MyPing("8.8.4.4");
        Sleep(500);
        i ++;
    }

    return 0;
}