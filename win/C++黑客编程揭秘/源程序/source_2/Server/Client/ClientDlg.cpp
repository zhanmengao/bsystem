// ClientDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Client.h"
#include "ClientDlg.h"

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
// CClientDlg dialog

CClientDlg::CClientDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CClientDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CClientDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CClientDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CClientDlg, CDialog)
	//{{AFX_MSG_MAP(CClientDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_MESSAGE(UM_CLIENT, OnSock)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BTN_CONNECT, OnBtnConnect)
	ON_BN_CLICKED(IDC_BTN_SEND, OnBtnSend)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClientDlg message handlers

BOOL CClientDlg::OnInitDialog()
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
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CClientDlg::OnPaint() 
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
HCURSOR CClientDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

VOID CClientDlg::OnSock(WPARAM wParam, LPARAM lParam)
{
    if ( WSAGETSELECTERROR(lParam) )
    {
        return ;
    }
    
    switch ( WSAGETSELECTEVENT(lParam))
    {
        // 处理FD_ACCEPT
    case FD_CONNECT:
        {
            (GetDlgItem(IDC_SZCMD))->EnableWindow(TRUE);
            (GetDlgItem(IDC_BTN_SEND))->EnableWindow(TRUE);
            (GetDlgItem(IDC_IPADDR))->EnableWindow(FALSE);

            SetDlgItemText(IDC_BTN_CONNECT, "断开连接");
            m_StrMsg = "连接成功";
            break;
        }
        // 处理FD_READ
    case FD_READ:
        {
            DATA_MSG DataMsg;
            recv(m_Socket, (char *)&DataMsg, sizeof(DataMsg), 0);
            DispatchMsg((char *)&DataMsg);
            break;
        }
        // 处理FD_CLOSE
    case FD_CLOSE:
        {
            (GetDlgItem(IDC_SZCMD))->EnableWindow(FALSE);
            (GetDlgItem(IDC_BTN_SEND))->EnableWindow(FALSE);
            (GetDlgItem(IDC_IPADDR))->EnableWindow(TRUE);
            closesocket(m_Socket);
            m_StrMsg = "对方关闭连接";
            break;
        }
    }
    
    InsertMsg();
}

void CClientDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
    closesocket(m_Socket);
    WSACleanup();
	
	CDialog::OnClose();
}

VOID CClientDlg::InsertMsg()
{
    CString strMsg;
    GetDlgItemText(IDC_MSG, strMsg);
    
    m_StrMsg += "\r\n";
    m_StrMsg += "----------------------------------------\r\n";
    m_StrMsg += strMsg;
    SetDlgItemText(IDC_MSG, m_StrMsg);
    m_StrMsg = "";
}

void CClientDlg::OnBtnConnect() 
{
	// TODO: Add your control notification handler code here
    char szBtnName[10] = { 0 };
    GetDlgItemText(IDC_BTN_CONNECT, szBtnName, 10);

    // 断开连接
    if ( !lstrcmp(szBtnName, "断开连接") )
    {
        SetDlgItemText(IDC_BTN_CONNECT, "连接");
        (GetDlgItem(IDC_SZCMD))->EnableWindow(FALSE);
        (GetDlgItem(IDC_BTN_SEND))->EnableWindow(FALSE);
        (GetDlgItem(IDC_IPADDR))->EnableWindow(TRUE);
        closesocket(m_Socket);
        m_StrMsg = "主动断开连接";
        InsertMsg();
        return ;
    }

    // 连接远程服务器端
    char szIpAddr[MAXBYTE] = { 0 };
    GetDlgItemText(IDC_IPADDR, szIpAddr, MAXBYTE);

    m_Socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    WSAAsyncSelect(m_Socket, GetSafeHwnd(), UM_CLIENT, FD_READ | FD_CONNECT | FD_CLOSE);

    sockaddr_in ServerAddr;
    ServerAddr.sin_family = AF_INET;
    ServerAddr.sin_addr.S_un.S_addr = inet_addr(szIpAddr);
    ServerAddr.sin_port = htons(5555);

    connect(m_Socket, (SOCKADDR*)&ServerAddr, sizeof(ServerAddr));
}

VOID CClientDlg::DispatchMsg(char *szBuf)
{
    DATA_MSG DataMsg;
    memcpy((void*)&DataMsg, (const void *)szBuf, sizeof(DATA_MSG));

    if ( DataMsg.bType == TEXTMSG )
    {
        m_StrMsg = DataMsg.szValue;
    } 
    else
    {
        if ( DataMsg.bClass == SYSTEMINFO )
        {
            ParseSysInfo((PSYS_INFO)&DataMsg.szValue);
        }
    }
}

void CClientDlg::OnBtnSend() 
{
	// TODO: Add your control notification handler code here
    char szBuf[MAXBYTE] = { 0 };
    GetDlgItemText(IDC_SZCMD, szBuf, MAXBYTE);

    send(m_Socket, szBuf, MAXBYTE, 0);
}

VOID CClientDlg::ParseSysInfo(PSYS_INFO SysInfo)
{
    if ( SysInfo->OsVer.dwPlatformId == VER_PLATFORM_WIN32_NT )
    {
        if ( SysInfo->OsVer.dwMajorVersion == 5 && SysInfo->OsVer.dwMinorVersion == 1 )
        {
            m_StrMsg.Format("对方系统信息:\r\n\t Windows XP %s", SysInfo->OsVer.szCSDVersion);
        }
        else if ( SysInfo->OsVer.dwMajorVersion == 5 && SysInfo->OsVer.dwMinorVersion == 0)
        {
            m_StrMsg.Format("对方系统信息:\r\n\t Windows 2K");
        }
    }
    else
    {
        m_StrMsg.Format("对方系统信息:\r\n\t Other System \r\n");
    }
    
    m_StrMsg += "\r\n";
    m_StrMsg += "\t Computer Name is ";
    m_StrMsg += SysInfo->szComputerName;
    m_StrMsg += "\r\n";
    m_StrMsg += "\t User Name is ";
    m_StrMsg += SysInfo->szUserName;
}
