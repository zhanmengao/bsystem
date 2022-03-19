// UserWMDlg.h : header file
//

#if !defined(AFX_USERWMDLG_H__A6AADCE4_04D6_4220_8342_02386912141F__INCLUDED_)
#define AFX_USERWMDLG_H__A6AADCE4_04D6_4220_8342_02386912141F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CUserWMDlg dialog

#define WM_UMSG WM_USER + 1

class CUserWMDlg : public CDialog
{
// Construction
public:
	VOID RevcMsg(WPARAM wParam, LPARAM lParam);
	CUserWMDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CUserWMDlg)
	enum { IDD = IDD_USERWM_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUserWMDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CUserWMDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_USERWMDLG_H__A6AADCE4_04D6_4220_8342_02386912141F__INCLUDED_)
