#ifndef _BIOSINFO_H
#define _BIOSINFO_H
#pragma once
#ifdef _WIN32
#include"../../SystemInfo/SystemInfo/WMI/WMIUtils.h"

vector<WMIBIOS> GetBIOSInfo()
{
	vector<WMIBIOS> vec;
	std::string exec;
	WMIUtils wmi;
	wmi.Begin();
	wmi.GetBIOS(vec, exec);
	wmi.End();
	return vec;
}
#else
#include"../vmstat/Hardware.h"
struct BIOSInfoC
{
    char biosid[128];
};

void GetBIOSInfo(struct BIOSInfoC* info)
{
    memset(info,0,sizeof(struct BIOSInfoC));
    GetBIOSID(info->biosid,sizeof(info->biosid));
}

#endif

#endif
