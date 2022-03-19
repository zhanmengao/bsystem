#include <Windows.h>
#include "ILHook.h"

// ����һ��ȫ�ֵı���
CILHook MsgHook;


// �Զ����MessageBoxA����
int
/*WINAPI*/
MyMessageBoxA(
        HWND hWnd,
        LPCSTR lpText,
        LPCSTR lpCaption,
        UINT uType)
{
    // �ָ�HOOK
    MsgHook.UnHook();
    MessageBox(hWnd, "Hook����", lpCaption, uType);
    MessageBox(hWnd, lpText, lpCaption, uType);
    // ����HOOK
    MsgHook.ReHook();

    __asm
    {
        pop edi
        pop esi
        pop ebx
        add esp, 0x40
        pop ebp

        ret 0x10
    }
}

// int
// WINAPI
// MyMessageBoxA(
//               HWND hWnd,
//               LPCSTR lpText,
//               LPCSTR lpCaption,
//               UINT uType)
// {
//     // �ָ�HOOK
//     MsgHook.UnHook();
//     MessageBox(hWnd, "Hook����", lpCaption, uType);
//     MessageBox(hWnd, lpText, lpCaption, uType);
//     // ����HOOK
//     MsgHook.ReHook();
//     
//     return 0;
// }

int main()
{
    // ������HOOK��MessageBox
    MessageBox(NULL, "��������1", "test", MB_OK);

    // HOOK���MessageBox
    MsgHook.Hook("User32.dll", "MessageBoxA", (PROC)MyMessageBoxA);
    MessageBox(NULL, "��HOOK��1", "test", MB_OK);
    MessageBox(NULL, "��HOOK��2", "test", MB_OK);
    MsgHook.UnHook();

    MessageBox(NULL, "��������2", "test", MB_OK);

    return 0;
}




















