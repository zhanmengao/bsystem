#include <stdio.h>
#include <winsock2.h>
#pragma comment (lib, "ws2_32")

int main()
{
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    // 创建套接字
    SOCKET sLisent = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    
    // 对sockaddr_in结构体填充地址、端口等信息
    struct sockaddr_in ServerAddr;
    ServerAddr.sin_family = AF_INET;
    ServerAddr.sin_addr.S_un.S_addr = ADDR_ANY;
    ServerAddr.sin_port = htons(1234);
    
    // 绑定套接字与地址信息
    bind(sLisent, (SOCKADDR *)&ServerAddr, sizeof(ServerAddr));

    // SOMAXCONN = 0x7fffffff
    // 端口监听
    listen(sLisent, SOMAXCONN);

    // 获取连接请求
    sockaddr_in ClientAddr;
    int nSize = sizeof(ClientAddr);

    SOCKET sClient = accept(sLisent, (SOCKADDR *)&ClientAddr, &nSize);
    // 输出客户端使用的IP地址和端口号 
    printf("ClientIP = %s : %d \r\n", inet_ntoa(ClientAddr.sin_addr), ntohs(ClientAddr.sin_port));

    // 发送消息
    char szMsg[MAXBYTE] = { 0 };
    lstrcpy(szMsg, "hello Client!\r\n");
    send(sClient, szMsg, strlen(szMsg) + sizeof(char), 0);

    // 接收消息
    recv(sClient, szMsg, MAXBYTE, 0);
    printf("Client Msg : %s \r\n", szMsg);

    WSACleanup();

    return 0;
}