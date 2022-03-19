#include <ws2tcpip.h>
#include"SysInfoUtils.h"
#include <WbemCli.h>
#include<comutil.h>
#include"WinStrUtils.h"
#include"Convert.h"
#include"CppConvert.h"
#pragma comment(lib,"wbemuuid.lib")
#pragma comment(lib, "comsuppw.lib")
#pragma comment(lib,"ws2_32.lib")
stdstr SysInfoUtils::GetUser()
{
	TCHAR buffer[MAX_PATH] = { 0 };
	DWORD code = GetEnvironmentVariable(TEXT("USERNAME"), buffer, MAX_PATH);
	return buffer;
}
stdstr SysInfoUtils::GetHostName()
{
	TCHAR buffer[MAX_PATH] = { 0 };
	DWORD code = GetEnvironmentVariable(TEXT("USERDOMAIN"), buffer, MAX_PATH);
	return buffer;
}
stdstr SysInfoUtils::_GetComputerName()															//获取主机
{
	TCHAR buffer[MAX_PATH] = { 0 };
	DWORD dwSize = MAX_PATH;
	GetComputerName(buffer, &dwSize);
	return buffer;
}
stdstr SysInfoUtils::GetPCName()
{
	TCHAR buffer[MAX_PATH] = { 0 };
	DWORD code = GetEnvironmentVariable(TEXT("COMPUTERNAME"), buffer, MAX_PATH);
	return buffer;
}
struct WindowsInfo SysInfoUtils::GetWindowsInfo()
{
	DWORD dwVersion = 0;
	WKSTA_INFO_100 *wkstaInfo = NULL;
	WindowsInfo info;
	NET_API_STATUS netStatus = NetWkstaGetInfo(NULL, 100, (BYTE **)&wkstaInfo);
	if (netStatus == NERR_Success)
	{
		info.computername = wkstaInfo->wki100_computername;
		info.platform_id = wkstaInfo->wki100_platform_id;
		info.langroup = wkstaInfo->wki100_langroup;
		info.dwMajVer = wkstaInfo->wki100_ver_major;
		info.dwMinVer = wkstaInfo->wki100_ver_minor;
	}
	if (wkstaInfo)
	{
		NetApiBufferFree(wkstaInfo);
		wkstaInfo = NULL;
	}
	return info;
}
struct WindowsVersion SysInfoUtils::GetWindowsVersion()
{
	OSVERSIONINFO info = { 0 };
	info.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	BOOL bRet = GetVersionEx(&info);
	return info;
}
stdstr SysInfoUtils::OsProductInfo(const TCHAR* str)
{
	HKEY hKey;
	DWORD dwType = REG_SZ;
	DWORD dwSize = 0;
	TCHAR data[MAX_PATH] = { 0 };
	DWORD data11 = NULL;
	int ret;

	ret = RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("HARDWARE\\DESCRIPTION\\System\\BIOS"),
		0, KEY_QUERY_VALUE | KEY_WOW64_64KEY, &hKey);
	if (ret != ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		return _T("None");
	}
	DWORD dwValueCount = 0, maxValueNameLen = 0, maxValueDataLen = 0;
	ret = RegQueryInfoKey(hKey, NULL, NULL, NULL, NULL, NULL, NULL, &dwValueCount, &maxValueNameLen, &maxValueDataLen, NULL, NULL);
	if (ret != ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		return _T("None");
	}
	dwSize = maxValueDataLen;
	ret = RegQueryValueEx(hKey, str, NULL, &dwType, (LPBYTE)data, &dwSize);
	if (ret != ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		return _T("None");
	}
	RegCloseKey(hKey);
	return data;
}
stdstr SysInfoUtils::OsCpuInfo(const TCHAR* str)
{
	HKEY hKey;
	DWORD dwType = REG_SZ;
	DWORD dwSize = 0;
	TCHAR data[MAX_PATH] = { 0 };
	DWORD data11 = NULL;
	int ret;

	ret = RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0"), 
		0, KEY_QUERY_VALUE | KEY_WOW64_64KEY, &hKey);
	if (ret != ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		return _T("None");
	}
	DWORD dwValueCount = 0, maxValueNameLen = 0, maxValueDataLen = 0;
	ret = RegQueryInfoKey(hKey, NULL, NULL, NULL, NULL, NULL, NULL, 
		&dwValueCount, &maxValueNameLen, &maxValueDataLen, NULL, NULL);
	if (ret != ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		return _T("None");
	}
	dwSize = maxValueDataLen;
	ret = RegQueryValueEx(hKey, str, NULL, &dwType, (LPBYTE)data, &dwSize);
	if (ret != ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		return _T("None");
	}
	RegCloseKey(hKey);
	return data;
}
stdstr SysInfoUtils::OsInfo(const TCHAR* str)
{
	HKEY hKey;
	DWORD dwType = REG_SZ;
	DWORD dwSize = 0;
	TCHAR data[MAX_PATH] = { 0 };
	DWORD data11 = NULL;
	int ret;
	ret = RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion"), 
		0, KEY_QUERY_VALUE | KEY_WOW64_64KEY, &hKey);
	if (ret != ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		return _T("None");
	}

	DWORD dwValueCount = 0, maxValueNameLen = 0, maxValueDataLen = 0;
	ret = RegQueryInfoKey(hKey, NULL, NULL, NULL, NULL, NULL, NULL, &dwValueCount, &maxValueNameLen, &maxValueDataLen, NULL, NULL);
	if (ret != ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		return _T("None");
	}

	dwSize = maxValueDataLen;
	ret = RegQueryValueEx(hKey, str, NULL, &dwType, (LPBYTE)data, &dwSize);
	if (ret != ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		return _T("None");
	}
	RegCloseKey(hKey);
	return data;
}
DWORD SysInfoUtils::DWOsInfo(const TCHAR* str)
{
	HKEY hKey;
	DWORD dwType = REG_DWORD;
	DWORD dwData = 0;
	DWORD dwSize = sizeof(dwData);
	int ret;
	ret = RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion"),
		0, KEY_QUERY_VALUE | KEY_WOW64_64KEY, &hKey);
	if (ret != ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		return 0;
	}
	DWORD dwValueCount = 0, maxValueNameLen = 0, maxValueDataLen = 0;
	ret = RegQueryInfoKey(hKey, NULL, NULL, NULL, NULL, NULL, NULL, &dwValueCount, 
		&maxValueNameLen, &maxValueDataLen, NULL, NULL);
	if (ret != ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		return 0;
	}
	dwSize = maxValueDataLen;
	ret = RegQueryValueEx(hKey, str, NULL, &dwType, (BYTE*)&dwData, &dwSize);
	if (ret != ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		return 0;
	}
	return dwData;
}


