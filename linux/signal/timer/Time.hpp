#pragma once
#include<time.h>				//时间标准库
#include<stdlib.h>
static time_t GetAnsiTime()
{
	return time(NULL);
}
static struct tm GetTime(time_t t)
{
    return *localtime(&t);
}
static struct tm Time()
{
	return GetTime(GetAnsiTime());
}
static const char* GetCTime(time_t t)
{
    return ctime(&t);
}
//"%Y-%m-%d %H:%M:%S"
static void GetStandardTime(struct tm t,char* buffer,size_t bufSize,const char* format)
{
	strftime(buffer, bufSize, format, &t);
}
static int CompareMonth(struct tm t1, struct tm t2)
{
	return t1.tm_year == t2.tm_year && t1.tm_mon == t2.tm_mon;
}
static int CompareDay(struct tm t1, struct tm t2)
{
	return t1.tm_year == t2.tm_year && t1.tm_mon == t2.tm_mon && t1.tm_mday == t2.tm_mday;
}
static int CompareSec(struct tm t1, struct tm t2, int maxSec)
{
	if (CompareDay(t1, t2))
	{
		if (t1.tm_hour == t2.tm_hour)
		{
			if (t1.tm_min == t2.tm_min)
			{
				if (abs(t1.tm_sec - t2.tm_sec) <= maxSec)
				{
					return 1;
				}
			}
		}
	}
	return 0;
}
#ifdef __cplusplus
#include <string>
#include <iostream>
using namespace std;
class TimeUtils
{
public:
	static string GetTime(const string& format)
	{
		time_t timep;
		time(&timep);
		char tmp[64] = { 0 };
		strftime(tmp, sizeof(tmp), format.c_str(), localtime(&timep));
		return tmp;
	}
	static string GetTimeNumYm(const time_t& t)
	{
		tm  mt = ::GetTime(t);
		char buf[1024] = { 0 };
		strftime(buf, sizeof(buf), "%Y_%m", &mt);
		return buf;
	}
	static string GetTimeNumYmD(const time_t& t)
	{
		tm  mt= ::GetTime(t);
		char buf[1024] = { 0 };
		strftime(buf, sizeof(buf), "%Y_%m_%d", &mt);
		return buf;
	}
	static string GetStandardTime(const time_t& t)
	{
		return GetStandardTime(::GetTime(t));
	}
	static string GetStandardTime(const tm& t)
	{
		char str[100] = { 0 };
		strftime(str, sizeof(str), "%Y-%m-%d %H:%M:%S", &t);
		return str;
	}
	static string FormatTime(time_t time64)
	{
		struct tm *info;
		char buffer[20] = { 0 };

		info = localtime(&time64);
		string timeStr(asctime(info));

		//localtime_s(&info, &time64);
		//sprintf(buffer, "%d年%d月%d日 %d时%d分%d秒", info.tm_year + 1900, info.tm_mon + 1, info.tm_mday, info.tm_hour, info.tm_min, info.tm_sec);
		size_t  index = timeStr.find_last_of("\n");
		if (index != timeStr.npos)
		{
			timeStr = timeStr.substr(0, index);
		}
		return timeStr;
	}
};
#endif