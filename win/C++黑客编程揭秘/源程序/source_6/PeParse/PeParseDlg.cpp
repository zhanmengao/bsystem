// PeParseDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PeParse.h"
#include "PeParseDlg.h"

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
// CPeParseDlg dialog

CPeParseDlg::CPeParseDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPeParseDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPeParseDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

    m_hFile = NULL;
    m_hMap = NULL;
    m_lpBase = NULL;

    m_pDosHdr = NULL;
    m_pNtHdr = NULL;
    m_pSecHdr = NULL;
}

void CPeParseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPeParseDlg)
	DDX_Control(pDX, IDC_LIST_SECTION, m_SectionLIst);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPeParseDlg, CDialog)
	//{{AFX_MSG_MAP(CPeParseDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_EXIT, OnBtnExit)
	ON_BN_CLICKED(IDC_BTN_SHOW, OnBtnShow)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPeParseDlg message handlers

BOOL CPeParseDlg::OnInitDialog()
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
    InitSectionList();
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CPeParseDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CPeParseDlg::OnPaint() 
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
HCURSOR CPeParseDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CPeParseDlg::OnBtnExit() 
{
	// TODO: Add your control notification handler code here
	EndDialog(0);
}

void CPeParseDlg::OnBtnShow() 
{
	// TODO: Add your control notification handler code here
    char szFileName[MAX_PATH] = { 0 };
    BOOL bRet = FALSE;

	GetDlgItemText(IDC_EDIT_FILE_PATH, szFileName, MAX_PATH);
    bRet = FileCreate(szFileName);

    if ( bRet == FALSE )
    {
        AfxMessageBox("�ļ��򿪴���!");
        return ;
    }

    bRet = IsPeFileAndGetPEPointer();
    if ( bRet == FALSE )
    {
        AfxMessageBox("������Ч��PE�ļ���ʽ!");
        return ;
    }

    ParseBasePe();

    EnumSections();

    FileClose();
}

VOID CPeParseDlg::InitSectionList()
{
    m_SectionLIst.SetExtendedStyle(
            m_SectionLIst.GetExtendedStyle()
            | LVS_EX_GRIDLINES);

    m_SectionLIst.InsertColumn(0, " ���� ");
    m_SectionLIst.InsertColumn(1, " V.ƫ�� ");
    m_SectionLIst.InsertColumn(2, " V.��С ");
    m_SectionLIst.InsertColumn(3, " R.ƫ�� ");
    m_SectionLIst.InsertColumn(4, " R.��С ");
    m_SectionLIst.InsertColumn(5, "��־");

    m_SectionLIst.SetColumnWidth(0, LVSCW_AUTOSIZE_USEHEADER);
    m_SectionLIst.SetColumnWidth(1, LVSCW_AUTOSIZE_USEHEADER);
    m_SectionLIst.SetColumnWidth(2, LVSCW_AUTOSIZE_USEHEADER);
    m_SectionLIst.SetColumnWidth(3, LVSCW_AUTOSIZE_USEHEADER);
    m_SectionLIst.SetColumnWidth(4, LVSCW_AUTOSIZE_USEHEADER);
    m_SectionLIst.SetColumnWidth(5, LVSCW_AUTOSIZE_USEHEADER);
}

BOOL CPeParseDlg::IsPeFileAndGetPEPointer()
{
    BOOL bRet = FALSE;

    // �ж��Ƿ�ΪMZͷ
    m_pDosHdr = (PIMAGE_DOS_HEADER)m_lpBase;

    if ( m_pDosHdr->e_magic != IMAGE_DOS_SIGNATURE )
    {
        return bRet;
    }

    // ����IMAGE_DOS_HEADER��e_lfanew��ֵ�õ�PEͷ��λ��
    m_pNtHdr = (PIMAGE_NT_HEADERS)((DWORD)m_lpBase + m_pDosHdr->e_lfanew);

    // �ж��Ƿ�ΪPE\0\0
    if ( m_pNtHdr->Signature != IMAGE_NT_SIGNATURE )
    {
        return bRet;
    }

    // ��ýڱ��λ��
    m_pSecHdr = (PIMAGE_SECTION_HEADER)((DWORD)&(m_pNtHdr->OptionalHeader) 
                + m_pNtHdr->FileHeader.SizeOfOptionalHeader);

    bRet = TRUE;
    return bRet;
}

