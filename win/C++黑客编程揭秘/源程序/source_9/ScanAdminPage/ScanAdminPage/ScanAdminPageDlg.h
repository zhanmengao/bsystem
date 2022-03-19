
// ScanAdminPageDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"


// CScanAdminPageDlg �Ի���
class CScanAdminPageDlg : public CDialogEx
{
// ����
public:
	CScanAdminPageDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_SCANADMINPAGE_DIALOG };

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
    CListCtrl m_ScanList;   // �б��ؼ�����
    HANDLE m_hEvent;        // �¼����������ڶ��̴߳��ݲ���ʱʹ��
    afx_msg void OnBnClickedButton1();
    // ɨ���߳�
    static DWORD WINAPI ScanThread(LPVOID lpParam);
    // ���URL�Ƿ���Ч��������¼��̨ҳ���Ƿ���Ч
    BOOL CheckUrl(CString strUrl);
};
