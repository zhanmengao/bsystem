#include "stdafx.h"

#define   INITGUID 

#include "RegEdit.h"
#include <string>
#include <InitGuid.h>
#include <GPEdit.h>
#include <Guiddef.h>

#pragma comment(lib, "Gpedit.lib")

using namespace std;


DWORD CRegEdit::GetRDPPort()
{

	DWORD rdpPort = 0;
	GetRegEdit(L"System\\CurrentControlSet\\Control\\Terminal Server\\Wds\\rdpwd\\Tds\\Tcp",
		L"PortNumber",
		NULL,
		rdpPort	);

	return rdpPort;
}

LONG CRegEdit::SetRDPPort(int rdpPort)
{	

	LONG ret = SetRegEdit(L"System\\CurrentControlSet\\Control\\Terminal Server\\Wds\\rdpwd\\Tds\\Tcp",
		L"PortNumber",
		REG_DWORD,
		NULL,
		rdpPort );

	ret = SetRegEdit(L"SYSTEM\\CurrentControlSet\\Control\\Terminal Server\\WinStations\\RDP-Tcp",
		L"PortNumber",
		REG_DWORD,
		NULL,
		rdpPort);
	

	return ret;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool CRegEdit::EnablePriviledge(LPCTSTR lpSystemName)
{
	HANDLE hToken;
	TOKEN_PRIVILEGES tkp = { 1 };
	if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
	{
		if (LookupPrivilegeValue(NULL, lpSystemName, &tkp.Privileges[0].Luid))
		{
			tkp.PrivilegeCount = 1;
			tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
			AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0);
			if (GetLastError() != ERROR_SUCCESS)
			{
				CloseHandle(hToken);
				return false;
			}
		}
		CloseHandle(hToken);
	}

	return true;
}

LONG CRegEdit::WriteShellFolderRegKey(wchar_t* wCLSID, DWORD wValue)
{

	wchar_t idbuf[128] = { 0 };
	wchar_t *CLSID = wCLSID;

	wchar_t strVal[] = L"";
	DWORD value = wValue;

	HKEY hResult = NULL;
	DWORD dwDisposition = 0;

	wsprintf(idbuf, L"CLSID\\%ls\\ShellFolder", CLSID);

	LONG lResult = RegCreateKeyEx(
								HKEY_CLASSES_ROOT,
								idbuf,
								0,
								NULL,
								REG_OPTION_BACKUP_RESTORE,
								KEY_ALL_ACCESS | KEY_WOW64_64KEY,
								NULL,
								&hResult,
								&dwDisposition);

	if (lResult != ERROR_SUCCESS)
	{
		return lResult;
	}


	lResult = RegSetValueEx(hResult, L"Attributes", NULL, REG_DWORD, (LPBYTE)&value, sizeof(DWORD));
	RegSetValueEx(hResult, L"QueryForOverlay", NULL, REG_SZ, (LPBYTE)strVal, sizeof(strVal));
	//RegSetValueEx(hResult, L"HideOnDesktopPerUser", NULL, REG_SZ, (LPBYTE)strVal, sizeof(strVal));
	RegSetValueEx(hResult, L"PinToNameSpaceTree", NULL, REG_SZ, (LPBYTE)strVal, sizeof(strVal));

	printf("SetValueEx %ls %d\n", idbuf, lResult);

	RegCloseKey(hResult);

	//x32
	lResult = RegCreateKeyEx(
							HKEY_CLASSES_ROOT,
							idbuf,
							0,
							NULL,
							REG_OPTION_BACKUP_RESTORE,
							KEY_ALL_ACCESS | KEY_WOW64_32KEY,
							NULL,
							&hResult,
							&dwDisposition);

	if (lResult != ERROR_SUCCESS)
	{
		return lResult;
	}


	lResult = RegSetValueEx(hResult, L"Attributes", NULL, REG_DWORD, (LPBYTE)&value, sizeof(DWORD));
	RegSetValueEx(hResult, L"QueryForOverlay", NULL, REG_SZ, (LPBYTE)strVal, sizeof(strVal));
	//RegSetValueEx(hResult, L"HideOnDesktopPerUser", NULL, REG_SZ, (LPBYTE)strVal, sizeof(strVal));
	RegSetValueEx(hResult, L"PinToNameSpaceTree", NULL, REG_SZ, (LPBYTE)strVal, sizeof(strVal));

	printf("SetValueEx %ls %d\n", idbuf, lResult);

	RegCloseKey(hResult);


	return lResult;

}

