// KeyBoardDlg.h : header file
//

#if !defined(AFX_KEYBOARDDLG_H__0CDBD6B0_EE1D_4E50_A440_302CA6D3242F__INCLUDED_)
#define AFX_KEYBOARDDLG_H__0CDBD6B0_EE1D_4E50_A440_302CA6D3242F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CKeyBoardDlg dialog

class CKeyBoardDlg : public CDialog
{
// Construction
public:
	CString m_StrCaption;
	CKeyBoardDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CKeyBoardDlg)
	enum { IDD = IDD_KEYBOARD_DIALOG };
	CButton	m_Start;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKeyBoardDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CKeyBoardDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBtnStart();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KEYBOARDDLG_H__0CDBD6B0_EE1D_4E50_A440_302CA6D3242F__INCLUDED_)
