#pragma once
#include"GlobalData.h"
#include"../../SystemInfo/SystemInfo/SysInfoUtils.h"
#include"../../SystemInfo/WMI/WMIUtils.h"


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
	stdstr mainboardT;
};
struct IDPPrinter
{
	stdstr name;
	stdstr driver;
	stdstr isDefault;
	stdstr isShare;
	stdstr status;
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
class HardWareUtils
{
public:
	static  BOOL GetIDPDisk(WMIUtils &wmi,iniManager& ini, std::vector<IDPDrive>& vecDrive,
		std::vector<IDPDisk>& vecDisk);
	static BOOL GetIDPCPU(iniManager& ini,std::vector<IDPCPU>& vec);
	static BOOL GetIDPMother(iniManager& ini, std::vector<IDPMother>& vec);
	static BOOL GetKeyboard(iniManager& ini, std::vector<IDPKeyboard>& vec);
	static BOOL GetMouse(iniManager& ini, std::vector<IDPMouse>& vec);
};