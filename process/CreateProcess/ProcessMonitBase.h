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
	//����ͳ��
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
	//����ͳ��
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
	std::map<stdstr, DWORD> mCountMap;							//����map
	std::vector<stdstr> increatList;							//��������map
	bool bUpdate;																	//�Ƿ����
	DWORD mTimer = 3000;
};