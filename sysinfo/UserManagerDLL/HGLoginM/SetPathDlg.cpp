// SetPathDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "HGLoginM.h"
#include "SetPathDlg.h"
#include "afxdialogex.h"
#include "GCDirFile.h"
#include "HGLoginMView.h"


// SetPathDlg 对话框

IMPLEMENT_DYNAMIC(SetPathDlg, CDialogEx)

SetPathDlg::SetPathDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(SetPathDlg::IDD, pParent)
{

}

SetPathDlg::~SetPathDlg()
{
}

void SetPathDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, PathStrEdit);
	DDX_Control(pDX, IDC_IPADDRESS1, sIPEdit);
}


BEGIN_MESSAGE_MAP(SetPathDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &SetPathDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDOK, &SetPathDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &SetPathDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// SetPathDlg 消息处理程序

CString SetPathDlg::BrowseOutDir()
{
	// TODO: 在此添加控件通知处理程序代码
	WCHAR szPath[MAX_PATH]; //存放选择的目录路径 
	memset(szPath, 0, sizeof(szPath));
	BROWSEINFO bi;
	bi.hwndOwner = m_hWnd;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = (LPWSTR)szPath;
	bi.lpszTitle = _T("指定用户主文件夹路径：");
	bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;
	bi.lpfn = NULL;
	bi.lParam = 0;
	bi.iImage = 0;
	//弹出选择目录对话框
	LPITEMIDLIST lp = SHBrowseForFolder(&bi);
	if( lp && SHGetPathFromIDList(lp, (LPWSTR)szPath) )
	{
		CString strOutDir;
		strOutDir.Format(_T("%s"), szPath);
		//AfxMessageBox(strOutDir);
		return strOutDir;
	}
	else
	{
		return _T("");
	}
}


void SetPathDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码

	CString tmpStr = BrowseOutDir();

	PathStrEdit.SetWindowText(  tmpStr   );

	return;
}


void SetPathDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码

	CRegKey tKey;
//	WCHAR SaveM[MAX_PATH];
//	ULONG   len;

	//GCDirFile tmpF;
	CString   tPahtEditStr;
	PathStrEdit.GetWindowText(  tPahtEditStr );

	//tPahtEditStr = tmpF.RemovePathBackslash( tPahtEditStr );

	//if ( ! tmpF.DirectoryExist( tPahtEditStr ) )
	//{
	//	AfxMessageBox( _T("路径填写有误，指定的保存路径不存在")  );
	//	return;
	//}
	CString sipStr;
	sIPEdit.GetWindowText(sipStr);
	
	if ( tKey.Open( HKEY_CURRENT_USER,_T("Software\\GWinUserManage") ) == ERROR_SUCCESS )
	{
		tKey.SetStringValue( _T("SaveMHDir"), tPahtEditStr );
		tKey.SetStringValue(_T("SaveSIP"), sipStr );
	}

	tKey.Close();

	GetMListView->SetSaveMHDir( tPahtEditStr );
	GetMListView->SetSaveSIP(sipStr);

	CDialogEx::OnOK();
}


void SetPathDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}


BOOL SetPathDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	PathStrEdit.SetWindowText( GetMListView->GetSaveMHDir() );
	sIPEdit.SetWindowText(GetMListView->GetSaveSIP());

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
