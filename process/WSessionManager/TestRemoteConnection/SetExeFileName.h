#pragma once
#include "afxwin.h"


// SetExeFileName 对话框

class SetExeFileName : public CDialogEx
{
	DECLARE_DYNAMIC(SetExeFileName)

public:
	SetExeFileName(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~SetExeFileName();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EXEFILENAME };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedCancel();
	CEdit m_exeFileName;

	CString Get_exefilepath();

private:
	CString exefilepath;

};
