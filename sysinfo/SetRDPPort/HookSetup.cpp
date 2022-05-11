// HookSetup.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "HookSetup.h"
#include "afxdialogex.h"
#include "resource.h"

#include "RegEdit.h"
#include <vector>


// HookSetup �Ի���

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


// HookSetup ��Ϣ�������


void HookSetup::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	//

	CDialogEx::OnCancel();
}


void HookSetup::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	if ( m_AddSysReg.GetCheck() == BST_CHECKED)
	{
		/**
		*  ע�� ע��� ��ʽ
		**/
		AddSysReg();

	}
	else
	{
		/**
		* ���� �����ű� ��ʽ
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

		AfxMessageBox(L"ָ�������ļ����ɿ�...");
		return;
	}
	else
	{
		CRegEdit::DelScriptsStartup0();
		CRegEdit::SetAppInitDLLs(x86dllfile, x64dllfile);

		::MessageBox(NULL, x86dllfile + L"\n"+ x64dllfile, L"DLL HOOK ����", MB_OK | MB_ICONASTERISK);

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

		AfxMessageBox(L"ָ�������ļ����ɿ�...");
		return;
	}
	else
	{
		CRegEdit::DelAppInitDLLs();
		
		CRegEdit::SetScriptsLogon(x86exelfile, 1);
		CRegEdit::SetScriptsLogon(x64exelfile, 2);

		::MessageBox( NULL, x86exelfile + L"\n" + x64exelfile,L"�����ű�����", MB_OK | MB_ICONASTERISK  );

		CDialogEx::OnOK();
	}
	

}

void HookSetup::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void HookSetup::OnBnClickedRadio1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


BOOL HookSetup::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

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
				  // �쳣: OCX ����ҳӦ���� FALSE
}


void HookSetup::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

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
