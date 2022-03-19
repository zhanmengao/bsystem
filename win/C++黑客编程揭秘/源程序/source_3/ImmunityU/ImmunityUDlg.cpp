// ImmunityUDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ImmunityU.h"
#include "ImmunityUDlg.h"

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
// CImmunityUDlg dialog

CImmunityUDlg::CImmunityUDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CImmunityUDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CImmunityUDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CImmunityUDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CImmunityUDlg)
	DDX_Control(pDX, IDC_COMBO_DRIVE, m_CbDrive);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CImmunityUDlg, CDialog)
	//{{AFX_MSG_MAP(CImmunityUDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_IMMUNITY, OnBtnImmunity)
	ON_BN_CLICKED(IDC_BTN_CANCEL, OnBtnCancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImmunityUDlg message handlers

BOOL CImmunityUDlg::OnInitDialog()
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
    InitComboDrive();
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CImmunityUDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CImmunityUDlg::OnPaint() 
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
HCURSOR CImmunityUDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CImmunityUDlg::InitComboDrive()
{
    char szDriveStr[MAXBYTE] = { 0 };
    char *pTmp = NULL;

    SetDlgItemText(IDC_COMBO_DRIVE, "��ѡ�������ߵĴ����̷�");
    
    GetLogicalDriveStrings(MAXBYTE, szDriveStr);
    pTmp = szDriveStr;

    while ( *pTmp )
    {
        m_CbDrive.AddString(pTmp);
        pTmp += 4;
    }
}

// ����autorun.inf�ļ���
#define AUTORUN "autorun.inf"
// �����޷�ɾ�����ļ���
#define ANTI    "\\anti...\\"

void CImmunityUDlg::OnBtnImmunity() 
{
	// TODO: Add your control notification handler code here
    char szPath[MAX_PATH] = { 0 };
    GetDlgItemText(IDC_COMBO_DRIVE, szPath, MAX_PATH);

    // ����autorun.inf�ļ���
    strcat(szPath, AUTORUN);
    BOOL bRet = CreateDirectory(szPath, NULL);
    if ( !bRet )
    {
        AfxMessageBox("�޷����߸��̷��� \
            �����Ѿ����ߣ����߸ô���Ϊ����д״̬��");
        return ;
    }

    // �����޷�ɾ�����ļ���
    strcat(szPath, ANTI);
    bRet = CreateDirectory(szPath, NULL);
    if ( !bRet )
    {
        AfxMessageBox("�޷����߸��̷��� \
            �����Ѿ����ߣ����߸ô���Ϊ����д״̬��");
    }
}

void CImmunityUDlg::OnBtnCancel() 
{
	// TODO: Add your control notification handler code here
    
    char szPath[MAX_PATH] = { 0 };
    // ɾ��ANTI...\Ŀ¼
    GetDlgItemText(IDC_COMBO_DRIVE, szPath, MAX_PATH);
    strcat(szPath, AUTORUN);
    strcat(szPath, ANTI);
    RemoveDirectory(szPath);
    
    ZeroMemory(szPath, MAX_PATH);
    // ɾ��autorun.infĿ¼
    GetDlgItemText(IDC_COMBO_DRIVE, szPath, MAX_PATH);
    strcat(szPath, AUTORUN);
    RemoveDirectory(szPath);
}