ULONG SysInfoUtils::GetHostOrder(const std::string& strIp)
{
	//char buf[64] = { 0 };
	//inet_pton(AF_INET, strIp.c_str(), buf);
	ULONG iIp = inet_addr(strIp.c_str());
	iIp = ntohl(iIp);
	return iIp;
}
net_info SysInfoUtils::GetNetInfo()
{
	net_info  result;
	ULONG buffer_size = 0;
	GetAdaptersInfo(NULL, &buffer_size);
	PIP_ADAPTER_INFO p_ip_info = (IP_ADAPTER_INFO *)malloc(buffer_size);
	GetAdaptersInfo(p_ip_info, &buffer_size);
	while (p_ip_info)
	{
		ULONG gate_way = GetHostOrder(p_ip_info->GatewayList.IpAddress.String);
		IP_ADDR_STRING* p_ip_item = &p_ip_info->IpAddressList;
		bool is_right_ip = false;
		while (p_ip_item)
		{
			ULONG tpIp = GetHostOrder(p_ip_item->IpAddress.String);
			result.strIp = p_ip_item->IpAddress.String;

			ULONG net_mask = GetHostOrder(p_ip_item->IpMask.String);
			ULONG sub_net = gate_way & net_mask;
			ULONG sub_net_x = tpIp & net_mask;
			if (tpIp != 0)
			{
				if (sub_net == sub_net_x)
				{
					is_right_ip = true;
					break;
				}
			}
			p_ip_item = p_ip_item->Next;
		}

		if (is_right_ip)
		{
			UCHAR mac[MAX_PATH] = { 0 };
			int iCount = 0;

			for (int i = 0; i<(int)p_ip_info->AddressLength; i++)
			{
				Byte2Hex(p_ip_info->Address[i], &mac[iCount]);
				iCount += 2;
				if (i<(int)p_ip_info->AddressLength - 1)
				{
					mac[iCount++] = ':';
				}
			}
			result.strMac = (char*)mac;
			break;
		}
		p_ip_info = p_ip_info->Next;
	}
	if (p_ip_info)
	{
		free(p_ip_info);
		p_ip_info = NULL;
	}

	return result;
}
BOOL SysInfoUtils::RegGetIP(AdapterInfo *pAI, const char* lpszAdapterName, int nIndex/* =0 */)
{
	//ASSERT(pAI);
	HKEY hKey, hSubKey, hNdiIntKey;

	string strKeyName = "SYSTEM\\CurrentControlSet\\Services\\Tcpip\\Parameters\\Interfaces\\";
	strKeyName += lpszAdapterName;
	if (RegOpenKeyExA(HKEY_LOCAL_MACHINE,
		strKeyName.c_str(),
		0,
		KEY_READ,
		&hKey) != ERROR_SUCCESS)
		return FALSE;

	unsigned char szData[256];
	DWORD dwDataType, dwBufSize;
	dwBufSize = 256;
	if (RegQueryValueExA(hKey, "DhcpIPAddress", 0, &dwDataType, szData, &dwBufSize) == ERROR_SUCCESS)
		pAI->strIP = (const char*)szData;
	else
	{
		if (RegQueryValueExA(hKey, "IPAddress", 0, &dwDataType, szData, &dwBufSize) == ERROR_SUCCESS)
			pAI->strIP = (const char*)szData;
	}



	dwBufSize = 256;
	if (RegQueryValueExA(hKey, "DhcpSubnetMask", 0, &dwDataType, szData, &dwBufSize) == ERROR_SUCCESS)
		pAI->strNetMask = (const char*)szData;
	else
	{
		if (RegQueryValueExA(hKey, "SubnetMask", 0, &dwDataType, szData, &dwBufSize) == ERROR_SUCCESS)
			pAI->strNetMask = (const char*)szData;
	}


	dwBufSize = 256;
	if (RegQueryValueExA(hKey, "DhcpDefaultGateway", 0, &dwDataType, szData, &dwBufSize) == ERROR_SUCCESS)
		pAI->strNetGate = (const char*)szData;
	else
	{
		if (RegQueryValueExA(hKey, "DefaultGateway", 0, &dwDataType, szData, &dwBufSize) == ERROR_SUCCESS)
			pAI->strNetGate = (const char*)szData;
	}


	RegCloseKey(hKey);

	strKeyName = "SYSTEM\\ControlSet001\\Control\\Network";
	if (RegOpenKeyExA(HKEY_LOCAL_MACHINE,
		strKeyName.c_str(),
		0,
		KEY_READ,
		&hKey) != ERROR_SUCCESS)
		return FALSE;
	char szSubKey[256];
	char szSubKey_two[256];
	memset(szSubKey, 0, 256);
	DWORD dwIndex = 0;
	DWORD dwIndex_two = 0;
	dwBufSize = 256;
	DWORD dwBufSize_two = 256;
	pAI->strName = pAI->strDriverDesc;
	while (RegEnumKeyA(hKey, dwIndex++, szSubKey, dwBufSize) == ERROR_SUCCESS)
	{
		string strKeyName_two;
		strKeyName_two = strKeyName + "\\";
		strKeyName_two += szSubKey;
		if (RegOpenKeyExA(hKey, szSubKey, 0, KEY_READ, &hSubKey) == ERROR_SUCCESS)
		{
			while (RegEnumKeyA(hSubKey, dwIndex_two++, szSubKey_two, dwBufSize_two) == ERROR_SUCCESS)
			{
				if (strstr(szSubKey_two, lpszAdapterName) != NULL)
				{
					strcat(szSubKey_two, "\\Connection");
					if (RegOpenKeyExA(hSubKey, szSubKey_two, 0, KEY_READ, &hNdiIntKey) == ERROR_SUCCESS)
					{
						if (RegQueryValueExA(hNdiIntKey, "Name", 0, &dwDataType, szData, &dwBufSize) == ERROR_SUCCESS)
						{
							pAI->strName = (char*)szData;
							break;
						}

						RegCloseKey(hNdiIntKey);
					}

				}
			}
			RegCloseKey(hSubKey);
		}
	}
	RegCloseKey(hKey);

	/*
	算法：
	1. 子网掩码与IP地址进行位与运算，得处网络地址
	2. 网络地址 | (~子网掩码)，得出广播地址
	*/
	in_addr broadcast;
	broadcast.S_un.S_addr = (
		inet_addr(pAI->strIP.c_str())
		& inet_addr(pAI->strNetMask.c_str())
		)
		| (~inet_addr(pAI->strNetMask.c_str()));
	pAI->strBroadcastIp = inet_ntoa(broadcast);
	return TRUE;
}
BOOL SysInfoUtils::GetAdapterInfo(vector<AdapterInfo>& vec)						//获取网卡信息
{
	// 这里的代码适合WINDOWS2000，对于NT需要读取HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows NT\CurrentVersion\NetworkCards
	HKEY hKey, hSubKey, hNdiIntKey;

	if (RegOpenKeyExA(HKEY_LOCAL_MACHINE,
		"System\\CurrentControlSet\\Control\\Class\\{4d36e972-e325-11ce-bfc1-08002be10318}",
		0,
		KEY_READ,
		&hKey) != ERROR_SUCCESS)
		return FALSE;

	DWORD dwIndex = 0;
	DWORD dwBufSize = 256;
	DWORD dwDataType;
	char szSubKey[256];
	unsigned char szData[256];

	while (RegEnumKeyExA(hKey, dwIndex++, szSubKey, &dwBufSize, NULL, NULL, NULL, NULL) == ERROR_SUCCESS)
	{
		if (RegOpenKeyExA(hKey, szSubKey, 0, KEY_READ, &hSubKey) == ERROR_SUCCESS)
		{
			if (RegOpenKeyExA(hSubKey, "Ndi\\Interfaces", 0, KEY_READ, &hNdiIntKey) == ERROR_SUCCESS)
			{
				dwBufSize = 256;
				if (RegQueryValueExA(hNdiIntKey, "LowerRange", 0, &dwDataType, szData, &dwBufSize) == ERROR_SUCCESS)
				{
					if (strstr((char*)szData, "ethernet") != NULL)// 判断是不是以太网卡
					{
						dwBufSize = 256;
						if (RegQueryValueExA(hSubKey, "DriverDesc", 0, &dwDataType, szData, &dwBufSize) == ERROR_SUCCESS)
						{
							AdapterInfo ai;
							ai.strDriverDesc = (char*)szData;
							dwBufSize = 256;
							if (RegQueryValueExA(hSubKey, "NetCfgInstanceID", 0, &dwDataType, szData, &dwBufSize) == ERROR_SUCCESS)
							{
								ai.strS = (char*)szData;
								RegGetIP(&ai, (char*)szData);
							}
							vec.push_back(ai); // 加入到容器中
						}
					}
				}
				RegCloseKey(hNdiIntKey);
			}
			RegCloseKey(hSubKey);
		}

		dwBufSize = 256;
	} /* end of while */

	RegCloseKey(hKey);
	return true;
}
std::string SysInfoUtils::GetMac(const char* ipaddr)
{
	unsigned char dwRouteMAC[6] = { 6 };
	ULONG uLen = 6;
	unsigned long dwGetWayIp = inet_addr(ipaddr);
	if (SendARP(dwGetWayIp, NULL, (PULONG)dwRouteMAC, &uLen) == NO_ERROR)
	{
		char dhost[1024] = { 0 };
		//当前网卡网关MAC
		memcpy(dhost, dwRouteMAC, 6);
		sprintf(dhost,"%02X:%02X:%02X:%02X:%02X:%02X",
			dwRouteMAC[0], dwRouteMAC[1], dwRouteMAC[2], dwRouteMAC[3], dwRouteMAC[4], dwRouteMAC[5]);
		return dhost;
	}
	return "";
}
net_info SysInfoUtils::GetNetGateInfo()
{
	net_info info;
	vector<AdapterInfo> vec;
	SysInfoUtils::GetAdapterInfo(vec);
	for (auto it = vec.begin(); it != vec.end(); it++)
	{
		if (!it->strNetGate.empty())
		{
			info.strIp = it->strNetGate;
			info.strMac = SysInfoUtils::GetMac(it->strNetGate.c_str());
			return info;
		}
	}
	return info;
}
int SysInfoUtils::GetTCPportlist(std::vector<TCPPort>& vec)
{
	int iErrno;
	PMIB_TCPTABLE_OWNER_PID pMibTcpTableOwnerPid;
	DWORD dwSize = 0;
	int i;
	HMODULE hModule;

	if ((iErrno = GetExtendedTcpTable(NULL, &dwSize, TRUE, AF_INET, TCP_TABLE_OWNER_PID_ALL, 0)) != NO_ERROR)
	{
		if (iErrno != ERROR_INSUFFICIENT_BUFFER)
		{
			printf("GetExtendedTcpTable Error: %d\n", iErrno);
			return iErrno;
		}
	}
	pMibTcpTableOwnerPid = (PMIB_TCPTABLE_OWNER_PID)malloc(dwSize);
	if (pMibTcpTableOwnerPid == NULL)
	{
		return FALSE;
	}
	if ((iErrno = GetExtendedTcpTable(pMibTcpTableOwnerPid, &dwSize, TRUE, AF_INET, TCP_TABLE_OWNER_PID_ALL, 0)) != NO_ERROR)
	{
		printf("GetExtendedTcpTable Error: %d\n", iErrno);
		return iErrno;
	}

	for (i = 0; i < (int)pMibTcpTableOwnerPid->dwNumEntries; i++)
	{
		TCPPort info;
		IN_ADDR localAddr;
		IN_ADDR remoteAddr;

		auto& data = pMibTcpTableOwnerPid->table[i];
		localAddr.S_un.S_addr = data.dwLocalAddr;
		remoteAddr.S_un.S_addr = data.dwRemoteAddr;

		inet_ntop(AF_INET, &localAddr, info.szLocalAddr,_countof(info.szLocalAddr));
		inet_ntop(AF_INET, &remoteAddr, info.szRemoteAddr, _countof(info.szRemoteAddr));
		//sprintf_s(info.szLocalAddr,_countof(info.szLocalAddr), "%s", inet_ntoa(localAddr));
		//sprintf_s(info.szRemoteAddr, _countof(info.szRemoteAddr), "%s", inet_ntoa(remoteAddr));

		info.dwState = data.dwState;

		info.dwRemotePort = ntohs((USHORT)data.dwRemotePort);
		info.dwLocalPort = ntohs((USHORT)data.dwLocalPort);
		info.dwOwningPid = data.dwOwningPid;
		vec.push_back(info);
	}

	free(pMibTcpTableOwnerPid);

	return 0;
}

