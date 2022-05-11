// UserInfoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "HGLoginM.h"
#include "UserInfoDlg.h"
#include "afxdialogex.h"
#include "HGLoginMView.h"


// CUserInfoDlg 对话框

IMPLEMENT_DYNAMIC(CUserInfoDlg, CDialogEx)

CUserInfoDlg::CUserInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CUserInfoDlg::IDD, pParent)
{

}

CUserInfoDlg::~CUserInfoDlg()
{
}

void CUserInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, UserNameEdit);
	DDX_Control(pDX, IDC_LIST1, GroupListBox1);
}


BEGIN_MESSAGE_MAP(CUserInfoDlg, CDialogEx)
	ON_BN_CLICKED(IDCANCEL, &CUserInfoDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CUserInfoDlg::OnBnClickedOk)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_BUTTON1, &CUserInfoDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CUserInfoDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CUserInfoDlg 消息处理程序


void CUserInfoDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}


void CUserInfoDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}

void CUserInfoDlg::SetFindUserName( CString uStr ,int mSel_i )
{

	this->UserName = uStr;
	this->mList_Sel_i = mSel_i;

}

void CUserInfoDlg::TCUserInfo( CString UserName  )
{



}

void CUserInfoDlg::RSUserDlgShow(  int mlist_i )
{

	CString tmpStr = GetMListView->GetListCtrl().GetItemText( mlist_i ,0 );

	UserNameEdit.EnableWindow( FALSE );
	UserNameEdit.SetWindowText( tmpStr );


	GroupListBox1.ResetContent();

	CStringList groupList;
	GetMListView->UserCon.GetUserToGroups( tmpStr, &groupList );

	POSITION P = groupList.GetHeadPosition();
	while( P )
	{
		GroupListBox1.AddString( groupList.GetNext(P) );
	}


}

void CUserInfoDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: 在此处添加消息处理程序代码
	UserNameEdit.EnableWindow(FALSE);
	UserNameEdit.SetWindowText( UserName );

	RSUserDlgShow( mList_Sel_i );
	TCUserInfo( UserName );
}


void CUserInfoDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码

	if (  this->mList_Sel_i < GetMListView->GetListCtrl().GetItemCount()-1 )
	{
		GetMListView->GetListCtrl().SetItemState( this->mList_Sel_i , 0, LVIS_SELECTED|LVIS_FOCUSED);
		GetMListView->GetListCtrl().SetItemState( this->mList_Sel_i + 1, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);

		RSUserDlgShow(  this->mList_Sel_i + 1 );

		this->mList_Sel_i ++;

		
	}	
	
}


void CUserInfoDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	if (  this->mList_Sel_i > 0 )
	{
		GetMListView->GetListCtrl().SetItemState( this->mList_Sel_i , 0, LVIS_SELECTED|LVIS_FOCUSED);
		GetMListView->GetListCtrl().SetItemState( this->mList_Sel_i - 1, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);

		RSUserDlgShow(  this->mList_Sel_i  -1 );

		this->mList_Sel_i --;		
	}

}
