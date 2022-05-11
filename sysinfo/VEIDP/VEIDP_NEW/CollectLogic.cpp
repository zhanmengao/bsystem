#include"stdafx.h"
#include"CollectRoutinue.h"
#include"../../SystemInfo/SystemInfo/SysInfoUtils.h"
#include"../../../../Network/SocketEncode/SocketEncode/base64.h"
#include"../../../Setting/WDesktop/LnkUtils.h"
#include<direct.h>
#include"../../../File/FileManager/FileManager/DiskManager.h"
#include <windows.h>
#include <urlmon.h>
#include <string>
#include <vector>
#include <algorithm>
#include"../../../Process/WindowsProcessHelper/Fork/ForkUtils.h"
#include"../../SystemInfo/HardwareUtils/MonitUtils.h"
#pragma comment(lib, "urlmon.lib")
#include "Wincrypt.h"
enum DISK_TYPE
{
	DISK_INVALID,
	DISK_ATA,
	DISK_NVME,
};
BOOL CollectRoutinue::GetIDPDisk(std::vector<IDPDrive>& vecDrive, std::vector<IDPDisk>& vecDisk)
{
	//Windows API
	auto diskVec = DiskTools::GetDiskInfo();
	for (auto it = diskVec.begin(); it != diskVec.end(); it++)
	{
		IDPDisk disk;
		disk.totalSize = it->TotalSize / 1024 / 1024 / 1024;
		disk.residueSize = it->freeSize / 1024 / 1024 / 1024;
		disk.name = it->diskName;
		vecDisk.push_back(disk);
	}

	//ini
	for (int j = 0; j < 10; j++)
	{
		IDPDrive drive;
		DISK_TYPE type = DISK_INVALID;
		char keyName[1024] = { 0 };
		sprintf_s(keyName, _countof(keyName), "ATA%d|ATA 设备信息|序列号", j + 1);
		drive.SerialNumber = mINIHW.ReadString("ATA", keyName, "");
		//有可能不是ATA
		if (!drive.SerialNumber.empty())
		{
			type = DISK_ATA;
		}
		else
		{
			sprintf_s(keyName, _countof(keyName), "ATA%d|NVMe 设备信息|序列号", j + 1);
			drive.SerialNumber = mINIHW.ReadString("ATA", keyName, "");
			if (!drive.SerialNumber.empty())
			{
				type = DISK_NVME;
			}
			else
			{
				type = DISK_INVALID;					//没取到，后续用WMI补充
			}
		}
		stdstr brand;
		stdstr InterfaceType;
		stdstr csSize;
		stdstr rev;
		switch (type)
		{
		case DISK_ATA:
		{
			sprintf_s(keyName, _countof(keyName), "ATA%d|ATA 设备信息|型号 ID", j + 1);
			brand = mINIHW.ReadString("ATA", keyName, "");
			sprintf_s(keyName, _countof(keyName), "ATA%d|ATA 设备信息|设备类型", j + 1);
			InterfaceType = mINIHW.ReadString("ATA", keyName, "");
			sprintf_s(keyName, _countof(keyName), "ATA%d|ATA 设备信息|盘片转速", j + 1);
			rev = mINIHW.ReadString("ATA", keyName, "*");
			sprintf_s(keyName, _countof(keyName), "ATA%d|ATA 设备信息|未格式化容量", j + 1);
			csSize = mINIHW.ReadString("ATA", keyName, "*");

			break;
		}
		case DISK_NVME:
		{
			sprintf_s(keyName, _countof(keyName), "ATA%d|NVMe 设备信息|型号 ID", j + 1);
			brand = mINIHW.ReadString("ATA", keyName, "");
			sprintf_s(keyName, _countof(keyName), "ATA%d|NVMe 设备信息|设备类型", j + 1);
			InterfaceType = mINIHW.ReadString("ATA", keyName);
			sprintf_s(keyName, _countof(keyName), "ATA%d|NVMe 设备信息|盘片转速", j + 1);
			rev = mINIHW.ReadString("ATA", keyName);
			sprintf_s(keyName, _countof(keyName), "ATA%d|NVMe 设备信息|未格式化容量", j + 1);
			csSize = mINIHW.ReadString("ATA", keyName, "*");
			break;
		}
		}

		drive.brand = brand;
		drive.rev = rev;
		drive.InterfaceType = InterfaceType;
		//大小，去除最后三位（ MB）然后/1024
		size_t pos = csSize.find(" MB");
		if (pos != csSize.npos)
		{
			csSize = csSize.substr(0, pos);
		}
		drive.totalSize = atoi(csSize.c_str()) / 1024;

		if (!drive.SerialNumber.empty())
		{
			vecDrive.push_back(drive);
		}
	}

	//WMI 补充 AIDA中未识别的设备
	vector<WMIDisk> wmivec;
	std::string exce;
	if (mWMI.Begin() && mWMI.GetDiskInfo(wmivec, exce))
	{
		//两两匹配
		for (auto it = wmivec.begin(); it != wmivec.end(); it++)
		{
			mWMI.End();
			bool bFind = false;
			stdstr SerialNumber =  StrUtils::Toupper(StrUtils::Trim(it->SerialNumber));
			stdstr ID = StrUtils::Toupper(StrUtils::Trim(it->csModel));
			for (int i = 0; i < vecDrive.size(); i++)
			{
				auto &ini = vecDrive[i];
				bool bMatch = false;

				stdstr aidaSerialNumber = StrUtils::Toupper(StrUtils::Trim(ini.SerialNumber));
				stdstr aidaID = StrUtils::Toupper(StrUtils::Trim(ini.brand));
				//型号ID
				if (aidaID.find(ID)!=stdstr::npos || ID.find(aidaID) != stdstr::npos)
				{
					bMatch = true;
				}
				//序列号有效，一票决定权
				if (!IsInvalidAIDAValue(SerialNumber) && !IsInvalidAIDAValue(aidaSerialNumber))
				{
					if (SerialNumber == aidaSerialNumber)
					{
						bMatch = true;
					}
					else
					{
						bMatch = false;
					}
				}
				if(bMatch)
				{
					if (ini.totalSize == 0)
					{
						ini.totalSize = it->csSize / 1024;
					}
					if (IsInvalidAIDAValue(ini.brand))
					{
						ini.brand = it->csModel;
					}
					if (IsInvalidAIDAValue(ini.InterfaceType))
					{
						ini.InterfaceType = it->csInterfaceType;
					}
					if (IsInvalidAIDAValue(ini.SerialNumber))
					{
						ini.SerialNumber = it->SerialNumber;
					}
					bFind = true;
					break;
				}
			}
			if (!bFind && vecDrive.size() < wmivec.size())
			{
				IDPDrive drive;
				drive.brand = it->csModel;
				drive.SerialNumber = it->SerialNumber;
				drive.totalSize = it->csSize / 1024;
				drive.InterfaceType = it->csInterfaceType;
				drive.rev = "*";
				vecDrive.push_back(drive);
			}
		}
	}
	else
	{
		LOG(std::string("WMI GetDiskInfo Error") + ErrorUtils::ErrorMessage(mWMI.GetLastErrorWMI()) + exce);
	}


	return TRUE;
}
BOOL CollectRoutinue::GetIDPMemory(std::vector<IDPMemory>& vec)
{
	for (int i = 0; i < 10; i++)
	{
		char str[1024] = { 0 };
		sprintf(str, "内存设备%d|内存设备|大小", (i + 1));
		stdstr size = mINIHW.ReadString("DMI", str, "*");
		if (size != "*")
		{
			IDPMemory info;
			sprintf(str, "内存设备%d|内存设备|制造商", (i + 1));
			info.brand = mINIHW.ReadString("DMI", str, "*");

			sprintf(str, "内存设备%d|内存设备|类型", (i + 1));
			info.series = mINIHW.ReadString("DMI", str, "*");

			sprintf(str, "内存设备%d|内存设备|大小", (i + 1));
			info.size = mINIHW.ReadString("DMI", str, "*");

			sprintf(str, "内存设备%d|内存设备|最大时钟频率", (i + 1));
			info.memoryMZ = mINIHW.ReadString("DMI", str, "*");

			sprintf(str, "内存设备%d|内存设备|序列号", (i + 1));
			info.SerialNumber = mINIHW.ReadString("DMI", str, "*");

			sprintf(str, "内存设备%d|内存设备|设备定位", (i + 1));
			info.Channel = mINIHW.ReadString("DMI", str, "*");

			sprintf(str, "内存设备%d|内存设备|Bank 定位", (i + 1));
			info.Bank = mINIHW.ReadString("DMI", str, "*");
			vec.push_back(info);
		}
	}

	//用WMI修缮
	vector<WMIMemory> wmivec;
	std::string exce;
	if (mWMI.Begin() && mWMI.GetMemorySlots(wmivec, exce))
	{
		//两两匹配
		for (auto it = wmivec.begin(); it != wmivec.end(); it++)
		{
			//用BANK定位 和 设备定位匹配
			stdstr BankLabel = StrUtils::Trim(StrUtils::Toupper(it->BankLabel));
			stdstr DeviceLocator = StrUtils::Trim(StrUtils::Toupper(it->csDeviceLocator));
			stdstr seri = StrUtils::Trim(StrUtils::Toupper(it->csSerialNum));
			bool bFind = false;
			for (int i = 0; i < vec.size(); i++)
			{
				auto &ini = vec[i];
				bool bMatch = false;
				{
					stdstr aidaBankLabel = StrUtils::Trim(StrUtils::Toupper(ini.Bank));
					if (!BankLabel.empty() && !aidaBankLabel.empty())
					{
						if (BankLabel.find(aidaBankLabel) != stdstr::npos || aidaBankLabel.find(BankLabel) != stdstr::npos)
						{
							bMatch = true;
						}
					}
				}
				{
					stdstr aidaDeviceLocator = StrUtils::Trim(StrUtils::Toupper(ini.Channel));
					if (!aidaDeviceLocator.empty() && !DeviceLocator.empty())
					{
						if (aidaDeviceLocator.find(DeviceLocator) != stdstr::npos || DeviceLocator.find(aidaDeviceLocator) != stdstr::npos)
						{
							bMatch = true;
						}
					}
				}
				{
					stdstr aidaSerialNumber = StrUtils::Trim(StrUtils::Toupper(ini.SerialNumber));
					if (!aidaSerialNumber.empty() && !seri.empty())
					{
						if (aidaSerialNumber.find(seri) != stdstr::npos || seri.find(aidaSerialNumber) != stdstr::npos)
						{
							bMatch = true;
						}
					}
				}

				//开始修缮
				if (bMatch)
				{
					if (IsInvalidAIDAValue(ini.SerialNumber))
					{
						ini.SerialNumber = it->csSerialNum;
					}
					if (IsInvalidAIDAValue(ini.size))
					{
						ini.size = it->csCapacity / 1024 / 1024 / 1024 + " G";
					}
					if (IsInvalidAIDAValue(ini.brand))
					{
						ini.brand = it->Manufacturer;
					}
					if (IsInvalidAIDAValue(ini.memoryMZ))
					{
						ini.memoryMZ = it->mSpeed + " MT/s";
					}
					if (IsInvalidAIDAValue(ini.series))
					{
						ini.series = it->csType;
					}
					break;
				}
			}
		}
		mWMI.End();
	}
	else
	{
		LOG(std::string("WMI GetMemorySlots Error	") + ErrorUtils::ErrorMessage(mWMI.GetLastErrorWMI()) +"	"+ exce);
	}
	return TRUE;
}
BOOL CollectRoutinue::GetPrinter(std::vector<IDPPrinter>& vec)
{
	for (int i = 0; i < 50; i++)
	{
		char str[1024] = { 0 };
		sprintf(str, "打印机%d|打印机属性|打印机名称", (i + 1));
		stdstr name = mINIHW.ReadString("打印机", str, "*");
		if (name != "*")
		{
			IDPPrinter info;
			info.Name = name;

			sprintf(str, "打印机%d|打印机属性|默认打印机", (i + 1));
			info.Default = mINIHW.ReadString("打印机", str, "*");

			sprintf(str, "打印机%d|打印机属性|打印机驱动程序", (i + 1));
			info.Driver = mINIHW.ReadString("打印机", str, "*");

			sprintf(str, "打印机%d|打印机属性|共享", (i + 1));
			info.Share = mINIHW.ReadString("打印机", str, "*");

			sprintf(str, "打印机%d|打印机属性|状态", (i + 1));
			info.Status = mINIHW.ReadString("打印机", str, "*");
			vec.push_back(info);
		}
	}

	//WMI 修缮
	vector<WMIPrint> wmivec;
	std::string error;
	if (mWMI.Begin() && mWMI.GetPrinterInfo(wmivec, error))
	{
		for (auto it = vec.begin(); it != vec.end(); it++)
		{
			for (int i = 0; i < wmivec.size(); i++)
			{
				auto& info = wmivec[i];
				//匹配
				if (it->Name == info.csName)
				{
					if (IsInvalidAIDAValue(it->Default))
					{
						if (info.Default)
						{
							it->Default = "是";
						}
						else
						{
							it->Default = "否";
						}
					}
					if (IsInvalidAIDAValue(it->Share))
					{
						if (info.Shared)
						{
							it->Share = "共享";
						}
						else
						{
							it->Share = "未共享";
						}
					}
					if (IsInvalidAIDAValue(it->Status))
					{
						it->Status = info.Status;
					}
					if (IsInvalidAIDAValue(it->Driver))
					{
						it->Driver = info.csDeviceName;
					}
					break;
				}
			}
		}
		mWMI.End();
	}
	else
	{
		LOG(std::string("WMI GetPrinterInfo Error") + ErrorUtils::ErrorMessage(mWMI.GetLastErrorWMI()) + "	" + error);
	}
	return TRUE;
}
BOOL CollectRoutinue::GetIDPCPU(std::vector<IDPCPU>& vec)
{
	//ini
	IDPCPU _cpu;
	char keyName[1024] = { 0 };
	sprintf_s(keyName, _countof(keyName), "CPU 属性|制造商");
	_cpu.brand = mINIHW.ReadString("CPUID", keyName);

	sprintf_s(keyName, _countof(keyName), "CPU 属性|名称");
	_cpu.series = mINIHW.ReadString("CPUID", keyName);

	sprintf_s(keyName, _countof(keyName), "CPU 属性|过热保护温度(Tjmax)");
	_cpu.Tjmax = mINIHW.ReadString("CPUID", keyName);

	sprintf_s(keyName, _countof(keyName), "温度|CPU Package");
	_cpu.cpuT = mINIHW.ReadString("传感器", keyName);
	if (IsInvalidAIDAValue(_cpu.cpuT))
	{
		sprintf_s(keyName, _countof(keyName), "温度|中央处理器(CPU)");
		_cpu.cpuT = mINIHW.ReadString("传感器", keyName);
	}

	sprintf_s(keyName, _countof(keyName), "冷却风扇|中央处理器(CPU)", "*");
	_cpu.cpuFanR = mINIHW.ReadString("传感器", keyName);
	//直接从缓存取
	_cpu.cpuid = CPUID;
	std::string error;
	vector<WMICPU> wmivec;
	if (mWMI.Begin() && mWMI.GetCPU(wmivec, error))
	{
		for (auto it = wmivec.begin(); it != wmivec.end(); it++)
		{
			if (IsInvalidAIDAValue(_cpu.brand))
			{
				_cpu.brand = it->Manufacturer;
			}
			if (IsInvalidAIDAValue(_cpu.series))
			{
				_cpu.series = it->Name;
			}
			if (IsInvalidAIDAValue(_cpu.Tjmax))
			{
				_cpu.Tjmax = it->ExtClock + " °C";
			}
		}
	}
	else
	{
		LOG("mWMI.GetCPU Error" + ErrorUtils::ComErrorMessage(mWMI.GetLastErrorWMI()) + "	"+ error);
	}
	vec.push_back(_cpu);
	return TRUE;
}

