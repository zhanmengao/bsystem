
// InstallDlg.h : ͷ�ļ�
//

#pragma once
#include<map>
#include"../CommandCall/CommandCall.h"
#include<thread>
#include"../../../Setting/Readini/INIManager.hpp"
#include<mutex>
struct ScriptInfo
{
	struct ScriptItem item;
	bool bIsInstall;					//�Ƿ��Ѱ�װ
	stdstr mDesc;					//����
	 int ret = -1;			//��װ����ֵ
};
enum INSTALL_STATE
{
	INSTALL_FREE,						//��ȫû�а�װʱ������
	INSTALL_RUN,						//���ڰ�װʱ
	INSTALL_PAUSE_WAIT,		//��װ���ֶ������ȴ��߳��˳�
	INSTALL_PAUSE,					//��װ��һ�룬�ֶ���ͣ
	INSTALL_RESTART,				//�ȴ�����
	INSTALL_END,						//��װ����ʱ
};
// CInstallDlg �Ի���
class CInstallDlg : public CDialogEx
{
// ����
public:
	CInstallDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_INSTALL_DIALOG };
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
	CListCtrl mScriptList;
	CStatic mTips;
	CEdit mOutput;
	CButton mOKBtn;
	CButton mCancelBtn;
	CButton mResetBtn;
	std::vector<ScriptInfo> mScriptVec;
	std::thread *t = nullptr;
	std::mutex mOutputLock;
	ScriptInfo* mCurrentInstall = nullptr;
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	void UpdateScriptState();
	void SetOutputText(const TCHAR* text);

	void SetState(INSTALL_STATE state);
	INSTALL_STATE mState;
	iniManager ini;
private:
	void ShellScripts();
	void WaitThread();
public:
	afx_msg void OnBnClickedReset();
};
