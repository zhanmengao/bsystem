#include"IDPEvent.h"
#include"../../../File/FileManager/FileManager/DiskManager.h"
#include"veidp.h"
#include"infoCollect.h"
#include"../../../Error/WErrorUtils/ErrorInfo.h"

enum DISK_TYPE
{
	DISK_INVALID,
	DISK_ATA,
	DISK_NVME,
};
BOOL HardWareUtils::GetIDPDisk(WMIUtils &wmi, 
	iniManager& ini, std::vector<IDPDrive>& vecDrive, std::vector<IDPDisk>& vecDisk)
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

	int invalidCount = 0;
	//ini
	for (int j = 0; j < 10; j++)
	{
		IDPDrive drive;
		DISK_TYPE type = DISK_INVALID;
		char keyName[1024] = { 0 };
		sprintf_s(keyName, _countof(keyName), "ATA%d|ATA 设备信息|序列号", j + 1);
		drive.SerialNumber = ini.ReadString("ATA", keyName, "");
		//有可能不是ATA
		if (!drive.SerialNumber.empty())
		{
			type = DISK_ATA;
		}
		else
		{
			sprintf_s(keyName, _countof(keyName), "ATA%d|NVMe 设备信息|序列号", j + 1);
			drive.SerialNumber = ini.ReadString("ATA", keyName, "");
			if (!drive.SerialNumber.empty())
			{
				type = DISK_NVME;
			}
			else
			{
				type = DISK_INVALID;					//没取到，后续用WMI补充
				invalidCount++;
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
			brand = ini.ReadString("ATA", keyName, "");
			sprintf_s(keyName, _countof(keyName), "ATA%d|ATA 设备信息|设备类型", j + 1);
			InterfaceType = ini.ReadString("ATA", keyName, "");
			sprintf_s(keyName, _countof(keyName), "ATA%d|ATA 设备信息|盘片转速", j + 1);
			rev = ini.ReadString("ATA", keyName, "*");
			sprintf_s(keyName, _countof(keyName), "ATA%d|ATA 设备信息|未格式化容量", j + 1);
			csSize = ini.ReadString("ATA", keyName, "*");

			break;
		}
		case DISK_NVME:
		{
			sprintf_s(keyName, _countof(keyName), "ATA%d|NVMe 设备信息|型号 ID", j + 1);
			brand = ini.ReadString("ATA", keyName, "");
			sprintf_s(keyName, _countof(keyName), "ATA%d|NVMe 设备信息|设备类型", j + 1);
			InterfaceType = ini.ReadString("ATA", keyName);
			sprintf_s(keyName, _countof(keyName), "ATA%d|NVMe 设备信息|盘片转速", j + 1);
			rev = ini.ReadString("ATA", keyName);	
			sprintf_s(keyName, _countof(keyName), "ATA%d|NVMe 设备信息|未格式化容量", j + 1);
			csSize = ini.ReadString("ATA", keyName, "*");
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
	if (invalidCount > 0)
	{
		vector<WMIDisk> wmivec;
		std::string exce;
		if (!wmi.GetDiskInfo(wmivec, exce))
		{
			LOG(std::string("WMI GetDiskInfo Error") + ErrorUtils::ErrorMessage(wmi.GetLastErrorWMI()));
		}
		//两两匹配
		for (auto it = wmivec.begin(); it != wmivec.end(); it++)
		{
			bool bFind = false;
			for (int i = 0; i < vecDrive.size(); i++)
			{
				auto &ini = vecDrive[i];
				if (ini.SerialNumber == it->SerialNumber)
				{
					if (ini.totalSize == 0)
					{
						ini.totalSize = it->csSize / 1024;
					}
					if (ini.brand.empty())
					{
						ini.brand = it->csModel;
					}
					if (ini.InterfaceType.empty())
					{
						ini.InterfaceType = it->csInterfaceType;
					}
					bFind = true;
					break;
				}
			}
			if (!bFind)
			{
				IDPDrive drive;
				drive.brand = it->csModel;
				drive.SerialNumber = it->SerialNumber;
				drive.totalSize = it->csSize / 1024;
				drive.InterfaceType = it->csInterfaceType;
				drive.rev = "Default";
			}
		}
	}

	return TRUE;
}

BOOL HardWareUtils::GetIDPCPU(iniManager& ini, std::vector<IDPCPU>& vec)
{
	//ini
	IDPCPU _cpu;
	char keyName[1024] = { 0 };
	sprintf_s(keyName, _countof(keyName), "CPU 属性|制造商");
	_cpu.brand = ini.ReadString("CPUID", keyName);

	sprintf_s(keyName, _countof(keyName), "CPU 属性|名称");
	_cpu.series = ini.ReadString("CPUID", keyName);

	sprintf_s(keyName, _countof(keyName), "温度|CPU Package");
	_cpu.cpuT = ini.ReadString("传感器", keyName);

	sprintf_s(keyName, _countof(keyName), "冷却风扇|中央处理器(CPU)");
	_cpu.cpuFanR = ini.ReadString("传感器", keyName);

	sprintf_s(keyName, _countof(keyName), "CPU 属性|过热保护温度(Tjmax)");
	_cpu.Tjmax = ini.ReadString("CPUID", keyName);

	//直接从缓存取
	_cpu.cpuid = IDPManager::User.CPUID;

	vec.push_back(_cpu);
	return TRUE;
}

BOOL HardWareUtils::GetIDPMother(iniManager& ini, std::vector<IDPMother>& vec)
{
	//ini
	IDPMother _cpu;
	char keyName[1024] = { 0 };

	sprintf_s(keyName, _countof(keyName), "主板|主板名称");
	_cpu.name = ini.ReadString("系统概述", keyName);

	sprintf_s(keyName, _countof(keyName), "主板|主板芯片组");
	_cpu.mainboardCore = ini.ReadString("系统概述", keyName);

	sprintf_s(keyName, _countof(keyName), "主板|主板|制造商");
	_cpu.brand = ini.ReadString("DMI", keyName);

	sprintf_s(keyName, _countof(keyName), "主板|主板|产品");
	_cpu.series = ini.ReadString("DMI", keyName);

	sprintf_s(keyName, _countof(keyName), "内存总线特性|带宽");
	_cpu.busBandwidth = ini.ReadString("主板", keyName);

	sprintf_s(keyName, _countof(keyName), "温度|主板");
	_cpu.mainboardT = ini.ReadString("传感器", keyName);
	vec.push_back(_cpu);
	return TRUE;
}

BOOL HardWareUtils::GetKeyboard(iniManager& ini, std::vector<IDPKeyboard>& vec)
{
	char keyName[1024] = { 0 };
	for (int i = 0; i < 10; i++)
	{
		sprintf_s(keyName, _countof(keyName), "输入设备%d|键盘信息|键盘名称", (i + 1));
		stdstr str = ini.ReadString("输入设备", keyName, "*");
		if (str != "*")
		{
			IDPKeyboard info;
			info.name = str;

			sprintf_s(keyName, _countof(keyName), "输入设备%d|键盘信息|键盘类型", (i + 1));
			info.type = ini.ReadString("输入设备", keyName, "*");

			sprintf_s(keyName, _countof(keyName), "输入设备%d|键盘信息|键盘界面", (i + 1));
			info.keyboardInterface = ini.ReadString("输入设备", keyName, "*");
			vec.push_back(info);
		}
	}
	return TRUE;
}

BOOL HardWareUtils::GetMouse(iniManager& ini, std::vector<IDPMouse>& vec)
{
	char keyName[1024] = { 0 };
	for (int i = 0; i < 10; i++)
	{
		sprintf_s(keyName, _countof(keyName), "输入设备%d|鼠标信息|鼠标名称", (i + 1));
		stdstr str = ini.ReadString("输入设备", keyName, "*");
		if (str != "*")
		{
			IDPMouse info;
			info.name = str;

			sprintf_s(keyName, _countof(keyName), "输入设备%d|鼠标特性|鼠标滚轮", (i + 1));
			info.isExistRoller = ini.ReadString("输入设备", keyName);

			sprintf_s(keyName, _countof(keyName), "输入设备%d|鼠标信息|滚轮一次滚动行数", (i + 1));
			info.rollingLine = ini.ReadNum("输入设备", keyName);
			vec.push_back(info);
		}
	}
	return TRUE;
}