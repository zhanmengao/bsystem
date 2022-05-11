#include"WinTime.h"
#include "atltime.h"
typedef long(__stdcall *fnNtQuerySystemInformation)(
	IN  UINT SystemInformationClass,
	OUT PVOID SystemInformation,
	IN  ULONG SystemInformationLength,
	OUT PULONG ReturnLength OPTIONAL);

fnNtQuerySystemInformation NtQuerySystemInformation = NULL;

typedef struct {
	LARGE_INTEGER liKeBootTime;
	LARGE_INTEGER liKeSystemTime;
	LARGE_INTEGER liExpTimeZoneBias;
	ULONG uCurrentTimeZoneId;
	DWORD dwReserved;
} SYSTEM_TIME_INFORMATION;

SYSTEMTIME WinTimeUtils::GetSysTime()
{
	_SYSTEMTIME t;
	GetLocalTime(&t);
	return t;
}
WORD WinTimeUtils::GetMilliseconds()
{
	_SYSTEMTIME t;
	GetLocalTime(&t);
	return t.wMilliseconds;
}

std::string  WinTimeUtils::GetNowTime()
{
	const string& format = "%04d-%02d-%02d %02d:%02d:%02d:%04d";
	time_t nowtime = time(NULL);
	struct tm *l = localtime(&nowtime);
	WORD millsec = GetMilliseconds();
	char buf[128] = { 0 };
	snprintf(buf, sizeof(buf), format.c_str(),
		l->tm_year + 1900, l->tm_mon + 1, l->tm_mday, l->tm_hour, l->tm_min, l->tm_sec, millsec);
	return buf;
}

std::string  WinTimeUtils::GetNowYMD()
{
	const string& format = "%04d%02d%02d";
	time_t nowtime = time(NULL);
	struct tm *l = localtime(&nowtime);
	char buf[128] = { 0 };
	snprintf(buf, sizeof(buf), format.c_str(), l->tm_year + 1900, l->tm_mon + 1, l->tm_mday);
	return buf;
}
std::string  WinTimeUtils::GetNowHMS()
{
	const string& format = "%02d%02d%02d";
	time_t nowtime = time(NULL);
	struct tm *l = localtime(&nowtime);
	char buf[128] = { 0 };
	snprintf(buf, sizeof(buf), format.c_str(), l->tm_hour, l->tm_min,l->tm_sec);
	return buf;
}
std::string  WinTimeUtils::GetNowHMSM()
{
	const string& format = "%02d%02d%02d%04d";
	time_t nowtime = time(NULL);
	struct tm *l = localtime(&nowtime);
	WORD millsec = GetMilliseconds();
	char buf[128] = { 0 };
	snprintf(buf, sizeof(buf), format.c_str(), l->tm_hour, l->tm_min, l->tm_sec, millsec);
	return buf;
}
//SYSTEMTIMEתtime_t
time_t WinTimeUtils::SystemTimeToTimet(SYSTEMTIME st)
{
	FILETIME ft;
	SystemTimeToFileTime(&st, &ft);
	LONGLONG nLL;
	ULARGE_INTEGER ui;
	ui.LowPart = ft.dwLowDateTime;
	ui.HighPart = ft.dwHighDateTime;
	nLL = (ft.dwHighDateTime << 32) + ft.dwLowDateTime;
	time_t pt = (long)((LONGLONG)(ui.QuadPart - 116444736000000000) / 10000000);
	return pt;
}
//time_tתSYSTEMTIME
SYSTEMTIME WinTimeUtils::TimetToSystemTime(time_t t)
{
	FILETIME ft;
	SYSTEMTIME pst;
	LONGLONG nLL = Int32x32To64(t, 10000000) + 116444736000000000;
	ft.dwLowDateTime = (DWORD)nLL;
	ft.dwHighDateTime = (DWORD)(nLL >> 32);
	FileTimeToSystemTime(&ft, &pst);
	return pst;
}
//time_tתSYSTEMTIME
SYSTEMTIME WinTimeUtils::Time_tToSystemTime(time_t t)
{
	tm temptm = *localtime(&t);
	SYSTEMTIME st = { 1900 + temptm.tm_year,1 + temptm.tm_mon,
		temptm.tm_wday,temptm.tm_mday,temptm.tm_hour,
		temptm.tm_min,temptm.tm_sec,0 };
	return st;
}
//SYSTEMTIMEתtime_t
time_t WinTimeUtils::SystemTimeToTime_t(const SYSTEMTIME& st)
{
	tm temptm = { st.wSecond,st.wMinute,st.wHour,
		st.wDay,st.wMonth - 1,st.wYear - 1900,st.wDayOfWeek,0,0 };
	return mktime(&temptm);
}
#define SystemTimeInformation 3
SYSTEMTIME WinTimeUtils::GetBootTime()
{
	SYSTEMTIME ret = { 0 };
	NtQuerySystemInformation = (fnNtQuerySystemInformation)GetProcAddress(LoadLibrary(_T("ntdll.dll")),
		"NtQuerySystemInformation");
	if (NtQuerySystemInformation == NULL)
	{
		return ret;
	}

	LONG status;
	SYSTEM_TIME_INFORMATION sti;
	status = NtQuerySystemInformation(SystemTimeInformation, &sti, sizeof(sti), 0);
	if (NO_ERROR != status)
	{
		return ret;
	}

	FILETIME ft;
	SYSTEMTIME st;
	memcpy(&ft, &sti.liKeBootTime, sizeof(ft));
	FileTimeToLocalFileTime(&ft, &ft);
	FileTimeToSystemTime(&ft, &st);
	return st;
}

DWORD WinTimeUtils::GetBootRunTimeM()
{
	DWORD s = GetTickCount() / 1000;

	DWORD second = s % 60;
	DWORD minute = s / 60 % 60;
	DWORD hour = s / 60 / 60;

	DWORD minutes = hour * 60 + minute;
	return minutes;
}
DWORD WinTimeUtils::GetBootRunTimeS()
{
	DWORD s = GetTickCount() / 1000;

	return s;
}
void WinTimeUtils::PrintTime(const SYSTEMTIME& st)
{
	char timeBuf[MAX_PATH] = { 0 };
	sprintf(timeBuf, "%04d-%02d-%02d %02d:%02d:%02d\n", st.wYear, st.wMonth, st.wDay,
		st.wHour, st.wMinute, st.wSecond);
	printf("Time: %s", timeBuf);
}
UINT_PTR WinTimeUtils::Setitimer(UINT timer, UINT_PTR eid, TIMERPROC cb)
{
	return SetTimer(NULL, eid, timer, cb);
}