
// ResourceFree_TestDlg.h : ͷ�ļ�
//

#pragma once

#include "ResourceFree.h"


// CResourceFree_TestDlg �Ի���
class CResourceFree_TestDlg : public CDialogEx
{
// ����
public:
	CResourceFree_TestDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_RESOURCEFREE_TEST_DIALOG };

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
	afx_msg void OnBnClickedButtonRelease();
};
