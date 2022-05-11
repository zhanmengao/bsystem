#pragma once
#include"../../../File/FileManager/SocketCtrl/SocketCtrlCommon/ThreadBase.h"
#include"THREAD_ID.h"
#include"CollectMessage.h"
#ifdef _WIN32
#include"../../../Process/WindowsProcessHelper/CreateProcess/ProcessMonitWMI.h"
#include"../../SystemInfo/SystemInfo/SysInfoUtils.h"
#include"../../../../Network/Serialization/Json/JsonTools.hpp"
#include"../../../USB/WUSBDevice/USBUtils.h"
#include"../../SystemInfo/WMI/WMIUtils.h"
#include"../../../Setting/Readini/INIManager.hpp"
#else
typedef bool BOOL;
#define FALSE false
#include"../../OSInfo/NetInfo.h"
typedef NetInfo net_info;
#define strMac Mac
#endif
#include<set>
#include"../../../Setting/TableManager/TableManager.h"
#include<json/value.h>
class CollectRoutinue :public ThreadBase
{
private:
	struct IDPMonit
	{
		stdstr brand;
		stdstr imageSize;
		stdstr size;
		stdstr model;
		stdstr SerialNumber;
		stdstr ID;
		stdstr maker;
	};
	struct IDPMemory
	{
		stdstr brand;
		stdstr series;
		stdstr size;
		stdstr memoryMZ;
		stdstr SerialNumber;
	};
	struct IDPDisk
	{
		stdstr name;
		int totalSize = 0;
		int residueSize = 0;
	};
	struct IDPDrive
	{
		stdstr brand;
		stdstr InterfaceType;
		stdstr SerialNumber;
		stdstr rev;
		int totalSize = 0;
	};
	struct IDPCPU
	{
		stdstr brand;
		stdstr series;
		stdstr cpuT;
		stdstr cpuFanR;
		stdstr Tjmax;
		stdstr cpuid;
	};
	struct IDPMother
	{
		stdstr name;
		stdstr mainboardCore;
		stdstr brand;
		stdstr series;
		stdstr busBandwidth;
	};
	struct IDPKeyboard
	{
		stdstr name;
		stdstr type;
		stdstr keyboardInterface;
	};
	struct IDPMouse
	{
		stdstr name;
		stdstr isExistRoller;
		int rollingLine;
	};
	struct IDPUSB
	{
		stdstr Description;
		stdstr BusReportedDeviceDescription;
	};
	struct IDPPrinter
	{
		stdstr Name;
		stdstr Driver;
		stdstr Share;
		stdstr Status;
		stdstr Default;
	};
	struct MsgBox
	{
		std::string Message;
		std::string Title;
	};
public:
	virtual unsigned int GetId() const
	{
		return THREAD_COLLECT;
	}
	virtual unsigned int TickTime() const
	{
		return 500;
	}
	virtual DESTROY_SORT DestroySort() const
	{
		return DESTROY_SORT::DESTROY_IMMED;
	}
public:

    CollectRoutinue()
#ifdef _WIN32
        :mMonit(&mWMI)
#endif
	{
		Awake();
	}
protected:
	virtual void ProcessMsg(MsgBase &rMsg);
	virtual void Awake();
	virtual bool Start();
	virtual bool Tick();
	virtual void Destroy();
private:
	Json::Value OnCommandCollAll();
	Json::Value OnCommandHardware();
	Json::Value OnEventCollect();
	void SendPacket(const Json::Value& rData, int tag, int command,MQ_TYPE target);
private:
	Json::Value instructScreenCapture();							//截图
	Json::Value SendMemoryInfo();							//内存
	Json::Value SendPrinterInfo();										//打印机
	Json::Value SendMonitInfo();								//屏幕信息
	Json::Value SendDiskInfo();
	Json::Value SendCPUInfo();
	Json::Value SendMotherInfo();									//主板
	Json::Value SendKeyMouse();
	Json::Value SendUSB();
	void SendEnd();

	void OnCommandCollStart(const std::string& orgCode);
	void OnCommandCollEnd();
	void ShowMessageBox(const Msg_CommandCollect& rMsg);
	void ShowMessageBox(Json::Value data);
	void SetVersion(Json::Value data);
	void SetOrgcode(Json::Value data);
	Json::Value TickColllect();
	void TickEnd();
private:
#ifdef _WIN32
	int GetWindowScreen2(const stdstr& path);
	BOOL SetImaeTextDate(string filename1);
	int GetEncoderClsid(const WCHAR* format, CLSID* pClsid);
	bool ReadPhotoFile(std::basic_string<TCHAR> strFileName, std::string &strData);
#endif
	BOOL GetIDPDisk(std::vector<CollectRoutinue::IDPDrive>& vecDrive,std::vector<IDPDisk>& vecDisk);
	BOOL GetIDPMemory(std::vector<IDPMemory>& vec);
	BOOL GetIDPCPU(std::vector<IDPCPU>& vec);
	BOOL GetIDPMother(std::vector<IDPMother>& vec);
	BOOL GetKeyboard(std::vector<IDPKeyboard>& vec);
	BOOL GetMouse(std::vector<IDPMouse>& vec);
	BOOL GetMonit(std::vector<IDPMonit>& vec);
	BOOL GetPrinter(std::vector<IDPPrinter>& vec);
	void Download(int instruct, bool ifSortSent, const string& reader);
	bool CompareUSBMap();
private:
	bool IsInvalidAIDAValue(const string& val);
	bool IsMsgWMI(int c);
private:
	void HandleMsg(const Msg_CreateCollect& rMsg);
	void HandleMsg(const Msg_PantCollect& rMsg);
	void HandleMsg(const Msg_CommandCollect& rMsg);
	void HandleMsg(const Msg_EndCache& rMsg);
	void HandleMsg(const Msg_Exit& rMsg);
	void HandleMsg(const Msg_EventCollect& rMsg);
private:
#ifdef _WIN32
	WMIUtils mWMI;
	ProcessMonitWMI mMonit;
    std::map<stdstr, USBDevice> mUsbmap;
    vector<WMIPatchInfo> patchvec;
    DWORD  mSession;
#endif
    net_info	netInfo;
	Json::Value StartRoot;
	Json::Value EndRoot;
	time_t GlobalwbUptime;
	stdstr CPUID;
	time_t mLastMonit = 0;
	TableManager mInvalidVal;
	std::set<int> mWMISet;

};
