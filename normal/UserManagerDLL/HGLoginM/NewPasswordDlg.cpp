// NewPasswordDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "NewPasswordDlg.h"
#include "afxdialogex.h"


// NewPasswordDlg 对话框

IMPLEMENT_DYNAMIC(NewPasswordDlg, CDialogEx)

NewPasswordDlg::NewPasswordDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(NewPasswordDlg::IDD, pParent)
{

}

NewPasswordDlg::~NewPasswordDlg()
{
}

void NewPasswordDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_newPstr);
}


BEGIN_MESSAGE_MAP(NewPasswordDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &NewPasswordDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// NewPasswordDlg 消息处理程序


void NewPasswordDlg::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码
	//CDialogEx::OnOK();
		
	m_newPstr.GetWindowText(m_str);

	if (m_newPstr.GetWindowTextLength() == 0)
	{
		AfxMessageBox(  L"不可设置空密码"  );
		return;
	}
	//AfxMessageBox( m_str );
	CDialogEx::OnOK();
}
