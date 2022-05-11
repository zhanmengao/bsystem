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
		sprintf_s(keyName, _countof(keyName), "ATA%d|ATA �豸��Ϣ|���к�", j + 1);
		drive.SerialNumber = ini.ReadString("ATA", keyName, "");
		//�п��ܲ���ATA
		if (!drive.SerialNumber.empty())
		{
			type = DISK_ATA;
		}
		else
		{
			sprintf_s(keyName, _countof(keyName), "ATA%d|NVMe �豸��Ϣ|���к�", j + 1);
			drive.SerialNumber = ini.ReadString("ATA", keyName, "");
			if (!drive.SerialNumber.empty())
			{
				type = DISK_NVME;
			}
			else
			{
				type = DISK_INVALID;					//ûȡ����������WMI����
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
			sprintf_s(keyName, _countof(keyName), "ATA%d|ATA �豸��Ϣ|�ͺ� ID", j + 1);
			brand = ini.ReadString("ATA", keyName, "");
			sprintf_s(keyName, _countof(keyName), "ATA%d|ATA �豸��Ϣ|�豸����", j + 1);
			InterfaceType = ini.ReadString("ATA", keyName, "");
			sprintf_s(keyName, _countof(keyName), "ATA%d|ATA �豸��Ϣ|��Ƭת��", j + 1);
			rev = ini.ReadString("ATA", keyName, "*");
			sprintf_s(keyName, _countof(keyName), "ATA%d|ATA �豸��Ϣ|δ��ʽ������", j + 1);
			csSize = ini.ReadString("ATA", keyName, "*");

			break;
		}
		case DISK_NVME:
		{
			sprintf_s(keyName, _countof(keyName), "ATA%d|NVMe �豸��Ϣ|�ͺ� ID", j + 1);
			brand = ini.ReadString("ATA", keyName, "");
			sprintf_s(keyName, _countof(keyName), "ATA%d|NVMe �豸��Ϣ|�豸����", j + 1);
			InterfaceType = ini.ReadString("ATA", keyName);
			sprintf_s(keyName, _countof(keyName), "ATA%d|NVMe �豸��Ϣ|��Ƭת��", j + 1);
			rev = ini.ReadString("ATA", keyName);	
			sprintf_s(keyName, _countof(keyName), "ATA%d|NVMe �豸��Ϣ|δ��ʽ������", j + 1);
			csSize = ini.ReadString("ATA", keyName, "*");
			break;
		}
		}

		drive.brand = brand;
		drive.rev = rev;
		drive.InterfaceType = InterfaceType;
		//��С��ȥ�������λ�� MB��Ȼ��/1024
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
	
	//WMI ���� AIDA��δʶ����豸
	if (invalidCount > 0)
	{
		vector<WMIDisk> wmivec;
		std::string exce;
		if (!wmi.GetDiskInfo(wmivec, exce))
		{
			LOG(std::string("WMI GetDiskInfo Error") + ErrorUtils::ErrorMessage(wmi.GetLastErrorWMI()));
		}
		//����ƥ��
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
	sprintf_s(keyName, _countof(keyName), "CPU ����|������");
	_cpu.brand = ini.ReadString("CPUID", keyName);

	sprintf_s(keyName, _countof(keyName), "CPU ����|����");
	_cpu.series = ini.ReadString("CPUID", keyName);

	sprintf_s(keyName, _countof(keyName), "�¶�|CPU Package");
	_cpu.cpuT = ini.ReadString("������", keyName);

	sprintf_s(keyName, _countof(keyName), "��ȴ����|���봦����(CPU)");
	_cpu.cpuFanR = ini.ReadString("������", keyName);

	sprintf_s(keyName, _countof(keyName), "CPU ����|���ȱ����¶�(Tjmax)");
	_cpu.Tjmax = ini.ReadString("CPUID", keyName);

	//ֱ�Ӵӻ���ȡ
	_cpu.cpuid = IDPManager::User.CPUID;

	vec.push_back(_cpu);
	return TRUE;
}

BOOL HardWareUtils::GetIDPMother(iniManager& ini, std::vector<IDPMother>& vec)
{
	//ini
	IDPMother _cpu;
	char keyName[1024] = { 0 };

	sprintf_s(keyName, _countof(keyName), "����|��������");
	_cpu.name = ini.ReadString("ϵͳ����", keyName);

	sprintf_s(keyName, _countof(keyName), "����|����оƬ��");
	_cpu.mainboardCore = ini.ReadString("ϵͳ����", keyName);

	sprintf_s(keyName, _countof(keyName), "����|����|������");
	_cpu.brand = ini.ReadString("DMI", keyName);

	sprintf_s(keyName, _countof(keyName), "����|����|��Ʒ");
	_cpu.series = ini.ReadString("DMI", keyName);

	sprintf_s(keyName, _countof(keyName), "�ڴ���������|����");
	_cpu.busBandwidth = ini.ReadString("����", keyName);

	sprintf_s(keyName, _countof(keyName), "�¶�|����");
	_cpu.mainboardT = ini.ReadString("������", keyName);
	vec.push_back(_cpu);
	return TRUE;
}

BOOL HardWareUtils::GetKeyboard(iniManager& ini, std::vector<IDPKeyboard>& vec)
{
	char keyName[1024] = { 0 };
	for (int i = 0; i < 10; i++)
	{
		sprintf_s(keyName, _countof(keyName), "�����豸%d|������Ϣ|��������", (i + 1));
		stdstr str = ini.ReadString("�����豸", keyName, "*");
		if (str != "*")
		{
			IDPKeyboard info;
			info.name = str;

			sprintf_s(keyName, _countof(keyName), "�����豸%d|������Ϣ|��������", (i + 1));
			info.type = ini.ReadString("�����豸", keyName, "*");

			sprintf_s(keyName, _countof(keyName), "�����豸%d|������Ϣ|���̽���", (i + 1));
			info.keyboardInterface = ini.ReadString("�����豸", keyName, "*");
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
		sprintf_s(keyName, _countof(keyName), "�����豸%d|�����Ϣ|�������", (i + 1));
		stdstr str = ini.ReadString("�����豸", keyName, "*");
		if (str != "*")
		{
			IDPMouse info;
			info.name = str;

			sprintf_s(keyName, _countof(keyName), "�����豸%d|�������|������", (i + 1));
			info.isExistRoller = ini.ReadString("�����豸", keyName);

			sprintf_s(keyName, _countof(keyName), "�����豸%d|�����Ϣ|����һ�ι�������", (i + 1));
			info.rollingLine = ini.ReadNum("�����豸", keyName);
			vec.push_back(info);
		}
	}
	return TRUE;
}