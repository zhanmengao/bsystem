#pragma once
#include "afxwin.h"


// LogonScripts 对话框

class LogonScripts : public CDialogEx
{
	DECLARE_DYNAMIC(LogonScripts)

public:
	LogonScripts(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~LogonScripts();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	CEdit m_sPath;
	afx_msg void OnBnClickedOk();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnDropFiles(HDROP hDropInfo);
};
