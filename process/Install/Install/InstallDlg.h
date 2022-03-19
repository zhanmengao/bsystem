
// InstallDlg.h : 头文件
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
	bool bIsInstall;					//是否已安装
	stdstr mDesc;					//描述
	 int ret = -1;			//安装返回值
};
enum INSTALL_STATE
{
	INSTALL_FREE,						//完全没有安装时，空闲
	INSTALL_RUN,						//正在安装时
	INSTALL_PAUSE_WAIT,		//安装，手动结束等待线程退出
	INSTALL_PAUSE,					//安装了一半，手动暂停
	INSTALL_RESTART,				//等待重启
	INSTALL_END,						//安装结束时
};
// CInstallDlg 对话框
class CInstallDlg : public CDialogEx
{
// 构造
public:
	CInstallDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_INSTALL_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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
