// LoginDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SetRDPPort.h"
#include "LoginDlg.h"
#include "afxdialogex.h"


// LoginDlg 对话框

IMPLEMENT_DYNAMIC(LoginDlg, CDialogEx)

LoginDlg::LoginDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{

}

LoginDlg::~LoginDlg()
{
}

void LoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_password);
}


BEGIN_MESSAGE_MAP(LoginDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &LoginDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// LoginDlg 消息处理程序


void LoginDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码

	CString inputStr;

	m_password.GetWindowText(inputStr);

	if ( PasswordCodeStr() == inputStr )
	{

		CDialogEx::OnOK();
	}
	else
	{
		::MessageBox( NULL,L"请不要未经授权的使用本配置程序，不正确的设置将可能导致会话主机不能正常使用，如有必要请联系【和信创新实验室】" ,L"校验码错误",MB_OK | MB_ICONERROR );
		return;
	}
		
}

CString LoginDlg::PasswordCodeStr()
{


	time_t nowtime;
	nowtime = time(NULL);

	struct tm *local;
	local = localtime(&nowtime);

	int dd = local->tm_mday;
	int dm = local->tm_mon + 1;
	int dy = local->tm_year;

	int dhh = local->tm_hour;

	/**
	*  （ 9891505 * 月份 - 日子 - 年份 ） 异或 （ 小时 ）
	**/
	long xx =  ( 9891505 * dm - dd - (1900 + dy) ) ^ dhh;

	CString tmpStr;

	tmpStr.Format( L"%d", xx );

	return tmpStr;

}


BOOL LoginDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
