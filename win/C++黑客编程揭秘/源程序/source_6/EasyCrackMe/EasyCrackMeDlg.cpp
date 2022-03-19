// EasyCrackMeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "EasyCrackMe.h"
#include "EasyCrackMeDlg.h"

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
// CEasyCrackMeDlg dialog

CEasyCrackMeDlg::CEasyCrackMeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEasyCrackMeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEasyCrackMeDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CEasyCrackMeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEasyCrackMeDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CEasyCrackMeDlg, CDialog)
	//{{AFX_MSG_MAP(CEasyCrackMeDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_END, OnBtnEnd)
	ON_BN_CLICKED(IDC_BTN_REG, OnBtnReg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEasyCrackMeDlg message handlers

BOOL CEasyCrackMeDlg::OnInitDialog()
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

void CEasyCrackMeDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CEasyCrackMeDlg::OnPaint() 
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
HCURSOR CEasyCrackMeDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CEasyCrackMeDlg::OnBtnEnd() 
{
	// TODO: Add your control notification handler code here
	SetDlgItemText(IDC_EDIT_USER, "");
    SetDlgItemText(IDC_EDIT_PASSWORD, "");
}

void CEasyCrackMeDlg::OnBtnReg() 
{
	// TODO: Add your control notification handler code here
    char szUser[MAXBYTE] = { 0 };
    char szPassword[MAXBYTE] = { 0 };
    char szTmpPassword[MAXBYTE] = { 0 };

    // 获取输入的帐号和密码
    GetDlgItemText(IDC_EDIT_USER, szUser, MAXBYTE);
    GetDlgItemText(IDC_EDIT_PASSWORD, szPassword, MAXBYTE);

    // 判断帐号是否为空
    if ( strlen(szUser) == 0 )
    {
        return ;
    }

    // 判断密码是否为空
    if ( strlen(szPassword) == 0 )
    {
        return ;
    }

    // 判断帐号长度是否小于7
    if ( strlen(szUser) < 7 )
    {
        return ;
    }

    // 根据帐号生成密码
    for ( int i = 0; i < strlen(szUser); i ++ )
    {
        if ( szUser[i] == 'Z' 
            || szUser[i] == 'z'
            || szUser[i] == '9' )
        {
            szTmpPassword[i] = szUser[i];
        }
        else
        {
            szTmpPassword[i] = szUser[i] + 1;
        }
    }

    // 把生成的密码和输入的密码进行匹配
    if ( strcmp(szTmpPassword, szPassword) == 0 )
    {
        MessageBox("密码正确");
    } 
    else
    {
        MessageBox("密码错误");
    }
}
