
// InstallDlg.cpp : ʵ���ļ�
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


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CInstallDlg �Ի���



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


// CInstallDlg ��Ϣ�������

BOOL CInstallDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	pSysMenu->EnableMenuItem(SC_CLOSE, MF_DISABLED);
	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	//mScriptList.SetExtendedStyle(mScriptList.GetStyle() | LVS_EX_CHECKBOXES);//ǰ���й�ѡ��
	mScriptList.InsertColumn(0, _T("����"),0,300);
	mScriptList.InsertColumn(2,_T("����"), 0, 250);
	mScriptList.InsertColumn(1, _T("״̬"), 0, 80);
	//��ini �ӵ��б�
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
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}
void CInstallDlg::UpdateScriptState()
{
	//��ini �ӵ��б�
	for (int i = 0;i<mScriptVec.size();i++)
	{
		auto& item = mScriptVec[i];
		mScriptList.SetItemText(i, 0, item.item.Command.c_str());
		mScriptList.SetItemText(i, 2, item.mDesc.c_str());
		if (item.bIsInstall)
		{
			if (item.ret == -1)
			{
				mScriptList.SetItemText(i, 1, _T("�Ѱ�װ"));
			}
			else
			{
				TCHAR str[1024] = { 0 };
				wsprintf(str, _T("%s %d"), _T("�Ѱ�װ"), item.ret);
				mScriptList.SetItemText(i, 1, str);
			}
		}
		else
		{
			mScriptList.SetItemText(i, 1, _T("δ��װ"));
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CInstallDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
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
		mTips.SetWindowText(_T("��ӭ��ʹ�ð�װ��"));
		mOKBtn.SetWindowText(_T("��ʼ"));
		mCancelBtn.SetWindowText(_T("�˳�"));
	}
	break;
	case INSTALL_RUN:
	{
		mOKBtn.SetWindowText(_T("ִ����"));
		mCancelBtn.SetWindowText(_T("ֹͣ"));
	}
	break;
	case INSTALL_PAUSE_WAIT:
	{
		mTips.SetWindowText(_T("����ִ���У��ȴ��߳��˳������Ժ�..."));
	}
	break;
	case INSTALL_PAUSE:
	{
		mTips.SetWindowText(_T("��װ��ͣ"));
		mOKBtn.SetWindowText(_T("����"));
		mCancelBtn.SetWindowText(_T("�˳�"));
	}
	break;
	case INSTALL_RESTART:
	{
		mTips.SetWindowText(_T("��װ�޷��������ȴ�����"));
		mOKBtn.SetWindowText(_T("��������"));
		mCancelBtn.SetWindowText(_T("�˳�"));
	}
	break;
	case INSTALL_END:
	{
		mTips.SetWindowText(_T("��װ�����"));
		mOKBtn.SetWindowText(_T("���"));
		mCancelBtn.SetWindowText(_T("�˳�"));
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
	//��ʼִ�У�����map
	while (mState == INSTALL_RUN)
	{
		//��ȡ��һ����ִ�е�����
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
			SetOutputText((_T("��ʼ��װ	") + mCurrentInstall->mDesc+ _T("	") + mCurrentInstall->item.Command).c_str());
			mScriptList.SetItemState(index, LVNI_FOCUSED | LVIS_SELECTED, LVNI_FOCUSED | LVIS_SELECTED);
			mScriptList.SetSelectionMark(index);
			mTips.SetWindowText((_T("���ڰ�װ	") + mCurrentInstall->mDesc).c_str());
			mScriptList.SetItemText(index, 1, _T("���ڰ�װ"));
			//����
			DWORD exitCode = CommandCall(mCurrentInstall->item, ReadPipe,this);
			SetOutputText((_T("��ɰ�װ	") + mCurrentInstall->mDesc + _T("	") +
				mCurrentInstall->item.Script + _T("	") + mCurrentInstall->item.Command).c_str());
			mCurrentInstall->ret = exitCode;
			//��¼ini�ļ�
			mCurrentInstall->bIsInstall = true;
			ini.WriteNum(mCurrentInstall->item.appName, _T("IsInstall"), 1);
			ini.WriteNum(mCurrentInstall->item.appName, _T("dwret"), exitCode);
			UpdateScriptState();
			if (mCurrentInstall->item.bRestart)
			{
				SetState(INSTALL_RESTART);
				SetOutputText((_T("��Ҫ�����������ܼ�����װ	")));
			}
		}
	}
}
void CInstallDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (mState == INSTALL_FREE || mState == INSTALL_PAUSE)
	{
		SetState(INSTALL_RUN);
		t = new thread(&CInstallDlg::ShellScripts, this);
	}
	else if (mState == INSTALL_RESTART)
	{
		if (MessageBox(_T("����ʹϵͳ�������������Ƿ�ȷ���ѱ����й�Ӧ����Ϣ��"), _T("��ʾ")
			, MB_ICONEXCLAMATION | MB_OKCANCEL) == IDOK)
		{
			if (!PowerOffUtils::Reboot())
			{
				TCHAR buffer[1024] = { 0 };
				wsprintf(buffer, _T("%s   ����:%d"), _T("����ʧ�ܣ����ֶ�����"), GetLastError());
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (mState == INSTALL_RUN)
	{
		if (MessageBox(_T("��װ��δ��ɣ��Ƿ�Ҫ��ͣ��װ"), _T("��ʾ")
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
	
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (mState == INSTALL_END)
	{
		if (MessageBox(_T("���������Ѱ�װ�Ľű���Ϣ"), _T("��ʾ")
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
		MessageBox(_T("��װ��ɺ�ſ�������"), _T("��ʾ"));
	}
}
