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
	ON_BN_CLICKED(IDC_BTN_TRANSLATE, OnBtnTranslate)
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
    int n = 0;
    while ( n = m_ListPort.GetCount() )
    {
        m_ListPort.DeleteString(0);
    }

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

    struct in_addr destIp;
    destIp.S_un.S_addr = htonl(dwIpAddr);
    BOOL bRet = MyPing(inet_ntoa(destIp));
    if ( bRet == FALSE )
    {
        m_ListPort.AddString("主机不存在或有防火墙!");
        m_BtnScan.EnableWindow(TRUE);
        return ;
    }

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

        TIMEVAL TimeOut;
        FD_SET mask;

        unsigned long mode=1;    // ioctlsocket函数的最后一个参数

        // 获取超时时间
        int nTimeOut = GetDlgItemInt(IDC_EDIT_SCAN_TIME, FALSE, FALSE);

        TimeOut.tv_sec = 0;
        // 设置超时毫秒数
        TimeOut.tv_usec = nTimeOut;    
        
        FD_ZERO(&mask);   
        FD_SET(s, &mask);

        // 设置为非阻塞模式
        ioctlsocket(s, FIONBIO, &mode);   
        
        connect(s, (struct sockaddr *)&ServAddr, sizeof(ServAddr));

        // 查询可写入状态
        int ret = select(0, NULL, &mask, NULL, &TimeOut);    
        
        if( ret != 0 && ret != -1 )
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

void CTCPConnectDlg::OnBtnTranslate() 
{
	// TODO: Add your control notification handler code here
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    char szDnsName[MAXBYTE] = { 0 };
    GetDlgItemText(IDC_EDIT3, szDnsName, MAXBYTE);

    HOSTENT *pHostent;
    struct sockaddr_in sAddr;

    pHostent = gethostbyname(szDnsName);

    if ( pHostent->h_addr_list[0] )
    {
        memcpy(&sAddr.sin_addr.s_addr, pHostent->h_addr_list[0], pHostent->h_length);
        m_IpAddr.SetAddress(ntohl(sAddr.sin_addr.S_un.S_addr));
    }
    else
    {
        MessageBox("转换失败！请输入正确的域名！");
    }

    WSACleanup();
}

// ICMP协议结构体定义
struct icmp_header
{
    unsigned char icmp_type;    // 消息类型
    unsigned char icmp_code;    // 代码
    unsigned short icmp_checksum;    // 校验和
    unsigned short icmp_id;     // 用来惟一标识此请求的ID号，通常设置为进程ID
    unsigned short icmp_sequence;   // 序列号
    unsigned long icmp_timestamp;   // 时间戳
};

// ICMP结构体大小
#define ICMP_HEADER_SIZE sizeof(icmp_header)

// 请求回显
#define ICMP_ECHO_REQUEST 0x08
// 回显应答
#define ICMP_ECHO_REPLY 0x00

// 计算校验和
unsigned short chsum(struct icmp_header *picmp, int len)
{
    long sum = 0;
    unsigned short *pusicmp = (unsigned short *)picmp;
    
    while ( len > 1 )
    {
        sum += *(pusicmp++);
        if ( sum & 0x80000000 )
        {
            sum = (sum & 0xffff) + (sum >> 16);
        }
        len -= 2;
    }
    
    if ( len )
    {
        sum += (unsigned short)*(unsigned char *)pusicmp;
    }
    
    while ( sum >> 16 )
    {
        sum = (sum & 0xffff) + (sum >> 16);
    }
    
    return (unsigned short)~sum;
}

BOOL CTCPConnectDlg::MyPing(char *szDestIp)
{
    BOOL bRet = TRUE;
    WSADATA wsaData;
    int nTimeOut = 1000;
    char szBuff[ICMP_HEADER_SIZE + 32] = { 0 };
    icmp_header *pIcmp = (icmp_header *)szBuff;
    char icmp_data[32] = { 0 };
    
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    // 创建原始套接字
    SOCKET s = socket(PF_INET, SOCK_RAW, IPPROTO_ICMP);
    
    // 设置接收超时
    setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (char const*)&nTimeOut, sizeof(nTimeOut));
    
    // 设置目的地址
    sockaddr_in dest_addr;
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_addr.S_un.S_addr = inet_addr(szDestIp);
    dest_addr.sin_port = htons(0);
    
    // 构造ICMP封包
    pIcmp->icmp_type = ICMP_ECHO_REQUEST;
    pIcmp->icmp_code = 0;
    pIcmp->icmp_id = (USHORT)::GetCurrentProcessId();
    pIcmp->icmp_sequence = 0;
    pIcmp->icmp_timestamp = 0;
    pIcmp->icmp_checksum = 0;
    
    // 拷贝数据
    memcpy((szBuff + ICMP_HEADER_SIZE), "abcdefghijklmnopqrstuvwabcdefghi", 32);
    
    // 计算校验和
    pIcmp->icmp_checksum = chsum((struct icmp_header *)szBuff, sizeof(szBuff));
    
    sockaddr_in from_addr;
    char szRecvBuff[1024];
    int nLen = sizeof(from_addr);
    sendto(s, szBuff, sizeof(szBuff), 0, (SOCKADDR *)&dest_addr, sizeof(SOCKADDR));
    bRet = recvfrom(s, szRecvBuff, 1024, 0, (SOCKADDR *)&from_addr, &nLen);

    if ( bRet == SOCKET_ERROR )
    {
        if ( WSAGetLastError() == WSAETIMEDOUT )
        {
            bRet = FALSE;
        }
    }
    
    return bRet;
}
