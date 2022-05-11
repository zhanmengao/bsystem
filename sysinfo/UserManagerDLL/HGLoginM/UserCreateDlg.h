#pragma once
#include "afxwin.h"
#include "Resource.h"


// CUserCreateDlg �Ի���

class CUserCreateDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CUserCreateDlg)

public:
	CUserCreateDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CUserCreateDlg();

// �Ի�������
	enum { IDD = IDD_CUSERSETUP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	void SZTChong();
	CEdit SLEdit;
	CEdit PawStrEdit;
	CEdit UNzEdit;
	CEdit b0Edit;
	CEdit SZsEdit;
	CEdit SZeEdit;
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnEnChangeEdit5();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	BOOL TestUserName();
	afx_msg void OnEnChangeEdit3();
	afx_msg void OnEnChangeEdit4();
	afx_msg void OnBnClickedCheck1();
	//CString SCnewUserName( int zz_i );
	CString SCnewUserName( int zz_i,CString& bwStr );
	CButton PawTUserN;
	CButton PawTIPs;
	afx_msg void OnBnClickedCheck2();
	afx_msg void OnBnClickedCheck3();
	CButton PawTUAM;
};
