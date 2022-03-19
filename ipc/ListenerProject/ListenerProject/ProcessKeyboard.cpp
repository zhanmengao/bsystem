//==================================CompositeKeyShortCut.h==========================

#ifndef OMPOSITE_KEY_SHORT_CUT_H
#define OMPOSITE_KEY_SHORT_CUT_H
#include "windows.h"

typedef struct ComposeKey
{
	USHORT FirstVirtualKeys;
	USHORT SecondVirtualKeys;
	USHORT ThirdVirtualKeys;

	ComposeKey()
	{
		FirstVirtualKeys = 0;
		SecondVirtualKeys = 0;
		ThirdVirtualKeys = 0;
	}
} ComposeKey, PComposeKey;

typedef struct KeyStatus
{
	bool iskeyup;
	bool iskeydown;
} KeyStatus, PKeyStatus;


typedef struct KeyBoardEventComPoseArgs
{
	USHORT FirstVirtualKeys;
	USHORT SecondVirtualKeys;
	USHORT ThirdVirtualKeys;

} KeyBoardEventComPoseArgs, *PKeyBoardEventComPoseArgs;

extern "C" BOOL CompositeKeyShortCut_RegisitKeyBoard(HWND hWnd);

extern "C" BOOL CompositeKeyShortCut_DoCommand(UINT message, WPARAM wParam, LPARAM lParam);

extern "C" void(*KeyBoardComPoseEventHandle)(KeyBoardEventComPoseArgs keyBoardEventComPoseArgs);

extern "C" void(*KeyBoardThreeComPoseEventHandle)(KeyBoardEventComPoseArgs keyBoardEventComPoseArgs);

extern "C" void(*KeyBoardSingleEventHandle)(USHORT virtualKey);

extern "C" void CompositeKeyShortCut_AddComposeKey(ComposeKey composeKey);

extern "C" void CompositeKeyShortCut_ClearComposeKey();

extern "C" void CompositeKeyShortCut_AddSingleKey(USHORT singleKey);

extern "C" void CompositeKeyShortCut_ClearSingleKey();

#endif

/*
1.窗口初始化事件中调用CompositeKeyShortCut_RegisitKeyBoard

2.在wnproc事件中调用CompositeKeyShortCut_DoCommand

3.使用CompositeKeyShortCut_AddSingleKey，CompositeKeyShortCut_AddComposeKey注册快捷键

4.实现和注册KeyBoardSingleEventHandle，KeyBoardComPoseEventHandle,....这些快捷键触发的回调函数
*/

//====================================CompositeKeyShortCut.cpp=================================

#include <list>
#include <map>
//#include "CompositeKeyShortCut.h"
using namespace std;

static list<ComposeKey> ComposeKeyList;
static list<USHORT> singleKeyList;
static map<USHORT, KeyStatus> dictionaryKeyStatus;

void(*KeyBoardSingleEventHandle)(USHORT virtualKey) = NULL;

void(*KeyBoardComPoseEventHandle)(KeyBoardEventComPoseArgs keyBoardEventComPoseArgs) = NULL;

void(*KeyBoardThreeComPoseEventHandle)(KeyBoardEventComPoseArgs keyBoardEventComPoseArgs) = NULL;

BOOL CompositeKeyShortCut_RegisitKeyBoard(HWND hWnd)
{
	if (NULL == hWnd)
		return false;
	RAWINPUTDEVICE rid;
	rid.usUsagePage = 0x01;
	rid.usUsage = 0x06;
	rid.dwFlags = RIM_INPUT;//RIDEV_INPUTSINK;
	rid.hwndTarget = hWnd;
	return RegisterRawInputDevices(&rid, 1, sizeof(RAWINPUTDEVICE));
}
/******************************增加快捷键*********************************/
void CompositeKeyShortCut_AddComposeKey(ComposeKey composeKey)
{
	bool isFind = false;
	list<ComposeKey>::iterator it;
	for (it = ComposeKeyList.begin(); it != ComposeKeyList.end(); it++)
	{
		if (composeKey.FirstVirtualKeys == it->FirstVirtualKeys
			&& composeKey.SecondVirtualKeys == it->SecondVirtualKeys
			&& composeKey.ThirdVirtualKeys == it->ThirdVirtualKeys)
		{
			isFind = true;
			break;
		}
	}
	if (isFind == false)
	{
		ComposeKeyList.push_back(composeKey);
	}

}
/*********************************清除快捷键*********************************/
void CompositeKeyShortCut_ClearComposeKey()
{
	ComposeKeyList.clear();
}

