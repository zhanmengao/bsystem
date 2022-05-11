/***********************************************
�ļ���:			GetMonitorAllResolution.h
����/�޸���:		benben
����/�޸�ʱ��:	2013.01.14
����:			��ȡ������ʾ�������зֱ��ʵ����
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
			wcout << "�Ƿ�����ʾ����  " << (pos->mi_DevicePrimary ? "��" : "����") << endl;
			wcout << "�豸��ʶID                         " << pos->mi_Id << endl;
			wcout << "��ǰʹ�õķֱ��� : " << pos->m_CurUsedResolution.m_nWidth << " x " << pos->m_CurUsedResolution.m_nHeight << endl;

			wcout << "������֧�ֵķֱ���: " << endl;
			int nSupportSize = pos->m_AllSupportSize;
			for (int i = 0; i < nSupportSize; ++i)
			{
				wcout << " " << pos->m_AllSupportResolution[i].m_nWidth << " x " << pos->m_AllSupportResolution[i].m_nHeight << endl;
			}
		}

	}
	return 0;
} 
