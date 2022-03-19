// HGLoginMView.cpp : CHGLoginMView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "HGLoginM.h"
#endif

#include "HGLoginMDoc.h"
#include "HGLoginMView.h"
#include "MainFrm.h"
#include <lmerr.h>
#include <afxcoll.h>
#include <lmaccess.h>
#include "SetPathDlg.h"
#include "NewPasswordDlg.h"


//#include "NetFileShare.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

///////////////////////////////////////////////////////////////////
#define  SHARE_SET     22001 
#define  SHARE_DEL     22002

#define  UHDIR_CRETE   22011
#define  UHDIR_CLEAR   22012
#define  UHDIR_OPEN    22013
#define  UHDIR_CHECK   22014
#define  UHDIR_XFGX    20015
#define  UHDIR_GNDIR   20016

////////////////////////////////////////////////////////////////////


// CHGLoginMView

IMPLEMENT_DYNCREATE(CHGLoginMView, CListView)

BEGIN_MESSAGE_MAP(CHGLoginMView, CListView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_32771, &CHGLoginMView::OnGetUserList)
	ON_COMMAND(ID_32772, &CHGLoginMView::OnAddUser)
	ON_COMMAND(ID_32773, &CHGLoginMView::OnDelUser)
	ON_COMMAND(ID_32774, &CHGLoginMView::OnTest)
	ON_COMMAND(ID_32775, &CHGLoginMView::OnMCreateUser)
	ON_COMMAND(ID_32776, &CHGLoginMView::OnUserInfo)



	ON_NOTIFY_REFLECT(NM_DBLCLK, &CHGLoginMView::OnNMDblclk)
	ON_NOTIFY_REFLECT(LVN_BEGINDRAG, &CHGLoginMView::OnLvnBegindrag)
//	ON_NOTIFY_REFLECT(NM_KILLFOCUS, &CHGLoginMView::OnNMKillfocus)
	ON_WM_KILLFOCUS()
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_32790, &CHGLoginMView::OnRemoveUserToGroup)
	ON_COMMAND(ID_32792, &CHGLoginMView::OnSelectAllItem)
	ON_COMMAND(ID_32791, &CHGLoginMView::OnNotSelectItem)

	ON_COMMAND(ID_32793, &CHGLoginMView::OnCreateUDir)
	ON_COMMAND(ID_32794, &CHGLoginMView::OnSetupDlg)
	ON_COMMAND(ID_32796, &CHGLoginMView::OnDelUserDir)
	ON_COMMAND(ID_32795, &CHGLoginMView::OnOpenUserDir)
	ON_COMMAND(ID_32798, &CHGLoginMView::OnShareSetup)
	ON_COMMAND(ID_32799, &CHGLoginMView::OnShareClear)
	ON_COMMAND(ID_32801, &CHGLoginMView::OnCheckHDirZ)
	ON_COMMAND(ID_32802, &CHGLoginMView::OnXFUserHDir)
	ON_COMMAND(ID_32803, &CHGLoginMView::OnGNUserHDir)
	ON_COMMAND(ID_32804, &CHGLoginMView::OnSetNewPassword)
END_MESSAGE_MAP()

// CHGLoginMView 构造/析构
CHGLoginMView* GetMListView;
CHGLoginMView::CHGLoginMView()
{
	// TODO: 在此处添加构造代码
	GetMListView = this;
}

CHGLoginMView::~CHGLoginMView()
{
}

void CHGLoginMView::GetRegKeyVStr()
{
	ULONG   len1,len2;
	WCHAR SaveKey1[MAX_PATH];
	WCHAR SaveKey2[MAX_PATH];
	CRegKey tKey;

	tKey.Create( HKEY_CURRENT_USER ,_T("Software\\GWinUserManage")  );

	if(  tKey.QueryStringValue( _T("SaveMHDir"),SaveKey1 ,&len1 ) == ERROR_SUCCESS )
	{
		this->SaveMHDir = (CString) SaveKey1;
	}
	else
	{
		if( tKey.SetStringValue( _T("SaveMHDir"),_T("D:\\") ) == ERROR_SUCCESS )

			this->SaveMHDir  = _T("D:\\");
	}

	if (tKey.QueryStringValue(_T("SaveSIP"), SaveKey2, &len2) == ERROR_SUCCESS)
	{
		this->SaveSIP = (CString)SaveKey2;
	}
	else
	{
		if (tKey.SetStringValue(_T("SaveSIP"), _T("127.0.0.1")) == ERROR_SUCCESS)

			this->SaveSIP = _T("127.0.0.1");
	}

	tKey.Close();

}