BOOL CollectRoutinue::GetIDPMother(std::vector<IDPMother>& vec)
{
	IDPMother mother;
	char keyName[1024] = { 0 };

	sprintf_s(keyName, _countof(keyName), "主板|主板名称");
	mother.name = mINIHW.ReadString("系统概述", keyName);

	sprintf_s(keyName, _countof(keyName), "主板|主板芯片组", "*");
	mother.mainboardCore = mINIHW.ReadString("系统概述", keyName);
	if (IsInvalidAIDAValue(mother.mainboardCore))
	{
		mother.mainboardCore = mother.name;

		sprintf_s(keyName, _countof(keyName), "DMI|DMI 主板序列号", "*");
		stdstr series = mINIHW.ReadString("系统概述", keyName);
		if (!IsInvalidAIDAValue(series))
		{
			mother.mainboardCore += " " + series;
		}
	}

	sprintf_s(keyName, _countof(keyName), "主板|主板|制造商", "*");
	mother.brand = mINIHW.ReadString("DMI", keyName);

	sprintf_s(keyName, _countof(keyName), "主板|主板|产品", "*");
	mother.series = mINIHW.ReadString("DMI", keyName);

	sprintf_s(keyName, _countof(keyName), "内存总线特性|带宽", "*");
	mother.busBandwidth = mINIHW.ReadString("主板", keyName);

	//WMI修缮
	if (mWMI.Begin())
	{
		vector<WMIMainBoard> wmivec;
		std::string exce;
		if (mWMI.GetMainBoard(wmivec, exce))
		{
			for (auto it = wmivec.begin(); it != wmivec.end(); it++)
			{
				if (IsInvalidAIDAValue(mother.name))
				{
					mother.name = it->Name;
				}
				if (IsInvalidAIDAValue(mother.brand))
				{
					mother.brand = it->Manufacturer;
				}
			}
		}
		else
		{
			LOG(std::string("WMI GetMainBoard Error") + ErrorUtils::ErrorMessage(mWMI.GetLastErrorWMI()) + exce);
		}
		mWMI.End();
	}

	vec.push_back(mother);
	return TRUE;
}

