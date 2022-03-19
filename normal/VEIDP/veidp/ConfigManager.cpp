#include "stdafx.h"
#include"ConfigManager.h"
#include"veidp.h"

void ConfigManager::ReloadConfig()
{
	LOG_DAILY("ReloadConfig");
	static stdstr config = PathHelper::GetModuleFolder() + _T("config.ini");
	mConfig.SetPath(config);
	IP = mConfig.ReadString(_T("commonInfo"), _T("ip"));
	Topic = mConfig.ReadString(_T("commonInfo"), _T("Topic"));
	InstructIp = mConfig.ReadString(_T("instructInfo"), _T("ip"));
	InstructTopic = mConfig.ReadString(_T("instructInfo"), _T("Topic"));
	pantTime = mConfig.ReadNum(_T("pantTime"), _T("time"), 5);
	organization = mConfig.ReadString(_T("organization"), _T("name"));

	static stdstr aidaconfig = PathHelper::GetModuleFolder() + _T("veidp.ini");
	mConfig.SetPath(aidaconfig);
	bStartAida = mConfig.ReadNum(_T("aida"), _T("count"), 15);
	aidaTick = mConfig.ReadNum(_T("aida"), _T("tick"), 59);
	aidaSpac = mConfig.ReadNum(_T("aida"), _T("AidaSpac"), 10);
	endTick = mConfig.ReadNum(_T("end"), _T("tick"), 1);
	net_info	netInfo = SysInfoUtils::GetNetInfo();
	Group = netInfo.strMac;
	Mac = netInfo.strMac;
}