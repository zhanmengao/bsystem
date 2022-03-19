
// SetWindowsInfoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SetWindowsInfo.h"
#include "SetWindowsInfoDlg.h"
#include "afxdialogex.h"

#include "FileCmdTool.h"
//#include <strsafe.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include <winver.h>
//#include <io.h>


//#pragma comment(lib, "mincore.lib")
#pragma comment(lib, "version.lib")

// CSetWindowsInfoDlg 对话框
CSetWindowsInfoDlg::CSetWindowsInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SETWINDOWSINFO_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSetWindowsInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_Owner);
	DDX_Control(pDX, IDC_EDIT2, m_Organization);
	DDX_Control(pDX, IDC_STATIC_OS, m_OSVersion);
}

BEGIN_MESSAGE_MAP(CSetWindowsInfoDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CSetWindowsInfoDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CSetWindowsInfoDlg::OnBnClickedCancel)
	ON_STN_CLICKED(IDC_STATIC_PICTURE, &CSetWindowsInfoDlg::OnStnClickedStaticPicture)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CSetWindowsInfoDlg 消息处理程序

BOOL CSetWindowsInfoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
		

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	/**
	* 进程提权
	* 读写系统注册表、Windows 目录下面的文件需要提升进程权限
	*/
	bool bRet = true;
	bRet = EnablePriviledge(SE_BACKUP_NAME);
	bRet = EnablePriviledge(SE_RESTORE_NAME);
	bRet = EnablePriviledge(SE_DEBUG_NAME);	

	/**
	* 当前系统版本信息 版本号
	*/
	m_OSVersion.SetWindowText(  L"Windows " + getSystemName() );

	/**
	[HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows NT\CurrentVersion]
	"RegisteredOwner" = "VEAPP"
	"RegisteredOrganization" = "VEsystem"
	**/
	WCHAR RegisteredOwner[20] = {0};
	DWORD valued = 0;
	GetRegEdit( L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion",
		L"RegisteredOwner",		
		RegisteredOwner,
		valued,
		HKEY_LOCAL_MACHINE);

	WCHAR RegisteredOrganization[20] = { 0 };
	GetRegEdit(L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion",
		L"RegisteredOrganization",
		RegisteredOrganization,
		valued,
		HKEY_LOCAL_MACHINE);

	m_Owner.SetWindowText(RegisteredOwner);
	m_Organization.SetWindowText(RegisteredOrganization);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CSetWindowsInfoDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	
	CDialogEx::OnSysCommand(nID, lParam);
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。
void CSetWindowsInfoDlg::OnPaint()
{
	
	CBitmap bitmap;
	bitmap.LoadBitmapW(IDB_BITMAP1);
	CStatic *p = (CStatic*)(GetDlgItem(IDC_STATIC_PICTURE));
	p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE);
	p->SetBitmap(bitmap);


	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CSetWindowsInfoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CSetWindowsInfoDlg::OnCbnSelchangeCombo1()
{
	// TODO: 在此添加控件通知处理程序代码

}

BOOL CSetWindowsInfoDlg::ReleaseRes(CString strFileName, WORD wResID, CString strFileType)
{
	DWORD   dwWrite = 0;

	HANDLE  hFile = CreateFile(strFileName, GENERIC_WRITE, FILE_SHARE_WRITE, NULL,
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}
 
	HRSRC   hrsc = FindResource(NULL, MAKEINTRESOURCE(wResID), strFileType);
	HGLOBAL hG = LoadResource(NULL, hrsc);
	DWORD   dwSize = SizeofResource(NULL, hrsc);
	if (!(hrsc&&hG&&dwSize))
	{
		return FALSE;
	}

	WriteFile(hFile, hG, dwSize, &dwWrite, NULL);
	CloseHandle(hFile);
	return TRUE;
}

