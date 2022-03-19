// PeParseDlg.h : header file
//

#if !defined(AFX_PEPARSEDLG_H__13847462_85E3_4E68_BAB8_D6548059CB2B__INCLUDED_)
#define AFX_PEPARSEDLG_H__13847462_85E3_4E68_BAB8_D6548059CB2B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CPeParseDlg dialog

class CPeParseDlg : public CDialog
{
// Construction
public:
	VOID EnumSections();                // ö�ٽڱ�
	VOID ParseBasePe();                 // ��������PE�ֶ�
	VOID FileClose();                   // �ر��ļ�
	PIMAGE_DOS_HEADER     m_pDosHdr;    // DOSͷָ��
    PIMAGE_NT_HEADERS     m_pNtHdr;     // PEͷָ��
    PIMAGE_SECTION_HEADER m_pSecHdr;    // �ڱ�ָ��

	BOOL FileCreate(char *szFileName);  // ���ļ�������ӳ����ͼ
	LPVOID m_lpBase;                    // ӳ����ͼָ��
    HANDLE m_hMap;                      // ӳ����
	HANDLE m_hFile;                     // �ļ����
	BOOL   IsPeFileAndGetPEPointer();   // �ж��Ƿ�ΪPE�ļ�������ȡ��ؽṹָ��
	VOID   InitSectionList();           // ��ʼ���б��

	CPeParseDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CPeParseDlg)
	enum { IDD = IDD_PEPARSE_DIALOG };
	CListCtrl	m_SectionLIst;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPeParseDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CPeParseDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBtnExit();
	afx_msg void OnBtnShow();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PEPARSEDLG_H__13847462_85E3_4E68_BAB8_D6548059CB2B__INCLUDED_)
