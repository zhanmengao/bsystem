// veidp.cpp : 定义应用程序的入口点。
//
#include "stdafx.h"
#include<atlstr.h>
#include "veidp.h"
#include "infoCollect.h"
#include "downloadFile.h"
#include <Winuser.h>
#include"../../SystemInfo/SystemInfo/SysInfoUtils.h"
#include<Dbt.h>


#include"../../../IPC/ListenerProject/ListenerProject/MessageUtils.h"

#include"../../../Time/Time/WinTime.h"
#include"../../SystemInfo/WMI/WMIUtils.h"
#include"../../../Windows/AppManager/AppManager/Apps.h"
#include"../../../Setting/SysSetting/TaskSchedule/TaskSchduleManager.h"
#include"../../../../Network/Serialization/Json/JsonTools.hpp"
#include"../../../USB/WUSBDevice/USBUtils.h"
#include"../../../Error/WErrorUtils/ErrorInfo.h"
using namespace std;


#define MAX_LOADSTRING 100
Json::Value StartRoot;
Json::Value EndRoot;
// 全局变量: 
HINSTANCE hInst;                                // 当前实例
HINSTANCE hgAppInst;							// dialog做主窗口 实列
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名
// 此代码模块中包含的函数的前向声明: 
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);


static string GlobalwbUptime;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: 在此放置代码。
	IDPManager::Log.SetPath(PathHelper::GetModuleFolder() + "log\\");
	LOG_DAILY("VEIDP Start Begin");
	if (!IDPManager::WMI().Begin())
	{
		LOG("GlobalData::wmi.Begin Error");
	}
	// 初始化全局字符串
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_VEIDP, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 执行应用程序初始化: 
	if (!InitInstance(hInstance, nCmdShow))
	{
		LOG("InitInstance Error！！！！！");
		return FALSE;
	}
	LOG_DAILY("VEIDP Start End");
	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_VEIDP));
	MSG msg;

	// 主消息循环: 
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	_CrtDumpMemoryLeaks();
	LOG_DAILY("Process Exit");
	return (int)msg.wParam;
}

//  函数: MyRegisterClass()
//  目的: 注册窗口类。

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_VEIDP));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_VEIDP);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_VEIDP));

	return RegisterClassExW(&wcex);
}

//   函数: InitInstance(HINSTANCE, int)
//
//   目的: 保存实例句柄并创建主窗口
//
//   注释: 
//
//   在此函数中，我们在全局变量中保存实例句柄并
//   创建和显示主程序窗口。

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HANDLE hMutext = CreateMutex(NULL, FALSE, "veidp");
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		LOG("CreateMutex Error！！！！！");
		//MessageBox(NULL, _T("程序运行中，请不要重复运行！"), _T("信息提示"), MB_OK);
		return 0;
	}

	hInst = hInstance; // 将实例句柄存储在全局变量中
	hgAppInst = hInstance;
	int width = 0;
	int high = 0;
	int iShow = SWP_HIDEWINDOW;
#if __DEBUG
	width = 800;
	high = 600;
	iShow = SWP_SHOWWINDOW;
#endif
	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		width, 0, width, high, nullptr, nullptr, hInstance, nullptr);
	if (!hWnd)
	{
		LOG("CreateWindowW Error！！！！！");
		return FALSE;
	}
	ShowWindow(hWnd, iShow);
	UpdateWindow(hWnd);
	//设置窗口居中显示
	int scrWidth, scrHeight;
	RECT rect;
	//获得屏幕尺寸
	scrWidth = GetSystemMetrics(SM_CXSCREEN);
	scrHeight = GetSystemMetrics(SM_CYSCREEN);
	//取得窗口尺寸
	GetWindowRect(hWnd, &rect);
	//重新设置rect里的值
	rect.left = (scrWidth - rect.right) / 2;
	rect.top = (scrHeight - rect.bottom) / 2;
	//移动窗口到指定的位置

	SetWindowPos(hWnd, HWND_TOP, rect.left, rect.top, rect.right, rect.bottom, iShow);

	return TRUE;
}


