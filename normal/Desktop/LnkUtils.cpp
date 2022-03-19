#define  _WININET_ 
#define INTERNET_MAX_URL_LENGTH 260
#include <Windows.h>
#include"LnkUtils.h"
#include "io.h"
#include<string>
#include <atlstr.h>
#include <Shlobj.h>
#include <objbase.h>
#include <shellapi.h>
#include<string>
#include<comdef.h>
#include<atlbase.h>
#pragma comment(lib, "shell32")
#pragma comment(lib, "ole32.lib")

CString Get_ENV(wchar_t* hzName);
BOOL isVeAppLink(WCHAR* szLink);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//创建快捷方式
int CreateLink(TTCHAR* szPath, TTCHAR* szArguments, WCHAR* szLink, TTCHAR* szLinkICO)
{
	HRESULT   hres;
	IShellLink* psl;
	IPersistFile*   ppf;
	//WCHAR wsz[MAX_PATH];

	//初始化COM   
	CoInitialize(NULL);

	//创建一个IShellLink实例   
	hres = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (void   **)&psl);
	if (FAILED(hres))
	{
		CoUninitialize();
		return FALSE;
	}

	/**
	IShellLinkA::SetArguments	设置Shell链接对象的命令行参数。
	IShellLinkA::SetDescription	设置Shell链接对象的描述。描述可以是任何应用程序定义的字符串。
	IShellLinkA::SetHotkey	为Shell链接对象设置键盘快捷键（热键）。//psl->SetHotkey( MAKEWORD( 'R', HOTKEYF_SHIFT |HOTKEYF_CONTROL))
	IShellLinkA::SetIconLocation	设置Shell链接对象的图标的位置（路径和索引）。
	IShellLinkA::SetIDList	将指针设置为Shell链接对象的项标识符列表（PIDL）。
	IShellLinkA::的setpath	设置Shell链接对象的目标的路径和文件名。
	IShellLinkA::SetRelativePath	设置Shell链接对象的相对路径。
	IShellLinkA::SetShowCmd	设置Shell链接对象的show命令。show命令设置窗口的初始显示状态。
	IShellLinkA::SetWorkingDirectory 设置Shell链接对象的工作目录的名称。
	**/

	//设置目标应用程序   
	psl->SetPath(szPath);
	//参数
	psl->SetArguments(szArguments);
	//图标
	psl->SetIconLocation(szLinkICO, 0);

	psl->SetDescription(_T("VEAPP-CloudApplicationProgram"));

	//设置工作目录
	CString csWorkDir(szPath);
	int idx = csWorkDir.ReverseFind('\\');
	csWorkDir = csWorkDir.Left(idx);
	psl->SetWorkingDirectory(csWorkDir);

	//从IShellLink获取其IPersistFile接口   
	//用于保存快捷方式的数据文件 (*.lnk)   
	hres = psl->QueryInterface(IID_IPersistFile, (void**)&ppf);
	if (FAILED(hres))
	{
		CoUninitialize();
		return FALSE;
	}

	//确保数据文件名为ANSI格式   
	//MultiByteToWideChar(CP_ACP, 0, szLink, -1, wsz, MAX_PATH);

	//调用IPersistFile::Save   
	//保存快捷方式的数据文件   (*.lnk)   
	hres = ppf->Save(szLink, STGM_READWRITE);
	//释放IPersistFile和IShellLink接口   
	ppf->Release();
	psl->Release();

	CoUninitialize();
	return   TRUE;
}

//创建桌面快捷方式
BOOL CreateDesktopShortcut(TTCHAR* szPath, TTCHAR* szArguments, WCHAR* szLinkName, TTCHAR* szLinkICO)
{
	//快捷方式的数据文件名
	WCHAR szLink[MAX_PATH] = { 0 };

	//SHGetSpecialFolderPath(NULL, szLink, CSIDL_COMMON_DESKTOPDIRECTORY, FALSE);

	CString HOMEPATH = Get_ENV(L"USERPROFILE");
	wsprintfW(szLink, L"%s\\Desktop\\",HOMEPATH);
	if (szLink[lstrlenW(szLink) - 1] == _T('\\'))
	{
		lstrcatW(szLink, szLinkName);
	}
	else
	{
		lstrcatW(szLink, L"\\");
		lstrcatW(szLink, szLinkName);
	}


	HOMEPATH.ReleaseBuffer();
	//创建快捷方式   
	if (!CreateLink(szPath, szArguments, szLink, szLinkICO))
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	}

}

//清理桌面快捷方式
BOOL CleanDesktopShortcut()
{
	wchar_t dskpath[MAX_PATH] = { 0 };
	//SHGetSpecialFolderPath(NULL, dskpath, CSIDL_COMMON_DESKTOPDIRECTORY, FALSE);
	//lstrcat(szLink ,L"\\*.lnk");
	CString HOMEPATH = Get_ENV(L"USERPROFILE");
	wsprintfW(dskpath, L"%s\\Desktop\\", HOMEPATH.GetBuffer());

	wchar_t desktopLinkPath[MAX_PATH] = { 0 };
	wsprintfW(desktopLinkPath, L"%s\\*.lnk", dskpath);

	long   hFile = 0;
	struct _wfinddata_t fileinfo;

	if ((hFile = _wfindfirst(desktopLinkPath, &fileinfo)) != -1)
	{
		do
		{
			if (!(fileinfo.attrib &  _A_SUBDIR))
			{

				WCHAR tmpfilepath[MAX_PATH] = { 0 };
				wsprintfW(tmpfilepath, L"%s\\%s", dskpath, fileinfo.name);
				//printf( "%ls\n", tmpfilepath );

				if (isVeAppLink(tmpfilepath))
				{
					DeleteFileW(tmpfilepath);
				}
			}

		} while (_wfindnext(hFile, &fileinfo) == 0);

		_findclose(hFile);
	}


	::PostMessage(GetDesktopWindow(), WM_KEYDOWN, VK_F5, 0);

	HOMEPATH.ReleaseBuffer();
	return FALSE;
}


