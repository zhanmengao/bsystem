#include <Windows.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
    // �ҵ�ɨ����Ϸ��Ӧ�Ĵ��ھ���ͽ���ID
    HWND hWinmine = FindWindow(NULL, "ɨ��");
    DWORD dwPid = 0;
    GetWindowThreadProcessId(hWinmine, &dwPid);
    
    // ��ɨ����Ϸ��ȡ����
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
    
    PBYTE pByte = NULL;
    DWORD dwHight = 0, dwWidth = 0;
    
    DWORD dwAddr = 0x01005330;
    DWORD dwNum = 0;
    DWORD dwRead = 0;

    // ��ȡ�׵�������
    // ��ȡ�����Ŀ�͸�
    ReadProcessMemory(hProcess, (LPVOID)(dwAddr), 
                      &dwNum, sizeof(DWORD), &dwRead);
    ReadProcessMemory(hProcess, (LPVOID)(dwAddr + 4), 
                      &dwWidth, sizeof(DWORD), &dwRead);
    ReadProcessMemory(hProcess, (LPVOID)(dwAddr + 8), 
                      &dwHight, sizeof(DWORD), &dwRead);
    
    // ������ֻ���ɨ�׵ĸ߼�����
    // �����Ҫ�ж�һ�¸ߺͿ�
    if ( dwWidth != 30 || dwHight != 16 )
    {
        return 0;
    }

    DWORD dwBoomAddr = 0x01005340;
    // dwWidth * dwHight = ��Ϸ���ӵ�����
    // dwWidth * 2 = ����ǽ
    // dwHight * 2 = ����ǽ
    // 4 = 4���Ƕ�ǽ
    DWORD dwSize = dwWidth * dwHight + dwWidth * 2 + dwHight * 2 + 4;
    pByte = (PBYTE)malloc(dwSize);
    
    // ��ȡ��������������
    ReadProcessMemory(hProcess, (LPVOID)dwBoomAddr, pByte, dwSize, &dwRead);
    BYTE bClear = 0x8E;
    int i = 0;
    int n = dwNum;
    while( i < dwSize )
    {
        if ( pByte[i] == 0x8F )
        {
            DWORD dwAddr1 = 0x01005340 + i;
            WriteProcessMemory(hProcess, (LPVOID)dwAddr1, 
                               &bClear, sizeof(BYTE), &dwRead);
            n --;
        }
        
        i ++;
    }
    
    // ˢ��ɨ�׵Ŀͻ���
    RECT rt;
    GetClientRect(hWinmine, &rt);
    InvalidateRect(hWinmine, &rt, TRUE);
    
    free(pByte);
    
    printf("%d \r\n", n);
    
    CloseHandle(hProcess);
    
    return 0;
}