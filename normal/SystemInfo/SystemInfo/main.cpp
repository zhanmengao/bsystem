#include"SysInfoUtils.h"
#include<iostream>
#include<stdio.h>
#include"BootTime.h"
#include"Drivers.h"
#include"Printer.h"
#include"SockListen.h"
#include<vector>
using namespace std;
int _tmain(int argc,TCHAR* argv[],TCHAR* env[])
{
	STDOUT << SysInfoUtils::GetUser() << endl;
	STDOUT << SysInfoUtils::GetHostName() << endl;
	STDOUT << SysInfoUtils::GetPCName() << endl;
	STDOUT << SysInfoUtils::_GetComputerName() << endl;
	STDOUT << SysInfoUtils::GetWindowsInfo() << endl;
	STDOUT << SysInfoUtils::GetWindowsVersion() << endl;
	STDOUT << SysInfoUtils::GetProductName() << endl;
	STDOUT << SysInfoUtils::GetRegisteredOwner() << endl;
	STDOUT << SysInfoUtils::GetCPU() << endl;
	STDOUT << SysInfoUtils::GetOSName() << endl;
	STDOUT << SysInfoUtils::GetOSVersion() << endl;
	STDOUT << SysInfoUtils::GetOSReleaseID() << endl;
	STDOUT << SysInfoUtils::GetOSCurrBuild() << endl;
	STDOUT << SysInfoUtils::GetOSInstallDate() << endl;

	std::vector<TCPPort> tcplist;
	SysInfoUtils::GetTCPportlist(tcplist);

	vector<UDPPort> udplist;
	SysInfoUtils::GetUDPportlist(udplist);

	vector<EventLog> eventlist;
	SysInfoUtils::GetEventLog(eventlist);
	ReadWindowsEventLog();

	net_info net = SysInfoUtils::GetNetInfo();
	std::cout << "IP"<<net .strIp<<"	MAC"<<net.strMac<< endl;
	GetDriversList();

	GetPrinterList();

	net_info netinfo = SysInfoUtils::GetNetGateInfo();
	std::cout << "IP" << netinfo.strIp << "	MAC" << netinfo.strMac << endl;

	auto sysinfo = SysInfoUtils::GetSystemInfo();
	system("pause");
	return 0;
}