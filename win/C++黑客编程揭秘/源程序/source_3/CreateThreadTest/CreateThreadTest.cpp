#include <windows.h>
#include <stdio.h>

DWORD WINAPI ThreadProc(LPVOID lpParam)
{
    printf("ThreadProc \r\n");

    return 0;
}

int main()
{
    HANDLE hThread = CreateThread(NULL, 
                        0, 
                        ThreadProc, 
                        NULL, 
                        0, 
                        NULL);

    WaitForSingleObject(hThread, INFINITE);

    printf("main \r\n");

    CloseHandle(hThread);

    return 0;
}
