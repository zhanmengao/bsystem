#include"RemoteApp.h"
#include<tchar.h>
#define REMOTE_PATH L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Terminal Server\\TSAppAllowList\\Applications"
#include"../../../Setting/SysSetting/Regedit/RegeditUtils.h"
LSTATUS RemoteAppUtils::GetRemoteAppList(vector<RemoteApp>& appList)
{
	appList.clear();
	HKEY hKey = NULL;
	LSTATUS ret = ERROR_SUCCESS;
	TCHAR szNameValue[MAXBYTE] = { 0 };
	DWORD szNameSize = MAXBYTE;
	do
	{
		ret = RegOpenKeyEx(HKEY_LOCAL_MACHINE, REMOTE_PATH, 0, KEY_ALL_ACCESS, &hKey);
		if (ret != ERROR_SUCCESS)
		{
			break;
		}
		unsigned i = 0;
		while (1)
		{
			//枚举
			ret = RegEnumKey(hKey, i, szNameValue, szNameSize);
			if (ret != ERROR_SUCCESS)
			{
				break;
			}
			HKEY hSubKey;
			//打开每个key
			if (RegOpenKeyEx(hKey, szNameValue, NULL, KEY_READ, &hSubKey) == ERROR_SUCCESS)
			{
				RemoteApp info;
				info.RegName = szNameValue;

				TCHAR szKeyValue[MAXBYTE] = { 0 };
				DWORD dwSize = MAXBYTE;											//Key数组大小
				DWORD dwLongValue = 0;

				RegeditUtils::GetVal(hSubKey, _T(NAME_KEY), szKeyValue, MAXBYTE, dwSize);
				info.ShowName = szKeyValue;

				RegeditUtils::GetVal(hSubKey, _T(PATH_KEY), szKeyValue, MAXBYTE, dwSize);
				info.Path = szKeyValue;

				RegeditUtils::GetVal(hSubKey, _T(ICON_IDX_KEY), dwLongValue);
				info.IconIndex = dwLongValue;

				RegeditUtils::GetVal(hSubKey, _T(ICON_PATH_KEY), szKeyValue, MAXBYTE, dwSize);
				info.IconPath = szKeyValue;

				RegeditUtils::GetVal(hSubKey, _T(COMM_LINE_SET_KEY), dwLongValue);
				info.CommandLineSetting = dwLongValue;

				if (dwLongValue)
				{
					RegeditUtils::GetVal(hSubKey, _T(REQ_COMM_LINE_KEY), szKeyValue, MAXBYTE, dwSize);
					info.RequiredCommandLine = szKeyValue;
				}

				RegeditUtils::GetVal(hSubKey, _T(SHOW_TSWA_KEY), dwLongValue);
				info.ShowInTSWA = (long)dwLongValue;

				RegeditUtils::GetVal(hSubKey, _T(VPATH_KEY), szKeyValue, MAXBYTE, dwSize);
				info.VPath = szKeyValue;

				RegeditUtils::GetVal(hSubKey, _T(SECU_DESC_KEY), szKeyValue, MAXBYTE, dwSize);
				info.SecurityDescriptor = szKeyValue;

				appList.push_back(info);
			}
			if (hSubKey != NULL)
			{
				RegCloseKey(hSubKey);
				hSubKey = NULL;
			}
			i++;
		}
	} while (0);
	if (hKey != NULL)
	{
		RegCloseKey(hKey);
		hKey = NULL;
	}
	return ret;
}

