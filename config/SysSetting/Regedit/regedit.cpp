#include<Windows.h>
//advapi32.lib
#define BASE_KEY  L"HKEY_CURRENT_USER"
#define SUB_KEY L"Control Panel\\Desktop"
#define VALUE_NAME L"ShowMenuDelay"
//��ȡ����
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
	//�����Ӽ������Ӽ��Ѵ��ڣ���ȼ���OpenKeyEx
	LONG retVal = RegCreateKeyEx(bKey, SUB_KEY, 0, NULL,
		REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &cKey, NULL);
	if (retVal != ERROR_SUCCESS)
		return;

	HKEY OKey = { 0 };
	//���Ӽ� 
	retVal = RegOpenKeyEx(bKey, SUB_KEY, 0, KEY_ALL_ACCESS, &OKey);

	//д���ֵ
	WCHAR szModule[MAX_PATH];
	GetModuleFileName(NULL, szModule, MAX_PATH);
	//����һ���µļ�ֵ
	retVal = RegSetValueEx(OKey, VALUE_NAME, 0,
		REG_SZ, (BYTE*)szModule, lstrlen(szModule));

	//��ѯ��ֵ
	WCHAR szLocation[MAX_PATH] = { '\0' };
	DWORD dwSize = sizeof(szLocation);
	DWORD dwType = REG_SZ;
	RegQueryValueEx(OKey, VALUE_NAME, NULL, &dwType, (LPBYTE)&szLocation, &dwSize);

	//ɾ����ֵ
	retVal = RegDeleteValue(OKey, VALUE_NAME);

	//�ر�ע���
	RegCloseKey(OKey);
	RegCloseKey(cKey);
	//ɾ���Ӽ�
	//RegDeleteKey(bKey, SUB_KEY);
}

/*
REG_SZ				�ַ���
REG_MULTI_SZ	���ж���ı�ֵ���ַ���
REG_BINARY		������ֵ����ʮ��������ʾ
REG_DWORD		һ��32λ�Ķ�����ֵ����ʾΪ8λ��ʮ������ֵ
*/