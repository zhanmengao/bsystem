
// uploadmumaDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "uploadmuma.h"
#include "uploadmumaDlg.h"
#include "afxdialogex.h"
#include <WinSock2.h>
#pragma comment (lib, "ws2_32")

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


// CuploadmumaDlg 对话框



CuploadmumaDlg::CuploadmumaDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CuploadmumaDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CuploadmumaDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_IPADDRESS1, m_IpAddr);
}

BEGIN_MESSAGE_MAP(CuploadmumaDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BUTTON1, &CuploadmumaDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CuploadmumaDlg 消息处理程序

BOOL CuploadmumaDlg::OnInitDialog()
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

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CuploadmumaDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CuploadmumaDlg::OnPaint()
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
HCURSOR CuploadmumaDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CuploadmumaDlg::OnBnClickedButton1()
{
    // TODO: 在此添加控件通知处理程序代码
    WSAData wsa = { 0 };
    // 初始化
    WSAStartup(MAKEWORD(2, 2), &wsa);

    // 获取IP地址
    DWORD dwIPAddr = 0;
    m_IpAddr.GetAddress(dwIPAddr);

    // 获取端口号
    WORD dwPort = 0;
    dwPort = GetDlgItemInt(IDC_PORT);

    // 创建SOCKET
    SOCKET s = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    sockaddr_in sockaddr = { 0 };

    sockaddr.sin_family = AF_INET;
    sockaddr.sin_addr.S_un.S_addr = htonl(dwIPAddr);
    sockaddr.sin_port = htons(dwPort);

    // 连接服务器
    int nRet = connect(s, (SOCKADDR*)&sockaddr, sizeof(SOCKADDR));

    // 获取从BURP截取的HTTP的request包
    char szText[2048] = { 0 };
    GetDlgItemText(IDC_TEXT, szText, 2048);
    // 发送包
    nRet = send(s, szText, lstrlen(szText), 0);

    closesocket(s);

    WSACleanup();
}
