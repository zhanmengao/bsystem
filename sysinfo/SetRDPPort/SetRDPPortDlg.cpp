
// SetRDPPortDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SetRDPPort.h"
#include "SetRDPPortDlg.h"
#include "afxdialogex.h"
#include "RegEdit.h"

#include "FileCmdTool.h"
#include "LogonScripts.h"
#include "HookSetup.h"
#include "SetLogobmp.h"

#include "Userenv.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#pragma comment( lib,"Userenv.lib" )


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CSetRDPPortDlg �Ի���



CSetRDPPortDlg::CSetRDPPortDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SETRDPPORT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSetRDPPortDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_rdpPort);
	DDX_Control(pDX, IDC_CHECK1, m_cleanlibLink);
	DDX_Control(pDX, IDC_CHECK2, m_hidethisPCPolicy);
	DDX_Control(pDX, IDC_CHECK4, m_cleanproDir);
	DDX_Control(pDX, IDC_CHECK6, m_fdisabledAllowList);
	DDX_Control(pDX, IDC_CHECK7, m_fSingleSessionPerUser);
	DDX_Control(pDX, IDC_EDIT2, m_LicenseServers);
	DDX_Control(pDX, IDC_CHECK8, m_InputLicense);
	DDX_Control(pDX, IDC_CHECK9, m_defaultPort);
	DDX_Control(pDX, IDC_CHECK10, m_NoDrives);
	DDX_Control(pDX, IDC_CHECK11, m_NoViewOnDrive);
	DDX_Control(pDX, IDC_CHECK3, m_UserAuthentication);
	DDX_Control(pDX, IDC_CHECK5, m_MaxInstanceCount);
	DDX_Control(pDX, IDC_EDIT3, m_MaxContionEdit);
	DDX_Control(pDX, IDC_CHECK13, m_EnableLogonScriptDelay);
	DDX_Control(pDX, IDC_CHECK12, m_NoControlPanel);
	DDX_Control(pDX, IDC_CHECK14, m_NoViewContextMenu);
	DDX_Control(pDX, IDC_CHECK15, m_ExplorerWindows);
	DDX_Control(pDX, IDC_CHECK16, m_NoHelpPane);
	DDX_Control(pDX, IDC_CHECK17, m_DisableCMD);
	DDX_Control(pDX, IDC_CHECK18, m_IE);
	DDX_Control(pDX, IDC_CHECK19, m_cadKetCall);
	DDX_Control(pDX, IDC_CHECK20, m_utilman);
	DDX_Control(pDX, IDC_CHECK21, m_KOneDrive);
}

BEGIN_MESSAGE_MAP(CSetRDPPortDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CSetRDPPortDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CSetRDPPortDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON1, &CSetRDPPortDlg::OnBnClickedButton1)	
	ON_BN_CLICKED(IDC_BUTTON2, &CSetRDPPortDlg::OnBnClickedButton2)	
	ON_BN_CLICKED(IDC_BUTTON3, &CSetRDPPortDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CSetRDPPortDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON8, &CSetRDPPortDlg::OnBnClickedButton8)
	//ON_BN_CLICKED(IDC_CHECK17, &CSetRDPPortDlg::OnBnClickedCheck17)
	ON_BN_CLICKED(IDC_BUTTON9, &CSetRDPPortDlg::OnBnClickedButton9)
	ON_BN_CLICKED(IDC_BUTTON10, &CSetRDPPortDlg::OnBnClickedButton10)
END_MESSAGE_MAP()


// CSetRDPPortDlg ��Ϣ�������

