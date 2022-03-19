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
    BYTE bType;             // ���ݵ�����
    BYTE bClass;            // �������͵Ĳ���
    char szValue[0x200];  // ���ݵ���Ϣ
}DATA_MSG, *PDATA_MSG;

#define UM_SERVER   (WM_USER + 200)


#define HELPMSG "������Ϣ: \r\n" \
                "\t help       : ��ʾ�����˵� \r\n" \
                "\t getsysinfo : ��öԷ�������Ϣ\r\n" \
                "\t open       : �򿪹��� \r\n" \
                "\t close      : �رչ��� \r\n" \
                "\t swap       : ����������Ҽ� \r\n" \
                "\t restore    : �ָ�������Ҽ�" \

#define NAME_LEN 20

typedef struct _SYS_INFO
{
    OSVERSIONINFO OsVer;            // �������ϵͳ��Ϣ
    char szComputerName[NAME_LEN];   // ����������
    char szUserName[NAME_LEN];       // ���浱ǰ��¼��
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
	SOCKET m_ClientSock;    // ������ͻ���ͨ�ŵ��׽���
	SOCKET m_ListenSock;    // ���ڼ������׽���

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
