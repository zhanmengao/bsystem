// NewPasswordDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "NewPasswordDlg.h"
#include "afxdialogex.h"


// NewPasswordDlg �Ի���

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


// NewPasswordDlg ��Ϣ�������


void NewPasswordDlg::OnBnClickedOk()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	//CDialogEx::OnOK();
		
	m_newPstr.GetWindowText(m_str);

	if (m_newPstr.GetWindowTextLength() == 0)
	{
		AfxMessageBox(  L"�������ÿ�����"  );
		return;
	}
	//AfxMessageBox( m_str );
	CDialogEx::OnOK();
}
