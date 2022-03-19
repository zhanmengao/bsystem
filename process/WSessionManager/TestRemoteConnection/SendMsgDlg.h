#pragma once
#include "afxwin.h"


// SendMsgDlg �Ի���

class SendMsgDlg : public CDialogEx
{
	DECLARE_DYNAMIC(SendMsgDlg)

public:
	SendMsgDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~SendMsgDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:

	CString GetSendText();

	afx_msg void OnBnClickedCancel();
	CEdit m_msgTxt;
	afx_msg void OnBnClickedOk();


private:
	CString sendText;

public:
	virtual BOOL OnInitDialog();
};
