#pragma once
#include <iostream>
#include <iomanip>
#include <atlstr.h>
#include<vector>
#include"WMIDef.h"
using namespace std;
#pragma comment(lib, "version.lib")

struct process_info
{
	std::string name;
	stdstr version;
	int count;
};
typedef long(__stdcall *fnNtQuerySystemInformation)(
	IN     UINT SystemInformationClass,
	OUT PVOID SystemInformation,
	IN     ULONG SystemInformationLength,
	OUT PULONG ReturnLength OPTIONAL
	);
class off_on
{
public:
	BOOL g_IsWork;
	HANDLE g_hThread;
	string g_boot_time;
	string g_countIniPath;
	map<string, process_info> g_processList;
	fnNtQuerySystemInformation NtQuerySystemInformation;
};

class CWMI
{
public:
private:
	IWbemLocator *pLoc_;
	IWbemServices *pSvc_;
public:
	CWMI() :pLoc_(NULL), pSvc_(NULL) {}
	~CWMI() { ClearWMI(); }
	bool InitWMI();
	bool ClearWMI();
	bool QuerySystemInfo();
	static stdstr GetFileVersion(const stdstr& exePath);
public:
	static HRESULT Query(char* wql, std::vector<WMIQueryData>& rData);
	static WMIQuery GetInfo(const WCHAR* wszQueryInfo, IWbemClassObject *pclsObj);
	static void PrintWMIVec(const std::vector<WMIQueryData>& rData);
	static HRESULT GetPatchList(vector<WMIPatchInfo>& patchlist);					//获取补丁信息
	static HRESULT GetMonitorProcess(off_on& ofon);
public:
	static HRESULT GetDiskInfo(std::vector<WMIQueryData>& vec)									//获取磁盘
	{
		return Query("SELECT * from Win32_DiskDrive", vec);
	}
	static HRESULT GetWin32_LogicDisk(std::vector<WMIQueryData>& vec)					//虚拟磁盘
	{
		return Query("SELECT * FROM Win32_LogicalDisk", vec);
	}
	static HRESULT GetWin32_LogicalDiskToPartition(std::vector<WMIQueryData>& vec)					//虚拟磁盘
	{
		return Query("SELECT * FROM Win32_LogicalDiskToPartition", vec);
	}
	static HRESULT GetMemoryInfo(std::vector<WMIQueryData>& vec)							//获取内存
	{
		return Query("SELECT * from Win32_PhysicalMemory", vec);
	}
	static HRESULT GetDesktop(std::vector<WMIQueryData>& vec)								//获取桌面
	{
		return Query("SELECT * from Win32_Desktop", vec);
	}
	static HRESULT GetMonitor(std::vector<WMIQueryData>& vec)								//获取显示器
	{
		return Query("SELECT * from Win32_DesktopMonitor", vec);
	}
	static HRESULT GetKeyBoard(std::vector<WMIQueryData>& vec)
	{
		return Query("SELECT * from Win32_Keyboard", vec);
	}
	static HRESULT GetMouse(std::vector<WMIQueryData>& vec)
	{
		return Query("SELECT * from Win32_PointingDevice", vec);
	}
	static HRESULT GetPrint(std::vector<WMIQueryData>& vec)
	{
		return Query("SELECT * from Win32_Printer", vec);
	}
	static HRESULT GetWin32_BIOS(std::vector<WMIQueryData>& vec)
	{
		return Query("SELECT * FROM win32_BIOS", vec);
	}
	static HRESULT GetWin32_CPU(std::vector<WMIQueryData>& vec)
	{
		return Query("SELECT * FROM Win32_Processor", vec);
	}
	static HRESULT GetWin32_BaseBoard(std::vector<WMIQueryData>& vec)
	{
		return Query("SELECT * FROM WIN32_BaseBoard", vec);
	}	
	static HRESULT GetWin32_ComputerSystem(std::vector<WMIQueryData>& vec)
	{
		return Query("SELECT * FROM Win32_ComputerSystem", vec);
	}
	static HRESULT GetWin32_OS(std::vector<WMIQueryData>& vec)
	{
		return Query("SELECT * FROM Win32_OperatingSystem", vec);
	}
	static HRESULT GetWin32_PageFile(std::vector<WMIQueryData>& vec)
	{
		return Query("SELECT * FROM Win32_PageFile", vec);
	}
	static HRESULT GetWin32_Thread(std::vector<WMIQueryData>& vec)
	{
		return Query("SELECT * FROM Win32_Thread", vec);
	}
	static HRESULT GetWin32_Service(std::vector<WMIQueryData>& vec)
	{
		return Query("SELECT * FROM Win32_Service", vec);
	}
	static HRESULT GetWin32_Fan(std::vector<WMIQueryData>& vec)
	{
		return Query("SELECT * FROM Win32_Fan", vec);
	}
	static HRESULT GetWin32_TemperatureProbe(std::vector<WMIQueryData>& vec)
	{
		return Query("SELECT * FROM Win32_TemperatureProbe", vec);
	}
	static HRESULT GetWin32_Battery(std::vector<WMIQueryData>& vec)
	{
		return Query("SELECT EstimatedChargeRemaining,EstimatedRunTime FROM Win32_Battery", vec);
	}
	static HRESULT GetWin32_Process(std::vector<WMIQueryData>& vec)
	{
		return Query("SELECT Name,Caption,ExecutablePath,SessionId,ProcessId FROM Win32_Process", vec);
	}
};