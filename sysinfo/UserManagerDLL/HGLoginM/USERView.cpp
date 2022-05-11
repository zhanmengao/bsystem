
#include "stdafx.h"
#include "mainfrm.h"
#include "USERView.h"
#include "Resource.h"
#include "HGLoginM.h"
#include "WinUser.h"
#include "HGLoginMView.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// USERView

USERView::USERView()
{
}

USERView::~USERView()
{
}

BEGIN_MESSAGE_MAP( USERView, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_PROPERTIES, OnProperties)
	ON_COMMAND(ID_OPEN, OnFileOpen)
	ON_COMMAND(ID_OPEN_WITH, OnFileOpenWith)
	ON_COMMAND(ID_DUMMY_COMPILE, OnDummyCompile)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_CLEAR, OnEditClear)
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWorkspaceBar 消息处理程序

int USERView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// 创建视图:
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS;

	if (!m_UserGroupView.Create(dwViewStyle, rectDummy, this, 4))
	{
		TRACE0("未能创建文件视图\n");
		return -1;      // 未能创建
	}

	// 加载视图图像:
	//m_FileViewImages.Create(IDB_FILE_VIEW, 16, 0, RGB(255, 0, 255));

	m_UsersViewImages.Create(16,16,ILC_COLOR32|ILC_MASK,1,0);
	m_UsersViewImages.Add( AfxGetApp()->LoadIcon( IDI_ICON1 ) );
	m_UsersViewImages.Add( AfxGetApp()->LoadIcon( IDI_ICON2 ) );
	m_UsersViewImages.Add( AfxGetApp()->LoadIcon( IDI_ICON3 ) );

	m_UserGroupView.SetImageList(&m_UsersViewImages, LVSIL_SMALL);

	
	
	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_EXPLORER);
	m_wndToolBar.LoadToolBar(IDR_EXPLORER, 0, 0, TRUE );

	OnChangeVisualStyle();

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);

	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));

	m_wndToolBar.SetOwner(this);
	
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);
	


	// 填入一些静态树视图数据(此处只需填入虚拟代码，而不是复杂的数据)

	SelGroupName = _T("Error:RootTree");

	FillGroupView();
	//AdjustLayout();

	return 0;
}

void USERView::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void USERView::FillGroupView()
{

	CWinUser test;
	test.GetGroupList( (CTreeCtrl*)&m_UserGroupView );

	return;
}

void USERView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	/**
	CTreeCtrl* pWndTree = (CTreeCtrl*) &m_UserGroupView;
	ASSERT_VALID(pWndTree);

	if (pWnd != pWndTree)
	{
		CDockablePane::OnContextMenu(pWnd, point);
		return;
	}

	if (point != CPoint(-1, -1))
	{
		// 选择已单击的项:
		CPoint ptTree = point;
		pWndTree->ScreenToClient(&ptTree);

		UINT flags = 0;
		HTREEITEM hTreeItem = pWndTree->HitTest(ptTree, &flags);
		if (hTreeItem != NULL)
		{
			pWndTree->SelectItem(hTreeItem);			
		}
	}

	pWndTree->SetFocus();
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EXPLORER, point.x, point.y, this, TRUE);

	**/

}

void USERView::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;
	m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);

	m_UserGroupView.SetWindowPos(NULL, rectClient.left + 1, rectClient.top +  1, rectClient.Width() - 2, rectClient.Height() -  2, SWP_NOACTIVATE | SWP_NOZORDER);
}


CString USERView::GetSelectGroup()
{
	return SelGroupName;
}

void USERView::SetSelectGroup( CString groupStr )
{
	SelGroupName = groupStr;
}

void USERView::SelectTreeN(  HTREEITEM item_i )
{
	CString getStr;


	if ( m_UserGroupView.GetParentItem( item_i ) == NULL )
	{
		//getStr =  _T("Error:RootTree");
		return;
	}


	if ( m_UserGroupView.GetParentItem( m_UserGroupView.GetParentItem( item_i ) ) == NULL )
	{
		getStr =  _T("Error:RootTree");
	}
	else
	{
		getStr =  m_UserGroupView.GetItemText( item_i );
	}

	GetMListView->GetListCtrl().DeleteAllItems();
	GetMListView->UserCon.GetUserList( GetMListView ,getStr );

	SetSelectGroup( getStr );
	//AfxMessageBox( GetSelectGroup(item_i) );
}