BOOL CHGLoginMView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CListView::PreCreateWindow(cs);
}

void CHGLoginMView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();


	// TODO: 调用 GetListCtrl() 直接访问 ListView 的列表控件，
	//  从而可以用项填充 ListView。

	InitUserInfo();

	//TCHAR exeFullPath[MAX_PATH]; // MAX_PATH
	//GetModuleFileName(NULL,exeFullPath,MAX_PATH);//得到程序模块名称，全路径

	//CString Sfile;
	//Sfile.Format(_T("%s\\%s"), FileDirCon.GetFilePathStr( exeFullPath  ),_T("UserInfo.ini"));
	//tRWini = new CRwINIfile( Sfile );
	
	CreateListView();
	GetRegKeyVStr();


}

void CHGLoginMView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	

	CListView::OnRButtonDown(nFlags, point);
}

void CHGLoginMView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}


void CHGLoginMView::PopMenuEnable( CMenu* vReigheMenu )
{

	CString tmpStr;


	if( GetListCtrl().GetItemCount() == 0 )
	{
		vReigheMenu->GetMenuString( ID_32791, tmpStr , MF_BYCOMMAND ); //全选
		vReigheMenu->ModifyMenu( ID_32791,MF_BYCOMMAND|MF_DISABLED, ID_32791, tmpStr );

		vReigheMenu->GetMenuString( ID_32792, tmpStr , MF_BYCOMMAND ); //反选
		vReigheMenu->ModifyMenu( ID_32792,MF_BYCOMMAND|MF_DISABLED, ID_32792,tmpStr );

	}
	else
	{
		vReigheMenu->GetMenuString( ID_32791, tmpStr , MF_BYCOMMAND ); //全选
		vReigheMenu->ModifyMenu( ID_32791,MF_BYCOMMAND|MF_ENABLED, ID_32791, tmpStr );

		vReigheMenu->GetMenuString( ID_32792, tmpStr , MF_BYCOMMAND ); //反选
		vReigheMenu->ModifyMenu( ID_32792,MF_BYCOMMAND|MF_ENABLED, ID_32792,tmpStr );
	}

/*
	vReigheMenu->GetMenuString( ID_32771, tmpStr , MF_BYCOMMAND ); //读取用户
	vReigheMenu->ModifyMenu( ID_32771,MF_BYCOMMAND|MF_DISABLED, ID_32771,tmpStr );

	vReigheMenu->GetMenuString( ID_32772, tmpStr , MF_BYCOMMAND );//添加用户
	vReigheMenu->ModifyMenu( ID_32772,MF_BYCOMMAND|MF_DISABLED, ID_32772,tmpStr );
*/

	if ( GetListCtrl().GetSelectedCount() == 0 )
	{

		vReigheMenu->GetMenuString( ID_32773, tmpStr , MF_BYCOMMAND );
		vReigheMenu->ModifyMenu( ID_32773,MF_BYCOMMAND|MF_DISABLED, ID_32773,tmpStr );

		vReigheMenu->GetMenuString( ID_32774, tmpStr , MF_BYCOMMAND );
		vReigheMenu->ModifyMenu( ID_32774,MF_BYCOMMAND|MF_DISABLED, ID_32774,tmpStr );

		vReigheMenu->GetMenuString( ID_32776, tmpStr , MF_BYCOMMAND );
		vReigheMenu->ModifyMenu( ID_32776,MF_BYCOMMAND|MF_DISABLED, ID_32776,tmpStr );

		vReigheMenu->GetMenuString( ID_32790, tmpStr , MF_BYCOMMAND );
		vReigheMenu->ModifyMenu( ID_32790,MF_BYCOMMAND|MF_DISABLED, ID_32790,tmpStr );
	}
	else
	{
		vReigheMenu->GetMenuString( ID_32772, tmpStr , MF_BYCOMMAND );
		vReigheMenu->ModifyMenu( ID_32772,MF_BYCOMMAND|MF_ENABLED, ID_32772,tmpStr );

		vReigheMenu->GetMenuString( ID_32773, tmpStr , MF_BYCOMMAND );
		vReigheMenu->ModifyMenu( ID_32773,MF_BYCOMMAND|MF_ENABLED, ID_32773,tmpStr );

		vReigheMenu->GetMenuString( ID_32774, tmpStr , MF_BYCOMMAND );
		vReigheMenu->ModifyMenu( ID_32774,MF_BYCOMMAND|MF_ENABLED, ID_32774,tmpStr );

		vReigheMenu->GetMenuString( ID_32776, tmpStr , MF_BYCOMMAND );
		vReigheMenu->ModifyMenu( ID_32776,MF_BYCOMMAND|MF_ENABLED, ID_32776,tmpStr );

		vReigheMenu->GetMenuString( ID_32790, tmpStr , MF_BYCOMMAND );
		vReigheMenu->ModifyMenu( ID_32790,MF_BYCOMMAND|MF_ENABLED, ID_32790,tmpStr );
	}

	

}

