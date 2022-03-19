// SendMsgDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TestRemoteConnection.h"
#include "SendMsgDlg.h"
#include "afxdialogex.h"


// SendMsgDlg �Ի���

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


// SendMsgDlg ��Ϣ�������


CString SendMsgDlg::GetSendText()
{
	return this->sendText;
}

void SendMsgDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnCancel();
}


void SendMsgDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	CString text;
	m_msgTxt.GetWindowText( text );
	
	if (  text.GetLength() == 0  ||  text.GetLength() > 256  )
	{
		AfxMessageBox(L"��Ҫ������Ŷ����");
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

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	m_msgTxt.SetWindowText(L"�°�һ��Է���");


	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}
