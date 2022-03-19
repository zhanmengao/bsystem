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

    // ��ʼ��SOCK��
    WSAStartup(MAKEWORD(2, 2), &wsa);

    // �����׽���
    s = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    
    bVal = TRUE;

    // �����׽���Ϊ����ģʽ
    if ( setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (char *)&bVal, sizeof(bVal)) != 0 )
    {
        printf("error! \r\n");

        return -1;
    }
    

    sockaddr_in sListen;
    sListen.sin_family = AF_INET;
    // �����IP��ַ������ȷָ��һ����ַ
    sListen.sin_addr.S_un.S_addr = inet_addr("192.168.1.102");
    sListen.sin_port = htons(21);

    // ��21�Ŷ˿�
    if ( bind(s, (SOCKADDR*)&sListen, sizeof(SOCKADDR)) == SOCKET_ERROR )
    {
        printf("%d\r\n", GetLastError());
        printf("error bind! \r\n");
        return -1;
    }

    // �����׽���
    listen(s, 1);

    // ѭ����������FTP�ͻ��˻�ľ�������
    while ( TRUE )
    {
        HANDLE hThread;

        nAddrSize = sizeof(SOCKADDR);
        // ��������
        sc = accept(s, (SOCKADDR*)&ClientAddr, &nAddrSize);
        if ( sc != INVALID_SOCKET )
        {
            // �������߳̽��д���
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
    // ������FTP�ͻ���ͨ�ŵ�SOCKET
    SOCKET sc = (SOCKET)lpParam;
    // ������FTP��������ͨ�ŵ�SOCKET
    SOCKET sFtp;
    sockaddr_in saddr;
    DWORD dwTimeOut;
    DWORD dwNum;
    BYTE bBuffer[0x1000] = { 0 };
    sFtp = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    
    saddr.sin_family = AF_INET;
    saddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
    saddr.sin_port = htons(21);
    
    // ���ó�ʱ
    dwTimeOut = 100;
    setsockopt(sc, SOL_SOCKET, SO_RCVTIMEO, 
                (char *)&dwTimeOut, sizeof(dwTimeOut));
    setsockopt(sFtp, SOL_SOCKET, SO_RCVTIMEO, 
                (char *)&dwTimeOut, sizeof(dwTimeOut));

    // ����FTP������
    connect(sFtp, (SOCKADDR*)&saddr, sizeof(SOCKADDR));

    // ѭ�����ܿͻ������������ͨ������
    while ( TRUE )
    {
        // ���տͻ��˵�����
        dwNum = recv(sc, (char *)bBuffer, 0x1000, 0);
        if ( dwNum > 0 && dwNum != SOCKET_ERROR )
        {
            bBuffer[dwNum] = '\0';
            printf("%s \r\n", bBuffer);
            // ת����FTP��������
            send(sFtp, (char *)bBuffer, dwNum, 0);
        }
        else if ( dwNum == 0 )
        {
            break;
        }

        ZeroMemory(bBuffer, 0x1000);
        
        // ����FTP�������˵�����
        dwNum = recv(sFtp, (char *)bBuffer, 0x1000, 0);
        if ( dwNum > 0 && dwNum != SOCKET_ERROR )
        {
            bBuffer[dwNum] = '\0';
            printf("%s \r\n", bBuffer);
            // ת�����ͻ���
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