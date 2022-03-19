// MultiPortScanDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MultiPortScan.h"
#include "MultiPortScanDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMultiPortScanDlg dialog

CMultiPortScanDlg::CMultiPortScanDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMultiPortScanDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMultiPortScanDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMultiPortScanDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMultiPortScanDlg)
	DDX_Control(pDX, IDC_IPTREE, m_IpTree);
	DDX_Control(pDX, IDC_IPEND, m_IpEnd);
	DDX_Control(pDX, IDC_IPSTART, m_IpStart);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMultiPortScanDlg, CDialog)
	//{{AFX_MSG_MAP(CMultiPortScanDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTNSCAN, OnBtnscan)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMultiPortScanDlg message handlers

BOOL CMultiPortScanDlg::OnInitDialog()
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

void CMultiPortScanDlg::OnPaint() 
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
HCURSOR CMultiPortScanDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CMultiPortScanDlg::OnBtnscan() 
{
	// TODO: Add your control notification handler code here
	DWORD dwStartIp, dwEndIp;
    WORD dwStartPort, dwEndPort;

    // 得到起始IP地址和端口号
    m_IpStart.GetAddress(dwStartIp);
    m_IpEnd.GetAddress(dwEndIp);

    dwStartPort = GetDlgItemInt(IDC_PORTSTART, FALSE, FALSE);
    dwEndPort = GetDlgItemInt(IDC_PORTEND, FALSE, FALSE);

    // 创建事件
    HANDLE hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

    // 填充传给子线程的参数结构体
    THREAD_PARAM ThreadParam = { 0 };
    ThreadParam.dwStartIp = dwStartIp;
    ThreadParam.dwEndIp = dwEndIp;
    ThreadParam.dwStartPort = dwStartPort;
    ThreadParam.dwEndPort = dwEndPort;
    ThreadParam.pThis = this;
    ThreadParam.hEvent = hEvent;

    // 创建新线程并等待参数拷贝完成
    HANDLE hThread = CreateThread(NULL, 0, MainThread, 
                                  (LPVOID)&ThreadParam, 
                                  0, NULL);
    WaitForSingleObject(hEvent, INFINITE);
    ResetEvent(hEvent);
    CloseHandle(hEvent);
    CloseHandle(hThread);
}

DWORD WINAPI CMultiPortScanDlg::MainThread(LPVOID lpParam)
{
    // 拷贝主线程传递来的参数
    THREAD_PARAM ThreadParam = { 0 };
    MoveMemory(&ThreadParam, lpParam, sizeof(ThreadParam));
    SetEvent(ThreadParam.hEvent);

    // 定义传递给扫描线程的结构体
    SCAN_PARAM ScanParam = { 0 };

    // 创建事件对象和信号量对象
    // 并赋值给扫描结构体
    HANDLE hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    HANDLE hSemaphore = CreateSemaphore(NULL, MAX_THREAD, MAX_THREAD, NULL);

    ScanParam.hEvent = hEvent;
    ScanParam.hSemaphore = hSemaphore;

    DWORD dwCurIp;
    WORD  dwCurPort;
    
    // 外层循环控制IP地址
    for ( dwCurIp = ThreadParam.dwStartIp; 
          dwCurIp <= ThreadParam.dwEndIp; 
          dwCurIp ++ )
    {
        // 添加IP地址到树形控件
        sockaddr_in sockaddr;
        sockaddr.sin_addr.S_un.S_addr = ntohl(dwCurIp);
        HTREEITEM hTree = ((CMultiPortScanDlg *)ThreadParam.pThis)->m_IpTree.InsertItem(inet_ntoa(sockaddr.sin_addr));
        // 内层循环控制端口号
        for ( dwCurPort = ThreadParam.dwStartPort; 
              dwCurPort <= ThreadParam.dwEndPort; 
              dwCurPort ++ )
        {
            // 判断信号量
            DWORD dwWaitRet = WaitForSingleObject(hSemaphore, 200);

            if ( dwWaitRet == WAIT_OBJECT_0 )
            {
                ScanParam.dwIp = dwCurIp;
                ScanParam.dwPort = dwCurPort;
                ScanParam.pThis = ThreadParam.pThis;
                ScanParam.hTree = hTree;

                HANDLE hThread = CreateThread(NULL, 0, ScanThread, (LPVOID)&ScanParam, 0, NULL);
                WaitForSingleObject(hEvent, INFINITE);
                ResetEvent(hEvent);
            } 
            else if ( dwWaitRet == WAIT_TIMEOUT )
            {
                dwCurPort --;
                continue;
            }
        }
    }

    return 0;
}

DWORD WINAPI CMultiPortScanDlg::ScanThread(LPVOID lpParam)
{
    // 拷贝传递来的扫描参数
    SCAN_PARAM ScanParam = { 0 };
    MoveMemory(&ScanParam, lpParam, sizeof(SCAN_PARAM));
    SetEvent(ScanParam.hEvent);

    WSADATA wsa;

    WSAStartup(MAKEWORD(2, 2), &wsa);

    SOCKET s = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

    sockaddr_in sockaddr;

    sockaddr.sin_family = AF_INET;
    sockaddr.sin_addr.S_un.S_addr = htonl(ScanParam.dwIp);
    sockaddr.sin_port = htons(ScanParam.dwPort);

    if ( connect(s, (SOCKADDR*)&sockaddr, sizeof(SOCKADDR)) == 0 )
    {
        // 开发的端口添加到树形控件中
        CString strPort;
        strPort.Format("%d", ScanParam.dwPort);
        ((CMultiPortScanDlg *)ScanParam.pThis)->m_IpTree.InsertItem(strPort, 1, 1, ScanParam.hTree);
        ((CMultiPortScanDlg *)ScanParam.pThis)->m_IpTree.Invalidate(FALSE);
    }

    closesocket(s);
    WSACleanup();

    // 释放一个信号量
    ReleaseSemaphore(ScanParam.hSemaphore, 1, NULL);

    return 0;
}
