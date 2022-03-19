#pragma once
#include "ListenerBase.h"
#include<string>


class ClipboardManager :public ListenerBase
{
public:
	virtual LISTENER_TYPE GetType()
	{
		return LISTENER_TYPE::LISTENER_CLIP;
	}
	virtual void Start();
	virtual void Update();
	virtual void HandleMsg(const MSG& rMsg);
	virtual void OnDestroy();
protected:
private:
	void OnClipboardChange();
private:
	std::string mClipOldStr;
};