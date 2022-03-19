#include "MouseManager.h"
#include "LogManager.h"
#include<string>
#include"../../../../Paint/GDI/PicHelper/PicHelper/PicHelper.h"
#include"Tools.h"
#include"../../../Time/Time/Time.hpp"
#include"../../../Time/Time/WinTime.h"
#include<direct.h>
#include"Config.h"
#include"../../../../C++space/Convert/Convert/Convert.h"
using namespace std;

void MouseManager::Start()
{
	InitRaw();
}

void MouseManager::OnDestroy()
{

}
void MouseManager::Update()
{

}
void MouseManager::HandleMsg(const MSG& rMsg)
{
	if (mLastPt.x == rMsg.pt.x && mLastPt.y == rMsg.pt.y)
	{
		return;
	}
	HandleMsg(rMsg.pt);
}
void MouseManager::HandleMsg(const POINT& pt)
{
	KeyLog::LogEvent(EVENT_TYPE::EVENT_MOUSE_DOWN,to_string(pt.x) + "," +to_string(pt.y));
	//截图
	string folderName = ConfigManager::PicRoot() + WinTimeUtils::GetNowYMD() + "\\";
	mkdir((folderName).c_str());
	string fileName = WinTimeUtils::GetNowHMSM();
	string bigPicName = folderName + fileName + "_max.jpg";
	string smailPicName = folderName + fileName + ".jpg";
	RECT rect;
	rect.left = pt.x - ConfigManager::picWidth() / 2;
	rect.right = pt.x + ConfigManager::picWidth() / 2;
	rect.bottom = pt.y + ConfigManager::picHeight() / 2;
	rect.top = pt.y - ConfigManager::picHeight() / 2;
	PicHelper::GetScreenSnap(Convert::StringToWString(bigPicName),&rect);
	PicHelper::CompressImageQuality(Convert::StringToWString(bigPicName).c_str(),
		Convert::StringToWString(smailPicName).c_str(), ConfigManager::PicQual());
	//删除旧的
	remove(bigPicName.c_str());
	mLastPt = pt;
}
void MouseManager::InitRaw()
{
	RAWINPUTDEVICE rid;  //设备信息
	rid.usUsagePage = 0x01;
	rid.usUsage = 0x02; //rid.usUsagePage = 0x01; rid.usUsage = 0x02; 为鼠标
	rid.dwFlags = RIDEV_INPUTSINK;
	rid.hwndTarget = GlobalData::hWindow;
	bool ret = RegisterRawInputDevices(&rid, 1, sizeof(RAWINPUTDEVICE));
	if (ret)
	{
		KeyLog::Log("MouseManager RegisterRawInputDevices Sec!\n");
	}
	else
	{
		KeyLog::Log("MouseManager RegisterRawInputDevices Fail! Error = " + to_string(GetLastError()));
	}
}