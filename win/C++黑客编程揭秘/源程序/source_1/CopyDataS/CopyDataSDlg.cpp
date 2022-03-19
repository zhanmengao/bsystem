// CopyDataSDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CopyDataS.h"
#include "CopyDataSDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCopyDataSDlg dialog

CCopyDataSDlg::CCopyDataSDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCopyDataSDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCopyDataSDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCopyDataSDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCopyDataSDlg)
	DDX_Control(pDX, IDC_LIST_REVCDATA, m_ListData);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCopyDataSDlg, CDialog)
	//{{AFX_MSG_MAP(CCopyDataSDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_COPYDATA()
	ON_BN_CLICKED(IDC_BTN_DELALL, OnBtnDelall)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCopyDataSDlg message handlers

BOOL CCopyDataSDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CCopyDataSDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CCopyDataSDlg::OnPaint() 
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
HCURSOR CCopyDataSDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

BOOL CCopyDataSDlg::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct) 
{
	// TODO: Add your message handler code here and/or call default
    CString strText;

    // 通过发送消息的窗口句柄获得窗口对应的进程号，即PID
    DWORD dwPid = 0;
    ::GetWindowThreadProcessId(pWnd->m_hWnd, &dwPid);
	
    // 格式化字符串并添加至列表框中
    strText.Format("PID=[%d]的进程发来的消息为：%s", 
                dwPid, pCopyDataStruct->lpData);

    m_ListData.AddString(strText);

    // 获取本地时间
    SYSTEMTIME st;
    GetLocalTime(&st);

    CString strTime;
    strTime.Format("%02d:%02d:%02d", st.wHour, st.wMinute, st.wSecond);

    // 将本地时间发送给客户端程序
    COPYDATASTRUCT cds;
    cds.dwData = 0;
    cds.cbData = strTime.GetLength() + 1;
    cds.lpData = strTime.GetBuffer(cds.cbData);

    // 注意SendMessage()函数的第三个参数为NULL
    ::SendMessage(pWnd->m_hWnd, WM_COPYDATA, NULL, (LPARAM)&cds);

	return CDialog::OnCopyData(pWnd, pCopyDataStruct);
}

void CCopyDataSDlg::OnBtnDelall() 
{
	// TODO: Add your control notification handler code here

    // 清空列表框内容
    while ( m_ListData.GetCount() )
    {
        m_ListData.DeleteString(0);
    }
}
