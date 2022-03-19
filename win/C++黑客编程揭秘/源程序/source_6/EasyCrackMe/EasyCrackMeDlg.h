// EasyCrackMeDlg.h : header file
//

#if !defined(AFX_EASYCRACKMEDLG_H__1BD88A1A_5471_48E7_A5F4_6456ED3A5BE9__INCLUDED_)
#define AFX_EASYCRACKMEDLG_H__1BD88A1A_5471_48E7_A5F4_6456ED3A5BE9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CEasyCrackMeDlg dialog

class CEasyCrackMeDlg : public CDialog
{
// Construction
public:
	CEasyCrackMeDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CEasyCrackMeDlg)
	enum { IDD = IDD_EASYCRACKME_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEasyCrackMeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CEasyCrackMeDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBtnEnd();
	afx_msg void OnBtnReg();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EASYCRACKMEDLG_H__1BD88A1A_5471_48E7_A5F4_6456ED3A5BE9__INCLUDED_)
