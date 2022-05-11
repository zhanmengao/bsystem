#pragma once
#define VC_EXTRALEAN        // 从 Windows 头中排除极少使用的资料
#include <comdef.h>  
#include<stdint.h>
#include <comutil.h>
#include <Wbemidl.h> 
#include"../../../../C++space/Convert/Convert/Convert.h"
#include"../../../../C++space/StringTools/StringTools/WinStrUtils.h"
#include<map>


#pragma comment(lib, "wbemuuid.lib")  
#pragma comment(lib, "comsuppw.lib")
struct WMIPatchInfo
{
	stdstr Description;
	stdstr Name;
	int installYear;
	int installMonth;
	int installDay;
	friend OUTSTREAM& operator<<(OUTSTREAM& os, const WMIPatchInfo& info)
	{
		STDOUT << L"name:" << info.Name
			<< L"	install_time:" << info.GetInstall() << endl;
		return os;
	}
	stdstr GetDesc() const
	{
		//映射
		if (Description == _T("Update"))
			return _T("更新补丁");
		else if (Description == _T("Hotfix"))
			return  _T("修复补丁");
		else if (Description == _T("Security Update"))
			return _T("安全补丁");
		return _T("");
	}
	stdstr GetInstall() const
	{
		TCHAR strTime[100] = { 0 };
		wsprintf(strTime, _T("%04d-%02d-%02d"), installYear, installMonth, installDay);
		return strTime;
	}
};

//一条数据
struct WMIQuery
{
	union WMIQueryUnion
	{
		char data[1024] = { 0 };
		DWORD dwData;
		bool bVal;
	};
	VARTYPE dataType = USHRT_MAX;
	WMIQueryUnion data;
	WMIQuery()
	{}
	WMIQuery(const WMIQuery& other) :dataType(other.dataType)
	{
		memcpy(&data, &other.data, sizeof(data));
	}
	WMIQuery& operator=(const WMIQuery& other)
	{
		memcpy(&data, &other.data, sizeof(data));
		dataType = other.dataType;
	}
};

//一个硬件的所有数据 key名称 Data数据
typedef  map<stdstr, WMIQuery> WMIQueryData;


struct WMIDisk
{
	stdstr csManufacturer;				//制造商
	stdstr csInterfaceType;
	stdstr csName;
	stdstr csModel;
	stdstr csDescription;
	stdstr SerialNumber;
	unsigned __int64 csSize;									//大小（MB)
	DWORD BytesPerSector;
	stdstr Caption;
	DWORD Index;
};

struct WMIMemory
{
	TCHAR CreationClassName[MAX_PATH];
	TCHAR csDeviceLocator[MAX_PATH];				//内存位置
	TCHAR csType[MAX_PATH];								//内存种类
	TCHAR csDescription[MAX_PATH];
	TCHAR csName[MAX_PATH];
	TCHAR csSerialNum[MAX_PATH];								//序列号
	TCHAR Manufacturer[MAX_PATH];						//制造商
	uint64_t csCapacity;								//内存大小
	UINT	uSlotNumber;							//内存插槽数
	DWORD mSpeed;									//最大时钟频率

