
// TestRemoteConnectionDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TestRemoteConnection.h"
#include "TestRemoteConnectionDlg.h"
#include "afxdialogex.h"
#include "WTSSProcessesDlg.h"
#include "SetExeFileName.h"
#include "SendMsgDlg.h"

#include <lm.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#if defined(GNUC)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored ��-Wdeprecated-declarations��
#elif defined(_MSC_VER)
#pragma warning(disable : 4996)
#endif

#include "Wtsapi32.h"
#pragma comment(lib,"Wtsapi32.lib")
#pragma comment(lib, "netapi32.lib")

#include <winsock2.h>
#pragma comment(lib, "WS2_32")    // ���ӵ�WS2_32.lib

#include <json\json.h>
#ifdef _DEBUG
#pragma comment(lib,"jsoncpp_static")
#else
#pragma comment(lib,"jsoncpp_static")
#endif




bool EnablePriviledge(LPCTSTR lpSystemName)
{
	HANDLE hToken;
	TOKEN_PRIVILEGES tkp = { 1 };
	if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
	{
		if (LookupPrivilegeValue(NULL, lpSystemName, &tkp.Privileges[0].Luid))
		{
			tkp.PrivilegeCount = 1;
			tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
			AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0);
			if (GetLastError() != ERROR_SUCCESS)
			{
				CloseHandle(hToken);
				return false;
			}
		}
		CloseHandle(hToken);
	}

	return true;
}

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
	//DECLARE_MESSAGE_MAP()
public:
	
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


// CTestRemoteConnectionDlg �Ի���

CTestRemoteConnectionDlg::CTestRemoteConnectionDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_TESTREMOTECONNECTION_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTestRemoteConnectionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list);
}

BEGIN_MESSAGE_MAP(CTestRemoteConnectionDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CTestRemoteConnectionDlg::OnBnClickedOk)
	ON_WM_SIZE()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, &CTestRemoteConnectionDlg::OnLvnItemchangedList1)
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &CTestRemoteConnectionDlg::OnRclickList1)
	ON_COMMAND(ID_RMENU_112, &CTestRemoteConnectionDlg::OnRmenu112)
	ON_COMMAND(ID_RMENU_111, &CTestRemoteConnectionDlg::OnRmenu111)
	ON_COMMAND(ID_RMENU_113, &CTestRemoteConnectionDlg::OnRmenu113)
	ON_COMMAND(ID_RMENU_114, &CTestRemoteConnectionDlg::OnRmenu114)
	ON_COMMAND(ID_RMENU_118, &CTestRemoteConnectionDlg::OnRmenu118)
	ON_COMMAND(ID_RMENU_32785, &CTestRemoteConnectionDlg::OnRmenu32785)
END_MESSAGE_MAP()


// CTestRemoteConnectionDlg ��Ϣ�������

