#include"Time.hpp"
#include"WinTime.h"

VOID __stdcall TimerCB(HWND hWnd, UINT uMsg, UINT_PTR eID, DWORD dwTime)
{
	DWORD dwRunTime = WinTimeUtils::GetBootRunTimeM();
	printf("%s = %lu %lu\n", "TimerCB", WinTimeUtils::GetBootRunTimeM(), WinTimeUtils::GetBootRunTimeS());
}
int main()
{
	cout << "strftime" << TimeUtils::GetTime("%Y-%m-%d %H:%M:%S") << endl;

	cout << "getNowTime:" << WinTimeUtils::GetNowTime() << endl;
	cout << "getNowYMD:" << WinTimeUtils::GetNowYMD() << endl;
	cout << "getNowHMSM:" << WinTimeUtils::GetNowHMSM() << endl;
	cout << "GetMilliseconds:" << WinTimeUtils::GetMilliseconds() << endl;

	cout << "GetSysTime.Day:" << WinTimeUtils::GetSysTime().wDay << endl;
	cout << "GetTime" << GetAnsiTime() + 100000 << endl;
	WinTimeUtils::PrintTime(WinTimeUtils::GetBootTime());
	cout << "TimeUtils::FormatTime" << TimeUtils::FormatTime(1599192160) << endl;
	//闹钟
	char buf[1024] = { 0 };
	UINT_PTR ret = WinTimeUtils::Setitimer(1000, 1, TimerCB);
	DWORD err = GetLastError();
	MSG msg = { 0 };
	while (GetMessage(&msg, NULL, 0, 0))
	{
		//scanf("%s", buf);								//定时器中断级别比IO设备高
		if (msg.message == WM_TIMER)
		{
			DispatchMessage(&msg);
		}
	}
}