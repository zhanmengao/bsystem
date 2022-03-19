#include "Tools.h"
#include <map>
#include "tlhelp32.h"
#include "LogManager.h"
#include <ctype.h>
#include <tchar.h>
#include"../../../File/FileManager/FileManager/PathHelper.hpp"
#include"../../../../C++space/Convert/Convert/Convert.h"

#include"../../../Windows/WNDUtils/WNDUtils/WindowHelper.h"
#include"../../../Process/WindowsProcessHelper/CreateProcess/ProcessElevationUtils.h"
ofstream Tools::mLogFileStream;
bool Tools::SaveDataToFile(const std::string& szFilePath, const std::string& text)
{
	ofstream ofs;
	ofs.open(szFilePath.c_str(), fstream::app);
	if (!ofs.is_open())
	{
		return false;
	}
	//写数据
	ofs << text;
	ofs.close();
	return true;
}

wstring Tools::GetProcessNameByPid(DWORD nPid)
{
	return ProcessHelper::GetProcessName(nPid);
}
string Tools::GetCurDir()
{
	return Convert::WStringToString(PathHelper::GetCurDir());
}
string Tools::GetModuleFolder()
{
	return  Convert::WStringToString(PathHelper::GetModuleFolder(GlobalData::hinstDLL));
}

