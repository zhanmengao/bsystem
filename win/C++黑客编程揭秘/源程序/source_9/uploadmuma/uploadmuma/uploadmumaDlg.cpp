
// uploadmumaDlg.cpp : ʵ���ļ�
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


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CuploadmumaDlg �Ի���



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


// CuploadmumaDlg ��Ϣ�������

BOOL CuploadmumaDlg::OnInitDialog()
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CuploadmumaDlg::OnPaint()
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
HCURSOR CuploadmumaDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CuploadmumaDlg::OnBnClickedButton1()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    WSAData wsa = { 0 };
    // ��ʼ��
    WSAStartup(MAKEWORD(2, 2), &wsa);

    // ��ȡIP��ַ
    DWORD dwIPAddr = 0;
    m_IpAddr.GetAddress(dwIPAddr);

    // ��ȡ�˿ں�
    WORD dwPort = 0;
    dwPort = GetDlgItemInt(IDC_PORT);

    // ����SOCKET
    SOCKET s = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    sockaddr_in sockaddr = { 0 };

    sockaddr.sin_family = AF_INET;
    sockaddr.sin_addr.S_un.S_addr = htonl(dwIPAddr);
    sockaddr.sin_port = htons(dwPort);

    // ���ӷ�����
    int nRet = connect(s, (SOCKADDR*)&sockaddr, sizeof(SOCKADDR));

    // ��ȡ��BURP��ȡ��HTTP��request��
    char szText[2048] = { 0 };
    GetDlgItemText(IDC_TEXT, szText, 2048);
    // ���Ͱ�
    nRet = send(s, szText, lstrlen(szText), 0);

    closesocket(s);

    WSACleanup();
}
