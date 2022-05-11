// LogonScripts.cpp : 实现文件
//

#include "stdafx.h"
#include "LogonScripts.h"
#include "afxdialogex.h"

#include "resource.h"

#include "RegEdit.h"



// LogonScripts 对话框

IMPLEMENT_DYNAMIC(LogonScripts, CDialogEx)

LogonScripts::LogonScripts(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG2, pParent)
{

}

LogonScripts::~LogonScripts()
{
}

void LogonScripts::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_sPath);
}


BEGIN_MESSAGE_MAP(LogonScripts, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &LogonScripts::OnBnClickedButton1)
	ON_BN_CLICKED(IDOK, &LogonScripts::OnBnClickedOk)
	ON_WM_SHOWWINDOW()
	ON_WM_DROPFILES()	
END_MESSAGE_MAP()


// LogonScripts 消息处理程序


void LogonScripts::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog GetFile( TRUE,
		NULL,
		NULL,
		OFN_FILEMUSTEXIST,
		L"exe file (*.exe)|*.exe|bat files (*.bat)|*.bat|ps files (*.p1)|*.p1||");

	if ( GetFile.DoModal() )
	{
		
		m_sPath.SetWindowText(GetFile.GetPathName());
	}

}


void LogonScripts::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码


	CString spathStr;
	m_sPath.GetWindowText( spathStr );

	AfxMessageBox(spathStr);

	if (  spathStr.GetLength() == 0 || spathStr.GetLength() > MAX_PATH )
	{
		return;
	}


	CRegEdit::SetScriptsLogon0( spathStr );
	

	CDialogEx::OnOK();
}


void LogonScripts::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: 在此处添加消息处理程序代码

	CString filename =  CRegEdit::GetScriptsLogon0();

	m_sPath.SetWindowText( filename  );
}


void LogonScripts::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	int DropCount = DragQueryFile(hDropInfo, -1, NULL, 0);

	for (int i = 0; i< DropCount; i++)
	{
		WCHAR wcStr[MAX_PATH];
		DragQueryFile(hDropInfo, i, wcStr, MAX_PATH);
		AfxMessageBox(wcStr);
	}
	DragFinish(hDropInfo); 


	CDialogEx::OnDropFiles(hDropInfo);
}
