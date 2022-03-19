// SetExeFileName.cpp : 实现文件
//

#include "stdafx.h"
#include "TestRemoteConnection.h"
#include "SetExeFileName.h"
#include "afxdialogex.h"


// SetExeFileName 对话框

IMPLEMENT_DYNAMIC(SetExeFileName, CDialogEx)

SetExeFileName::SetExeFileName(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_EXEFILENAME, pParent)
{

}

SetExeFileName::~SetExeFileName()
{
}

void SetExeFileName::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_exeFileName);
}


BEGIN_MESSAGE_MAP(SetExeFileName, CDialogEx)
	ON_BN_CLICKED(IDOK, &SetExeFileName::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &SetExeFileName::OnBnClickedButton1)
	ON_BN_CLICKED(IDCANCEL, &SetExeFileName::OnBnClickedCancel)
END_MESSAGE_MAP()


// SetExeFileName 消息处理程序


void SetExeFileName::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码

	exefilepath = L"";
	m_exeFileName.GetWindowText(exefilepath);

	if ( exefilepath.GetLength() == 0  ||  ! PathFileExists(exefilepath)  )
	{
		::MessageBox( NULL,L"请正确指定执行文件的路径",L"提示",MB_OK | MB_ICONERROR  );
		return;
	}

	CDialogEx::OnOK();
}


void SetExeFileName::OnBnClickedButton1()
{

	// TODO: 在此添加控件通知处理程序代码	
	CFileDialog GetFile( TRUE, NULL, NULL, OFN_FILEMUSTEXIST, L"EXE File(*.exe)|*.exe|BAT Files(*.bat)|*.bat||");

	if ( IDOK == GetFile.DoModal())
	{

		m_exeFileName.SetWindowText(GetFile.GetPathName() );
	}

	
}


void SetExeFileName::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}

CString SetExeFileName::Get_exefilepath()
{
	return exefilepath;
}
