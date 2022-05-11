#include"Time.hpp"
#if _WIN32
#pragma once
#include <windows.h>
#include <time.h>


typedef 
class WinTimeUtils
{
public:
	static SYSTEMTIME GetSysTime();
	static WORD GetMilliseconds();

	static std::string  GetNowTime();
	static std::string  GetNowYMD();
	static std::string GetNowHMS();
	static std::string  GetNowHMSM();

	//SYSTEMTIME转time_t
	static time_t SystemTimeToTimet(SYSTEMTIME st);
	//time_t转SYSTEMTIME
	static SYSTEMTIME TimetToSystemTime(time_t t);
	//time_t转SYSTEMTIME
	static SYSTEMTIME Time_tToSystemTime(time_t t);
	//SYSTEMTIME转time_t
	static time_t SystemTimeToTime_t(const SYSTEMTIME& st);
public:
	static SYSTEMTIME GetBootTime();

	static DWORD GetBootRunTimeM();									//开机后运行时长：分钟

	static DWORD GetBootRunTimeS();									//开机后运行时长：秒

	static void PrintTime(const SYSTEMTIME& st);

	static UINT_PTR Setitimer(UINT timer, UINT_PTR eid, TIMERPROC cb);
};
#endif
