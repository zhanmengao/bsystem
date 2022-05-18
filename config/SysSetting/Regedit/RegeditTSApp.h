#pragma once
#include<Windows.h>
#include<stdio.h>
#include<iostream>
#include<tchar.h>
#define REGEDIT_PATH L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Terminal Server\\TSAppAllowList\\Applications"
void RegeditMain()
{
	//��
	HKEY hKey = NULL;
	DWORD ret = 0;
	if ((ret = RegOpenKeyEx(HKEY_LOCAL_MACHINE, REGEDIT_PATH, 0, KEY_ALL_ACCESS, &hKey)) != ERROR_SUCCESS)
	{
		printf("RegOpenKeyEx error %u \n", ret);
		return;
	}
	//����
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
			//key��дֵ
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

	//�޸�
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
			//key��дֵ
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

	//��ѯ
	{
		//������ѯ
		int i = 0;
		TCHAR szNameValue[MAXBYTE] = { 0 };
		DWORD szNameSize = MAXBYTE;
		while (1)
		{
			//ö��
			ret = RegEnumKey(hKey, i, szNameValue, szNameSize);
			if (ret != ERROR_SUCCESS)
			{
				break;
			}
			printf("RegEnumKey[%d] %ws \n", i, szNameValue);
			i++;
			HKEY hSubKey;
			//��ÿ��key
			if (RegOpenKeyEx(hKey, szNameValue, NULL, KEY_READ, &hSubKey) == ERROR_SUCCESS)
			{
				int j = 0;
				DWORD dwNameSize = MAXBYTE;				//Name�����С
				DWORD dwKeySize = MAXBYTE;				//Key�����С
				DWORD dwType = 0;										//��������
				TCHAR szNameValue[MAXBYTE] = { 0 };
				TCHAR szKeyValue[MAXBYTE] = { 0 };
				while (1)
				{
					dwNameSize = MAXBYTE;
					dwKeySize = MAXBYTE;
					//�ٲ�ѯÿ��value
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

	//ɾ��
	{
		//ɾֵ
		HKEY hSubKey = NULL;
		{
			ret = RegDeleteKeyValue(hKey, L"MyTest", L"Name");
			if (ret != ERROR_SUCCESS)
			{
				printf("RegDeleteKeyValue error %u \n", ret);
			}
		}
		//�ȼ���
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
		//ɾ��
		ret = RegDeleteKey(hKey,L"MyTest");
		if (ret != ERROR_SUCCESS)
		{
			printf("RegDeleteKey error %u \n", ret);
		}
	}
	RegCloseKey(hKey);

}