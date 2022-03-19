// CopyDataCDlg.h : header file
//

#if !defined(AFX_COPYDATACDLG_H__34B93FCF_0B5D_43B0_ABF4_9BE938C97F2A__INCLUDED_)
#define AFX_COPYDATACDLG_H__34B93FCF_0B5D_43B0_ABF4_9BE938C97F2A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CCopyDataCDlg dialog

class CCopyDataCDlg : public CDialog
{
// Construction
public:
	CCopyDataCDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CCopyDataCDlg)
	enum { IDD = IDD_COPYDATAC_DIALOG };
	CListBox	m_ListRec;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCopyDataCDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CCopyDataCDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBtnSend();
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COPYDATACDLG_H__34B93FCF_0B5D_43B0_ABF4_9BE938C97F2A__INCLUDED_)
