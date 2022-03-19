#pragma once
#include "afxwin.h"


// SetExeFileName �Ի���

class SetExeFileName : public CDialogEx
{
	DECLARE_DYNAMIC(SetExeFileName)

public:
	SetExeFileName(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~SetExeFileName();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EXEFILENAME };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
