#pragma once
#include "afxwin.h"
#include "Resource.h"


// NewPasswordDlg 对话框

class NewPasswordDlg : public CDialogEx
{
	DECLARE_DYNAMIC(NewPasswordDlg)

public:
	NewPasswordDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~NewPasswordDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_newPstr;
	CString m_str;
	afx_msg void OnBnClickedOk();
};
