
// ScanAdminPageDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ScanAdminPage.h"
#include "ScanAdminPageDlg.h"
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


// CScanAdminPageDlg 对话框



CScanAdminPageDlg::CScanAdminPageDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CScanAdminPageDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CScanAdminPageDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST1, m_ScanList);
}

BEGIN_MESSAGE_MAP(CScanAdminPageDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BUTTON1, &CScanAdminPageDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CScanAdminPageDlg 消息处理程序

BOOL CScanAdminPageDlg::OnInitDialog()
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
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CScanAdminPageDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CScanAdminPageDlg::OnPaint()
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
HCURSOR CScanAdminPageDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CScanAdminPageDlg::OnBnClickedButton1()
{
    // TODO: 在此添加控件通知处理程序代码
    // 清除列表框内容
    m_ScanList.DeleteAllItems();

    // 获取输入的URL内容
    CString strURL;
    GetDlgItemText(IDC_EDIT1, strURL);

    // 创建事件
    m_hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

    // 创建扫描线程
    HANDLE hThread = NULL;
    hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ScanThread, this, 0, NULL);
    WaitForSingleObject(m_hEvent, INFINITE);

    ResetEvent(m_hEvent);
}


DWORD WINAPI CScanAdminPageDlg::ScanThread(LPVOID lpParam)
{
    CScanAdminPageDlg *pThis = (CScanAdminPageDlg*)lpParam;

    SetEvent(pThis->m_hEvent);

    // 在线程函数中获取下拉选框的内容
    // 下拉选项是asp和php两项
    CString strWebType;
    pThis->GetDlgItemText(IDC_COMBO1, strWebType);

    // 在线程函数中获取扫描URL地址
    CString strUrl;
    pThis->GetDlgItemText(IDC_EDIT1, strUrl);
    
    // 通过下拉选项来构造字典
    // asp.dic 或 php.dic
    char szFileName[MAX_PATH] = { 0 };
    wsprintf(szFileName, "%s.dic", strWebType);

    // 打开字典文件文件
    // 从中读取可能的后台页面
    FILE *DicFile = NULL;
    fopen_s(&DicFile, szFileName, "r");
    char szDic[MAXBYTE] = { 0 };
    while ( fgets(szDic, MAXBYTE, DicFile) )
    {
        if ( szDic[lstrlen(szDic) - 1] == '\n' )
        {
            szDic[lstrlen(szDic) - 1] = NULL;
        }

        // 扫描URL地址和字典中的页面文件进行拼接
        CString strCheckUrl = strUrl + szDic;

        // 判断页面是否存在
        // 存在则在地址的结尾增加“[OK]”字样
        if ( pThis->CheckUrl(strCheckUrl) )
        {
            strCheckUrl += "[OK]";
            pThis->m_ScanList.InsertItem(0, strCheckUrl);
            continue;
        }

        // 将扫描的地址添加至字典
        pThis->m_ScanList.InsertItem(pThis->m_ScanList.GetItemCount(), strCheckUrl);
    }

    return 0;
}

BOOL CScanAdminPageDlg::CheckUrl(CString strUrl)
{
    // 建立一个SESSION
    CInternetSession session("ScanAdminPage");
    // 建立一个HTTP连接
    CHttpConnection *pServer = NULL;
    // 获取一个HTTP文件
    CHttpFile *pFile = NULL;

    // 检测输入的URL是否符合格式，并把URL解析
    CString strServerName;  // 服务器地址
    CString strObject;      // URL指向的对象
    INTERNET_PORT nPort;    // 端口号
    DWORD dwServiceType;    // 服务类型

    // URL解析失败则返回
    // 解析扫描的URL
    if ( !AfxParseURL(strUrl, dwServiceType, strServerName, strObject, nPort) )
    {
        return NULL;
    }

    // 服务类型错误
    if ( dwServiceType != INTERNET_SERVICE_HTTP )
    {
        return NULL;
    }

    // 配置连接服务器的地址、端口，并获取该HTTP连接
    pServer = session.GetHttpConnection(strServerName, nPort);
    // 打开该HTTP连接
    pFile = pServer->OpenRequest(CHttpConnection::HTTP_VERB_GET, strObject, NULL, 1, NULL, NULL, INTERNET_FLAG_EXISTING_CONNECT | INTERNET_FLAG_NO_AUTO_REDIRECT);
    
    try
    {
        // 发起请求
        pFile->SendRequest();
    }
    catch (CException* e)
    {
        return NULL;
    }

    DWORD dwRet;
    // 获取该请求的回应状态码
    pFile->QueryInfoStatusCode(dwRet);

    BOOL bRet = FALSE;
    
    // HTTP返回值为200表示成功
    if ( dwRet == 200 )
    {
        bRet = TRUE;
    }

    // 释放指针
    if ( pFile != NULL )
    {
        delete pFile;
    }
    if ( pServer != NULL )
    {
        delete pServer;
    }

    // 关闭会话
    session.Close();

    return bRet;
}
