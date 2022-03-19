#include <Windows.h>

int main(int argc, char* argv[])
{
    HWND hNoteWnd = FindWindow(NULL, "无标题 - 记事本");
    if ( hNoteWnd == NULL )
    {
        return -1;
    }
    DWORD dwNotePid = 0;
    GetWindowThreadProcessId(hNoteWnd, &dwNotePid);
    if ( dwNotePid == 0 )
    {
        return -1;
    }
    
    HANDLE hNoteHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwNotePid);
    if ( hNoteHandle == NULL )
    {
        return -1;
    }

    BOOL bRet = TerminateProcess(hNoteHandle, 0);
    
    if ( bRet == TRUE )
    {
        MessageBox(NULL, "结束进程成功", NULL, MB_OK);
    }

    CloseHandle(hNoteHandle);
    
    return 0;
}