void USERView::OnProperties()
{

	HTREEITEM hItem = m_UserGroupView.GetSelectedItem();

	if ((hItem != NULL) /*&& m_UserGroupView.ItemHasChildren(hItem)*/ )
	{
		//m_UserGroupView.Expand(hItem, TVE_EXPAND);
		//m_UserGroupView.EnsureVisible(hItem);
		CString getStr =  m_UserGroupView.GetItemText( hItem );
		AfxMessageBox( getStr );
	}
}

void USERView::OnFileOpen()
{
	// TODO: 在此处添加命令处理程序代码
}

void USERView::OnFileOpenWith()
{
	// TODO: 在此处添加命令处理程序代码
}

void USERView::OnDummyCompile()
{
	// TODO: 在此处添加命令处理程序代码
}

void USERView::OnEditCut()
{
	// TODO: 在此处添加命令处理程序代码
}

void USERView::OnEditCopy()
{
	// TODO: 在此处添加命令处理程序代码
}

void USERView::OnEditClear()
{
	// TODO: 在此处添加命令处理程序代码
}

void USERView::OnPaint()
{
	CPaintDC dc(this); // 用于绘制的设备上下文

	CRect rectTree;
	m_UserGroupView.GetWindowRect(rectTree);
	ScreenToClient(rectTree);

	rectTree.InflateRect(1, 1);
	dc.Draw3dRect(rectTree, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DSHADOW));
}

void USERView::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);

	m_UserGroupView.SetFocus();
}

void USERView::OnChangeVisualStyle()
{
	m_wndToolBar.CleanUpLockedImages();
	m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_EXPLORER_24 : IDR_EXPLORER, 0, 0, TRUE /* 锁定*/);

	m_UsersViewImages.DeleteImageList();

	/*
	UINT uiBmpId = theApp.m_bHiColorIcons ? IDB_FILE_VIEW_24 : IDB_FILE_VIEW;

	CBitmap bmp;
	if (!bmp.LoadBitmap(uiBmpId))
	{
		TRACE(_T("无法加载位图: %x\n"), uiBmpId);
		ASSERT(FALSE);
		return;
	}
	*/
	//BITMAP bmpObj;
	//bmp.GetBitmap(&bmpObj);

	//UINT nFlags = ILC_MASK;

	//nFlags |= (theApp.m_bHiColorIcons) ? ILC_COLOR24 : ILC_COLOR4;

	//m_FileViewImages.Create(16, bmpObj.bmHeight, nFlags, 0, 0);
	//m_FileViewImages.Add(&bmp, RGB(255, 0, 255));

	m_UsersViewImages.Create(16,16,ILC_COLOR32|ILC_MASK,1,0);
	m_UsersViewImages.Add( AfxGetApp()->LoadIcon( IDI_ICON1 ) );
	m_UsersViewImages.Add( AfxGetApp()->LoadIcon( IDI_ICON2 ) );
	m_UsersViewImages.Add( AfxGetApp()->LoadIcon( IDI_ICON3 ) );

	m_UserGroupView.SetImageList(&m_UsersViewImages, TVSIL_NORMAL);
}

void USERView::SelectTreeGroupName( CString GroupName  )
{
	HTREEITEM hRoot = m_UserGroupView.GetRootItem();

	HTREEITEM LocalGroupItem = m_UserGroupView.GetNextItem( hRoot, TVGN_CHILD );

	HTREEITEM GroupItem = m_UserGroupView.GetNextItem( LocalGroupItem , TVGN_CHILD);

	while( GroupItem )
	{
		if( m_UserGroupView.GetItemText( GroupItem ) ==  GroupName )
		{
			//AfxMessageBox( GroupName );
			m_UserGroupView.SelectItem( hRoot );
			m_UserGroupView.SelectItem( GroupItem );
			break;
		}

		GroupItem = m_UserGroupView.GetNextItem( GroupItem , TVGN_NEXT);
	}
	  
}


