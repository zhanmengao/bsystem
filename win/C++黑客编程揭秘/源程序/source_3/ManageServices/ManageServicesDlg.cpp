// ManageServicesDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ManageServices.h"
#include "ManageServicesDlg.h"
#include <Winsvc.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CManageServicesDlg dialog

CManageServicesDlg::CManageServicesDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CManageServicesDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CManageServicesDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CManageServicesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CManageServicesDlg)
	DDX_Control(pDX, IDC_LIST_SERVICE, m_ServiceList);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CManageServicesDlg, CDialog)
	//{{AFX_MSG_MAP(CManageServicesDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_RADIO_WIN32, OnRadioWin32)
	ON_BN_CLICKED(IDC_RADIO_DRIVER, OnRadioDriver)
	ON_BN_CLICKED(IDC_BTN_START, OnBtnStart)
	ON_BN_CLICKED(IDC_BTN_STOP, OnBtnStop)
	ON_BN_CLICKED(IDC_BTN_EXIT, OnBtnExit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CManageServicesDlg message handlers

BOOL CManageServicesDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
    InitServiceList();
    ShowServiceList();
    ((CButton *)GetDlgItem(IDC_RADIO_WIN32))->SetCheck(BST_CHECKED);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CManageServicesDlg::OnPaint() 
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
HCURSOR CManageServicesDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

VOID CManageServicesDlg::InitServiceList()
{
    m_ServiceList.SetExtendedStyle(
        m_ServiceList.GetExtendedStyle()
        | LVS_EX_GRIDLINES
        | LVS_EX_FULLROWSELECT );
    
    m_ServiceList.InsertColumn(0, "服务名");
    m_ServiceList.InsertColumn(1, "显示名");
    m_ServiceList.InsertColumn(2, "状态");
    
    m_ServiceList.SetColumnWidth(0, 100);
    m_ServiceList.SetColumnWidth(1, 250);
    m_ServiceList.SetColumnWidth(2, 50);
}

VOID CManageServicesDlg::ShowServiceList(DWORD dwServiceType)
{
    m_ServiceList.DeleteAllItems();

    // 打开服务管理器
    SC_HANDLE hSCM = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

    if ( NULL == hSCM )
    {
        AfxMessageBox("OpenSCManger Error!");
        return ;
    }
    
    DWORD ServiceCount = 0;
    DWORD dwSize = 0;
    LPENUM_SERVICE_STATUS lpInfo;

    // 第一次调用
    BOOL bRet = EnumServicesStatus(hSCM,
                    dwServiceType, SERVICE_STATE_ALL,
                    NULL, 0, &dwSize,
                    &ServiceCount, NULL);

    // 由于我们没有给定接收服务列表的缓冲区，这里必定会调用失败
    // 失败的原因是ERROR_MORE_DATA
    // 说明需要更大的缓冲区来保存数据
    if ( !bRet && GetLastError() == ERROR_MORE_DATA )
    {
        // 分配缓冲区，保存服务列表
        lpInfo = (LPENUM_SERVICE_STATUS)(new BYTE[dwSize]);
        bRet = EnumServicesStatus(hSCM,
                    dwServiceType, SERVICE_STATE_ALL,
                    (LPENUM_SERVICE_STATUS)lpInfo,
                    dwSize, &dwSize,
                    &ServiceCount, NULL);
        if ( !bRet )
        {
            CloseServiceHandle(hSCM);
            return ;
        }
        // 获取逐个获取数据，添加至列表控件
        for ( DWORD i = 0; i < ServiceCount; i ++)
        {
            CString str;
            m_ServiceList.InsertItem(i, lpInfo[i].lpServiceName);
            m_ServiceList.SetItemText(i, 1, lpInfo[i].lpDisplayName);
            switch ( lpInfo[i].ServiceStatus.dwCurrentState )
            {
            case SERVICE_PAUSED:
                {
                    m_ServiceList.SetItemText(i, 2, "暂停");
                    break;
                }
            case SERVICE_STOPPED:
                {
                    m_ServiceList.SetItemText(i, 2, "停止");
                    break;
                }
            case SERVICE_RUNNING:
                {
                    m_ServiceList.SetItemText(i, 2, "运行");
                    break;
                }
            default:
                {
                    m_ServiceList.SetItemText(i, 2, "其他");
                }
            }
        }

        // 释放申请的空间
        delete lpInfo;
    }

    // 关闭服务管理器句柄
    CloseServiceHandle(hSCM);
}

void CManageServicesDlg::OnRadioWin32() 
{
    // TODO: Add your control notification handler code here
	ShowServiceList(SERVICE_WIN32);
}

void CManageServicesDlg::OnRadioDriver() 
{
    // TODO: Add your control notification handler code here
	ShowServiceList(SERVICE_DRIVER);
}

void CManageServicesDlg::OnBtnStart() 
{
	// TODO: Add your control notification handler code here
    // 选中服务的的索引
    POSITION Pos = m_ServiceList.GetFirstSelectedItemPosition();
    int nSelect = -1;
    
    while ( Pos )
    {
        nSelect = m_ServiceList.GetNextSelectedItem(Pos);
    }
    
    if ( -1 == nSelect )
    {
        AfxMessageBox("请选择要启动的服务");
        return ;
    }
    
    // 获取选中服务的服务名
    char szServiceName[MAXBYTE] = { 0 };
    m_ServiceList.GetItemText(nSelect, 0, szServiceName, MAXBYTE);

    SC_HANDLE hSCM = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if ( NULL == hSCM )
    {
        AfxMessageBox("OpenSCManager Error");
        return ;
    }
    
    // 打开指定的服务
    SC_HANDLE hSCService = OpenService(hSCM, szServiceName, SERVICE_ALL_ACCESS);
    
    // 启动服务
    BOOL bRet = StartService(hSCService, 0, NULL);
    if ( bRet == TRUE )
    {
        m_ServiceList.SetItemText(nSelect, 2, "运行");
    }
    else
    {
        AfxMessageBox("启动失败!");
    }

    CloseServiceHandle(hSCService);
    CloseServiceHandle(hSCM);
}

void CManageServicesDlg::OnBtnStop() 
{
	// TODO: Add your control notification handler code here
    // 选中服务的的索引
    POSITION Pos = m_ServiceList.GetFirstSelectedItemPosition();
    int nSelect = -1;
    
    while ( Pos )
    {
        nSelect = m_ServiceList.GetNextSelectedItem(Pos);
    }
    
    if ( -1 == nSelect )
    {
        AfxMessageBox("请选择要启动的服务");
        return ;
    }
    
    // 获取选中服务的服务名
    char szServiceName[MAXBYTE] = { 0 };
    m_ServiceList.GetItemText(nSelect, 0, szServiceName, MAXBYTE);

    SC_HANDLE hSCM = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if ( NULL == hSCM )
    {
        AfxMessageBox("OpenSCManager Error");
        return ;
    }
    
    // 打开指定的服务
    SC_HANDLE hSCService = OpenService(hSCM, szServiceName, SERVICE_ALL_ACCESS);
    SERVICE_STATUS ServiceStatus;
    // 停止服务
    BOOL bRet = ControlService(hSCService, SERVICE_CONTROL_STOP, &ServiceStatus);
    if ( bRet == TRUE )
    {
        m_ServiceList.SetItemText(nSelect, 2, "停止");
    }
    else
    {
        AfxMessageBox("停止失败!");
    }

    
    CloseServiceHandle(hSCService);
    CloseServiceHandle(hSCM);
}

void CManageServicesDlg::OnBtnExit() 
{
	// TODO: Add your control notification handler code here
	EndDialog(0);
}
