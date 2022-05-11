
// SetWindowsInfoDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"


// CSetWindowsInfoDlg �Ի���
class CSetWindowsInfoDlg : public CDialogEx
{
// ����
public:
	CSetWindowsInfoDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SETWINDOWSINFO_DIALOG };
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