BOOL CPeParseDlg::FileCreate(char *szFileName)
{
    BOOL bRet = FALSE;

    m_hFile = CreateFile(szFileName, 
                         GENERIC_READ | GENERIC_WRITE,
                         FILE_SHARE_READ,
                         NULL,
                         OPEN_EXISTING,
                         FILE_ATTRIBUTE_NORMAL,
                         NULL);
    if ( m_hFile == INVALID_HANDLE_VALUE )
    {
        return bRet;
    }

    m_hMap = CreateFileMapping(m_hFile, NULL, 
                               PAGE_READWRITE | SEC_IMAGE, 
                               0, 0, 0);
    if ( m_hMap == NULL )
    {
        CloseHandle(m_hFile);
        return bRet;
    }

    m_lpBase = MapViewOfFile(m_hMap, 
                             FILE_MAP_READ | FILE_SHARE_WRITE, 
                             0, 0, 0);
    if ( m_lpBase == NULL )
    {
        CloseHandle(m_hMap);
        CloseHandle(m_hFile);
        return bRet;
    }

    bRet = TRUE;
    return bRet;
}

VOID CPeParseDlg::FileClose()
{
    if ( m_lpBase != NULL )
    {
        UnmapViewOfFile(m_lpBase);
        m_lpBase = NULL;
    }

    if ( m_hMap != NULL )
    {
        CloseHandle(m_hMap);
        m_hMap = NULL;
    }

    if ( m_hFile != NULL )
    {
        CloseHandle(m_hFile);
        m_hFile = NULL;
    }
}

VOID CPeParseDlg::ParseBasePe()
{
    CString StrTmp;

    // ��ڵ�ַ
    StrTmp.Format("%08X", m_pNtHdr->OptionalHeader.AddressOfEntryPoint);
    SetDlgItemText(IDC_EDIT_EP, StrTmp);

    // ӳ�����ַ
    StrTmp.Format("%08X", m_pNtHdr->OptionalHeader.ImageBase);
    SetDlgItemText(IDC_EDIT_IMAGEBASE, StrTmp);

    // �������汾��
    StrTmp.Format("%d.%d", 
        m_pNtHdr->OptionalHeader.MajorLinkerVersion,
        m_pNtHdr->OptionalHeader.MinorLinkerVersion);
    SetDlgItemText(IDC_EDIT_LINKVERSION, StrTmp);

    // �ڱ�����
    StrTmp.Format("%02X", m_pNtHdr->FileHeader.NumberOfSections);
    SetDlgItemText(IDC_EDIT_SECTIONNUM, StrTmp);

    // �ļ�����ֵ��С
    StrTmp.Format("%08X", m_pNtHdr->OptionalHeader.FileAlignment);
    SetDlgItemText(IDC_EDIT_FILEALIGN, StrTmp);

    // �ڴ����ֵ��С
    StrTmp.Format("%08X", m_pNtHdr->OptionalHeader.SectionAlignment);
    SetDlgItemText(IDC_EDIT_SECALIGN, StrTmp);
}

VOID CPeParseDlg::EnumSections()
{
    int nSecNum = m_pNtHdr->FileHeader.NumberOfSections;

    int i = 0;
    CString StrTmp;

    for ( i = 0; i < nSecNum; i ++ )
    {
        m_SectionLIst.InsertItem(i, (const char *)m_pSecHdr[i].Name);

        StrTmp.Format("%08X", m_pSecHdr[i].VirtualAddress);
        m_SectionLIst.SetItemText(i, 1, StrTmp);

        StrTmp.Format("%08X", m_pSecHdr[i].Misc.VirtualSize);
        m_SectionLIst.SetItemText(i, 2, StrTmp);

        StrTmp.Format("%08X", m_pSecHdr[i].PointerToRawData);
        m_SectionLIst.SetItemText(i, 3, StrTmp);

        StrTmp.Format("%08X", m_pSecHdr[i].SizeOfRawData);
        m_SectionLIst.SetItemText(i, 4, StrTmp);

        StrTmp.Format("%08X", m_pSecHdr[i].Characteristics);
        m_SectionLIst.SetItemText(i, 5, StrTmp);
    }
}