BOOL CTestRemoteConnectionDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		
		pSysMenu->AppendMenu(MF_SEPARATOR);
		pSysMenu->AppendMenu(MF_STRING, IDM_CLEARLIST, L"����б�");
		pSysMenu->AppendMenu(MF_STRING, IDM_OFFSESSION, L"ǿ��ע��");
		pSysMenu->AppendMenu(MF_STRING, IDM_UPSESSION, L"����ˢ��");
		//pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);		
	}

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	//////////////////////////////////////////////////////////////////////////////////////////////

	WTSRegisterSessionNotification(GetSafeHwnd(), NOTIFY_FOR_ALL_SESSIONS);

	LONG lStyle;
	lStyle = GetWindowLong(m_list.m_hWnd, GWL_STYLE);
	lStyle &= ~LVS_TYPEMASK; 
	lStyle |= LVS_REPORT; 
	SetWindowLong(m_list.m_hWnd, GWL_STYLE, lStyle);

	DWORD dwStyle = m_list.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;
	//dwStyle |= LVS_EX_GRIDLINES;
	//dwStyle |= LVS_EX_CHECKBOXES;
	m_list.SetExtendedStyle(dwStyle);


	m_list.InsertColumn(0, _T("State|CONSOLE"), LVCFMT_LEFT, 100);
	m_list.InsertColumn(1, _T("TIME"), LVCFMT_LEFT, 100);
	m_list.InsertColumn(2, _T("SessionId"), LVCFMT_LEFT, 100);
	m_list.InsertColumn(3, _T("WTSInitialProgram"), LVCFMT_LEFT, 100);
	m_list.InsertColumn(4, _T("WTSApplicationName"), LVCFMT_LEFT, 100);
	m_list.InsertColumn(5, _T("WTSClientName"), LVCFMT_LEFT, 100);
	m_list.InsertColumn(6, _T("WTSDomainName"), LVCFMT_LEFT, 100);
	m_list.InsertColumn(7, _T("WTSWinStationName"), LVCFMT_LEFT, 100);
	m_list.InsertColumn(8, _T("DeviceId"), LVCFMT_LEFT, 100);
	m_list.InsertColumn(9, _T("WTSWorkingDirectory"), LVCFMT_LEFT, 100);
	m_list.InsertColumn(10, _T("WTSClientAddress"), LVCFMT_LEFT, 100);
	m_list.InsertColumn(11, _T("WTSClientDirectory"), LVCFMT_LEFT, 100);
	/**
	m_list.InsertColumn(12, _T("ClientHardwareId"), LVCFMT_LEFT, 100);
	m_list.InsertColumn(13, _T("ClientProductId"), LVCFMT_LEFT, 100);
	m_list.InsertColumn(14, _T("EncryptionLevel"), LVCFMT_LEFT, 100);
	**/
	m_list.InsertColumn(12, _T("WTSUserName"), LVCFMT_LEFT, 100);
	

	//////////////////////////////////////////////////////////////////////////////////////////////

	GetWTSSessions();

	//EnablePriviledge(SE_TCB_NAME);

	//AdjustPrivileges();

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CTestRemoteConnectionDlg::OnSysCommand(UINT nID, LPARAM lParam)
{

	switch (nID )
	{

//	case IDM_OFFSESSION:
//		AfxMessageBox(L"IDM_OFFSESSION");
//		break;

	case IDM_CLEARLIST:
		AfxMessageBox(L"IDM_CLEARLIST");
		break;

	case IDM_OFFSESSION:
		AfxMessageBox(L"IDM_OFFSESSION");
		break;

	case IDM_UPSESSION:
		AfxMessageBox(L"IDM_UPSESSION");
		break;

	default:
		CDialogEx::OnSysCommand(nID, lParam);
		break;
	}
	
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CTestRemoteConnectionDlg::OnPaint()
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
HCURSOR CTestRemoteConnectionDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CTestRemoteConnectionDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnOK();
}


BOOL CTestRemoteConnectionDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���

	if (pMsg->message == WM_WTSSESSION_CHANGE)
	{

		GetRDPUserLoginMessage(pMsg->wParam, pMsg->lParam);
	}



	return CDialogEx::PreTranslateMessage(pMsg);
}

void CTestRemoteConnectionDlg::GetRDPUserLoginMessage(WPARAM wParam, LPARAM  lParam)
{
	CTime time = CTime::GetCurrentTime();
	CString m_strTime = time.Format("%Y-%m-%d %H:%M:%S");

	CString ConsoleStr;
	RDPLoginUserInfo gULinfo = GetLoginUserInfo(lParam);

	switch (wParam)
	{

	case WTS_CONSOLE_CONNECT:
		ConsoleStr = _T("WTS_CONSOLE_CONNECT");
		break;

	case WTS_CONSOLE_DISCONNECT:
		ConsoleStr = _T("WTS_CONSOLE_DISCONNECT");
		break;

	case WTS_REMOTE_CONNECT:
		ConsoleStr = _T("WTS_REMOTE_CONNECT");
		break;

	case WTS_REMOTE_DISCONNECT:
		ConsoleStr = _T("WTS_REMOTE_DISCONNECT");
		break;

	case WTS_SESSION_LOGON:
		ConsoleStr = _T("WTS_SESSION_LOGON");
		break;

	case WTS_SESSION_LOGOFF:
		ConsoleStr = _T("WTS_SESSION_LOGOFF");
		break;

	case WTS_SESSION_LOCK:
		ConsoleStr = _T("WTS_SESSION_LOCK");
		break;

	case WTS_SESSION_UNLOCK:
		ConsoleStr = _T("WTS_SESSION_UNLOCK");
		break;

	case WTS_SESSION_REMOTE_CONTROL:
		ConsoleStr = _T("WTS_SESSION_REMOTE_CONTROL");
		break;

	default:
		ConsoleStr = _T("===");
		break;
	}

		
	CString  SessionId, ClientHardwareId, ClientProductId, EncryptionLevel;
	SessionId.Format(_T("%d"), gULinfo.SessionId);
	ClientHardwareId.Format(_T("%d"), gULinfo.ClientHardwareId);
	ClientProductId.Format(_T("%d"), gULinfo.ClientProductId);
	EncryptionLevel.Format(_T("%d"), gULinfo.EncryptionLevel);

	m_list.InsertItem(nRow, ConsoleStr );
	m_list.SetItemText(nRow, 1, m_strTime);
	m_list.SetItemText(nRow, 2, SessionId );
	m_list.SetItemText(nRow, 3, (CString)gULinfo.WTSInitialProgram);
	m_list.SetItemText(nRow, 4, (CString)gULinfo.WTSApplicationName);
	m_list.SetItemText(nRow, 5, (CString)gULinfo.WTSClientName);
	m_list.SetItemText(nRow, 6, (CString)gULinfo.WTSDomainName);
	m_list.SetItemText(nRow, 7, (CString)gULinfo.WTSWinStationName);
	m_list.SetItemText(nRow, 8, (CString)gULinfo.DeviceId);
	m_list.SetItemText(nRow, 9, (CString)gULinfo.WTSWorkingDirectory);
	m_list.SetItemText(nRow, 10,(CString)gULinfo.WTSClientAddress);
	m_list.SetItemText(nRow, 11,(CString)gULinfo.WTSClientDirectory);

	/**
	m_list.SetItemText(nRow, 12, ClientHardwareId);
	m_list.SetItemText(nRow, 13, ClientProductId);
	m_list.SetItemText(nRow, 14, EncryptionLevel);
	**/
	m_list.SetItemText(nRow, 12, (CString)gULinfo.WTSUserName);
	nRow++;
}



