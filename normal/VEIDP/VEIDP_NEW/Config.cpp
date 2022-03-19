#include "stdafx.h"
#include"Config.h"
#include"../../SystemInfo/SystemInfo/SysInfoUtils.h"
void Config::ReloadConfig()
{
	LOG_DAILY("ReloadConfig Begin");
	stdstr config = PathHelper::GetModuleFolder() + _T("config.ini");
	mConfig.SetPath(config);
	IP = mConfig.ReadString(_T("commonInfo"), _T("ip"));
	Topic = mConfig.ReadString(_T("commonInfo"), _T("Topic"));

	wanProduceIp = mConfig.ReadString(_T("waninfo"), _T("ProduceIp"));
	wanProduceTopic = mConfig.ReadString(_T("waninfo"), _T("ProduceTopic"));

	InstructIp = mConfig.ReadString(_T("instructInfo"), _T("ip"));
	InstructTopic = mConfig.ReadString(_T("instructInfo"), _T("Topic"));

	wanConsuIp = mConfig.ReadString(_T("waninfo"), _T("ConsuIp"));
	wanConsuTopic = mConfig.ReadString(_T("waninfo"), _T("ConsuTopic"));

	pantTime = mConfig.ReadNum(_T("pantTime"), _T("time"), 5);
	organization = mConfig.ReadString(_T("organization"), _T("name"));

	stdstr veidpconfig = PathHelper::GetModuleFolder() + _T("veidp.ini");
	mConfig.SetPath(veidpconfig);
	bStartAida = mConfig.ReadNum(_T("aida"), _T("begin"), 1);
	aidaPath = mConfig.ReadString(_T("aida"), _T("Path"));
	endTick = mConfig.ReadNum(_T("end"), _T("tick"), 1);

	net_info	netInfo = SysInfoUtils::GetNetInfo();
	Group = StrUtils::ReplaceAllA(netInfo.strMac, ':', '-');
	Mac = netInfo.strMac;
	wanGroup = StrUtils::ReplaceAllA(netInfo.strMac, ':', '_');
	LOG_DAILY("ReloadConfig End");
}

void Config::SaveVersion(const std::string& version, const std::string& url, const std::string& md5,int updateType)
{
	stdstr veidpconfig = PathHelper::GetModuleFolder() + _T("veidp.ini");
	mConfig.SetPath(veidpconfig);
	mConfig.WriteString(_T("version"), _T("newversion"), version);
	mConfig.WriteString(_T("version"), _T("url"), url);
	mConfig.WriteString(_T("version"), _T("md5"), md5);
	mConfig.WriteNum(_T("version"), _T("type"), updateType);
}

void Config::SaveOrgcode(const std::string& orgcode)
{
	stdstr veidpconfig = PathHelper::GetModuleFolder() + _T("config.ini");
	mConfig.SetPath(veidpconfig);
	mConfig.WriteString(_T("organization"), _T("name"), orgcode);
}