BOOL CollectRoutinue::GetKeyboard(std::vector<IDPKeyboard>& vec)
{
	char keyName[1024] = { 0 };
	for (int i = 0; i < 10; i++)
	{
		sprintf_s(keyName, _countof(keyName), "输入设备%d|键盘信息|键盘名称", (i + 1));
		stdstr str = mINIHW.ReadString("输入设备", keyName, "*");
		if (str != "*")
		{
			IDPKeyboard info;
			info.name = str;

			sprintf_s(keyName, _countof(keyName), "输入设备%d|键盘信息|键盘类型", (i + 1));
			info.type = mINIHW.ReadString("输入设备", keyName, "*");

			sprintf_s(keyName, _countof(keyName), "输入设备%d|键盘信息|键盘界面", (i + 1));
			info.keyboardInterface = mINIHW.ReadString("输入设备", keyName, "*");
			vec.push_back(info);
		}
	}
	return TRUE;
}

BOOL CollectRoutinue::GetMouse(std::vector<IDPMouse>& vec)
{
	char keyName[1024] = { 0 };
	for (int i = 0; i < 10; i++)
	{
		sprintf_s(keyName, _countof(keyName), "输入设备%d|鼠标信息|鼠标名称", (i + 1));
		stdstr str = mINIHW.ReadString("输入设备", keyName, "*");
		if (str != "*")
		{
			IDPMouse info;
			info.name = str;

			sprintf_s(keyName, _countof(keyName), "输入设备%d|鼠标特性|鼠标滚轮", (i + 1));
			info.isExistRoller = mINIHW.ReadString("输入设备", keyName);

			sprintf_s(keyName, _countof(keyName), "输入设备%d|鼠标信息|滚轮一次滚动行数", (i + 1));
			info.rollingLine = mINIHW.ReadNum("输入设备", keyName);
			vec.push_back(info);
		}
	}
	return TRUE;
}
BOOL CollectRoutinue::GetMonit(std::vector<IDPMonit>& vec)
{
	char keyName[1024] = { 0 };
	for (int i = 0; i < 10; i++)
	{
		sprintf_s(keyName, _countof(keyName), "显示器%d|显示器属性|显示器名称", (i + 1));
		stdstr str = mINIHW.ReadString("显示器", keyName, "*");
		if (!IsInvalidAIDAValue(str))
		{
			IDPMonit info;
			sprintf_s(keyName, _countof(keyName), "显示器%d|显示器属性|显示器名称", (i + 1));
			info.brand = mINIHW.ReadString("显示器", keyName);

			sprintf_s(keyName, _countof(keyName), "显示器%d|显示器属性|最大分辨率", (i + 1));
			info.imageSize = mINIHW.ReadString("显示器", keyName);

			sprintf_s(keyName, _countof(keyName), "显示器%d|显示器属性|最大可用屏幕", (i + 1));
			info.size = mINIHW.ReadString("显示器", keyName);

			sprintf_s(keyName, _countof(keyName), "显示器%d|显示器属性|显示器 ID", (i + 1));
			info.ID = mINIHW.ReadString("显示器", keyName);

			sprintf_s(keyName, _countof(keyName), "显示器%d|显示器属性|型号", (i + 1));
			info.model = mINIHW.ReadString("显示器", keyName);

			sprintf_s(keyName, _countof(keyName), "显示器%d|显示器属性|制造商", (i + 1));
			info.maker = mINIHW.ReadString("显示器", keyName);

			sprintf_s(keyName, _countof(keyName), "显示器%d|显示器属性|序列号", (i + 1));
			info.SerialNumber = mINIHW.ReadString("显示器", keyName);
			vec.push_back(info);
		}
	}
	//EDID接口 修缮
	std::vector<MonitInfo> apivec;
	MonitUtils::GetMonitInfo(apivec);
	//用EDID填充AIDA（序列号可能为空）
	for (auto it = apivec.begin(); it != apivec.end(); it++)
	{
		stdstr name = StrUtils::Trim(StrUtils::Toupper(it->edid.csName));
		stdstr serial = StrUtils::Trim(StrUtils::Toupper(it->edid.csSerialNum));
		stdstr ID = StrUtils::Trim(StrUtils::Toupper(it->edid.csDescription));
		ID = StrUtils::ReplaceAllA(ID, ".", "");
		ID = StrUtils::ReplaceAllA(ID, ",", "");
		bool bFind = false;
		for (int i = 0; i < vec.size(); i++)
		{
			IDPMonit& aida = vec[i];
			stdstr aidaName = StrUtils::Trim(StrUtils::Toupper(aida.brand));
			stdstr aidaSeri = StrUtils::Trim(StrUtils::Toupper(aida.SerialNumber));
			stdstr aidaID = StrUtils::Trim(StrUtils::Toupper(aida.ID));
			aidaID = StrUtils::ReplaceAllA(aidaID, ".", "");
			aidaID = StrUtils::ReplaceAllA(aidaID, ",", "");
			bool bMatch = false;

			//转大写比较字符串
			if (name.find(aidaName) != stdstr::npos || aidaName.find(name) != stdstr::npos)
			{
				bMatch = true;
			}
			//转大写比较字符串
			if (ID.find(aidaID) != stdstr::npos || aidaID.find(ID) != stdstr::npos)
			{
				bMatch = true;
			}
			//有效的序列号不同，直接不认可
			if (!IsInvalidAIDAValue(aidaSeri) && !IsInvalidAIDAValue(serial) )
			{
				if (aidaSeri != serial)
				{
					bMatch = false;
				}
				else
				{
					bMatch = true;
				}
			}
			

			if (bMatch)
			{
				bFind = true;
				//名称修缮
				if (IsInvalidAIDAValue(aida.brand))
				{
					aida.brand = it->edid.csName;
				}
				//序列号修缮
				if (IsInvalidAIDAValue(aida.SerialNumber))
				{
					aida.SerialNumber = it->edid.csSerialNum;
				}
				//型号修缮
				if (IsInvalidAIDAValue(aida.model))
				{
					aida.model = it->edid.csDescription;
				}
				//最大可用屏幕修缮
				if (IsInvalidAIDAValue(aida.size))
				{
					aida.size = it->edid.MonitorSize();
				}
				//最大分辨率修缮
				if (IsInvalidAIDAValue(aida.imageSize))
				{
					aida.imageSize = it->monitor.MaxMonitorMode().Desc();
				}
				//ID
				if (IsInvalidAIDAValue(aida.ID))
				{
					aida.ID = it->edid.csDescription;
				}
				//制造商
				if (IsInvalidAIDAValue(aida.maker))
				{
					aida.maker = it->edid.csManufacturer;
				}
			}
		}

		//如果没有找到，则填充一个新数据
		if (!bFind && vec.size() < apivec.size())
		{
			IDPMonit info;
			info.brand = it->edid.csName;
			info.imageSize = it->monitor.MaxMonitorMode().Desc();
			info.size = it->edid.MonitorSize();
			info.model = it->edid.csDescription;
			info.SerialNumber = it->edid.csSerialNum;
			info.ID = it->edid.csDescription;
			info.maker = it->edid.csManufacturer;
			vec.push_back(info);
		}
	}
	for (auto it = vec.begin(); it != vec.end(); it++)
	{
		auto& info = *it;
		//做修缮
		if (IsInvalidAIDAValue(info.model))
		{
			if (!IsInvalidAIDAValue(info.maker))
			{
				info.model = info.maker;
			}
			if (!IsInvalidAIDAValue(info.ID))
			{
				if (IsInvalidAIDAValue(info.model))
				{
					info.model = info.ID;
				}
				else
				{
					info.model += " " + info.ID;
				}
			}
		}
	}
	return TRUE;
}
bool CollectRoutinue::ReadPhotoFile(std::basic_string<TCHAR> strFileName, std::string &strData)
{
	HANDLE hFile;
	hFile = CreateFile(strFileName.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		return false;
	}

	DWORD dFileSize = GetFileSize(hFile, NULL);
	char * pBuffer = new char[dFileSize + 1];

	if (pBuffer == NULL)
		return false;

	memset(pBuffer, 0, dFileSize);

	DWORD dReadSize(0);
	if (!ReadFile(hFile, pBuffer, dFileSize, &dReadSize, NULL))
	{
		LOG("ReadFile Error" + strFileName);
		delete[]pBuffer;
		CloseHandle(hFile);
		return false;
	}

	CBase64 base64;
	strData = "";
	if (!base64.Encode((const char*)pBuffer, dReadSize, strData))
	{
		LOG("base64.Encode Error");
	}

	delete[] pBuffer;
	CloseHandle(hFile);
	return true;
}

