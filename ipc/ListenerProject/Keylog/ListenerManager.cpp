#include "ListenerManager.h"
#include "LogManager.h"
#include<set>
using namespace std;
std::map<unsigned int, LISTENER_TYPE> ListenerManager::mMessageMap;      //�¼� ����ӳ��
std::map<LISTENER_TYPE, ListenerBase*> ListenerManager::mListenerMap;
MSG ListenerManager::mLastMsg = { 0 };
set<DWORD> keys;

//���ⰴס���Ų�ͣ����Ӧkeydown
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
//���µ�ʱ�򣬽�key��set�в���
void Insertkey(DWORD key)
{
	keys.insert(key);
}
//�ɿ���ʱ�򣬽�key��set���Ƴ�
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
	//����¼�ӳ��
	mMessageMap.insert(std::make_pair(msgType, pListener->GetType()));
	//��Ӽ�����ӳ��
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
			//������Ϣ
			UINT dwSize = 0;
			GetRawInputData((HRAWINPUT)msg.lParam, RID_INPUT, NULL,
				&dwSize, sizeof(RAWINPUTHEADER)); //�Ȼ�ȡ���ݴ�СdwSize
			LPBYTE lpb = new BYTE[dwSize]; //���ݴ�С ����

			if (GetRawInputData((HRAWINPUT)msg.lParam, RID_INPUT, lpb,
				&dwSize, sizeof(RAWINPUTHEADER)) != dwSize) //��ȡ��Ϣ��Ϣ
				break;
			RAWINPUT *raw = (RAWINPUT*)lpb;
			if (raw->header.dwType == RIM_TYPEKEYBOARD)
			{
				if (raw->data.keyboard.Message == WM_KEYDOWN)
				{
					if (!IsInKeys(raw->data.keyboard.VKey))
					{
						//���¼���ת��
						rMsgCpy.wParam = raw->data.keyboard.VKey;  //���� ��ֵ;
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
	//���¼����˼���
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