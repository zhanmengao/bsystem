
// InstallDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Install.h"
#include "InstallDlg.h"
#include "afxdialogex.h"
#include"../CommandCall/CommandCall.h"
#include"../../../File/FileManager/FileManager/PathHelper.hpp"
#include<thread>
#include<mutex>
#include"../../../Setting/PowerOff/PowerOff/PowerOff.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CInstallDlg 对话框



CInstallDlg::CInstallDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_INSTALL_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CInstallDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TIPS, mTips);
	DDX_Control(pDX, IDC_OUTPUT, mOutput);
	DDX_Control(pDX, IDC_LIST1, mScriptList);
	DDX_Control(pDX, IDOK, mOKBtn);
	DDX_Control(pDX, IDCANCEL, mCancelBtn);
	DDX_Control(pDX, IDC_RESET, mResetBtn);
}

BEGIN_MESSAGE_MAP(CInstallDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CInstallDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CInstallDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_RESET, &CInstallDlg::OnBnClickedReset)
END_MESSAGE_MAP()


// CInstallDlg 消息处理程序

BOOL CInstallDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	pSysMenu->EnableMenuItem(SC_CLOSE, MF_DISABLED);
	// TODO: 在此添加额外的初始化代码
	//mScriptList.SetExtendedStyle(mScriptList.GetStyle() | LVS_EX_CHECKBOXES);//前面有勾选格
	mScriptList.InsertColumn(0, _T("命令"),0,300);
	mScriptList.InsertColumn(2,_T("描述"), 0, 250);
	mScriptList.InsertColumn(1, _T("状态"), 0, 80);
	//读ini 加到列表
	ini.SetPath(PathHelper::GetModuleFolder() + _T("install.ini"));
	UINT size = ini.ReadNum(_T("Common"),_T("MaxCount"),0);
	TCHAR appName[256] = { 0 };
	int iInstall = 0;
	for (unsigned i = 0; i < size; i++)
	{
		wsprintf(appName, _T("Script%d"), i + 1);
		stdstr script = ini.ReadString(appName, _T("Script"),_T(""));
		stdstr command = ini.ReadString(appName, _T("Command"), _T(""));
		if (command.empty())
		{
			continue;
		}
		unsigned j = mScriptVec.size();
		stdstr desc = ini.ReadString(appName, _T("Desc"), _T(""));
		UINT restart = ini.ReadNum(appName, _T("ReStart"), 0);
		UINT isInstall = ini.ReadNum(appName, _T("IsInstall"), 0);
		UINT ret = ini.ReadNum(appName, _T("dwret"), 0);
		mScriptList.InsertItem(j, to_wstring(j).c_str());
		if (isInstall)
		{
			iInstall++;
		}
		ScriptInfo info;
		info.item.bRestart = restart;
		info.item.Command = command;
		info.item.Script = script;
		info.bIsInstall = isInstall;
		info.mDesc = desc;
		info.item.appName = appName;
		info.ret = ret;
		mScriptVec.push_back(info);
	}
	if (iInstall == mScriptVec.size())
	{
		SetState(INSTALL_END);
	}
	else if (iInstall >0)
	{
		SetState(INSTALL_PAUSE);
	}
	else
	{
		SetState(INSTALL_FREE);
	}
	UpdateScriptState();
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}
void CInstallDlg::UpdateScriptState()
{
	//读ini 加到列表
	for (int i = 0;i<mScriptVec.size();i++)
	{
		auto& item = mScriptVec[i];
		mScriptList.SetItemText(i, 0, item.item.Command.c_str());
		mScriptList.SetItemText(i, 2, item.mDesc.c_str());
		if (item.bIsInstall)
		{
			if (item.ret == -1)
			{
				mScriptList.SetItemText(i, 1, _T("已安装"));
			}
			else
			{
				TCHAR str[1024] = { 0 };
				wsprintf(str, _T("%s %d"), _T("已安装"), item.ret);
				mScriptList.SetItemText(i, 1, str);
			}
		}
		else
		{
			mScriptList.SetItemText(i, 1, _T("未安装"));
		}
	}
}
void CInstallDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CInstallDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CInstallDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CInstallDlg::SetOutputText(const TCHAR* text)
{
	std::lock_guard<std::mutex> lock(mOutputLock);
	CString str;
	mOutput.GetWindowText(str);
	mOutput.SetWindowText(str + text + +_T("\r\n"));
	mOutput.SendMessage(WM_VSCROLL, SB_BOTTOM, 0);
}
void CInstallDlg::SetState(INSTALL_STATE state)
{
	std::lock_guard<std::mutex> lock(mOutputLock);
	mState = state;
	switch (state)
	{
	case INSTALL_FREE:
	{
		mTips.SetWindowText(_T("欢迎您使用安装向导"));
		mOKBtn.SetWindowText(_T("开始"));
		mCancelBtn.SetWindowText(_T("退出"));
	}
	break;
	case INSTALL_RUN:
	{
		mOKBtn.SetWindowText(_T("执行中"));
		mCancelBtn.SetWindowText(_T("停止"));
	}
	break;
	case INSTALL_PAUSE_WAIT:
	{
		mTips.SetWindowText(_T("正在执行中，等待线程退出，请稍候..."));
	}
	break;
	case INSTALL_PAUSE:
	{
		mTips.SetWindowText(_T("安装暂停"));
		mOKBtn.SetWindowText(_T("继续"));
		mCancelBtn.SetWindowText(_T("退出"));
	}
	break;
	case INSTALL_RESTART:
	{
		mTips.SetWindowText(_T("安装无法继续，等待重启"));
		mOKBtn.SetWindowText(_T("重新启动"));
		mCancelBtn.SetWindowText(_T("退出"));
	}
	break;
	case INSTALL_END:
	{
		mTips.SetWindowText(_T("安装已完成"));
		mOKBtn.SetWindowText(_T("完成"));
		mCancelBtn.SetWindowText(_T("退出"));
	}
	break;
	}
}
void ReadPipe(const std::string& rData,void* argv)
{
	CInstallDlg* pDlg =static_cast<CInstallDlg*>(argv);
	if (pDlg)
	{
		pDlg->SetOutputText(Convert::StringToWString(rData).c_str());
	}
}
void CInstallDlg::ShellScripts()
{
	//开始执行，遍历map
	while (mState == INSTALL_RUN)
	{
		//获取下一条可执行的任务
		int index = -1;
		mCurrentInstall = nullptr;
		for (int i = 0;i<mScriptVec.size();i++)
		{
			auto& item = mScriptVec[i];
			if (!item.bIsInstall)
			{
				index = i;
				mCurrentInstall = &item;
				break;
			}
		}
		if (index == -1)
		{
			SetState(INSTALL_END);
			break;
		}
		else
		{
			SetOutputText((_T("开始安装	") + mCurrentInstall->mDesc+ _T("	") + mCurrentInstall->item.Command).c_str());
			mScriptList.SetItemState(index, LVNI_FOCUSED | LVIS_SELECTED, LVNI_FOCUSED | LVIS_SELECTED);
			mScriptList.SetSelectionMark(index);
			mTips.SetWindowText((_T("正在安装	") + mCurrentInstall->mDesc).c_str());
			mScriptList.SetItemText(index, 1, _T("正在安装"));
			//调用
			DWORD exitCode = CommandCall(mCurrentInstall->item, ReadPipe,this);
			SetOutputText((_T("完成安装	") + mCurrentInstall->mDesc + _T("	") +
				mCurrentInstall->item.Script + _T("	") + mCurrentInstall->item.Command).c_str());
			mCurrentInstall->ret = exitCode;
			//记录ini文件
			mCurrentInstall->bIsInstall = true;
			ini.WriteNum(mCurrentInstall->item.appName, _T("IsInstall"), 1);
			ini.WriteNum(mCurrentInstall->item.appName, _T("dwret"), exitCode);
			UpdateScriptState();
			if (mCurrentInstall->item.bRestart)
			{
				SetState(INSTALL_RESTART);
				SetOutputText((_T("需要重新启动才能继续安装	")));
			}
		}
	}
}
void CInstallDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	if (mState == INSTALL_FREE || mState == INSTALL_PAUSE)
	{
		SetState(INSTALL_RUN);
		t = new thread(&CInstallDlg::ShellScripts, this);
	}
	else if (mState == INSTALL_RESTART)
	{
		if (MessageBox(_T("程序将使系统重新启动，您是否确认已保存有关应用信息？"), _T("提示")
			, MB_ICONEXCLAMATION | MB_OKCANCEL) == IDOK)
		{
			if (!PowerOffUtils::Reboot())
			{
				TCHAR buffer[1024] = { 0 };
				wsprintf(buffer, _T("%s   错误:%d"), _T("重启失败，请手动重启"), GetLastError());
				SetOutputText(buffer);
			}
		}
	}
	else if (mState == INSTALL_END)
	{
		CDialogEx::OnOK();
	}
}
void CInstallDlg::OnBnClickedCancel()
{
	thread t2(&CInstallDlg::WaitThread, this);
	t2.detach();
}
void CInstallDlg::WaitThread()
{
	// TODO: 在此添加控件通知处理程序代码
	if (mState == INSTALL_RUN)
	{
		if (MessageBox(_T("安装尚未完成，是否要暂停安装"), _T("提示")
			, MB_ICONEXCLAMATION | MB_OKCANCEL) == IDCANCEL)
		{
			return;
		}
		if (mState == INSTALL_RUN)
		{
			SetState(INSTALL_PAUSE_WAIT);
			if (t != nullptr && t->joinable())
			{
				t->join();
				delete t;
				t = NULL;
			}
			SetState(INSTALL_PAUSE);
		}
	}
	else
	{
		if (t != nullptr && t->joinable())
		{
			t->join();
			delete t;
			t = NULL;
		}
		CDialogEx::OnCancel();
	}
}

void CInstallDlg::OnBnClickedReset()
{
	
	// TODO: 在此添加控件通知处理程序代码
	if (mState == INSTALL_END)
	{
		if (MessageBox(_T("程序将重置已安装的脚本信息"), _T("提示")
			, MB_ICONEXCLAMATION | MB_OKCANCEL) == IDCANCEL)
		{
			return;
		}
		for (int i = 0; i<mScriptVec.size(); i++)
		{
			auto& item = mScriptVec[i];
			item.bIsInstall = false;
			ini.WriteNum(item.item.appName, _T("IsInstall"), 0);
		}
		UpdateScriptState();
		SetState(INSTALL_FREE);
	}
	else
	{
		MessageBox(_T("安装完成后才可以重置"), _T("提示"));
	}
}
