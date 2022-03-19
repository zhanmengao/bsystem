#include "ListenerManager.h"
#include "LogManager.h"
#include<set>
using namespace std;
std::map<unsigned int, LISTENER_TYPE> ListenerManager::mMessageMap;      //事件 类型映射
std::map<LISTENER_TYPE, ListenerBase*> ListenerManager::mListenerMap;
MSG ListenerManager::mLastMsg = { 0 };
set<DWORD> keys;

//避免按住不放不停的响应keydown
bool IsInKeys(DWORD key)
{
	set<DWORD>::iterator itor =
		find(keys.begin(), keys.end(), key);

	bool result = false;
	if (itor == keys.end())
	{
		keys.insert(key);
		result = false;
	}
	else
	{
		result = true;
	}
	return result;
}
//按下的时候，将key从set中插入
void Insertkey(DWORD key)
{
	keys.insert(key);
}
//松开的时候，将key从set中移除
void erasekey(DWORD key)
{
	keys.erase(key);
}
void ListenerManager::RegisterListener(unsigned int msgType, ListenerBase* pListener)
{
	if (pListener == nullptr)
	{
		return;
	}
	//添加事件映射
	mMessageMap.insert(std::make_pair(msgType, pListener->GetType()));
	//添加监听者映射
	if (mListenerMap.find(pListener->GetType()) == mListenerMap.end())
	{
		mListenerMap.insert(std::make_pair(pListener->GetType(), pListener));
	}
}
void ListenerManager::ListenerInit()
{
	KeyLog::Log("ListenerInit");
	for (auto it = mListenerMap.begin(); it != mListenerMap.end(); it++)
	{
		it->second->Start();
	}
}
void ListenerManager::ListenerUpdate()
{
	KeyLog::Log("ListenerUpdate");
	MSG msg = { 0 };
	while (true)
	{
		PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);
		MSG rMsgCpy = msg;
		if (msg.message == WM_INPUT)
		{
			//解析消息
			UINT dwSize = 0;
			GetRawInputData((HRAWINPUT)msg.lParam, RID_INPUT, NULL,
				&dwSize, sizeof(RAWINPUTHEADER)); //先获取数据大小dwSize
			LPBYTE lpb = new BYTE[dwSize]; //根据大小 创建

			if (GetRawInputData((HRAWINPUT)msg.lParam, RID_INPUT, lpb,
				&dwSize, sizeof(RAWINPUTHEADER)) != dwSize) //获取消息信息
				break;
			RAWINPUT *raw = (RAWINPUT*)lpb;
			if (raw->header.dwType == RIM_TYPEKEYBOARD)
			{
				if (raw->data.keyboard.Message == WM_KEYDOWN)
				{
					if (!IsInKeys(raw->data.keyboard.VKey))
					{
						//把事件做转换
						rMsgCpy.wParam = raw->data.keyboard.VKey;  //按键 键值;
						rMsgCpy.message = WM_KEYDOWN;
						Insertkey(raw->data.keyboard.VKey);
					}
				}
				else if (raw->data.keyboard.Message == WM_KEYUP)
				{
					erasekey(raw->data.keyboard.VKey);
				}
			}
			else if (raw->header.dwType == RIM_TYPEMOUSE)
			{
				if (raw->data.mouse.usButtonFlags == RI_MOUSE_LEFT_BUTTON_DOWN)
				{
					rMsgCpy.message = WM_LBUTTONDOWN;
				}
				else if (raw->data.mouse.usButtonFlags == RI_MOUSE_RIGHT_BUTTON_DOWN)
				{
					rMsgCpy.message = WM_RBUTTONDOWN;
				}
			}
			delete[] lpb;
			DefWindowProc(msg.hwnd, msg.message, msg.wParam, msg.lParam);
		}
		auto pListener = GetListener(rMsgCpy.message);
		if (pListener != nullptr)
		{
			pListener->HandleMsg(rMsgCpy);
			mLastMsg = rMsgCpy;
		}
		for (auto it = mListenerMap.begin(); it != mListenerMap.end(); it++)
		{
			it->second->Update();
		}
		Sleep(5);
	}
}
void ListenerManager::ListenerDestroy()
{
	for (auto it = mListenerMap.begin(); it != mListenerMap.end();)
	{
		it->second->OnDestroy();
		delete it->second;
		it = mListenerMap.erase(it);
	}
}
ListenerBase* ListenerManager::GetListener(unsigned int msgType)
{
	auto msgIt = mMessageMap.find(msgType);
	//该事件有人监听
	if (msgIt != mMessageMap.end())
	{
		auto it = mListenerMap.find(msgIt->second);
		if (it != mListenerMap.end())
		{
			return it->second;
		}
	}
	return nullptr;
}