#pragma once
#include "afxwin.h"


// CUserInfoDlg �Ի���

class CUserInfoDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CUserInfoDlg)

public:
	CUserInfoDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CUserInfoDlg();

// �Ի�������
	enum { IDD = IDD_USERINFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
