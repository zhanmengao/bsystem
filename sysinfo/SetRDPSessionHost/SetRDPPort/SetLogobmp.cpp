// SetLogobmp.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SetLogobmp.h"
#include "afxdialogex.h"

#include "resource.h"
#include "RegEdit.h"

// SetLogobmp �Ի���

IMPLEMENT_DYNAMIC(SetLogobmp, CDialogEx)

SetLogobmp::SetLogobmp(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG4, pParent)
{

}

SetLogobmp::~SetLogobmp()
{
}

void SetLogobmp::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_sPath);
}


BEGIN_MESSAGE_MAP(SetLogobmp, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &SetLogobmp::OnBnClickedButton1)
	//ON_BN_CLICKED(IDC_BUTTON2, &SetLogobmp::OnBnClickedButton2)
	ON_BN_CLICKED(IDOK, &SetLogobmp::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &SetLogobmp::OnBnClickedCancel)
END_MESSAGE_MAP()


// SetLogobmp ��Ϣ�������


void SetLogobmp::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	CFileDialog GetFile(TRUE,
		NULL,
		NULL,
		OFN_FILEMUSTEXIST,
		L"jpg file (*.jpg)|*.jpg|png files (*.png)|*.png||");

	if (GetFile.DoModal())
	{

		m_sPath.SetWindowText(GetFile.GetPathName());
	}

}





void SetLogobmp::OnBnClickedOk()
{
	
	CString bmpPath;

	m_sPath.GetWindowText( bmpPath );

	if ( bmpPath.GetLength() == 0  ) {
	
		::MessageBox(NULL,L"δָ��ͼƬ�ļ�",L"��ʾ",MB_OK|MB_ICONASTERISK );
		return;
	}

	CRegEdit::SetLockScreenImage( bmpPath );

	CDialogEx::OnOK();
}


void SetLogobmp::OnBnClickedCancel()
{
	
	CDialogEx::OnCancel();
}


BOOL SetLogobmp::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	WCHAR BMPPATH[MAX_PATH] = { 0 };
	CRegEdit::GetLockScreenImage(BMPPATH);

	m_sPath.SetWindowText(BMPPATH);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}
