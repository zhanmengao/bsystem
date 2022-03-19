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

//������ݷ�ʽ
int CreateLink(TTCHAR* szPath, TTCHAR* szArguments, WCHAR* szLink, TTCHAR* szLinkICO)
{
	HRESULT   hres;
	IShellLink* psl;
	IPersistFile*   ppf;
	//WCHAR wsz[MAX_PATH];

	//��ʼ��COM   
	CoInitialize(NULL);

	//����һ��IShellLinkʵ��   
	hres = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (void   **)&psl);
	if (FAILED(hres))
	{
		CoUninitialize();
		return FALSE;
	}

	/**
	IShellLinkA::SetArguments	����Shell���Ӷ���������в�����
	IShellLinkA::SetDescription	����Shell���Ӷ���������������������κ�Ӧ�ó�������ַ�����
	IShellLinkA::SetHotkey	ΪShell���Ӷ������ü��̿�ݼ����ȼ�����//psl->SetHotkey( MAKEWORD( 'R', HOTKEYF_SHIFT |HOTKEYF_CONTROL))
	IShellLinkA::SetIconLocation	����Shell���Ӷ����ͼ���λ�ã�·������������
	IShellLinkA::SetIDList	��ָ������ΪShell���Ӷ�������ʶ���б�PIDL����
	IShellLinkA::��setpath	����Shell���Ӷ����Ŀ���·�����ļ�����
	IShellLinkA::SetRelativePath	����Shell���Ӷ�������·����
	IShellLinkA::SetShowCmd	����Shell���Ӷ����show���show�������ô��ڵĳ�ʼ��ʾ״̬��
	IShellLinkA::SetWorkingDirectory ����Shell���Ӷ���Ĺ���Ŀ¼�����ơ�
	**/

	//����Ŀ��Ӧ�ó���   
	psl->SetPath(szPath);
	//����
	psl->SetArguments(szArguments);
	//ͼ��
	psl->SetIconLocation(szLinkICO, 0);

	psl->SetDescription(_T("VEAPP-CloudApplicationProgram"));

	//���ù���Ŀ¼
	CString csWorkDir(szPath);
	int idx = csWorkDir.ReverseFind('\\');
	csWorkDir = csWorkDir.Left(idx);
	psl->SetWorkingDirectory(csWorkDir);

	//��IShellLink��ȡ��IPersistFile�ӿ�   
	//���ڱ����ݷ�ʽ�������ļ� (*.lnk)   
	hres = psl->QueryInterface(IID_IPersistFile, (void**)&ppf);
	if (FAILED(hres))
	{
		CoUninitialize();
		return FALSE;
	}

	//ȷ�������ļ���ΪANSI��ʽ   
	//MultiByteToWideChar(CP_ACP, 0, szLink, -1, wsz, MAX_PATH);

	//����IPersistFile::Save   
	//�����ݷ�ʽ�������ļ�   (*.lnk)   
	hres = ppf->Save(szLink, STGM_READWRITE);
	//�ͷ�IPersistFile��IShellLink�ӿ�   
	ppf->Release();
	psl->Release();

	CoUninitialize();
	return   TRUE;
}

//���������ݷ�ʽ
BOOL CreateDesktopShortcut(TTCHAR* szPath, TTCHAR* szArguments, WCHAR* szLinkName, TTCHAR* szLinkICO)
{
	//��ݷ�ʽ�������ļ���
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
	//������ݷ�ʽ   
	if (!CreateLink(szPath, szArguments, szLink, szLinkICO))
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	}

}

//���������ݷ�ʽ
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

	//��ʼ��COM   
	CoInitialize(NULL);

	//����һ��IShellLinkʵ��   
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

//ɾ����ݷ�ʽ�������ļ� (*.lnk)
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

//strPicFile��ͼ���ļ���,֧��BMP JPEG GIF�ȸ�ʽ
//dwStyle��ǽֽ����ʽ
//WPSTYLE_CENTER ���� 0
//WPSTYLE_TILE ƽ�� 1
//WPSTYLE_STRETCH ���� 2 
//WPSTYLE_MAX 3
bool  SetWallpaper(const wchar_t* szPath, DWORD dwStyle)
{
	HRESULT hr = S_OK;
	IActiveDesktop* pIAD = NULL;
	//��ʼ��COM   
	CoInitialize(NULL);
	//�����ӿڵ�ʵ��
	hr = CoCreateInstance(CLSID_ActiveDesktop, NULL, CLSCTX_INPROC_SERVER, IID_IActiveDesktop, (void **)&pIAD);
	if (FAILED(hr))
	{
		return  false;
	}

	//����ǽֽ
	hr = pIAD->SetWallpaper(szPath, 0);
	if (FAILED(hr))
	{
		return  false;
	}

	//����ǽֽ����ʽ
	WALLPAPEROPT wpo;
	wpo.dwSize = sizeof(wpo);
	wpo.dwStyle = dwStyle;
	hr = pIAD->SetWallpaperOptions(&wpo, 0);
	if (!SUCCEEDED(hr))
	{
		return  false;
	}

	//Ӧ��ǽֽ������
	hr = pIAD->ApplyChanges(AD_APPLY_ALL);
	if (!SUCCEEDED(hr))
	{
		return  false;
	}
	WCHAR wszWallpaper[MAX_PATH] = { 0 };
	//��ȡǽֽ���ļ�������ӡ��debug������
	hr = pIAD->GetWallpaper(wszWallpaper, MAX_PATH, 0);
	CString strFile = wszWallpaper;

	//�ͷŽӿڵ�ʵ��
	pIAD->Release();
	return  true;
}

HRESULT GetWallpaper(std::wstring& wallpaper)
{
	HRESULT hr = S_OK;
	IActiveDesktop* pIAD = NULL;
	//��ʼ��COM   
	CoInitialize(NULL);
	//�����ӿڵ�ʵ��
	hr = CoCreateInstance(CLSID_ActiveDesktop, NULL, CLSCTX_INPROC_SERVER, IID_IActiveDesktop, (void **)&pIAD);
	if (FAILED(hr))                                         
	{
		return hr;
	}

	WCHAR wszWallpaper[MAX_PATH] = { 0 };
	//��ȡǽֽ���ļ�������ӡ��debug������
	hr = pIAD->GetWallpaper(wszWallpaper, MAX_PATH, 0);

	//�ͷŽӿڵ�ʵ��
	pIAD->Release();
	wallpaper = wszWallpaper;
	return hr;
}