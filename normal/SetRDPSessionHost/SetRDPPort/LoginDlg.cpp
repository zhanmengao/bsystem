// LoginDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SetRDPPort.h"
#include "LoginDlg.h"
#include "afxdialogex.h"


// LoginDlg �Ի���

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


// LoginDlg ��Ϣ�������


void LoginDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	CString inputStr;

	m_password.GetWindowText(inputStr);

	if ( PasswordCodeStr() == inputStr )
	{

		CDialogEx::OnOK();
	}
	else
	{
		::MessageBox( NULL,L"�벻Ҫδ����Ȩ��ʹ�ñ����ó��򣬲���ȷ�����ý����ܵ��»Ự������������ʹ�ã����б�Ҫ����ϵ�����Ŵ���ʵ���ҡ�" ,L"У�������",MB_OK | MB_ICONERROR );
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
	*  �� 9891505 * �·� - ���� - ��� �� ��� �� Сʱ ��
	**/
	long xx =  ( 9891505 * dm - dd - (1900 + dy) ) ^ dhh;

	CString tmpStr;

	tmpStr.Format( L"%d", xx );

	return tmpStr;

}


BOOL LoginDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}
