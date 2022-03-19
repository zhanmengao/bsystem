// WTSSProcessesDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TestRemoteConnection.h"
#include "WTSSProcessesDlg.h"
#include "afxdialogex.h"

#include <psapi.h>
#include <tlhelp32.h>

#include "Wtsapi32.h"
// WTSSProcessesDlg 对话框

IMPLEMENT_DYNAMIC(WTSSProcessesDlg, CDialogEx)




WTSSProcessesDlg::WTSSProcessesDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_WTSSProcesses_DIALOG, pParent)
{

}

WTSSProcessesDlg::~WTSSProcessesDlg()
{
}

void WTSSProcessesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ProcessesList);	
}


BEGIN_MESSAGE_MAP(WTSSProcessesDlg, CDialogEx)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, &WTSSProcessesDlg::OnLvnItemchangedList1)
	ON_WM_SHOWWINDOW()
	ON_WM_SIZE()	
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &WTSSProcessesDlg::OnNMRClickList1)
	ON_COMMAND(ID_RMENU_115, &WTSSProcessesDlg::OnRmenu115)
	ON_COMMAND(ID_RMENU_116, &WTSSProcessesDlg::OnRmenu116)
	ON_COMMAND(ID_RMENU_117, &WTSSProcessesDlg::OnRmenu117)
END_MESSAGE_MAP()


// WTSSProcessesDlg 消息处理程序


void WTSSProcessesDlg::OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}

DWORD WTSSProcessesDlg::GetSessionId()
{
	return SessionId;
}

void WTSSProcessesDlg::SetSessionId(DWORD sessionID)
{

	SessionId = sessionID;
}




BOOL WTSSProcessesDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	LONG lStyle;
	lStyle = GetWindowLong(  m_ProcessesList.m_hWnd, GWL_STYLE);
	lStyle &= ~LVS_TYPEMASK;
	lStyle |= LVS_REPORT;
	SetWindowLong(m_ProcessesList.m_hWnd, GWL_STYLE, lStyle);

	DWORD dwStyle = m_ProcessesList.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;
	//dwStyle |= LVS_EX_GRIDLINES;
	//dwStyle |= LVS_EX_CHECKBOXES;
	m_ProcessesList.SetExtendedStyle(dwStyle);

	m_ProcessesList.InsertColumn(0, _T("SessionId"), LVCFMT_LEFT, 100);
	m_ProcessesList.InsertColumn(1, _T("ProcessId"), LVCFMT_LEFT, 100);
	m_ProcessesList.InsertColumn(2, _T("ProcessName"), LVCFMT_LEFT, 100);
	m_ProcessesList.InsertColumn(3, _T("UserSid"), LVCFMT_LEFT, 100);
	m_ProcessesList.InsertColumn(4, _T("NumberOfThreads"), LVCFMT_LEFT, 100);
	m_ProcessesList.InsertColumn(5, _T("HandleCount"), LVCFMT_LEFT, 100);
	/**
	m_ProcessesList.InsertColumn(6, _T("PagefileUsage"), LVCFMT_LEFT, 100);
	m_ProcessesList.InsertColumn(7, _T("PeakPagefileUsage"), LVCFMT_LEFT, 100);
	m_ProcessesList.InsertColumn(8, _T("WorkingSetSize"), LVCFMT_LEFT, 100);
	m_ProcessesList.InsertColumn(9, _T("PeakWorkingSetSize"), LVCFMT_LEFT, 100);
	m_ProcessesList.InsertColumn(10, _T("UserTime"), LVCFMT_LEFT, 100);
	m_ProcessesList.InsertColumn(11, _T("KernelTime"), LVCFMT_LEFT, 100);
	**/
	
	CString titleStr;
	titleStr.Format(L"Session %d 进程列表" , GetSessionId());
	SetWindowText(titleStr);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void WTSSProcessesDlg::PreInitDialog()
{
	// TODO: 在此添加专用代码和/或调用基类

	CDialogEx::PreInitDialog();
}


INT_PTR WTSSProcessesDlg::DoModal()
{
	// TODO: 在此添加专用代码和/或调用基类
	
	return CDialogEx::DoModal();
}


void WTSSProcessesDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);
	
	GetWTSProcesses(GetSessionId());
	// TODO: 在此处添加消息处理程序代码
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

