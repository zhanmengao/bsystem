
// WM_DEVICECHANGE_MFC_TestDlg.h : ͷ�ļ�
//

#pragma once
#include <Dbt.h>


// CWM_DEVICECHANGE_MFC_TestDlg �Ի���
class CWM_DEVICECHANGE_MFC_TestDlg : public CDialogEx
{
public:
	LRESULT OnDeviceChange(WPARAM wParam, LPARAM lParam);

// ����
public:
	CWM_DEVICECHANGE_MFC_TestDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_WM_DEVICECHANGE_MFC_TEST_DIALOG };

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
};
