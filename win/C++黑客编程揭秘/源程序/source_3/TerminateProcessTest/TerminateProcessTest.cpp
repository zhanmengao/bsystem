#include <Windows.h>

int main(int argc, char* argv[])
{
    HWND hNoteWnd = FindWindow(NULL, "�ޱ��� - ���±�");
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
        MessageBox(NULL, "�������̳ɹ�", NULL, MB_OK);
    }

    CloseHandle(hNoteHandle);
    
    return 0;
}