BOOL isVeAppLink(WCHAR* szLink)
{
	HRESULT   hres;
	IShellLink* psl;
	IPersistFile* ppf = NULL;
	BOOL veApp = FALSE;

	//初始化COM   
	CoInitialize(NULL);

	//创建一个IShellLink实例   
	hres = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (void   **)&psl);
	if (!FAILED(hres))
	{
		hres = psl->QueryInterface(IID_IPersistFile, (void**)&ppf);
		if (!FAILED(hres))
		{

			hres = ppf->Load(szLink, STGM_READ);

			if (SUCCEEDED(hres))
			{

				TTCHAR tmpbuf[MAX_PATH] = { 0 };
				int nSize = MAX_PATH;

				psl->GetDescription(tmpbuf, nSize);
				printf("GetDescription: %ls\n", tmpbuf);

				if (!lstrcmp(_T("VEAPP-CloudApplicationProgram"), tmpbuf))
				{
					veApp = TRUE;
				}
			}
		}

	}

	ppf->Release();
	psl->Release();

	CoUninitialize();

	return veApp;
}

//删除快捷方式的数据文件 (*.lnk)
BOOL DeleteDesktopShortcut(TTCHAR* lpszShortcut)
{

	SHFILEOPSTRUCT fos;
	ZeroMemory(&fos, sizeof(fos));
	fos.hwnd = HWND_DESKTOP;
	fos.wFunc = FO_DELETE;
	fos.pFrom = lpszShortcut;
	fos.pTo = NULL;

	fos.fFlags = FOF_SILENT | FOF_ALLOWUNDO;

	if (0 != SHFileOperation(&fos))
	{
		return FALSE;
	}

	return TRUE;

}

CString Get_ENV(wchar_t* hzName)
{

	wchar_t* hzName_v;
	size_t requiredSize;

	_wgetenv_s(&requiredSize, NULL, 0, hzName);
	if (requiredSize == 0)
	{
		return "";
	}

	hzName_v = (wchar_t*)malloc(requiredSize * sizeof(wchar_t));
	if (!hzName_v)
	{
		return "";
	}
	_wgetenv_s(&requiredSize, hzName_v, requiredSize, hzName);
	CString str(hzName_v);
	free(hzName_v);
	return str;
}

//strPicFile是图像文件名,支持BMP JPEG GIF等格式
//dwStyle是墙纸的样式
//WPSTYLE_CENTER 居中 0
//WPSTYLE_TILE 平铺 1
//WPSTYLE_STRETCH 拉伸 2 
//WPSTYLE_MAX 3
bool  SetWallpaper(const wchar_t* szPath, DWORD dwStyle)
{
	HRESULT hr = S_OK;
	IActiveDesktop* pIAD = NULL;
	//初始化COM   
	CoInitialize(NULL);
	//创建接口的实例
	hr = CoCreateInstance(CLSID_ActiveDesktop, NULL, CLSCTX_INPROC_SERVER, IID_IActiveDesktop, (void **)&pIAD);
	if (FAILED(hr))
	{
		return  false;
	}

	//设置墙纸
	hr = pIAD->SetWallpaper(szPath, 0);
	if (FAILED(hr))
	{
		return  false;
	}

	//设置墙纸的样式
	WALLPAPEROPT wpo;
	wpo.dwSize = sizeof(wpo);
	wpo.dwStyle = dwStyle;
	hr = pIAD->SetWallpaperOptions(&wpo, 0);
	if (!SUCCEEDED(hr))
	{
		return  false;
	}

	//应用墙纸的设置
	hr = pIAD->ApplyChanges(AD_APPLY_ALL);
	if (!SUCCEEDED(hr))
	{
		return  false;
	}
	WCHAR wszWallpaper[MAX_PATH] = { 0 };
	//读取墙纸的文件名并打印在debug窗口内
	hr = pIAD->GetWallpaper(wszWallpaper, MAX_PATH, 0);
	CString strFile = wszWallpaper;

	//释放接口的实例
	pIAD->Release();
	return  true;
}

HRESULT GetWallpaper(std::wstring& wallpaper)
{
	HRESULT hr = S_OK;
	IActiveDesktop* pIAD = NULL;
	//初始化COM   
	CoInitialize(NULL);
	//创建接口的实例
	hr = CoCreateInstance(CLSID_ActiveDesktop, NULL, CLSCTX_INPROC_SERVER, IID_IActiveDesktop, (void **)&pIAD);
	if (FAILED(hr))                                         
	{
		return hr;
	}

	WCHAR wszWallpaper[MAX_PATH] = { 0 };
	//读取墙纸的文件名并打印在debug窗口内
	hr = pIAD->GetWallpaper(wszWallpaper, MAX_PATH, 0);

	//释放接口的实例
	pIAD->Release();
	wallpaper = wszWallpaper;
	return hr;
}