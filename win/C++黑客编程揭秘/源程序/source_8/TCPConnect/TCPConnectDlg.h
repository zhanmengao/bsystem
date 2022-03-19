// TCPConnectDlg.h : header file
//

#if !defined(AFX_TCPCONNECTDLG_H__98658B03_B0AD_4958_805C_53E12A481CC9__INCLUDED_)
#define AFX_TCPCONNECTDLG_H__98658B03_B0AD_4958_805C_53E12A481CC9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CTCPConnectDlg dialog

class CTCPConnectDlg : public CDialog
{
// Construction
public:
	CTCPConnectDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CTCPConnectDlg)
	enum { IDD = IDD_TCPCONNECT_DIALOG };
	CListBox	m_ListPort;
	CIPAddressCtrl	m_IpAddr;
	CButton	m_BtnScan;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTCPConnectDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CTCPConnectDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBtnScan();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TCPCONNECTDLG_H__98658B03_B0AD_4958_805C_53E12A481CC9__INCLUDED_)
