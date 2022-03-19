// LoadNtDriverDlg.cpp : implementation file
//

#include "stdafx.h"
#include "LoadNtDriver.h"
#include "LoadNtDriverDlg.h"

#include <winsvc.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLoadNtDriverDlg dialog

CLoadNtDriverDlg::CLoadNtDriverDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLoadNtDriverDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLoadNtDriverDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CLoadNtDriverDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLoadNtDriverDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CLoadNtDriverDlg, CDialog)
	//{{AFX_MSG_MAP(CLoadNtDriverDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_LOAD, OnLoad)
	ON_BN_CLICKED(IDC_UNLOAD, OnUnload)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLoadNtDriverDlg message handlers

BOOL CLoadNtDriverDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CLoadNtDriverDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CLoadNtDriverDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CLoadNtDriverDlg::OnLoad() 
{
	// TODO: Add your control notification handler code here
    char szDriverPath[MAX_PATH];
    char szFileName[MAXBYTE];
    
    SC_HANDLE hScm;
    SC_HANDLE hService;

    GetDlgItemText(IDC_DRIVER_PATH, szDriverPath, MAX_PATH);
    // 获得驱动的文件名
    ::_splitpath(szDriverPath, NULL, NULL, szFileName, NULL);

    // 打开服务控制管理器
    hScm = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    
    // 创建驱动所对应的服务
    hService = CreateService(hScm, szFileName, szFileName,
                        SERVICE_ALL_ACCESS, 
                        SERVICE_KERNEL_DRIVER,
                        SERVICE_DEMAND_START,
                        SERVICE_ERROR_IGNORE,
                        szDriverPath, NULL, NULL,
                        NULL, NULL, NULL);

    // 启动服务
    StartService(hService, NULL, NULL);

    // 关闭句柄
    CloseServiceHandle(hService);
    CloseServiceHandle(hScm);
}

void CLoadNtDriverDlg::OnUnload() 
{
	// TODO: Add your control notification handler code here
    char szDriverPath[MAX_PATH];
    char szFileName[MAXBYTE];
    
    SC_HANDLE hScm;
    SC_HANDLE hService;

    SERVICE_STATUS status;
    
    GetDlgItemText(IDC_DRIVER_PATH, szDriverPath, MAX_PATH);
    // 获得驱动的文件名
    ::_splitpath(szDriverPath, NULL, NULL, szFileName, NULL);

    // 打开服务控制管理器
    hScm = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    // 打开驱动对应的服务
    hService = OpenService(hScm, szFileName, SERVICE_ALL_ACCESS);
    // 停止驱动
    ControlService(hService, SERVICE_CONTROL_STOP, &status);
    // 卸载驱动程序
    DeleteService(hService);

    // 关闭句柄
    CloseServiceHandle(hService);
    CloseServiceHandle(hScm);
}
