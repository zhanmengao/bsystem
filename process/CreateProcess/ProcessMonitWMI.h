#pragma once
#include"ProcessMonitBase.h"
#include"../../../SystemInfo/SystemInfo/WMI/WMIUtils.h"
#include"../../../SystemInfo/SystemInfo/WMI/WMI.h"



class ProcessMonitWMI :public ProcessMonitBase
{
public:
	ProcessMonitWMI(WMIUtils* pWmi) :mWMI(pWmi)
	{}
	virtual ~ProcessMonitWMI()
	{}
	void Count(DWORD sid, std::vector<stdstr>& increatList)
	{
		try
		{
			std::map<DWORD, WMIProcess> promap;
			std::string exce;
			if (mWMI->Begin() && mWMI->GetProcess(promap, exce))
			{
				mWMI->End();
				for (auto it = promap.begin(); it != promap.end(); it++)
				{
					if (it->second.SessionId != sid)
					{
						continue;
					}
					auto fit = mOldProcess.find(it->first);
					//找到了，比较进程ID和路径
					if (fit != mOldProcess.end())
					{
						//一致，同进程
						if (fit->first == it->first &&
							fit->second.ExecutablePath == it->second.ExecutablePath &&
							fit->second.Name == it->second.Name)
						{
							continue;
						}
					}
					stdstr path = it->second.ExecutablePath.empty() ? it->second.Name : it->second.ExecutablePath;
					increatList.push_back(path);
				}
				mOldProcess = promap;
			}
			else
			{
			}	
		}
		catch (exception& e)
		{

		}
	}
private:
	WMIUtils* mWMI;
	std::map<DWORD, WMIProcess> mOldProcess;				//上一次的快照
};