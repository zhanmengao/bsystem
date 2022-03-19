#pragma once
#include <windows.h>
#include <devguid.h>    // for GUID_DEVCLASS_CDROM etc
#include <setupapi.h>
#include <cfgmgr32.h>   // for MAX_DEVICE_ID_LEN, CM_Get_Parent and CM_Get_Device_ID
#define INITGUID
#include <tchar.h>
#include <stdio.h>
#include<string>
#include<vector>
#include<map>
#include"../../../C++space/StringTools/StringTools/StringHead.h"

//#include "c:\WinDDK\7600.16385.1\inc\api\devpkey.h"

// include DEVPKEY_Device_BusReportedDeviceDesc from WinDDK\7600.16385.1\inc\api\devpropdef.h
struct USBDevice
{
	stdstr DeviceInstanceID;
	stdstr Description;
	std::wstring ContainerId;
	std::wstring BusReportedDeviceDescription;
	std::wstring DeviceFriendlyName;
	std::wstring Manufacturer;
	std::wstring LocationInfo;
	std::wstring SecurityDescriptor;
	std::wstring DisplayCategory;
	stdstr vid;
	stdstr pid;
	stdstr mi;
	bool operator==(const USBDevice& other)
	{
		return DeviceInstanceID == other.DeviceInstanceID &&
			vid == other.vid &&
			pid == other.pid;
	}
	bool operator!=(const USBDevice& other)
	{
		return !(*this == other);
	}
};
class USBUtils
{
public:
	static std::vector<USBDevice> GetUsbList();
	static std::map<stdstr,USBDevice> GetUsbMap();
	static std::map<stdstr, USBDevice> GetUsbInstanceMap();
private:
	static std::vector<USBDevice> ListDevices(CONST GUID *pClassGuid, LPCTSTR pszEnumerator);
};