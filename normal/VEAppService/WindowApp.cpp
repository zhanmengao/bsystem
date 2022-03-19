#include "WindowApp.h"

#include <Windows.h>
#include <shobjidl.h>
#include <ShlObj.h>
#include <atlstr.h>
#include "GStringClass.h"
#include <msi.h>
#pragma comment(lib, "msi.lib")

BOOL GetMsiShortCutTarget(LPCWSTR strShortcutFile, LPWSTR strTargetFile, DWORD& nSize);

WindowApp::WindowApp()
{
}


WindowApp::~WindowApp()
{
}

int WindowApp::GetInstallAppList(Json::Value &Items)
{

    wchar_t strPath[MAX_PATH] = { L'\0' };
	wchar_t strDesktop[MAX_PATH] = { L'\0' };
		
	ExpandEnvironmentStrings(
		L"%PROGRAMDATA%\\Microsoft\\Windows\\Start Menu\\Programs", 
		strPath, MAX_PATH);
	lstrcpy(strDesktop, L"C:\\Users\\Administrator\\Desktop");
	
	Json::Value _Items;

	EnumShellLink( strPath, _Items);
	EnumShellLink( strDesktop, _Items );

	Items = _Items;

	Json::FastWriter writer;
	std::string josnstr = writer.write(Items);
	printf("%s\n", josnstr.c_str());

	return _Items.size();
}

int WindowApp::GetVEAppList(Json::Value &Items)
{
	Json::Value _Items;
	ReadTSAppAllowList( _Items );

	Items = _Items;

	Json::FastWriter writer;
	std::string josnstr = writer.write(Items);
	printf("%s\n", josnstr.c_str());

	return _Items.size();
}

int WindowApp::AppendVEApp(Json::Value Items)
{
	
	return PutTSAppRegKey( Items );
}

int WindowApp::DeleteVEApp(Json::Value Items)
{
	
	return DelTSAppRegKey( Items );
}



/**
*
*
* ============================================================================================ */

bool WindowApp::ReadTSAppAllowList( Json::Value& items )
{
	
	const int  MAX_LEG = 256 * sizeof(TCHAR);
	HKEY hKey;
	DWORD dwRegNum = MAX_LEG;
	TCHAR regBufferName[MAX_LEG] = { 0 };

	if ( RegOpenKeyEx(
		       HKEY_LOCAL_MACHINE, 
		       _T("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Terminal Server\\TSAppAllowList\\Applications"),
		       NULL,
		       KEY_READ | KEY_WOW64_64KEY,
		       &hKey ) == ERROR_SUCCESS)
	{
		if (RegQueryInfoKey(hKey, NULL, NULL, NULL, &dwRegNum, NULL, NULL, NULL,
			NULL, NULL, NULL, NULL) == ERROR_SUCCESS)
		{

			Json::Value _items;

			for (ULONG i = 0; i < dwRegNum; i++)
			{

				Json::Value item;

				DWORD dwRegSize = MAX_LEG;				
				RegEnumKeyEx(hKey, i, regBufferName, &dwRegSize, NULL, NULL,
					NULL, NULL);

				DWORD dwType;
				HKEY hSubKey;
				if (RegOpenKeyEx(hKey, regBufferName, NULL, KEY_READ, &hSubKey) == ERROR_SUCCESS)
				{
					
					TCHAR regBufferValue[MAX_LEG] = { 0 };
					DWORD regIntegerValue;					
					item["RegName"] = GStringClass::UnicodeToUtf8(regBufferValue);

					dwRegSize = MAX_LEG;
					RegQueryValueEx(hSubKey, _T("Name"), 0, &dwType, (LPBYTE)regBufferValue, &dwRegSize);					
					item["Name"] = GStringClass::UnicodeToUtf8(regBufferValue);

					dwRegSize = MAX_LEG;
					_tcscpy(regBufferValue, L"");
					RegQueryValueEx(hSubKey, _T("Path"), 0, &dwType, (LPBYTE)regBufferValue, &dwRegSize);					
					item["Path"] = GStringClass::UnicodeToUtf8(regBufferValue);

					dwRegSize = 4;
					_tcscpy(regBufferValue, L"");
					RegQueryValueEx(hSubKey, _T("IconIndex"), 0, &dwType, (LPBYTE)&regIntegerValue, &dwRegSize);					
					item["IconIndex"] = (long)regIntegerValue;

					dwRegSize = MAX_LEG;
					_tcscpy(regBufferValue, L"");
					RegQueryValueEx(hSubKey, _T("IconPath"), 0, &dwType, (LPBYTE)regBufferValue, &dwRegSize);					
					item["IconPath"] = GStringClass::UnicodeToUtf8(regBufferValue);

					dwRegSize = 4;
					_tcscpy(regBufferValue, L"");
					RegQueryValueEx(hSubKey, _T("CommandLineSetting"), 0, &dwType, (LPBYTE)&regIntegerValue, &dwRegSize);					
					item["CommandLineSetting"] = (long)regIntegerValue;

					if (regIntegerValue)
					{
						dwRegSize = MAX_LEG;
						_tcscpy(regBufferValue, L"");
						RegQueryValueEx(hSubKey, _T("RequiredCommandLine"), 0, &dwType, (LPBYTE)regBufferValue, &dwRegSize);						
						item["RequiredCommandLine"] = GStringClass::UnicodeToUtf8(regBufferValue);
					}

					dwRegSize = 4;
					_tcscpy(regBufferValue, L"");
					RegQueryValueEx(hSubKey, _T("ShowInTSWA"), 0, &dwType, (LPBYTE)&regIntegerValue, &dwRegSize);					
					item["ShowInTSWA"] = (long)regIntegerValue;

					dwRegSize = MAX_LEG;
					_tcscpy(regBufferValue, L"");
					RegQueryValueEx(hSubKey, _T("VPath"), 0, &dwType, (LPBYTE)regBufferValue, &dwRegSize);					
					item["VPath"] = GStringClass::UnicodeToUtf8(regBufferValue);

					dwRegSize = MAX_LEG;
					_tcscpy(regBufferValue, L"");
					RegQueryValueEx(hSubKey, _T("SecurityDescriptor"), 0, &dwType, (LPBYTE)regBufferValue, &dwRegSize);					
					item["SecurityDescriptor"] = GStringClass::UnicodeToUtf8(regBufferValue);
					
					_items.append(item);
				}

				items = _items;
			}
		}
	}
	else
	{
		return FALSE; 
	}
	
	RegCloseKey(hKey);	

	return TRUE;
}

