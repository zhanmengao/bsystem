#pragma once
#include "afxwin.h"


// HookSetup 对话框

class HookSetup : public CDialogEx
{
	DECLARE_DYNAMIC(HookSetup)

public:
	HookSetup(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~HookSetup();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG3 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:

	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();	
	afx_msg	void AddSysStart();
	afx_msg	void OnBnClickedButton1();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton7();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio1();

	CEdit m_x86DLL;
	CEdit m_x64DLL;
	CEdit m_x86Exe;
	CEdit m_x64Exe;
	CButton m_AddSysReg;
	CButton m_AddSysStart;

private:
	void AddSysReg();
	
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton2();
};
