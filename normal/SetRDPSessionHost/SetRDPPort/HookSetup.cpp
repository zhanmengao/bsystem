// HookSetup.cpp : 实现文件
//

#include "stdafx.h"
#include "HookSetup.h"
#include "afxdialogex.h"
#include "resource.h"

#include "RegEdit.h"
#include <vector>


// HookSetup 对话框

IMPLEMENT_DYNAMIC(HookSetup, CDialogEx)

HookSetup::HookSetup(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG3, pParent)
{

}

HookSetup::~HookSetup()
{
}

void HookSetup::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_EDIT1, m_x86DLL);
	DDX_Control(pDX, IDC_EDIT2, m_x64DLL);
	DDX_Control(pDX, IDC_EDIT4, m_x86Exe);
	DDX_Control(pDX, IDC_EDIT5, m_x64Exe);
	DDX_Control(pDX, IDC_RADIO1, m_AddSysReg);
	DDX_Control(pDX, IDC_RADIO2, m_AddSysStart);
}


BEGIN_MESSAGE_MAP(HookSetup, CDialogEx)
	ON_BN_CLICKED(IDCANCEL, &HookSetup::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &HookSetup::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &HookSetup::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON5, &HookSetup::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &HookSetup::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, &HookSetup::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_RADIO2,  &HookSetup::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO1,  &HookSetup::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_BUTTON2, &HookSetup::OnBnClickedButton2)
END_MESSAGE_MAP()


// HookSetup 消息处理程序


void HookSetup::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码

	//

	CDialogEx::OnCancel();
}


void HookSetup::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码

	if ( m_AddSysReg.GetCheck() == BST_CHECKED)
	{
		/**
		*  注入 注册表 方式
		**/
		AddSysReg();

	}
	else
	{
		/**
		* 设置 启动脚本 方式
		**/
		AddSysStart();
	}	

	//CDialogEx::OnOK();
}

void HookSetup::AddSysReg()
{
	CString x86dllfile;
	CString x64dllfile;

	m_x86DLL.GetWindowText(x86dllfile);
	m_x64DLL.GetWindowText(x64dllfile);

	if (x86dllfile.GetLength() <= 0 || x64dllfile.GetLength() <= 0  )
	{

		AfxMessageBox(L"指定加载文件不可空...");
		return;
	}
	else
	{
		CRegEdit::DelScriptsStartup0();
		CRegEdit::SetAppInitDLLs(x86dllfile, x64dllfile);

		::MessageBox(NULL, x86dllfile + L"\n"+ x64dllfile, L"DLL HOOK 加载", MB_OK | MB_ICONASTERISK);

		CDialogEx::OnOK();
	}
	

}


void HookSetup::AddSysStart()
{

	CString x86exelfile;
	CString x64exelfile;

	m_x86Exe.GetWindowText(x86exelfile);
	m_x64Exe.GetWindowText(x64exelfile);

	if (x86exelfile.GetLength() <= 0 || x64exelfile.GetLength() <= 0)
	{

		AfxMessageBox(L"指定加载文件不可空...");
		return;
	}
	else
	{
		CRegEdit::DelAppInitDLLs();
		
		CRegEdit::SetScriptsLogon(x86exelfile, 1);
		CRegEdit::SetScriptsLogon(x64exelfile, 2);

		::MessageBox( NULL, x86exelfile + L"\n" + x64exelfile,L"启动脚本加载", MB_OK | MB_ICONASTERISK  );

		CDialogEx::OnOK();
	}
	

}

void HookSetup::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	
	/**
	* x86 DLL
	*/
	CFileDialog GetFile(TRUE,
		NULL,
		NULL,
		OFN_FILEMUSTEXIST,
		L"DLL File (*.dll)|*.dll||");

	if (GetFile.DoModal())
	{
		m_x86DLL.SetWindowText(GetFile.GetPathName());		
	}

}


void HookSetup::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码
	
	/**
	* x64 DLL
	*/
	CFileDialog GetFile(TRUE,
		NULL,
		NULL,
		OFN_FILEMUSTEXIST,
		L"DLL File (*.dll)|*.dll||");

	if (GetFile.DoModal())
	{
		m_x64DLL.SetWindowText(GetFile.GetPathName());
	}

}


void HookSetup::OnBnClickedButton6()
{
	// TODO: 在此添加控件通知处理程序代码
	
	/**
	* x86 EXE
	*/
	CFileDialog GetFile(TRUE,
		NULL,
		NULL,
		OFN_FILEMUSTEXIST,
		L"EXE File (*.exe)|*.exe||");

	if (GetFile.DoModal())
	{
		m_x86Exe.SetWindowText(GetFile.GetPathName());
	}
}


void HookSetup::OnBnClickedButton7()
{
	// TODO: 在此添加控件通知处理程序代码
	
	/**
	* x64 EXE
	*/
	CFileDialog GetFile(TRUE,
		NULL,
		NULL,
		OFN_FILEMUSTEXIST,
		L"EXE File (*.exe)|*.exe||");

	if (GetFile.DoModal())
	{
		m_x64Exe.SetWindowText(GetFile.GetPathName());
	}

}


void HookSetup::OnBnClickedRadio2()
{
	// TODO: 在此添加控件通知处理程序代码
}


void HookSetup::OnBnClickedRadio1()
{
	// TODO: 在此添加控件通知处理程序代码
}


BOOL HookSetup::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	std::vector<CString> dllfiles =  CRegEdit::GetAppInitDLLs();	
	CString exefiles1 =  CRegEdit::GetScriptsLogon(1);
	CString exefiles2 =  CRegEdit::GetScriptsLogon(2);

	m_x86DLL.SetWindowText(dllfiles.at(0));
	m_x64DLL.SetWindowText(dllfiles.at(1));
	
	m_x86Exe.SetWindowText(exefiles1);
	m_x64Exe.SetWindowText(exefiles2);
	
	
	if ( dllfiles.at(0).GetLength() >0 )
	{
		m_AddSysReg.SetCheck(BST_CHECKED);
	}
	else
	{
		m_AddSysReg.SetCheck(BST_UNCHECKED);
	}

	if ( exefiles1.GetLength() > 0)
	{
		m_AddSysStart.SetCheck(BST_CHECKED);
	}
	else
	{
		m_AddSysStart.SetCheck(BST_UNCHECKED);
	}
	
	
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void HookSetup::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码

	//
	CRegEdit::DelAppInitDLLs();
	CRegEdit::DelScriptsStartup0();

	m_x86DLL.SetWindowText(L"");
	m_x64DLL.SetWindowText(L"");
	m_x86Exe.SetWindowText(L"");
	m_x64Exe.SetWindowText(L"");

	m_AddSysReg.SetCheck(BST_UNCHECKED);
	m_AddSysStart.SetCheck(BST_UNCHECKED);


	CDialogEx::OnOK();
}
