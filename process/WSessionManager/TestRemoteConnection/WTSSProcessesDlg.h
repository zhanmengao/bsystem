#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// WTSSProcessesDlg 对话框

class WTSSProcessesDlg : public CDialogEx
{
	DECLARE_DYNAMIC(WTSSProcessesDlg)

public:
	WTSSProcessesDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~WTSSProcessesDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_WTSSProcesses_DIALOG};
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult);

	DWORD GetSessionId();
	void  SetSessionId( DWORD sessionID  );

private:

	DWORD SessionId;
	int nRow = 0;

	int  ActivateItem;


public:
	virtual BOOL OnInitDialog();
	virtual void PreInitDialog();
	virtual INT_PTR DoModal();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	BOOL GetWTSProcesses(DWORD SessionId);
	CListCtrl m_ProcessesList;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DWORD GetProceeIDfromParentID(DWORD dwParentProcessId);
	
	afx_msg void OnNMRClickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnRmenu115();
	afx_msg void OnRmenu116();
	afx_msg void OnRmenu117();
};
