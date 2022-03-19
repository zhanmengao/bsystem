#pragma once
#include "afxwin.h"


// CUserInfoDlg 对话框

class CUserInfoDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CUserInfoDlg)

public:
	CUserInfoDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CUserInfoDlg();

// 对话框数据
	enum { IDD = IDD_USERINFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

protected:
	CString UserName;
	int     mList_Sel_i;

public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();

	void SetFindUserName( CString uStr ,int mSel_i );


	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	CEdit UserNameEdit;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	void TCUserInfo( CString UserName );
	void RSUserDlgShow( int mlist_i );
	CListBox GroupListBox1;
};
