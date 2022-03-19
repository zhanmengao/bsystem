#if !defined(AFX_REGADD_H__21786919_3EF8_4698_B705_D4157A4287B4__INCLUDED_)
#define AFX_REGADD_H__21786919_3EF8_4698_B705_D4157A4287B4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RegAdd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRegAdd dialog

class CRegAdd : public CDialog
{
// Construction
public:
    char m_szKeyValue[MAX_PATH];
	char m_szKeyName[MAXBYTE];
	CRegAdd(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CRegAdd)
	enum { IDD = IDD_DLG_ADD };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRegAdd)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRegAdd)
	afx_msg void OnBtnOk();
	afx_msg void OnBtnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REGADD_H__21786919_3EF8_4698_B705_D4157A4287B4__INCLUDED_)
