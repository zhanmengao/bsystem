#include "ILHook.h"

CILHook CreateProcessHook;

// ����ʵ�ֵ�Hook����
BOOL
WINAPI
MyCreateProcessW(
    LPCWSTR lpApplicationName,
    LPWSTR lpCommandLine,
    LPSECURITY_ATTRIBUTES lpProcessAttributes,
    LPSECURITY_ATTRIBUTES lpThreadAttributes,
    BOOL bInheritHandles,
    DWORD dwCreationFlags,
    LPVOID lpEnvironment,
    LPCWSTR lpCurrentDirectory,
    LPSTARTUPINFOW lpStartupInfo,
    LPPROCESS_INFORMATION lpProcessInformation
    )
{
    BOOL bRet = FALSE;

    if ( MessageBoxW(NULL, lpApplicationName, lpCommandLine, MB_YESNO) == IDYES )
    {
        CreateProcessHook.UnHook();
        bRet = CreateProcessW(lpApplicationName,
                        lpCommandLine,
                        lpProcessAttributes,
                        lpThreadAttributes,
                        bInheritHandles,
                        dwCreationFlags,
                        lpEnvironment,
                        lpCurrentDirectory,
                        lpStartupInfo,
                        lpProcessInformation);
         CreateProcessHook.ReHook();
    } 
    else
    {
        MessageBox(NULL, "�������ĳ�������", "��ʾ", MB_OK);
    }
    
//     CreateProcessHook.UnHook();
//     // �������������̵Ľ�����
//     MessageBoxW(NULL, lpApplicationName, lpCommandLine, MB_OK);
// 
//     // ��������
//     bRet = CreateProcessW(lpApplicationName,
//                     lpCommandLine,
//                     lpProcessAttributes,
//                     lpThreadAttributes,
//                     bInheritHandles,
//                     dwCreationFlags,
//                     lpEnvironment,
//                     lpCurrentDirectory,
//                     lpStartupInfo,
//                     lpProcessInformation);
// 
//     CreateProcessHook.ReHook();

    return bRet;
}

BOOL APIENTRY DllMain( HANDLE hModule, 
                      DWORD  ul_reason_for_call, 
                      LPVOID lpReserved
                      )
{
    switch ( ul_reason_for_call )
    {
    case DLL_PROCESS_ATTACH:
        {
            // Hook CreateProcessW()����
            CreateProcessHook.Hook("kernel32.dll",
                "CreateProcessW",
                (PROC)MyCreateProcessW);
            break;
        }
    case DLL_PROCESS_DETACH:
        {
            CreateProcessHook.UnHook();
            break;
        }
    }
    
    return TRUE;
}