void CHGLoginMView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS

	CMenu menu;
	VERIFY(menu.LoadMenu( IDR_POPUP_EDIT ));  

	CMenu* pPopup = menu.GetSubMenu(0);
	ASSERT(pPopup != NULL);

	PopMenuEnable( pPopup );

	TestMenuTable.RemoveAll();
	

	CStringList* groupList = new CStringList;
	int Gcount = UserCon.GetGroupList(  groupList );
	POSITION P = groupList->GetHeadPosition();

	HMENU hSubMenu = CreateMenu();
	CString tmpStr;
	int i = 0;

	while( P != NULL )
	{
		tmpStr = groupList->GetNext(P);

		if ( GetListCtrl().GetSelectedCount() == 0 )
		{
			AppendMenu( hSubMenu,MF_STRING | MF_DISABLED,ID_ADDGROUPMENU + i,tmpStr );//
		}
		else
		{
           AppendMenu( hSubMenu,MF_STRING | MF_ENABLED,ID_ADDGROUPMENU + i,tmpStr );//
		}
		

		TestMenuTable.SetAt( ID_ADDGROUPMENU + i ,tmpStr);
		i++;
	}
		

	pPopup->AppendMenu(MF_POPUP,(UINT_PTR)hSubMenu,_T("添加到用户组"));

	pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y,this);
	pPopup->Detach();

	delete groupList;
    //theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}

void CHGLoginMView::TmpTest( UINT tt )
{

    CString strtext;

	GetDlgItemText(  tt, strtext);

	CString str_menustring;
	CMenu menu;     
	menu.LoadMenu( IDR_POPUP_EDIT );     
	//CMenu*  pPopup =  menu.GetSubMenu(0);     
	//ASSERT(pPopup!=NULL); 

	menu.GetSubMenu(0)->GetMenuString(tt,str_menustring,MF_BYCOMMAND );
	MessageBox(str_menustring);


}

// CHGLoginMView 诊断

#ifdef _DEBUG
void CHGLoginMView::AssertValid() const
{
	CListView::AssertValid();
}

void CHGLoginMView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}

CHGLoginMDoc* CHGLoginMView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CHGLoginMDoc)));
	return (CHGLoginMDoc*)m_pDocument;
}
#endif //_DEBUG