int CollectRoutinue::GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
	UINT  num = 0;
	UINT  size = 0;
	Gdiplus::ImageCodecInfo* pImageCodecInfo = NULL;
	Gdiplus::GetImageEncodersSize(&num, &size);
	if (size == 0)
		return -1;

	pImageCodecInfo = (Gdiplus::ImageCodecInfo*)(malloc(size));
	if (pImageCodecInfo == NULL)
		return -1;

	GetImageEncoders(num, size, pImageCodecInfo);
	for (UINT j = 0; j < num; ++j)
	{
		if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0)
		{
			*pClsid = pImageCodecInfo[j].Clsid;
			free(pImageCodecInfo);
			return j;
		}
	}
	free(pImageCodecInfo);
	return -1;
}

BOOL CollectRoutinue::SetImaeTextDate(string filename1)
{

	time_t now = time(0);
	tm *ltm = localtime(&now);
	/**
	* 在图片上加文字
	* ============================================================================================== */

	//https://docs.microsoft.com/zh-cn/windows/win32/api/gdiplusheaders/nl-gdiplusheaders-image
	//https://docs.microsoft.com/zh-cn/windows/win32/api/gdiplusgraphics/nl-gdiplusgraphics-graphics
	//

	stdstr filename0;
	filename0 = filename1.c_str();

	std::wstring wOldFileName = Convert::StringToWString(filename0);								//原文件名

	Gdiplus::Image image2(wOldFileName.c_str());
	Gdiplus::Graphics imageGraphics(&image2);
	imageGraphics.SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAlias);

	CString wxText;
	char buffer2[MAX_PATH] = { 0 };
	DWORD code = GetEnvironmentVariable("OS", buffer2, MAX_PATH);
	//sprintf(buffer2, "%s\\Screen.jpg", buffer1);

	wxText.Format(_TEXT("%d 年 %.2d 月 %.2d 日 %.2d:%.2d:%.2d\n%s"),
		ltm->tm_year + 1900, ltm->tm_mon + 1, ltm->tm_mday, ltm->tm_hour, ltm->tm_min, ltm->tm_sec, buffer2);

	Gdiplus::FontFamily fontFamily(L"Microsoft YaHei");
	Gdiplus::Font myFont(&fontFamily, 20, Gdiplus::FontStyleRegular, Gdiplus::UnitPoint);
	Gdiplus::SolidBrush blackBrush(Gdiplus::Color(255, 0, 0, 0));
	Gdiplus::PointF school_site((Gdiplus::REAL)10, (Gdiplus::REAL)10);
	Gdiplus::StringFormat format;
	format.SetAlignment(Gdiplus::StringAlignmentNear);

	auto wx = wxText.GetBuffer();
	auto strWrite = Convert::StringToWString(wx);									//要写的字
	wxText.ReleaseBuffer();
	imageGraphics.DrawString(strWrite.c_str(), strWrite.size(), &myFont, school_site, &format, &blackBrush);

	CString newfilename = wOldFileName.c_str();							//新文件名
	newfilename.Replace(".jpg", "_TEXT.jpg");
	CLSID pngClsid;
	GetEncoderClsid(L"image/jpeg", &pngClsid);

	wx = newfilename.GetBuffer();
	std::wstring wNewFileName = Convert::StringToWString(wx);
	newfilename.ReleaseBuffer();

	auto status = image2.Save(wNewFileName.c_str(), &pngClsid, NULL);
	if (status == Gdiplus::Ok)
	{
		LOG_DAILY("Save JPEG OK Path : " + Convert::WStringToString(wNewFileName));
		return TRUE;
	}
	else
	{
		LOG("Save JPEG Error Status : " + to_string(status));
		return FALSE;
	}
	return 0;
}
int CollectRoutinue::GetWindowScreen2(const stdstr& path)
{
	time_t now = time(0);
	tm *ltm = localtime(&now);


	//https://docs.microsoft.com/zh-cn/windows/win32/api/wingdi/
	//https://docs.microsoft.com/en-us/cpp/atl-mfc-shared/reference/cimage-class?view=vs-2015
	//https://docs.microsoft.com/zh-cn/windows/win32/api/wingdi/nf-wingdi-bitblt	

	HDC hdcSrc = ::GetDC(NULL);
	int nBitPerPixel = GetDeviceCaps(hdcSrc, BITSPIXEL);
	int nWidth = GetDeviceCaps(hdcSrc, HORZRES);
	int nHeight = GetDeviceCaps(hdcSrc, VERTRES);

	CImage image;
	image.Create(nWidth, nHeight, nBitPerPixel);

	BitBlt(image.GetDC(), 0, 0, nWidth, nHeight, hdcSrc, 0, 0, SRCCOPY);
	::ReleaseDC(NULL, hdcSrc);

	HRESULT saveJPEG = image.Save(path.c_str(), Gdiplus::ImageFormatJPEG);

	image.ReleaseDC();
	image.Destroy();

	if (S_OK == saveJPEG)
	{
		//图片上加文字戳
		if (SetImaeTextDate(path))
		{
			return 0;
		}
	}
	else
	{
		DWORD error = GetLastError();
		LOG(string("Save Normal JPEG Error ") + path + "  HRESULT : " + to_string(saveJPEG));
		return error;
	}
	return 0;
}

