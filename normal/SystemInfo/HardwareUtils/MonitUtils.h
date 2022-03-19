#pragma once
#include"EDID.h"
#include"GetAllResolutionInfo.h"
#include<vector>
class MonitInfo
{
public:
	st_MI monitor;
	MonitorInfo edid;
public:
};
class MonitUtils
{
public:
	static bool GetMonitInfo(std::vector<MonitInfo> &vec)
	{
		CMonitorList monitorList;
		vector<st_MI> vecMonitorList = monitorList.OutputMointorInfo();
		vector<MonitorInfo> monitorvec;
		CMonitorList::GetMonitors(monitorvec);

		//×éºÏ
		for (auto it = monitorvec.begin(); it != monitorvec.end(); it++)
		{
			for (int i = 0; i < vecMonitorList.size(); i++)
			{
				if (it->index + 1 == vecMonitorList[i].mi_Id)
				{
					MonitInfo info;
					info.edid = *it;
					info.monitor = vecMonitorList[i];
					vec.push_back(info);
					break;
				}
			}
		}
		return true;
	}
};