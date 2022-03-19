
// ScanAdminPageDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ScanAdminPage.h"
#include "ScanAdminPageDlg.h"
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


// CScanAdminPageDlg �Ի���



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


// CScanAdminPageDlg ��Ϣ�������

BOOL CScanAdminPageDlg::OnInitDialog()
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
    m_ScanList.InsertColumn(0, "ɨ����");
    m_ScanList.SetColumnWidth(0, 400);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CScanAdminPageDlg::OnPaint()
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
HCURSOR CScanAdminPageDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CScanAdminPageDlg::OnBnClickedButton1()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    // ����б������
    m_ScanList.DeleteAllItems();

    // ��ȡ�����URL����
    CString strURL;
    GetDlgItemText(IDC_EDIT1, strURL);

    // �����¼�
    m_hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

    // ����ɨ���߳�
    HANDLE hThread = NULL;
    hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ScanThread, this, 0, NULL);
    WaitForSingleObject(m_hEvent, INFINITE);

    ResetEvent(m_hEvent);
}


DWORD WINAPI CScanAdminPageDlg::ScanThread(LPVOID lpParam)
{
    CScanAdminPageDlg *pThis = (CScanAdminPageDlg*)lpParam;

    SetEvent(pThis->m_hEvent);

    // ���̺߳����л�ȡ����ѡ�������
    // ����ѡ����asp��php����
    CString strWebType;
    pThis->GetDlgItemText(IDC_COMBO1, strWebType);

    // ���̺߳����л�ȡɨ��URL��ַ
    CString strUrl;
    pThis->GetDlgItemText(IDC_EDIT1, strUrl);
    
    // ͨ������ѡ���������ֵ�
    // asp.dic �� php.dic
    char szFileName[MAX_PATH] = { 0 };
    wsprintf(szFileName, "%s.dic", strWebType);

    // ���ֵ��ļ��ļ�
    // ���ж�ȡ���ܵĺ�̨ҳ��
    FILE *DicFile = NULL;
    fopen_s(&DicFile, szFileName, "r");
    char szDic[MAXBYTE] = { 0 };
    while ( fgets(szDic, MAXBYTE, DicFile) )
    {
        if ( szDic[lstrlen(szDic) - 1] == '\n' )
        {
            szDic[lstrlen(szDic) - 1] = NULL;
        }

        // ɨ��URL��ַ���ֵ��е�ҳ���ļ�����ƴ��
        CString strCheckUrl = strUrl + szDic;

        // �ж�ҳ���Ƿ����
        // �������ڵ�ַ�Ľ�β���ӡ�[OK]������
        if ( pThis->CheckUrl(strCheckUrl) )
        {
            strCheckUrl += "[OK]";
            pThis->m_ScanList.InsertItem(0, strCheckUrl);
            continue;
        }

        // ��ɨ��ĵ�ַ������ֵ�
        pThis->m_ScanList.InsertItem(pThis->m_ScanList.GetItemCount(), strCheckUrl);
    }

    return 0;
}

BOOL CScanAdminPageDlg::CheckUrl(CString strUrl)
{
    // ����һ��SESSION
    CInternetSession session("ScanAdminPage");
    // ����һ��HTTP����
    CHttpConnection *pServer = NULL;
    // ��ȡһ��HTTP�ļ�
    CHttpFile *pFile = NULL;

    // ��������URL�Ƿ���ϸ�ʽ������URL����
    CString strServerName;  // ��������ַ
    CString strObject;      // URLָ��Ķ���
    INTERNET_PORT nPort;    // �˿ں�
    DWORD dwServiceType;    // ��������

    // URL����ʧ���򷵻�
    // ����ɨ���URL
    if ( !AfxParseURL(strUrl, dwServiceType, strServerName, strObject, nPort) )
    {
        return NULL;
    }

    // �������ʹ���
    if ( dwServiceType != INTERNET_SERVICE_HTTP )
    {
        return NULL;
    }

    // �������ӷ������ĵ�ַ���˿ڣ�����ȡ��HTTP����
    pServer = session.GetHttpConnection(strServerName, nPort);
    // �򿪸�HTTP����
    pFile = pServer->OpenRequest(CHttpConnection::HTTP_VERB_GET, strObject, NULL, 1, NULL, NULL, INTERNET_FLAG_EXISTING_CONNECT | INTERNET_FLAG_NO_AUTO_REDIRECT);
    
    try
    {
        // ��������
        pFile->SendRequest();
    }
    catch (CException* e)
    {
        return NULL;
    }

    DWORD dwRet;
    // ��ȡ������Ļ�Ӧ״̬��
    pFile->QueryInfoStatusCode(dwRet);

    BOOL bRet = FALSE;
    
    // HTTP����ֵΪ200��ʾ�ɹ�
    if ( dwRet == 200 )
    {
        bRet = TRUE;
    }

    // �ͷ�ָ��
    if ( pFile != NULL )
    {
        delete pFile;
    }
    if ( pServer != NULL )
    {
        delete pServer;
    }

    // �رջỰ
    session.Close();

    return bRet;
}
