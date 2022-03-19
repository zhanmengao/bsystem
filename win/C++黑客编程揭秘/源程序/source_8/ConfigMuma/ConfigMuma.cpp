// ConfigMuma.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include <winsock2.h>
#pragma comment (lib, "ws2_32")

#define IPLEN 20

typedef struct _SCONFIG
{
    char szIpAddress[IPLEN];
    DWORD dwPort;
}SCONFIG, *PCONFIG;

int main(int argc, char* argv[])
{
    char szFileName[MAX_PATH] = { 0 };

    HANDLE hFile = NULL;
    SCONFIG IpConfig = { 0 };
    DWORD dwFileSize = 0;
    DWORD dwRead = 0;

    GetModuleFileName(NULL, szFileName, MAX_PATH);

    hFile = CreateFile(szFileName,
                GENERIC_READ,
                FILE_SHARE_READ,
                NULL,
                OPEN_EXISTING,
                FILE_ATTRIBUTE_NORMAL,
                0);

    if ( INVALID_HANDLE_VALUE == hFile )
    {
        return -1;
    }

    dwFileSize = GetFileSize(hFile, 0);
    // 定位到配置信息的位置
    SetFilePointer(hFile, dwFileSize - sizeof(SCONFIG), 0, FILE_BEGIN);
    // 读取配置信息
    ReadFile(hFile, (LPVOID)&IpConfig, sizeof(SCONFIG), &dwRead, NULL);

    CloseHandle(hFile);

    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);

    SOCKET s = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

    sockaddr_in sAddr = { 0 };

    sAddr.sin_family = PF_INET;
    // 连接目标的IP地址
    sAddr.sin_addr.S_un.S_addr = inet_addr(IpConfig.szIpAddress);
    // 连接目标的端口号
    sAddr.sin_port = htonl(IpConfig.dwPort);

    printf("connecting %s : %d \r\n", IpConfig.szIpAddress, IpConfig.dwPort);
    connect(s, (SOCKADDR *)&sAddr, sizeof(SOCKADDR));

    closesocket(s);

    return 0;
}
