// ServerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Server.h"
#include "ServerDlg.h"

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
// CServerDlg dialog

CServerDlg::CServerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CServerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CServerDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CServerDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CServerDlg, CDialog)
	//{{AFX_MSG_MAP(CServerDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_MESSAGE(UM_SERVER, OnSock)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CServerDlg message handlers

BOOL CServerDlg::OnInitDialog()
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
    // 初始化Winsock库
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    
    // 创建套接字并将其设置为非阻塞模式 
    m_ListenSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    WSAAsyncSelect(m_ListenSock, GetSafeHwnd(), UM_SERVER, FD_ACCEPT);

    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.S_un.S_addr = ADDR_ANY;
    addr.sin_port = htons(5555);

    // 绑定IP地址及5555端口，并处于监听状态
    bind(m_ListenSock, (SOCKADDR*)&addr, sizeof(addr));
    listen(m_ListenSock, 1);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CServerDlg::OnPaint() 
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
HCURSOR CServerDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

VOID CServerDlg::OnSock(WPARAM wParam, LPARAM lParam)
{
    if ( WSAGETSELECTERROR(lParam) )
    {
        return ;
    }

    switch ( WSAGETSELECTEVENT(lParam))
    {
        // 处理FD_ACCEPT
    case FD_ACCEPT:
        {
            sockaddr_in ClientAddr;
            int nSize = sizeof(ClientAddr);
            
            m_ClientSock = accept(m_ListenSock, (SOCKADDR*)&ClientAddr, &nSize);
            WSAAsyncSelect(m_ClientSock, GetSafeHwnd(), UM_SERVER, FD_READ | FD_CLOSE);
            m_StrMsg.Format("请求地址是%s:%d", inet_ntoa(ClientAddr.sin_addr), ntohs(ClientAddr.sin_port));

            DATA_MSG DataMsg;
            DataMsg.bType = TEXTMSG;
            DataMsg.bClass = 0;
            lstrcpy(DataMsg.szValue, HELPMSG);
            send(m_ClientSock, (const char *)&DataMsg, sizeof(DataMsg), 0);

            break;
        }
        // 处理FD_READ
    case FD_READ:
        {
            char szBuf[MAXBYTE] = { 0 };
            recv(m_ClientSock, szBuf, MAXBYTE, 0);
            DispatchMsg(szBuf);
            m_StrMsg = "对方发来命令:";
            m_StrMsg += szBuf;
            break;
        }
        // 处理FD_CLOSE
    case FD_CLOSE:
        {
            closesocket(m_ClientSock);
            m_StrMsg = "对方关闭连接";
            break;
        }
    }
    
    InsertMsg();
}

void CServerDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
    // 关闭监听套接字，并释放Winsock库
    closesocket(m_ClientSock);
    closesocket(m_ListenSock);
    WSACleanup();
	
	CDialog::OnClose();
}

VOID CServerDlg::InsertMsg()
{
    CString strMsg;
    GetDlgItemText(IDC_MSG, strMsg);

    m_StrMsg += "\r\n";
    m_StrMsg += "----------------------------------------\r\n";
    m_StrMsg += strMsg;
    SetDlgItemText(IDC_MSG, m_StrMsg);
    m_StrMsg = "";
}

VOID CServerDlg::DispatchMsg(char *szBuf)
{
    DATA_MSG DataMsg;
    ZeroMemory((void*)&DataMsg, sizeof(DataMsg));

    if ( !strcmp(szBuf, "help") )
    {
        DataMsg.bType = TEXTMSG;
        DataMsg.bClass = 0;
        lstrcpy(DataMsg.szValue, HELPMSG);
    } 
    else if ( !strcmp(szBuf, "getsysinfo"))
    {
        SYS_INFO SysInfo;
        GetSysInfo(&SysInfo);
        DataMsg.bType = BINARYMSG;
        DataMsg.bClass = SYSINFO;
        memcpy((void *)DataMsg.szValue, (const char *)&SysInfo, sizeof(DataMsg));
    }
    else if ( !strcmp(szBuf, "open") )
    {
        SetCdaudio(TRUE);
        DataMsg.bType = TEXTMSG;
        DataMsg.bClass = 0;
        lstrcpy(DataMsg.szValue, "open命令执行完成");
    }
    else if ( !strcmp(szBuf, "close") )
    {
        SetCdaudio(FALSE);
        DataMsg.bType = TEXTMSG;
        DataMsg.bClass = 0;
        lstrcpy(DataMsg.szValue, "close命令执行完成");
    }
    else if ( !strcmp(szBuf, "swap") )
    {
        SetMouseButton(TRUE);
        DataMsg.bType = TEXTMSG;
        DataMsg.bClass = 0;
        lstrcpy(DataMsg.szValue, "swap命令执行完成");
    }
    else if ( !strcmp(szBuf, "restore") )
    {
        SetMouseButton(FALSE);
        DataMsg.bType = TEXTMSG;
        DataMsg.bClass = 0;
        lstrcpy(DataMsg.szValue, "restore命令执行完成");
    }
    else
    {
        DataMsg.bType = TEXTMSG;
        DataMsg.bClass = 0;
        lstrcpy(DataMsg.szValue, "无效的指令");
    }

    // 发送命令执行情况给客户端
    send(m_ClientSock, (const char *)&DataMsg, sizeof(DataMsg), 0);
}

VOID CServerDlg::GetSysInfo(PSYS_INFO SysInfo)
{
    unsigned long nSize = 0;
    
    SysInfo->OsVer.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    GetVersionEx(&SysInfo->OsVer);
    nSize = NAME_LEN;
    GetComputerName(SysInfo->szComputerName, &nSize);
    nSize = NAME_LEN;
    GetUserName(SysInfo->szUserName, &nSize);
}

VOID CServerDlg::SetCdaudio(BOOL bOpen)
{
    if ( bOpen )
    {
        // 打开光驱
        mciSendString("set cdaudio door open", NULL, NULL, NULL);
    } 
    else
    {
        // 关闭光驱
        mciSendString("set cdaudio door closed", NULL, NULL, NULL);
    }
}

VOID CServerDlg::SetMouseButton(BOOL bSwap)
{
    if ( bSwap)
    {
        // 交换
        SwapMouseButton(TRUE);
    } 
    else
    {
        // 恢复
        SwapMouseButton(FALSE);
    }
}