	DWORD ConfiguredClockSpeed;				//当前时钟频率
	stdstr BankLabel;
	DWORD MinVoltage;
	DWORD MaxVoltage;
	DWORD TotalWidth;
	stdstr PartNumber;
#ifdef __cplusplus
	WMIMemory()
	{}
	WMIMemory(const WMIMemory& obj) :mSpeed(obj.mSpeed), csCapacity(obj.csCapacity), uSlotNumber(obj.uSlotNumber)
		, BankLabel(obj.BankLabel), ConfiguredClockSpeed(obj.ConfiguredClockSpeed), MinVoltage(obj.MinVoltage),
		MaxVoltage(obj.MaxVoltage), TotalWidth(obj.TotalWidth), PartNumber(obj.PartNumber)
	{
		_tcscpy_s(CreationClassName, _countof(CreationClassName), obj.CreationClassName);
		_tcscpy_s(csDeviceLocator, _countof(csDeviceLocator), obj.csDeviceLocator);
		_tcscpy_s(csType, _countof(csType), obj.csType);
		_tcscpy_s(csDescription, _countof(csDescription), obj.csDescription);
		_tcscpy_s(csName, _countof(csName), obj.csName);
		_tcscpy_s(csSerialNum, _countof(csSerialNum), obj.csSerialNum);
		_tcscpy_s(Manufacturer, _countof(Manufacturer), obj.Manufacturer);
	}
	WMIMemory& operator=(const WMIMemory& obj)
	{
		_tcscpy_s(CreationClassName, _countof(CreationClassName), obj.CreationClassName);
		_tcscpy_s(csDeviceLocator, _countof(csDeviceLocator), obj.csDeviceLocator);
		_tcscpy_s(csType, _countof(csType), obj.csType);
		_tcscpy_s(csDescription, _countof(csDescription), obj.csDescription);
		_tcscpy_s(csName, _countof(csName), obj.csName);
		_tcscpy_s(csSerialNum, _countof(csSerialNum), obj.csSerialNum);
		_tcscpy_s(Manufacturer, _countof(Manufacturer), obj.Manufacturer);
		mSpeed = (obj.mSpeed);
		csCapacity = (obj.csCapacity);
		uSlotNumber = (obj.uSlotNumber);
		BankLabel = (obj.BankLabel);
		ConfiguredClockSpeed = (obj.ConfiguredClockSpeed);
		MinVoltage = (obj.MinVoltage);
		MaxVoltage=(obj.MaxVoltage);
		TotalWidth = (obj.TotalWidth); 
		PartNumber = (obj.PartNumber);
		return *this;
	}
#endif
};
struct WMIDesktop
{
	string szWallpaper;
};

//显示器（效果不好）
struct WMIMonitor
{
	TCHAR csManufacturer[MAX_PATH];			//制造商
	TCHAR csType[MAX_PATH];										//种类
	TCHAR csModel[MAX_PATH];
	TCHAR csCaption[MAX_PATH];
	TCHAR csDescription[MAX_PATH];
#ifdef __cplusplus
	WMIMonitor()
	{}
	WMIMonitor(const WMIMonitor& obj) 
	{
		_tcscpy_s(csManufacturer, _countof(csManufacturer), obj.csManufacturer);
		_tcscpy_s(csType, _countof(csType), obj.csType);
		_tcscpy_s(csModel, _countof(csModel), obj.csModel);
		_tcscpy_s(csCaption, _countof(csCaption), obj.csCaption);
		_tcscpy_s(csDescription, _countof(csDescription), obj.csDescription);
	}
	WMIMonitor& operator=(const WMIMonitor& obj)
	{
		_tcscpy_s(csManufacturer, _countof(csManufacturer), obj.csManufacturer);
		_tcscpy_s(csType, _countof(csType), obj.csType);
		_tcscpy_s(csModel, _countof(csModel), obj.csModel);
		_tcscpy_s(csCaption, _countof(csCaption), obj.csCaption);
		_tcscpy_s(csDescription, _countof(csDescription), obj.csDescription);
		return *this;
	}
#endif
};

//键盘
struct WMIKeyboard
{
	TCHAR csCaption[MAX_PATH];
	TCHAR csDescription[MAX_PATH];						//描述
	TCHAR csDeviceID[MAX_PATH];							//设备ID
	TCHAR csName[MAX_PATH];								//名称
	TCHAR csLayout[MAX_PATH];
	TCHAR SystemName[MAX_PATH];
	WMIKeyboard()
	{}
	WMIKeyboard(const WMIKeyboard& obj)
	{
		_tcscpy_s(csCaption, _countof(csCaption), obj.csCaption);
		_tcscpy_s(csDescription, _countof(csDescription), obj.csDescription);
		_tcscpy_s(csDeviceID, _countof(csDeviceID), obj.csDeviceID);
		_tcscpy_s(csName, _countof(csName), obj.csName);
		_tcscpy_s(csLayout, _countof(csLayout), obj.csLayout);
		_tcscpy_s(SystemName, _countof(SystemName), obj.SystemName);
	}
	WMIKeyboard& operator=(const WMIKeyboard& obj)
	{
		_tcscpy_s(csCaption, _countof(csCaption), obj.csCaption);
		_tcscpy_s(csDescription, _countof(csDescription), obj.csDescription);
		_tcscpy_s(csDeviceID, _countof(csDeviceID), obj.csDeviceID);
		_tcscpy_s(csName, _countof(csName), obj.csName);
		_tcscpy_s(csLayout, _countof(csLayout), obj.csLayout);
		_tcscpy_s(SystemName, _countof(SystemName), obj.SystemName);
		return *this;
	}
};