BOOL CSetRDPPortDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	/**
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	**/

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	//��Ȩ
	EnablePriviledge();

	/**
	* ��ȡ��ǰ����
	*/
	ReadCurrentSetup();
	
	defaultSelect = FALSE;

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CSetRDPPortDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CSetRDPPortDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CSetRDPPortDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CSetRDPPortDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	
	EditPortSetup();
	SetHideThisPCPolicy();
	SetCleanProDir();
	SetCleanLibLink();
	SetDisabledAllowList();
	SetSingleSessionPerUser();
	SetLicenseServers();
	SetNoViewOnDrive();
	SetNoDrives();
	SetUserAuthentication();
	SetMaxInstanceCount();
	SetEnableLogonScriptDelay();
	SetNoControlPanel();		
	SetNoViewContextMenu();
	SetExplorerWindows();
	SetWindowsHelp();
	SetIexploreFileName();
	SetDisableCMD();	
	SetCADKetCall();
	SetUtilmanFileName();
	SetKOneDrive();


	//https://docs.microsoft.com/en-us/windows/win32/api/userenv/nf-userenv-refreshpolicy
	RefreshPolicy( TRUE  );
	RefreshPolicy( FALSE );

	::MessageBox(NULL, L"����ִ����ɣ��������������֮����Ч�������ϵͳ�Ĳ��ɿ����أ�������ȷ������ѡ�����ö���Ԥ�ڵ�����...", L"��ʾ", MB_OK | MB_ICONEXCLAMATION);

	//CDialogEx::OnOK();
}






void CSetRDPPortDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	//HKEY_CLASSES_ROOT\file


	//{00000303-0000-0000-C000-000000000046}
	CRegEdit::SetTEST();
	CDialogEx::OnCancel();
}

void CSetRDPPortDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	if (::MessageBox(NULL, L"ȷ���������������? ��ȷ����δ������ļ�...", L"���ؾ���", MB_OKCANCEL | MB_ICONEXCLAMATION) == IDOK)
	{
		MySystemShutdown();
	}	

}

///////////////////////////////////////////////////////////////////////////////////////////


