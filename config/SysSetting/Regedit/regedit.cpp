#include<Windows.h>
//advapi32.lib
#define BASE_KEY  L"HKEY_CURRENT_USER"
#define SUB_KEY L"Control Panel\\Desktop"
#define VALUE_NAME L"ShowMenuDelay"
//获取根键
HKEY GetBaseRegKey(WCHAR* keystr)
{
	HKEY hKey;
	if (lstrcmp(keystr, L"HKEY_CLASSES_ROOT") == 0)
		hKey = HKEY_CLASSES_ROOT;
	else if (lstrcmp(keystr, L"HKEY_CURRENT_CONFIG") == 0)
		hKey = HKEY_CURRENT_CONFIG;
	else if (lstrcmp(keystr, L"HKEY_CURRENT_USER") == 0)
		hKey = HKEY_CURRENT_USER;
	else if (lstrcmp(keystr, L"HKEY_LOCAL_MACHINE") == 0)
		hKey = HKEY_LOCAL_MACHINE;
	else if (lstrcmp(keystr, L"HKEY_USERS") == 0)
		hKey = HKEY_USERS;
	return hKey;
}
void Enum()
{
	WCHAR *p360 = L"360SandBox";
	DWORD nSize = lstrlen(p360);
	FILETIME time = { 0 };
	int i = 0;
	while(true)
	{
		LONG retVal = RegEnumKeyEx(HKEY_USERS, i, p360, &nSize, NULL, NULL, NULL, &time);
		if (retVal != ERROR_SUCCESS)
		{
			break;
		}
		i++;
	}

	i = 0;
	WCHAR name[MAX_PATH] = { 0 };
	unsigned char dataBuf[1024] = { 0 };
	while (true)
	{
		nSize = MAX_PATH;
		DWORD dataSize = 1024;
		LONG retVal = RegEnumValue(HKEY_USERS, i, name, &nSize, NULL, NULL, (LPBYTE)dataBuf,&dataSize);
		if (retVal != ERROR_SUCCESS)
		{
			break;
		}
		i++;
	}
}
void mainRe()
{
	HKEY bKey = GetBaseRegKey(BASE_KEY);

	HKEY cKey = { 0 };
	//创建子键，若子键已存在，则等价于OpenKeyEx
	LONG retVal = RegCreateKeyEx(bKey, SUB_KEY, 0, NULL,
		REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &cKey, NULL);
	if (retVal != ERROR_SUCCESS)
		return;

	HKEY OKey = { 0 };
	//打开子键 
	retVal = RegOpenKeyEx(bKey, SUB_KEY, 0, KEY_ALL_ACCESS, &OKey);

	//写入键值
	WCHAR szModule[MAX_PATH];
	GetModuleFileName(NULL, szModule, MAX_PATH);
	//创建一个新的键值
	retVal = RegSetValueEx(OKey, VALUE_NAME, 0,
		REG_SZ, (BYTE*)szModule, lstrlen(szModule));

	//查询键值
	WCHAR szLocation[MAX_PATH] = { '\0' };
	DWORD dwSize = sizeof(szLocation);
	DWORD dwType = REG_SZ;
	RegQueryValueEx(OKey, VALUE_NAME, NULL, &dwType, (LPBYTE)&szLocation, &dwSize);

	//删除键值
	retVal = RegDeleteValue(OKey, VALUE_NAME);

	//关闭注册表
	RegCloseKey(OKey);
	RegCloseKey(cKey);
	//删除子键
	//RegDeleteKey(bKey, SUB_KEY);
}

/*
REG_SZ				字符串
REG_MULTI_SZ	含有多个文本值的字符串
REG_BINARY		二进制值，以十六进制显示
REG_DWORD		一个32位的二进制值，显示为8位的十六进制值
*/