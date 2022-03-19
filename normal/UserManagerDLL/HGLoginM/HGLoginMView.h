
// HGLoginMView.h : CHGLoginMView ��Ľӿ�
//

#pragma once
#include "UserCreateDlg.h"
#include "UserInfoDlg.h"
#include "WinUser.h"
#include "HGLoginMDoc.h"
#include "GCDirFile.h"
#include "NetFileShare.h"

#ifndef H_NETMESSAGE
#include "netmessage.h"
#endif


//#include "RwINIfile.h"


class CHGLoginMView : public CListView
{
protected: // �������л�����
	CHGLoginMView();
	DECLARE_DYNCREATE(CHGLoginMView)

// ����
public:
	CHGLoginMDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // ������һ�ε���

// ʵ��
public:
	virtual ~CHGLoginMView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()

	void CreateListView();

	CImageList m_imagelist;
	CUserCreateDlg MCUdlg;
	CUserInfoDlg UserInfoEdit;
	CMap< UINT,UINT ,CString,CString& >TestMenuTable;

	CString  SaveMHDir;
	CString  SaveSIP;


public:

	CString GetSaveMHDir();
	void    SetSaveMHDir( CString tStr );

	CString GetSaveSIP();
	void SetSaveSIP(CString tStr);

	BOOL BCUserList( CString usernameStr );
	void InitUserInfo();
	CWinUser  UserCon;
	CString zUserName;
	GCDirFile FileDirCon;
	NetFileShare zShare; 


	void PopMenuEnable( CMenu* vReigheMenu );
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual BOOL IsSelected(const CObject* pDocItem) const;
	afx_msg void OnGetUserList();
	afx_msg void OnAddUser();
	afx_msg void OnDelUser();
	afx_msg void OnTest();
	afx_msg void OnMCreateUser();
	afx_msg void OnUserInfo();
	afx_msg void OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnBegindrag(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnRemoveUserToGroup();
	afx_msg void OnSelectAllItem();
	afx_msg void OnNotSelectItem();


	afx_msg void OnCreateUDir();
	afx_msg void OnSetupDlg();
	afx_msg void OnDelUserDir();
	afx_msg void OnOpenUserDir();
	afx_msg void OnShareSetup();
	afx_msg void OnShareClear();

	void TmpTest( UINT tt );
	void AddUserToGroup( CString GroupName );
	void DelUserToGroup( CString GroupName );	
	void GetUserInfo( int iItem );
	//BOOL CheckUHDir( CString UserName );
	void EditLiveItemText( int Item_s ,int SubItem_i,CString outText );

	void ConUserList( DWORD con_i );
	
	afx_msg void OnCheckHDirZ();
	void GetRegKeyVStr();
	afx_msg void OnXFUserHDir();
	//BOOL XFUHDir( CString UserName );
	BOOL XFUHDir( CString UserName ,CString& UHDirStr );
	CString CheckHDir( CString UserName,CString UserHDirStr );

	void SaveUserInfo2ini( CString HKeyN ,CString ZKname ,CString vKStr );
	CString ReadUserInfo2ini(  CString HKeyN ,CString ZKname );

	//CRwINIfile* tRWini;

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnGNUserHDir();
	void MsgCommand(EUInfoStruct tt, CString ClientAddr, int i);
	afx_msg void OnSetNewPassword();
};
extern CHGLoginMView* GetMListView;

#ifndef _DEBUG  // HGLoginMView.cpp �еĵ��԰汾
inline CHGLoginMDoc* CHGLoginMView::GetDocument() const
   { return reinterpret_cast<CHGLoginMDoc*>(m_pDocument); }
#endif

