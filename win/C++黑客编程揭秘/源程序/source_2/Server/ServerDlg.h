// ServerDlg.h : header file
//

#if !defined(AFX_SERVERDLG_H__05292338_D7F0_471A_8178_2D13097E51BF__INCLUDED_)
#define AFX_SERVERDLG_H__05292338_D7F0_471A_8178_2D13097E51BF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CServerDlg dialog

#define TEXTMSG     't'
#define BINARYMSG   'b'

#define SYSINFO  0x01L

typedef struct _DATA_MSG
{
    BYTE bType;             // 数据的类型
    BYTE bClass;            // 数据类型的补充
    char szValue[0x200];  // 数据的信息
}DATA_MSG, *PDATA_MSG;

#define UM_SERVER   (WM_USER + 200)


#define HELPMSG "帮助信息: \r\n" \
                "\t help       : 显示帮助菜单 \r\n" \
                "\t getsysinfo : 获得对方主机信息\r\n" \
                "\t open       : 打开光驱 \r\n" \
                "\t close      : 关闭光驱 \r\n" \
                "\t swap       : 交换鼠标左右键 \r\n" \
                "\t restore    : 恢复鼠标左右键" \

#define NAME_LEN 20

typedef struct _SYS_INFO
{
    OSVERSIONINFO OsVer;            // 保存操作系统信息
    char szComputerName[NAME_LEN];   // 保存计算机名
    char szUserName[NAME_LEN];       // 保存当前登录名
}SYS_INFO, *PSYS_INFO;

class CServerDlg : public CDialog
{
// Construction
public:
	VOID SetMouseButton(BOOL bSwap);
	VOID SetCdaudio(BOOL bOpen);
	VOID GetSysInfo(PSYS_INFO SysInfo);
	VOID DispatchMsg(char *szBuf);
	VOID InsertMsg();
	CString m_StrMsg;
	SOCKET m_ClientSock;    // 用于与客户端通信的套接字
	SOCKET m_ListenSock;    // 用于监听的套接字

	CServerDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CServerDlg)
	enum { IDD = IDD_SERVER_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CServerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CServerDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    afx_msg VOID OnSock(WPARAM wParam, LPARAM lParam);
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SERVERDLG_H__05292338_D7F0_471A_8178_2D13097E51BF__INCLUDED_)
