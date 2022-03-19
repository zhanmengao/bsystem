// ClientDlg.h : header file
//

#if !defined(AFX_CLIENTDLG_H__9396010A_8557_4993_9FD6_CCDC89A59227__INCLUDED_)
#define AFX_CLIENTDLG_H__9396010A_8557_4993_9FD6_CCDC89A59227__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CClientDlg dialog

#define TEXTMSG     't'
#define BINARYMSG   'b'

#define SYSTEMINFO  0x01

typedef struct _DATA_MSG
{
    BYTE bType;             // 数据的类型
    BYTE bClass;            // 数据类型的补充
    char szValue[0x200];  // 数据的信息
}DATA_MSG, *PDATA_MSG;

#define UM_CLIENT   (WM_USER + 201)

#define NAME_LEN 20

typedef struct _SYS_INFO
{
    OSVERSIONINFO OsVer;            // 保存操作系统信息
    char szComputerName[NAME_LEN];   // 保存计算机名
    char szUserName[NAME_LEN];       // 保存当前登录名
}SYS_INFO, *PSYS_INFO;

class CClientDlg : public CDialog
{
// Construction
public:
	VOID ParseSysInfo(PSYS_INFO SysInfo);
	VOID DispatchMsg(char *szBuf);
	SOCKET m_Socket;
	VOID InsertMsg();
	CString m_StrMsg;
	CClientDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CClientDlg)
	enum { IDD = IDD_CLIENT_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CClientDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CClientDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
    afx_msg VOID OnSock(WPARAM wParam, LPARAM lParam);
	afx_msg void OnClose();
	afx_msg void OnBtnConnect();
	afx_msg void OnBtnSend();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLIENTDLG_H__9396010A_8557_4993_9FD6_CCDC89A59227__INCLUDED_)
