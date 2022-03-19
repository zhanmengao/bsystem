#include "stdafx.h"
#include "ILHook.h"

CILHook::CILHook()
{
    // �Գ�Ա�����ĳ�ʼ��
    m_pfnOrig = NULL;
    ZeroMemory(m_bOldBytes, 5);
    ZeroMemory(m_bNewBytes, 5);
}

CILHook::~CILHook()
{
    // ȡ��HOOK
    UnHook();
}

/*
�������ƣ�Hook
�������ܣ���ָ��ģ���еĺ������йҹ�
����˵����
    pszModuleName:ģ������
    pszFuncName:  ��������
    pfnHookFunc:  ���Ӻ���
*/
BOOL CILHook::Hook(LPSTR pszModuleName, LPSTR pszFuncName, PROC pfnHookFunc)
{
    BOOL bRet = FALSE;
    
    // ��ȡָ��ģ���к����ĵ�ַ
    m_pfnOrig = (PROC)GetProcAddress(GetModuleHandle(pszModuleName), pszFuncName);

    if ( m_pfnOrig != NULL )
    {

        // ����õ�ַ��5���ֽڵ�����
        DWORD dwNum = 0;
        ReadProcessMemory(GetCurrentProcess(), m_pfnOrig, m_bOldBytes, 5, &dwNum);

        // ����JMPָ��
        m_bNewBytes[0] = '\xe9';    // jmp Opcode
        // pfnHookFunc������HOOK���Ŀ���ַ
        // m_pfnOrig��ԭ���ĵ�ַ
        // 5��ָ���
        *(DWORD *)(m_bNewBytes + 1) = (DWORD)pfnHookFunc - (DWORD)m_pfnOrig - 5;

        // ������õĵ�ַд��õ�ַ��
        WriteProcessMemory(GetCurrentProcess(), m_pfnOrig, m_bNewBytes, 5, &dwNum);

        bRet = TRUE;
    }
    
    return bRet;
}

/*
�������ƣ�UnHook
�������ܣ�ȡ�������Ĺҹ�
*/
VOID CILHook::UnHook()
{
    if ( m_pfnOrig != 0 )
    {
        DWORD dwNum = 0;
        WriteProcessMemory(GetCurrentProcess(), m_pfnOrig, m_bOldBytes, 5, &dwNum);
    }
}

/*
�������ƣ�ReHook
�������ܣ����¶Ժ������йҹ�
*/
BOOL CILHook::ReHook()
{
    BOOL bRet = FALSE;

    if ( m_pfnOrig != 0 )
    {
        DWORD dwNum = 0;
        WriteProcessMemory(GetCurrentProcess(), m_pfnOrig, m_bNewBytes, 5, &dwNum);

        bRet = TRUE;
    }

    return bRet;
}