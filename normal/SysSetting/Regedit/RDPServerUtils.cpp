#define   INITGUID 
#include"RDPServerUtils.h"
#include <string>
#include <InitGuid.h>
#include <GPEdit.h>
#include <Guiddef.h>

#pragma comment(lib, "Gpedit.lib")

using namespace std;


DWORD RDPServerUtils::GetRDPPort()
{

	DWORD rdpPort = 0;
	GetRegEdit(L"System\\CurrentControlSet\\Control\\Terminal Server\\Wds\\rdpwd\\Tds\\Tcp",
		L"PortNumber",
		NULL,
		rdpPort);

	return rdpPort;
}

LONG RDPServerUtils::SetRDPPort(int rdpPort)
{

	LONG ret = SetRegEdit(L"System\\CurrentControlSet\\Control\\Terminal Server\\Wds\\rdpwd\\Tds\\Tcp",
		L"PortNumber",
		REG_DWORD,
		NULL,
		rdpPort);

	ret = SetRegEdit(L"SYSTEM\\CurrentControlSet\\Control\\Terminal Server\\WinStations\\RDP-Tcp",
		L"PortNumber",
		REG_DWORD,
		NULL,
		rdpPort);


	return ret;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


LONG RDPServerUtils::WriteShellFolderRegKey(wchar_t* wCLSID, DWORD wValue)
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

LONG RDPServerUtils::WriteThisPCPolicyRegKey(wchar_t* wCLSID, wchar_t* wValue)
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

LONG RDPServerUtils::SetRegEdit(wchar_t *lpSubKey, wchar_t* lpValueName, DWORD dwType, wchar_t* szValue, DWORD dValue)
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

LONG RDPServerUtils::SetRegEditWin32(wchar_t *lpSubKey, wchar_t* lpValueName, DWORD dwType, wchar_t* szValue, DWORD dValue)
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


LONG RDPServerUtils::GetRegEdit(wchar_t *lpSubKey, wchar_t* lpValueName, wchar_t* szValue, DWORD& dValue, HKEY MinhKey)
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

LONG RDPServerUtils::GetRegEditWin32(wchar_t *lpSubKey, wchar_t* lpValueName, wchar_t* szValue, DWORD& dValue, HKEY MinhKey)
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


LONG RDPServerUtils::SetComputerPolicies(wchar_t* gpoKey, DWORD dwType, wchar_t* name, wchar_t* valstr, DWORD valdword)
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

LONG RDPServerUtils::SetUserPolicies(wchar_t* gpoKey, DWORD dwType, wchar_t* name, wchar_t* valstr, DWORD valdword)
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
* 隐藏【此计算机】与【侧边导航栏】 多余项目
**/
LONG RDPServerUtils::HideThisPCPolicy()
{
	/**
	HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\Explorer\FolderDescriptions
	图片文件夹：{0ddd015d-b06c-45d5-8c4c-f59713854639}
	视频文件夹：{35286a68-3c57-41a1-bbb1-0eae73d76c95}
	下载文件夹：{7d83ee9b-2244-4e70-b1f5-5393042af1e4}
	音乐文件夹：{a0c69a99-21c8-4671-8703-7934162fcf1d}
	桌面文件夹：{b4bfcc3a-db2c-424c-b029-7fe99a87c641}
	文档文件夹：{f42ee2d3-909f-4907-8871-4c22fc0bf756}
	3D对象文件夹：{31c0dd25-9439-4f12-bf41-7ff4eda38722}
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
* 还原【此计算机】与【侧边导航栏】 多余项目
**/
LONG RDPServerUtils::ShowThisPCPolicy()
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
* 隐藏侧边导航栏【快速访问】多余项目
**/
LONG RDPServerUtils::CleanLibLink()
{
	/**
	隐藏“家庭组“项目
	HKEY_CLASSES_ROOT\CLSID\{B4FB3F98-C1EA-428d-A78A-D1F5659CBA93}\ShellFolder
	将Attributes的键值由 b084010c 修改为 b094010c

	隐藏“库”项目
	HKEY_CLASSES_ROOT\CLSID\{031E4825-7B94-4dc3-B131-E946B44C8DD5}\ShellFolder
	将Attributes的键值由 b080010d 修改为 b090010d

	隐藏“网络”项目
	HKEY_CLASSES_ROOT\CLSID\{F02C1A0D-BE21-4350-88B0-7367FC96EF3C}\ShellFolder
	将Attributes的键值由 b0040064 修改为 b0940064

	隐藏“收藏夹”项目
	HKEY_CLASSES_ROOT\CLSID\{323CA680-C24D-4099-B94D-446DD2D7249E}\ShellFolder
	将Attributes的键值由 a0900100 修改为 a9400100

	隐藏 快速访问
	HKEY_CLASSES_ROOT\CLSID\{679f85cb-0220-4080-b29b-5540cc05aab6}\ShellFolder
	"Attributes"=dword:a0600000

	隐藏 OneDrive
	HKEY_CLASSES_ROOT\CLSID\{018D5C66-4533-4307-9B53-224DE2ED1FE6}\ShellFolder
	"FolderValueFlags"=dword:00000028
	"Attributes"=dword:f090004d
	**/

	LONG result[6];

	result[0] = WriteShellFolderRegKey(L"{B4FB3F98-C1EA-428d-A78A-D1F5659CBA93}", 0xb094010c); //家庭组
	result[1] = WriteShellFolderRegKey(L"{031E4825-7B94-4dc3-B131-E946B44C8DD5}", 0xb090010d); //库
	result[2] = WriteShellFolderRegKey(L"{F02C1A0D-BE21-4350-88B0-7367FC96EF3C}", 0xb0940064); //网络
	result[3] = WriteShellFolderRegKey(L"{323CA680-C24D-4099-B94D-446DD2D7249E}", 0xa9400100); //收藏夹
	result[4] = WriteShellFolderRegKey(L"{679f85cb-0220-4080-b29b-5540cc05aab6}", 0xa0600000); //快速访问
	result[5] = WriteShellFolderRegKey(L"{018D5C66-4533-4307-9B53-224DE2ED1FE6}", 0xf090004d); //OneDrive


	return 0;

}

/**
* 还原侧边导航栏【快速访问】多余项目
**/
LONG RDPServerUtils::ResetLibLink()
{
	LONG result[6];
	result[0] = WriteShellFolderRegKey(L"{B4FB3F98-C1EA-428d-A78A-D1F5659CBA93}", 0xb084010c); //家庭组
	result[1] = WriteShellFolderRegKey(L"{031E4825-7B94-4dc3-B131-E946B44C8DD5}", 0xb080010d); //库
	result[2] = WriteShellFolderRegKey(L"{F02C1A0D-BE21-4350-88B0-7367FC96EF3C}", 0xb0040064); //网络
	result[3] = WriteShellFolderRegKey(L"{323CA680-C24D-4099-B94D-446DD2D7249E}", 0xa0900100); //收藏夹
	result[4] = WriteShellFolderRegKey(L"{679f85cb-0220-4080-b29b-5540cc05aab6}", 0xa0000000); //快速访问
	result[5] = WriteShellFolderRegKey(L"{018D5C66-4533-4307-9B53-224DE2ED1FE6}", 0xf0000000); //OneDrive


	return 0;

}

/**
* 清除【此计算机】中其他程序生成的访问链接图标...：
**/
LONG RDPServerUtils::CleanProDir()
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
				ret = RegDeleteKeyEx(HKEY_LOCAL_MACHINE, tmpSubKey.c_str(), KEY_WOW64_64KEY, 0);
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


LONG RDPServerUtils::SetfDisabledAllowList(DWORD value)
{

	//HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Terminal Server\TSAppAllowList		
	LONG lResult = SetRegEdit(L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Terminal Server\\TSAppAllowList",
		L"fDisabledAllowList",
		REG_DWORD,
		NULL,
		value);

	return lResult;

}


LONG RDPServerUtils::SetfSingleSessionPerUser(DWORD value)
{

	/*
	将远程桌面服务用户限制到单独的会话中
	[HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\Terminal Server]
	"fSingleSessionPerUser" = dword:00000001
	*/

	/**
	*  不限制远程用户只能在一个会话中： 0
	*/
	LONG lResult = SetRegEdit(L"SYSTEM\\CurrentControlSet\\Control\\Terminal Server",
		L"fSingleSessionPerUser",
		REG_DWORD,
		NULL,
		value);


	return lResult;

}

LONG RDPServerUtils::SetfSingleSessionPerUser2(DWORD value)
{

	/*
	将远程桌面服务用户限制到单独的会话中 == 修改组策略
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

LONG RDPServerUtils::SetLicenseServers(wchar_t* server)
{

	LONG lResult = SetComputerPolicies(L"SOFTWARE\\Policies\\Microsoft\\Windows NT\\Terminal Services",
		REG_SZ,
		L"LicenseServers",
		server,
		0);

	return lResult;

}

LONG RDPServerUtils::SetLicensingMode(DWORD mode)
{
	LONG lResult = SetComputerPolicies(L"SOFTWARE\\Policies\\Microsoft\\Windows NT\\Terminal Services",
		REG_DWORD,
		L"LicensingMode",
		NULL,
		mode);

	return lResult;

}

//HKEY_LOCAL_MACHINE\SOFTWARE\Policies\Microsoft\Windows NT\Terminal Services
LONG RDPServerUtils::GetLicenseServers(wchar_t* server)
{

	wchar_t LicenseServers[MAX_PATH] = { 0 };
	DWORD td = 0;
	LONG lResult = GetRegEdit(L"SOFTWARE\\Policies\\Microsoft\\Windows NT\\Terminal Services",
		L"LicenseServers",
		LicenseServers,
		td);

	if (lstrlen(LicenseServers) != 0)
	{
		lstrcpy(server, LicenseServers);
	}


	return lResult;

}

LONG RDPServerUtils::SetNoDrives()
{
	LONG lResult = SetRegEdit(L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer",
		L"NoDrives",
		REG_DWORD,
		NULL,
		0x00ffffff);

	return lResult;

}

LONG RDPServerUtils::SetNoViewOnDrive()
{
	LONG lResult = SetRegEdit(L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer",
		L"NoViewOnDrive",
		REG_DWORD,
		NULL,
		0x00ffffff);

	return lResult;

}

LONG RDPServerUtils::ClearNoViewOnDrive()
{
	LONG lResult = SetRegEdit(L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer",
		L"NoViewOnDrive",
		REG_DWORD,
		NULL,
		0);

	return lResult;
}

LONG RDPServerUtils::ClearNoDrives()
{
	LONG lResult = SetRegEdit(L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer",
		L"NoDrives",
		REG_DWORD,
		NULL,
		0);

	return lResult;

}


LONG RDPServerUtils::SetUserAuthentication(DWORD mode)
{
	LONG lResult = SetComputerPolicies(L"SOFTWARE\\Policies\\Microsoft\\Windows NT\\Terminal Services",
		REG_DWORD,
		L"UserAuthentication",
		NULL,
		mode);

	return lResult;
}

LONG RDPServerUtils::SetRemoteAppLogoffTimeLimit(DWORD mode)
{
	LONG lResult = SetComputerPolicies(L"SOFTWARE\\Policies\\Microsoft\\Windows NT\\Terminal Services",
		REG_DWORD,
		L"MaxInstanceCount",
		NULL,
		mode);

	return lResult;

}

LONG RDPServerUtils::SetMaxInstanceCount(DWORD mode)
{
	LONG lResult = SetComputerPolicies(L"SOFTWARE\\Policies\\Microsoft\\Windows NT\\Terminal Services",
		REG_DWORD,
		L"MaxInstanceCount",
		NULL,
		mode);

	return lResult;

}

LONG RDPServerUtils::SetEnableRemoteFXAdvancedRemoteApp(DWORD mode)
{
	LONG lResult = SetComputerPolicies(L"SOFTWARE\\Policies\\Microsoft\\Windows NT\\Terminal Services",
		REG_DWORD,
		L"EnableRemoteFXAdvancedRemoteApp",
		NULL,
		mode);

	return lResult;

}


LONG RDPServerUtils::SetEnableLogonScriptDelay(DWORD mode)
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

LONG RDPServerUtils::SetNoControlPanel(DWORD mode)
{

	//计算机\HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Policies\Explorer
	LONG lResult = SetUserPolicies(L"Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer",
		REG_DWORD,
		L"NoControlPanel",
		NULL,
		mode);

	return lResult;

}

LONG RDPServerUtils::SetExplorerWindows(DWORD mode)
{

	//计算机\HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Policies\Explorer
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
	* 文件管理器外壳
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
	*  通用对话框
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


LONG RDPServerUtils::SetNoViewContextMenu(DWORD mode)
{

	//计算机\HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Policies\Explorer
	LONG lResult = SetUserPolicies(L"Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer",
		REG_DWORD,
		L"NoViewContextMenu",
		NULL,
		mode);

	return lResult;

}

//
LONG RDPServerUtils::SetDisableCMD(DWORD mode)
{

	//计算机\HKEY_CURRENT_USER\Software\Policies\Microsoft\Windows\System
	LONG lResult = SetUserPolicies(L"Software\\Policies\\Microsoft\\Windows\\System",
		REG_DWORD,
		L"DisableCMD",
		NULL,
		mode);

	return lResult;

}

LONG RDPServerUtils::SetKOneDrive(DWORD mode)
{
	//计算机\HKEY_LOCAL_MACHINE\SOFTWARE\Policies\Microsoft\Windows\OneDrive
	//DisableFileSyncNGSC
	LONG lResult = SetComputerPolicies(L"SOFTWARE\\Policies\\Microsoft\\Windows\\OneDrive",
		REG_DWORD,
		L"DisableFileSyncNGSC",
		NULL,
		mode);

	return lResult;

}
//
LONG RDPServerUtils::SetCADKetCall(DWORD mode)
{

	//计算机\HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Policies\System

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


	//计算机\HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Policies\Explorer
	//NoLogoff
	lResult = SetUserPolicies(L"Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer",
		REG_DWORD,
		L"NoLogoff",
		NULL,
		mode);



	//HideFastUserSwitching
	//计算机\HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\Policies\System
	lResult = SetComputerPolicies(L"Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System",
		REG_DWORD,
		L"HideFastUserSwitching",
		NULL,
		mode);

	//计算机\HKEY_LOCAL_MACHINE\SOFTWARE\Policies\Microsoft\Windows\System
	//DontDisplayNetworkSelectionUI
	lResult = SetComputerPolicies(L"Software\\Policies\\Microsoft\\Windows\\System",
		REG_DWORD,
		L"DontDisplayNetworkSelectionUI",
		NULL,
		mode);

	return lResult;

}


LONG RDPServerUtils::SetLockScreenImage(const wstring& bmpPath)
{

	//计算机\HKEY_LOCAL_MACHINE\SOFTWARE\Policies\Microsoft\Windows\Personalization
	//LockScreenImage

	LONG lResult;

	TCHAR tStr[1024] = { 0 };
	lstrcpy(tStr, bmpPath.c_str());

	lResult = SetComputerPolicies(L"SOFTWARE\\Policies\\Microsoft\\Windows\\Personalization",
		REG_SZ,
		L"LockScreenImage",
		tStr,0);



	return lResult;

}



LONG RDPServerUtils::DelAppInitDLLs()
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

LONG RDPServerUtils::SetAppInitDLLs(const wstring& filePath_x86, const wstring& filePath_x64)
{
	/**
	计算机\HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Windows
	计算机\HKEY_LOCAL_MACHINE\SOFTWARE\WOW6432Node\Microsoft\Windows NT\CurrentVersion\Windows
	AppInit_DLLs
	LoadAppInit_DLLs = 1
	RequireSignedAppInit_DLLs = 0
	**/

	TCHAR tFilePath[MAX_PATH] = { 0 };
	lstrcpy(tFilePath, filePath_x64.c_str());
	SetRegEdit(L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Windows",
		L"AppInit_DLLs",
		REG_SZ,
		tFilePath,
		0);

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
	lstrcpy(tFilePath, filePath_x86.c_str());
	SetRegEditWin32(L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Windows",
		L"AppInit_DLLs",
		REG_SZ,
		tFilePath,
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



	return 0;
}

vector<wstring> RDPServerUtils::GetAppInitDLLs()
{
	vector<wstring> tScriptFile;

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

LONG RDPServerUtils::DelScriptsStartup0()
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

LONG RDPServerUtils::SetScriptsStartup0(const wstring& filePath_x86, const wstring& filePath_x64)
{

	/**
	[HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\Group Policy\Scripts\Startup\0]
	"GPO-ID"="LocalGPO"
	"SOM-ID"="Local"
	"FileSysPath"="C:\\WINDOWS\\System32\\GroupPolicy\\Machine"
	"DisplayName"="本地组策略"
	"GPOName"="本地组策略"
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
	wchar_t *DisplayName = L"本地组策略";
	wchar_t *GPOName = L"本地组策略";
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

	const wchar_t *Script = filePath_x64.c_str();
	wchar_t *Parameters = L"";
	DWORD IsPowershell = 0;
	WORD ExecTime = 0;

	lResult = RegSetValueEx(hSubKey, L"Script", 0, REG_SZ, (const BYTE *)Script, wcslen(Script) * sizeof(wchar_t));
	lResult = RegSetValueEx(hSubKey, L"Parameters", 0, REG_SZ, (const BYTE *)Parameters, wcslen(Parameters) * sizeof(wchar_t));
	lResult = RegSetValueEx(hSubKey, L"IsPowershell", 0, REG_DWORD, (const BYTE *)&IsPowershell, sizeof(DWORD));
	lResult = RegSetValueEx(hSubKey, L"ExecTime", 0, REG_QWORD, (const BYTE *)&ExecTime, sizeof(WORD));

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

	const wchar_t *Script1 = filePath_x86.c_str();
	wchar_t *Parameters1 = L"";
	IsPowershell = 0;
	ExecTime = 0;

	lResult = RegSetValueEx(hSubKey, L"Script", 0, REG_SZ, (const BYTE *)Script1, wcslen(Script1) * sizeof(wchar_t));
	lResult = RegSetValueEx(hSubKey, L"Parameters", 0, REG_SZ, (const BYTE *)Parameters1, wcslen(Parameters1) * sizeof(wchar_t));
	lResult = RegSetValueEx(hSubKey, L"IsPowershell", 0, REG_DWORD, (const BYTE *)&IsPowershell, sizeof(DWORD));
	lResult = RegSetValueEx(hSubKey, L"ExecTime", 0, REG_QWORD, (const BYTE *)&ExecTime, sizeof(WORD));

	RegCloseKey(hSubKey);

	/**
	C:\Windows\System32\GroupPolicy\Machine\Scripts\scripts.ini
	[Startup]
	0CmdLine=
	0Parameters=
	**/

	/**
	*
	* 32 位程序直接访问 system32 中的文件会默认重向定
	* 使用： sysnative 自动转跳
	* ============================================= */
	wchar_t *scriptfile = L"C:\\Windows\\sysnative\\GroupPolicy\\Machine\\Scripts\\scripts.ini";
	::WritePrivateProfileString(L"Startup", L"0CmdLine", Script, scriptfile);
	::WritePrivateProfileString(L"Startup", L"0Parameters", Parameters, scriptfile);

	::WritePrivateProfileString(L"Startup", L"1CmdLine", Script1, scriptfile);
	::WritePrivateProfileString(L"Startup", L"1Parameters", Parameters1, scriptfile);



	::CoUninitialize();

	return lResult;

}

vector<wstring> RDPServerUtils::GetScriptsStartup0()
{
	//计算机\HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\Group Policy\Scripts\Startup
	vector<wstring> tScriptFile;

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

LONG RDPServerUtils::SetScriptsLogon0(const wstring& filePath)
{

	return SetScriptsLogon(filePath, 0);

}

wstring RDPServerUtils::GetScriptsLogon0()
{

	wchar_t *scriptfile = L"C:\\Windows\\sysnative\\GroupPolicy\\User\\Scripts\\scripts.ini";
	wchar_t filename[MAX_PATH] = { 0 };
	::GetPrivateProfileString(L"Logon", L"0Parameters", L"", filename, MAX_PATH, scriptfile);

	wstring rStr(filename);

	return rStr;
}
wstring RDPServerUtils::GetScriptsLogon(int id)
{

	wchar_t *scriptfile = L"C:\\Windows\\sysnative\\GroupPolicy\\User\\Scripts\\scripts.ini";
	wchar_t filename[MAX_PATH] = { 0 };

	TCHAR parameters[1024] = { 0 };
	wsprintf(parameters,L"%dParameters", id);

	::GetPrivateProfileString(L"Logon", parameters, L"", filename, MAX_PATH, scriptfile);

	wstring rStr(filename);

	return rStr;
}


LONG RDPServerUtils::SetScriptsLogon(const wstring& filePath, int id)
{
	/**
	[HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Group Policy\Scripts\Logon]

	[HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Group Policy\Scripts\Logon\0]
	"GPO-ID"="LocalGPO"
	"SOM-ID"="Local"
	"FileSysPath"="C:\\WINDOWS\\System32\\GroupPolicy\\User"
	"DisplayName"="本地组策略"
	"GPOName"="本地组策略"
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
	wchar_t *DisplayName = L"本地组策略";
	wchar_t *GPOName = L"本地组策略";
	DWORD PSScriptOrder = 1;

	lResult = RegSetValueEx(hSubKey, L"GPO-ID", 0, REG_SZ, (const BYTE *)GPOID, wcslen(GPOID) * sizeof(wchar_t));
	lResult = RegSetValueEx(hSubKey, L"SOM-ID", 0, REG_SZ, (const BYTE *)SOMID, wcslen(SOMID) * sizeof(wchar_t));
	lResult = RegSetValueEx(hSubKey, L"FileSysPath", 0, REG_SZ, (const BYTE *)FileSysPath, wcslen(FileSysPath) * sizeof(wchar_t));
	lResult = RegSetValueEx(hSubKey, L"DisplayName", 0, REG_SZ, (const BYTE *)DisplayName, wcslen(DisplayName) * sizeof(wchar_t));
	lResult = RegSetValueEx(hSubKey, L"GPOName", 0, REG_SZ, (const BYTE *)GPOName, wcslen(GPOName) * sizeof(wchar_t));
	lResult = RegSetValueEx(hSubKey, L"PSScriptOrder", 0, REG_DWORD, (const BYTE *)&PSScriptOrder, sizeof(DWORD));

	RegCloseKey(hSubKey);

	WCHAR GPO_REG_Z[MAX_PATH] = { 0 };
	wsprintf(GPO_REG_Z, L"%ls\\%d", GPO_REG, id);
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
	const wchar_t *Parameters = filePath.c_str();
	DWORD IsPowershell = 0;
	WORD ExecTime = 0;

	lResult = RegSetValueEx(hSubKey, L"Script", 0, REG_SZ, (const BYTE *)Script, wcslen(Script) * sizeof(wchar_t));
	lResult = RegSetValueEx(hSubKey, L"Parameters", 0, REG_SZ, (const BYTE *)Parameters, wcslen(Parameters) * sizeof(wchar_t));
	lResult = RegSetValueEx(hSubKey, L"IsPowershell", 0, REG_DWORD, (const BYTE *)&IsPowershell, sizeof(DWORD));
	lResult = RegSetValueEx(hSubKey, L"ExecTime", 0, REG_QWORD, (const BYTE *)&ExecTime, sizeof(WORD));

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
	* 32 位程序直接访问 system32 中的文件会默认重向定
	* 使用： sysnative 自动转跳
	* ============================================= */
	wchar_t *scriptfile = L"C:\\Windows\\sysnative\\GroupPolicy\\User\\Scripts\\scripts.ini";

	TCHAR CmdLine[1024] = { 0 };
	wsprintf(CmdLine, L"%dCmdLine", id);

	TCHAR _Parameters[1024] = { 0 };
	wsprintf(_Parameters,L"%dParameters", id);

	::WritePrivateProfileString(L"Logon", CmdLine, Script, scriptfile);

	::WritePrivateProfileString(L"Logon", _Parameters, Parameters, scriptfile);


	::CoUninitialize();

	return lResult;

}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

BOOL RDPServerUtils::GetNoViewOnDrive()
{
	BOOL lResult = FALSE;

	DWORD tmp = 0;
	GetRegEdit(L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer",
		L"NoViewOnDrive",
		NULL,
		tmp);

	if (tmp != 0)
	{

		lResult = TRUE;
	}

	return lResult;
}

BOOL RDPServerUtils::GetNoDrives()
{

	BOOL lResult = FALSE;

	DWORD tmp = 0;
	GetRegEdit(L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer",
		L"NoDrives",
		NULL,
		tmp);

	if (tmp != 0)
	{

		lResult = TRUE;
	}

	return lResult;

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

BOOL RDPServerUtils::GetfSingleSessionPerUser()
{

	/*
	将远程桌面服务用户限制到单独的会话中
	[HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\Terminal Server]
	"fSingleSessionPerUser" = dword:00000001
	*/

	/**
	*  不限制远程用户只能在一个会话中： 0
	*/

	BOOL lResult = FALSE;
	DWORD t = 0;
	GetRegEdit(L"SYSTEM\\CurrentControlSet\\Control\\Terminal Server",
		L"fSingleSessionPerUser",
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

BOOL RDPServerUtils::GetfDisabledAllowList()
{

	//HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Terminal Server\TSAppAllowList		
	BOOL lResult = FALSE;
	DWORD t = 0;
	GetRegEdit(L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Terminal Server\\TSAppAllowList",
		L"fDisabledAllowList",
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

LONG RDPServerUtils::GetUserAuthentication()
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


LONG RDPServerUtils::GetRemoteAppLogoffTimeLimit()
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

LONG RDPServerUtils::GetMaxInstanceCount()
{

	BOOL lResult = FALSE;
	DWORD tCount = 0;
	GetRegEdit(L"SOFTWARE\\Policies\\Microsoft\\Windows NT\\Terminal Services",
		L"MaxInstanceCount",
		NULL,
		tCount);

	return tCount;

}

LONG RDPServerUtils::GetEnableRemoteFXAdvancedRemoteApp()
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

BOOL RDPServerUtils::GetNoControlPanel()
{

	//Software\Microsoft\Windows\CurrentVersion\Policies\Explorer	
	BOOL lResult = FALSE;
	DWORD t = 0;
	GetRegEdit(L"Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer",
		L"NoControlPanel",
		NULL,
		t,
		HKEY_CURRENT_USER);

	if (t == 1)
	{
		lResult = TRUE;
	}
	else
	{
		lResult = FALSE;
	}

	return lResult;
}

BOOL RDPServerUtils::GetEnableLogonScriptDelay()
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

BOOL RDPServerUtils::GetLibLink()
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
	DWORD vsize = sizeof(DWORD);

	lResult = RegQueryValueEx(hResult, L"Attributes", NULL, &dwType, (BYTE*)&value, &vsize);

	BOOL bResult = FALSE;
	if (value == 0xb0040064)
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

BOOL RDPServerUtils::GetThisPCPolicy()
{
	//计算机\HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\Explorer\FolderDescriptions\{B4BFCC3A-DB2C-424C-B029-7FE99A87C641}\PropertyBag

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

	wchar_t strVal[MAX_PATH] = { 0 };
	DWORD vsize = MAX_PATH;
	DWORD dwType = 0;
	lResult = RegQueryValueEx(hResult, L"ThisPCPolicy", NULL, &dwType, (BYTE*)strVal, &vsize);

	BOOL bResult = FALSE;

	if (lstrcmp(L"Hide", strVal) == 0)
	{
		bResult = TRUE;
	}
	else
	{
		bResult = FALSE;
	}

	return bResult;

}

BOOL RDPServerUtils::GetNoViewContextMenu()
{

	//计算机\HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Policies\Explorer
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

LONG RDPServerUtils::GetExplorerWindows()
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

LONG RDPServerUtils::GetDisableCMD()
{

	//计算机\HKEY_CURRENT_USER\Software\Policies\Microsoft\Windows\System
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

LONG RDPServerUtils::GetCADKetCall()
{

	//计算机\HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Policies\System
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


LONG RDPServerUtils::GetLockScreenImage(__out wchar_t* getbmpPath)
{

	//计算机\HKEY_LOCAL_MACHINE\SOFTWARE\Policies\Microsoft\Windows\Personalization
	//LockScreenImage

	DWORD tmp = 0;
	LONG lResult = GetRegEdit(L"SOFTWARE\\Policies\\Microsoft\\Windows\\Personalization",
		L"LockScreenImage",
		getbmpPath,
		tmp,
		HKEY_LOCAL_MACHINE);


	return lResult;

}

LONG RDPServerUtils::GetKOneDrive()
{
	//计算机\HKEY_LOCAL_MACHINE\SOFTWARE\Policies\Microsoft\Windows\OneDrive
	//DisableFileSyncNGSC
	DWORD tmp = 0;
	LONG lResult = GetRegEdit(L"SOFTWARE\\Policies\\Microsoft\\Windows\\OneDrive",
		L"DisableFileSyncNGSC",
		NULL,
		tmp,
		HKEY_LOCAL_MACHINE);

	return tmp;

}



LONG RDPServerUtils::SetTEST()
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

计算机\HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Policies\Explorer

NoViewContextMenu
NoHardwareTab
NoNetConnectDisconnect
NoSecurityTab
NoDFSTab
NoCDBurning
NoFileMenu
NoShellSearchButton
NoManageMyComputerVerb

计算机\HKEY_CURRENT_USER\Software\Policies\Microsoft\Windows\Explorer
ExplorerRibbonStartsMinimized
NoSearchInternetTryHarderButton
DisableSearchBoxSuggestions

计算机\HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Policies\Comdlg32
NoPlacesBar
NoFileMru
NoBackButton

**/