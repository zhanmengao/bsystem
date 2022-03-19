#include <windows.h>
#include <stdio.h>

int g_Num_One = 0;
CRITICAL_SECTION g_cs;

DWORD WINAPI ThreadProc(LPVOID lpParam)
{
    int nTmp = 0;

    for ( int i = 0; i < 10; i ++ )
    {
        // 进入临界区
        EnterCriticalSection(&g_cs);

        nTmp = g_Num_One;
        nTmp ++;
        Sleep(1);
        g_Num_One = nTmp;

        // 离开临界区
        LeaveCriticalSection(&g_cs);
    }

    return 0;
}

int main()
{
    InitializeCriticalSection(&g_cs);

    HANDLE hThread[10] = { 0 };
    int i;
    for ( i = 0; i < 10; i ++ )
    {
        hThread[i] = CreateThread(NULL, 0, ThreadProc, NULL, 0, NULL);
    }
    
    WaitForMultipleObjects(10, hThread, TRUE, INFINITE);

    printf("g_Num_One = %d \r\n", g_Num_One);

    for ( i = 0; i < 10; i ++ )
    {
        CloseHandle(hThread[i]);
    }

    DeleteCriticalSection(&g_cs);
    return 0;
}