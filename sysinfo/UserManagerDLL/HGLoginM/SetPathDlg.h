#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// SetPathDlg 对话框

class SetPathDlg : public CDialogEx
{
	DECLARE_DYNAMIC(SetPathDlg)

public:
	SetPathDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~SetPathDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	CString BrowseOutDir();
	CEdit PathStrEdit;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
	CIPAddressCtrl sIPEdit;
};
