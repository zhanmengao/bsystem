#include <stdio.h>
#include <winsock2.h>

#pragma comment (lib, "ws2_32")

int main()
{
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    // �����׽���
    SOCKET sServer = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
    
    // ��sockaddr_in�ṹ������ַ���˿ڵ���Ϣ
    struct sockaddr_in ServerAddr;
    ServerAddr.sin_family = AF_INET;
    ServerAddr.sin_addr.S_un.S_addr = inet_addr("10.10.30.77");
    ServerAddr.sin_port = htons(1234);
    
    // ���׽������ַ��Ϣ
    bind(sServer, (SOCKADDR *)&ServerAddr, sizeof(ServerAddr));

    // ������Ϣ
    char szMsg[MAXBYTE] = { 0 };
    struct sockaddr_in ClientAddr;
    int nSize = sizeof(ClientAddr);
    recvfrom(sServer, szMsg, MAXBYTE, 0, (SOCKADDR*)&ClientAddr, &nSize);
    printf("Client Msg: %s \r\n", szMsg);

    printf("ClientIP = %s : %d \r\n", inet_ntoa(ClientAddr.sin_addr), ntohs(ClientAddr.sin_port));

    // ������Ϣ
    lstrcpy(szMsg, "Hello Client!\r\n");
    nSize = sizeof(ClientAddr);
    sendto(sServer, szMsg, strlen(szMsg) + sizeof(char), 0, (SOCKADDR*)&ClientAddr, nSize);
    
    WSACleanup();
    
    return 0;
}