void CompositeKeyShortCut_AddSingleKey(USHORT singleKey)
{
	bool isFindSigleKey = false;

	list<USHORT>::iterator itList;
	for (itList = singleKeyList.begin(); itList != singleKeyList.end(); itList++)
	{
		if (*itList == singleKey)
		{
			isFindSigleKey = true;
			break;
		}
	}
	if (isFindSigleKey == false)
	{
		singleKeyList.push_back(singleKey);
	}
}

void CompositeKeyShortCut_ClearSingleKey()
{
	singleKeyList.clear();
}


//处理快捷键向下按
static void ShortCutPressDown(RAWINPUT* raw)
{
	map<USHORT, KeyStatus>::iterator it;
	it = dictionaryKeyStatus.find(raw->data.keyboard.VKey);

	if (it != dictionaryKeyStatus.end())
	{
		dictionaryKeyStatus.erase(it);
	}
	//else
	// {
	KeyStatus keyStatus;
	keyStatus.iskeydown = true;
	keyStatus.iskeyup = false;
	dictionaryKeyStatus.insert(pair<USHORT, KeyStatus>(raw->data.keyboard.VKey, keyStatus));
	// }
}

//获取某按键状态
static int GetKeyStatus(USHORT key, KeyStatus *pKeyStatus)
{
	int result = 0;
	map<USHORT, KeyStatus>::iterator it;
	it = dictionaryKeyStatus.find(key);
	if (it != dictionaryKeyStatus.end())
	{
		result = 0;
		pKeyStatus->iskeyup = (it->second).iskeyup;
		pKeyStatus->iskeydown = (it->second).iskeydown;
	}
	else
	{
		result = -1;
		pKeyStatus->iskeyup = false;
		pKeyStatus->iskeydown = false;
	}

	return result;
}

