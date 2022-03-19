#pragma once
#include "Define.h"

enum EVENT_TYPE
{
	EVENT_MOUSE_DOWN,
	EVENT_CLIP_CHANGE,
	EVENT_WINDOW_CHANGE,
	EVENT_KEY_DOWN,
};
class KeyLog
{
public:
	static void Log(const string& str,bool bEvent = false);
	static void LogEvent(EVENT_TYPE e, const string& parameter);
protected:
private:
	static string GetEventStrBegin(EVENT_TYPE e);
	static string GetEventStrEnd(EVENT_TYPE e);
private:
	static ofstream mLogFileStream;
};