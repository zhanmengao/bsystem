#include <windows.h>
#include <stdio.h>

int g_Num_One = 0;

DWORD WINAPI ThreadProc(LPVOID lpParam)
{
    int nTmp = 0;

    for ( int i = 0; i < 10; i ++ )
    {
        nTmp = g_Num_One;
        nTmp ++;
        // Sleep(1)的作用是让出CPU
        // 使其他线程被调度运行
        Sleep(1);
        g_Num_One = nTmp;
    }

    return 0;
}

int main()
{
    HANDLE hThread[10] = { 0 };
    int i;

    for ( i = 0; i < 10; i ++ )
    {
        hThread[i] = CreateThread(NULL, 0, ThreadProc, NULL, 0, NULL);
    }

    WaitForMultipleObjects(10, hThread, TRUE, INFINITE);

    for ( i = 0; i < 10; i ++ )
    {
        CloseHandle(hThread[i]);
    }

    printf("g_Num_One = %d \r\n", g_Num_One);

    return 0;
}