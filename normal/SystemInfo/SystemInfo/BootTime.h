#pragma once
#include "Windows.h"
#include "tchar.h"
#include "atltime.h"
#include "time.h"
#include<string>

#define SystemTimeInformation 3

#define RECORDCOUNT    100

using namespace std;



//读取 Windows 事件日志
void ReadWindowsEventLog()
{

	HANDLE h;
	EVENTLOGRECORD* pevlr;
	TCHAR bBuffer[4096] = { 0 };

	DWORD dwRead, dwNeeded, cRecords, dwThisRecord = 0;

	/*
	Windows 日志：
	应用程序 "Application"  安全 "Security"  设置 "setup"  系统 "System"
	*/
	h = OpenEventLog(NULL, L"System");
	if (h == NULL)
	{
		printf("Could not open the System event log.");

		return;
	}

	pevlr = (EVENTLOGRECORD*)&bBuffer;

	int count = 0;


	printf("\n最近 %d 次开机记录：\n", RECORDCOUNT);

	while (ReadEventLog(h,
		EVENTLOG_SEQUENTIAL_READ |
		EVENTLOG_BACKWARDS_READ,
		0,
		pevlr,
		4096,
		&dwRead,
		&dwNeeded))
	{
		bool find = false;

		while (dwRead > 0)
		{
			int EventID = (short)pevlr->EventID;

			if (EventID == 6005)
			{
				CTime time(pevlr->TimeWritten);
				time_t tmt = time.GetTime();
				tm logtime;
				localtime_s(&logtime, &tmt);

				char datatime[25] = { 0 };
				sprintf(datatime, "%d-%02d-%02d %02d:%02d:%02d", logtime.tm_year + 1900, logtime.tm_mon + 1, logtime.tm_mday, logtime.tm_hour, logtime.tm_min, logtime.tm_sec);

				printf("%02d %s\n", dwThisRecord++, datatime);
				//printf("%02d  Event ID: %d %s ", dwThisRecord++, (short)pevlr->EventID, datatime);
				//printf("EventType: %d Source: %s\n", pevlr->EventType, (LPCTSTR)((LPBYTE)pevlr + sizeof(EVENTLOGRECORD)));

				find = true;
				count++;

				break;
			}

			dwRead -= pevlr->Length;
			pevlr = (EVENTLOGRECORD*)((LPBYTE)pevlr + pevlr->Length);
		}

		pevlr = (EVENTLOGRECORD*)&bBuffer;

		if (count >= RECORDCOUNT)/*(find)*/
		{
			break;
		}
	}

	CloseEventLog(h);
}