// ManageServicesDlg.h : header file
//

#if !defined(AFX_MANAGESERVICESDLG_H__15A6DCFE_CA0E_4496_A23D_58AA949B204E__INCLUDED_)
#define AFX_MANAGESERVICESDLG_H__15A6DCFE_CA0E_4496_A23D_58AA949B204E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CManageServicesDlg dialog

class CManageServicesDlg : public CDialog
{
// Construction
public:
	VOID ShowServiceList(DWORD dwServiceType = SERVICE_WIN32);
	VOID InitServiceList();
	CManageServicesDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CManageServicesDlg)
	enum { IDD = IDD_MANAGESERVICES_DIALOG };
	CListCtrl	m_ServiceList;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CManageServicesDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CManageServicesDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnRadioWin32();
	afx_msg void OnRadioDriver();
	afx_msg void OnBtnStart();
	afx_msg void OnBtnStop();
	afx_msg void OnBtnExit();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MANAGESERVICESDLG_H__15A6DCFE_CA0E_4496_A23D_58AA949B204E__INCLUDED_)