string SetTime(string Time)
{
	string year;
	char strRet[1024] = { 0 };
	if (Time != "*" && Time != "")
	{
		int i = 0, j = 0;
		int TimeSize = Time.length();
		while (i < TimeSize)
		{
			if (i == 4)
			{
				strRet[j + i] = '-';
				j++;
				strRet[j + i] = Time[i];
				i++;
				continue;
			}
			else if (i == 6)
			{
				strRet[j + i] = '-';
				j++;
				strRet[j + i] = Time[i];
				i++;
				continue;
			}
			strRet[j + i] = Time[i];
			i++;
		}
	}
	return year = strRet;
}
void StartCreate()
{
	thread t(StartThread);
	if (t.joinable())
	{
		t.join();
	}
}
void StartThread()
{
	std::unique_lock<std::mutex> lock(IDPManager::User.StartLock);
	if (!IDPManager::User.bStart)
	{
		IDPManager::User.bStart = true;
		//=============== 开机发送的数据 ===============
		StartRoot["wbUptime"] = GlobalwbUptime;
		char tval[MAX_PATH] = { 0 };
		aidaIf = IDPManager::Config.bStartAida;
		Sleep(1000);
		if (aidaIf)
		{
			aidaIf = FALSE;
			aide();
		}
		char* filename = "C:\\VEIDP\\aida64business\\Reports\\Report.ini";
		net_info netInfo = SysInfoUtils::GetNetInfo();

		//用户名	"week"
		string LoginName11 = wbLoginName();
		StartRoot["LoginName"] = LoginName11;

		//域		"WORKGROUP",
		string wbyu1 = region();
		StartRoot["wbyu"] = wbyu1;

		//ip	"F4:4D:30:F2:23:60",
		StartRoot["ip"] = netInfo.strIp;

		//mac	"192.168.50.88",
		StartRoot["mac"] = netInfo.strMac;


		::GetPrivateProfileString("系统概述", "DMI|DMI 系统制造商", "*", tval, MAX_PATH, filename);
		StartRoot["wbRegisteredOwner"] = tval;

		::GetPrivateProfileString("系统概述", "计算机|计算机名称", "*", tval, MAX_PATH, filename);
		StartRoot["MachineName"] = tval;


		//系统补丁
		vector<WMIPatchInfo> patchvec;
		std::string err;
		IDPManager::WMI().GetPatch(patchvec, err);
		Json::Value wbPatch;
		for (int i = 0; i < patchvec.size(); i++)
		{
			Json::Value hotfix;
			hotfix["description"] = CONVERT(patchvec[i].GetDesc());//hotfixInfo[i].description;
			hotfix["name"] = CONVERT(patchvec[i].Name);// hotfixInfo[i].name;
			hotfix["install_time"] = patchvec[i].GetInstall();
			wbPatch.append(hotfix);
		}
		if (wbPatch.isNull())
		{
			StartRoot["wbPatch"] = "";
		}
		else
		{
			StartRoot["wbPatch"] = wbPatch;
		}

		//错误日志采集
		std::vector<EventLog> eventvec;
		SysInfoUtils::GetEventLog(eventvec);
		Json::Value wberrLog;
		for (auto it = eventvec.begin(); it != eventvec.end(); it++)
		{
			if (it->EventType == EVENTLOG_ERROR_TYPE)
			{
				Json::Value error;
				char str2[64] = { 0 };
				sprintf(str2, "%d", it->EventID & 0xFFFF);

				error["LogTime"] = to_string(it->TimeGenerated);
				error["LogID"] = str2;
				error["LogTypeName"] = CONVERT(it->GetEventTypeStr());
				wberrLog.append(error);
			}
		}
		if (wberrLog.isNull())
		{
			StartRoot["errorLog"] = "";
		}
		else
		{
			StartRoot["errorLog"] = wberrLog;
		}

		//系统制造商"Haier",
		::GetPrivateProfileString("系统概述", "DMI|DMI 系统制造商", "*", tval, MAX_PATH, filename);
		StartRoot["wbRegisteredOwner"] = tval;

		//系统型号	"Haier DT Computer",
		::GetPrivateProfileString("系统概述", "主板|主板名称", "*", tval, MAX_PATH, filename);
		StartRoot["SystemProductName"] = tval;

		//CPU	"Intel(R) Core(TM) i5-7400 CPU @ 3.00GHz",
		string wbOsCpuInfo1 = wbOsCpuInfo("ProcessorNameString");
		StartRoot["cpu"] = wbOsCpuInfo1;
		//cpuid

		vector<WMICPU> vecCPU;
		if (IDPManager::WMI().GetCPU(vecCPU,err))
		{
			if (vecCPU.size() >= 1 && !vecCPU[0].ProcessorId.empty())
			{
				IDPManager::User.CPUID = vecCPU[0].ProcessorId;
			}
		}
		else
		{
			LOG(std::string("GetCPU Error") + ErrorUtils::ErrorMessage(IDPManager::WMI().GetLastErrorWMI()));
		}

		StartRoot["cpuid"] = IDPManager::User.CPUID;


		//操作系统+版本	"Windows 10 Pro 1903 18362",
		string wbHostInfo1 = wbOsInfo("ProductName");
		wbHostInfo1 += " ";
		string wbHostInfoVersions1 = wbOsInfo("ReleaseId");
		wbHostInfo1 += wbHostInfoVersions1;

		string wbHostInfoVersions2 = wbOsInfo("CurrentBuild");
		wbHostInfo1 += " ";
		wbHostInfo1 += wbHostInfoVersions2;
		StartRoot["os"] = wbHostInfo1;

		//机器名	"DESKTOP-DQFMI7A",
		string wbHostName1 = wbHostName();
		StartRoot["wbHostName"] = wbHostName1;

		//系统初始安装日期	"2019-08-07 10:46:07"
		string wbInstaDate11 = wbOsInfo11("InstallDate");

		//转换时间戳
		//const char* wbStr = wbInstaDate11.data();
		StartRoot["wbinitializeDate"] = wbInstaDate11;

		//系统ID 设备列号(BIOS 序列号) "EIH110HE05",

		//sprintf(str1, "打印机%d|打印机属性|打印机名称", i);
		::GetPrivateProfileString("系统概述", "DMI|DMI BIOS 版本", "*", tval, MAX_PATH, filename);
		StartRoot["BIOSVersion"] = tval;

		::GetPrivateProfileString("DMI", "系统|系统|序列号", "*", tval, MAX_PATH, filename);
		StartRoot["serial_num"] = tval;

		StartRoot["orgCode"] = CONVERT(IDPManager::Config.organization);

		//已安装的应用程序
		Json::Value softList;
		vector<RegApplication> info;
		AppManager::GetSoftListInfo(info);
		for (int i = 0; i < info.size(); i++)
		{
			Json::Value soft;
			soft["name"] = CONVERT(info[i].DisplayName);  //info[i].displayName;
			soft["version"] = CONVERT(info[i].DisplayVersion);
			if (info[i].InstallDate != "*")
			{
				soft["Time"] = CONVERT(SetTime(info[i].InstallDate));
			}
			else
			{
				soft["Time"] = CONVERT(info[i].InstallDate);
			}
			soft["Post"] = CONVERT(info[i].Publisher);
			softList.append(soft);
		}

		if (softList.isNull())
		{
			StartRoot["softlist"] = "";
		}
		else
		{
			StartRoot["softlist"] = softList;
		}
		Sleep(1000);
	}
	RocketMQLink(StartRoot, "start");
}
//将关机数据预存，但不发
void CALLBACK Tick_End(HWND hwnd, UINT message, UINT idTimer, DWORD dwTime)
{
	try
	{
		LOG_DAILY("Tick_End Begin");
		//系统补丁
		vector<WMIPatchInfo> patchvec;
		std::string err;
		if (IDPManager::WMI().GetPatch(patchvec,err))
		{
			Json::Value wbPatch;
			for (int i = 0; i < patchvec.size(); i++)
			{
				Json::Value hotfix;
				hotfix["description"] = CONVERT(patchvec[i].GetDesc());//hotfixInfo[i].description;
				hotfix["name"] = CONVERT(patchvec[i].Name);// hotfixInfo[i].name;
				hotfix["install_time"] = patchvec[i].GetInstall();
				wbPatch.append(hotfix);
			}
			if (wbPatch.isNull())
			{
				EndRoot["wbPatch"] = "";
			}
			else
			{
				EndRoot["wbPatch"] = wbPatch;
			}
		}
		else
		{
			LOG("WMI GetPatch Error" + IDPManager::WMI().GetLastErrorWMI());
		}


		//错误日志采集
		std::vector<EventLog> eventvec;
		SysInfoUtils::GetEventLog(eventvec);
		Json::Value wberrLog;
		for (auto it = eventvec.begin(); it != eventvec.end(); it++)
		{
			if (it->EventType == EVENTLOG_ERROR_TYPE)
			{
				Json::Value js;
				char str2[64] = { 0 };
				sprintf(str2, "%d", it->EventID & 0xFFFF);
				js["LogTime"] = std::to_string(it->TimeGenerated);
				js["LogID"] = str2;
				js["LogTypeName"] = CONVERT(it->GetEventTypeStr());
				wberrLog.append(js);
			}
		}
		if (wberrLog.isNull())
		{
			EndRoot["errorLog"] = "";
		}
		else
		{
			EndRoot["errorLog"] = wberrLog;
		}
		//已安装的应用程序
		Json::Value softList;
		vector<RegApplication> info;
		AppManager::GetSoftListInfo(info);
		for (int i = 0; i < info.size(); i++)
		{
			Json::Value soft;
			soft["name"] = CONVERT(info[i].DisplayName);  //info[i].displayName;
			soft["version"] = CONVERT(info[i].DisplayVersion);
			if (info[i].InstallDate != "*")
			{
				soft["Time"] = CONVERT(SetTime(info[i].InstallDate));
			}
			else
			{
				soft["Time"] = CONVERT(info[i].InstallDate);
			}
			soft["Post"] = CONVERT(info[i].Publisher);
			softList.append(soft);
		}

		if (softList.isNull())
		{
			EndRoot["softlist"] = "";
		}
		else
		{
			EndRoot["softlist"] = softList;
		}
		LOG_DAILY("Tick_End End");
	}
	catch (exception& e)
	{
		char buffer[1024] = { 0 };
		sprintf(buffer, "[Catch exception]:%s : %s \n", __FUNCTION__, e.what());
		LOG(buffer);
	}
}
void CALLBACK Tick_Pant(HWND hwnd, UINT message, UINT idTimer, DWORD dwTime)
{
	Tick();
}