BOOL CSetRDPPortDlg::MySystemShutdown()
{
	HANDLE hToken;
	TOKEN_PRIVILEGES tkp;
	
	if (!OpenProcessToken(GetCurrentProcess(),
		TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
		return(FALSE);
	
	LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME,
		&tkp.Privileges[0].Luid);

	tkp.PrivilegeCount = 1;  // one privilege to set    
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	

	AdjustTokenPrivileges(hToken, FALSE, &tkp, 0,
		(PTOKEN_PRIVILEGES)NULL, 0);

	if (GetLastError() != ERROR_SUCCESS)
		return FALSE;
	/*
	if (!ExitWindowsEx(EWX_SHUTDOWN | EWX_FORCE,
		SHTDN_REASON_MAJOR_OPERATINGSYSTEM |
		SHTDN_REASON_MINOR_UPGRADE |
		SHTDN_REASON_FLAG_PLANNED))
		return FALSE;
	*/
	if (!ExitWindowsEx(EWX_REBOOT | EWX_FORCEIFHUNG,
		SHTDN_REASON_MAJOR_OPERATINGSYSTEM |
		SHTDN_REASON_MINOR_UPGRADE |
		SHTDN_REASON_FLAG_PLANNED ))
		return FALSE;

		
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////////////////
BOOL CSetRDPPortDlg::EnablePriviledge()
{
	bool bRet = true;
	bRet = CRegEdit::EnablePriviledge(SE_BACKUP_NAME);
	bRet = CRegEdit::EnablePriviledge(SE_RESTORE_NAME);

	return bRet;
}

BOOL CSetRDPPortDlg::EditPortSetup()
{

	BOOL zbool = TRUE;

	if (m_defaultPort.GetCheck() == BST_CHECKED)
	{
		CString tStr;
		m_rdpPort.GetWindowText(tStr);

		int iPort = _wtoi(tStr);
		
		if (iPort >= 3389 && iPort < 65535)
		{

			long wCode = CRegEdit::SetRDPPort(iPort);

			if (wCode != 0)
			{
				CString outError;
				outError.Format(L"�޸Ķ˿ں�ʧ�ܣ�%l", wCode);

				::MessageBox(NULL, outError, L"��ʾ", MB_OK | MB_ICONERROR);
				zbool = FALSE;
			}
		}
		else
		{
			::MessageBox(NULL, L"�޶���RDP �˿�Ӧ���� 3389 - 65535 ֮��", L"��ʾ", MB_OK | MB_ICONERROR);
			zbool = FALSE;

		}
		
	}
	
	return zbool;

}

DWORD CSetRDPPortDlg::SetCleanLibLink()
{

	DWORD result = 0;

	if (m_cleanlibLink.GetCheck() == BST_CHECKED )
	{

		result = CRegEdit::CleanLibLink();
	}
	else
	{
		result = CRegEdit::ResetLibLink();
	}
	
	return result;
}


DWORD CSetRDPPortDlg::SetCleanProDir()
{

	DWORD result = 0;

	if ( m_cleanproDir.GetCheck() == BST_CHECKED )
	{
		result = CRegEdit::CleanProDir();
	}
	else
	{
		//
	}

	return result;
}

DWORD CSetRDPPortDlg::SetHideThisPCPolicy()
{
	DWORD result = 0;

	if ( m_hidethisPCPolicy.GetCheck() == BST_CHECKED )
	{
		result = CRegEdit::HideThisPCPolicy();
	}
	else
	{
		result = CRegEdit::ShowThisPCPolicy();
	}

	return result;
}


DWORD CSetRDPPortDlg::SetDisabledAllowList()
{

	DWORD result = 0;

	if ( m_fdisabledAllowList.GetCheck() == BST_CHECKED)
	{
		result = CRegEdit::SetfDisabledAllowList(1);
	}
	else
	{
		result = CRegEdit::SetfDisabledAllowList(0);
	}

	return result;
}


DWORD CSetRDPPortDlg::SetSingleSessionPerUser()
{

	DWORD result = 0;	

	if ( m_fSingleSessionPerUser.GetCheck() == BST_CHECKED)
	{
		CRegEdit::SetfSingleSessionPerUser(0);
		result = CRegEdit::SetfSingleSessionPerUser2(0);
	}
	else
	{
		CRegEdit::SetfSingleSessionPerUser(1);
		result = CRegEdit::SetfSingleSessionPerUser2(1);
	}

	return result;
}


DWORD CSetRDPPortDlg::SetLicenseServers()
{

    DWORD result = 0;
	CString licenseAddr;
	
	if ( m_InputLicense.GetCheck() == BST_CHECKED )
	{

        m_LicenseServers.GetWindowText( licenseAddr );

		if ( licenseAddr.GetLength() == 0  || licenseAddr.GetLength() > 100  )
		{
			::MessageBox( NULL,L"�������������֤������...",L"��ʾ",MB_OK|MB_ICONASTERISK  );
			return -1;
		}

		wchar_t *pLicenseAddr = licenseAddr.GetBuffer();
		licenseAddr.ReleaseBuffer();

		result = CRegEdit::SetLicenseServers(pLicenseAddr);
		result = CRegEdit::SetLicensingMode(0x4);


	}
	
	return result;

}

DWORD CSetRDPPortDlg::SetNoDrives()
{

	DWORD result = 0;

	if ( m_NoDrives.GetCheck() == BST_CHECKED)
	{
		result = CRegEdit::SetNoDrives();
	}
	else 
	{
		result = CRegEdit::ClearNoDrives();
	}

	return result;
}

DWORD CSetRDPPortDlg::SetNoViewOnDrive()
{
	DWORD result = 0;

	if ( m_NoViewOnDrive.GetCheck() == BST_CHECKED )
	{
		result = CRegEdit::SetNoViewOnDrive();
	}
	else
	{
		result = CRegEdit::ClearNoViewOnDrive();
	}

	return result;
}


DWORD CSetRDPPortDlg::SetUserAuthentication()
{
	DWORD result = 0;

	if ( m_UserAuthentication.GetCheck() == BST_CHECKED)
	{
		result = CRegEdit::SetUserAuthentication(0);
	}
	else
	{
		result = CRegEdit::SetUserAuthentication(1);
	}

	return result;
}

DWORD CSetRDPPortDlg::SetMaxInstanceCount()
{
	DWORD result = 0;
	
	if ( m_MaxInstanceCount.GetCheck() == BST_CHECKED )
	{

		CString maxCountStr;
		m_MaxContionEdit.GetWindowText(maxCountStr);

		int maxCount = _wtoi(maxCountStr);

		if (maxCount < 2 || maxCount > 999999)
		{
			::MessageBox(NULL, L"�û��Ự������Ӧ���� 2 - 999999 ֮��... ", L"��ʾ", MB_OK | MB_ICONASTERISK);
			return -1;
		}
		else
		{
			result = CRegEdit::SetMaxInstanceCount(maxCount);
		}		
	}
	

	return result;
}


DWORD CSetRDPPortDlg::SetEnableLogonScriptDelay()
{
	DWORD result = 0;

	if (m_EnableLogonScriptDelay.GetCheck() == BST_CHECKED)
	{
		
		result = CRegEdit::SetEnableLogonScriptDelay(0);		
	}
	else
	{
		result = CRegEdit::SetEnableLogonScriptDelay(1);
	}


	return result;
}

DWORD CSetRDPPortDlg::SetNoControlPanel()
{
	DWORD result = 0;

	if (m_NoControlPanel.GetCheck() == BST_CHECKED)
	{

		result = CRegEdit::SetNoControlPanel(1);
	}
	else
	{
		result = CRegEdit::SetNoControlPanel(0);
	}


	return result;
}


DWORD CSetRDPPortDlg::SetNoViewContextMenu()
{
	DWORD result = 0;

	if (m_NoViewContextMenu.GetCheck() == BST_CHECKED)
	{

		result = CRegEdit::SetNoViewContextMenu(1);
	}
	else
	{
		result = CRegEdit::SetNoViewContextMenu(0);
	}

	return result;

}


DWORD CSetRDPPortDlg::SetDisableCMD()
{
	DWORD result = 0;

	if ( m_DisableCMD.GetCheck() == BST_CHECKED )
	{

		/*
		* 1 Ҳ����������
		* 2 ֻ����CMD    
		*/
		result = CRegEdit::SetDisableCMD(2);
	}
	else
	{
		result = CRegEdit::SetDisableCMD(0);
	}

	return result;

}

void CSetRDPPortDlg::SetCADKetCall()
{

	if (m_cadKetCall.GetCheck() == BST_CHECKED)
	{
		CRegEdit::SetCADKetCall(1);
	}
	else
	{
		CRegEdit::SetCADKetCall(0);
	}

	return;
}

void CSetRDPPortDlg::SetKOneDrive()
{
	if ( m_KOneDrive.GetCheck() == BST_CHECKED)
	{
		CRegEdit::SetKOneDrive(1);
	}
	else
	{
		CRegEdit::SetKOneDrive(0);
	}

}


DWORD CSetRDPPortDlg::SetExplorerWindows()
{
	DWORD result = 0;

	if (m_ExplorerWindows.GetCheck() == BST_CHECKED)
	{
		
		result = CRegEdit::SetExplorerWindows(1);
	}
	else
	{
		result = CRegEdit::SetExplorerWindows(0);
	}


	return result;
}

DWORD CSetRDPPortDlg::SetWindowsHelp()
{
	DWORD result = 0;
	
	if (m_NoHelpPane.GetCheck() == BST_CHECKED)
	{				

		/**
		https://docs.microsoft.com/en-us/previous-versions/orphan-topics/ws.10/cc755342(v=ws.10)
		https://docs.microsoft.com/en-us/previous-versions/windows/it-pro/windows-server-2012-R2-and-2012/cc753525(v=ws.11)
		*/

		FileCmdTool::SetSysFile2AdminACL( L"C:\\Windows\\HelpPane.exe" );

		result = FileCmdTool::FileRename(
			L"C:\\Windows\\HelpPane.exe",
			L"C:\\Windows\\HelpPane.GC.exe"	);

		if (result != 0)
		{
			// ������ʧ��...
		}		

	}
	else
	{
		result = FileCmdTool::FileRename(
			L"C:\\Windows\\HelpPane.GC.exe",
			L"C:\\Windows\\HelpPane.exe");

	}

	
	return result;
}

DWORD CSetRDPPortDlg::SetIexploreFileName()
{
	DWORD result = 0;

	if (m_IE.GetCheck() == BST_CHECKED)
	{

		/**
		https://docs.microsoft.com/en-us/previous-versions/orphan-topics/ws.10/cc755342(v=ws.10)
		https://docs.microsoft.com/en-us/previous-versions/windows/it-pro/windows-server-2012-R2-and-2012/cc753525(v=ws.11)
		*/

		//C:\Program Files (x86)\Internet Explorer\iexplore.exe

		FileCmdTool::SetSysFile2AdminACL(L"C:\\Program Files (x86)\\Internet Explorer\\iexplore.exe");

		result = FileCmdTool::FileRename(
			L"C:\\Program Files (x86)\\Internet Explorer\\iexplore.exe",
			L"C:\\Program Files (x86)\\Internet Explorer\\iexplore.GC.exe");

		if (result != 0)
		{
			// ������ʧ��...
		}

	}
	else
	{
		result = FileCmdTool::FileRename(
			L"C:\\Program Files (x86)\\Internet Explorer\\iexplore.GC.exe",
			L"C:\\Program Files (x86)\\Internet Explorer\\iexplore.exe"	);

	}


	return result;
}

//utilman.exe
DWORD CSetRDPPortDlg::SetUtilmanFileName()
{
	DWORD result = 0;

	if ( m_utilman.GetCheck() == BST_CHECKED)
	{

		//C:\\Windows\\sysnative\\utilman.exe
		FileCmdTool::SetSysFile2AdminACL(L"C:\\Windows\\sysnative\\utilman.exe");

		result = FileCmdTool::FileRename(
			L"C:\\Windows\\sysnative\\utilman.exe",
			L"C:\\Windows\\sysnative\\utilman.GC.exe"
		);

		if (result != 0)
		{
			// ������ʧ��...
		}

	}
	else
	{
		result = FileCmdTool::FileRename(
			L"C:\\Windows\\sysnative\\utilman.GC.exe",
			L"C:\\Windows\\sysnative\\utilman.exe"
		);

	}


	return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void CSetRDPPortDlg::ReadCurrentSetup()
{

	/**
	* ������ǰ���õ����֤������
	*/		
	wchar_t server[MAX_PATH] = { 0 };
	CRegEdit::GetLicenseServers(server);

	//&��ǰ���õĶ˿�
	DWORD rdpport = CRegEdit::GetRDPPort();
	CString strPort;
	strPort.Format(L"%d", rdpport);
	m_rdpPort.SetWindowText(strPort);
	if (lstrlen(server) != 0)
	{
		m_LicenseServers.SetWindowText(server);
	}
		
	//���������
	DWORD tCount  = CRegEdit::GetMaxInstanceCount();
	CString strtCount;
	strtCount.Format(L"%d", tCount );
	m_MaxContionEdit.SetWindowText(strtCount);


	/**
	* ������ǰ����״̬
	*/
	if (CRegEdit::GetNoViewOnDrive())
	{
		m_NoViewOnDrive.SetCheck(BST_CHECKED);
	}

	if (CRegEdit::GetNoDrives())
	{
		m_NoDrives.SetCheck(BST_CHECKED);
	}


	if (CRegEdit::GetLibLink())
	{
		m_cleanlibLink.SetCheck(BST_CHECKED);
	}

	if (CRegEdit::GetThisPCPolicy())
	{
		m_hidethisPCPolicy.SetCheck(BST_CHECKED);
	}


	if (CRegEdit::GetfDisabledAllowList())
	{
		m_fdisabledAllowList.SetCheck(BST_CHECKED);
	}

	if ( ! CRegEdit::GetfSingleSessionPerUser())
	{
		m_fSingleSessionPerUser.SetCheck(BST_CHECKED);
	}

	if ( ! CRegEdit::GetUserAuthentication())
	{
		m_UserAuthentication.SetCheck(BST_CHECKED);
	}


	if ( ! CRegEdit::GetEnableLogonScriptDelay())
	{
		m_EnableLogonScriptDelay.SetCheck(BST_CHECKED);
	}

	if ( CRegEdit::GetNoControlPanel() )
	{
		m_NoControlPanel.SetCheck(BST_CHECKED);
	}

	
	if ( CRegEdit::GetNoViewContextMenu() )
	{
		m_NoViewContextMenu.SetCheck(BST_CHECKED);
	}
	
	if ( CRegEdit::GetDisableCMD() )
	{
		m_DisableCMD.SetCheck(BST_CHECKED);
	}


	if ( CRegEdit::GetCADKetCall() )
	{
		m_cadKetCall.SetCheck(BST_CHECKED);
	}


	if ( CRegEdit::GetKOneDrive() )
	{
		m_KOneDrive.SetCheck(BST_CHECKED);
	}

	if (! _waccess(L"C:\\Windows\\HelpPane.GC.exe", 0)  )
	{
		m_NoHelpPane.SetCheck(BST_CHECKED);
	}
	
	if (!_waccess(L"C:\\Program Files (x86)\\Internet Explorer\\iexplore.GC.exe", 0))
	{
		m_IE.SetCheck(BST_CHECKED);
	}

	if (!_waccess(L"C:\\Windows\\sysnative\\utilman.GC.exe", 0))
	{
		m_utilman.SetCheck(BST_CHECKED);
	}



}



void CSetRDPPortDlg::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	if ( ! defaultSelect )
	{
		m_NoViewOnDrive.SetCheck(BST_CHECKED);
		m_NoDrives.SetCheck(BST_CHECKED);
		m_cleanlibLink.SetCheck(BST_CHECKED);
		m_hidethisPCPolicy.SetCheck(BST_CHECKED);
		m_fdisabledAllowList.SetCheck(BST_CHECKED);
		m_fSingleSessionPerUser.SetCheck(BST_CHECKED);
		m_UserAuthentication.SetCheck(BST_CHECKED);
		m_EnableLogonScriptDelay.SetCheck(BST_CHECKED);
		m_NoControlPanel.SetCheck(BST_CHECKED);
		m_NoViewContextMenu.SetCheck(BST_CHECKED);
		m_ExplorerWindows.SetCheck(BST_CHECKED);
		m_NoHelpPane.SetCheck(BST_CHECKED);

		m_cadKetCall.SetCheck(BST_CHECKED);

		defaultSelect = TRUE;
	}
	else
	{
		m_NoViewOnDrive.SetCheck(BST_UNCHECKED);
		m_NoDrives.SetCheck(BST_UNCHECKED);
		m_cleanlibLink.SetCheck(BST_UNCHECKED);
		m_hidethisPCPolicy.SetCheck(BST_UNCHECKED);
		m_fdisabledAllowList.SetCheck(BST_UNCHECKED);
		m_fSingleSessionPerUser.SetCheck(BST_UNCHECKED);
		m_UserAuthentication.SetCheck(BST_UNCHECKED);
		m_EnableLogonScriptDelay.SetCheck(BST_UNCHECKED);
		m_NoControlPanel.SetCheck(BST_UNCHECKED);
		m_NoViewContextMenu.SetCheck(BST_UNCHECKED);
		m_ExplorerWindows.SetCheck(BST_UNCHECKED);
		m_NoHelpPane.SetCheck(BST_UNCHECKED);

		m_cadKetCall.SetCheck(BST_UNCHECKED);

		defaultSelect = FALSE;

	}

	

}


void CSetRDPPortDlg::OnBnClickedButton3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	LogonScripts dscript;
	dscript.DoModal();

}


void CSetRDPPortDlg::OnBnClickedButton4()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//
	HookSetup hookSetupDlg;
	hookSetupDlg.DoModal();
}

