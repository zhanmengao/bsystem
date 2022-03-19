
// SQLInjectToolsDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include <afxinet.h>

// CSQLInjectToolsDlg �Ի���
class CSQLInjectToolsDlg : public CDialogEx
{
// ����
public:
	CSQLInjectToolsDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_SQLINJECTTOOLS_DIALOG };

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
    CListCtrl m_ScanList;
    afx_msg void OnBnClickedButton1();
    void HttpGet(char* strGetPacket, char* strUrl, char* strHost);
    afx_msg void OnClose();
    void CheckInject(CString strServer, CString strObject, INTERNET_PORT nPort);
    CString m_strSign;
    BOOL Check(CString strServer, CString strObject, CString str11, CString str12);
    SOCKET m_sock;
    afx_msg void OnBnClickedRadio1();
    int m_nSel;
    afx_msg void OnBnClickedRadio2();
    afx_msg void OnBnClickedRadio3();
    afx_msg void OnBnClickedButton2();
    afx_msg void OnBnClickedButton3();
    afx_msg void OnBnClickedButton4();
    afx_msg void OnBnClickedButton5();
};