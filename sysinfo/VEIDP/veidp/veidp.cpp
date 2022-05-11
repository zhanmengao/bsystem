// veidp.cpp : ����Ӧ�ó������ڵ㡣
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
// ȫ�ֱ���: 
HINSTANCE hInst;                                // ��ǰʵ��
HINSTANCE hgAppInst;							// dialog�������� ʵ��
WCHAR szTitle[MAX_LOADSTRING];                  // �������ı�
WCHAR szWindowClass[MAX_LOADSTRING];            // ����������
// �˴���ģ���а����ĺ�����ǰ������: 
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

	// TODO: �ڴ˷��ô��롣
	IDPManager::Log.SetPath(PathHelper::GetModuleFolder() + "log\\");
	LOG_DAILY("VEIDP Start Begin");
	if (!IDPManager::WMI().Begin())
	{
		LOG("GlobalData::wmi.Begin Error");
	}
	// ��ʼ��ȫ���ַ���
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_VEIDP, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// ִ��Ӧ�ó����ʼ��: 
	if (!InitInstance(hInstance, nCmdShow))
	{
		LOG("InitInstance Error����������");
		return FALSE;
	}
	LOG_DAILY("VEIDP Start End");
	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_VEIDP));
	MSG msg;

	// ����Ϣѭ��: 
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

//  ����: MyRegisterClass()
//  Ŀ��: ע�ᴰ���ࡣ

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

