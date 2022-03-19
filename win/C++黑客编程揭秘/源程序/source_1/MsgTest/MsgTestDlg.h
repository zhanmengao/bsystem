// MsgTestDlg.h : header file
//

#if !defined(AFX_MSGTESTDLG_H__635C0D57_6164_4827_9803_227894304992__INCLUDED_)
#define AFX_MSGTESTDLG_H__635C0D57_6164_4827_9803_227894304992__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CMsgTestDlg dialog

class CMsgTestDlg : public CDialog
{
// Construction
public:
	CMsgTestDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CMsgTestDlg)
	enum { IDD = IDD_MSGTEST_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMsgTestDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CMsgTestDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnClose();
	afx_msg void OnExec();
	afx_msg void OnEditWnd();
	afx_msg void OnGetWnd();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MSGTESTDLG_H__635C0D57_6164_4827_9803_227894304992__INCLUDED_)