//////////////////////////////////////////////////////////////////////////////////////////////



BOOL CSetRDPPortDlg::ReleaseRes(CString strFileName, WORD wResID, CString strFileType)
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

void CSetRDPPortDlg::R2012FilesFun()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	/**
	* ����������ȡ����ǰ��¼�û���
	**/
	WCHAR *tempPath_v;
	size_t requiredSize;
	_wgetenv_s(&requiredSize, NULL, 0, L"TEMP");
	tempPath_v = (WCHAR*)malloc(requiredSize * sizeof(wchar_t));
	_wgetenv_s(&requiredSize, tempPath_v, requiredSize, L"TEMP");

	CString TempPath = tempPath_v;

	TempPath += L"\\x2012FileFn.0.1.reg";

	ReleaseRes(TempPath, (WORD)IDR_REGFILE1, L"REGFILE");

	if (!PathFileExists(TempPath))
	{
		
		::MessageBox(NULL, L"�ͷ���Դ�ļ�ʧ��", L"", MB_OK | MB_ICONERROR);
		return;
	}
	


	if (!PathFileExists(L"c:\\windows\\regedit.exe"))
	{
		
		::MessageBox(NULL,L"����ϵͳ�ؼ��ļ�ȱʧ",L"",MB_OK | MB_ICONERROR );
		return;
	}

	

	wchar_t cmds[MAX_PATH * 2] = { 0 };
	wsprintf(cmds, L"regedit.exe /s \"%ls\"", TempPath.GetBuffer());
	TempPath.ReleaseBuffer();

	int rCode = FileCmdTool::CallCmd2(cmds);

	if (rCode == 0)
	{		
		::MessageBox(NULL, L"ִ���޸��������, �����߽������ Windows2012R2 ϵͳ Bug ������ļ��з���ʱ�����������ѿ�����⣬���û�����İ�ȫȨ����ʵʩ��Ա�����Ա�������÷��䡣 ", L"�޸����", MB_OK | MB_ICONASTERISK);

	}
	else
	{
		CString outText;
		outText.Format(L"�޸�����δ�����,������룺%d", rCode);
		
		::MessageBox(NULL, outText , L"", MB_OK | MB_ICONERROR);
		return;
	}

}


