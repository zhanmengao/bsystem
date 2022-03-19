// ManageProcessDlg.h : header file
//

#if !defined(AFX_MANAGEPROCESSDLG_H__88218AFB_AD0A_4C5F_9BA1_0D24DBA2D223__INCLUDED_)
#define AFX_MANAGEPROCESSDLG_H__88218AFB_AD0A_4C5F_9BA1_0D24DBA2D223__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CManageProcessDlg dialog

class CManageProcessDlg : public CDialog
{
// Construction
public:
	VOID DebugPrivilege();
	int GetSelectPid();
	VOID ShowModule();
	VOID ShowProcess();
	VOID InitProcessList();
	CManageProcessDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CManageProcessDlg)
	enum { IDD = IDD_MANAGEPROCESS_DIALOG };
	CListCtrl	m_ListModule;
	CListCtrl	m_ListProcess;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CManageProcessDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CManageProcessDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBtnCreate();
	afx_msg void OnBtnTerminate();
	afx_msg void OnBtnStop();
	afx_msg void OnBtnResume();
	afx_msg void OnBtnShowmodule();
	afx_msg void OnBtnExit();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MANAGEPROCESSDLG_H__88218AFB_AD0A_4C5F_9BA1_0D24DBA2D223__INCLUDED_)