// CHGLoginMView 消息处理程序
void CHGLoginMView::CreateListView()
{
	m_imagelist.Create( 16,16,ILC_COLOR32|ILC_MASK,1,0 );

	m_imagelist.Add( AfxGetApp()->LoadIcon( IDI_ICON2 ) );
	//m_imagelist.Add(AfxGetApp()->LoadIcon( IDI_ICON2 ));

	//将图像列表关联到列表视图控件中
	GetListCtrl().SetImageList(&m_imagelist,LVSIL_SMALL );

	LONG lStyle;
	lStyle = GetWindowLong(GetListCtrl().m_hWnd, GWL_STYLE);//获取当前窗口style
	lStyle &= ~LVS_TYPEMASK; //清除显示方式位
	lStyle |= LVS_SHOWSELALWAYS;
	lStyle |= LVS_REPORT; //设置style
	
	SetWindowLong(GetListCtrl().m_hWnd, GWL_STYLE, lStyle);//设置style

	DWORD dwStyle = GetListCtrl().GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;//选中某行使整行高亮（只适用与report风格的listctrl）
	//dwStyle |= LVS_EX_GRIDLINES;//网格线（只适用与report风格的listctrl）
	dwStyle |= LVS_EX_CHECKBOXES;//item前生成checkbox控件
	GetListCtrl().SetExtendedStyle(dwStyle); //设置扩展风格

	GetListCtrl().InsertColumn( 0, _T(""), LVCFMT_LEFT, 100 );
	GetListCtrl().InsertColumn( 1, _T(""), LVCFMT_LEFT, 100 );
	GetListCtrl().InsertColumn( 2, _T(""), LVCFMT_LEFT, 150 );//插入列
	GetListCtrl().InsertColumn( 3, _T(""), LVCFMT_LEFT, 120 );	
	GetListCtrl().InsertColumn( 4, _T(""), LVCFMT_LEFT, 250 );
	GetListCtrl().InsertColumn( 5, _T(""), LVCFMT_LEFT, 250 );
	GetListCtrl().InsertColumn( 6, _T(""), LVCFMT_LEFT, 250 );
	GetListCtrl().InsertColumn( 7, _T(""), LVCFMT_LEFT, 250 );
	GetListCtrl().InsertColumn( 8, _T(""), LVCFMT_LEFT, 250 );
	GetListCtrl().InsertColumn( 9, _T(""), LVCFMT_LEFT, 250 );

	UserCon.GetUserList( this );
}

void CHGLoginMView::OnGetUserList()
{
	// TODO: 在此添加命令处理程序代码
	GetListCtrl().DeleteAllItems();
	UserCon.GetUserList( this);
}


void CHGLoginMView::OnAddUser()
{
   //
	OnMCreateUser();
}


void CHGLoginMView::InitUserInfo()
{

	WCHAR gUserName[20];
	ZeroMemory( gUserName,sizeof(gUserName) );
	DWORD ulen;
	GetUserName( gUserName, &ulen );
	
	this->zUserName = (CString)gUserName;


}

BOOL CHGLoginMView::BCUserList( CString usernameStr )
{


	if (  usernameStr != zUserName && 
		  usernameStr !=_T("Administrator") && 
		  usernameStr != _T("Guest") )
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}

	
}

void CHGLoginMView::OnDelUser()
{
	// TODO: 在此添加命令处理程序代码

	CString outTextStr,rStr,tmpStr;

	for( int i =0 ;i< GetListCtrl().GetItemCount();i++  )
	{

		if (  GetListCtrl().GetItemState( i, LVIS_SELECTED) == LVIS_SELECTED  )
		{
			tmpStr = GetListCtrl().GetItemText( i,0 );

		    if (  ! BCUserList(tmpStr) )
			{
				outTextStr.Format( _T("删除用户：%s %s"),tmpStr ,_T("自动忽略系统保留用户"));
				GetMainFrame->m_wndOutput.FillBuildWindow( outTextStr );
				continue;
			}

			rStr = UserCon.ReturnErrInfo(
			                       UserCon.DelSysUser(  tmpStr ));

			outTextStr.Format( _T("删除用户：%s %s"),tmpStr ,rStr);

			GetMainFrame->m_wndOutput.FillBuildWindow( outTextStr );
		}

	}	

	GetListCtrl().DeleteAllItems();
	UserCon.GetUserList( this);		

}


void CHGLoginMView::OnTest()
{
	// TODO: 在此添加命令处理程序代码

	UserStruct_I1 test;
	test.HomeDirPath = _T("F:\\1234\\USER001");
	UserCon.SetUserProfile(_T("USER001"), test);
}




void CHGLoginMView::OnMCreateUser()
{
	// TODO: 在此添加命令处理程序代码
    
	if( MCUdlg.DoModal() == IDOK )
	{
		GetListCtrl().DeleteAllItems();
		UserCon.GetUserList( this );
	}


}


void CHGLoginMView::OnUserInfo()
{
	// TODO: 在此添加命令处理程序代码

	//UserInfoEdit.DoModal();
}


BOOL CHGLoginMView::IsSelected(const CObject* pDocItem) const
{
	// TODO: 在此添加专用代码和/或调用基类
	return CListView::IsSelected(pDocItem);
}


void CHGLoginMView::GetUserInfo( int iItem )
{

	if ( iItem < 0 ) return;

	CString UserName = GetListCtrl().GetItemText(  iItem ,0 );

	UserInfoEdit.SetFindUserName( UserName ,iItem );
	UserInfoEdit.DoModal();

}