RDPLoginUserInfo CTestRemoteConnectionDlg::GetLoginUserInfo(DWORD SessionId)
{
	WTS_INFO_CLASS WTSInfoClass;
	DWORD pCount;
	WCHAR* ppBuffer;

	RDPLoginUserInfo rinfo;
	rinfo.SClear();

	WTSInfoClass = WTSInitialProgram;
	if (WTSQuerySessionInformation(WTS_CURRENT_SERVER_HANDLE,
		SessionId,
		WTSInfoClass,
		&ppBuffer,
		&pCount))
	{
		lstrcpy(rinfo.WTSInitialProgram, ppBuffer);

		WTSFreeMemory(ppBuffer);

	}

	WTSInfoClass = WTSApplicationName;
	if (WTSQuerySessionInformation(WTS_CURRENT_SERVER_HANDLE,
		SessionId,
		WTSInfoClass,
		&ppBuffer,
		&pCount))
	{

		lstrcpy(rinfo.WTSApplicationName, ppBuffer);

		WTSFreeMemory(ppBuffer);

	}


	WTSInfoClass = WTSWorkingDirectory;
	if (WTSQuerySessionInformation(WTS_CURRENT_SERVER_HANDLE,
		SessionId,
		WTSInfoClass,
		&ppBuffer,
		&pCount))
	{

		lstrcpy(rinfo.WTSWorkingDirectory, ppBuffer);

		WTSFreeMemory(ppBuffer);

	}


	WTSInfoClass = WTSUserName;
	if (WTSQuerySessionInformation(WTS_CURRENT_SERVER_HANDLE,
		SessionId,
		WTSInfoClass,
		&ppBuffer,
		&pCount))
	{
		lstrcpy(rinfo.WTSUserName, ppBuffer);

		WTSFreeMemory(ppBuffer);

	}

	WTSInfoClass = WTSWinStationName;
	if (WTSQuerySessionInformation(WTS_CURRENT_SERVER_HANDLE,
		SessionId,
		WTSInfoClass,
		&ppBuffer,
		&pCount))
	{

		lstrcpy(rinfo.WTSWinStationName, ppBuffer);

		WTSFreeMemory(ppBuffer);

	}

	WTSInfoClass = WTSDomainName;
	if (WTSQuerySessionInformation(WTS_CURRENT_SERVER_HANDLE,
		SessionId,
		WTSInfoClass,
		&ppBuffer,
		&pCount))
	{
		lstrcpy(rinfo.WTSDomainName, ppBuffer);

		WTSFreeMemory(ppBuffer);

	}

	WTSInfoClass = WTSClientName;
	if (WTSQuerySessionInformation(WTS_CURRENT_SERVER_HANDLE,
		SessionId,
		WTSInfoClass,
		&ppBuffer,
		&pCount))
	{
		lstrcpy(rinfo.WTSClientName, ppBuffer);

		WTSFreeMemory(ppBuffer);

	}

	WTSInfoClass = WTSClientDirectory;
	if (WTSQuerySessionInformation(WTS_CURRENT_SERVER_HANDLE,
		SessionId,
		WTSInfoClass,
		&ppBuffer,
		&pCount))
	{

		lstrcpy(rinfo.WTSClientDirectory, ppBuffer);

		WTSFreeMemory(ppBuffer);
	}



	WTSInfoClass = WTSClientAddress;
	if (WTSQuerySessionInformation(WTS_CURRENT_SERVER_HANDLE,
		SessionId,
		WTSInfoClass,
		&ppBuffer,
		&pCount))
	{
		CString tmpAddStr = L"";
		PWTS_CLIENT_ADDRESS pWTSCA = (PWTS_CLIENT_ADDRESS)ppBuffer;

		if (pWTSCA->AddressFamily != 0)		{
			
			tmpAddStr.Format(_T("%d.%d.%d.%d"),
				pWTSCA->Address[2],
				pWTSCA->Address[3],
				pWTSCA->Address[4],
				pWTSCA->Address[5]);

		}

		lstrcpy(rinfo.WTSClientAddress, tmpAddStr.GetBuffer());
		tmpAddStr.ReleaseBuffer();
		

		WTSFreeMemory(ppBuffer);

	}



	WTSInfoClass = WTSClientInfo;
	if (WTSQuerySessionInformation(WTS_CURRENT_SERVER_HANDLE,
		SessionId,
		WTSInfoClass,
		&ppBuffer,
		&pCount))
	{

		PWTSCLIENT test = (PWTSCLIENTW)ppBuffer;

		rinfo.ClientBuildNumber = test->ClientBuildNumber;
		rinfo.ClientHardwareId = test->ClientHardwareId;
		rinfo.ClientProductId = test->ClientProductId;
		rinfo.ColorDepth = test->ColorDepth;
		rinfo.HRes = test->HRes;
		rinfo.VRes = test->VRes;
		rinfo.EncryptionLevel = test->EncryptionLevel;

		lstrcpy(rinfo.DeviceId, test->DeviceId);

		WTSFreeMemory(ppBuffer);

	}
	

	WTSInfoClass = WTSSessionInfo;
	if (WTSQuerySessionInformation(WTS_CURRENT_SERVER_HANDLE,
		SessionId,
		WTSInfoClass,
		&ppBuffer,
		&pCount))
	{

		PWTSINFO test = (PWTSINFO)ppBuffer;
		rinfo.SessionId = test->SessionId;		
		WTSFreeMemory(ppBuffer);
	}

		
	return rinfo;

}




void CTestRemoteConnectionDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������

	if (NULL != m_list.m_hWnd)
	{
		m_list.SetWindowPos(this, -1, -1, cx - 20, cy - 20, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
	}	

}


void CTestRemoteConnectionDlg::OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
}


void CTestRemoteConnectionDlg::OnRclickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (pNMItemActivate->iItem != -1)
	{
		DWORD dwPos = GetMessagePos();
		CPoint point(LOWORD(dwPos), HIWORD(dwPos));

		CMenu menu;
		VERIFY(menu.LoadMenu(IDR_MENU3));
		CMenu* popup = menu.GetSubMenu(0);
		ASSERT(popup != NULL);
		popup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);

		ActivateItem = pNMItemActivate->iItem;
	}
	
	*pResult = 0;
		

}

/**
* ǿ��ע��
*/
void CTestRemoteConnectionDlg::OnRmenu112()
{
	// TODO: �ڴ���������������
	
	CString SessionID = m_list.GetItemText( ActivateItem ,2  );
	int sid = _wtoi(SessionID);

	WTSDisconnectSession(WTS_CURRENT_SERVER_HANDLE, sid, FALSE);
	WTSLogoffSession(WTS_CURRENT_SERVER_HANDLE, sid, FALSE);

}


void CTestRemoteConnectionDlg::OnRmenu111()
{
	// TODO: �ڴ���������������
	m_list.DeleteAllItems();	
}


