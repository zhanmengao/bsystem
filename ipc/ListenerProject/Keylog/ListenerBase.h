#pragma once
#include<Windows.h>
enum LISTENER_TYPE
{
	LISTENER_KEY,
	LISTENER_MOUSE,
	LISTENER_CLIP,
	LISTENER_WINDOW,
};
class ListenerBase
{
public:
	virtual LISTENER_TYPE GetType() = 0;
	virtual void Start() = 0;
	virtual void Update() = 0;
	virtual void HandleMsg(const MSG& rMsg) = 0;
	virtual void OnDestroy() = 0;
protected:
private:
};