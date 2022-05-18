#include<Windows.h>
#include<tchar.h>
#include<stdio.h>
int main()
{
	//新增
	HKEY hKey = NULL;
	LSTATUS ret = 0;
	do
	{
		ret = RegCreateKeyEx(HKEY_LOCAL_MACHINE,
			_T("SYSTEM\CurrentControlSet\Control\Session Manager\kernel\"), NULL, NULL, REG_OPTION_NON_VOLATILE"),
			NULL, NULL, REG_OPTION_NON_VOLATILE,
			KEY_ALL_ACCESS, NULL, &hKey, NULL);
		if (ret != ERROR_SUCCESS)
		{
			break;
		}
		//key里写值
		DWORD val = 0;
		ret = RegSetValueEx(hKey, L"obcaseinsensitive", NULL, REG_DWORD, (BYTE*)&val, sizeof(val));
		if (ret != ERROR_SUCCESS)
		{
			printf("RegCreateKeyEx error %u \n", ret);
			break;
		}
	} while (0);
	if (hKey != NULL)
	{
		RegCloseKey(hKey);
		hKey = NULL;
	}
	return ret;
}