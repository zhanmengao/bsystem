#pragma once
#include<Windows.h>

class HardwareUtils
{
public:
	//����Ƿ����
	static bool GetMouseExist()
	{
		return GetSystemMetrics(SM_MOUSEPRESENT);
	}
	//�ж�����ϼ��ĸ���
	static bool GetMouseCount()
	{
		return GetSystemMetrics(SM_CMOUSEBUTTONS);
	}
};