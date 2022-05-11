#pragma once
#include"SysDef.h"


class SysInfoUtils
{
public:
	static SYSTEM_INFO GetSystemInfo();														//获取系统信息								
	static stdstr GetUser();																					//获取用户名
	static stdstr GetHostName();																		//获取主机名
	static stdstr _GetComputerName();																		//获取主机名
	static stdstr GetPCName();																			//获取机器名
	static struct WindowsInfo GetWindowsInfo();											//获取windows信息
	static struct WindowsVersion GetWindowsVersion();										//获取版本信息
	static net_info GetNetInfo();																		//获取网络信息
	static BOOL GetAdapterInfo(vector<AdapterInfo>& vec);						//获取网卡信息
	static std::string GetMac(const char* ipaddr);												//根据ip获取mac
	static net_info GetNetGateInfo();
	static int GetTCPportlist(std::vector<TCPPort>& vec);								//扫描TCP端口
	static int GetUDPportlist(std::vector<UDPPort>& vec);								//扫描UDP端口
	static int GetEventLog(std::vector<EventLog>& vec);								//获取Windows日志
public:
	inline static stdstr GetRegisteredOwner()												//获取注册所有者
	{
		return OsInfo(_T("RegisteredOwner"));
	}
	inline static stdstr GetProductName()															//获取产品名称
	{
		return OsProductInfo(_T("SystemProductName"));
	}
	inline static stdstr GetProductID()															//获取产品ID
	{
		return OsProductInfo(_T("ProductId"));
	}
	inline static stdstr GetCPU()																		//获取CPU名称
	{
		return OsCpuInfo(_T("ProcessorNameString"));
	}
	inline static stdstr GetOSName()																	//获取操作系统名称
	{
		return OsInfo(_T("ProductName"));
	}
	inline static stdstr GetOSVersion()																//操作系统版本
	{
		return OsInfo(_T("CurrentVersion"));
	}
	inline static stdstr GetOSReleaseID()																//操作系统发布ID
	{
		return OsInfo(_T("ReleaseId"));
	}
	inline static stdstr GetOSCurrBuild()													//操作系统内部版本
	{
		return OsInfo(_T("CurrentBuild"));
	}
	inline static DWORD GetOSInstallDate()													//系统安装日期
	{
		return DWOsInfo(_T("InstallDate"));
	}
private:
	static stdstr OsProductInfo(const TCHAR* str);
	static stdstr OsCpuInfo(const TCHAR* str);
	static stdstr OsInfo(const TCHAR* str);
	static DWORD DWOsInfo(const TCHAR* str);
	static ULONG GetHostOrder(const std::string& strIp);
	static BOOL RegGetIP(AdapterInfo *pAI, const char* lpszAdapterName, int nIndex = 0);
};