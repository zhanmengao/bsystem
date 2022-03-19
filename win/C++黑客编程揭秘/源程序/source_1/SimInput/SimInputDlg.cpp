// SimInputDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SimInput.h"
#include "SimInputDlg.h"

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
// CSimInputDlg dialog

CSimInputDlg::CSimInputDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSimInputDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSimInputDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSimInputDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSimInputDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSimInputDlg, CDialog)
	//{{AFX_MSG_MAP(CSimInputDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_SIMKEYBD, OnBtnSimkeybd)
	ON_BN_CLICKED(IDC_BTN_SIMMOUSE, OnBtnSimmouse)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSimInputDlg message handlers

BOOL CSimInputDlg::OnInitDialog()
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

void CSimInputDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CSimInputDlg::OnPaint() 
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
HCURSOR CSimInputDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CSimInputDlg::OnBtnSimkeybd() 
{
	// TODO: Add your control notification handler code here
	// 找到窗口
    // 将其设置到前台并激活
    FindAndFocus();

    Sleep(1000);

    // 模拟F5三次
    keybd_event(VK_F5, 0, 0, 0);
    Sleep(1000);
    keybd_event(VK_F5, 0, 0, 0);
    Sleep(1000);
    keybd_event(VK_F5, 0, 0, 0);
}

void CSimInputDlg::OnBtnSimmouse() 
{
	// TODO: Add your control notification handler code here
	FindAndFocus();

    // 得到窗口在屏幕的坐标(x, y)
    POINT pt = { 0 };
    ::ClientToScreen(m_hWnd, &pt);

    // 设置鼠标位置
    SetCursorPos(pt.x + 36, pt.y + 395);

    // 模拟鼠标单击右键
    // 单击右键后浏览器会弹出右键菜单
    mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, 0);
    Sleep(100);
    mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);

    Sleep(1000);
    // 0x52 = r
    // 在弹出右键菜单后按下r键
    // 会刷新页面
    keybd_event(0x52, 0, 0, 0);
}

VOID CSimInputDlg::FindAndFocus()
{
    GetDlgItemText(IDC_EDIT_CAPTION, m_StrCaption);

    // 判断输入是否为空
    if ( m_StrCaption == "" )
    {
        return ;
    }

    m_hWnd = ::FindWindow(NULL, m_StrCaption.GetBuffer(0));

    // 该函数将创建指定窗口的线程设置到前台
    // 并且激活该窗口
    ::SetForegroundWindow(m_hWnd);
}
