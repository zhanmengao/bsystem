// SendMsgDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TestRemoteConnection.h"
#include "SendMsgDlg.h"
#include "afxdialogex.h"


// SendMsgDlg 对话框

IMPLEMENT_DYNAMIC(SendMsgDlg, CDialogEx)

SendMsgDlg::SendMsgDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{

}

SendMsgDlg::~SendMsgDlg()
{
}

void SendMsgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_msgTxt);
}


BEGIN_MESSAGE_MAP(SendMsgDlg, CDialogEx)
	ON_BN_CLICKED(IDCANCEL, &SendMsgDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &SendMsgDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// SendMsgDlg 消息处理程序


CString SendMsgDlg::GetSendText()
{
	return this->sendText;
}

void SendMsgDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}


void SendMsgDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码

	CString text;
	m_msgTxt.GetWindowText( text );
	
	if (  text.GetLength() == 0  ||  text.GetLength() > 256  )
	{
		AfxMessageBox(L"不要搞事情哦！！");
		return;
	}
	else
	{
		this->sendText = text;
	}


	CDialogEx::OnOK();
}


BOOL SendMsgDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	m_msgTxt.SetWindowText(L"下班一起吃饭吗？");


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
