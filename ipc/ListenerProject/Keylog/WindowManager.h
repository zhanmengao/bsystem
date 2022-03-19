#pragma once
#include "ListenerBase.h"
#include<string>


class WindowManager :public ListenerBase
{
public:
	virtual LISTENER_TYPE GetType()
	{
		return LISTENER_TYPE::LISTENER_WINDOW;
	}
	virtual void Start();
	virtual void Update();
	virtual void HandleMsg(const MSG& rMsg);
	virtual void OnDestroy();
protected:
private:
	DWORD mLastPid;
	std::wstring mLastTitle;
};