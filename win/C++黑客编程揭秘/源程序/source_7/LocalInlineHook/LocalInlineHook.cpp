#include <Windows.h>
#include "ILHook.h"

// 创建一个全局的变量
CILHook MsgHook;


// 自定义的MessageBoxA函数
int
/*WINAPI*/
MyMessageBoxA(
        HWND hWnd,
        LPCSTR lpText,
        LPCSTR lpCaption,
        UINT uType)
{
    // 恢复HOOK
    MsgHook.UnHook();
    MessageBox(hWnd, "Hook流程", lpCaption, uType);
    MessageBox(hWnd, lpText, lpCaption, uType);
    // 重新HOOK
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
//     // 恢复HOOK
//     MsgHook.UnHook();
//     MessageBox(hWnd, "Hook流程", lpCaption, uType);
//     MessageBox(hWnd, lpText, lpCaption, uType);
//     // 重新HOOK
//     MsgHook.ReHook();
//     
//     return 0;
// }

int main()
{
    // 不进行HOOK的MessageBox
    MessageBox(NULL, "正常流程1", "test", MB_OK);

    // HOOK后的MessageBox
    MsgHook.Hook("User32.dll", "MessageBoxA", (PROC)MyMessageBoxA);
    MessageBox(NULL, "被HOOK了1", "test", MB_OK);
    MessageBox(NULL, "被HOOK了2", "test", MB_OK);
    MsgHook.UnHook();

    MessageBox(NULL, "正常流程2", "test", MB_OK);

    return 0;
}




