int SysInfoUtils::GetUDPportlist(std::vector<UDPPort>& vec)
{
	int iErrno;
	PMIB_UDPTABLE_OWNER_PID pMibUdpTableOwnerPid;
	DWORD dwSize = 0;
	int i;
	HMODULE hModule;

	if ((iErrno = GetExtendedUdpTable(NULL, &dwSize, TRUE, AF_INET, UDP_TABLE_OWNER_PID, 0)) != NO_ERROR)
	{
		if (iErrno != ERROR_INSUFFICIENT_BUFFER)
		{
			printf("GetExtendedUdpTable Error: %d\n", iErrno);
			return FALSE;
		}
	}
	pMibUdpTableOwnerPid = (PMIB_UDPTABLE_OWNER_PID)malloc(dwSize);
	if (pMibUdpTableOwnerPid == NULL)
	{
		return -1;
	}
	if ((iErrno = GetExtendedUdpTable(pMibUdpTableOwnerPid, &dwSize, TRUE, AF_INET, UDP_TABLE_OWNER_PID, 0)) != NO_ERROR)
	{
		printf("GetExtendedUdpTable Error: %d\n", iErrno);
		return iErrno;
	}

	for (i = 0; i < (int)pMibUdpTableOwnerPid->dwNumEntries; i++)
	{
		UDPPort info;
		IN_ADDR localAddr;

		auto& rData = pMibUdpTableOwnerPid->table[i];
		localAddr.S_un.S_addr = rData.dwLocalAddr;
		inet_ntop(AF_INET, &localAddr, info.szLocalAddr, _countof(info.szLocalAddr));
		//sprintf_s(info.szLocalAddr,_countof(info.szLocalAddr), "%s", inet_ntoa(localAddr));

		info.dwLocalPort = ntohs((USHORT)rData.dwLocalPort);
		info.dwOwningPid = rData.dwOwningPid;
		vec.push_back(info);
	}
	free(pMibUdpTableOwnerPid);
	return 0;
}

