#include"GlobalData.h"


void IDPManager::Start()
{
	Reload();
	User.Start();
}
void IDPManager::Update()
{
	Reload();
}
void IDPManager::Destroy()
{
	User.Destroy();
}
void IDPManager::Reload()
{
	Config.ReloadConfig();
}
WMIUtils& IDPManager::WMI()
{
	return wmi;
}
UserManager	IDPManager::User;
LogManager IDPManager::Log;
tm IDPManager::mLastDrivetm = { 0 };
bool IDPManager::bSendEndMessage = false;
WMIUtils IDPManager::wmi;
ConfigManager IDPManager::Config;
time_t IDPManager::mLastAIDA = 0;

