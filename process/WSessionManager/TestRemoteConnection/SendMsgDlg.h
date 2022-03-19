#pragma once
#include "afxwin.h"


// SendMsgDlg 对话框

class SendMsgDlg : public CDialogEx
{
	DECLARE_DYNAMIC(SendMsgDlg)

public:
	SendMsgDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~SendMsgDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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
