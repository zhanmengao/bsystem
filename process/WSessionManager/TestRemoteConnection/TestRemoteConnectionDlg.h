
// TestRemoteConnectionDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"

typedef struct _RDPLoginUserInfo
{

	DWORD SessionId;             // session id

	WCHAR  DeviceId[MAX_PATH + 1];

	WCHAR  WTSInitialProgram[MAX_PATH + 1]; //初始应用程序名
	WCHAR  WTSApplicationName[MAX_PATH + 1]; //应用名称
	WCHAR  WTSWorkingDirectory[MAX_PATH + 1]; //工作目录名（发起连接的程序）

	WCHAR  WTSUserName[MAX_PATH + 1];  //用户名
	WCHAR  WTSWinStationName[MAX_PATH + 1]; //连接名
	WCHAR  WTSDomainName[MAX_PATH + 1];  //域名

	WCHAR  WTSClientName[MAX_PATH + 1];  //客户机器名
	WCHAR  WTSClientAddress[MAX_PATH + 1]; //客户机IP 地址
	WCHAR  WTSClientDirectory[MAX_PATH + 1]; //


	ULONG ClientBuildNumber;
	ULONG ClientHardwareId;
	USHORT ClientProductId;

	BYTE EncryptionLevel;       // 加密PD的安全级别

	USHORT HRes;
	USHORT VRes;
	USHORT ColorDepth;

	void SClear()
	{
		SessionId = 0;

		EncryptionLevel = 0;

		ClientBuildNumber = 0;
		ClientHardwareId = 0;
		ClientProductId = 0;

		HRes = 0;
		VRes = 0;
		ColorDepth = 0;

		ZeroMemory(WTSInitialProgram, MAX_PATH + 1);
		ZeroMemory(WTSApplicationName, MAX_PATH + 1);
		ZeroMemory(WTSWorkingDirectory, MAX_PATH + 1);

		ZeroMemory(WTSUserName, MAX_PATH + 1);
		ZeroMemory(WTSWinStationName, MAX_PATH + 1);
		ZeroMemory(WTSDomainName, MAX_PATH + 1);

		ZeroMemory(WTSClientName, MAX_PATH + 1);
		ZeroMemory(WTSClientAddress, MAX_PATH + 1);
		ZeroMemory(WTSClientDirectory, MAX_PATH + 1);

		ZeroMemory(DeviceId, MAX_PATH + 1);
	}

}RDPLoginUserInfo, *PRDPLoginUserInfo;


// CTestRemoteConnectionDlg 对话框
class CTestRemoteConnectionDlg : public CDialogEx
{
// 构造
public:
	CTestRemoteConnectionDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TESTREMOTECONNECTION_DIALOG };
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
	afx_msg void OnBnClickedOk();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

private:
	void GetRDPUserLoginMessage(WPARAM wParam, LPARAM  lParam);
	RDPLoginUserInfo GetLoginUserInfo(DWORD SessionId);


private:
	CListCtrl m_list;
	int nRow = 0;

	int  ActivateItem;

public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnRclickList1(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg void OnRmenu112();
	afx_msg void OnRmenu111();
	afx_msg void OnRmenu113();
	BOOL GetWTSSessions();
	bool RunSessionNewProcesses(DWORD sessionID, CString exefilePath);
	int SendMsg2RDPS(const char * sendtxt);
	afx_msg void OnRmenu114();
	afx_msg void OnRmenu118();
	afx_msg void OnRmenu32785();
};
