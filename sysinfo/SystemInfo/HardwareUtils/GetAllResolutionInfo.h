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
// ��ʾ��ģʽ��Ϣ
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
	stdstr   mi_DeviceName;		//�豸����
	stdstr   mi_DeviceString;	//��Ƶ����豸��
	stdstr	mi_DeviceKey;
	stdstr   mi_DeviceID;
	BOOL   mi_DeviceActive;			//�ӿڼ���״̬
	BOOL   mi_DevicePrimary;		//�Ƿ�����ʾ��
	DWORD  mi_StateFlags;			//״ֵ̬
	RECT   mi_rcWork;				//��Ļ��ʾ����
	DWORD  mi_dmPelsWidth;			//��Ļ��ʾ���
	DWORD  mi_dmPelsHeight;			//��Ļ��ʾ�߶�
	DWORD  mi_dmDisplayFrequency;	//��Ļ��ʾƵ��
	int    mi_nColorDepth;			//��Ļ��ʾɫ��
	// benben �����ֶ�
	int    mi_Id;                   //�豸��ʶID
	MonitorModeInfo m_CurUsedResolution;  // ��ǰʹ�õķֱ���
	int   m_AllSupportSize;          // ֧�ֵ���ʾ���ֱ��ʵĸ���
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
	static BOOL GetMonitors(vector<MonitorInfo> &vec);								//��ȡ��ʾ����Ϣ
public:
	CMonitorList();
	~CMonitorList();
	long setDisplayPosition(LPCTSTR lpszDeviceName, long x, long y);		// ������ʾ��λ��	
	long setDisplayRatio(LPCTSTR lpszDeviceName, int width, int height);		// �޸�ָ���豸�ֱ���	
	void setGlobalDisplayRatio(int width, int height);		// ���÷�����ʾ���ֱ��ʣ�ע�⣺�Ƿ�����ʾ����ƴ������	
	vector<st_MI> OutputMointorInfo();				// �����ʾ����Ϣ	
private:
	vector<st_MI> m_miGroup;		//��ʾ����Ϣ
	static vector<HMONITOR> hMonitorGroup;
private:
	int ExactNumicFromString(const TCHAR* ch);
public:
	static int CALLBACK MonitorEnumProc(HMONITOR hMonitor, 
		HDC hdc,
		LPRECT lpRMonitor,
		LPARAM dwData);
private:
	int EnumDevicePort();							// ö���豸����˿�
	int EnumMonitor();								// ö����������ʾ��
	void GetAllDisplayMode();		// �õ�������ʾ����ģʽ
};

#endif 