void CSetWindowsInfoDlg::ReplaceBasebrd()
{

	FileCmdTool::SetSysFile2AdminACL(L"C:\\Windows\\Branding\\Basebrd\\basebrd.dll");
	
	int result = FileCmdTool::FileRename(
			L"C:\\Windows\\Branding\\Basebrd\\basebrd.dll",
			L"C:\\Windows\\Branding\\Basebrd\\basebrd.dll.bak"	);

}

void CSetWindowsInfoDlg::SetWindowsLogo()
{

	CString basebrd = L"C:\\Windows\\Branding\\Basebrd\\basebrd.dll";
	CString osVerwion = getSystemName();

	bool editLogo = false;

	if (osVerwion.Compare(L"10.0.17763") == 0) //Win2019( 1809 )
	{

		ReplaceBasebrd();
		//IDR_MUI6   10.0.17763
		editLogo = ReleaseRes(
			basebrd,
			(WORD)IDR_MUI6,
			L"MUI");

		
	}
	else
		if ( osVerwion.Find(L"6.1.760") >= 0 )  //Win2008 R2
		{

			ReplaceBasebrd();
			//IDR_MUI4   6.1.7601
			editLogo = ReleaseRes(
				basebrd,
				(WORD)IDR_MUI4,
				L"MUI");
		}
		else
			if (osVerwion.Find(L"6.3.960") >= 0 )   //Win2012 R2
			{
				
				ReplaceBasebrd();
				//IDR_MUI5   6.3.9600
				editLogo = ReleaseRes(
					basebrd,
					(WORD)IDR_MUI5,
					L"MUI");
			}


	if (!editLogo)
	{

		::MessageBox( NULL,L"当操作系统的版本未做适配，暂不支持个性化系统LOGO，请向 gaochong@vesystem.com 反馈问题...",L"友情提示", MB_OK | MB_ICONASTERISK  );

	}

}

void CSetWindowsInfoDlg::SetWindowsOEMInfo()
{

	/**
	[HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\OEMInformation]
	"Logo" = "OEMLOGO.bmp"
	"Model" = "VEAPP"
	"Manufacturer" = "VEsystem"
	"SupportURL" = "http://www.vesystem.com"
	"SupportAppURL" = "http://www.vesystem.com"
	"SupportProvider" = "GaoChong"
	**/

	SetRegEdit(L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\OEMInformation",
		L"Logo",
		REG_SZ,
		L"C:\\Windows\\OEMLOGO.bmp",
		0);

	SetRegEdit(L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\OEMInformation",
		L"Model",
		REG_SZ,
		L"VEAPP",
		0);

	SetRegEdit(L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\OEMInformation",
		L"Manufacturer",
		REG_SZ,
		L"VEsystem",
		0);

	SetRegEdit(L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\OEMInformation",
		L"SupportURL",
		REG_SZ,
		L"http://www.vesystem.com",
		0);

	SetRegEdit(L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\OEMInformation",
		L"SupportAppURL",
		REG_SZ,
		L"http://www.vesystem.com",
		0);

	SetRegEdit(L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\OEMInformation",
		L"SupportProvider",
		REG_SZ,
		L"vesystem.com",
		0);

	/**
	SetRegEdit(L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\OEMInformation",
	L"SupportHours",
	REG_SZ,
	L"5 * 8",
	0);
	**/

	SetRegEdit(L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\OEMInformation",
		L"SupportPhone",
		REG_SZ,
		L"010-60845985",
		0);

	/**
	HANDLE hToken;
	TOKEN_PRIVILEGES tkp;
	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))	return;
	LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &tkp.Privileges[0].Luid);
	tkp.PrivilegeCount = 1;
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0);
	if (GetLastError() != ERROR_SUCCESS) return;
	**/

	ReleaseRes(L"C:\\Windows\\OEMLOGO.bmp", (WORD)IDR_LOGO, L"MUI");

}