void Tick()
{
	try
	{
		LOG_DAILY("Pant Begin");
		//=============== 心跳发送的数据 ===============
		Json::Value RunTimeRoot;
		BUILD_MSG_HEAD(RunTimeRoot);

		RunTimeRoot["wbUptime"] = GlobalwbUptime;

		//开机运行时长
		RunTimeRoot["wbRunUptime"] = to_string(WinTimeUtils::GetBootRunTimeS());

		//获取TCP监听端口
		RunTimeRoot["wbTCP"] = GetTCPportlist();

		//获取UDP监听端口
		RunTimeRoot["wbUDP"] = GetUDPportlist();

		//获取已运行的进程
		RunTimeRoot["runPRO"] = runProcess();

		//操作系统+版本	"Windows 10 Pro 1903 18362",
		string wbHostInfo1 = wbOsInfo("ProductName");

		if (!(strncmp(wbHostInfo1.c_str(), "Windows", 7)))
		{
			wbHostInfo1 = "Windows";
		}
		else if (!(strncmp(wbHostInfo1.c_str(), "windows", 7)))
		{
			wbHostInfo1 = "windows";
		}

		RunTimeRoot["os"] = wbHostInfo1;
		RocketMQLink(RunTimeRoot, "pant");
		LOG_DAILY("Pant End");
	}
	catch (exception& e)
	{
		char buffer[1024] = { 0 };
		sprintf(buffer, "[Catch exception]:%s : %s \n", __FUNCTION__, e.what());
		LOG(buffer);
	}
}

