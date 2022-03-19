#include <stdio.h>
#include <winsock2.h>
#pragma comment (lib, "ws2_32")

int main()
{
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    
    // �����׽���
    SOCKET sClient = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
    
    // ��sockaddr_in�ṹ������ַ���˿ڵ���Ϣ
    struct sockaddr_in ServerAddr;
    ServerAddr.sin_family = AF_INET;
    ServerAddr.sin_addr.S_un.S_addr = inet_addr("10.10.30.77");
    ServerAddr.sin_port = htons(1234);
    
    // ������Ϣ
    char szMsg[MAXBYTE] = { 0 };
    lstrcpy(szMsg, "Hello Server!\r\n");
    int nSize = sizeof(ServerAddr);
    sendto(sClient, szMsg, strlen(szMsg) + sizeof(char), 0, (SOCKADDR*)&ServerAddr, nSize);

    // ������Ϣ
    nSize = sizeof(ServerAddr);
    recvfrom(sClient, szMsg, MAXBYTE, 0, (SOCKADDR*)&ServerAddr, &nSize);
    printf("Server Msg: %s \r\n", szMsg);
    
    WSACleanup();
    
    return 0;
}