//   ����: InitInstance(HINSTANCE, int)
//
//   Ŀ��: ����ʵ�����������������
//
//   ע��: 
//
//   �ڴ˺����У�������ȫ�ֱ����б���ʵ�������
//   ��������ʾ�����򴰿ڡ�

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HANDLE hMutext = CreateMutex(NULL, FALSE, "veidp");
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		LOG("CreateMutex Error����������");
		//MessageBox(NULL, _T("���������У��벻Ҫ�ظ����У�"), _T("��Ϣ��ʾ"), MB_OK);
		return 0;
	}

	hInst = hInstance; // ��ʵ������洢��ȫ�ֱ�����
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
		LOG("CreateWindowW Error����������");
		return FALSE;
	}
	ShowWindow(hWnd, iShow);
	UpdateWindow(hWnd);
	//���ô��ھ�����ʾ
	int scrWidth, scrHeight;
	RECT rect;
	//�����Ļ�ߴ�
	scrWidth = GetSystemMetrics(SM_CXSCREEN);
	scrHeight = GetSystemMetrics(SM_CYSCREEN);
	//ȡ�ô��ڳߴ�
	GetWindowRect(hWnd, &rect);
	//��������rect���ֵ
	rect.left = (scrWidth - rect.right) / 2;
	rect.top = (scrHeight - rect.bottom) / 2;
	//�ƶ����ڵ�ָ����λ��

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
		//=============== �������͵����� ===============
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

		//�û���	"week"
		string LoginName11 = wbLoginName();
		StartRoot["LoginName"] = LoginName11;

		//��		"WORKGROUP",
		string wbyu1 = region();
		StartRoot["wbyu"] = wbyu1;

		//ip	"F4:4D:30:F2:23:60",
		StartRoot["ip"] = netInfo.strIp;

		//mac	"192.168.50.88",
		StartRoot["mac"] = netInfo.strMac;


		::GetPrivateProfileString("ϵͳ����", "DMI|DMI ϵͳ������", "*", tval, MAX_PATH, filename);
		StartRoot["wbRegisteredOwner"] = tval;

		::GetPrivateProfileString("ϵͳ����", "�����|���������", "*", tval, MAX_PATH, filename);
		StartRoot["MachineName"] = tval;


		//ϵͳ����
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

		//������־�ɼ�
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

		//ϵͳ������"Haier",
		::GetPrivateProfileString("ϵͳ����", "DMI|DMI ϵͳ������", "*", tval, MAX_PATH, filename);
		StartRoot["wbRegisteredOwner"] = tval;

		//ϵͳ�ͺ�	"Haier DT Computer",
		::GetPrivateProfileString("ϵͳ����", "����|��������", "*", tval, MAX_PATH, filename);
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


		//����ϵͳ+�汾	"Windows 10 Pro 1903 18362",
		string wbHostInfo1 = wbOsInfo("ProductName");
		wbHostInfo1 += " ";
		string wbHostInfoVersions1 = wbOsInfo("ReleaseId");
		wbHostInfo1 += wbHostInfoVersions1;

		string wbHostInfoVersions2 = wbOsInfo("CurrentBuild");
		wbHostInfo1 += " ";
		wbHostInfo1 += wbHostInfoVersions2;
		StartRoot["os"] = wbHostInfo1;

		//������	"DESKTOP-DQFMI7A",
		string wbHostName1 = wbHostName();
		StartRoot["wbHostName"] = wbHostName1;

		//ϵͳ��ʼ��װ����	"2019-08-07 10:46:07"
		string wbInstaDate11 = wbOsInfo11("InstallDate");

		//ת��ʱ���
		//const char* wbStr = wbInstaDate11.data();
		StartRoot["wbinitializeDate"] = wbInstaDate11;

		//ϵͳID �豸�к�(BIOS ���к�) "EIH110HE05",

		//sprintf(str1, "��ӡ��%d|��ӡ������|��ӡ������", i);
		::GetPrivateProfileString("ϵͳ����", "DMI|DMI BIOS �汾", "*", tval, MAX_PATH, filename);
		StartRoot["BIOSVersion"] = tval;

		::GetPrivateProfileString("DMI", "ϵͳ|ϵͳ|���к�", "*", tval, MAX_PATH, filename);
		StartRoot["serial_num"] = tval;

		StartRoot["orgCode"] = CONVERT(IDPManager::Config.organization);

		//�Ѱ�װ��Ӧ�ó���
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
//���ػ�����Ԥ�棬������
void CALLBACK Tick_End(HWND hwnd, UINT message, UINT idTimer, DWORD dwTime)
{
	try
	{
		LOG_DAILY("Tick_End Begin");
		//ϵͳ����
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


		//������־�ɼ�
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
		//�Ѱ�װ��Ӧ�ó���
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
		//=============== �������͵����� ===============
		Json::Value RunTimeRoot;
		BUILD_MSG_HEAD(RunTimeRoot);

		RunTimeRoot["wbUptime"] = GlobalwbUptime;

		//��������ʱ��
		RunTimeRoot["wbRunUptime"] = to_string(WinTimeUtils::GetBootRunTimeS());

		//��ȡTCP�����˿�
		RunTimeRoot["wbTCP"] = GetTCPportlist();

		//��ȡUDP�����˿�
		RunTimeRoot["wbUDP"] = GetUDPportlist();

		//��ȡ�����еĽ���
		RunTimeRoot["runPRO"] = runProcess();

		//����ϵͳ+�汾	"Windows 10 Pro 1903 18362",
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
	//=============== �ػ����͵����� ���Ϲػ�����Ԥ������� ===============
	EndRoot["mac"] = IDPManager::Config.Mac;
	//ʹ��ʱ�������
	EndRoot["Endtime"] = to_string(::GetAnsiTime());
	EndRoot["wbUptime"] = GlobalwbUptime;

	//=========================����ͳ��=====================
	auto countMap = IDPManager::User.MonitProcessEnd();
	Json::Value procList;
	//�õ�name�ͼ���
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
			//�յ�����Json�������ݶ�����ȥ
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
			case COMMAND::COMM_COLLALL:				//ȫ����Ϣ�ɼ�
			{
				OnCommandCollAll();
				break;
			}
			case COMMAND::COMM_SCREEN:			//��Ļ��ͼ
			{
				instructScreenCapture(false);
				break;
			}
			case COMMAND::COMM_DISK:					//����	
			{
				SendDiskInfo(false);
				break;
			}
			case COMMAND::COMM_MEMORY:				//�ڴ�
			{
				instructCreateMemory(false);
				break;
			}
			case COMMAND::COMM_MONIT:			//��Ļ��Ϣ
			{
				instructScreenInfo(false);
				break;
			}
			case COMMAND::COMM_PRINTER:				//��ӡ��
			{
				instructPrinter(false);
				break;
			}
			case COMMAND::COMM_KEY_MOUSE:				//�������	
			{
				SendKeyMouse(false);
				break;
			}
			case COMMAND::COMM_DOWNFILE:			//����
			{
				Download(Mess, false, resp.toStyledString());
				break;
			}
			case COMMAND::COMM_HARDWARE:			//Ӳ��
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
		// �����˵�ѡ��: 
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
			//��2
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
			//��9��
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
		//����������һ�βɼ�����
		EndCreate();
		return 1;
	}
	case WM_CLOSE:
		//ȷ���˳�,���ٴ���,�׳�һ��WM_DESTYRY����Ϣ
		LOG_DAILY("Recv Message WM_CLOSE");
		DestroyWindow(hWnd);
		EndCreate();
		break;
	case WM_DESTROY:
	{
		LOG_DAILY("Recv Message WM_DESTROY");
		//������ǰ����
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

// �����ڡ������Ϣ�������
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
		//���Լ���ӵ�����������
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

		//ɾ��������־
		IDPManager::Log.DeleteLogMonth(2);
		//���Կ���ʱ��
		GlobalwbUptime = to_string(::GetAnsiTime());
		//����ʱ����һ�βɼ�����
		StartCreate();

		//�����̲߳ɼ�����  ���ö�ʱ��
		int pantTime = IDPManager::Config.pantTime;
		int aidaTimer = IDPManager::Config.aidaTick;
		int endTimer = IDPManager::Config.endTick;
		//��ʱ��������
		pantTime = pantTime * 1000 * 60;
		SetTimer(NULL, 10, pantTime, Tick_Pant);
		//��ʱˢ��AIDA
		SetTimer(NULL, 110, aidaTimer * 1000 * 60, Tick_AIDA);

		//��ʱˢ�¹ػ���Ϣ���뼶��
		SetTimer(NULL, 11, endTimer * 1000, Tick_End);
		Tick_End(NULL, 0, 0, 0);
		BOOL ret = wbGetMessageFun();

		//ע���¼�
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
		//�����¼����ɼ�һ��
		if (wParam == DBT_DEVNODES_CHANGED)
		{
			if (!IDPManager::User.CompareUSBMap())
			{
				OnEventSend();
			}
		}
		else
		{
			//�����λ�ȡ��Ϣ
				//USB�豸�䶯������Ϣ
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