//指针设备
struct WMIPointer
{
	TCHAR csCaption[MAX_PATH];
	TCHAR csDescription[MAX_PATH];						//描述
	TCHAR csHardwareType[MAX_PATH];						//硬件类型
	TCHAR csManufacturer[MAX_PATH];
	TCHAR csName[MAX_PATH];
	TCHAR csDeviceID[MAX_PATH];											//设备ID
	TCHAR SystemName[MAX_PATH];
	TCHAR csPointingType[MAX_PATH];									//类型
	DWORD dwType;														//类型
	DWORD DeviceInterface;
	WMIPointer()
	{}
	WMIPointer(const WMIPointer& obj):dwType(obj.dwType), DeviceInterface(obj.DeviceInterface)
	{
		_tcscpy_s(csCaption, _countof(csCaption), obj.csCaption);
		_tcscpy_s(csDescription, _countof(csDescription), obj.csDescription);
		_tcscpy_s(csHardwareType, _countof(csHardwareType), obj.csHardwareType);
		_tcscpy_s(csManufacturer, _countof(csManufacturer), obj.csManufacturer);
		_tcscpy_s(csName, _countof(csName), obj.csName);
		_tcscpy_s(csDeviceID, _countof(csDeviceID), obj.csDeviceID);
		_tcscpy_s(SystemName, _countof(SystemName), obj.SystemName);
		_tcscpy_s(csPointingType, _countof(csPointingType), obj.csPointingType);
	}
	WMIPointer& operator=(const WMIPointer& obj)
	{
		_tcscpy_s(csCaption, _countof(csCaption), obj.csCaption);
		_tcscpy_s(csDescription, _countof(csDescription), obj.csDescription);
		_tcscpy_s(csHardwareType, _countof(csHardwareType), obj.csHardwareType);
		_tcscpy_s(csManufacturer, _countof(csManufacturer), obj.csManufacturer);
		_tcscpy_s(csName, _countof(csName), obj.csName);
		_tcscpy_s(csDeviceID, _countof(csDeviceID), obj.csDeviceID);
		_tcscpy_s(SystemName, _countof(SystemName), obj.SystemName);
		_tcscpy_s(csPointingType, _countof(csPointingType), obj.csPointingType);
		dwType = (obj.dwType);
		DeviceInterface = (obj.DeviceInterface);
		return *this;
	}
	stdstr GetTypeStr()
	{
		TCHAR csPointingType[64] = { 0 };
		switch (dwType)
		{
		case 1:
			_tcscpy_s(csPointingType, _countof(csPointingType), _T("Other"));
			break;
		case 2:
			_tcscpy_s(csPointingType, _countof(csPointingType), _T("Unknown"));
			break;
		case 3:
			_tcscpy_s(csPointingType, _countof(csPointingType), _T("Mouse"));
			break;
		case 4:
			_tcscpy_s(csPointingType, _countof(csPointingType), _T("Track Ball"));
			break;
		case 5:
			_tcscpy_s(csPointingType, _countof(csPointingType), _T("Track Point"));
			break;
		case 6:
			_tcscpy_s(csPointingType, _countof(csPointingType), _T("Glide Point"));
			break;
		case 7:
			_tcscpy_s(csPointingType, _countof(csPointingType), _T("Touch Pad"));
			break;
		case 8:
			_tcscpy_s(csPointingType, _countof(csPointingType), _T("Touch Screen"));
			break;
		case 9:
			_tcscpy_s(csPointingType, _countof(csPointingType), _T("Mouse - Optical Sensor"));
			break;
		default:
			break;
		}
		return csPointingType;
	}
};

//打印机
struct WMIPrint
{
	stdstr csName;
	stdstr csDeviceName;
	stdstr csPortName;
	stdstr PrintProcessor;
	bool WorkOffline;
	stdstr SystemName;
	bool Shared;
	stdstr Status;
	bool Network;
	bool Local;
	bool Hidden;
	bool Default;
};

