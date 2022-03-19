// NoDllInjectDlg.h : header file
//

#if !defined(AFX_NODLLINJECTDLG_H__7049CBFD_60F0_4FB9_BCEB_09198F6DFAC2__INCLUDED_)
#define AFX_NODLLINJECTDLG_H__7049CBFD_60F0_4FB9_BCEB_09198F6DFAC2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CNoDllInjectDlg dialog

class CNoDllInjectDlg : public CDialog
{
// Construction
public:
	VOID InjectCode(DWORD dwPid);
    DWORD GetProcId(char *szProcessName);
    VOID DebugPrivilege();
	CNoDllInjectDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CNoDllInjectDlg)
	enum { IDD = IDD_NODLLINJECT_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNoDllInjectDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CNoDllInjectDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBtnInject();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NODLLINJECTDLG_H__7049CBFD_60F0_4FB9_BCEB_09198F6DFAC2__INCLUDED_)
