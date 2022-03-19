#include <stdio.h>
#include <winsock2.h>
#pragma comment (lib, "ws2_32")

int main()
{
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    
    // 创建套接字
    SOCKET sServer = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    
    // 对sockaddr_in结构体填充地址、端口等信息
    struct sockaddr_in ServerAddr;
    ServerAddr.sin_family = AF_INET;
    ServerAddr.sin_addr.S_un.S_addr = inet_addr("10.10.30.77");
    ServerAddr.sin_port = htons(1234);

    // 连接服务器
    connect(sServer, (SOCKADDR *)&ServerAddr, sizeof(ServerAddr));

    char szMsg[MAXBYTE] = { 0 };

    // 接收消息
    recv(sServer, szMsg, MAXBYTE, 0);  
    printf("Server Msg : %s \r\n", szMsg);
    
    // 发送消息
    lstrcpy(szMsg, "hello Server!\r\n");
    send(sServer, szMsg, strlen(szMsg) + sizeof(char), 0);
    
    WSACleanup();
    
    return 0;
}