static void DeleteKeyFromKeyStatusDic(USHORT key)
{
	map<USHORT, KeyStatus>::iterator it;
	it = dictionaryKeyStatus.find(key);
	if (it != dictionaryKeyStatus.end())
	{
		dictionaryKeyStatus.erase(it);
	}
}
//处理快捷键向上回的方法 
static void ShortCutPressUp(RAWINPUT* raw)
{
	//*******************************记录keyup的状态*********************
	DeleteKeyFromKeyStatusDic(raw->data.keyboard.VKey);

	KeyStatus keyStatus;
	keyStatus.iskeydown = false;
	keyStatus.iskeyup = true;
	dictionaryKeyStatus.insert(pair<USHORT, KeyStatus>(raw->data.keyboard.VKey, keyStatus));

	//*********************************************************


	bool isTriggerComposeKey = false;
	list<ComposeKey>::iterator itList;


	KeyStatus firstKeyStatus;
	KeyStatus secondKeyStatus;
	KeyStatus thirdKeyStatus;


	//触发3键
	for (itList = ComposeKeyList.begin(); itList != ComposeKeyList.end(); itList++)
	{

		//如果不是3键，则跳过
		if (itList->ThirdVirtualKeys == 0) continue;


		GetKeyStatus(itList->FirstVirtualKeys, &firstKeyStatus);
		GetKeyStatus(itList->SecondVirtualKeys, &secondKeyStatus);
		GetKeyStatus(itList->ThirdVirtualKeys, &thirdKeyStatus);

		if ((firstKeyStatus.iskeydown == true && firstKeyStatus.iskeyup == false)
			&& (secondKeyStatus.iskeydown == true && secondKeyStatus.iskeyup == false)
			&& (thirdKeyStatus.iskeydown == false && thirdKeyStatus.iskeyup == true))
		{

			isTriggerComposeKey = true;
		}

		if (isTriggerComposeKey == true)
		{
			KeyBoardEventComPoseArgs keyBoardEventComPoseArgs;
			keyBoardEventComPoseArgs.FirstVirtualKeys = itList->FirstVirtualKeys;
			keyBoardEventComPoseArgs.SecondVirtualKeys = itList->SecondVirtualKeys;
			keyBoardEventComPoseArgs.ThirdVirtualKeys = itList->ThirdVirtualKeys;
			KeyBoardThreeComPoseEventHandle(keyBoardEventComPoseArgs);
			//删除这些键的记录
			DeleteKeyFromKeyStatusDic(itList->FirstVirtualKeys);
			DeleteKeyFromKeyStatusDic(itList->SecondVirtualKeys);
			DeleteKeyFromKeyStatusDic(itList->ThirdVirtualKeys);
			break;
		}
	}




	//触发2键
	if (isTriggerComposeKey == false)
	{
		for (itList = ComposeKeyList.begin(); itList != ComposeKeyList.end(); itList++)
		{

			//如果不是2键，则跳过
			if (itList->ThirdVirtualKeys != 0) continue;

			GetKeyStatus(itList->FirstVirtualKeys, &firstKeyStatus);
			GetKeyStatus(itList->SecondVirtualKeys, &secondKeyStatus);


			if ((firstKeyStatus.iskeydown == true && firstKeyStatus.iskeyup == false))
			{
				if ((secondKeyStatus.iskeydown == false && secondKeyStatus.iskeyup == true))
				{
					if (KeyBoardComPoseEventHandle != NULL)
					{
						KeyBoardEventComPoseArgs keyBoardEventComPoseArgs;
						keyBoardEventComPoseArgs.FirstVirtualKeys = itList->FirstVirtualKeys;
						keyBoardEventComPoseArgs.SecondVirtualKeys = itList->SecondVirtualKeys;
						KeyBoardComPoseEventHandle(keyBoardEventComPoseArgs);
						isTriggerComposeKey = true;
						//删除这些键的记录
						DeleteKeyFromKeyStatusDic(itList->FirstVirtualKeys);
						DeleteKeyFromKeyStatusDic(itList->SecondVirtualKeys);
						break;
					}
				}
			}


		}
	}
	//组合键触发完成后再判断单键
	if (isTriggerComposeKey == false)
	{
		list<USHORT>::iterator itSingleList;
		for (itSingleList = singleKeyList.begin(); itSingleList != singleKeyList.end(); itSingleList++)
		{
			GetKeyStatus((*itSingleList), &firstKeyStatus);


			if ((firstKeyStatus.iskeydown == false && firstKeyStatus.iskeyup == true))
			{
				if (KeyBoardSingleEventHandle != NULL)
				{
					KeyBoardSingleEventHandle(raw->data.keyboard.VKey);
					//删除这些键的记录
					DeleteKeyFromKeyStatusDic(*itSingleList);
				}
			}
		}

	}
	map<USHORT, KeyStatus>::iterator it;
	it = dictionaryKeyStatus.find(raw->data.keyboard.VKey);
	if (it != dictionaryKeyStatus.end())
	{
		dictionaryKeyStatus.erase(it);
	}

}

BOOL CompositeKeyShortCut_DoCommand(UINT message, WPARAM wParam, LPARAM lParam)
{
	UINT dwSize;
	LPBYTE lpb = NULL;
	RAWINPUT* raw = NULL;
	if (message == WM_INPUT)
	{
		UINT uRet = GetRawInputData((HRAWINPUT)lParam, RID_INPUT, NULL, &dwSize, sizeof(RAWINPUTHEADER));
		if (uRet < 0)
		{
			// LOG_MSG("CompositeKeyShortCut_DoCommand::GetRawInputData fail");
			return false;
		}

		lpb = (LPBYTE)malloc(dwSize);
		if (lpb != NULL)
		{
			if (GetRawInputData((HRAWINPUT)lParam, RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER)) != dwSize)
			{

			}
			raw = (RAWINPUT*)lpb;

			if (WM_KEYUP == raw->data.keyboard.Message
				|| WM_SYSKEYUP == raw->data.keyboard.Message)
			{
				ShortCutPressUp(raw);
			}


			if (WM_KEYDOWN == raw->data.keyboard.Message
				|| WM_SYSKEYDOWN == raw->data.keyboard.Message)
			{
				ShortCutPressDown(raw);
			}

			free(lpb);
			lpb = NULL;
		}
	}
	return true;
}