void CHGLoginMView::OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	//GetUserInfo( pNMItemActivate->iItem  );
	//MessageBox( GetListCtrl().GetItemText(  pNMItemActivate->iItem ,0 ) );

	*pResult = 0;
}


void CHGLoginMView::OnLvnBegindrag(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	*pResult = 0;
}


void CHGLoginMView::OnKillFocus(CWnd* pNewWnd)
{
	CListView::OnKillFocus(pNewWnd);
	GetListCtrl().SendMessage( WM_SETFOCUS,0,0 );
	// TODO: 在此处添加消息处理程序代码
}


void CHGLoginMView::AddUserToGroup( CString GroupName )
{

	CString NameStr;
	for(int i=0; i<GetListCtrl().GetItemCount(); i++)
	{
		if( GetListCtrl().GetItemState( i, LVIS_SELECTED) == LVIS_SELECTED )
		{
			NameStr = GetListCtrl().GetItemText( i,0);
			//AfxMessageBox(str);
			UserCon.AddUserToGroup(  GroupName , NameStr );

		}
	}
	//GetListCtrl().DeleteAllItems();	
	//UserCon.GetUserList( this ,GroupName);
	GetMainFrame->m_wndUSERView.SelectTreeGroupName( GroupName );
}

void CHGLoginMView::DelUserToGroup( CString GroupName )
{
	
	CString NameStr;
	for(int i=0; i<GetListCtrl().GetItemCount(); i++)
	{
		if( GetListCtrl().GetItemState( i, LVIS_SELECTED) == LVIS_SELECTED )
		{
			NameStr = GetListCtrl().GetItemText( i,0);
			//AfxMessageBox(str);
			UserCon.DelUserToGroup(  GroupName , NameStr );

		}
	}
	//GetListCtrl().DeleteAllItems();
	//UserCon.GetUserList( this ,GroupName);
	GetMainFrame->m_wndUSERView.SelectTreeGroupName( GroupName );
}


BOOL CHGLoginMView::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类
	
	CString tmpStr;

	int gcount = TestMenuTable.GetCount();

	if(  LOWORD( wParam) >= ID_ADDGROUPMENU  &&  LOWORD( wParam) < ID_ADDGROUPMENU + gcount )
	{

		TestMenuTable.Lookup(   LOWORD( wParam) ,tmpStr );
		//AfxMessageBox(  tmpStr );
		AddUserToGroup( tmpStr );
	}

	return CListView::OnCommand(wParam, lParam);
}


void CHGLoginMView::OnRemoveUserToGroup()
{
	// TODO: 在此添加命令处理程序代码
	CString SelGroupName = GetMainFrame->m_wndUSERView.GetSelectGroup();  

	if(  SelGroupName != _T("Error:RootTree") )
	{
		DelUserToGroup( SelGroupName );
	}
	
	
}