void CALLBACK Tick_AIDA(HWND hwnd, UINT message, UINT idTimer, DWORD dwTime)
{
	try
	{
		aide();
	}
	catch (exception& e)
	{
		char buffer[1024] = { 0 };
		sprintf(buffer, "[Catch exception]:%s : %s \n", __FUNCTION__, e.what());
		LOG(buffer);
	}
}
void EndCreate()
{
	if (!IDPManager::bSendEndMessage)
	{
		IDPManager::bSendEndMessage = true;
	}
	else
	{
		return;
	}
	//=============== 关机发送的数据 加上关机心跳预存的数据 ===============
	EndRoot["mac"] = IDPManager::Config.Mac;
	//使用时间戳发送
	EndRoot["Endtime"] = to_string(::GetAnsiTime());
	EndRoot["wbUptime"] = GlobalwbUptime;

	//=========================进程统计=====================
	auto countMap = IDPManager::User.MonitProcessEnd();
	Json::Value procList;
	//拿到name和计数
	for (auto it = countMap.begin(); it != countMap.end(); it++)
	{
		Json::Value procInfo;
		procInfo["name"] = CONVERT(it->first.c_str());
		procInfo["counter"] = to_string(it->second);
		procList.append(procInfo);
	}
	if (procList.isNull())
	{
		EndRoot["softwareCount"] = "";
	}
	else
	{
		EndRoot["softwareCount"] = procList;
	}
	RocketMQLink(EndRoot, "end");
	IDPManager::Destroy();
}



