
// SetRDPPortDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"


// CSetRDPPortDlg �Ի���
class CSetRDPPortDlg : public CDialogEx
{
// ����
public:
	CSetRDPPortDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SETRDPPORT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:

	BOOL  EditPortSetup();
	DWORD SetHideThisPCPolicy();
	DWORD SetCleanProDir();
	DWORD SetCleanLibLink();
	DWORD SetDisabledAllowList();
	DWORD SetSingleSessionPerUser();
	DWORD SetLicenseServers();
	DWORD SetNoDrives();
	DWORD SetNoViewOnDrive();

	DWORD SetUserAuthentication();
	DWORD SetMaxInstanceCount();
	DWORD SetEnableLogonScriptDelay();

	DWORD SetNoViewContextMenu();

	DWORD SetDisableCMD();

	DWORD SetExplorerWindows();

	DWORD SetWindowsHelp();

	DWORD SetIexploreFileName();

	DWORD SetUtilmanFileName();

	DWORD SetNoControlPanel();

	void SetCADKetCall();

	void SetKOneDrive();

	BOOL  EnablePriviledge();
	BOOL  MySystemShutdown();

	void  ReadCurrentSetup();

	BOOL  defaultSelect;

	WCHAR * Get_ENV(const wchar_t * hzName);

public:
	CEdit m_rdpPort;
	afx_msg void OnBnClickedOk();
	
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButton1();


	CButton m_cleanlibLink;
	CButton m_cleanproDir;
	CButton m_hidethisPCPolicy;
	CButton m_fdisabledAllowList;
	
	CButton m_fSingleSessionPerUser;
	CEdit   m_LicenseServers;
	CButton m_InputLicense;
	
	CButton m_defaultPort;
	CButton m_NoDrives;
	CButton m_NoViewOnDrive;
	CButton m_UserAuthentication;
	CButton m_MaxInstanceCount;	
	CEdit   m_MaxContionEdit;
	CButton m_EnableLogonScriptDelay;
	afx_msg void OnBnClickedButton2();
	CButton m_NoControlPanel;
	CButton m_NoViewContextMenu;
	CButton m_ExplorerWindows;
	CButton m_NoHelpPane;
	afx_msg void OnNMClickSyslink1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	BOOL ReleaseRes(CString strFileName, WORD wResID, CString strFileType);
	void R2012FilesFun();
	afx_msg void OnBnClickedButton8();
	
	CButton m_DisableCMD;
	CButton m_IE;
	CButton m_cadKetCall;
	CButton m_utilman;
	afx_msg void OnBnClickedButton9();
	afx_msg void OnBnClickedButton10();
	
	CButton m_KOneDrive;
};