LONG CRegEdit::WriteThisPCPolicyRegKey(wchar_t* wCLSID, wchar_t* wValue)
{

	wchar_t idbuf[128] = { 0 };
	wchar_t *CLSID = wCLSID;

	const wchar_t* value = wValue;


	HKEY hResult = NULL;
	DWORD dwDisposition = 0;

	wsprintf(idbuf, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\FolderDescriptions\\%ls\\PropertyBag", CLSID);

	LONG lResult = RegCreateKeyEx(
									HKEY_LOCAL_MACHINE,
									idbuf,
									0,
									NULL,
									REG_OPTION_BACKUP_RESTORE,
									KEY_ALL_ACCESS | KEY_WOW64_64KEY,
									NULL,
									&hResult,
									&dwDisposition);

	if (lResult != ERROR_SUCCESS)
	{
		return lResult;
	}

	lResult = RegSetValueEx(hResult, L"ThisPCPolicy", NULL, REG_SZ, (LPBYTE)value, wcslen(value) * sizeof(wchar_t));
	printf("SetThisPCPolicy %ls %d\n", idbuf, lResult);

	RegCloseKey(hResult);

	//x32
	wsprintf(idbuf, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\FolderDescriptions\\%ls\\PropertyBag", CLSID);
	lResult = RegCreateKeyEx(
								HKEY_LOCAL_MACHINE,
								idbuf,
								0,
								NULL,
								REG_OPTION_BACKUP_RESTORE,
								KEY_ALL_ACCESS | KEY_WOW64_32KEY,
								NULL,
								&hResult,
								&dwDisposition);

	if (lResult != ERROR_SUCCESS)
	{
		return lResult;
	}

	lResult = RegSetValueEx(hResult, L"ThisPCPolicy", NULL, REG_SZ, (LPBYTE)value, wcslen(value) * sizeof(wchar_t));
	printf("SetThisPCPolicy %ls %d\n", idbuf, lResult);

	RegCloseKey(hResult);
	
	return lResult;

}

LONG CRegEdit::SetRegEdit(wchar_t *lpSubKey, wchar_t* lpValueName, DWORD dwType, wchar_t* szValue, DWORD dValue)
{

	const wchar_t* strVal = szValue;
	const DWORD dVal = dValue;

	HKEY hResult = NULL;
	DWORD dwDisposition = 0;

	LONG lResult = RegCreateKeyEx(
									HKEY_LOCAL_MACHINE,
									lpSubKey,
									0,
									NULL,
									REG_OPTION_BACKUP_RESTORE,
									KEY_ALL_ACCESS | KEY_WOW64_64KEY,
									NULL,
									&hResult,
									&dwDisposition);

	if (lResult != ERROR_SUCCESS)
	{
		RegCloseKey(hResult);
		return lResult;
	}

	if (dwType == REG_DWORD )
	{
		lResult = RegSetValueEx(hResult, lpValueName, NULL, REG_DWORD, (LPBYTE)&dVal, sizeof(DWORD));
	}
	else
	{
		lResult = RegSetValueEx(hResult, lpValueName, NULL, dwType, (LPBYTE)strVal, wcslen(strVal) * sizeof(wchar_t));
	}
	
	

	RegCloseKey(hResult);

	return lResult;
}

LONG CRegEdit::SetRegEditWin32(wchar_t *lpSubKey, wchar_t* lpValueName, DWORD dwType, wchar_t* szValue, DWORD dValue)
{

	const wchar_t* strVal = szValue;
	const DWORD dVal = dValue;

	HKEY hResult = NULL;
	DWORD dwDisposition = 0;

	LONG lResult = RegCreateKeyEx(
		HKEY_LOCAL_MACHINE,
		lpSubKey,
		0,
		NULL,
		REG_OPTION_BACKUP_RESTORE,
		KEY_ALL_ACCESS | KEY_WOW64_32KEY,
		NULL,
		&hResult,
		&dwDisposition);

	if (lResult != ERROR_SUCCESS)
	{
		RegCloseKey(hResult);
		return lResult;
	}

	if (dwType == REG_DWORD)
	{
		lResult = RegSetValueEx(hResult, lpValueName, NULL, REG_DWORD, (LPBYTE)&dVal, sizeof(DWORD));
	}
	else
	{
		lResult = RegSetValueEx(hResult, lpValueName, NULL, dwType, (LPBYTE)strVal, wcslen(strVal) * sizeof(wchar_t));
	}



	RegCloseKey(hResult);

	return lResult;
}


LONG CRegEdit::GetRegEdit(wchar_t *lpSubKey, wchar_t* lpValueName, wchar_t* szValue, DWORD& dValue, HKEY MinhKey)
{
	HKEY hKey;
	DWORD dwDisp;
	DWORD dwType;

	//HKEY MinhKey = HKEY_LOCAL_MACHINE;
	LONG ret = RegCreateKeyEx(
								MinhKey,
								lpSubKey,
								0,
								NULL,
								REG_OPTION_NON_VOLATILE,
								KEY_ALL_ACCESS | KEY_WOW64_64KEY,
								NULL,
								&hKey,
								&dwDisp);
		
	DWORD   dtVal = 0;
	wchar_t stVal[MAX_PATH] = { 0 };
	DWORD   vsize = MAX_PATH;	

	if (ret == ERROR_SUCCESS)
	{
		
		ret = RegQueryValueEx(hKey, lpValueName, NULL, &dwType, (BYTE*)stVal, &vsize);
	
		if ( dwType == REG_DWORD )
		{
			dtVal = stVal[0];
			dValue = dtVal;
		}
		else
		{
			if ( lstrlen( stVal ) != 0  )
			{
				lstrcpy(szValue, stVal);
			}
			 
		}
		
	}

	RegCloseKey(hKey);

	return ret;
}

LONG CRegEdit::GetRegEditWin32(wchar_t *lpSubKey, wchar_t* lpValueName, wchar_t* szValue, DWORD& dValue, HKEY MinhKey)
{
	HKEY hKey;
	DWORD dwDisp;
	DWORD dwType;

	//HKEY MinhKey = HKEY_LOCAL_MACHINE;
	LONG ret = RegCreateKeyEx(
		MinhKey,
		lpSubKey,
		0,
		NULL,
		REG_OPTION_NON_VOLATILE,
		KEY_ALL_ACCESS | KEY_WOW64_32KEY,
		NULL,
		&hKey,
		&dwDisp);

	DWORD   dtVal = 0;
	wchar_t stVal[MAX_PATH] = { 0 };
	DWORD   vsize = MAX_PATH;

	if (ret == ERROR_SUCCESS)
	{

		ret = RegQueryValueEx(hKey, lpValueName, NULL, &dwType, (BYTE*)stVal, &vsize);

		if (dwType == REG_DWORD)
		{
			dtVal = stVal[0];
			dValue = dtVal;
		}
		else
		{
			if (lstrlen(stVal) != 0)
			{
				lstrcpy(szValue, stVal);
			}

		}

	}

	RegCloseKey(hKey);

	return ret;
}


LONG CRegEdit::SetComputerPolicies( wchar_t* gpoKey , DWORD dwType,wchar_t* name, wchar_t* valstr, DWORD valdword )
{

	::CoInitialize(NULL);

	HKEY ghoKey, ghoSubKey, hSubKey;

	WCHAR *GPO_REG = gpoKey;
	//L"SOFTWARE\\Policies\\Microsoft\\Windows NT\\Terminal Services";

	DWORD value_dword = valdword;
	const wchar_t* value_pwchar = valstr;
	const wchar_t* lpValueName = name;

	IGroupPolicyObject *pGPO = NULL;
	CoCreateInstance(CLSID_GroupPolicyObject, NULL, CLSCTX_ALL, IID_IGroupPolicyObject, (LPVOID*)&pGPO);

	LONG lResult = RegCreateKeyEx(HKEY_LOCAL_MACHINE,
									GPO_REG,
									0,
									NULL,
									REG_OPTION_NON_VOLATILE,
									KEY_ALL_ACCESS | KEY_WOW64_64KEY,
									NULL,
									&hSubKey,
									NULL);

	if ( dwType == REG_DWORD )
	{
		lResult = RegSetValueEx(hSubKey, lpValueName, 0, REG_DWORD, (const BYTE *)&value_dword, sizeof(DWORD));
	}
	else
	{
		lResult = RegSetValueEx(hSubKey, lpValueName, 0, dwType, (const BYTE *)value_pwchar, wcslen(value_pwchar) * sizeof(wchar_t) );
	}
	
	RegCloseKey(hSubKey);
	
	pGPO->OpenLocalMachineGPO(GPO_OPEN_LOAD_REGISTRY);
	pGPO->GetRegistryKey(GPO_SECTION_MACHINE, &ghoKey);

	lResult = RegCreateKeyEx(ghoKey,
								GPO_REG,
								0,
								NULL,
								REG_OPTION_NON_VOLATILE,
								KEY_ALL_ACCESS | KEY_WOW64_64KEY,
								NULL,
								&ghoSubKey,
								NULL);
	

	if (dwType == REG_DWORD)
	{
		lResult = RegSetValueEx(ghoSubKey, lpValueName, 0, REG_DWORD, (const BYTE *)&value_dword, sizeof(DWORD));
	}
	else
	{
		lResult = RegSetValueEx(ghoSubKey, lpValueName, 0, dwType, (const BYTE *)value_pwchar, wcslen(value_pwchar) * sizeof(wchar_t));
	}

	GUID RegistryId = REGISTRY_EXTENSION_GUID;

	pGPO->Save(TRUE, TRUE, const_cast<GUID*>(&RegistryId), const_cast<GUID*>(&CLSID_GPESnapIn));
	lResult = pGPO->Release();

	RegCloseKey(ghoKey);
	RegCloseKey(ghoSubKey);


	::CoUninitialize();

	return lResult;

}

LONG CRegEdit::SetUserPolicies(wchar_t* gpoKey, DWORD dwType, wchar_t* name, wchar_t* valstr, DWORD valdword)
{	

		::CoInitialize(NULL);

		HKEY ghoKey, ghoSubKey, hSubKey;

		WCHAR *GPO_REG = gpoKey;
		
		DWORD value_dword = valdword;
		const wchar_t* value_pwchar = valstr;
		const wchar_t* lpValueName = name;

		IGroupPolicyObject *pGPO = NULL;
		CoCreateInstance(CLSID_GroupPolicyObject, NULL, CLSCTX_ALL, IID_IGroupPolicyObject, (LPVOID*)&pGPO);

		LONG lResult = RegCreateKeyEx(HKEY_CURRENT_USER,
			GPO_REG,
			0,
			NULL,
			REG_OPTION_NON_VOLATILE,
			KEY_ALL_ACCESS | KEY_WOW64_64KEY,
			NULL,
			&hSubKey,
			NULL);

		if (dwType == REG_DWORD)
		{
			lResult = RegSetValueEx(hSubKey, lpValueName, 0, REG_DWORD, (const BYTE *)&value_dword, sizeof(DWORD));
		}
		else
		{
			lResult = RegSetValueEx(hSubKey, lpValueName, 0, dwType, (const BYTE *)value_pwchar, wcslen(value_pwchar) * sizeof(wchar_t));
		}

		RegCloseKey(hSubKey);
		
	    pGPO->OpenLocalMachineGPO(GPO_OPEN_LOAD_REGISTRY);
		pGPO->GetRegistryKey(GPO_SECTION_USER, &ghoKey);

		lResult = RegCreateKeyEx(ghoKey,
			GPO_REG,
			0,
			NULL,
			REG_OPTION_NON_VOLATILE,
			KEY_ALL_ACCESS | KEY_WOW64_64KEY,
			NULL,
			&ghoSubKey, NULL);
						

		if (dwType == REG_DWORD)
		{
			lResult = RegSetValueEx(ghoSubKey, lpValueName, 0, REG_DWORD, (const BYTE *)&value_dword, sizeof(DWORD));
		}
		else
		{
			lResult = RegSetValueEx(ghoSubKey, lpValueName, 0, dwType, (const BYTE *)value_pwchar, wcslen(value_pwchar) * sizeof(wchar_t));
		}


		GUID RegistryId = REGISTRY_EXTENSION_GUID;

		GUID ThisAdminToolGuid = {
			0x0F6B957E,
			0x509E,
			0x11D1,
			{ 0xA7, 0xCC, 0x00, 0x00, 0xF8, 0x75, 0x71, 0xE3 }
		};
	
		//pGPO->Save(FALSE, TRUE, const_cast<GUID*>(&RegistryId), const_cast<GUID*>(&CLSID_GPESnapIn));
		pGPO->Save(FALSE, TRUE, const_cast<GUID*>(&RegistryId), const_cast<GUID*>(&ThisAdminToolGuid));
		
		lResult = pGPO->Release();

		RegCloseKey(ghoKey);
		RegCloseKey(ghoSubKey);

		
		::CoUninitialize();

		return lResult;	

}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
* ���ء��˼�������롾��ߵ������� ������Ŀ
**/
LONG CRegEdit::HideThisPCPolicy()
{
	/**
	HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\Explorer\FolderDescriptions
	ͼƬ�ļ��У�{0ddd015d-b06c-45d5-8c4c-f59713854639}
	��Ƶ�ļ��У�{35286a68-3c57-41a1-bbb1-0eae73d76c95}
	�����ļ��У�{7d83ee9b-2244-4e70-b1f5-5393042af1e4}
	�����ļ��У�{a0c69a99-21c8-4671-8703-7934162fcf1d}
	�����ļ��У�{b4bfcc3a-db2c-424c-b029-7fe99a87c641}
	�ĵ��ļ��У�{f42ee2d3-909f-4907-8871-4c22fc0bf756}
	3D�����ļ��У�{31c0dd25-9439-4f12-bf41-7ff4eda38722}
	\\PropertyBag
	ThisPCPolicy = Hide
	**/

	LONG result[10];

	result[0] = WriteThisPCPolicyRegKey(L"{b4bfcc3a-db2c-424c-b029-7fe99a87c641}", L"Hide");
	result[1] = WriteThisPCPolicyRegKey(L"{31c0dd25-9439-4f12-bf41-7ff4eda38722}", L"Hide");
	result[2] = WriteThisPCPolicyRegKey(L"{f42ee2d3-909f-4907-8871-4c22fc0bf756}", L"Hide");
	result[3] = WriteThisPCPolicyRegKey(L"{a0c69a99-21c8-4671-8703-7934162fcf1d}", L"Hide");
	result[4] = WriteThisPCPolicyRegKey(L"{7d83ee9b-2244-4e70-b1f5-5393042af1e4}", L"Hide");
	result[5] = WriteThisPCPolicyRegKey(L"{35286a68-3c57-41a1-bbb1-0eae73d76c95}", L"Hide");
	result[6] = WriteThisPCPolicyRegKey(L"{0ddd015d-b06c-45d5-8c4c-f59713854639}", L"Hide");
	result[7] = WriteThisPCPolicyRegKey(L"{33E28130-4E1E-4676-835A-98395C3BC3BB}", L"Hide");
	result[8] = WriteThisPCPolicyRegKey(L"{0ddd015d-b06c-45d5-8c4c-f59713854639}", L"Hide");
	result[9] = WriteThisPCPolicyRegKey(L"{C5ABBF53-E17F-4121-8900-86626FC2C973}", L"Hide");

	return 0;
}

/**
* ��ԭ���˼�������롾��ߵ������� ������Ŀ
**/
LONG CRegEdit::ShowThisPCPolicy()
{

	LONG result[10];

	result[0] = WriteThisPCPolicyRegKey(L"{b4bfcc3a-db2c-424c-b029-7fe99a87c641}", L"Show");
	result[1] = WriteThisPCPolicyRegKey(L"{31c0dd25-9439-4f12-bf41-7ff4eda38722}", L"Show");
	result[2] = WriteThisPCPolicyRegKey(L"{f42ee2d3-909f-4907-8871-4c22fc0bf756}", L"Show");
	result[3] = WriteThisPCPolicyRegKey(L"{a0c69a99-21c8-4671-8703-7934162fcf1d}", L"Show");
	result[4] = WriteThisPCPolicyRegKey(L"{7d83ee9b-2244-4e70-b1f5-5393042af1e4}", L"Show");
	result[5] = WriteThisPCPolicyRegKey(L"{35286a68-3c57-41a1-bbb1-0eae73d76c95}", L"Show");
	result[6] = WriteThisPCPolicyRegKey(L"{0ddd015d-b06c-45d5-8c4c-f59713854639}", L"Show");
	result[7] = WriteThisPCPolicyRegKey(L"{33E28130-4E1E-4676-835A-98395C3BC3BB}", L"Show");
	result[8] = WriteThisPCPolicyRegKey(L"{0ddd015d-b06c-45d5-8c4c-f59713854639}", L"Show");
	result[9] = WriteThisPCPolicyRegKey(L"{C5ABBF53-E17F-4121-8900-86626FC2C973}", L"Show");

	return 0;
}


/**
* ���ز�ߵ����������ٷ��ʡ�������Ŀ
**/
LONG CRegEdit::CleanLibLink()
{
	/**
	���ء���ͥ�顰��Ŀ
	HKEY_CLASSES_ROOT\CLSID\{B4FB3F98-C1EA-428d-A78A-D1F5659CBA93}\ShellFolder
	��Attributes�ļ�ֵ�� b084010c �޸�Ϊ b094010c

	���ء��⡱��Ŀ
	HKEY_CLASSES_ROOT\CLSID\{031E4825-7B94-4dc3-B131-E946B44C8DD5}\ShellFolder
	��Attributes�ļ�ֵ�� b080010d �޸�Ϊ b090010d

	���ء����硱��Ŀ
	HKEY_CLASSES_ROOT\CLSID\{F02C1A0D-BE21-4350-88B0-7367FC96EF3C}\ShellFolder
	��Attributes�ļ�ֵ�� b0040064 �޸�Ϊ b0940064

	���ء��ղؼС���Ŀ
	HKEY_CLASSES_ROOT\CLSID\{323CA680-C24D-4099-B94D-446DD2D7249E}\ShellFolder
	��Attributes�ļ�ֵ�� a0900100 �޸�Ϊ a9400100

	���� ���ٷ���
	HKEY_CLASSES_ROOT\CLSID\{679f85cb-0220-4080-b29b-5540cc05aab6}\ShellFolder
	"Attributes"=dword:a0600000

	���� OneDrive
	HKEY_CLASSES_ROOT\CLSID\{018D5C66-4533-4307-9B53-224DE2ED1FE6}\ShellFolder
	"FolderValueFlags"=dword:00000028
	"Attributes"=dword:f090004d
	**/

	LONG result[6];

	result[0] = WriteShellFolderRegKey(L"{B4FB3F98-C1EA-428d-A78A-D1F5659CBA93}", 0xb094010c); //��ͥ��
	result[1] = WriteShellFolderRegKey(L"{031E4825-7B94-4dc3-B131-E946B44C8DD5}", 0xb090010d); //��
	result[2] = WriteShellFolderRegKey(L"{F02C1A0D-BE21-4350-88B0-7367FC96EF3C}", 0xb0940064); //����
	result[3] = WriteShellFolderRegKey(L"{323CA680-C24D-4099-B94D-446DD2D7249E}", 0xa9400100); //�ղؼ�
	result[4] = WriteShellFolderRegKey(L"{679f85cb-0220-4080-b29b-5540cc05aab6}", 0xa0600000); //���ٷ���
	result[5] = WriteShellFolderRegKey(L"{018D5C66-4533-4307-9B53-224DE2ED1FE6}", 0xf090004d); //OneDrive


	return 0;

}

/**
* ��ԭ��ߵ����������ٷ��ʡ�������Ŀ
**/
LONG CRegEdit::ResetLibLink()
{
	LONG result[6];
	result[0] = WriteShellFolderRegKey(L"{B4FB3F98-C1EA-428d-A78A-D1F5659CBA93}", 0xb084010c); //��ͥ��
	result[1] = WriteShellFolderRegKey(L"{031E4825-7B94-4dc3-B131-E946B44C8DD5}", 0xb080010d); //��
	result[2] = WriteShellFolderRegKey(L"{F02C1A0D-BE21-4350-88B0-7367FC96EF3C}", 0xb0040064); //����
	result[3] = WriteShellFolderRegKey(L"{323CA680-C24D-4099-B94D-446DD2D7249E}", 0xa0900100); //�ղؼ�
	result[4] = WriteShellFolderRegKey(L"{679f85cb-0220-4080-b29b-5540cc05aab6}", 0xa0000000); //���ٷ���
	result[5] = WriteShellFolderRegKey(L"{018D5C66-4533-4307-9B53-224DE2ED1FE6}", 0xf0000000); //OneDrive


	return 0;

}

/**
* ������˼�������������������ɵķ�������ͼ��...��
**/
LONG CRegEdit::CleanProDir()
{
	//[HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\Explorer\MyComputer\NameSpace]
	int i = 0;
	long ret = 0;
	HKEY hkey;

	ret = RegOpenKeyEx(
						HKEY_LOCAL_MACHINE,
						L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\MyComputer\\NameSpace",
						0,
						KEY_READ | KEY_WOW64_64KEY,
						&hkey);

	if (ret == ERROR_SUCCESS)
	{
		TCHAR subkey_name[MAX_PATH] = { 0 };
		unsigned long subkey_len = sizeof(subkey_name);

		while (TRUE)
		{
			subkey_len = sizeof(subkey_name);

			ret = RegEnumKeyEx(hkey,
				i,
				subkey_name,
				&subkey_len,
				0,
				NULL,
				NULL,
				NULL);

			wstring tmpSubKey = (wstring)subkey_name;
			//printf("%ls\n", tmpSubKey.c_str());
			if (tmpSubKey.compare(L"DelegateFolders") != 0) {

				tmpSubKey = L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\MyComputer\\NameSpace\\" + tmpSubKey;
				//ret = RegDeleteKey(HKEY_LOCAL_MACHINE, tmpSubKey.c_str());
				ret = RegDeleteKeyEx(HKEY_LOCAL_MACHINE, tmpSubKey.c_str(), KEY_WOW64_64KEY,0 );
				printf("DeleteKey %ls %d\n", tmpSubKey.c_str(), ret);
			}

			if (ret != ERROR_SUCCESS)
			{
				break;
			}
			i++;
		}
	}

	RegCloseKey(hkey);

    //x32
	ret = RegOpenKeyEx(
		HKEY_LOCAL_MACHINE,
		L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\MyComputer\\NameSpace",
		0,
		KEY_READ | KEY_WOW64_32KEY,
		&hkey);

	if (ret == ERROR_SUCCESS)
	{
		TCHAR subkey_name[MAX_PATH] = { 0 };
		unsigned long subkey_len = sizeof(subkey_name);

		while (TRUE)
		{
			subkey_len = sizeof(subkey_name);

			ret = RegEnumKeyEx(hkey,
				i,
				subkey_name,
				&subkey_len,
				0,
				NULL,
				NULL,
				NULL);

			wstring tmpSubKey = (wstring)subkey_name;
			//printf("%ls\n", tmpSubKey.c_str());
			if (tmpSubKey.compare(L"DelegateFolders") != 0) {

				tmpSubKey = L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\MyComputer\\NameSpace\\" + tmpSubKey;
				//ret = RegDeleteKey(HKEY_LOCAL_MACHINE, tmpSubKey.c_str());
				ret = RegDeleteKeyEx(HKEY_LOCAL_MACHINE, tmpSubKey.c_str(), KEY_WOW64_32KEY, 0);
				printf("DeleteKey %ls %d\n", tmpSubKey.c_str(), ret);
			}

			if (ret != ERROR_SUCCESS)
			{
				break;
			}
			i++;
		}
	}

	RegCloseKey(hkey);



	return ret;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


LONG CRegEdit::SetfDisabledAllowList( DWORD value  )
{

	//HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Terminal Server\TSAppAllowList		
	LONG lResult = SetRegEdit( L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Terminal Server\\TSAppAllowList" ,
								 L"fDisabledAllowList",
								 REG_DWORD,
								 NULL,
								 value );
	
	return lResult;

}


LONG CRegEdit::SetfSingleSessionPerUser( DWORD value )
{

	/*
	��Զ����������û����Ƶ������ĻỰ��
	[HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\Terminal Server]
	"fSingleSessionPerUser" = dword:00000001
	*/

	/**
	*  ������Զ���û�ֻ����һ���Ự�У� 0
	*/	
	LONG lResult = SetRegEdit(L"SYSTEM\\CurrentControlSet\\Control\\Terminal Server",
								L"fSingleSessionPerUser",
								REG_DWORD,
								NULL,
								value );

		
	return lResult;
	
}

LONG CRegEdit::SetfSingleSessionPerUser2( DWORD value )
{

	/*
	��Զ����������û����Ƶ������ĻỰ�� == �޸������
	[HKEY_LOCAL_MACHINE\SOFTWARE\Policies\Microsoft\Windows NT\Terminal Services]
	"fSingleSessionPerUser" = dword:00000001	
	*/
		
	LONG lResult = SetComputerPolicies(L"SOFTWARE\\Policies\\Microsoft\\Windows NT\\Terminal Services",
								REG_DWORD,
								L"fSingleSessionPerUser",
								NULL,
								value);


	return lResult;

}

LONG CRegEdit::SetLicenseServers( wchar_t* server )
{
	
	LONG lResult = SetComputerPolicies( L"SOFTWARE\\Policies\\Microsoft\\Windows NT\\Terminal Services",
								REG_SZ,
								L"LicenseServers",
								server,
								0 );

	return lResult;

}

LONG CRegEdit::SetLicensingMode( DWORD mode )
{
	LONG lResult = SetComputerPolicies( L"SOFTWARE\\Policies\\Microsoft\\Windows NT\\Terminal Services",
								REG_DWORD,
								L"LicensingMode",
								NULL,
								mode );

	return lResult;

}

//HKEY_LOCAL_MACHINE\SOFTWARE\Policies\Microsoft\Windows NT\Terminal Services
LONG CRegEdit::GetLicenseServers(wchar_t* server)
{

	wchar_t LicenseServers[MAX_PATH] = { 0 };
	DWORD td = 0;
	LONG lResult = GetRegEdit( L"SOFTWARE\\Policies\\Microsoft\\Windows NT\\Terminal Services",
		        L"LicenseServers",
		        LicenseServers,
		        td );

	if ( lstrlen(LicenseServers) != 0 )
	{
		lstrcpy(server, LicenseServers);
	}
	

	return lResult;

}

LONG CRegEdit::SetNoDrives()
{
	LONG lResult = SetRegEdit(L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer",
		L"NoDrives",
		REG_DWORD,
		NULL,
		0x00ffffff);

	return lResult;

}

LONG CRegEdit::SetNoViewOnDrive()
{
	LONG lResult = SetRegEdit(L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer",
		L"NoViewOnDrive",
		REG_DWORD,
		NULL,
		0x00ffffff);

	return lResult;

}

LONG CRegEdit::ClearNoViewOnDrive()
{
	LONG lResult = SetRegEdit(L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer",
		L"NoViewOnDrive",
		REG_DWORD,
		NULL,
		0);

	return lResult;
}

LONG CRegEdit::ClearNoDrives()
{
	LONG lResult = SetRegEdit(L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer",
		L"NoDrives",
		REG_DWORD,
		NULL,
		0);

	return lResult;

}


LONG CRegEdit::SetUserAuthentication(DWORD mode)
{
	LONG lResult = SetComputerPolicies(L"SOFTWARE\\Policies\\Microsoft\\Windows NT\\Terminal Services",
		REG_DWORD,
		L"UserAuthentication",
		NULL,
		mode);

	return lResult;	  
}

LONG CRegEdit::SetRemoteAppLogoffTimeLimit(DWORD mode)
{
	LONG lResult = SetComputerPolicies(L"SOFTWARE\\Policies\\Microsoft\\Windows NT\\Terminal Services",
		REG_DWORD,
		L"MaxInstanceCount",
		NULL,
		mode);

	return lResult;
	
}

LONG CRegEdit::SetMaxInstanceCount(DWORD mode)
{
	LONG lResult = SetComputerPolicies(L"SOFTWARE\\Policies\\Microsoft\\Windows NT\\Terminal Services",
		REG_DWORD,
		L"MaxInstanceCount",
		NULL,
		mode);

	return lResult;

}

LONG CRegEdit::SetEnableRemoteFXAdvancedRemoteApp(DWORD mode)
{
	LONG lResult = SetComputerPolicies(L"SOFTWARE\\Policies\\Microsoft\\Windows NT\\Terminal Services",
		REG_DWORD,
		L"EnableRemoteFXAdvancedRemoteApp",
		NULL,
		mode);

	return lResult;

}


LONG CRegEdit::SetEnableLogonScriptDelay(DWORD mode)
{

	//HKEY_LOCAL_MACHINE\SOFTWARE\Policies\Microsoft\Windows\System\EnableLogonScriptDelay
	//DWORD: 0 (0)
	LONG lResult = SetComputerPolicies(L"SOFTWARE\\Policies\\Microsoft\\Windows\\System",
		REG_DWORD,
		L"EnableLogonScriptDelay",
		NULL,
		mode);

	return lResult;

}

LONG CRegEdit::SetNoControlPanel(DWORD mode)
{

	//�����\HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Policies\Explorer
	LONG lResult = SetUserPolicies(L"Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer",
		REG_DWORD,
		L"NoControlPanel",
		NULL,
		mode);

	return lResult;

}

LONG CRegEdit::SetExplorerWindows(DWORD mode)
{

	//�����\HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Policies\Explorer
	LONG lResult = SetUserPolicies(L"Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer",
		REG_DWORD,
		L"NoHardwareTab",
		NULL,
		mode);

	lResult = SetUserPolicies(L"Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer",
		REG_DWORD,
		L"NoNetConnectDisconnect",
		NULL,
		mode);

	lResult = SetUserPolicies(L"Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer",
		REG_DWORD,
		L"NoSecurityTab",
		NULL,
		mode);

	lResult = SetUserPolicies(L"Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer",
		REG_DWORD,
		L"NoDFSTab",
		NULL,
		mode);

	lResult = SetUserPolicies(L"Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer",
		REG_DWORD,
		L"NoCDBurning",
		NULL,
		mode);

	lResult = SetUserPolicies(L"Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer",
		REG_DWORD,
		L"NoFileMenu",
		NULL,
		mode);

	lResult = SetUserPolicies(L"Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer",
		REG_DWORD,
		L"NoShellSearchButton",
		NULL,
		mode);

	lResult = SetUserPolicies(L"Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer",
		REG_DWORD,
		L"NoManageMyComputerVerb",
		NULL,
		mode);

	/**
	* �ļ����������
	* ============================================ */
	lResult = SetUserPolicies(L"Software\\Policies\\Microsoft\\Windows\\Explorer",
		REG_DWORD,
		L"ExplorerRibbonStartsMinimized",
		NULL,
		mode);

	lResult = SetUserPolicies(L"Software\\Policies\\Microsoft\\Windows\\Explorer",
		REG_DWORD,
		L"NoSearchInternetTryHarderButton",
		NULL,
		mode);

	lResult = SetUserPolicies(L"Software\\Policies\\Microsoft\\Windows\\Explorer",
		REG_DWORD,
		L"DisableSearchBoxSuggestions",
		NULL,
		mode);

	/**
	*  ͨ�öԻ���
	* ============================================ */
	lResult = SetUserPolicies(L"Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Comdlg32",
		REG_DWORD,
		L"NoPlacesBar",
		NULL,
		mode);

	lResult = SetUserPolicies(L"Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Comdlg32",
		REG_DWORD,
		L"NoFileMru",
		NULL,
		mode);

	lResult = SetUserPolicies(L"Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Comdlg32",
		REG_DWORD,
		L"NoBackButton",
		NULL,
		mode);


	return lResult;

}


LONG CRegEdit::SetNoViewContextMenu(DWORD mode)
{

	//�����\HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Policies\Explorer
	LONG lResult = SetUserPolicies(L"Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer",
		REG_DWORD,
		L"NoViewContextMenu",
		NULL,
		mode);

	return lResult;

}

//
LONG CRegEdit::SetDisableCMD(DWORD mode)
{

	//�����\HKEY_CURRENT_USER\Software\Policies\Microsoft\Windows\System
	LONG lResult = SetUserPolicies(L"Software\\Policies\\Microsoft\\Windows\\System",
		REG_DWORD,
		L"DisableCMD",
		NULL,
		mode);

	return lResult;

}

LONG CRegEdit::SetKOneDrive(DWORD mode)
{
	//�����\HKEY_LOCAL_MACHINE\SOFTWARE\Policies\Microsoft\Windows\OneDrive
	//DisableFileSyncNGSC
	LONG lResult = SetComputerPolicies(L"SOFTWARE\\Policies\\Microsoft\\Windows\\OneDrive",
		REG_DWORD,
		L"DisableFileSyncNGSC",
		NULL,
		mode);

	return lResult;

}
//
LONG CRegEdit::SetCADKetCall(DWORD mode)
{

	//�����\HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Policies\System

	/**
	DisableTaskMgr
	DisableLockWorkstation
	DisableChangePassword
	**/
	LONG lResult = SetUserPolicies(L"Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System",
		REG_DWORD,
		L"DisableTaskMgr",
		NULL,
		mode);

	lResult = SetUserPolicies(L"Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System",
		REG_DWORD,
		L"DisableLockWorkstation",
		NULL,
		mode);

	lResult = SetUserPolicies(L"Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System",
		REG_DWORD,
		L"DisableChangePassword",
		NULL,
		mode);


	//�����\HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Policies\Explorer
	//NoLogoff
	lResult = SetUserPolicies(L"Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer",
		REG_DWORD,
		L"NoLogoff",
		NULL,
		mode);



	//HideFastUserSwitching
	//�����\HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\Policies\System
	lResult = SetComputerPolicies(L"Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System",
		REG_DWORD,
		L"HideFastUserSwitching",
		NULL,
		mode);

	//�����\HKEY_LOCAL_MACHINE\SOFTWARE\Policies\Microsoft\Windows\System
	//DontDisplayNetworkSelectionUI
	lResult = SetComputerPolicies(L"Software\\Policies\\Microsoft\\Windows\\System",
		REG_DWORD,
		L"DontDisplayNetworkSelectionUI",
		NULL,
		mode);

	return lResult;

}


LONG CRegEdit::SetLockScreenImage( CString bmpPath )
{

	//�����\HKEY_LOCAL_MACHINE\SOFTWARE\Policies\Microsoft\Windows\Personalization
	//LockScreenImage

	LONG lResult;

	
		CString tStr = bmpPath;
		
		lResult = SetComputerPolicies(L"SOFTWARE\\Policies\\Microsoft\\Windows\\Personalization",
			REG_SZ,
			L"LockScreenImage",
			tStr.GetBuffer(),
			0);

		tStr.ReleaseBuffer();


	return lResult;

}



LONG CRegEdit::DelAppInitDLLs()
{
	SetRegEdit(L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Windows",
		L"AppInit_DLLs",
		REG_SZ,
		L"",
		0);

	SetRegEdit(L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Windows",
		L"LoadAppInit_DLLs",
		REG_DWORD,
		NULL,
		0);

	SetRegEdit(L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Windows",
		L"RequireSignedAppInit_DLLs",
		REG_DWORD,
		NULL,
		1);

	/**
	* Win32 x86
	*/
	SetRegEditWin32(L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Windows",
		L"AppInit_DLLs",
		REG_SZ,
		L"",
		0);

	SetRegEditWin32(L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Windows",
		L"LoadAppInit_DLLs",
		REG_DWORD,
		NULL,
		0);

	SetRegEditWin32(L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Windows",
		L"RequireSignedAppInit_DLLs",
		REG_DWORD,
		NULL,
		1);


	return 0;
}

LONG CRegEdit::SetAppInitDLLs(CString filePath_x86, CString filePath_x64)
{
	/**
	�����\HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Windows
	�����\HKEY_LOCAL_MACHINE\SOFTWARE\WOW6432Node\Microsoft\Windows NT\CurrentVersion\Windows
	AppInit_DLLs
	LoadAppInit_DLLs = 1
	RequireSignedAppInit_DLLs = 0
	**/
	
	SetRegEdit( L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Windows",
		L"AppInit_DLLs",
		REG_SZ,
		filePath_x64.GetBuffer(),
		0 );

	SetRegEdit(L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Windows",
		L"LoadAppInit_DLLs",
		REG_DWORD,
		NULL,
		1);

	SetRegEdit(L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Windows",
		L"RequireSignedAppInit_DLLs",
		REG_DWORD,
		NULL,
		0);

	/**
	* Win32 x86
	*/
	SetRegEditWin32(L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Windows",
		L"AppInit_DLLs",
		REG_SZ,
		filePath_x86.GetBuffer(),
		0);

	SetRegEditWin32(L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Windows",
		L"LoadAppInit_DLLs",
		REG_DWORD,
		NULL,
		1);

	SetRegEditWin32(L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Windows",
		L"RequireSignedAppInit_DLLs",
		REG_DWORD,
		NULL,
		0);



	filePath_x86.ReleaseBuffer();
	filePath_x64.ReleaseBuffer();

	return 0;
}

vector<CString> CRegEdit::GetAppInitDLLs()
{
	vector<CString> tScriptFile;
		
	wchar_t dllx64file[MAX_PATH] = { 0 };
	wchar_t dllx86file[MAX_PATH] = { 0 };
	DWORD d = 0;

	GetRegEdit(L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Windows",
		L"AppInit_DLLs",		
		dllx64file,
		d);

	GetRegEditWin32(L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Windows",
		L"AppInit_DLLs",
		dllx86file,
		d);

/**
	if (lstrlen(dllx64file) == 0)
	{
		lstrcpy( dllx64file, L"C:\\VECloudApp\\ProcessGuard\\ProcessGuard_x64.dll" );
	}

	if (lstrlen(dllx86file) == 0)
	{
		lstrcpy(dllx86file, L"C:\\VECloudApp\\ProcessGuard\\ProcessGuard_Win32.dll");
	}
**/
	tScriptFile.push_back(dllx86file);
	tScriptFile.push_back(dllx64file);

	return tScriptFile;
}

LONG CRegEdit::DelScriptsStartup0()
{
	//C:\Windows\System32\GroupPolicy\User\Scripts\scripts.ini
	wchar_t *scriptfile = L"C:\\Windows\\sysnative\\GroupPolicy\\User\\Scripts\\scripts.ini";
	::WritePrivateProfileString(L"Logon", L"1CmdLine", NULL, scriptfile);
	::WritePrivateProfileString(L"Logon", L"1Parameters", NULL, scriptfile);
	::WritePrivateProfileString(L"Logon", L"2CmdLine", NULL, scriptfile);
	::WritePrivateProfileString(L"Logon", L"2Parameters", NULL, scriptfile);


	//[HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Group Policy\Scripts\Logon\0\0]

	WCHAR *GPO_REG = L"Software\\Microsoft\\Windows\\CurrentVersion\\Group Policy\\Scripts\\Logon\\0\\0";
	RegDeleteKeyEx(HKEY_LOCAL_MACHINE, GPO_REG, KEY_WOW64_64KEY, 0);

	GPO_REG = L"Software\\Microsoft\\Windows\\CurrentVersion\\Group Policy\\Scripts\\Logon\\0\\1";
	RegDeleteKeyEx(HKEY_CURRENT_USER, GPO_REG, KEY_WOW64_64KEY, 0);

	GPO_REG = L"Software\\Microsoft\\Windows\\CurrentVersion\\Group Policy\\Scripts\\Logon\\0\\2";
	RegDeleteKeyEx(HKEY_CURRENT_USER, GPO_REG, KEY_WOW64_64KEY, 0);

	return 0;
}

LONG CRegEdit::SetScriptsStartup0(CString filePath_x86, CString filePath_x64)
{

/**
	[HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\Group Policy\Scripts\Startup\0]
	"GPO-ID"="LocalGPO"
	"SOM-ID"="Local"
	"FileSysPath"="C:\\WINDOWS\\System32\\GroupPolicy\\Machine"
	"DisplayName"="���������"
	"GPOName"="���������"
	"PSScriptOrder"=dword:00000001

	[HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\Group Policy\Scripts\Startup\0\0]
	"Script"=""
	"Parameters"=""
	"IsPowershell"=dword:00000000
	"ExecTime"=hex(b):00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00
**/

	::CoInitialize(NULL);

	HKEY hSubKey;

	WCHAR *GPO_REG = L"Software\\Microsoft\\Windows\\CurrentVersion\\Group Policy\\Scripts\\Startup\\0";

	LONG lResult = RegCreateKeyEx(HKEY_LOCAL_MACHINE,
		GPO_REG,
		0,
		NULL,
		REG_OPTION_NON_VOLATILE,
		KEY_ALL_ACCESS | KEY_WOW64_64KEY,
		NULL,
		&hSubKey,
		NULL);

	wchar_t *GPOID = L"LocalGPO";
	wchar_t *SOMID = L"Local";
	wchar_t *FileSysPath = L"C:\\WINDOWS\\System32\\GroupPolicy\\Machine";
	wchar_t *DisplayName = L"���������";
	wchar_t *GPOName = L"���������";
	DWORD PSScriptOrder = 1;

	lResult = RegSetValueEx(hSubKey, L"GPO-ID", 0, REG_SZ, (const BYTE *)GPOID, wcslen(GPOID) * sizeof(wchar_t));
	lResult = RegSetValueEx(hSubKey, L"SOM-ID", 0, REG_SZ, (const BYTE *)SOMID, wcslen(SOMID) * sizeof(wchar_t));
	lResult = RegSetValueEx(hSubKey, L"FileSysPath", 0, REG_SZ, (const BYTE *)FileSysPath, wcslen(FileSysPath) * sizeof(wchar_t));
	lResult = RegSetValueEx(hSubKey, L"DisplayName", 0, REG_SZ, (const BYTE *)DisplayName, wcslen(DisplayName) * sizeof(wchar_t));
	lResult = RegSetValueEx(hSubKey, L"GPOName", 0, REG_SZ, (const BYTE *)GPOName, wcslen(GPOName) * sizeof(wchar_t));
	lResult = RegSetValueEx(hSubKey, L"PSScriptOrder", 0, REG_DWORD, (const BYTE *)&PSScriptOrder, sizeof(DWORD));
	
	RegCloseKey(hSubKey);

	GPO_REG = L"Software\\Microsoft\\Windows\\CurrentVersion\\Group Policy\\Scripts\\Startup\\0\\0";
	lResult = RegCreateKeyEx(
		HKEY_LOCAL_MACHINE,
		GPO_REG,
		0,
		NULL,
		REG_OPTION_NON_VOLATILE,
		KEY_ALL_ACCESS | KEY_WOW64_64KEY,
		NULL,
		&hSubKey,
		NULL);

	wchar_t *Script = filePath_x64.GetBuffer();
	wchar_t *Parameters = L"";
	DWORD IsPowershell = 0;
	QWORD ExecTime = 0;

	lResult = RegSetValueEx(hSubKey, L"Script", 0, REG_SZ, (const BYTE *)Script, wcslen(Script) * sizeof(wchar_t));
	lResult = RegSetValueEx(hSubKey, L"Parameters", 0, REG_SZ, (const BYTE *)Parameters, wcslen(Parameters) * sizeof(wchar_t));
	lResult = RegSetValueEx(hSubKey, L"IsPowershell", 0, REG_DWORD, (const BYTE *)&IsPowershell, sizeof(DWORD));
	lResult = RegSetValueEx(hSubKey, L"ExecTime", 0, REG_QWORD, (const BYTE *)&ExecTime, sizeof(QWORD));

	RegCloseKey(hSubKey);
	
	GPO_REG = L"Software\\Microsoft\\Windows\\CurrentVersion\\Group Policy\\Scripts\\Startup\\0\\1";
	lResult = RegCreateKeyEx(
		HKEY_LOCAL_MACHINE,
		GPO_REG,
		0,
		NULL,
		REG_OPTION_NON_VOLATILE,
		KEY_ALL_ACCESS | KEY_WOW64_64KEY,
		NULL,
		&hSubKey,
		NULL);

	wchar_t *Script1 = filePath_x86.GetBuffer();
	wchar_t *Parameters1 = L"";
	IsPowershell = 0;
	ExecTime = 0;

	lResult = RegSetValueEx(hSubKey, L"Script", 0, REG_SZ, (const BYTE *)Script1, wcslen(Script1) * sizeof(wchar_t));
	lResult = RegSetValueEx(hSubKey, L"Parameters", 0, REG_SZ, (const BYTE *)Parameters1, wcslen(Parameters1) * sizeof(wchar_t));
	lResult = RegSetValueEx(hSubKey, L"IsPowershell", 0, REG_DWORD, (const BYTE *)&IsPowershell, sizeof(DWORD));
	lResult = RegSetValueEx(hSubKey, L"ExecTime", 0, REG_QWORD, (const BYTE *)&ExecTime, sizeof(QWORD));

	RegCloseKey(hSubKey);

	/**
	C:\Windows\System32\GroupPolicy\Machine\Scripts\scripts.ini
	[Startup]
	0CmdLine=
	0Parameters=
	**/

	/**
	*
	* 32 λ����ֱ�ӷ��� system32 �е��ļ���Ĭ������
	* ʹ�ã� sysnative �Զ�ת��
	* ============================================= */
	wchar_t *scriptfile = L"C:\\Windows\\sysnative\\GroupPolicy\\Machine\\Scripts\\scripts.ini";
	::WritePrivateProfileString(L"Startup", L"0CmdLine", Script, scriptfile);
	::WritePrivateProfileString(L"Startup", L"0Parameters", Parameters, scriptfile);

	::WritePrivateProfileString(L"Startup", L"1CmdLine", Script1, scriptfile);
	::WritePrivateProfileString(L"Startup", L"1Parameters", Parameters1, scriptfile);


	filePath_x64.ReleaseBuffer();
	filePath_x86.ReleaseBuffer();


	::CoUninitialize();

	return lResult;

}

vector<CString> CRegEdit::GetScriptsStartup0()
{
	//�����\HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\Group Policy\Scripts\Startup
	vector<CString> tScriptFile;

	wchar_t exex64file[MAX_PATH] = { 0 };
	wchar_t exex86file[MAX_PATH] = { 0 };
	DWORD d = 0;

	//Software\\Microsoft\\Windows\\CurrentVersion\\Group Policy\\Scripts\\Startup\\0\\1
	GetRegEdit(L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Group Policy\\Scripts\\Startup\\0\\0",
		L"Script",
		exex64file,
		d);

	GetRegEdit(L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Group Policy\\Scripts\\Startup\\0\\1",
		L"Script",
		exex86file,
		d);


/**
	if (lstrlen(exex64file) == 0)
	{
		lstrcpy(exex64file, L"C:\\VECloudApp\\ProcessGuard\\AutoStartProcessGuardHook.exe");
	}

	if (lstrlen(exex86file) == 0)
	{
		lstrcpy(exex86file, L"C:\\VECloudApp\\ProcessGuard\\AutoStartProcessGuardHookx86.exe");
	}
**/

	tScriptFile.push_back(exex86file);
	tScriptFile.push_back(exex64file);
	
	return tScriptFile;
}

LONG CRegEdit::SetScriptsLogon0( CString filePath   )
{
		
	return SetScriptsLogon(filePath, 0);

}

CString CRegEdit::GetScriptsLogon0()
{

	wchar_t *scriptfile = L"C:\\Windows\\sysnative\\GroupPolicy\\User\\Scripts\\scripts.ini";
	wchar_t filename[MAX_PATH] = { 0 };
	::GetPrivateProfileString(L"Logon", L"0Parameters", L"", filename, MAX_PATH, scriptfile);

	CString rStr( filename );

	return rStr;
}

CString CRegEdit::GetScriptsLogon( int id )
{

	wchar_t *scriptfile = L"C:\\Windows\\sysnative\\GroupPolicy\\User\\Scripts\\scripts.ini";
	wchar_t filename[MAX_PATH] = { 0 };

	CString parameters;
	parameters.Format(L"%dParameters", id );

	::GetPrivateProfileString(L"Logon", parameters, L"", filename, MAX_PATH, scriptfile);

	CString rStr(filename);

	return rStr;
}


LONG CRegEdit::SetScriptsLogon(CString filePath ,int id )
{
	/**
	[HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Group Policy\Scripts\Logon]

	[HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Group Policy\Scripts\Logon\0]
	"GPO-ID"="LocalGPO"
	"SOM-ID"="Local"
	"FileSysPath"="C:\\WINDOWS\\System32\\GroupPolicy\\User"
	"DisplayName"="���������"
	"GPOName"="���������"
	"PSScriptOrder"=dword:00000001

	[HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Group Policy\Scripts\Logon\0\0]
	"Script"="0000"
	"Parameters"="147258369000"
	"IsPowershell"=dword:00000000
	"ExecTime"=hex(b):00,00,00,00,00,00,00,00,00,00,00,00,00,00,00,00
	**/

	::CoInitialize(NULL);

	HKEY hSubKey;		
	WCHAR *GPO_REG = L"Software\\Microsoft\\Windows\\CurrentVersion\\Group Policy\\Scripts\\Logon\\0";
	
	LONG lResult = RegCreateKeyEx(HKEY_CURRENT_USER,
		GPO_REG,
		0,
		NULL,
		REG_OPTION_NON_VOLATILE,
		KEY_ALL_ACCESS | KEY_WOW64_64KEY,
		NULL,
		&hSubKey,
		NULL);

	wchar_t *GPOID = L"LocalGPO";
	wchar_t *SOMID = L"Local";
	wchar_t *FileSysPath = L"C:\\WINDOWS\\System32\\GroupPolicy\\User";
	wchar_t *DisplayName = L"���������";
	wchar_t *GPOName = L"���������";
	DWORD PSScriptOrder = 1;

	lResult = RegSetValueEx(hSubKey, L"GPO-ID", 0, REG_SZ, (const BYTE *)GPOID, wcslen(GPOID) * sizeof(wchar_t));
	lResult = RegSetValueEx(hSubKey, L"SOM-ID", 0, REG_SZ, (const BYTE *)SOMID, wcslen(SOMID) * sizeof(wchar_t));
	lResult = RegSetValueEx(hSubKey, L"FileSysPath", 0, REG_SZ, (const BYTE *)FileSysPath, wcslen(FileSysPath) * sizeof(wchar_t));
	lResult = RegSetValueEx(hSubKey, L"DisplayName", 0, REG_SZ, (const BYTE *)DisplayName, wcslen(DisplayName) * sizeof(wchar_t));
	lResult = RegSetValueEx(hSubKey, L"GPOName", 0, REG_SZ, (const BYTE *)GPOName, wcslen(GPOName) * sizeof(wchar_t));
	lResult = RegSetValueEx(hSubKey, L"PSScriptOrder", 0, REG_DWORD, (const BYTE *)&PSScriptOrder, sizeof(DWORD));
	
	RegCloseKey(hSubKey);
	
	WCHAR GPO_REG_Z[MAX_PATH] = { 0 };
	wsprintf( GPO_REG_Z, L"%ls\\%d", GPO_REG, id );
	//"Software\\Microsoft\\Windows\\CurrentVersion\\Group Policy\\Scripts\\Logon\\0\\1";
	
	lResult = RegCreateKeyEx(
		HKEY_CURRENT_USER,
		GPO_REG_Z,
		0,
		NULL,
		REG_OPTION_NON_VOLATILE,
		KEY_ALL_ACCESS | KEY_WOW64_64KEY,
		NULL,
		&hSubKey,
		NULL);

	wchar_t *Script = L"explorer.exe";
	wchar_t *Parameters = filePath.GetBuffer();
	DWORD IsPowershell = 0;
	QWORD ExecTime = 0;

	lResult = RegSetValueEx(hSubKey, L"Script", 0, REG_SZ, (const BYTE *)Script, wcslen(Script) * sizeof(wchar_t));
	lResult = RegSetValueEx(hSubKey, L"Parameters", 0, REG_SZ, (const BYTE *)Parameters, wcslen(Parameters) * sizeof(wchar_t));
	lResult = RegSetValueEx(hSubKey, L"IsPowershell", 0, REG_DWORD, (const BYTE *)&IsPowershell, sizeof(DWORD));
	lResult = RegSetValueEx(hSubKey, L"ExecTime", 0, REG_QWORD, (const BYTE *)&ExecTime, sizeof(QWORD));

	RegCloseKey(hSubKey);

	/**
	C:\Windows\System32\GroupPolicy\User\Scripts\scripts.ini
	[Logoff]
	0CmdLine = 000
	0Parameters = 000
	[Logon]
	0CmdLine = AAAA
	0Parameters = BBBBB
	**/

	/**
	*
	* 32 λ����ֱ�ӷ��� system32 �е��ļ���Ĭ������
	* ʹ�ã� sysnative �Զ�ת��
	* ============================================= */
	wchar_t *scriptfile = L"C:\\Windows\\sysnative\\GroupPolicy\\User\\Scripts\\scripts.ini";

	CString CmdLine;
	CmdLine.Format( L"%dCmdLine",id );

	CString _Parameters;
	_Parameters.Format(L"%dParameters", id);

	::WritePrivateProfileString(L"Logon", CmdLine, Script, scriptfile);
	
	::WritePrivateProfileString(L"Logon", _Parameters, Parameters, scriptfile);


	filePath.ReleaseBuffer();

	::CoUninitialize();

	return lResult;

}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

BOOL CRegEdit::GetNoViewOnDrive()
{
	BOOL lResult = FALSE;

	DWORD tmp  = 0;
	GetRegEdit(L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer",
		L"NoViewOnDrive",		
		NULL,
		tmp );

	if ( tmp != 0 ) 
	{

		lResult = TRUE;
	}

	return lResult;
}

BOOL CRegEdit::GetNoDrives()
{

	BOOL lResult = FALSE;

	DWORD tmp = 0;
	GetRegEdit(L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer",
		L"NoDrives",		
		NULL,
		tmp );

	if (tmp != 0)
	{

		lResult = TRUE;
	}

	return lResult;

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

BOOL CRegEdit::GetfSingleSessionPerUser()
{

	/*
	��Զ����������û����Ƶ������ĻỰ��
	[HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\Terminal Server]
	"fSingleSessionPerUser" = dword:00000001
	*/

	/**
	*  ������Զ���û�ֻ����һ���Ự�У� 0
	*/

	BOOL lResult = FALSE;
	DWORD t = 0;
	GetRegEdit(L"SYSTEM\\CurrentControlSet\\Control\\Terminal Server",
		L"fSingleSessionPerUser",		
		NULL,
		t);

	if ( t != 0 )
	{
		lResult = TRUE;
	}
	else
	{
		lResult = FALSE;
	}

	return lResult;

}

BOOL CRegEdit::GetfDisabledAllowList()
{

	//HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Terminal Server\TSAppAllowList		
	BOOL lResult = FALSE;
	DWORD t = 0;
	GetRegEdit(L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Terminal Server\\TSAppAllowList",
		L"fDisabledAllowList",
		NULL,
		t);

	if ( t != 0)
	{
		lResult = TRUE;
	}
	else
	{
		lResult = FALSE;
	}

	return lResult;
}

LONG CRegEdit::GetUserAuthentication()
{

	BOOL lResult = FALSE;
	DWORD t = 0;
	GetRegEdit(L"SOFTWARE\\Policies\\Microsoft\\Windows NT\\Terminal Services",
		L"UserAuthentication",
		NULL,
		t);

	if (t != 0)
	{
		lResult = TRUE;
	}
	else
	{
		lResult = FALSE;
	}

	return lResult;
	
}


LONG CRegEdit::GetRemoteAppLogoffTimeLimit()
{

	BOOL lResult = FALSE;
	DWORD t = 0;
	GetRegEdit(L"SOFTWARE\\Policies\\Microsoft\\Windows NT\\Terminal Services",
		L"RemoteAppLogoffTimeLimit",
		NULL,
		t);

	if (t != 0)
	{
		lResult = TRUE;
	}
	else
	{
		lResult = FALSE;
	}

	return lResult;

}

LONG CRegEdit::GetMaxInstanceCount()
{

	BOOL lResult = FALSE;
	DWORD tCount = 0;
	GetRegEdit(L"SOFTWARE\\Policies\\Microsoft\\Windows NT\\Terminal Services",
		L"MaxInstanceCount",
		NULL,
		tCount );

	return tCount;

}

LONG CRegEdit::GetEnableRemoteFXAdvancedRemoteApp()
{

	BOOL lResult = FALSE;
	DWORD t = 0;
	GetRegEdit(L"SOFTWARE\\Policies\\Microsoft\\Windows NT\\Terminal Services",
		L"EnableRemoteFXAdvancedRemoteApp",
		NULL,
		t);

	if (t != 0)
	{
		lResult = TRUE;
	}
	else
	{
		lResult = FALSE;
	}

	return lResult;

}

BOOL CRegEdit::GetNoControlPanel()
{

	//Software\Microsoft\Windows\CurrentVersion\Policies\Explorer	
	BOOL lResult = FALSE;
	DWORD t = 0;
	GetRegEdit( L"Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer",
		L"NoControlPanel",
		NULL,
		t,
		HKEY_CURRENT_USER);

	if ( t == 1 )
	{
		lResult = TRUE;
	}
	else
	{
		lResult = FALSE;
	}

	return lResult;
}

BOOL CRegEdit::GetEnableLogonScriptDelay()
{

	BOOL lResult = FALSE;
	DWORD t = 0;
	lResult = GetRegEdit(L"SOFTWARE\\Policies\\Microsoft\\Windows\\System",
		L"EnableLogonScriptDelay",
		NULL,
		t);

	if (t != 0)
	{
		lResult = TRUE;
	}
	else
	{
		lResult = FALSE;
	}

	return lResult;

}
 
BOOL CRegEdit::GetLibLink()
{
	wchar_t idbuf[256] = { 0 };
		
	HKEY hResult = NULL;
	DWORD dwDisposition = 0;

	wsprintf(idbuf, L"CLSID\\%ls\\ShellFolder", L"{F02C1A0D-BE21-4350-88B0-7367FC96EF3C}");

	LONG lResult = RegCreateKeyEx(
		HKEY_CLASSES_ROOT,
		idbuf,
		0,
		NULL,
		REG_OPTION_BACKUP_RESTORE,
		KEY_ALL_ACCESS | KEY_WOW64_64KEY,
		NULL,
		&hResult,
		&dwDisposition);

	if (lResult != ERROR_SUCCESS)
	{
		return lResult;
	}

	DWORD dwType = 0;
	DWORD value = 0;
	DWORD vsize = sizeof( DWORD );

	lResult = RegQueryValueEx(hResult, L"Attributes", NULL, &dwType, (BYTE*)&value, &vsize);

	BOOL bResult = FALSE;
	if(  value == 0xb0040064 )
	{
		bResult = FALSE;
	}
	else
	{
		bResult = TRUE;
	}

	return bResult;
	//0904631744
}

BOOL CRegEdit::GetThisPCPolicy()
{
	//�����\HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\Explorer\FolderDescriptions\{B4BFCC3A-DB2C-424C-B029-7FE99A87C641}\PropertyBag

	wchar_t idbuf[256] = { 0 };	
	
	HKEY hResult = NULL;
	DWORD dwDisposition = 0;

	wsprintf(idbuf, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\FolderDescriptions\\%ls\\PropertyBag", L"{B4BFCC3A-DB2C-424C-B029-7FE99A87C641}");
		
	LONG lResult = RegCreateKeyEx(
									HKEY_LOCAL_MACHINE,
									idbuf,
									0,
									NULL,
									REG_OPTION_BACKUP_RESTORE,
									KEY_ALL_ACCESS | KEY_WOW64_64KEY,
									NULL,
									&hResult,
									&dwDisposition);

	if (lResult != ERROR_SUCCESS)
	{
		return lResult;
	}

	wchar_t strVal[MAX_PATH] = {0};
	DWORD vsize = MAX_PATH;
	DWORD dwType = 0;
	lResult = RegQueryValueEx(hResult, L"ThisPCPolicy", NULL, &dwType, (BYTE*)strVal, &vsize);

	BOOL bResult = FALSE;

	if ( lstrcmp(L"Hide", strVal) == 0 )
	{
		bResult = TRUE;
	}
	else
	{
		bResult = FALSE;
	}
	
	return bResult;

}

BOOL CRegEdit::GetNoViewContextMenu()
{

	//�����\HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Policies\Explorer
	BOOL lResult = FALSE;

	DWORD tmp = 0;
	GetRegEdit(L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer",
		L"NoViewContextMenu",
		NULL,
		tmp, HKEY_CURRENT_USER);

	if (tmp != 0)
	{

		lResult = TRUE;
	}

	return lResult;
}

LONG CRegEdit::GetExplorerWindows()
{	

	BOOL lResult = FALSE;

	DWORD tmp = 0;
	GetRegEdit(L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer",
		L"NoHardwareTab",
		NULL,
		tmp);

	if (tmp != 0)
	{
		lResult = TRUE;
	}

	return lResult;
}

LONG CRegEdit::GetDisableCMD()
{

	//�����\HKEY_CURRENT_USER\Software\Policies\Microsoft\Windows\System
	BOOL lResult = FALSE;

	DWORD tmp = 0;
	GetRegEdit(L"Software\\Policies\\Microsoft\\Windows\\System",		
		L"DisableCMD",
		NULL,
		tmp, HKEY_CURRENT_USER);

	if (tmp != 0)
	{
		lResult = TRUE;
	}


	return lResult;

}

LONG CRegEdit::GetCADKetCall()
{

	//�����\HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Policies\System
	/**
	DisableTaskMgr
	DisableLockWorkstation
	DisableChangePassword
	**/
	BOOL lResult = FALSE;

	DWORD tmp = 0;
	GetRegEdit(L"Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System",
		L"DisableTaskMgr",
		NULL,
		tmp, HKEY_CURRENT_USER);

	if (tmp != 0)
	{
		lResult = TRUE;
	}


	return lResult;

}


LONG CRegEdit::GetLockScreenImage( __out wchar_t* getbmpPath  )
{

	//�����\HKEY_LOCAL_MACHINE\SOFTWARE\Policies\Microsoft\Windows\Personalization
	//LockScreenImage

	DWORD tmp = 0;
	LONG lResult = GetRegEdit(L"SOFTWARE\\Policies\\Microsoft\\Windows\\Personalization",
		L"LockScreenImage",
		getbmpPath,
		tmp, 
		HKEY_LOCAL_MACHINE);


	return lResult;

}

LONG CRegEdit::GetKOneDrive()
{
	//�����\HKEY_LOCAL_MACHINE\SOFTWARE\Policies\Microsoft\Windows\OneDrive
	//DisableFileSyncNGSC
	DWORD tmp = 0;
	LONG lResult = GetRegEdit(L"SOFTWARE\\Policies\\Microsoft\\Windows\\OneDrive",
		L"DisableFileSyncNGSC",
		NULL,
		tmp,
		HKEY_LOCAL_MACHINE);

	return tmp;

}



LONG CRegEdit::SetTEST()
{

	//wchar_t idbuf[128] = { 0 };
	//wchar_t *CLSID = wCLSID;

	//wchar_t strVal[] = L"";
	//DWORD value = wValue;

	HKEY hResult = NULL;
	DWORD dwDisposition = 0;

	//wsprintf(idbuf, L"file\\CLSID", CLSID);

	LONG lResult = RegCreateKeyEx(
		HKEY_CLASSES_ROOT,
		L"CLSID\\{E436EBB6-524F-11CE-9F53-0020AF0BA770}\\InprocServer32",
		0,
		NULL,
		REG_OPTION_BACKUP_RESTORE,
		KEY_ALL_ACCESS | KEY_WOW64_64KEY,
		NULL,
		&hResult,
		&dwDisposition);

	if (lResult != ERROR_SUCCESS)
	{
		return lResult;
	}

		
	wchar_t strVal[] = L"C:\\Windows\\System32\\quartz.dll.00";
	lResult = RegSetValueEx(hResult, L"", NULL, REG_SZ, (LPBYTE)strVal, sizeof(strVal));
	
	RegCloseKey(hResult);

	//x32
	lResult = RegCreateKeyEx(
		HKEY_CLASSES_ROOT,
		L"CLSID\\{E436EBB6-524F-11CE-9F53-0020AF0BA770}\\InprocServer32", 
		0,
		NULL,
		REG_OPTION_BACKUP_RESTORE,
		KEY_ALL_ACCESS | KEY_WOW64_32KEY,
		NULL,
		&hResult,
		&dwDisposition);

	if (lResult != ERROR_SUCCESS)
	{
		return lResult;
	}


	lResult = RegSetValueEx(hResult, L"", NULL, REG_SZ, (LPBYTE)strVal, sizeof(strVal));

	RegCloseKey(hResult);


	return lResult;

}



/***

�����\HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Policies\Explorer

NoViewContextMenu
NoHardwareTab
NoNetConnectDisconnect
NoSecurityTab
NoDFSTab
NoCDBurning
NoFileMenu
NoShellSearchButton
NoManageMyComputerVerb

�����\HKEY_CURRENT_USER\Software\Policies\Microsoft\Windows\Explorer
ExplorerRibbonStartsMinimized
NoSearchInternetTryHarderButton
DisableSearchBoxSuggestions

�����\HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Policies\Comdlg32
NoPlacesBar
NoFileMru
NoBackButton

**/