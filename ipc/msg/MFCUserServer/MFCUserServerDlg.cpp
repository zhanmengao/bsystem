
// MFCUserServerDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MFCUserServer.h"
#include "MFCUserServerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFCUserServerDlg �Ի���



CMFCUserServerDlg::CMFCUserServerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MFCUSERSERVER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCUserServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_REVCDATA, m_ListData);
}

BEGIN_MESSAGE_MAP(CMFCUserServerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_UMSG, &CMFCUserServerDlg::RecvUserMsg)
	ON_WM_COPYDATA()
END_MESSAGE_MAP()


// CMFCUserServerDlg ��Ϣ�������

BOOL CMFCUserServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CMFCUserServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMFCUserServerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CMFCUserServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

LRESULT  CMFCUserServerDlg::RecvUserMsg(WPARAM wParam, LPARAM lParam)
{
	int num1 = wParam;
	int num2 = lParam;
	CString str;
	str.Format(_T("Recv %d"), num1 + num2);
	SetDlgItemText(IDC_TEXT_RECVDATA, str);
	return TRUE;
}


BOOL CMFCUserServerDlg::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CString str;
	DWORD dwPid = 0;
	GetWindowThreadProcessId(pWnd->m_hWnd, &dwPid);
	//��ʽ���ַ�������ӵ��б��
	str.Format(_T("Pid = [%d]�Ľ��̷�����Ϣ��%s"), dwPid, pCopyDataStruct->lpData);
	m_ListData.AddString(str);

	//��һ����Ϣ
	SYSTEMTIME st;
	GetLocalTime(&st);
	str.Format(_T("%02d:%02d:%02d"),st.wHour,st.wMinute,st.wSecond);
	COPYDATASTRUCT cds = { 0 };
	cds.dwData = 0;
	cds.cbData = (str.GetLength()* + 1)*sizeof(TCHAR);
	cds.lpData = str.GetBuffer();

	//����Ϣ
	::SendMessage(pWnd->m_hWnd, WM_COPYDATA, NULL, (LPARAM)&cds);
	return CDialogEx::OnCopyData(pWnd, pCopyDataStruct);
}
