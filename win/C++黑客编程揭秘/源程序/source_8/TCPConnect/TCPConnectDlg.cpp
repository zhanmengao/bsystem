// TCPConnectDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TCPConnect.h"
#include "TCPConnectDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTCPConnectDlg dialog

CTCPConnectDlg::CTCPConnectDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTCPConnectDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTCPConnectDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTCPConnectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTCPConnectDlg)
	DDX_Control(pDX, IDC_LIST_PORT, m_ListPort);
	DDX_Control(pDX, IDC_IPADDR, m_IpAddr);
	DDX_Control(pDX, IDC_BTN_SCAN, m_BtnScan);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTCPConnectDlg, CDialog)
	//{{AFX_MSG_MAP(CTCPConnectDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_SCAN, OnBtnScan)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTCPConnectDlg message handlers

BOOL CTCPConnectDlg::OnInitDialog()
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

void CTCPConnectDlg::OnPaint() 
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
HCURSOR CTCPConnectDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CTCPConnectDlg::OnBtnScan() 
{
	// TODO: Add your control notification handler code here
    m_BtnScan.EnableWindow(FALSE);
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    // IP地址
    DWORD dwIpAddr = 0;
    // 开始端口、结束端口和当前端口
    WORD wStartPort = 0, wEndPort = 0, wCurrPort = 0;
    
    // 得到IP地址
    m_IpAddr.GetAddress(dwIpAddr);
    // 得到开始端口号和结束端口号
    wStartPort = GetDlgItemInt(IDC_EDIT1, FALSE, FALSE);
    wEndPort = GetDlgItemInt(IDC_EDIT2, FALSE, FALSE);

    CTime starttime, endtime;

    // 获得扫描开始时间
    starttime = CTime::GetCurrentTime();

    // 逐个连接从开始端口至结束端口之间的所有端口
    for ( wCurrPort = wStartPort; wCurrPort <= wEndPort; wCurrPort ++ )
    {
        SOCKET s = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

        struct sockaddr_in ServAddr;
        ServAddr.sin_family = AF_INET;
        ServAddr.sin_addr.S_un.S_addr = htonl(dwIpAddr);
        ServAddr.sin_port = htons(wCurrPort);

        // 连接当前端口
        if ( connect(s, (SOCKADDR*)&ServAddr, sizeof(SOCKADDR)) == 0 )
        {
            CString strPort;
            strPort.Format("[%d] is open", wCurrPort);
            m_ListPort.AddString(strPort);
        }

        closesocket(s);
    }

    // 获得扫描结束时间
    endtime = CTime::GetCurrentTime();

    // 计算开始时间和结束时间的差值
    CTimeSpan t = endtime - starttime;
    CString str;
    str.Format("耗时：%02d:%02d:%02d", t.GetHours(), t.GetMinutes(), t.GetSeconds());

    m_ListPort.AddString(str);

    m_BtnScan.EnableWindow(TRUE);

    WSACleanup();
}
