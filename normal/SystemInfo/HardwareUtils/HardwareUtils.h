#pragma once
#include<Windows.h>

class HardwareUtils
{
public:
	//鼠标是否存在
	static bool GetMouseExist()
	{
		return GetSystemMetrics(SM_MOUSEPRESENT);
	}
	//判断鼠标上键的个数
	static bool GetMouseCount()
	{
		return GetSystemMetrics(SM_CMOUSEBUTTONS);
	}
};