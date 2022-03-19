#pragma once
#include<Windows.h>
#include<map>
#include"../../../../C++space/StringTools/StringTools/StringHead.h"
#include"ProcessHelper.h"
#include"SessionManager.h"
#include"../../../SystemInfo/SystemInfo/WMI/WMIUtils.h"
class ProcessMonitBase
{
public:
	ProcessMonitBase()
	{
	}
	virtual ~ProcessMonitBase()
	{

	}
	void MonitProcess(DWORD sid)
	{
		bUpdate = true;
		while(bUpdate)
		{
			increatList.clear();
			Count(sid, increatList);
			for(auto it = increatList.begin();it!=increatList.end();it++)
			{
				mCountMap[*it]++;
			}
			Sleep(mTimer);
		}
	}
	//持续统计
	void MonitProcessLoop(DWORD sid)
	{
		bUpdate = true;
		while (bUpdate)
		{
			MonitProcessOnce(sid);
			Sleep(mTimer);
		}
	}
	void SetUpdate(bool bupdate = false)
	{
		bUpdate = bupdate;
	}
	//单次统计
	void MonitProcessOnce(DWORD session)
	{
		increatList.clear();
		Count(session, increatList);
		for (auto it = increatList.begin(); it != increatList.end(); it++)
		{
			mCountMap[*it]++;
		}
	}
	virtual void Count(DWORD session, std::vector<stdstr>& increatList)
	{
	}
	std::map<stdstr, DWORD> GetCountMap()
	{
		return mCountMap;
	}
	void SetTimer(DWORD timer)
	{
		mTimer = timer;
	}
private:
	std::map<stdstr, DWORD> mCountMap;							//计数map
	std::vector<stdstr> increatList;							//本次新增map
	bool bUpdate;																	//是否更新
	DWORD mTimer = 3000;
};