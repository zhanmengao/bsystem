// MultiPortScanDlg.h : header file
//

#if !defined(AFX_MULTIPORTSCANDLG_H__FA1A8EBB_D0E4_45E4_92EA_ED404C1E1E07__INCLUDED_)
#define AFX_MULTIPORTSCANDLG_H__FA1A8EBB_D0E4_45E4_92EA_ED404C1E1E07__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CMultiPortScanDlg dialog
class CMultiPortScanDlg;

typedef struct _SCAN_PARAM
{
    DWORD dwIp;                 // IP地址
    WORD dwPort;                // 端口号
    CMultiPortScanDlg *pThis;   // 主窗口的this指针
    HANDLE hEvent;              // 事件句柄
    HANDLE hSemaphore;          // 信号量句柄
    HTREEITEM hTree;            // 树形控件句柄
}SCAN_PARAM, *PSCAN_PARAM;

typedef struct _THREAD_PARAM
{
    DWORD dwStartIp;            // 开始IP地址
    DWORD dwEndIp;              // 结束IP地址
    WORD dwStartPort;           // 开始端口号
    WORD dwEndPort;             // 结束端口号
    CMultiPortScanDlg *pThis;   // 主窗口的this指针
    HANDLE hEvent;              // 事件句柄
}THREAD_PARAM, *PTHREAD_PARAM;

// 最大线程数，用于控制信号量数量
#define MAX_THREAD 10

class CMultiPortScanDlg : public CDialog
{
// Construction
public:
	static DWORD WINAPI ScanThread(LPVOID lpParam);
	static DWORD WINAPI MainThread(LPVOID lpParam);
	CMultiPortScanDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CMultiPortScanDlg)
	enum { IDD = IDD_MULTIPORTSCAN_DIALOG };
	CTreeCtrl	m_IpTree;
	CIPAddressCtrl	m_IpEnd;
	CIPAddressCtrl	m_IpStart;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMultiPortScanDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CMultiPortScanDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBtnscan();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MULTIPORTSCANDLG_H__FA1A8EBB_D0E4_45E4_92EA_ED404C1E1E07__INCLUDED_)
