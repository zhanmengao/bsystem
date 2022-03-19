// CopyDataSDlg.h : header file
//

#if !defined(AFX_COPYDATASDLG_H__B67C4122_B7D4_4EF7_908D_2588F6561B56__INCLUDED_)
#define AFX_COPYDATASDLG_H__B67C4122_B7D4_4EF7_908D_2588F6561B56__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CCopyDataSDlg dialog

class CCopyDataSDlg : public CDialog
{
// Construction
public:
	CCopyDataSDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CCopyDataSDlg)
	enum { IDD = IDD_COPYDATAS_DIALOG };
	CListBox	m_ListData;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCopyDataSDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CCopyDataSDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
	afx_msg void OnBtnDelall();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COPYDATASDLG_H__B67C4122_B7D4_4EF7_908D_2588F6561B56__INCLUDED_)
