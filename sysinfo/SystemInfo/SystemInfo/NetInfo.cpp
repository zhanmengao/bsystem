#pragma comment(lib, "ws2_32.lib")
#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <windows.h>
#include<Iphlpapi.h>

using namespace std;

typedef struct tagAdapterInfo
{
	string strName; // 适配器名称
	string strDriverDesc; // 适配器描述
	string strIP; // IP地址
	string strNetMask; // 子网掩码
	string strNetGate; // 网关
	string strBroadcastIp; // 广播地址
	string strS; // 测试用的
}ADAPTER_INFO;

BOOL GetAdapterInfo();
BOOL GetLocalRouteMacAddress();
BOOL RegGetIP(ADAPTER_INFO *pAI, const char* lpszAdapterName, int nIndex = 0);

vector<ADAPTER_INFO*> AdapterInfoVector;

int main33()
{
	GetAdapterInfo();
	int i;
	for (i = 0; i < AdapterInfoVector.size(); i++)
	{
		cout << AdapterInfoVector[i]->strName << ":" << endl << endl;
		cout << " " << AdapterInfoVector[i]->strDriverDesc << endl;
		cout << " " << AdapterInfoVector[i]->strIP << endl;
		cout << " " << AdapterInfoVector[i]->strNetMask << endl;
		cout << " " << AdapterInfoVector[i]->strNetGate << endl;
		cout << " " << AdapterInfoVector[i]->strBroadcastIp << endl;
		cout << endl;

	}
	GetLocalRouteMacAddress();
	system("pause");
	return 0;
}

//—————————————————————–
// 取得所有网卡信息
//—————————————————————–
BOOL GetAdapterInfo()
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
							ADAPTER_INFO *pAI = new ADAPTER_INFO;
							pAI->strDriverDesc = (char*)szData;
							dwBufSize = 256;
							if (RegQueryValueExA(hSubKey, "NetCfgInstanceID", 0, &dwDataType, szData, &dwBufSize) == ERROR_SUCCESS)
							{
								pAI->strS = (char*)szData;
								RegGetIP(pAI, (char*)szData);
							}
							AdapterInfoVector.push_back(pAI); // 加入到容器中
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

//—————————————————————–
// 得到注册表中的IP信息
// nIndex暂时未处理
//—————————————————————–

BOOL RegGetIP(ADAPTER_INFO *pAI, const char* lpszAdapterName, int nIndex/* =0 */)
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

BOOL GetLocalRouteMacAddress()
{
	WSADATA wsaData;
	char name[255];
	PHOSTENT hostinfo;
	char *Myip = NULL;
	if (WSAStartup(MAKEWORD(2, 0), &wsaData) == 0)  //初始化
	{
		//得到当前使用网络中 注册的hostname
		if (gethostname(name, sizeof(name)) == 0)
		{
			//从hostname得到网络地址 就是正在使用的网络的IP 如果未联网 则此方法无效
			if ((hostinfo = gethostbyname(name)) != NULL)
			{
				//获得IP
				Myip = inet_ntoa(*(struct in_addr *)*hostinfo->h_addr_list);
			}
		}
		WSACleanup();
	}
	DWORD dwRetVal = 0;
	PIP_ADAPTER_INFO pAdapter = NULL;
	PIP_ADAPTER_INFO pAdapterInfo = NULL;
	pAdapterInfo = (IP_ADAPTER_INFO *)malloc(sizeof(IP_ADAPTER_INFO));
	ULONG ulOutBufLen = sizeof(IP_ADAPTER_INFO);
	if (GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW)
	{
		free(pAdapterInfo);
		pAdapterInfo = (IP_ADAPTER_INFO*)malloc(ulOutBufLen);
	}
	char szGetWayIp[64] = { 0 };
	unsigned char dwRouteMAC[6] = { 6 };
	if ((dwRetVal = GetAdaptersInfo(pAdapterInfo, &ulOutBufLen)) == NO_ERROR)
	{
		pAdapter = pAdapterInfo;
		do
		{
			if (pAdapter)
			{
				printf("%s\r\nTYPE:%d\r\nIP:%s\r\nGATEWAY:%s\r\n", pAdapter->Description, pAdapter->Type, pAdapter->IpAddressList.IpAddress.String, pAdapter->GatewayList.IpAddress.String);
				if (true || strcmp(pAdapter->IpAddressList.IpAddress.String, Myip) == 0)
				{
					printf("Now In Use!!!!!!!!!!\r\n");
					strcpy(szGetWayIp, pAdapter->GatewayList.IpAddress.String);
					ULONG uLen = 6;
					unsigned long dwGetWayIp = inet_addr(szGetWayIp);
					if (SendARP(dwGetWayIp, NULL, (PULONG)dwRouteMAC, &uLen) == NO_ERROR)
					{
						char shost[1024] = { 0 };
						char dhost[1024] = { 0 };
						//当前网卡MAC
						memcpy(shost, pAdapter->Address, 6);
						//当前网卡网关MAC
						memcpy(dhost, dwRouteMAC, 6);
						printf("My MAC:%02x:%02x:%02x:%02x:%02x:%02x\r\n", shost[0], shost[1], shost[2], shost[3], shost[4], shost[5]);
						printf("GateWay MAC:%02x:%02x:%02x:%02x:%02x:%02x\r\n", dhost[0], dhost[1], dhost[2], dhost[3], dhost[4], dhost[5]);
						printf("GateWayIP:%s\r\n", szGetWayIp);
					}
				}
				printf("\r\n\r\n");
				pAdapter = pAdapter->Next;
			}
		} while (pAdapter);
	}
	free(pAdapterInfo);
	return FALSE;
}