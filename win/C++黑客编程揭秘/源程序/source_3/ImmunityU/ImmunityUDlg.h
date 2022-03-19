// ImmunityUDlg.h : header file
//

#if !defined(AFX_IMMUNITYUDLG_H__AB5A6852_D59A_42B4_B44B_63ABD302802B__INCLUDED_)
#define AFX_IMMUNITYUDLG_H__AB5A6852_D59A_42B4_B44B_63ABD302802B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CImmunityUDlg dialog

class CImmunityUDlg : public CDialog
{
// Construction
public:
	VOID InitComboDrive();
	CImmunityUDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CImmunityUDlg)
	enum { IDD = IDD_IMMUNITYU_DIALOG };
	CComboBox	m_CbDrive;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImmunityUDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CImmunityUDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBtnImmunity();
	afx_msg void OnBtnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMMUNITYUDLG_H__AB5A6852_D59A_42B4_B44B_63ABD302802B__INCLUDED_)
