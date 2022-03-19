#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// SetPathDlg �Ի���

class SetPathDlg : public CDialogEx
{
	DECLARE_DYNAMIC(SetPathDlg)

public:
	SetPathDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~SetPathDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