LSTATUS RemoteAppUtils::AppendRemoteApp(const RemoteApp& app)
{
	//新增
	HKEY hSubKey = NULL;
	HKEY hKey = NULL;
	LSTATUS ret = 0;
	do
	{
		ret = RegOpenKeyEx(HKEY_LOCAL_MACHINE, REMOTE_PATH, 0, KEY_ALL_ACCESS, &hKey);
		if (ret != ERROR_SUCCESS)
		{
			break;
		}
		ret = RegCreateKeyEx(hKey, app.RegName.c_str(), NULL, NULL, REG_OPTION_NON_VOLATILE,
			KEY_ALL_ACCESS, NULL, &hSubKey, NULL);
		if (ret != ERROR_SUCCESS)
		{
			break;
		}
		//key里写值
		RegeditUtils::SetVal(hSubKey, _T(NAME_KEY), app.ShowName.c_str());
		RegeditUtils::SetVal(hSubKey, _T(PATH_KEY), app.Path.c_str());
		RegeditUtils::SetVal(hSubKey, _T(ICON_IDX_KEY), app.IconIndex);
		RegeditUtils::SetVal(hSubKey, _T(ICON_PATH_KEY), app.IconPath.c_str());
		RegeditUtils::SetVal(hSubKey, _T(COMM_LINE_SET_KEY), app.CommandLineSetting);
		RegeditUtils::SetVal(hSubKey, _T(REQ_COMM_LINE_KEY), app.RequiredCommandLine.c_str());
		RegeditUtils::SetVal(hSubKey, _T(SHOW_TSWA_KEY), app.ShowInTSWA);
		RegeditUtils::SetVal(hSubKey, _T(VPATH_KEY), app.VPath.c_str());
		RegeditUtils::SetVal(hSubKey, _T(SECU_DESC_KEY), app.SecurityDescriptor.c_str());
	} while (0);
	if (hSubKey != NULL)
	{
		RegCloseKey(hSubKey);
		hSubKey = NULL;
	}
	if (hKey != NULL)
	{
		RegCloseKey(hKey);
		hKey = NULL;
	}
	return ret;
}
LSTATUS RemoteAppUtils::DeleteRemoteApp(const TCHAR* regName)
{
	LSTATUS ret = 0;
	HKEY hKey = NULL;
	do
	{
		ret = RegOpenKeyEx(HKEY_LOCAL_MACHINE, REMOTE_PATH, 0, KEY_ALL_ACCESS, &hKey);
		if (ret != ERROR_SUCCESS)
		{
			break;
		}
		ret = RegDeleteKey(hKey, regName);
	} while (0);
	if (hKey != NULL)
	{
		RegCloseKey(hKey);
		hKey = NULL;
	}
	return ret;
}
LSTATUS RemoteAppUtils::SetRemoteAppVal(const TCHAR* regName, const TCHAR* key, const TCHAR* val)
{
	HKEY hKey = NULL;
	HKEY hSubKey = NULL;
	LSTATUS ret = 0;
	do
	{
		ret = RegOpenKeyEx(HKEY_LOCAL_MACHINE, REMOTE_PATH, 0, KEY_ALL_ACCESS, &hKey);
		if (ret != ERROR_SUCCESS)
		{
			break;
		}
		//打开子键
		ret = RegOpenKeyEx(hKey, regName, 0, KEY_ALL_ACCESS, &hSubKey);
		if (ret != ERROR_SUCCESS)
		{
			break;
		}
		ret = RegeditUtils::SetVal(hSubKey, key, val);
		if (ret != ERROR_SUCCESS)
		{
			break;
		}
	} while (0);
	if (hSubKey != NULL)
	{
		RegCloseKey(hSubKey);
		hSubKey = NULL;
	}
	if (hKey != NULL)
	{
		RegCloseKey(hKey);
		hKey = NULL;
	}
	return ret;
}
LSTATUS RemoteAppUtils::SetRemoteAppVal(const TCHAR* regName, const TCHAR* key, DWORD val)
{
	HKEY hKey = NULL;
	HKEY hSubKey = NULL;
	LSTATUS ret = 0;
	do
	{
		ret = RegOpenKeyEx(HKEY_LOCAL_MACHINE, REMOTE_PATH, 0, KEY_ALL_ACCESS, &hKey);
		if (ret != ERROR_SUCCESS)
		{
			break;
		}
		//打开子键
		ret = RegOpenKeyEx(hKey, regName, 0, KEY_ALL_ACCESS, &hSubKey);
		if (ret != ERROR_SUCCESS)
		{
			break;
		}
		ret = RegeditUtils::SetVal(hSubKey, key, val);
		if (ret != ERROR_SUCCESS)
		{
			break;
		}
	} while (0);
	if (hSubKey != NULL)
	{
		RegCloseKey(hSubKey);
		hSubKey = NULL;
	}
	if (hKey != NULL)
	{
		RegCloseKey(hKey);
		hKey = NULL;
	}
	return ret;
}
