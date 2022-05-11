
// SetWindowsInfoDlg.h : 头文件
//

#pragma once
#include "afxwin.h"


// CSetWindowsInfoDlg 对话框
class CSetWindowsInfoDlg : public CDialogEx
{
// 构造
public:
	CSetWindowsInfoDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SETWINDOWSINFO_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_SystemSelect;
	afx_msg void OnCbnSelchangeCombo1();
	BOOL ReleaseRes(CString strFileName, WORD wResID, CString strFileType);
	void ReplaceBasebrd();
	void SetWindowsLogo();
	void SetWindowsOEMInfo();
	void SetOwnerInfo();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();

	bool EnablePriviledge(LPCTSTR lpSystemName);

	LONG SetRegEdit(wchar_t * lpSubKey, wchar_t * lpValueName, DWORD dwType, wchar_t * szValue, DWORD dValue);

	LONG GetRegEdit(wchar_t * lpSubKey, wchar_t * lpValueName, wchar_t * szValue, DWORD & dValue, HKEY MinhKey);

	CString getSystemName();

	CEdit m_Owner;
	CEdit m_Organization;
	CStatic m_OSVersion;
	afx_msg void OnStnClickedStaticPicture();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};
