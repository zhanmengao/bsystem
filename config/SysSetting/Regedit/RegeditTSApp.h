#pragma once
#include<Windows.h>
#include<stdio.h>
#include<iostream>
#include<tchar.h>
#define REGEDIT_PATH L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Terminal Server\\TSAppAllowList\\Applications"
void RegeditMain()
{
	//打开
	HKEY hKey = NULL;
	DWORD ret = 0;
	if ((ret = RegOpenKeyEx(HKEY_LOCAL_MACHINE, REGEDIT_PATH, 0, KEY_ALL_ACCESS, &hKey)) != ERROR_SUCCESS)
	{
		printf("RegOpenKeyEx error %u \n", ret);
		return;
	}
	//新增
	{
		HKEY hSubKey = NULL;
		do
		{
			ret = RegCreateKeyEx(hKey, L"MyTest", NULL, NULL, REG_OPTION_NON_VOLATILE,
				KEY_ALL_ACCESS, NULL, &hSubKey, NULL);
			if (ret != ERROR_SUCCESS)
			{
				printf("RegCreateKeyEx error %u \n", ret);
				break;
			}
			TCHAR szModule[MAX_PATH];
			_tcscpy_s(szModule,MAX_PATH, L"QAQAAA");
			//key里写值
			ret = RegSetValueEx(hSubKey, L"Name", NULL, REG_SZ, (BYTE*)szModule, lstrlen(szModule) * sizeof(TCHAR));
			if (ret != ERROR_SUCCESS)
			{
				printf("RegCreateKeyEx error %u \n", ret);
				break;
			}
		} while (0);
		if (hSubKey != NULL)
		{
			RegCloseKey(hSubKey);
			hSubKey = NULL;
		}
	}

	//修改
	{
		HKEY hSubKey = NULL;
		do
		{
			ret = RegOpenKeyEx(hKey, L"MyTest", NULL, KEY_ALL_ACCESS, &hSubKey);
			if (ret != ERROR_SUCCESS)
			{
				printf("RegOpenKeyEx error %u \n", ret);
				break;
			}
			TCHAR szModule[MAX_PATH];
			GetModuleFileName(NULL, szModule, MAX_PATH);
			//key里写值
			ret = RegSetValueEx(hSubKey, L"Name", NULL, REG_SZ, (BYTE*)szModule, lstrlen(szModule) * sizeof(TCHAR));
			if (ret != ERROR_SUCCESS)
			{
				printf("RegCreateKeyEx error %u \n", ret);
				break;
			}
		} while (0);
		if (hSubKey != NULL)
		{
			RegCloseKey(hSubKey);
			hSubKey = NULL;
		}
	}

	//查询
	{
		//遍历查询
		int i = 0;
		TCHAR szNameValue[MAXBYTE] = { 0 };
		DWORD szNameSize = MAXBYTE;
		while (1)
		{
			//枚举
			ret = RegEnumKey(hKey, i, szNameValue, szNameSize);
			if (ret != ERROR_SUCCESS)
			{
				break;
			}
			printf("RegEnumKey[%d] %ws \n", i, szNameValue);
			i++;
			HKEY hSubKey;
			//打开每个key
			if (RegOpenKeyEx(hKey, szNameValue, NULL, KEY_READ, &hSubKey) == ERROR_SUCCESS)
			{
				int j = 0;
				DWORD dwNameSize = MAXBYTE;				//Name数组大小
				DWORD dwKeySize = MAXBYTE;				//Key数组大小
				DWORD dwType = 0;										//数据类型
				TCHAR szNameValue[MAXBYTE] = { 0 };
				TCHAR szKeyValue[MAXBYTE] = { 0 };
				while (1)
				{
					dwNameSize = MAXBYTE;
					dwKeySize = MAXBYTE;
					//再查询每个value
					ret = RegEnumValue(hSubKey, j, szNameValue, &dwNameSize, NULL,
						&dwType, (unsigned char*)szKeyValue, &dwKeySize);
					if (ret != ERROR_SUCCESS)
					{
						break;
					}
					switch (dwType)
					{
					case REG_SZ:
						printf("	key[%d] = %ws \n	val:%ws  \n", j, szNameValue, szKeyValue);
						break;
					case REG_DWORD:
						printf("	key[%d] = %ws \n	val:%d  \n", j, szNameValue, szKeyValue);
						break;
					}

					j++;
				}
			}
			if (hSubKey != NULL)
			{
				RegCloseKey(hSubKey);
			}
		}
	}

	//删除
	{
		//删值
		HKEY hSubKey = NULL;
		{
			ret = RegDeleteKeyValue(hKey, L"MyTest", L"Name");
			if (ret != ERROR_SUCCESS)
			{
				printf("RegDeleteKeyValue error %u \n", ret);
			}
		}
		//等价于
		{
			ret = RegOpenKeyEx(hKey, L"MyTest",NULL, KEY_ALL_ACCESS,&hSubKey);
			if (ret != ERROR_SUCCESS)
			{
				printf("RegOpenKeyEx error %u \n", ret);
			}
			ret = RegDeleteValue(hSubKey, L"Name");
			if (ret != ERROR_SUCCESS)
			{
				printf("RegDeleteValue error %u \n", ret);
			}
		}
		//删键
		ret = RegDeleteKey(hKey,L"MyTest");
		if (ret != ERROR_SUCCESS)
		{
			printf("RegDeleteKey error %u \n", ret);
		}
	}
	RegCloseKey(hKey);

}