vector<string> split(const string &s, const string &seperator)
{
	vector<string> result;
	typedef string::size_type string_size;
	string_size i = 0;


	/*
	http://192.168.50.35:8080/DataMapManager/veTerminal/veidp.exe
	vector<string> v = split(data_downloadAdd, "/");
	*/
	while (i != s.size())
	{
		//找到字符串中首个不等于分隔符的字母；
		int flag = 0;
		while (i != s.size() && flag == 0)
		{
			flag = 1;
			for (string_size x = 0; x < seperator.size(); ++x)
				if (s[i] == seperator[x])
				{
					++i;
					flag = 0;
					break;
				}
		}

		//找到又一个分隔符，将两个分隔符之间的字符串取出；
		flag = 0;
		string_size j = i;
		while (j != s.size() && flag == 0)
		{
			for (string_size x = 0; x < seperator.size(); ++x)
				if (s[j] == seperator[x])
				{
					flag = 1;
					break;
				}
			if (flag == 0)
				++j;
		}
		if (i != j)
		{
			result.push_back(s.substr(i, j - i));
			i = j;
		}
	}
	return result;
}

/***
* 取环境变量
*/
std::string Get_ENV(char* hzName)
{

	char* hzName_v;
	size_t requiredSize;

	getenv_s(&requiredSize, NULL, 0, hzName);
	if (requiredSize == 0)
	{
		return NULL;
	}

	hzName_v = (char*)malloc(requiredSize * sizeof(wchar_t));
	if (!hzName_v)
	{
		return NULL;
	}
	getenv_s(&requiredSize, hzName_v, requiredSize, hzName);
	std::string ret;
	if (hzName_v)
	{
		ret = hzName_v;
		free(hzName_v);
		hzName_v = NULL;
	}
	return ret;

}
string GetMd5(LPCSTR FileDirectory)
{
	HANDLE hFile = CreateFile(FileDirectory, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, NULL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)																//如果CreateFile调用失败
	{
		cout << "CreateFile go wrong :" << GetLastError() << endl;									//提示CreateFile调用失败，并输出错误号。visual studio中可在“工具”>“错误查找”中利用错误号得到错误信息。
		CloseHandle(hFile);
		return "";
	}
	HCRYPTPROV hProv = NULL;
	if (CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT) == FALSE)       //获得CSP中一个密钥容器的句柄
	{
		cout << "CryptAcquireContext go wrong:" << GetLastError() << endl;
	}
	HCRYPTPROV hHash = NULL;
	if (CryptCreateHash(hProv, CALG_MD5, 0, 0, &hHash) == FALSE)									//初始化对数据流的hash，创建并返回一个与CSP的hash对象相关的句柄。这个句柄接下来将被CryptHashData调用。
	{
		cout << "CryptCreateHash go wrong:" << GetLastError() << endl;
	}
	DWORD dwFileSize = GetFileSize(hFile, 0);		//获取文件的大小
	if (dwFileSize == 0xFFFFFFFF)					//如果获取文件大小失败
	{
		cout << "GetFileSize go wrong:" << GetLastError() << endl;
	}

	BYTE* lpReadFileBuffer = new BYTE[dwFileSize];
	DWORD lpReadNumberOfBytes;
	if (ReadFile(hFile, lpReadFileBuffer, dwFileSize, &lpReadNumberOfBytes, NULL) == 0)			//读取文件
	{
		cout << "ReadFile go wrong:" << GetLastError() << endl;
	}
	if (CryptHashData(hHash, lpReadFileBuffer, lpReadNumberOfBytes, 0) == FALSE)				//hash文件
	{
		cout << "CryptHashData go wrong:" << GetLastError() << endl;
	}

	delete[] lpReadFileBuffer;
	CloseHandle(hFile);								//关闭文件句柄
	BYTE *pbHash;
	DWORD dwHashLen = sizeof(DWORD);

	if (CryptGetHashParam(hHash, HP_HASHVAL, NULL, &dwHashLen, 0))      //我也不知道为什么要先这样调用CryptGetHashParam，这块是参照的msdn       
	{
	}
	else
	{
		cout << "get length wrong" << endl;
	}
	if (pbHash = (BYTE*)malloc(dwHashLen))
	{
	}
	else
	{
		cout << "allocation failed" << endl;
	}

	string md5;

	char firstNum[32] = { 0 };

	if (CryptGetHashParam(hHash, HP_HASHVAL, pbHash, &dwHashLen, 0))            //获得md5值
	{
		for (DWORD i = 0; i < dwHashLen; i++)         //输出md5值
		{
			sprintf_s(firstNum, "%02x", pbHash[i]);
			md5 += firstNum;
			//printf("%02x", pbHash[i]);
		}

		cout << endl;
	}

	//	printf("%s", md5);

	//善后工作
	if (pbHash)
	{
		free(pbHash);
		pbHash = NULL;
	}
	if (CryptDestroyHash(hHash) == FALSE)          //销毁hash对象
	{
		cout << "CryptDestroyHash go wrong:" << GetLastError() << endl;
	}
	if (CryptReleaseContext(hProv, 0) == FALSE)
	{
		cout << "CryptReleaseContext go wrong:" << GetLastError() << endl;
	}
	return md5;
}
BOOL DownloadFiles(const CHAR* url, const CHAR* filename, string &hashCode)
{
	//将大写的都转换成小写
	transform(hashCode.begin(), hashCode.end(), hashCode.begin(), ::tolower);
	string md5 = "";
	//do{
	remove(filename);
	string s = filename;
	HRESULT hr = URLDownloadToFile(NULL, url, filename, 0, 0);
	if (hr == S_OK)
	{
		md5 = GetMd5(s.c_str());

		//将大写的都转换成小写
		transform(md5.begin(), md5.end(), md5.begin(), ::tolower);

		if (md5 == hashCode)
		{
			//MessageBox(0, hashCode.c_str(), "相等文件", 1);
			return TRUE;
		}
		else
		{
			remove(filename);
		}
	}
	else
	{
		remove(filename);
	}
	//} while (md5.c_str() != hashCode.c_str());
	//MessageBox(0, hashCode.c_str(), "文件", 1);
	return TRUE;
}

