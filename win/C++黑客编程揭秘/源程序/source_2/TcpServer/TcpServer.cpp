#include <stdio.h>
#include <winsock2.h>
#pragma comment (lib, "ws2_32")

int main()
{
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    // �����׽���
    SOCKET sLisent = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    
    // ��sockaddr_in�ṹ������ַ���˿ڵ���Ϣ
    struct sockaddr_in ServerAddr;
    ServerAddr.sin_family = AF_INET;
    ServerAddr.sin_addr.S_un.S_addr = ADDR_ANY;
    ServerAddr.sin_port = htons(1234);
    
    // ���׽������ַ��Ϣ
    bind(sLisent, (SOCKADDR *)&ServerAddr, sizeof(ServerAddr));

    // SOMAXCONN = 0x7fffffff
    // �˿ڼ���
    listen(sLisent, SOMAXCONN);

    // ��ȡ��������
    sockaddr_in ClientAddr;
    int nSize = sizeof(ClientAddr);

    SOCKET sClient = accept(sLisent, (SOCKADDR *)&ClientAddr, &nSize);
    // ����ͻ���ʹ�õ�IP��ַ�Ͷ˿ں� 
    printf("ClientIP = %s : %d \r\n", inet_ntoa(ClientAddr.sin_addr), ntohs(ClientAddr.sin_port));

    // ������Ϣ
    char szMsg[MAXBYTE] = { 0 };
    lstrcpy(szMsg, "hello Client!\r\n");
    send(sClient, szMsg, strlen(szMsg) + sizeof(char), 0);

    // ������Ϣ
    recv(sClient, szMsg, MAXBYTE, 0);
    printf("Client Msg : %s \r\n", szMsg);

    WSACleanup();

    return 0;
}