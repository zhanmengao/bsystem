#include"RegeditUtils.h"
#include<tchar.h>
//避免重定位 KEY_WOW64_64KEY
LSTATUS RegeditUtils::GetKeyVal(const HKEY& hMain, const TCHAR* key, std::vector<stdstr> &vec)
{
	HKEY hKey = NULL;
	LSTATUS bRet = TRUE;
	do
	{
		//打开键
		bRet = RegOpenKeyEx(HKEY_CURRENT_USER, key, 0, KEY_ALL_ACCESS, &hKey);
		if (bRet != ERROR_SUCCESS)
		{
			break;
		}
		int i = 0;
		DWORD dwNameSize = MAXBYTE;				//Name数组大小
		DWORD dwKeySize = MAXBYTE;				//Key数组大小
		DWORD dwType = 0;										//数据类型
		TCHAR szNameValue[MAXBYTE] = { 0 };
		TCHAR szKeyValue[MAXBYTE] = { 0 };
		while (true)
		{
			dwKeySize = MAXBYTE;
			dwNameSize = MAXBYTE;
			//枚举值
			bRet = RegEnumValue(hKey, i, szNameValue, &dwNameSize, NULL, &dwType,
				(unsigned char*)szKeyValue, &dwKeySize);
			if (bRet != ERROR_SUCCESS)
			{
				break;
			}
			switch (dwType)
			{
			case REG_SZ:
				vec.push_back(szKeyValue);
				break;
			case REG_DWORD:
				vec.push_back(TOSTR((DWORD)*szKeyValue));
				break;
			}

			i++;
		}
	} while (0);
	if (hKey != NULL)
	{
		RegCloseKey(hKey);
		hKey = NULL;
	}
	return bRet;
}
LSTATUS RegeditUtils::SetKeyVal(const HKEY& hMain, const TCHAR* key, const TCHAR* newKey, const TCHAR* valName, const TCHAR* val)
{
	//新增
	HKEY hKey = NULL;
	HKEY hSubKey = NULL;
	LSTATUS ret = 0;
	do
	{
		//ret = RegOpenKeyEx(hMain, key, 0, KEY_ALL_ACCESS, &hKey);
		//if (ret != ERROR_SUCCESS)
		//{
		//	break;
		//}
		ret = RegCreateKeyEx(hMain, key, NULL, NULL, REG_OPTION_NON_VOLATILE,
			KEY_ALL_ACCESS, NULL, &hKey, NULL);
		if (ret != ERROR_SUCCESS)
		{
			break;
		}
		ret = RegCreateKeyEx(hKey, newKey, NULL, NULL, REG_OPTION_NON_VOLATILE,
			KEY_ALL_ACCESS, NULL, &hSubKey, NULL);
		if (ret != ERROR_SUCCESS)
		{
			break;
		}
		ret = RegeditUtils::SetVal(hSubKey, valName, val);
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
LSTATUS RegeditUtils::SetVal(const HKEY& hKey, const TCHAR* key, DWORD val)
{
	return RegSetValueEx(hKey, key, NULL, REG_DWORD, (BYTE*)&val, sizeof(DWORD));
}
LSTATUS RegeditUtils::SetVal(const HKEY& hKey, const TCHAR* key, const TCHAR* val)
{
	return RegSetValueEx(hKey, key, NULL, REG_SZ, (BYTE*)val, lstrlen(val) * sizeof(TCHAR));
}
LSTATUS RegeditUtils::GetVal(const HKEY& hKey, const TCHAR* key, DWORD& val)
{
	DWORD dwType = 0;
	DWORD dwLongSize = sizeof(DWORD);
	val = 0;
	return RegQueryValueEx(hKey, key, 0, &dwType, (LPBYTE)&val, &dwLongSize);
}
LSTATUS RegeditUtils::GetVal(const HKEY& hKey, const TCHAR* key, TCHAR* val, DWORD dataSize, DWORD& retSize)
{
	DWORD dwType = 0;
	lstrcpy(val, _T(""));
	retSize = dataSize;
	return RegQueryValueEx(hKey, key, 0, &dwType, (LPBYTE)val, &retSize);
}


#define KEY_AUTORUN _T("Software\\Microsoft\\Windows\\CurrentVersion\\Run")
LSTATUS RegeditUtils::RegAutoRun_CurrentUser(const TCHAR* lpName, const TCHAR* lpFilePath)
{
	HKEY hKey = NULL;
	LSTATUS bRet = TRUE;
	do
	{
		//打开键
		bRet = RegOpenKeyEx(HKEY_CURRENT_USER, KEY_AUTORUN, 0, KEY_WRITE | KEY_WOW64_64KEY, &hKey);
		if (bRet != ERROR_SUCCESS)
		{
			break;
		}
		//修改值
		TCHAR buffer[1024] = { 0 };
		_tcscat_s(buffer, _countof(buffer), _T("\""));
		_tcscat_s(buffer, _countof(buffer), lpFilePath);
		_tcscat_s(buffer, _countof(buffer), _T("\" -autorun"));
		bRet = RegSetValueEx(hKey, lpName, 0, REG_SZ, (const BYTE*)buffer, lstrlen(buffer) * sizeof(TCHAR));
		if (bRet != ERROR_SUCCESS)
		{
			break;
		}
	} while (0);
	if (hKey != NULL)
	{
		RegCloseKey(hKey);
		hKey = NULL;
	}
	return bRet;
}

LSTATUS RegeditUtils::ShowRunList(std::vector<stdstr>& vec)
{
	HKEY hKey = NULL;
	LSTATUS bRet = TRUE;
	do
	{
		//打开键
		bRet = RegOpenKeyEx(HKEY_CURRENT_USER, KEY_AUTORUN, 0, KEY_ALL_ACCESS, &hKey);
		if (bRet != ERROR_SUCCESS)
		{
			break;
		}
		int i = 0;
		DWORD dwNameSize = MAXBYTE;				//Name数组大小
		DWORD dwKeySize = MAXBYTE;				//Key数组大小
		DWORD dwType = 0;										//数据类型
		TCHAR szNameValue[MAXBYTE] = { 0 };
		TCHAR szKeyValue[MAXBYTE] = { 0 };
		while (true)
		{
			dwKeySize = MAXBYTE;
			dwNameSize = MAXBYTE;
			//枚举值
			bRet = RegEnumValue(hKey, i, szNameValue, &dwNameSize, NULL, &dwType,
				(unsigned char*)szKeyValue, &dwKeySize);
			if (bRet != ERROR_SUCCESS)
			{
				break;
			}
			vec.push_back(szKeyValue);
			printf("[%d] = name:%ws value:%ws  \n", i, szNameValue, szKeyValue);
			i++;
		}
	} while (0);
	if (hKey != NULL)
	{
		RegCloseKey(hKey);
		hKey = NULL;
	}

	do
	{
		//打开键
		bRet = RegOpenKeyEx(HKEY_LOCAL_MACHINE, KEY_AUTORUN, 0, KEY_ALL_ACCESS, &hKey);
		if (bRet != ERROR_SUCCESS)
		{
			break;
		}
		int i = 0;
		DWORD dwNameSize = MAXBYTE;				//Name数组大小
		DWORD dwKeySize = MAXBYTE;				//Key数组大小
		DWORD dwType = 0;										//数据类型
		TCHAR szNameValue[MAXBYTE] = { 0 };
		TCHAR szKeyValue[MAXBYTE] = { 0 };
		while (true)
		{
			dwKeySize = MAXBYTE;
			dwNameSize = MAXBYTE;
			//枚举值
			bRet = RegEnumValue(hKey, i, szNameValue, &dwNameSize, NULL, &dwType,
				(unsigned char*)szKeyValue, &dwKeySize);
			if (bRet != ERROR_SUCCESS)
			{
				break;
			}
			vec.push_back(szKeyValue);
			printf("%ws[%d] = name:%ws \nvalue:%ws  \n", KEY_AUTORUN, i, szNameValue, szKeyValue);
			i++;
		}
	} while (0);
	if (hKey != NULL)
	{
		RegCloseKey(hKey);
		hKey = NULL;
	}
	return bRet;
}

LSTATUS RegeditUtils::DelAutoRun_CurrentUser(const TCHAR* lpKeyValue)
{
	HKEY hKey = NULL;
	LSTATUS bRet = TRUE;
	do
	{
		//打开键
		bRet = RegOpenKeyEx(HKEY_CURRENT_USER, KEY_AUTORUN, 0, KEY_WRITE, &hKey);
		if (bRet != ERROR_SUCCESS)
		{
			break;
		}
		//删除值
		bRet = RegDeleteValue(hKey, lpKeyValue);
		if (bRet != ERROR_SUCCESS)
		{
			break;
		}
	} while (0);
	if (hKey != NULL)
	{
		RegCloseKey(hKey);
		hKey = NULL;
	}
	return bRet;
}