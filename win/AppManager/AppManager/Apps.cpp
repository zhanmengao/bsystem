#include"Apps.h"
#include<tchar.h>
#include<stdio.h>
#include <shobjidl.h>
#include <ShlObj.h>
#include <atlstr.h>
#include <msi.h>
#include"../../../../C++space/Convert/Convert/Convert.h"
#include"../../../../C++space/StringTools/StringTools/WinStrUtils.h"
#pragma comment(lib, "msi.lib")
#define APP_KEY _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall"
int AppManager::GetSoftListInfo(vector<RegApplication>& vec)
{
	const int  MAX_LEG = 256 * sizeof(TCHAR);
	HKEY hKey;
	DWORD dwRegNum = MAX_LEG;
	TCHAR regBufferName[MAX_LEG] = { 0 };
	LSTATUS ret;
	ret = RegOpenKeyEx(HKEY_LOCAL_MACHINE, APP_KEY), NULL, KEY_READ, &hKey);
	if (ret == ERROR_SUCCESS)
	{
		ret = RegQueryInfoKey(hKey, NULL, NULL, NULL, &dwRegNum, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
		if (ret == ERROR_SUCCESS)
		{
			for (ULONG i = 0; i < dwRegNum; i++)
			{
				DWORD dwRegSize = MAX_LEG;
				RegEnumKeyEx(hKey, i, regBufferName, &dwRegSize, NULL, NULL, NULL, NULL);
				DWORD dwType;
				HKEY hSubKey;
				if (RegOpenKeyEx(hKey, regBufferName, NULL, KEY_READ, &hSubKey) == ERROR_SUCCESS)
				{
					TCHAR DisplayName[MAX_LEG] = { 0 };
					dwRegSize = MAX_LEG;
					ZeroMemory(DisplayName, MAX_LEG);
					RegQueryValueEx(hSubKey, _T("DisplayName"), 0, &dwType, (LPBYTE)DisplayName, &dwRegSize);


					TCHAR DisplayVersion[MAX_LEG] = { 0 };
					dwRegSize = MAX_LEG;
					ZeroMemory(DisplayVersion, MAX_LEG);
					RegQueryValueEx(hSubKey, _T("DisplayVersion"), 0, &dwType, (LPBYTE)DisplayVersion, &dwRegSize);

					TCHAR Publisher[MAX_LEG] = { 0 };
					dwRegSize = MAX_LEG;
					ZeroMemory(Publisher, MAX_LEG);
					RegQueryValueEx(hSubKey, _T("Publisher"), 0, &dwType, (LPBYTE)Publisher, &dwRegSize);

					TCHAR InstallDate[MAX_LEG] = { 0 };
					dwRegSize = MAX_LEG;
					ZeroMemory(InstallDate, MAX_LEG);
					RegQueryValueEx(hSubKey, _T("InstallDate"), 0, &dwType, (LPBYTE)InstallDate, &dwRegSize);

					if (hSubKey)
					{
						RegCloseKey(hSubKey);
						hSubKey = NULL;
					}

					RegApplication app;
					app.DisplayName = DisplayName;
					app.DisplayVersion = DisplayVersion;
					app.Publisher = Publisher;
					app.InstallDate = InstallDate;
					if (app.DisplayName.empty())
					{
						continue;
					}
					vec.push_back(app);				
				}
			}
		}
	}
	if (hKey != NULL)
	{
		RegCloseKey(hKey);
		hKey = NULL;
	}
	return ret;
}