int ProcessMQMessage(struct CPushConsumer * consumer, CMessageExt * msgExt)
{
	const char* msg = GetMessageBody(msgExt);
	thread t(ProcessMQThread, msg);
	if (t.joinable())
	{
		t.join();
	}
	return E_CONSUME_SUCCESS;
}

int ProcessMQThread(const char* msg)
{
	try
	{
		LOG_DAILY("ProcessMQMessage Begin");
		std::unique_lock<std::mutex> lock(IDPManager::User.MsgLock);

		Json::Value  resp;
		int Mess = 0;
		string mac;
		if (JsonTools::SetJsonData(msg, resp))
		{
			if (!resp["commandType"].isNull())
			{
				Mess = resp["commandType"].asInt();
			}

			if (!resp["mac"].isNull())
			{
				mac = resp["mac"].asString();
			}
			LOG_DAILY("ProcessMQMessage Recv" + to_string(Mess) + "	Mac :" + mac + "    Json \n" + msg);
		}
		else
		{
			LOG(string("ProcessMQMessage recv a error json \n") + msg);
			lock.unlock();
			aide(true);
			lock.lock();
			//收到错误Json，把数据都发过去
			instructScreenCapture(false);
			OnCommandHardware();
			return E_CONSUME_SUCCESS;
		}

		net_info	netInfo = SysInfoUtils::GetNetInfo();
		if (netInfo.strMac == mac)
		{
			if (IDPManager::User.IsMsgAIDA((COMMAND)Mess))
			{
				lock.unlock();
				aide(true);
				lock.lock();
			}
			switch (Mess)
			{
			case COMMAND::COMM_COLLALL:				//全部信息采集
			{
				OnCommandCollAll();
				break;
			}
			case COMMAND::COMM_SCREEN:			//屏幕截图
			{
				instructScreenCapture(false);
				break;
			}
			case COMMAND::COMM_DISK:					//磁盘	
			{
				SendDiskInfo(false);
				break;
			}
			case COMMAND::COMM_MEMORY:				//内存
			{
				instructCreateMemory(false);
				break;
			}
			case COMMAND::COMM_MONIT:			//屏幕信息
			{
				instructScreenInfo(false);
				break;
			}
			case COMMAND::COMM_PRINTER:				//打印机
			{
				instructPrinter(false);
				break;
			}
			case COMMAND::COMM_KEY_MOUSE:				//键盘鼠标	
			{
				SendKeyMouse(false);
				break;
			}
			case COMMAND::COMM_DOWNFILE:			//下载
			{
				Download(Mess, false, resp.toStyledString());
				break;
			}
			case COMMAND::COMM_HARDWARE:			//硬件
			{
				OnCommandHardware();
				break;
			}
			case COMMAND::COMM_CPU:
			{
				SendCPUInfo();
				break;
			}
			case COMMAND::COMM_MAINBOARD:
			{
				SendMotherInfo();
				break;
			}
			case COMMAND::COMM_USB:
			{
				SendUSB(true);
				break;
			}
			case COMMAND::COMM_START:
			{
				StartCreate();
				break;
			}
			case COMMAND::COMM_TICK:
			{
				Tick();
				break;
			}
			default:
				break;
			}
		}
		else
		{
			LOG(string("ProcessMQMessage different mac=  ") + mac + "My Mac" + netInfo.strMac + "	Json \n" + msg);
		}
		LOG_DAILY("ProcessMQMessage End");
		return E_CONSUME_SUCCESS;
	}
	catch (exception& e)
	{
		char buffer[1024] = { 0 };
		sprintf(buffer, "[Catch exception]:%s : %s \n", __FUNCTION__, e.what());
		LOG(buffer);
	}
	return E_CONSUME_SUCCESS;
}
static CPushConsumer* consumer = NULL;
BOOL wbGetMessageFun()
{
	if (consumer != NULL)
	{
		ShutdownPushConsumer(consumer);
		DestroyPushConsumer(consumer);
	}
	consumer = CreatePushConsumer(IDPManager::Config.Group.c_str());
	int ret = SetPushConsumerNameServerAddress(consumer, IDPManager::Config.InstructIp.c_str());
	if (ret != 0)
	{
		LOG("SetPushConsumerNameServerAddress fail " + to_string(ret) + "ip : " + IDPManager::Config.InstructIp);
		return ret;
	}
	LOG_DAILY("SetPushConsumerNameServerAddress sec group : " +
		IDPManager::Config.Group + " ip " + IDPManager::Config.InstructIp);
	ret = Subscribe(consumer, IDPManager::Config.InstructTopic.c_str(), IDPManager::Config.Mac.c_str());
	if (ret != 0)
	{
		LOG("Subscribe fail " + to_string(ret));
		return ret;
	}
	LOG_DAILY("Subscribe Sec Topic : " + IDPManager::Config.InstructTopic + " Mac:" + IDPManager::Config.Mac);
	ret = RegisterMessageCallback(consumer, ProcessMQMessage);
	if (ret != 0)
	{
		LOG("Register MQ Callback Fail " + to_string(ret));
		return ret;
	}
	ret = StartPushConsumer(consumer);
	if (ret != 0)
	{
		LOG("StartPushConsumer Fail " + to_string(ret));
		return ret;
	}
	LOG_DAILY("StartPushConsumer Sec ip : " + IDPManager::Config.InstructIp);
	return ret;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
	{
		LOG_DAILY("Recv WM_CREATE");
		OnWMCreate(hWnd);
		break;
	}
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// 分析菜单选择: 
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		case ID_COMM_1:
			OnCommandCollAll();
			break;
		case ID_COMM_2:
			//发2
			instructScreenCapture();
			break;
		case ID_COMM_3:
			SendDiskInfo();
			break;
		case ID_COMM_4:
			instructCreateMemory();
			break;
		case ID_COMM_5:
			instructScreenInfo();
			break;
		case ID_COMM_6:
			instructPrinter();
			break;
		case ID_COMM_7:
			SendKeyMouse();
			break;
		case ID_COMM_8:
			StartCreate();
			break;
		case ID_COMM_9:
			//发9号
			OnCommandHardware();
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_DEVICECHANGE:
	{
		OnWMDevice(hWnd, wParam, lParam);
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_QUERYENDSESSION:
	{
		LOG_DAILY("Recv Message WM_QUERYENDSESSION");
		//结束程序发送一次采集数据
		EndCreate();
		return 1;
	}
	case WM_CLOSE:
		//确定退出,销毁窗口,抛出一个WM_DESTYRY的消息
		LOG_DAILY("Recv Message WM_CLOSE");
		DestroyWindow(hWnd);
		EndCreate();
		break;
	case WM_DESTROY:
	{
		LOG_DAILY("Recv Message WM_DESTROY");
		//结束当前进程
		EndCreate();
		HANDLE hself = GetCurrentProcess();
		TerminateProcess(hself, 0);

		PostQuitMessage(0);
		break;
	}
	default:
		break;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

void OnWMCreate(HWND hWnd)
{
	try
	{

#if __TASK_
		//将自己添加到开机启动项
		{
			TaskSchduleManager task;
			if (task.NewTask(_T("VEIDP"), PathHelper::GetModuleFilePath().c_str()))
			{
				LOG_DAILY("Reg Auto Run ERROR_SUCCESS");
			}
			else
			{
				LOG("Reg Auto Run Error" + to_string(GetLastError()));
			}
	}
#endif
		IDPManager::Start();

		//删除无用日志
		IDPManager::Log.DeleteLogMonth(2);
		//电脑开机时间
		GlobalwbUptime = to_string(::GetAnsiTime());
		//开机时发送一次采集数据
		StartCreate();

		//心跳线程采集数据  设置定时器
		int pantTime = IDPManager::Config.pantTime;
		int aidaTimer = IDPManager::Config.aidaTick;
		int endTimer = IDPManager::Config.endTick;
		//定时发送心跳
		pantTime = pantTime * 1000 * 60;
		SetTimer(NULL, 10, pantTime, Tick_Pant);
		//定时刷新AIDA
		SetTimer(NULL, 110, aidaTimer * 1000 * 60, Tick_AIDA);

		//定时刷新关机信息（秒级）
		SetTimer(NULL, 11, endTimer * 1000, Tick_End);
		Tick_End(NULL, 0, 0, 0);
		BOOL ret = wbGetMessageFun();

		//注册事件
		auto pHandle = MessageUtils::RegDeviceNotification(hWnd);
		if (pHandle)
		{
			LOG_DAILY("RegDeviceNotification Sec");
		}
		else
		{
			LOG("RegDeviceNotification Fail");
		}
}
	catch (exception& e)
	{
		char buffer[1024] = { 0 };
		sprintf(buffer, "[Catch exception]:%s : %s \n", __FUNCTION__, e.what());
		LOG(buffer);
	}
}
void OnWMDevice(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	try
	{
		tm current = ::GetTime(::GetAnsiTime());
		LOG_DAILY("Recv WM_DEVICECHANGE : " + MessageUtils::GetDeviceNotfiDesc(wParam));
		//发生事件，采集一下
		if (wParam == DBT_DEVNODES_CHANGED)
		{
			if (!IDPManager::User.CompareUSBMap())
			{
				OnEventSend();
			}
		}
		else
		{
			//避免多次获取消息
				//USB设备变动，发消息
			if (!::CompareSec(current, IDPManager::mLastDrivetm, 1))
			{
				if (!IDPManager::User.CompareUSBMap())
				{
					OnEventSend();
				}
				IDPManager::mLastDrivetm = current;
			}
		}
	}
	catch (exception& e)
	{
		char buffer[1024] = { 0 };
		sprintf(buffer, "[Catch exception]:%s : %s \n", __FUNCTION__, e.what());
		LOG(buffer);
	}
}

