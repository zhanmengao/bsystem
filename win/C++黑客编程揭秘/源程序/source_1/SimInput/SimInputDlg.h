// SimInputDlg.h : header file
//

#if !defined(AFX_SIMINPUTDLG_H__CB1B8E5A_A616_4456_AFB1_5FC56949A1AA__INCLUDED_)
#define AFX_SIMINPUTDLG_H__CB1B8E5A_A616_4456_AFB1_5FC56949A1AA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CSimInputDlg dialog

class CSimInputDlg : public CDialog
{
// Construction
public:
	HWND m_hWnd;
	CString m_StrCaption;
	VOID FindAndFocus();
	CSimInputDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CSimInputDlg)
	enum { IDD = IDD_SIMINPUT_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSimInputDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CSimInputDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBtnSimkeybd();
	afx_msg void OnBtnSimmouse();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SIMINPUTDLG_H__CB1B8E5A_A616_4456_AFB1_5FC56949A1AA__INCLUDED_)
