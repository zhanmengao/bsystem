#pragma once
#include"ProcessMonitBase.h"


class ProcessMonitList :public ProcessMonitBase
{
protected:
	void Count(DWORD sid, std::vector<stdstr>& increatList)
	{
		std::map<DWORD, ProcessInfo> mapProcess;
		std::map<DWORD, WTSProcessInfo> promap;
		SessionManager::GetWTSProcesses(sid, promap);
		ProcessHelper::GetProcessList(mapProcess);
		for (auto it = promap.begin(); it != promap.end(); it++)
		{
			auto globalIt = mapProcess.find(it->first);
			if (globalIt == mapProcess.end())
			{
				continue;
			}
			auto fit = mOldProcess.find(it->first);
			//�ҵ��ˣ��ȽϽ���ID��·��
			if (fit != mOldProcess.end())
			{
				//һ�£�ͬ����
				if (fit->first == it->first && fit->second.szExeFile == globalIt->second.szExeFile)
				{
					continue;
				}
			}
			increatList.push_back(globalIt->second.szExeFile);
		}
		mOldProcess = mapProcess;
	}
private:
	std::map<DWORD, ProcessInfo> mOldProcess;				//��һ�εĿ���
};