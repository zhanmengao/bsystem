#pragma once
#include "afxwin.h"


// SetLogobmp �Ի���

class SetLogobmp : public CDialogEx
{
	DECLARE_DYNAMIC(SetLogobmp)

public:
	SetLogobmp(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~SetLogobmp();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG4 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CEdit m_sPath;
	virtual BOOL OnInitDialog();
};
