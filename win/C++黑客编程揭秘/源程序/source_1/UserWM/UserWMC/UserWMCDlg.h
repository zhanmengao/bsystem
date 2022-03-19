// UserWMCDlg.h : header file
//

#if !defined(AFX_USERWMCDLG_H__7C707E58_2F9C_4CD0_BA93_9411A131F7D2__INCLUDED_)
#define AFX_USERWMCDLG_H__7C707E58_2F9C_4CD0_BA93_9411A131F7D2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CUserWMCDlg dialog

#define WM_UMSG WM_USER + 1

class CUserWMCDlg : public CDialog
{
// Construction
public:
	CUserWMCDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CUserWMCDlg)
	enum { IDD = IDD_USERWMC_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUserWMCDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CUserWMCDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBtnSend();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_USERWMCDLG_H__7C707E58_2F9C_4CD0_BA93_9411A131F7D2__INCLUDED_)
