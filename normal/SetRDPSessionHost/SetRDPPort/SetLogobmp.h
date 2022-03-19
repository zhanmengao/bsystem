#pragma once
#include "afxwin.h"


// SetLogobmp 对话框

class SetLogobmp : public CDialogEx
{
	DECLARE_DYNAMIC(SetLogobmp)

public:
	SetLogobmp(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~SetLogobmp();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG4 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CEdit m_sPath;
	virtual BOOL OnInitDialog();
};
