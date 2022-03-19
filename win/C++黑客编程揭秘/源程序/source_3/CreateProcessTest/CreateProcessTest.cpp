#include <windows.h>
#include <stdio.h>

#define EXEC_FILE "c:\\windows\\system32\\notepad.exe"

int main()
{
    PROCESS_INFORMATION pi = { 0 };
    STARTUPINFO si = { 0 };
    
    si.cb = sizeof(STARTUPINFO);
    
    BOOL bRet = CreateProcess(EXEC_FILE,
                        NULL, NULL, NULL, FALSE,
                        NULL, NULL, NULL, &si, &pi);
    
    if ( bRet == FALSE )
    {
        printf("CreateProcess Error ! \r\n");
        return -1;
    }
    
    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);

    return 0;
}