// ShowPassword.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include <stdio.h>

// ��Ҫ����INT3�ϵ��λ��
#define BP_VA   0x00401E9E
// INT3�Ļ�����
const BYTE bInt3 = '\xCC';

int main(int argc, char* argv[])
{
    if ( argc != 2 )
    {
        printf("usage: %s crackme.exe \r\n", argv[0]);
        return -1;
    }

    // �����ļ���
    char *pszFileName = argv[1];

    // ������Ϣ
    STARTUPINFO si = { 0 };
    si.cb = sizeof(STARTUPINFO);
    GetStartupInfo(&si);

    // ������Ϣ
    PROCESS_INFORMATION pi = { 0 };

    // ���������Խ���
    BOOL bRet = CreateProcess(pszFileName, 
                        NULL,
                        NULL,
                        NULL,
                        FALSE,
                        DEBUG_PROCESS | DEBUG_ONLY_THIS_PROCESS,
                        NULL,
                        NULL,
                        &si,
                        &pi);

    if ( bRet == FALSE )
    {
        printf("CreateProcess Error \r\n");
        return -1;
    }

    DEBUG_EVENT de = { 0 };
    CONTEXT context = { 0 };
    // ����ԭʼ�Ļ�����
    BYTE bOldByte = 0;
    DWORD dwReadWriteNum = 0;

    // ������ȷ����ʹ��
    char pszPassword[MAXBYTE] = { 0 };
    // �����������ʹ��
    char pszErrorPass[MAXBYTE] = { 0 };

    while ( TRUE )
    {
        // ��ȡ�����¼�
        WaitForDebugEvent(&de, INFINITE);

        // �ж��¼�����
        switch ( de.dwDebugEventCode )
        {
            // ��������ʱ�ĵ����¼�
        case CREATE_PROCESS_DEBUG_EVENT:
            {
                // ��ȡ������INT3�ϵ㴦�Ļ�����
                // �������ָ�
                ReadProcessMemory(pi.hProcess,
                                (LPVOID)BP_VA,
                                (LPVOID)&bOldByte,
                                sizeof(BYTE),
                                &dwReadWriteNum);

                // ��INT3�Ļ�����0xCCд��ϵ㴦
                WriteProcessMemory(pi.hProcess,
                                (LPVOID)BP_VA,
                                (LPVOID)&bInt3,
                                sizeof(BYTE),
                                &dwReadWriteNum);
        	    break;
            }
            // �����쳣ʱ�ĵ����¼�
        case EXCEPTION_DEBUG_EVENT:
            {
                // �ж��쳣����
                switch ( de.u.Exception.ExceptionRecord.ExceptionCode )
                {
                    // INT3���͵��쳣
                case EXCEPTION_BREAKPOINT:
                    {
                        // ��ȡ�̻߳���
                        context.ContextFlags = CONTEXT_FULL;
                        GetThreadContext(pi.hThread, &context);

                        // �ж��Ƿ�����������õĶϵ�λ�ô�
                        if ( (BP_VA + 1) == context.Eip )
                        {
                            // ��ȡ��ȷ������
                            ReadProcessMemory(pi.hProcess,
                                        (LPVOID)context.Edx,
                                        (LPVOID)pszPassword,
                                        MAXBYTE,
                                        &dwReadWriteNum);
                            // ��ȡ��������
                            ReadProcessMemory(pi.hProcess,
                                        (LPVOID)context.Ecx,
                                        (LPVOID)pszErrorPass,
                                        MAXBYTE,
                                        &dwReadWriteNum);
                            
                            printf("�������������: %s \r\n", pszErrorPass);
                            printf("��ȷ��������: %s \r\n", pszPassword);

                            // ��Ϊ���ǵ�ָ��ִ����INT3��˱��ж�
                            // INT3�Ļ���ָ���Ϊһ���ֽ�
                            // ���������Ҫ��EIP��һ������EIP
                            // EIP��ָ��ָ��Ĵ���
                            // ���б���������Ҫִ��ָ��ĵ�ַ
                            context.Eip --;

                            // ����ԭ���õ�ַ�Ļ�����
                            WriteProcessMemory(pi.hProcess, 
                                        (LPVOID)BP_VA,
                                        (LPVOID)&bOldByte,
                                        sizeof(BYTE),
                                        &dwReadWriteNum);
                            // ���õ�ǰ���̻߳���
                            SetThreadContext(pi.hThread, &context);
                        }
                        break;
                    }
                }
            }
        }
        
		ContinueDebugEvent(de.dwProcessId,de.dwThreadId,DBG_CONTINUE);
    }

    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);

	return 0;
}