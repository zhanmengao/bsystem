#pragma once
#include<Windows.h>
#include<string>
LONG GetSysInfo()
{
	HKEY  hKey;
	LONG  nRet = 0;
	do
	{
		//打开注册表
		nRet = RegOpenKeyEx(HKEY_LOCAL_MACHINE,
			L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion",
			0,
			KEY_ALL_ACCESS,
			&hKey);
		if (nRet != ERROR_SUCCESS)
			break;
		//获取操作系统名称  “Windows 7 Ultimate”

		DWORD type;
		WCHAR szProductName[100] = { 0 };
		DWORD dwSize = 100;
		nRet = RegQueryValueExW(hKey, L"ProductName", NULL, &type, (BYTE *)szProductName, &dwSize);
		if (nRet != ERROR_SUCCESS)
			break;

		//获取CurrentVersion  “6.1”
		WCHAR szCurrentVersion[100] = { 0 };
		dwSize = 100;
		nRet = RegQueryValueExW(hKey, L"CurrentVersion", NULL, &type, (BYTE *)szCurrentVersion, &dwSize);
		if (nRet != ERROR_SUCCESS)
			break;
	} while (false);
	if (hKey != NULL)
	{
		RegCloseKey(hKey);
	}
	return nRet;
}

std::string GetDhcpIPAddress()
{

	std::string NetCardsName = "";

	long ret = 0;
	HKEY hkey;
	ret = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\NetworkCards", 0, KEY_READ, &hkey);
	if (ret == ERROR_SUCCESS)
	{

		CHAR subkey_name[MAX_PATH] = { 0 };
		unsigned long subkey_len = sizeof(subkey_name);
		int i = 0;
		subkey_len = sizeof(subkey_name);
		ret = RegEnumKeyExA(hkey, i, subkey_name, &subkey_len, 0, NULL, NULL, NULL);

		std::string tmpSubKey(subkey_name);
		tmpSubKey = "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\NetworkCards\\" + tmpSubKey;

		HKEY subhKey;
		RegOpenKeyA(HKEY_LOCAL_MACHINE, tmpSubKey.c_str(), &subhKey);

		DWORD dwType;
		DWORD vsize = MAX_PATH;
		char  stVal[MAX_PATH] = { 0 };
		memset(stVal, '\0', sizeof(stVal) * sizeof(char));
		ret = RegQueryValueExA(subhKey, "ServiceName", NULL, &dwType, (BYTE*)stVal, &vsize);

		printf("ServiceName: %s\n", stVal);

		NetCardsName = stVal;

		RegCloseKey(subhKey);

	}

	RegCloseKey(hkey);


	HKEY hKey;
	DWORD dwDisp;
	DWORD dwType;

	std::string DhcpIPAddress = "";

	NetCardsName = "SYSTEM\\CurrentControlSet\\services\\Tcpip\\Parameters\\Interfaces\\" + NetCardsName;

	ret = RegCreateKeyExA(
		HKEY_LOCAL_MACHINE,
		NetCardsName.c_str(),
		0,
		NULL,
		REG_OPTION_NON_VOLATILE,
		KEY_ALL_ACCESS | KEY_WOW64_64KEY,
		NULL,
		&hKey,
		&dwDisp);



	if (ret == ERROR_SUCCESS)
	{

		DWORD   dtVal = 0;
		char    stVal[MAX_PATH] = { 0 };
		memset(stVal, '\0', sizeof(stVal) * sizeof(char));

		DWORD   vsize = MAX_PATH;
		ret = RegQueryValueExA(hKey, "DhcpIPAddress", NULL, &dwType, (BYTE*)stVal, &vsize);

		printf("DhcpIPAddress: %s\n", stVal);

		DhcpIPAddress = stVal;
	}

	RegCloseKey(hKey);


	return DhcpIPAddress;

}