#include "LogManager.h"
#include "Config.h"
#include "Tools.h"

#define local_log "D://log.txt"


void KeyLog::Log(const string& str, bool bEvent)
{
	Tools::SaveDataToFile(local_log, str+"\n");
	cout << str<<endl;
	Tools::SaveDataToFile(ConfigManager::GetLogPath(), str + "\n");
	if (bEvent)
	{

	}
}
void KeyLog::LogEvent(EVENT_TYPE e, const string& parameter)
{
	Log(GetEventStrBegin(e) + parameter + GetEventStrEnd(e), true);
}
string KeyLog::GetEventStrBegin(EVENT_TYPE e)
{
	string outStr = Tools::GetFormatTime();
	switch (e)
	{
	case EVENT_TYPE::EVENT_MOUSE_DOWN:
		return "\n[MD:";
	case EVENT_TYPE::EVENT_KEY_DOWN:
		return "";
	case EVENT_TYPE::EVENT_CLIP_CHANGE:
		return "\n[Clip:";
	case EVENT_TYPE::EVENT_WINDOW_CHANGE:
		return string("\n\n") + outStr +"Window Change:";
	}
	return "";
}
string KeyLog::GetEventStrEnd(EVENT_TYPE e)
{
	switch (e)
	{
	case EVENT_TYPE::EVENT_MOUSE_DOWN:
		return "]";
	case EVENT_TYPE::EVENT_KEY_DOWN:
		return "";
	case EVENT_TYPE::EVENT_CLIP_CHANGE:
		return "]";
	case EVENT_TYPE::EVENT_WINDOW_CHANGE:
		return "";
	}
	return "";
}