
// DelayLoadDll_TestDlg.h : ͷ�ļ�
//

#pragma once

#include "Skins\\SkinPPWTL.h"
#pragma comment(lib, "Skins\\SkinPPWTL.lib")


// CDelayLoadDll_TestDlg �Ի���
class CDelayLoadDll_TestDlg : public CDialogEx
{
// ����
public:
	CDelayLoadDll_TestDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_DELAYLOADDLL_TEST_DIALOG };

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
