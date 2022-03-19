#pragma once
#include "ListenerBase.h"


class KeyboardManager :public ListenerBase
{
public:
	virtual LISTENER_TYPE GetType()
	{
		return LISTENER_TYPE::LISTENER_KEY;
	}
	virtual void Start();
	virtual void Update();
	virtual void HandleMsg(const MSG& rMsg);
	void HandleMsg(USHORT vCode);
	virtual void OnDestroy();
protected:
private:
	//Hook
	void InitHook();
	void DestroyHook();
	LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);
private:
	void InitRaw();
private:
	HHOOK mHook;
};