int HttpDownFile(const  string &strUrl, const  string &filename, string &hashCode)
{
	DownloadFiles(strUrl.c_str(), filename.c_str(), hashCode);
	return TRUE;
}
void CollectRoutinue::Download(int instruct, bool ifSortSent, const string&  strRet)
{
	Json::Reader reader;
	Json::Value  resp;

	if (reader.parse(strRet, resp, false))
	{
		string data_downloadAdd = "";
		string data_hashCode = "";

		Json::Value  r_data = resp["data"];

		data_downloadAdd = r_data["filename"].asString();
		data_hashCode = r_data["md5"].asString();
		string type = r_data["updateType"].asString();

		if (data_downloadAdd.length() != 0)
		{
			//http://192.168.50.35:8080/DataMapManager/veTerminal/veidp.exe   截取exe名字：20180929135321_aaa.exe
			vector<string> v = split(data_downloadAdd, "/");
			string filename = v[v.size() - 1];

			char * strc = new char[strlen(filename.c_str()) + 1];
			strcpy(strc, filename.c_str());

			//从环境名取temp路径 C:\Users\week\AppData\Local\Temp\20180929135321_aaa.exe
			char buffer[128] = { 0 };
			const char* temp = Get_ENV("temp").c_str();
			sprintf(buffer, "%s\\%s", temp, strc);


			if (type == "1")	//1 强制更新
			{
				ForkUtils::KillProcess(buffer);
				if (HttpDownFile(data_downloadAdd, buffer, data_hashCode) == 1)
				{
					WinExec(buffer, SW_SHOW);
				}
			}
			else if (type == "2")//2 建议更新（弹窗 提示）
			{
				int ifupdate = MessageBox(0, TEXT("建议更新到最新版本"), TEXT("提示"), 1);
				if (ifupdate == 1)//点击确定 更新 如果进程在运行 终止
				{
					ForkUtils::KillProcess(buffer);
					if (HttpDownFile(data_downloadAdd, buffer, data_hashCode) == 1)
					{
						WinExec(buffer, SW_SHOW);
					}
				}
				else if (ifupdate == 2)//点击取消 返回
				{
					delete strc;
					return;
				}
			}
			delete strc;
		}
	}
}


//用最新的和他比较
bool CollectRoutinue::CompareUSBMap()
{
	auto current = USBUtils::GetUsbInstanceMap();
	bool bRet = true;
	//大小不同，直接发
	if (current.size() != mUsbmap.size())
	{
		bRet = false;
	}
	else
	{
		for (auto it = current.begin(); it != current.end(); it++)
		{
			auto fit = mUsbmap.find(it->first);
			//在老集合找不到，直接发
			if (fit == mUsbmap.end())
			{
				bRet = false;
				break;
			}
			//和老集合的不同，直接发
			if (fit->second != it->second)
			{
				bRet = false;
				break;
			}
		}
	}

	mUsbmap = current;
	return bRet;
}

