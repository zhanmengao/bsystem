#ifndef __ILHOOK_H__F47BF581_8D85_49ef_923D_895DCC9E4471_
#define __ILHOOK_H__F47BF581_8D85_49ef_923D_895DCC9E4471_

#include <Windows.h>

class CILHook
{
public:
    CILHook();      // ����
    ~CILHook();     // ����

    // HOOK����
    BOOL Hook(LPSTR pszModuleName, LPSTR pszFuncName, PROC pfnHookFunc);
    // ȡ��HOOK����
    VOID UnHook();
    // ���½���HOOK����
    BOOL ReHook();

private:
    PROC m_pfnOrig; // ������ַ
    BYTE m_bOldBytes[5];    // ������ڴ���
    BYTE m_bNewBytes[5];    // Inline����
};

#endif