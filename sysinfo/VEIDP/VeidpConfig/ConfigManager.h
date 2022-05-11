#pragma once
#include"../../../../C++space/StringTools/StringTools/StringHead.h"
#include"../../../../System/Setting/Readini/INIManager.hpp"
#include"../../../../System/File/FileManager/FileManager/PathHelper.hpp"
#include"../../../../System/Process/WindowsProcessHelper/Fork/ForkUtils.h"
struct IPAddr
{
	stdstr IP;
	stdstr Port;
	stdstr Topic;
};
class Config
{
public:
	IPAddr commonInfo;
	IPAddr instructInfo;
	IPAddr wanProduce;
	IPAddr wanConsu;
	int pantTime = 1;
	stdstr orgCode;
public:
	void ReadConfig()
	{
		//先读ini配置文件
		iniManager ini(PathHelper::GetModuleFolder() + _T("config.ini"));
		{
			stdstr ipport = ini.ReadString(_T("commonInfo"), _T("ip"), _T("10.101.243.39:9876"));
			//截断
			std::vector<stdstr> ipvec;
			StrUtils::Split(ipport, ipvec, _T(':'));
			if (ipvec.size() >= 1)
			{
				commonInfo.IP = ipvec[0];
			}
			if (ipvec.size() >= 2)
			{
				commonInfo.Port = ipvec[1];
			}
			commonInfo.Topic = ini.ReadString(_T("commonInfo"), _T("Topic"), _T("idp_service"));

			pantTime = ini.ReadNum(_T("pantTime"), _T("time"), 15);
			orgCode = ini.ReadString(_T("organization"), _T("name"), _T("zxjt"));
		}

		{
			stdstr ipport = ini.ReadString(_T("instructInfo"), _T("ip"), _T("10.101.243.39:9876"));
			//截断
			std::vector<stdstr> ipvec;
			StrUtils::Split(ipport, ipvec, _T(':'));
			if (ipvec.size() >= 1)
			{
				instructInfo.IP = ipvec[0];
			}
			if (ipvec.size() >= 2)
			{
				instructInfo.Port = ipvec[1];
			}
			instructInfo.Topic = ini.ReadString(_T("instructInfo"), _T("Topic"), _T("idp_order"));
		}

		{
			stdstr ipport = ini.ReadString(_T("waninfo"), _T("ProduceIp"), _T("10.101.243.39:9876"));
			//截断
			std::vector<stdstr> ipvec;
			StrUtils::Split(ipport, ipvec, _T(':'));
			if (ipvec.size() >= 1)
			{
				wanProduce.IP = ipvec[0];
			}
			if (ipvec.size() >= 2)
			{
				wanProduce.Port = ipvec[1];
			}
			wanProduce.Topic = ini.ReadString(_T("waninfo"), _T("ProduceTopic"), _T("idp_order"));
		}

		{
			stdstr ipport = ini.ReadString(_T("waninfo"), _T("ConsuIp"), _T("10.101.243.39:9876"));
			//截断
			std::vector<stdstr> ipvec;
			StrUtils::Split(ipport, ipvec, _T(':'));
			if (ipvec.size() >= 1)
			{
				wanConsu.IP = ipvec[0];
			}
			if (ipvec.size() >= 2)
			{
				wanConsu.Port = ipvec[1];
			}
			wanConsu.Topic = ini.ReadString(_T("waninfo"), _T("ConsuTopic"), _T("idp_order"));
		}
	}

	void SaveConfig()
	{
		iniManager ini(PathHelper::GetModuleFolder() + _T("config.ini"));
		TCHAR buf[256] = { 0 };
		{
			_tcscat_s(buf, _countof(buf), commonInfo.IP.c_str());
			_tcscat_s(buf, _countof(buf), _T(":"));
			_tcscat_s(buf, _countof(buf), commonInfo.Port.c_str());
			ini.WriteString(_T("commonInfo"), _T("ip"), buf);

			ini.WriteString(_T("commonInfo"), _T("Topic"), commonInfo.Topic.c_str());
		}


		{
			memset(buf, 0, sizeof(buf));
			_tcscat_s(buf, _countof(buf), instructInfo.IP.c_str());
			_tcscat_s(buf, _countof(buf), _T(":"));
			_tcscat_s(buf, _countof(buf), instructInfo.Port.c_str());
			ini.WriteString(_T("instructInfo"), _T("ip"), buf);

			ini.WriteString(_T("instructInfo"), _T("Topic"), instructInfo.Topic);
		}

		{
			memset(buf, 0, sizeof(buf));
			_tcscat_s(buf, _countof(buf), wanProduce.IP.c_str());
			_tcscat_s(buf, _countof(buf), _T(":"));
			_tcscat_s(buf, _countof(buf), wanProduce.Port.c_str());
			ini.WriteString(_T("waninfo"), _T("ProduceIp"), buf);

			ini.WriteString(_T("waninfo"), _T("ProduceTopic"), wanProduce.Topic);
		}

		{
			memset(buf, 0, sizeof(buf));
			_tcscat_s(buf, _countof(buf), wanConsu.IP.c_str());
			_tcscat_s(buf, _countof(buf), _T(":"));
			_tcscat_s(buf, _countof(buf), wanConsu.Port.c_str());
			ini.WriteString(_T("waninfo"), _T("ConsuIp"), buf);

			ini.WriteString(_T("waninfo"), _T("ConsuTopic"), wanConsu.Topic);
		}
		ini.WriteNum(_T("pantTime"), _T("time"), pantTime);

		ini.WriteString(_T("organization"), _T("name"), orgCode);
	}

	void Destroy()
	{
		//运行脚本
		ForkUtils::ShellExec(PathHelper::GetModuleFolder() + _T("stop.bat"), _T(""), true);
		//退出后直接启动创建服务进程
		ForkUtils::ShellExec(PathHelper::GetModuleFolder() + _T("smve.exe"), _T(""));
		PostQuitMessage(0);
	}
};

