// LoadNtDriverDlg.h : header file
//

#if !defined(AFX_LOADNTDRIVERDLG_H__8155540F_9D3E_4B67_9AAE_11DD0876B107__INCLUDED_)
#define AFX_LOADNTDRIVERDLG_H__8155540F_9D3E_4B67_9AAE_11DD0876B107__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CLoadNtDriverDlg dialog

class CLoadNtDriverDlg : public CDialog
{
// Construction
public:
	CLoadNtDriverDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CLoadNtDriverDlg)
	enum { IDD = IDD_LOADNTDRIVER_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLoadNtDriverDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CLoadNtDriverDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnLoad();
	afx_msg void OnUnload();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOADNTDRIVERDLG_H__8155540F_9D3E_4B67_9AAE_11DD0876B107__INCLUDED_)
