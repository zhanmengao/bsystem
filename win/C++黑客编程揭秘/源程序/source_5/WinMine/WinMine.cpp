#include <Windows.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
    // 找到扫雷游戏对应的窗口句柄和进程ID
    HWND hWinmine = FindWindow(NULL, "扫雷");
    DWORD dwPid = 0;
    GetWindowThreadProcessId(hWinmine, &dwPid);
    
    // 打开扫雷游戏获取其句柄
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPid);
    
    PBYTE pByte = NULL;
    DWORD dwHight = 0, dwWidth = 0;
    
    DWORD dwAddr = 0x01005330;
    DWORD dwNum = 0;
    DWORD dwRead = 0;

    // 读取雷的数量级
    // 读取雷区的宽和高
    ReadProcessMemory(hProcess, (LPVOID)(dwAddr), 
                      &dwNum, sizeof(DWORD), &dwRead);
    ReadProcessMemory(hProcess, (LPVOID)(dwAddr + 4), 
                      &dwWidth, sizeof(DWORD), &dwRead);
    ReadProcessMemory(hProcess, (LPVOID)(dwAddr + 8), 
                      &dwHight, sizeof(DWORD), &dwRead);
    
    // 本代码只针对扫雷的高级级别
    // 因此需要判断一下高和宽
    if ( dwWidth != 30 || dwHight != 16 )
    {
        return 0;
    }

    DWORD dwBoomAddr = 0x01005340;
    // dwWidth * dwHight = 游戏格子的数量
    // dwWidth * 2 = 上下墙
    // dwHight * 2 = 左右墙
    // 4 = 4个角度墙
    DWORD dwSize = dwWidth * dwHight + dwWidth * 2 + dwHight * 2 + 4;
    pByte = (PBYTE)malloc(dwSize);
    
    // 读取整个雷区的数据
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
    
    // 刷新扫雷的客户区
    RECT rt;
    GetClientRect(hWinmine, &rt);
    InvalidateRect(hWinmine, &rt, TRUE);
    
    free(pByte);
    
    printf("%d \r\n", n);
    
    CloseHandle(hProcess);
    
    return 0;
}