DWORD AppManager::GetInstallAppList(set<Application>& s)
{
	s.clear();
	TCHAR strPath[MAX_PATH] = { 0 };
	TCHAR strDesktop[MAX_PATH] = { 0 };

	if (ExpandEnvironmentStrings(_T("%PROGRAMDATA%\\Microsoft\\Windows\\Start Menu\\Programs"),
		strPath, MAX_PATH) == 0)
	{
		return GetLastError();
	}
	_tcscpy_s(strDesktop, MAX_PATH, _T("C:\\Users\\Administrator\\Desktop"));
	EnumShellFolder(strPath, s, 3);
	EnumShellFolder(strDesktop, s, 3);
	return 0;
}
bool AppManager::EnumShellFolder(TCHAR*  strSrc, set<Application>& s, int flag)
{
	TCHAR strWildcard[MAX_PATH] = { '\0' };
	lstrcpy(strWildcard, strSrc);
	lstrcat(strWildcard, _T("\\*"));

	HANDLE hFind = NULL;
	WIN32_FIND_DATA findData;
	hFind = FindFirstFile(strWildcard, &findData);

	if (INVALID_HANDLE_VALUE == hFind)
		return false;

	do
	{
		TCHAR SrcFilePath[MAX_PATH] = { '\0' };

		_tcscpy_s(SrcFilePath, MAX_PATH, strSrc);
		_tcscat_s(SrcFilePath, _T("\\"));
		_tcscat_s(SrcFilePath, findData.cFileName);

		//如果是目录，向下递归
		if ((findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			if (!flag)
			{
				continue;
			}
			if (_tcsicmp(findData.cFileName, _T(".")) == 0 || _tcsicmp(findData.cFileName, _T("..")) == 0)
			{
				continue;
			}

			EnumShellFolder(SrcFilePath, s, flag - 1);
		}
		//检查是否是exe
		else
		{
			TCHAR PathBuf[MAX_PATH] = { 0 };
			TCHAR RequiredCommandLineBuf[MAX_PATH] = { 0 };

			if (!GetShortCutFile(SrcFilePath, PathBuf, MAX_PATH, RequiredCommandLineBuf, MAX_PATH))
			{
				//转换失败
				continue;
			}

			if (is_exe_file(PathBuf))
			{
				TCHAR lname[MAX_PATH] = { 0 };
				TCHAR* pTemp = _tcsrchr(findData.cFileName, '.');

				if (pTemp != NULL)
				{
					_tcsncpy(lname, findData.cFileName, pTemp - findData.cFileName);
					
					if (WinStrUtils::Find(lname, _T("卸载")) || WinStrUtils::Findi(lname, _T("uninst")))
					{
						continue;
					}

					TCHAR AppPath[MAX_PATH] = { 0 };
					_tcscpy_s(AppPath, MAX_PATH, PathBuf);
					TCHAR* regName = PathFindFileName(PathBuf);
					PathRemoveExtension(regName);


					Application app;
					app.mName = lname;
					app.mRegName = regName;
					app.mPath = AppPath;
					app.mRequiredCommandLine = RequiredCommandLineBuf;
					s.insert(app);
				}
			}
		}
	} while (FindNextFile(hFind, &findData));

	FindClose(hFind);

	return true;
}

BOOL AppManager::GetShortCutFile(TCHAR* ShortcutFile, TCHAR* filePath, int sizefile, TCHAR* argv, int sizeargv)
{
	HRESULT           hres;
	IShellLink        *psl = NULL;
	IPersistFile      *ppf;
	WIN32_FIND_DATA   fd;

	CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);

	hres = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (void**)&psl);
	if (!SUCCEEDED(hres))
		return   FALSE;

	hres = psl->QueryInterface(IID_IPersistFile, (void**)&ppf);
	if (SUCCEEDED(hres))
	{
#ifndef _UNICODE
		wchar_t wsz[MAX_PATH] = { 0 };
		wcscpy_s(wsz, _countof(wsz), Convert::StringToWString(ShortcutFile).c_str());
		char usz[MAX_PATH] = { 0 };
		strcpy_s(usz, _countof(usz), ShortcutFile);
#else		
		wchar_t wsz[MAX_PATH];
		wcscpy_s(wsz, _countof(wsz), ShortcutFile);
#define usz wsz
#endif
		hres = ppf->Load(wsz, STGM_READ);

		if (SUCCEEDED(hres))
		{
			hres = psl->GetPath(filePath, sizefile, &fd, 0);
			stdstr wstr(filePath);
			for (auto it = wstr.begin(); it != wstr.end(); it++)
			{
				*it = toupper(*it);
			}
			//if (wcsstr(filePath, L"C:\\WINDOWS\\INSTALLER\\{") != NULL)
			if (wstr.find(_T("C:\\WINDOWS\\INSTALLER\\{")) != wstr.npos)
			{
				DWORD dwSize = MAX_PATH;
				return GetMsiShortCutTarget(usz, filePath, dwSize);
			}
			hres = psl->GetArguments(argv, sizeargv);
			LPITEMIDLIST pidl = NULL;
			psl->GetIDList(&pidl);
			SHGetPathFromIDList(pidl, filePath);
		}
		ppf->Release();
	}
	psl->Release();
	return SUCCEEDED(hres);
}

bool AppManager::is_exe_file(TCHAR* filename)
{
	TCHAR buffer[MAX_PATH] = { 0 };
	_tcscpy_s(buffer, _countof(buffer), filename);

	TCHAR* tmp = _tcsrchr(buffer, _T('.'));
	if (tmp == NULL)
	{
		return false;
	}
	tmp++;
	if (_tcsicmp(tmp, _T("exe")) != 0)
	{
		return false;
	}
	return true;
}


BOOL AppManager::GetMsiShortCutTarget(const TCHAR* strShortcutFile, TCHAR* strTargetFile, DWORD& nSize)
{
	if (!strShortcutFile || !strTargetFile)
		return FALSE;


	TCHAR szProductCode[39] = { 0 };
	TCHAR szComponentCode[39] = { 0 };
	if (S_OK == MsiGetShortcutTarget(strShortcutFile, szProductCode, NULL, szComponentCode))
	{
		INSTALLSTATE nState = MsiGetComponentPath(szProductCode, szComponentCode, strTargetFile, &nSize);
		if (nState == INSTALLSTATE_LOCAL || nState == INSTALLSTATE_SOURCE)
		{
			return TRUE;
		}
	}

	return FALSE;
}
/**
特殊处理取 office 快捷方式目标文件路径
C:\\Windows\\Installer\\{90150000-0011-0000-1000-0000000FF1CE}\\pptico.exe"
可遍历注册表（废弃）
[HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\App Paths\....]

从MSI 接口获取，理论上这个问题不止 office 有，可能所有采用MSI 安装包部署的应用程序都该问题
// MSDN 参考
https://docs.microsoft.com/zh-cn/windows/win32/msi/database-functions
https://docs.microsoft.com/zh-cn/windows/win32/api/msi/nf-msi-msigetshortcuttargeta?redirectedfrom=MSDN
https://docs.microsoft.com/zh-cn/windows/win32/api/msi/nf-msi-msigetcomponentpatha?redirectedfrom=MSDN
**/