void CTestRemoteConnectionDlg::OnRmenu113()
{
	// TODO: �ڴ���������������

//	DWORD pCount;
//	WCHAR* ppBuffer;
	CString SessionID = m_list.GetItemText(ActivateItem, 2);
	int sid = _wtoi(SessionID);

	WTSSProcessesDlg wtsProcessesDlg;

	wtsProcessesDlg.SetSessionId(sid);
	wtsProcessesDlg.DoModal();

}

void CTestRemoteConnectionDlg::OnRmenu114()
{
	// TODO: �ڴ���������������
	m_list.DeleteAllItems();
	GetWTSSessions();
}

void CTestRemoteConnectionDlg::OnRmenu118()
{
	// TODO: �ڴ���������������

	SetExeFileName *setExeFileNameDlg = new SetExeFileName();
	if (setExeFileNameDlg->DoModal() == IDOK )
	{
		CString exefilepath = setExeFileNameDlg->Get_exefilepath();

		CString SessionID = m_list.GetItemText(ActivateItem, 2);
		int sid = _wtoi(SessionID);
		RunSessionNewProcesses(sid , exefilepath);

	}

	
	delete setExeFileNameDlg;
	setExeFileNameDlg = NULL;

}

BOOL CTestRemoteConnectionDlg::GetWTSSessions()
{
	//WTSEnumerateSessions()

	WTS_SESSION_INFO *sessionInfo = NULL;
	DWORD sessionInfoCount;

	DWORD Reserved = 0;
	DWORD Version = 1;

	BOOL result = WTSEnumerateSessions(WTS_CURRENT_SERVER_HANDLE, Reserved, Version, &sessionInfo, &sessionInfoCount);
	unsigned int userCount(0);

	nRow = 0;

	for (unsigned int i = 0; i < sessionInfoCount; ++i)
	{


		if (sessionInfo[i].SessionId == 65536)  continue;

		CString State;

		switch (sessionInfo[i].State)
		{

				case WTSActive:              // User logged on to WinStation
					State = _T("WTSActive");
					break;

				case WTSConnected:           // WinStation connected to client
					State = _T("WTSConnected");
					break;

				case WTSConnectQuery:        // In the process of connecting to client
					State = _T("WTSConnectQuery");
					break;

				case WTSShadow:              // Shadowing another WinStation
					State = _T("WTSShadow");
					break;

				case WTSDisconnected:        // WinStation logged on without client
					State = _T("WTSDisconnected");
					break;

				case WTSIdle:                // Waiting for client to connect
					State = _T("WTSIdle");
					break;

				case WTSListen:              // WinStation is listening for connection
					State = _T("WTSListen");
					break;

				case WTSReset:               // WinStation is being reset
					State = _T("WTSReset");
					break;

				case WTSDown:                // WinStation is down due to error
					State = _T("WTSConnected");
					break;

				case WTSInit:                // WinStation in initialization	
					State = _T("WTSInit");
					break;


				default:
					break;
		}

		CString SessionId;
		SessionId.Format(_T("%d"), sessionInfo[i].SessionId);
				
		m_list.InsertItem(nRow, State );//_T("Console")
		m_list.SetItemText(nRow,1,L"");// _T("TIME")
		m_list.SetItemText(nRow,2, SessionId);// _T("SessionId")
		m_list.SetItemText(nRow,3, GetLoginUserInfo(sessionInfo[i].SessionId).WTSInitialProgram);// _T("WTSInitialProgram")
		m_list.SetItemText(nRow,4, GetLoginUserInfo(sessionInfo[i].SessionId).WTSApplicationName);// _T("WTSApplicationName")
		m_list.SetItemText(nRow,5, GetLoginUserInfo(sessionInfo[i].SessionId).WTSDomainName);// _T("WTSClientName")
		m_list.SetItemText(nRow,6, GetLoginUserInfo(sessionInfo[i].SessionId).WTSDomainName);// _T("WTSDomainName")
		m_list.SetItemText(nRow,7, GetLoginUserInfo(sessionInfo[i].SessionId).WTSWinStationName);// _T("WTSWinStationName")
		m_list.SetItemText(nRow,8, GetLoginUserInfo(sessionInfo[i].SessionId).DeviceId);// _T("DeviceId")
		m_list.SetItemText(nRow,9, GetLoginUserInfo(sessionInfo[i].SessionId).WTSWorkingDirectory);// _T("WTSWorkingDirectory")
		m_list.SetItemText(nRow,10, GetLoginUserInfo(sessionInfo[i].SessionId).WTSClientAddress);// _T("WTSClientAddress")
		m_list.SetItemText(nRow,11, GetLoginUserInfo(sessionInfo[i].SessionId).WTSClientDirectory);// _T("WTSClientDirectory")
		/**
		m_list.SetItemText(nRow,12, L"");// _T("ClientHardwareId")
		m_list.SetItemText(nRow,13, L"");// _T("ClientProductId")
		m_list.SetItemText(nRow,14, L"");// _T("EncryptionLevel")
		**/
		m_list.SetItemText(nRow, 12, GetLoginUserInfo(sessionInfo[i].SessionId).WTSUserName );// _T("WTSUserName")		

		nRow++;

		userCount++;

	}

	WTSFreeMemory(sessionInfo);

	return result;

}