struct WMIBIOS
{
	stdstr csSerialNum;
	stdstr Version;
	stdstr Name;
	stdstr Description;
	stdstr SMBIOSBIOSVersion;
	DWORD SMBIOSMajorVersion;
	DWORD SMBIOSMinorVersion;
	stdstr SoftwareElementID;
};

struct WMIMainBoard
{
	stdstr Name;
	stdstr Manufacturer;
	stdstr Description;
	stdstr Product;
	stdstr Tag;
	stdstr Version;
};

struct WMICPU
{
	string ProcessorId;
	stdstr Name;
	stdstr Caption;
	stdstr Manufacturer;
	DWORD L2CacheSize;
	DWORD L3CacheSize;
	DWORD NumberOfCores;
	DWORD ThreadCount;
	DWORD ExtClock;
	DWORD CurrentClockSpeed;
	DWORD MaxClockSpeed;
};

struct WMIProcess
{
	stdstr Name;
	stdstr Caption;
	stdstr ExecutablePath;
	stdstr CommandLine;
	DWORD SessionId;
	DWORD ProcessId;
	DWORD ParentProcessId;
};

struct WMIComputerSystem
{
	stdstr Name;
	stdstr Manufacturer;
	stdstr Model;
	stdstr TotalPhysicalMemory;
};
#ifndef _THIS_IS_MACRO_DEFINES_
#define _THIS_IS_MACRO_DEFINES_

#define NOT_AVAILABLE	_T( "N/A")


#define		WINDOWS_WORKSTATION	0	// Windows workstation device type
#define		WINDOWS_SERVER		1	// Windows server device type
#define     WINDOWS_NOTEBOOK	2	// Windows notebook or laptop device type
#define		NOVELL_SERVER		3	// Novell Netware server device type
#define		MAC_WORKSTATION		4	// Apple MacOS workstation device type
#define		MAC_SERVER			5	// Apple MacOS server device type
#define		MAC_NOTEBOOK		6	// Apple MacOS notebook or laptop device type
#define		UNIX_WORKSTATION	7	// Unix workstation device type
#define		UNIX_SERVER			8	// Unix server device type
#define		UNIX_NOTEBOOK		9	// Unix notebook or laptop device type
#define		HANDHELD_DEVICE		10	// HandHeld device like Palm or Pocket PC
#define		NETWORK_PRINTER		11	// Network printer device type
#define		NETWORK_HUB			12	// Hub device type
#define		NETWORK_SWITCH		13	// Switch device type
#define		NETWORK_ROUTER		14	// Router device type
#define		IP_PHONE			15	// IP phone device type
#define		IP_PABX				16	// IP PABX device type
#define		USER_WORKGROUP		17	// User workgroup device type
#define		UNKNOWN_DEVICE		18	// Unknown device type
#define		NEW_DEVICE			65535 // Constant for adding new device
#define		CURRENT_DEVICE		65534 // Constant for retrieving the device type of the current device
#define ONE_MEGABYTE	1048576 // (1024 * 1024)


#define MEMORY_SLOT_TYPE_UNKNOWN			0
#define MEMORY_SLOT_TYPE_OTHER				1
#define MEMORY_SLOT_TYPE_DRAM				2
#define MEMORY_SLOT_TYPE_SYNCHRONOUS_DRAM	3
#define MEMORY_SLOT_TYPE_CACHE_DRAM			4
#define MEMORY_SLOT_TYPE_EDO				5
#define MEMORY_SLOT_TYPE_EDRAM				6
#define MEMORY_SLOT_TYPE_VRAM				7
#define MEMORY_SLOT_TYPE_SRAM				8
#define MEMORY_SLOT_TYPE_RAM				9
#define MEMORY_SLOT_TYPE_ROM				10
#define MEMORY_SLOT_TYPE_FLASH				11
#define MEMORY_SLOT_TYPE_EEPROM				12
#define MEMORY_SLOT_TYPE_FEPROM				13
#define MEMORY_SLOT_TYPE_EPROM				14
#define MEMORY_SLOT_TYPE_CDRAM				15
#define MEMORY_SLOT_TYPE_3DRAM				16
#define MEMORY_SLOT_TYPE_SDRAM				17
#define MEMORY_SLOT_TYPE_SGRAM				18
#define MEMORY_SLOT_TYPE_RDRAM				19
#define MEMORY_SLOT_TYPE_DDR				20
#define MEMORY_SLOT_TYPE_DDR2			21

#endif