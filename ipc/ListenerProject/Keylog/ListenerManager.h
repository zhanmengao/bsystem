#pragma once
#include "ListenerBase.h"
#include <map>


class ListenerManager
{
public:
	static void RegisterListener(unsigned int msgType,ListenerBase* pListener);
	static void ListenerInit();
	static void ListenerUpdate();
	static void ListenerDestroy();
protected:
	static ListenerBase* GetListener(unsigned int msgType);
private:
	static std::map<unsigned int, LISTENER_TYPE> mMessageMap;      //事件-类型映射
	static std::map<LISTENER_TYPE, ListenerBase*> mListenerMap;   //类型-指针映射
	static MSG mLastMsg;
};