bool CTestRemoteConnectionDlg::RunSessionNewProcesses(DWORD sessionID ,CString exefilePath  )
{
	
	std::string t = CStringA(exefilePath);

	Json::Value dataJson;

	dataJson["code"] = 1;
	dataJson["SessionID"] = (int)sessionID;	
	dataJson["ExefilePath"] = t.c_str();	

	Json::FastWriter writer;
	std::string strWrite = writer.write(dataJson);

	const char *sendtxt = strWrite.c_str();
	/**
	* ִ�� WTSQueryUserToken & CreateProcessAsUser ��Ҫ system �����Ȩ�ޣ���˷��ڷ�����ִ��
	* ���� socket ��Ϣ֪ͨ ��̨���� ��ִ��������� 
	*/
	SendMsg2RDPS(sendtxt);

	return true;
}

int CTestRemoteConnectionDlg::SendMsg2RDPS( const char *sendtxt )
{

	BYTE minorVer = 2;
	BYTE majorVer = 2;
	WSADATA wsaData;

	WORD sockVersion = MAKEWORD(minorVer, majorVer);
	WSAStartup(sockVersion, &wsaData);


	SOCKET s = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (s == INVALID_SOCKET)
	{
		printf(" Failed socket() \n");
		return 0;
	}


	sockaddr_in servAddr;
	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(59891);

	servAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

	if (::connect(s, (sockaddr*)&servAddr, sizeof(servAddr)) == -1)
	{
		printf(" Failed connect() \n");
		return 0;
	}

	::send(s, sendtxt, strlen(sendtxt), 0);
	closesocket(s);
	::WSACleanup();

	return 0;
}






void CTestRemoteConnectionDlg::OnRmenu32785()
{
	// TODO: �ڴ���������������
	

	CString SessionID = m_list.GetItemText(ActivateItem, 2);
	int sid = _wtoi(SessionID);

	SendMsgDlg sendMessageDlg;
	
	if (sendMessageDlg.DoModal() == IDOK)
	{
		CString inputText = sendMessageDlg.GetSendText();		

		DWORD response = 0;
		CString Title = L"�����Ự��������Ϣ";

		BOOL sendMsgBox = WTSSendMessage(
			WTS_CURRENT_SERVER_HANDLE,
			sid,
			Title.GetBuffer(),
			Title.GetLength() * 2,
			inputText.GetBuffer(),
			inputText.GetLength() * 2,
			MB_YESNO,  //MB_OK
			10,
			&response,
			TRUE);

		inputText.ReleaseBuffer();
		Title.ReleaseBuffer();

		if ( !sendMsgBox )
		{
			CString outText;
			outText.Format(L"��Ϣ����ʧ�ܣ�������룺%d" , GetLastError() );

			AfxMessageBox(outText);
		}
		else
		{

			switch (response)
			{
				/**
				case  IDABORT:
				case  IDCANCEL:
				case  IDCONTINUE:
				case  IDIGNORE:
				case  IDRETRY:
				case  IDTRYAGAIN:
				case  IDASYNC:
				**/
			case  IDYES:
				AfxMessageBox(L"�û��յ���Ϣ���ѵ�� YES");
				break;
				
			case  IDNO:
				AfxMessageBox(L"�û��յ���Ϣ���ѵ�� NO ");
				break;

			case  IDOK:
				AfxMessageBox(L"�û��յ���Ϣ���ѵ��ȷ��");
				break;

			case  IDTIMEOUT:
				AfxMessageBox(L"�û��յ���Ϣ����δ���ȷ�ϣ���ʱ 10 �����Զ��ر�");
				break;
			default:
				break;
			}
		}



	}

	

}