wstring Tools::string2wstring(const string& str)
{
	return Convert::StringToWString(str);
}
string Tools::wstring2string(const wstring& str)
{
	return Convert::WStringToString(str);
}
bool Tools::Shift()
{
	return ((unsigned short)GetAsyncKeyState(VK_SHIFT)) >> 15 == 1;
}
bool Tools::Ctrl()
{
	return ((unsigned short)GetAsyncKeyState(VK_CONTROL)) >> 15 == 1
		|| ((unsigned short)GetAsyncKeyState(VK_LCONTROL)) >> 15 == 1
		|| ((unsigned short)GetAsyncKeyState(VK_RCONTROL)) >> 15 == 1;
}
bool Tools::Alt()
{
	return ((unsigned short)GetAsyncKeyState(VK_MENU)) >> 15 == 1 ||
		((unsigned short)GetAsyncKeyState(VK_LMENU)) >> 15 == 1 ||
	((unsigned short)GetAsyncKeyState(VK_RMENU)) >> 15 == 1;
}
bool Tools::CapLock()
{
	DWORD dwRet = GetKeyState(VK_CAPITAL);
	return dwRet & 1 > 0;
}
string Tools::GetKeyName(unsigned int vkCode)
{
	bool bShift = Tools::Shift();
	bool bCtrl = Tools::Ctrl();
	bool bAlt = Tools::Alt();
	bool bCapLock = Tools::CapLock();
	string head = "";
	if (bCtrl)
	{
		head = "【Ctrl +";
	}
	if (bAlt)
	{
		if (head == "")
		{
			head = "【";
		}
		head += "Alt + ";
	}
	if (bShift)
	{
		if (head == "")
		{
			head = "【";
		}
		head += "Shift + ";
	}
	string ret = "";
	if (bShift)
	{
		switch (vkCode)
		{
		case 0x30:
			ret = ")";
			break;
		case 0x31:
			ret = "!";
			break;
		case 0x32:
			ret = "@";
			break;
		case 0x33:
			ret = "#";
			break;
		case 0x34:
			ret = "$";
			break;
		case 0x35:
			ret = "%";
			break;
		case 0x36:
			ret= "^";
			break;
		case 0x37:
			ret = "&";
			break;
		case 0x38:
			ret = "*";
			break;
		case 0x39:
			ret = "(";
			break;
		case 0xBF:
			ret = "?";
			break;
		case 0xBB:
			ret = "+";
			break;
		case 0xBD:
			ret = "_";
			break;
		case 0xE2:
			ret = ">";
			break;
		case 0x1C:
			ret = "【VK_CONVERT】";
			break;
		case 0x56:
			ret = "@";
			break;
		case  0x2A:
			ret = "【PRINT】";
			break;
		case  0x2E:
			ret = "【Delete】";
			break;
		case 0xAA:
			ret = "【Search】";
			break;
		case  0xF2:
			ret = "【Copy】";
			break;
		case 0xFE:
			ret = "【Clear】";
			break;
		case  0x3:
			ret = "【Connect】";
			break;
		case 0x6:
			ret = "【Logoff】";
			break;
		case VK_OEM_4:
			ret = "{";
			break;
		case VK_OEM_6:
			ret = "}";
			break;
		case 0xDC:
			ret = "|";
		case VK_OEM_1:
			ret = ":";
			break;
		case VK_OEM_7:
			ret = "\"";
			break;
		case 0xBC:
			ret = "<";
			break;
		case 0xBE:
			ret = ">";
			break;
		case 0x6D:
			ret = "_";
			break;
		case VK_OEM_3:
			ret = "~";
			break;
		}
	}
	else
	{
		switch (vkCode)
		{
		case 0x30:
			ret = "0";
			break;
		case 0x31:
			ret = "1";
			break;
		case 0x32:
			ret = "2";
			break;
		case 0x33:
			ret = "3";
			break;
		case 0x34:
			ret = "4";
			break;
		case 0x35:
			ret = "5";
			break;
		case 0x36:
			ret = "6";
			break;
		case 0x37:
			ret = "7";
			break;
		case 0x38:
			ret = "8";
			break;
		case 0x39:
			ret = "9";
			break;
		case 0xBF:
			ret = "/";
			break;
		case 0xBB:
			ret = "=";
			break;
		case 0xBC:
			ret = ",";
			break;
		case 0xBE:
			ret = ".";
			break;
		case 0xBD:
			ret = "-";
			break;
		case VK_OEM_4:
			ret = "[";
			break;
		case VK_OEM_6:
			ret = "]";
			break;
		case 0xDC:
			ret = "\\";
			break;
		case VK_OEM_1:
			ret = ";";
			break;
		case VK_OEM_7:
			ret = "'";
			break;
		case 0x6D:
			ret = "-";
			break;
		case VK_OEM_3:
			ret = "`";
			break;
		}
	}
	if (ret.empty())
	{
		if (!bShift && !bCapLock)
		{
			switch (vkCode)
			{
			case 0x41:
				ret = "a";
				break;
			case 0x42:
				ret = "b";
				break;
			case 0x43:
				ret = "c";
				break;
			case 0x44:
				ret = "d";
				break;
			case 0x45:
				ret = "e";
				break;
			case 0x46:
				ret = "f";
				break;
			case 0x47:
				ret = "g";
				break;
			case 0x48:
				ret = "h";
				break;
			case 0x49:
				ret = "i";
				break;
			case 0x4A:
				ret = "j";
				break;
			case 0x4B:
				ret = "k";
				break;
			case 0x4C:
				ret = "l";
				break;
			case 0x4D:
				ret = "m";
				break;
			case 0x4E:
				ret = "n";
				break;
			case 0x4F:
				ret = "o";
				break;
			case 0x50:
				ret = "p";
				break;
			case 0x52:
				ret = "r";
				break;
			case 0x53:
				ret = "s";
				break;
			case 0x54:
				ret = "t";
				break;
			case 0x55:
				ret = "u";
				break;
			case 0x56:
				ret = "v";
				break;
			case 0x5A:
				ret = "z";
				break;
			case 0x51:
				ret = "q";
				break;
			case 0x57:
				ret = "w";
				break;
			case 0x59:
				ret = "y";
				break;
			case 0x58:
				ret = "x";
				break;
			}
		}
		else
		{
			switch (vkCode)
			{
			case 0x41:
				ret = "A";
				break;
			case 0x42:
				ret = "B";
				break;
			case 0x43:
				ret = "C";
				break;
			case 0x44:
				ret = "D";
				break;
			case 0x45:
				ret = "E";
				break;
			case 0x46:
				ret = "F";
				break;
			case 0x47:
				ret = "G";
				break;
			case 0x48:
				ret = "H";
				break;
			case 0x49:
				ret = "I";
				break;
			case 0x4A:
				ret = "J";
				break;
			case 0x4B:
				ret = "K";
				break;
			case 0x4C:
				ret = "L";
				break;
			case 0x4D:
				ret = "M";
				break;
			case 0x4E:
				ret = "N";
				break;
			case 0x4F:
				ret = "O";
				break;
			case 0x50:
				ret = "P";
				break;
			case 0x52:
				ret = "R";
				break;
			case 0x53:
				ret = "S";
				break;
			case 0x54:
				ret = "T";
				break;
			case 0x55:
				ret = "U";
				break;
			case 0x56:
				ret = "V";
				break;
			case 0x5A:
				ret = "Z";
				break;
			case 0x51:
				ret = "Q";
				break;
			case 0x57:
				ret = "W";
				break;
			case 0x59:
				ret = "Y";
				break;
			case 0x58:
				ret = "X";
				break;
			}
		}
	}

	if (ret.empty())
	{
		switch (vkCode)
		{
		case VK_SPACE:
			ret = "【Space】";
			break;
		case 0x2E:
			ret = "【Delete】";
			break;
		case VK_BACK:
			ret = "【BackSpace】";
			break;
		case VK_RETURN:
			ret = "【Enter】";
			break;
		case VK_LCONTROL:
			ret = "【Ctrl】";
			break;
		case VK_RCONTROL:
			ret = "【Ctrl】";
			break;
		case VK_TAB:
			ret = "【Tab】";
			break;
		case 0x25:
			ret = "【Left Arrow】";
			break;
		case 0x26:
			ret = "【Up Arrow】";
			break;
		case 0x27:
			ret = "【Right Arrow】";
			break;
		case 0x28:
			ret = "【Down Arrow】";
			break;
		case VK_ESCAPE:
			ret = "【Esc】";
			break;
		case VK_CAPITAL:
			ret = "【Caps Lock】";
			break;
		case VK_RSHIFT:
			ret = "【Right Shift】";
			break;
		case VK_LSHIFT:
			ret = "【Left Shift】";
			break;
		case VK_LMENU:
			ret = "【Left Alt】";
			break;
		case VK_RMENU:
			ret = "【Right Alt】";
			break;
		case VK_LWIN:
			ret = "【Left Win】";
			break;
		case VK_RWIN:
			ret = "【Right Win】";
			break;
		case VK_INSERT:
			ret = "【Insert】";
			break;
		case VK_SCROLL:
			ret = ("【Scroll Lock】");
			break;
		case VK_HOME:
			ret = "【Home】";
			break;
		case VK_END:
			ret = "【End】";
			break;
		case VK_PRIOR:
			ret = "【Page Up】";
			break;
		case VK_NEXT:
			ret = "【Page Down】";
			break;
		case VK_SNAPSHOT:
			ret = "【Print Screen】";
			break;
		case VK_F1:
			ret = "【F1】";
			break;
		case VK_F2:
			ret = "【F2】";
			break;
		case VK_F3:
			ret = "【F3】";
			break;
		case VK_F4:
			ret = "【F4】";
			break;
		case VK_F5:
			ret = "【F5】";
			break;
		case VK_F6:
			ret = "【F6】";
			break;
		case VK_F7:
			ret = "【F7】";
			break;
		case VK_F8:
			ret = "【F8】";
			break;
		case VK_F9:
			ret = "【F9】";
			break;
		case VK_F10:
			ret = "【F10】";
			break;
		case VK_F11:
			ret = "【F11】";
			break;
		case VK_F12:
			ret = "【F12】";
			break;
		case VK_NUMPAD0:
			ret = "0";
			break;
		case VK_NUMPAD1:
			ret = "1";
			break;
		case VK_NUMPAD2:
			ret = "2";
			break;
		case VK_NUMPAD3:
			ret = "3";
			break;
		case VK_NUMPAD4:
			ret = "4";
			break;
		case VK_NUMPAD5:
			ret = "5";
			break;
		case VK_NUMPAD6:
			ret = "6";
			break;
		case VK_NUMPAD7:
			ret = "7";
			break;
		case VK_NUMPAD8:
			ret = "8";
			break;
		case VK_NUMPAD9:
			ret = "9";
			break;
		case 0x6F:
			ret = "/";
			break;
		case 0x6A:
			ret = "*";
			break;
		case 0x6B:
			ret = "+";
			break;
		case 0x6E:
			ret = ",";
			break;
		}
	}
	if (ret.empty())
	{
		ret = "【vkCode:" + to_string(vkCode) + "】";
	}
	if (head != "")
	{
		return head + ret + "】";
	}
	return ret;
}
wstring Tools::GetWindowTitle(HWND hwnd)
{
	return WindowHelper::GetWindowTitle(hwnd);
}
_SYSTEMTIME Tools::GetTime()
{
	_SYSTEMTIME t;
	GetLocalTime(&t);
	return t;
}
string Tools::GetFormatTime()
{
	return GetFormatTime(GetTime());
}
string Tools::GetFormatTime(const _SYSTEMTIME& t)
{
	string ret("[");
	ret += std::to_string(t.wYear);
	ret += "/";

	ret += to_string(t.wMonth);
	ret += "/";

	ret += to_string(t.wDay);
	ret += " ";

	ret += to_string(t.wHour);
	ret += ":";

	ret += to_string(t.wMinute);
	ret += ":";

	ret += to_string(t.wSecond);
	ret += "]";
	return ret;
}
bool Tools::IsRunasAdmin()
{
	return ProcessElevationUtils::IsRunasAdmin();
}
bool Tools::IsSystem()
{
	return ProcessElevationUtils::IsSystem();
}