void CSetWindowsInfoDlg::SetOwnerInfo()
{

	/**
	[HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows NT\CurrentVersion]
	"RegisteredOwner" = "VEAPP"
	"RegisteredOrganization" = "VEsystem"
	**/

	CString OwnerStr;
	CString OrganizationStr;

	m_Owner.GetWindowText(OwnerStr);
	m_Organization.GetWindowText(OrganizationStr);


	SetRegEdit(L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion",
		L"RegisteredOwner",
		REG_SZ,
		OwnerStr.GetBuffer(),
		0);

	SetRegEdit(L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion",
		L"RegisteredOrganization",
		REG_SZ,
		OrganizationStr.GetBuffer(),
		0);

	OwnerStr.ReleaseBuffer();
	OrganizationStr.ReleaseBuffer();

	/**
	*
	* 篡改 Windows 的版本信息
	* =============================================================== */
	SetRegEdit(L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion",
		L"CurrentBuild",
		REG_SZ,
		L"2019.6.0",
		0);

	SetRegEdit(L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion",
		L"ReleaseId",
		REG_SZ,
		L"VEsystem VEUAM 2019",
		0);

	SetRegEdit(L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion",
		L"UBR",
		REG_DWORD,
		NULL,
		0);
	

	

}

void CSetWindowsInfoDlg::OnBnClickedOk()
{
  
	SetOwnerInfo();          // 设置 用户 信息
	SetWindowsOEMInfo();	 // 设置 OEM  信息
	SetWindowsLogo();        // 换LOGO

	char* winverCmd = "C:\\Windows\\system32\\winver.exe";
	WinExec(winverCmd,SW_SHOW );

	char* systemCmd = "C:\\WINDOWS\\System32\\control.exe system";
	WinExec(systemCmd, SW_SHOW);
	
	CDialogEx::OnOK();
}


void CSetWindowsInfoDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码

	CDialogEx::OnCancel();
}

/**
* 提权申请（提升当前进程获取指定的权限）
*/
bool CSetWindowsInfoDlg::EnablePriviledge(LPCTSTR lpSystemName)
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


LONG CSetWindowsInfoDlg::SetRegEdit(wchar_t *lpSubKey, wchar_t* lpValueName, DWORD dwType, wchar_t* szValue, DWORD dValue)
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

	lResult = RegCreateKeyEx(
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

LONG CSetWindowsInfoDlg::GetRegEdit(wchar_t *lpSubKey, wchar_t* lpValueName, wchar_t* szValue, DWORD& dValue, HKEY MinhKey)
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


/**
* 获取 basebrd.dll 文件的版本号
*/
CString CSetWindowsInfoDlg::getSystemName()
{
	wchar_t *FileName = L"C:\\Windows\\Branding\\Basebrd\\basebrd.dll";

	CString asVer;

	VS_FIXEDFILEINFO *pVsInfo;
	unsigned int iFileInfoSize = sizeof(VS_FIXEDFILEINFO);

	int iVerInfoSize = GetFileVersionInfoSize(FileName, NULL);
	if ( iVerInfoSize != 0 ) {
		
		wchar_t *pBuf = NULL;
	
		while (!pBuf) 
		{
			pBuf = new wchar_t[iVerInfoSize];
		}

		if ( GetFileVersionInfo(FileName, 0, iVerInfoSize, pBuf ) ) 
		{

			if (VerQueryValue(pBuf, L"\\", (void **)&pVsInfo, &iFileInfoSize)) 
			{
				wsprintf( pBuf, 			
					//L"%d.%d.%d.%d",
					L"%d.%d.%d",
					HIWORD(pVsInfo->dwProductVersionMS),
					LOWORD(pVsInfo->dwProductVersionMS),
					HIWORD(pVsInfo->dwProductVersionLS)
					//LOWORD(pVsInfo->dwProductVersionLS)
					);

				asVer = CString( pBuf );
			}
			
		}
		delete pBuf;
	}

		
	return asVer;
}

void CSetWindowsInfoDlg::OnStnClickedStaticPicture()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CSetWindowsInfoDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: 在此处添加消息处理程序代码	

}