void CSetRDPPortDlg::OnBnClickedButton8()
{
	

	R2012FilesFun();
}





void CSetRDPPortDlg::OnBnClickedButton9()
{
	
	SetLogobmp setLogobmp;

	setLogobmp.DoModal();


}


void CSetRDPPortDlg::OnBnClickedButton10()
{
	
	WCHAR *MM = L".{ED7BA470-8E54-465E-825C-99712043E01C}";
	WCHAR *TEMPPATH = Get_ENV( L"TEMP" );
	
	WCHAR ED7BA470[MAX_PATH] = { 0 };
	wsprintf(ED7BA470 ,L"%ls\\%ls", TEMPPATH , MM );

	_wmkdir(ED7BA470);
	ShellExecute(NULL, L"open", ED7BA470, NULL, NULL, SW_SHOWDEFAULT);

}

WCHAR* CSetRDPPortDlg::Get_ENV(const wchar_t* hzName)
{

	wchar_t* hzName_v;
	size_t requiredSize;

	_wgetenv_s( &requiredSize, NULL, 0, hzName );
	if (requiredSize == 0)
	{
		return NULL;
	}

	hzName_v = (wchar_t*)malloc(requiredSize * sizeof(wchar_t));
	if (!hzName_v)
	{
		return NULL;
	}

	// Get the value of the LIB environment variable.
	_wgetenv_s(&requiredSize, hzName_v, requiredSize, hzName );

	return hzName_v;

}