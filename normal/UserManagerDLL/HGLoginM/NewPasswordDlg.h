#pragma once
#include "afxwin.h"
#include "Resource.h"


// NewPasswordDlg �Ի���

class NewPasswordDlg : public CDialogEx
{
	DECLARE_DYNAMIC(NewPasswordDlg)

public:
	NewPasswordDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~NewPasswordDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_newPstr;
	CString m_str;
	afx_msg void OnBnClickedOk();
};
