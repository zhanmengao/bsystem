// InjectDllDlg.h : header file
//

#if !defined(AFX_INJECTDLLDLG_H__D207D2C7_6BF9_47FD_B765_2F22C2DD9F3E__INCLUDED_)
#define AFX_INJECTDLLDLG_H__D207D2C7_6BF9_47FD_B765_2F22C2DD9F3E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CInjectDllDlg dialog

class CInjectDllDlg : public CDialog
{
// Construction
public:
	VOID UnInjectDll(DWORD dwPid, char *szDllName);
	VOID InjectDll(DWORD dwPid, char *szDllName);
	DWORD GetProcId(char *szProcessName);
	VOID DebugPrivilege();
	CInjectDllDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CInjectDllDlg)
	enum { IDD = IDD_INJECTDLL_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInjectDllDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CInjectDllDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBtnInject();
	afx_msg void OnBtnUninject();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INJECTDLLDLG_H__D207D2C7_6BF9_47FD_B765_2F22C2DD9F3E__INCLUDED_)