int WindowApp::DelTSAppRegKey(Json::Value DelItems)
{
	Json::Value delApp = DelItems;

	int itemcount = delApp.size();
	int tCount = 0;

	for (int i = 0; i < itemcount; i++)
	{

		WCHAR* name = GStringClass::MultiByteToWideChar2( delApp["Name"].asCString());

		wchar_t subkey[256] = { 0 };
		wsprintf(subkey, L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Terminal Server\\TSAppAllowList\\Applications\\%ls", name);
		//"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Terminal Server\\TSAppAllowList\\Applications\\" + name;
		
		LONG ret = RegDeleteKeyEx(HKEY_LOCAL_MACHINE, subkey, KEY_WOW64_64KEY, 0);
		
		
		if ( ret == ERROR_SUCCESS) 
		{		  
		   tCount++ ;
		}

	}

	return tCount;
}

int WindowApp::PutTSAppRegKey(Json::Value AddItems)
{

	Json::Value putApp = AddItems;

	int itemcount  = putApp.size();
	int tCount = 0;

	//for ( int i = 0 ; i < itemcount ; i++  )
	//{

/**
"Name"="China Geo-Explorer"
"Path"="C:\\Program Files (x86)\\Google\\Chrome\\Application\\chrome.exe"
"IconIndex"=dword:00000001
"IconPath"="c:\\program files (x86)\\google\\chrome\\application\\chrome.exe"
"VPath"="%SYSTEMDRIVE%\\program files (x86)\\google\\chrome\\application\\chrome.exe"
"SecurityDescriptor"=""
"ShowInTSWA"=dword:00000001
"CommandLineSetting"=dword:00000002
"RequiredCommandLine"="http://chinageoexplorer.com"
**/

//		{"Path":"D:/Program Files/Adobe/Adobe Flash CC 2015/Flash", "RegName" : "Flash", "RequiredCommandLine" : "", "Name" : "Adobe Flash Professional CC 2015"}

		 string _path = putApp["Path"].asString();
		 _path = GStringClass::replace_all( _path ,"/","\\"  );

		 WCHAR* LPPath = GStringClass::MultiByteToWideChar2(_path);

		 WCHAR* RegName = GStringClass::MultiByteToWideChar2(putApp["RegName"].asCString());
		 WCHAR* Name = GStringClass::MultiByteToWideChar2( putApp["Name"].asCString() );
		 WCHAR* Path = LPPath;
		 /**
		 int IconIndex = putApp["IconIndex"].asInt();
		 WCHAR* IconPath = GStringClass::MultiByteToWideChar2(putApp["IconPath"].asCString());
		 WCHAR* VPath = GStringClass::MultiByteToWideChar2(putApp["VPath"].asCString());
		 WCHAR* SecurityDescriptor = GStringClass::MultiByteToWideChar2(putApp["SecurityDescriptor"].asCString());
		 int ShowInTSWA = putApp["ShowInTSWA"].asInt();
		 **/

		 int CommandLineSetting = putApp["CommandLineSetting"].asInt();
		 WCHAR* RequiredCommandLine = GStringClass::MultiByteToWideChar2( putApp["RequiredCommandLine"].asCString());

		 int IconIndex = 1;
		 WCHAR* IconPath = LPPath;
		 WCHAR* VPath = LPPath;
		 WCHAR* SecurityDescriptor = L"";
		 int ShowInTSWA = 0;

		 HKEY hkey = NULL;
		 HKEY hkey1 = NULL;
		 LONG ret;

		 wchar_t subkey[256] = { 0 };
		 wsprintf( subkey, L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Terminal Server\\TSAppAllowList\\Applications\\%ls" , RegName);
		 //"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Terminal Server\\TSAppAllowList\\Applications\\" + name;
		   
		  ret = RegCreateKeyEx(
			  HKEY_LOCAL_MACHINE,
			  subkey,
			  0,
			  NULL,
			  0,
			  KEY_ALL_ACCESS | KEY_WOW64_64KEY,
			  NULL,
			  &hkey,
			  NULL);
		  
		  if ( ret == ERROR_SUCCESS ) 
		  {
			  tCount++;
		  }

		  RegSetValueEx(hkey, L"Name", 0, REG_SZ, (BYTE*)Name, lstrlen(Name) * sizeof(WCHAR) );
		  RegSetValueEx(hkey, L"Path", 0, REG_SZ, (BYTE*)Path, lstrlen( Path ) * sizeof(WCHAR) );
		  RegSetValueEx(hkey, L"IconIndex", 0, REG_DWORD, (BYTE*)&IconIndex, sizeof(IconIndex));
		  RegSetValueEx(hkey, L"IconPath", 0, REG_SZ, (BYTE*)IconPath, lstrlen( IconPath ) * sizeof(WCHAR) );
		  //RegSetValueEx(hkey, L"ShortPath", 0, REG_SZ, (BYTE*)ShortPath, lstrlen(ShortPath));
		  RegSetValueEx(hkey, L"VPath", 0, REG_SZ, (BYTE*)VPath, lstrlen(VPath) * sizeof(WCHAR) );
		  RegSetValueEx(hkey, L"SecurityDescriptor", 0, REG_SZ, (BYTE*)SecurityDescriptor, lstrlen(SecurityDescriptor) * sizeof(WCHAR) );
		  RegSetValueEx(hkey, L"ShowInTSWA", 0, REG_DWORD, (BYTE*)&ShowInTSWA, sizeof(ShowInTSWA));
		  RegSetValueEx(hkey, L"CommandLineSetting", 0, REG_DWORD, (BYTE*)&CommandLineSetting, sizeof(CommandLineSetting));
		  RegSetValueEx(hkey, L"RequiredCommandLine", 0, REG_SZ, (BYTE*)RequiredCommandLine, lstrlen(RequiredCommandLine) * sizeof(WCHAR) );

		  RegCloseKey(hkey);


		  delete LPPath;
		  delete RegName;
		  delete Name;
		  Path = NULL;
		  delete RequiredCommandLine;
		  if ( LPPath == NULL )
		  {
			  IconPath = NULL;
			  VPath = NULL;
		  }
		  
		  SecurityDescriptor = NULL;
	//}

	return tCount;
}

/** ========================================================================================== **/
bool WindowApp::EnumShellLink(wchar_t*  strSrc, Json::Value& items, int flag)
{
	wchar_t strWildcard[MAX_PATH] = { '\0' };
	lstrcpy(strWildcard, strSrc);
	lstrcat(strWildcard, L"\\*.lnk");

	HANDLE hFind = NULL;
	WIN32_FIND_DATA findData;
	hFind = FindFirstFile(strWildcard, &findData);

	if (INVALID_HANDLE_VALUE == hFind) return false;

	do
	{
		Json::Value item;

		wchar_t SrcFilePath[MAX_PATH] = { '\0' };

		lstrcpy(SrcFilePath, strSrc);
		lstrcat(SrcFilePath, L"\\");
		lstrcat(SrcFilePath, findData.cFileName);

		if ((findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && flag)
		{
			if (wcsicmp(findData.cFileName, L".") == 0 || wcsicmp(findData.cFileName, L"..") == 0)
			{
				continue;
			}

			EnumShellLink(SrcFilePath, items,2);
		}
		else
		{
			/*
			if (!is_shell_link(findData.cFileName))
			{
				continue;
			}*/
			wchar_t PathBuf[MAX_PATH] = { 0 };
			wchar_t RequiredCommandLineBuf[MAX_PATH] = { 0 };

			GetShortCutFile(SrcFilePath, PathBuf, MAX_PATH, RequiredCommandLineBuf, MAX_PATH);
			
			if (!is_exe_file(PathBuf))	continue;			
						
			wchar_t lname[MAX_PATH] = { 0 };
			wchar_t* pTemp = wcsrchr(findData.cFileName, '.');

			if (pTemp != NULL)
			{
				wcsncpy(lname, findData.cFileName, pTemp - findData.cFileName);

				if (wcsstr(lname, L"卸载") != 0 && wcsstr(lname, L"uninstall") != 0) continue;
				

				wchar_t AppPath[MAX_PATH] = { 0 };
				lstrcpy(AppPath, PathBuf);
				wchar_t* regName = PathFindFileName(PathBuf);
				PathRemoveExtension(regName);
				
				/**
				*
				*/
				item["Name"] = GStringClass::UnicodeToUtf8(lname);
				item["RegName"] = GStringClass::UnicodeToUtf8(regName);
				item["Path"] = GStringClass::UnicodeToUtf8(AppPath);
				item["RequiredCommandLine"] = GStringClass::UnicodeToUtf8(RequiredCommandLineBuf);
				items.append( item );
							
			}
		}
	}while(FindNextFile(hFind, &findData));

	FindClose(hFind);

	return true;
}
/*
bool WindowApp::is_shell_link(wchar_t* filename)
{
	wchar_t buffer[MAX_PATH] = { 0 };
	lstrcpy(buffer, filename);
	wchar_t* tmp = wcsrchr(buffer, '.');
	if (tmp == NULL)
	{
		return false;
	}
	tmp++;
	if (wcsicmp(tmp, L"Lnk") != 0)
	{
		return false;
	}
	return true;
}
*/
bool WindowApp::GetShortCutFile(wchar_t* ShortcutFile, wchar_t* filePath, int sizefile, wchar_t* argv, int sizeargv)
{
	HRESULT           hres;
	IShellLink        *psl = NULL;
	IPersistFile      *ppf;
	WIN32_FIND_DATA   fd;

	CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
	
	hres = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (void**)&psl);
	if (!SUCCEEDED(hres))
		return   false;

	hres = psl->QueryInterface(IID_IPersistFile, (void**)&ppf);
	if (SUCCEEDED(hres))
	{
		wchar_t wsz[MAX_PATH]; 
		lstrcpy(wsz, ShortcutFile);
		
		hres = ppf->Load(wsz, STGM_READ);
		
		if (SUCCEEDED(hres))
		{
			printf( "%ls\n" , wsz);
			
			
			hres = psl->GetPath(filePath, sizefile, &fd, 0);

			if (wcsstr(filePath, L"C:\\WINDOWS\\Installer\\{") != NULL)
			{
				//psl->Resolve(NULL, SLR_INVOKE_MSI);
				//WCHAR szTargetFile[MAX_PATH] = {};
				DWORD dwSize = MAX_PATH;
				GetMsiShortCutTarget(wsz, filePath, dwSize);

				//printf("%ls", szTargetFile);
				return true;

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

bool WindowApp::is_exe_file(wchar_t* filename)
{
	wchar_t buffer[MAX_PATH] = { 0 };
	lstrcpy(buffer, filename);
	
	wchar_t* tmp = wcsrchr(buffer, '.');
	if (tmp == NULL)
	{
		return false;
	}
	tmp++;
	if (wcsicmp(tmp, L"exe") != 0)
	{
		return false;
	}
	return true;
}

/*
void WindowApp::trim(string &s)
{
	int index = 0;
	if (!s.empty())
	{
		while ((index = s.find(' ', index)) != string::npos)
		{
			s.erase(index, 1);
		}
	}
}
*/

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
BOOL GetMsiShortCutTarget(LPCWSTR strShortcutFile, LPWSTR strTargetFile, DWORD& nSize)
{
	if (!strShortcutFile || !strTargetFile)
		return FALSE;


	WCHAR szProductCode[39] = {};
	WCHAR szComponentCode[39] = {};
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

