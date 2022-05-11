#pragma once

#include <afxtempl.h>

class GetSysInfo
{
public:
	GetSysInfo(void);
	~GetSysInfo(void);

public:
	/********��ȡ����ϵͳ�汾��Service pack�汾��ϵͳ����************/
	void GetOSVersion(CString &strOSVersion, CString &strServiceVersion);
	BOOL IsWow64();//�ж��Ƿ�Ϊ64λ����ϵͳ

	/***********��ȡ������Ŀ������***********/
	int  GetInterFaceCount();
	void GetInterFaceName(CString &InterfaceName, int pNum);

	/***��ȡ�����ڴ�������ڴ��С***/
	void GetMemoryInfo(CString &dwTotalPhys, CString &dwTotalVirtual);

	/****��ȡCPU���ơ��ں���Ŀ����Ƶ*******/
	void GetCpuInfo(CString &chProcessorName, CString &chProcessorType, DWORD &dwNum, DWORD &dwMaxClockSpeed);

	/****��ȡӲ����Ϣ****/
	void GetDiskInfo(DWORD &dwNum, CString chDriveInfo[]);

	/****��ȡ�Կ���Ϣ*****/
	void GetDisplayCardInfo(DWORD &dwNum, CString chCardName[]);
private:
	CStringList Interfaces;		                  //������������������
	CList <DWORD, DWORD&>		Bandwidths;	  //�������Ĵ���
	CList <DWORD, DWORD&>		TotalTraffics;    //��������������
};