int SysInfoUtils::GetEventLog(std::vector<EventLog>& vec)
{
	HANDLE h;
	EVENTLOGRECORD* pevlr;
	TCHAR bBuffer[4096] = { 0 };

	DWORD dwRead, dwNeeded, cRecords, dwThisRecord = 0;
	h = OpenEventLog(NULL, _T("System"));
	if (h == NULL)
	{
		printf("Could not open the System event log.");

		return GetLastError();
	}

	pevlr = (EVENTLOGRECORD*)&bBuffer;

	int count = 0;

	while (ReadEventLog(h,
		EVENTLOG_SEQUENTIAL_READ |
		EVENTLOG_BACKWARDS_READ,
		0,
		pevlr,
		4096,
		&dwRead,
		&dwNeeded))
	{
		bool find = false;

		while (dwRead > 0)
		{
			vec.push_back(*pevlr);

			dwRead -= pevlr->Length;
			pevlr = (EVENTLOGRECORD*)((LPBYTE)pevlr + pevlr->Length);							//后移
		}
		pevlr = (EVENTLOGRECORD*)&bBuffer;
	}
	CloseEventLog(h);
	return 0;
}

 SYSTEM_INFO SysInfoUtils::GetSystemInfo()
{
	 SYSTEM_INFO info;
	 ::GetSystemInfo(&info);
	 return info;
}