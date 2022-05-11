/***********************************************
文件名:			GetMonitorAllResolution.h
创建/修改者:		benben
创建/修改时间:	2013.01.14
功能:			获取所有显示器的所有分辨率的组合
***********************************************/
#include "GetAllResolutionInfo.h"
#include <iostream>
#include <vector>
using namespace std;


int _tmain(int argc, _TCHAR* argv[])
{ 
	
	//monitorList.EnumDevicePort();
	//monitorList.EnumMonitor();
	//monitorList.GetAllDisplayMode();
	CMonitorList monitorList;
	vector<st_MI> vecMonitorList = monitorList.OutputMointorInfo();
	vector<MonitorInfo> monitorvec;
	CMonitorList::GetMonitors(monitorvec);
	for (auto it = monitorvec.begin(); it != monitorvec.end(); it++)
	{
		for (int i = 0; i < vecMonitorList.size(); i++)
		{
		}
	}

	{
		vector<st_MI>::iterator pos = vecMonitorList.begin();
		vector<st_MI>::iterator itEnd = vecMonitorList.end();
		for (NULL; pos != itEnd; ++pos)
		{

			wcout << "=====================================" << endl;
			wcout << "mi_wDeviceName" << pos->mi_DeviceName << endl;
			wcout << "是否主显示器：  " << (pos->mi_DevicePrimary ? "是" : "不是") << endl;
			wcout << "设备标识ID                         " << pos->mi_Id << endl;
			wcout << "当前使用的分辨率 : " << pos->m_CurUsedResolution.m_nWidth << " x " << pos->m_CurUsedResolution.m_nHeight << endl;

			wcout << "所有能支持的分辨率: " << endl;
			int nSupportSize = pos->m_AllSupportSize;
			for (int i = 0; i < nSupportSize; ++i)
			{
				wcout << " " << pos->m_AllSupportResolution[i].m_nWidth << " x " << pos->m_AllSupportResolution[i].m_nHeight << endl;
			}
		}

	}
	return 0;
} 