BOOL WTSSProcessesDlg::GetWTSProcesses(DWORD SessionId)
{

	HANDLE hServer = NULL;
	DWORD Level = 1;
	WTS_PROCESS_INFO_EX* ppProcessInfo = NULL;
	DWORD pCount = 0;

	CString ProcessId_Str;
	CString pUserSid_Str;
	CString SessionId_Str;

	BOOL rbool = WTSEnumerateProcessesEx(WTS_CURRENT_SERVER_HANDLE, &Level, SessionId, (LPWSTR*)&ppProcessInfo, &pCount);

	int error  =  GetLastError();

	DWORD rdpinit_ProcessId = 0;

	m_ProcessesList.DeleteAllItems();

	for (int i = 0; i < pCount; i++)
	{
		
		CString SessionId; 
		SessionId.Format(L"%d", ppProcessInfo[i].SessionId );

		CString ProcessId; 
		ProcessId.Format(L"%d", ppProcessInfo[i].ProcessId );

		LPWSTR pProcessName = ppProcessInfo[i].pProcessName;
		
		PSID pUserSid = ppProcessInfo[i].pUserSid;		

		CString NumberOfThreads;
		NumberOfThreads.Format( L"%d",ppProcessInfo[i].NumberOfThreads);

		CString HandleCount;
		HandleCount.Format(L"%d", ppProcessInfo[i].HandleCount);

		CString PagefileUsage;
		PagefileUsage.Format(L"%d", ppProcessInfo[i].PagefileUsage);

		CString PeakPagefileUsage;
		PeakPagefileUsage.Format(L"%d", ppProcessInfo[i].PeakPagefileUsage);

		CString WorkingSetSize;
		WorkingSetSize.Format(L"%d", ppProcessInfo[i].WorkingSetSize);

		CString PeakWorkingSetSize;
		PeakWorkingSetSize.Format(L"%d", ppProcessInfo[i].PeakWorkingSetSize);
				
		LARGE_INTEGER UserTime = ppProcessInfo[i].UserTime;
		LARGE_INTEGER KernelTime = ppProcessInfo[i].KernelTime;
		
		m_ProcessesList.InsertItem(nRow, SessionId);
		m_ProcessesList.SetItemText(nRow, 1, ProcessId);
		m_ProcessesList.SetItemText(nRow, 2, pProcessName);
		m_ProcessesList.SetItemText(nRow, 3, L"-");
		m_ProcessesList.SetItemText(nRow, 4, NumberOfThreads);
		m_ProcessesList.SetItemText(nRow, 5, HandleCount);

		/**
		m_ProcessesList.SetItemText(nRow, 6, PagefileUsage);
		m_ProcessesList.SetItemText(nRow, 7, PeakPagefileUsage);
		m_ProcessesList.SetItemText(nRow, 8, WorkingSetSize);
		m_ProcessesList.SetItemText(nRow, 9, PeakWorkingSetSize);
		m_ProcessesList.SetItemText(nRow, 10, L"UserTime");
		m_ProcessesList.SetItemText(nRow, 11, L"KernelTime");
		**/
		nRow++;

		/***
		*  rdpinit.exe 进程属于 RemoteAPP 初始化程，有这个进程存在，则表明当前会话可能是由RemoteAPP 发起的；
		*  通过查找该进程的子进程，即可以查询出 RemoteAPP 应用程序
		*/
		if ( lstrcmp(  L"rdpinit.exe" , pProcessName) == 0 )
		{
			rdpinit_ProcessId = ppProcessInfo[i].ProcessId;			
		}

	}


	WTSFreeMemoryEx(WTSTypeProcessInfoLevel1, ppProcessInfo, pCount);


	if ( rdpinit_ProcessId >0 )
	{
		GetProceeIDfromParentID(rdpinit_ProcessId);
	}
	
	return rbool;
}


void WTSSProcessesDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if (NULL != m_ProcessesList.m_hWnd)
	{
		m_ProcessesList.SetWindowPos(this, -1, -1, cx - 20, cy - 20, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);		
	}
}

/**
* 通过父进程查找子进程
**/
DWORD WTSSProcessesDlg::GetProceeIDfromParentID(DWORD dwParentProcessId)
{

	DWORD dwProcessID = 0;

	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}

	PROCESSENTRY32 pe;
	TCHAR procID[100] = { 0 };

	pe.dwSize = sizeof(pe);
	BOOL bProcess = Process32First(hProcessSnap, &pe);

	while (bProcess)
	{
		if (pe.th32ParentProcessID == dwParentProcessId &&
			/** 忽略掉 rdpshell.exe 它是RemoteAPP 的外壳程序 **/
			lstrcmp( L"rdpshell.exe" , pe.szExeFile) != 0 )
		{
			dwProcessID = pe.th32ProcessID;

			CString outText;
			outText.Format( L"ProcessID: %d \nProcessExeFile: %s" , pe.th32ProcessID ,pe.szExeFile );

			::MessageBox( NULL,outText,L"RemoteApp",MB_OK| MB_ICONASTERISK );
			
		}
		bProcess = Process32Next(hProcessSnap, &pe);
	}

	CloseHandle(hProcessSnap);

	return dwProcessID;
}





void WTSSProcessesDlg::OnNMRClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	DWORD dwPos = GetMessagePos();
	CPoint point(LOWORD(dwPos), HIWORD(dwPos));

	CMenu menu;
	VERIFY(menu.LoadMenu(IDR_MENU4));
	CMenu* popup = menu.GetSubMenu(0);
	ASSERT(popup != NULL);
	popup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);

	ActivateItem = pNMItemActivate->iItem;


	*pResult = 0;
}

/**
* 当前由 RemoteAPP 发起的进程
*/
void WTSSProcessesDlg::OnRmenu115()
{
	// TODO: 在此添加命令处理程序代码
	
	GetProceeIDfromParentID(GetSessionId());
}


/**
* 结束进程
*/
void WTSSProcessesDlg::OnRmenu116()
{
	// TODO: 在此添加命令处理程序代码
	
	CString ProcessesID = m_ProcessesList.GetItemText(ActivateItem, 1);
	int pid = _wtoi(ProcessesID);
	
	WTSTerminateProcess(WTS_CURRENT_SERVER_HANDLE, pid,0);

	GetWTSProcesses(GetSessionId());

}


void WTSSProcessesDlg::OnRmenu117()
{
	// TODO: 在此添加命令处理程序代码
	/**
	CString SessionID = m_ProcessesList.GetItemText(ActivateItem, 0);
	CString ProcessesID = m_ProcessesList.GetItemText(ActivateItem, 1);

	int sid = _wtoi(SessionID);
	int pid = _wtoi(ProcessesID);
	**/
}


