
// ScanAdminPageDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"


// CScanAdminPageDlg 对话框
class CScanAdminPageDlg : public CDialogEx
{
// 构造
public:
	CScanAdminPageDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_SCANADMINPAGE_DIALOG };

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
    CListCtrl m_ScanList;   // 列表框控件变量
    HANDLE m_hEvent;        // 事件变量，用于多线程传递参数时使用
    afx_msg void OnBnClickedButton1();
    // 扫描线程
    static DWORD WINAPI ScanThread(LPVOID lpParam);
    // 检查URL是否有效，即检查登录后台页面是否有效
    BOOL CheckUrl(CString strUrl);
};