void CHGLoginMView::OnSelectAllItem()
{
	// TODO: 在此添加命令处理程序代码

	for( int i=0;i< GetListCtrl().GetItemCount();i++  )
	{
		 GetListCtrl().SetItemState( i, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
	}
}


void CHGLoginMView::OnNotSelectItem()
{
	// TODO: 在此添加命令处理程序代码

	for( int i=0;i< GetListCtrl().GetItemCount();i++  )
	{
        
      if(  GetListCtrl().GetItemState(i, LVIS_SELECTED) != LVIS_SELECTED   )
	  {
		GetListCtrl().SetItemState( i, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
	  }
	  else
	  {
          GetListCtrl().SetItemState(i, 0, LVIS_SELECTED|LVIS_FOCUSED);
	  }


	}

}

void CHGLoginMView::EditLiveItemText(  int Item_s ,int SubItem_i,CString outText )
{
	GetListCtrl().SetItemText( Item_s ,SubItem_i, outText);
}

void CHGLoginMView::SaveUserInfo2ini(  CString HKeyN ,CString ZKname ,CString vKStr )
{
   //tRWini->WriteString( HKeyN,ZKname ,vKStr);
}


void CHGLoginMView::OnCreateUDir()
{
	// TODO: 在此添加命令处理程序代码

	ConUserList( UHDIR_CRETE );

	return;

}


void CHGLoginMView::OnSetupDlg()
{
	// TODO: 在此添加命令处理程序代码

	SetPathDlg tDlg;
	tDlg.DoModal();
}


void CHGLoginMView::OnDelUserDir()
{
	// TODO: 在此添加命令处理程序代码

	ConUserList( UHDIR_CLEAR );
	return;

	CString UserNameStr;
	CString DirStr = GetSaveMHDir();
	for(  int i = 0 ; i< GetListCtrl().GetItemCount() ;i++  )
	{

		if( GetListCtrl().GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED )
		{
			UserNameStr = GetListCtrl().GetItemText( i ,0 );

			CString tmpStr;
			tmpStr.Format( _T("%s%s"),DirStr,UserNameStr );
			if( FileDirCon.DirectoryEmpty( tmpStr ) )
			{
				//EditLiveItemText( i,2 , _T("2222222222222") );
			}
			else
			{

			}

		}

	}
}


void CHGLoginMView::OnOpenUserDir()
{
	// TODO: 在此添加命令处理程序代码
   ConUserList( UHDIR_OPEN ); 

}


CString CHGLoginMView::CheckHDir( CString UserName,CString UserHDirStr )
{
//
  CString tt;

  if (  ! FileDirCon.DirectoryExist( UserHDirStr )  )
  {
	  tt.Format( _T("%s 文件夹不存在或关联.."),UserHDirStr );
	 // GetMainFrame->m_wndOutput.FillBuildWindow( tt);	  
  }
  else
  {
	  CString tmpStr1;
	  BOOL user2Share = FALSE;

	  if( zShare.GetSharePath( UserName ,tmpStr1 )  == ERROR_SUCCESS  )
	  {
/*--------------------------------------------------------------------*/
		  CStringList* test = new CStringList;
		  zShare.GetShareUsers( UserName,test );
		  POSITION P =  test->GetHeadPosition();
		  while(P)
		  {
			  //MessageBox( (CString)test->GetNext(P) );
			  if(  (CString)test->GetNext(P) == UserName )
				  user2Share = TRUE;
		  }
		  delete test;
/*--------------------------------------------------------------------*/

		  if( ! user2Share  || tmpStr1 != UserHDirStr   )
		  {			  
			  tt.Format( _T("共享 %s 指向的路径或预设访问权限的用户不相符,"),UserName );
			  //GetMainFrame->m_wndOutput.FillBuildWindow( tt );
		  }
		  else		  
		  {
			  tt.Format( _T("用户主目录%s 校验正确,共享 %s 正确"),UserHDirStr,UserName );
			  // GetMainFrame->m_wndOutput.FillBuildWindow( tt );
		  }

	  }
	  else
	  {
		  tt.Format( _T("用户主目录%s 未创建共享"),UserHDirStr,UserName );
	  }
  }

	return tt;
}

BOOL CHGLoginMView::XFUHDir( CString UserName ,CString& UHDirStr )
{

	//UserStruct_I1 NEditUi;
	//UserCon.GetUserInfo( UserName ,NEditUi );

	CString testHomeDir;
	testHomeDir.Format( _T("%s%s"),GetSaveMHDir(),UserName );

	if (testHomeDir == UHDirStr  && FileDirCon.DirectoryExist(testHomeDir) && zShare.CheckNetShare(UserName) == NERR_Success)
	{
		return TRUE;
	}
	else
	{

		if (FileDirCon.CreateMDirectory(UHDirStr) && zShare.SetNetShare(UHDirStr, UserName, UserName, 1) == NERR_Success)
		{

			return TRUE;
		}
		else
		{
			if( FileDirCon.DirectoryExist( UHDirStr )  )
			{
				zShare.SetNetShare( UHDirStr,UserName,UserName,1 );	
				return TRUE;
			}
			else
			{
				return FALSE;
			}			
		}

	}

}

void CHGLoginMView::ConUserList( DWORD con_i )
{
	 
	NET_API_STATUS res;

	UserStruct_I1 NEditUi;
	UserStruct_I1 test;
	CString xfNewHDirStr;
	CString outText;
	CString tmp;

	for(int i=0; i< GetListCtrl().GetItemCount(); i++)
	{
		if( GetListCtrl().GetItemState( i, LVIS_SELECTED) == LVIS_SELECTED )
		{
			NEditUi.Name = GetListCtrl().GetItemText( i,0 );
			NEditUi.HomeDirPath = GetListCtrl().GetItemText( i,2 );

			CString testHomeDir;
			testHomeDir.Format(_T("%s%s"), GetSaveMHDir(), NEditUi.Name);
			//AfxMessageBox(str);
			switch (  con_i )
			{
            case UHDIR_CHECK: //检查
				outText = CheckHDir(  NEditUi.Name,NEditUi.HomeDirPath );
				EditLiveItemText( i,3 ,outText );
				break;

			case UHDIR_XFGX:
				
				if (XFUHDir(NEditUi.Name, testHomeDir))
				{
					EditLiveItemText(i, 3, testHomeDir + _T(" 修复成功"));
					outText.Format(_T("个人主目录%s 校验正确，已共享"), testHomeDir);
				}
				else
				{
					EditLiveItemText( i,3 , NEditUi.HomeDirPath + _T(" 修复失败") );
					outText.Format( _T("个人主目录%s 修复失败"),NEditUi.HomeDirPath );
				}
				break;

            case UHDIR_CRETE:  //创建用户主目录
				NEditUi.HomeDirPath.Format( _T("%s%s"),GetSaveMHDir(),NEditUi.Name );
				if( FileDirCon.CreateMDirectory( NEditUi.HomeDirPath ) )
				{					
					outText.Format( _T("创建用户主目录 %s 成功 ") ,NEditUi.HomeDirPath );
					EditLiveItemText( i,2 , NEditUi.HomeDirPath );
					SaveUserInfo2ini( NEditUi.Name,_T("HMDir"),NEditUi.HomeDirPath );
				}
				else
				{
					if(  FileDirCon.DirectoryExist( NEditUi.HomeDirPath ) )
					{
						EditLiveItemText( i,2 , NEditUi.HomeDirPath );
						outText.Format( _T("用户主目录 %s 已存在，自动忽略 ") ,NEditUi.HomeDirPath );
					}
				}

				break;

			case UHDIR_CLEAR:  //删除目录
				if( FileDirCon.DirectoryEmpty( NEditUi.HomeDirPath ) )
				{
					outText.Format( _T("清空： %s 用户主目录 %s 完成"),NEditUi.Name , NEditUi.HomeDirPath );
				}
				else
				{
					outText.Format( _T("清空： %s 用户主目录 %s 失败"),NEditUi.Name , NEditUi.HomeDirPath );
				}
				break;

			case UHDIR_OPEN: //打开目录
				ShellExecute(NULL,NULL,NEditUi.HomeDirPath,NULL,NULL,SW_SHOW);
				break;

			case UHDIR_GNDIR:
				//
				tmp.Format(_T("\\\\%s\\%s"), SaveSIP, NEditUi.Name);
				test.HomeDirPath = tmp;
				//UserCon.SetUserProfile(NEditUi.Name, test);
				UserCon.SetUserProfile(NEditUi.Name, test);
				break;

			case SHARE_SET:  //设置共享
				res = zShare.SetNetShare(testHomeDir, NEditUi.Name, NEditUi.Name, 1);
				outText.Format( _T("设置共享： %s 用户主目录 %s"),NEditUi.Name , zShare.ReturnInfo( res ));
				break;

			case SHARE_DEL:  //取消共享
				res = zShare.RemoveNetShare( NEditUi.Name  );
				outText.Format( _T("取消共享： %s 用户主目录 %s"),NEditUi.Name , zShare.ReturnInfo( res ));
				break;

			default:
				break;
			}
			
			GetMainFrame->m_wndOutput.FillBuildWindow( outText );

		}
	}
}


void CHGLoginMView::OnShareSetup()
{
	// TODO: 在此添加命令处理程序代码

	ConUserList( SHARE_SET );
}


void CHGLoginMView::OnShareClear()
{
	// TODO: 在此添加命令处理程序代码

	ConUserList( SHARE_DEL );
}


void CHGLoginMView::OnCheckHDirZ()
{
	// TODO: 在此添加命令处理程序代码
	ConUserList( UHDIR_CHECK );
}

void CHGLoginMView::SetSaveMHDir( CString tStr )
{
	this->SaveMHDir = tStr;	
}

CString CHGLoginMView::GetSaveMHDir()
{
	return this->SaveMHDir;
}

void CHGLoginMView::SetSaveSIP(CString tStr)
{
	this->SaveSIP = tStr;
}

CString CHGLoginMView::GetSaveSIP()
{
	return this->SaveSIP;
}


void CHGLoginMView::OnXFUserHDir()
{
	// TODO: 在此添加命令处理程序代码
	ConUserList( UHDIR_XFGX );
}

CString CHGLoginMView::ReadUserInfo2ini( CString HKeyN ,CString ZKname )
{
	// CString tmpStr = tRWini->ReadString( HKeyN,ZKname ,_T("未创建个人主目录"));

	 return _T("");
	
}


BOOL CHGLoginMView::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
/*
	switch( pMsg->message )
	{
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
	}
*/
	if( pMsg->hwnd == GetSafeHwnd() && 
		pMsg->message == WM_KEYDOWN &&
		pMsg->wParam== 'A' && ( GetKeyState(VK_CONTROL)&0x8000) )
	{
		OnSelectAllItem();
	}


	if( pMsg->hwnd == GetSafeHwnd() && 
		pMsg->message == WM_KEYDOWN &&
		pMsg->wParam== 'D' && ( GetKeyState(VK_CONTROL)&0x8000) )
	{
		OnNotSelectItem();
	}


	return CListView::PreTranslateMessage(pMsg);
}


void CHGLoginMView::OnGNUserHDir()
{
	// TODO: 在此添加命令处理程序代码
	ConUserList( UHDIR_GNDIR );
}


void CHGLoginMView::MsgCommand(EUInfoStruct tt, CString ClientAddr, int i)
{

	DWORD nStatus = -1;
	if (1001 == i)
	{
		nStatus = UserCon.SetUserPassword(tt.USERNAME, tt.PASSWORD, tt.ZPASSWORD);
	}
	CString outText;

	switch (nStatus)
	{

	case NERR_Success:
		outText.Format(_T("%s 修改密码成功"), tt.USERNAME );
		break;

	case ERROR_ACCESS_DENIED:
		outText.Format(_T("%s 修改密码失败，不允许修改密码"), tt.USERNAME);
		break;
		//The user does not have access to the requested information.
	case ERROR_INVALID_PASSWORD:
		outText.Format(_T("%s 修改密码失败，原密码无效"), tt.USERNAME);
		break;
		//The user has entered an invalid password.
	case NERR_InvalidComputer:
		outText.Format(_T("%s 修改密码失败"), tt.USERNAME);
		break;
		//The computer name is invalid.
	case NERR_NotPrimary:
		outText.Format(_T("%s 修改密码失败"), tt.USERNAME);
		break;
		//The operation is allowed only on the primary domain controller of the domain.
	case NERR_UserNotFound:
		outText.Format(_T("%s 修改密码失败，指定用户不存在"), tt.USERNAME);
		break;
		//The user name could not be found.
	case NERR_PasswordTooShort:
		outText.Format(_T("%s 修改密码失败，新密码太短"), tt.USERNAME);
		break;

	default:
		break;
	}
     

	GetMainFrame->m_wndOutput.FillBuildWindow(outText);

	tt.zClear();
	tt.CODE = 2001;
	tt.RERROR = nStatus;

	GetMainFrame->m_epSocket.SendTo(  &tt,sizeof(tt), 2587,ClientAddr,0 );
	//SetUserPassword

}


void CHGLoginMView::OnSetNewPassword()
{
	// TODO:  在此添加命令处理程序代码

	NewPasswordDlg tmp;
	if (tmp.DoModal() != IDOK)
	{
		return;
	}
	AfxMessageBox( tmp.m_str );
	DWORD nStatus = -1;
	CString newpawSTR = tmp.m_str;
	CString outText;

	for (int i = 0; i < GetListCtrl().GetItemCount(); i++)
	{
		if (GetListCtrl().GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED)
		{
			CString username = GetListCtrl().GetItemText(i, 0);
			nStatus = UserCon.SetUserPassword(username, newpawSTR);
			//AfxMessageBox( username );


			if (nStatus == 0)
			{
				outText.Format(_T("修改 %s 密码成功"), username);
			}
			else
			{
				outText.Format(_T("修改 %s 密码失败"), username );
			}
			
			GetMainFrame->m_wndOutput.FillBuildWindow(outText);

		}

	}


}
