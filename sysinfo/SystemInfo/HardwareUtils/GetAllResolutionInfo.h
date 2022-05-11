#ifndef GETALLRESOLUTIONINFO_H
#define GETALLRESOLUTIONINFO_H
#include <SDKDDKVer.h>

#include <stdio.h>
#include <tchar.h>
#include <Windows.h>
#include <vector>
#include"EDID.h"
#include"../../../../C++space/StringTools/StringTools/StringHead.h"
using namespace std;
//////////////////////////////////////////////////////////////////////////
// 显示器模式信息
struct MonitorModeInfo
{
	unsigned int m_nWidth;				    
	unsigned int m_nHeight;

	MonitorModeInfo(int nWidth_ = 0, int nHeight_ = 0) : m_nWidth(nWidth_), m_nHeight(nHeight_) {}

	friend bool operator== (const MonitorModeInfo Left_, const MonitorModeInfo Right_)
	{
		return (Left_.m_nWidth == Right_.m_nWidth) && (Left_.m_nHeight == Right_.m_nHeight);
	}
	bool operator>(const MonitorModeInfo& other)
	{
		return m_nWidth*m_nHeight >= other.m_nWidth*other.m_nHeight;
	}
	bool operator<(const MonitorModeInfo& other)
	{
		return !(*this > other);
	}
	std::string Desc()
	{
		char buffer[1024] = { 0 };
		sprintf_s(buffer, sizeof(buffer), "%u x %u", m_nWidth, m_nHeight);
		return buffer;
	}
};

#define MAX_RESOULATION_CNTS 32

typedef struct Monitor
{
	stdstr   mi_DeviceName;		//设备名称
	stdstr   mi_DeviceString;	//视频输出设备名
	stdstr	mi_DeviceKey;
	stdstr   mi_DeviceID;
	BOOL   mi_DeviceActive;			//接口激活状态
	BOOL   mi_DevicePrimary;		//是否主显示器
	DWORD  mi_StateFlags;			//状态值
	RECT   mi_rcWork;				//屏幕显示区域
	DWORD  mi_dmPelsWidth;			//屏幕显示宽度
	DWORD  mi_dmPelsHeight;			//屏幕显示高度
	DWORD  mi_dmDisplayFrequency;	//屏幕显示频率
	int    mi_nColorDepth;			//屏幕显示色深
	// benben 新增字段
	int    mi_Id;                   //设备标识ID
	MonitorModeInfo m_CurUsedResolution;  // 当前使用的分辨率
	int   m_AllSupportSize;          // 支持的显示器分辨率的个数
	MonitorModeInfo m_AllSupportResolution[MAX_RESOULATION_CNTS];
public:
	MonitorModeInfo MaxMonitorMode()
	{
		MonitorModeInfo max = m_CurUsedResolution;
		for (int i = 0; i < m_AllSupportSize; i++)
		{
			if (max < m_AllSupportResolution[i])
			{
				max = m_AllSupportResolution[i];
			}
		}
		return max;
	}
}st_MI;


class CMonitorList
{
public:
	static BOOL GetMonitors(vector<MonitorInfo> &vec);								//获取显示器信息
public:
	CMonitorList();
	~CMonitorList();
	long setDisplayPosition(LPCTSTR lpszDeviceName, long x, long y);		// 设置显示器位置	
	long setDisplayRatio(LPCTSTR lpszDeviceName, int width, int height);		// 修改指定设备分辨率	
	void setGlobalDisplayRatio(int width, int height);		// 设置非主显示器分辨率，注意：是非主显示器（拼接屏）	
	vector<st_MI> OutputMointorInfo();				// 输出显示器信息	
private:
	vector<st_MI> m_miGroup;		//显示器信息
	static vector<HMONITOR> hMonitorGroup;
private:
	int ExactNumicFromString(const TCHAR* ch);
public:
	static int CALLBACK MonitorEnumProc(HMONITOR hMonitor, 
		HDC hdc,
		LPRECT lpRMonitor,
		LPARAM dwData);
private:
	int EnumDevicePort();							// 枚举设备输出端口
	int EnumMonitor();								// 枚举已连接显示器
	void GetAllDisplayMode();		// 得到所有显示器的模式
};

#endif 