#pragma once
#include "ListenerBase.h"


class MouseManager:public ListenerBase
{
public:
	virtual LISTENER_TYPE GetType()
	{
		return LISTENER_TYPE::LISTENER_MOUSE;
	}
	virtual void Start();
	virtual void Update();
	virtual void HandleMsg(const MSG& rMsg);
	void HandleMsg(const POINT& pt);
	virtual void OnDestroy();
protected:
private:
	void InitRaw();
	POINT mLastPt;
};