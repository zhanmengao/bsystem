// NoDllInjectDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NoDllInject.h"
#include "NoDllInjectDlg.h"
#include <tlhelp32.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNoDllInjectDlg dialog

CNoDllInjectDlg::CNoDllInjectDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNoDllInjectDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNoDllInjectDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CNoDllInjectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNoDllInjectDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CNoDllInjectDlg, CDialog)
	//{{AFX_MSG_MAP(CNoDllInjectDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_INJECT, OnBtnInject)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNoDllInjectDlg message handlers

BOOL CNoDllInjectDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
    DebugPrivilege();
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CNoDllInjectDlg::OnPaint() 
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
HCURSOR CNoDllInjectDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

VOID CNoDllInjectDlg::DebugPrivilege()
{
    HANDLE hToken = NULL;
    
    BOOL bRet = OpenProcessToken(GetCurrentProcess(), 
        TOKEN_ALL_ACCESS, &hToken);
    
    if ( bRet == TRUE )
    {
        TOKEN_PRIVILEGES tp;
        
        tp.PrivilegeCount = 1;
        LookupPrivilegeValue(NULL, SE_DEBUG_NAME, 
            &tp.Privileges[0].Luid);
        
        tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
        AdjustTokenPrivileges(hToken, FALSE, &tp, 
            sizeof(tp), NULL, NULL);
        
        CloseHandle(hToken);
    }
}

DWORD CNoDllInjectDlg::GetProcId(char *szProcessName)
{
    BOOL bRet;
    PROCESSENTRY32 pe32;
    HANDLE hSnap;
    
    hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
    pe32.dwSize = sizeof(pe32);
    bRet = Process32First(hSnap, &pe32);
    
    while ( bRet )
    {
        // strupr()函数是将字符串转化为大写
        if ( lstrcmp(strupr(pe32.szExeFile), 
            strupr(szProcessName)) == 0 )
        {
            return pe32.th32ProcessID;
        }
        
        bRet = Process32Next(hSnap, &pe32);
    }
    
    return 0;
}

void CNoDllInjectDlg::OnBtnInject() 
{
	// TODO: Add your control notification handler code here
    char szProcessName[MAX_PATH] = { 0 };
    DWORD dwPid;

    GetDlgItemText(IDC_EDIT_PROCESSNAME, szProcessName, MAX_PATH);
    dwPid = GetProcId(szProcessName);

    InjectCode(dwPid);
}

#define STRLEN 20

typedef struct _DATA
{
    DWORD dwLoadLibrary;
    DWORD dwGetProcAddress;
    DWORD dwGetModuleHandle;
    DWORD dwGetModuleFileName;

    char User32Dll[STRLEN];
    char MessageBox[STRLEN];
    char Str[STRLEN];
}DATA, *PDATA;

DWORD WINAPI RemoteThreadProc(LPVOID lpParam)
{
    PDATA pData = (PDATA)lpParam;

    // 定义API函数原型
    HMODULE (__stdcall *MyLoadLibrary)(LPCTSTR);
    FARPROC (__stdcall *MyGetProcAddress)(HMODULE, LPCSTR);
    HMODULE (__stdcall *MyGetModuleHandle)(LPCTSTR);
    int (__stdcall *MyMessageBox)(HWND, LPCTSTR, LPCTSTR, UINT);
    DWORD (__stdcall *MyGetModuleFileName)(HMODULE, LPTSTR, DWORD);

    MyLoadLibrary = (HMODULE (__stdcall *)(LPCTSTR))
                                            pData->dwLoadLibrary;
    MyGetProcAddress = (FARPROC (__stdcall *)(HMODULE, LPCSTR))
                                            pData->dwGetProcAddress;
    MyGetModuleHandle = (HMODULE (__stdcall *)(LPCSTR))
                                            pData->dwGetModuleHandle;
    MyGetModuleFileName = (DWORD (__stdcall *)(HMODULE, LPTSTR, DWORD))
                                            pData->dwGetModuleFileName;

    HMODULE hModule = MyLoadLibrary(pData->User32Dll);
    MyMessageBox = (int (__stdcall *)(HWND, LPCTSTR, LPCTSTR, UINT))
                                  MyGetProcAddress(hModule, pData->MessageBox);

    char szModuleFileName[MAX_PATH] = { 0 };
    MyGetModuleFileName(NULL, szModuleFileName, MAX_PATH);

    MyMessageBox(NULL, pData->Str, szModuleFileName, MB_OK);

    return 0;
}

VOID CNoDllInjectDlg::InjectCode(DWORD dwPid)
{
    // 打开进程并获取进程句柄
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, 
                                  FALSE, dwPid);

    if ( hProcess == NULL )
    {
        return ;
    }

    DATA Data = { 0 };

    // 获取kernel32.dll中相关的导出函数
    Data.dwLoadLibrary = (DWORD)GetProcAddress(
                                    GetModuleHandle("kernel32.dll"),
                                    "LoadLibraryA");
    Data.dwGetProcAddress = (DWORD)GetProcAddress(
                                    GetModuleHandle("kernel32.dll"),
                                    "GetProcAddress");
    Data.dwGetModuleHandle = (DWORD)GetProcAddress(
                                    GetModuleHandle("kernel32.dll"),
                                    "GetModuleHandleA");
    Data.dwGetModuleFileName = (DWORD)GetProcAddress(
                                    GetModuleHandle("kernel32.dll"),
                                    "GetModuleFileNameA");

    // 需要的其他DLL和导出函数
    lstrcpy(Data.User32Dll, "user32.dll");
    lstrcpy(Data.MessageBox, "MessageBoxA");
    // MessageBoxA()弹出的字符串
    lstrcpy(Data.Str, "Inject Code !!!");

    // 在目标进程申请空间
    LPVOID lpData = VirtualAllocEx(hProcess, NULL, sizeof(Data),
                                   MEM_COMMIT | MEM_RESERVE,
                                   PAGE_READWRITE);

    DWORD dwWriteNum = 0;
    WriteProcessMemory(hProcess, lpData, &Data, 
                       sizeof(Data), &dwWriteNum);

    // 在目标进程空间申请的用于保存代码的长度
    DWORD dwFunSize = 0x1000;
    LPVOID lpCode = VirtualAllocEx(hProcess, NULL, dwFunSize,
                                   MEM_COMMIT, 
                                   PAGE_EXECUTE_READWRITE);

    BOOL bRet = WriteProcessMemory(hProcess, lpCode, &RemoteThreadProc, 
                       dwFunSize, &dwWriteNum);

    if ( bRet == FALSE )
    {
        int n = GetLastError();
    }

    HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0,
                                        (LPTHREAD_START_ROUTINE)lpCode,
                                        lpData, 0, NULL);
    WaitForSingleObject(hThread, INFINITE);

    CloseHandle(hThread);
    CloseHandle(hProcess);
}
