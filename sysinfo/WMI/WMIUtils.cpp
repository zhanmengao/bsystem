#include"WMIUtils.h"
#include"../../../../C++space/Convert/Convert/Convert.h"
#include"WMI.h"
BOOL WMIUtils::GetDiskInfo(vector<WMIDisk> &vec, std::string& exce)
{
	std::unique_lock<std::mutex> lock(mLock);
	if (!m_bConnected)
		return FALSE;
	UINT				uIndex = 0;
	try
	{
		WMIDisk info;
		uIndex = 0;
		if (mConnect.BeginEnumClassObject(_T("Win32_DiskDrive")))
		{
			while (mConnect.MoveNextEnumClassObject())
			{
				info.csInterfaceType = mConnect.GetClassObjectStringValue(_T("InterfaceType"));

				info.csManufacturer = mConnect.GetClassObjectStringValue(_T("Manufacturer"));


				info.csModel = mConnect.GetClassObjectStringValue(_T("Model"));

				info.csDescription = mConnect.GetClassObjectStringValue(_T("Description"));

				info.csName = mConnect.GetClassObjectStringValue(_T("Name"));

				info.SerialNumber = mConnect.GetClassObjectStringValue(_T("SerialNumber"));
				info.SerialNumber = StrUtils::Trim(info.SerialNumber);

				info.csSize = mConnect.GetClassObjectU64Value(_T("Size")) / ONE_MEGABYTE;

				info.Caption = mConnect.GetClassObjectStringValue(_T("Caption"));

				info.BytesPerSector = mConnect.GetClassObjectU64Value(_T("BytesPerSector"));

				info.Index = mConnect.GetClassObjectU64Value(_T("Index"));

				vec.push_back(info);
				uIndex++;
			}
			mConnect.CloseEnumClassObject();
			return TRUE;
		}
		return FALSE;
	}
	catch (exception& e)
	{
		exce = e.what();
		return FALSE;
	}
	return FALSE;
}
BOOL WMIUtils::GetMemorySlots(vector<WMIMemory> &vec, std::string& exce)
{
	std::unique_lock<std::mutex> lock(mLock);
	UINT				uIndex = 0;
	DWORD			dwType = 0;
	stdstr		csBuffer;
	try
	{
		if (mConnect.BeginEnumClassObject(_T("Win32_PhysicalMemory")))
		{
			int i = 0;
			while (mConnect.MoveNextEnumClassObject())
			{
				WMIMemory info;
				ZeroMemory((void*)&info, sizeof(info));

				csBuffer = mConnect.GetClassObjectStringValue(_T("Name"));
				_tcscpy_s(info.csName, _countof(info.csName), csBuffer.c_str());

				csBuffer = mConnect.GetClassObjectStringValue(_T("CreationClassName"));
				_tcscpy_s(info.CreationClassName, _countof(info.CreationClassName), csBuffer.c_str());

				csBuffer = mConnect.GetClassObjectStringValue(_T("Description"));
				_tcscpy_s(info.csDescription, _countof(info.csDescription), csBuffer.c_str());

				stdstr csBuffer = mConnect.GetClassObjectStringValue(_T("DeviceLocator"));
				_tcscpy_s(info.csDeviceLocator, _countof(info.csDeviceLocator), csBuffer.c_str());

				csBuffer = mConnect.GetClassObjectStringValue(_T("SerialNumber"));
				_tcscpy_s(info.csSerialNum, _countof(info.csSerialNum), csBuffer.c_str());

				csBuffer = mConnect.GetClassObjectStringValue(_T("Manufacturer"));
				_tcscpy_s(info.Manufacturer, _countof(info.Manufacturer), csBuffer.c_str());

				info.BankLabel = mConnect.GetClassObjectStringValue(_T("BankLabel"));

				info.PartNumber = mConnect.GetClassObjectStringValue(_T("PartNumber"));

				info.MinVoltage = mConnect.GetClassObjectU64Value(_T("MinVoltage"));
				info.MaxVoltage = mConnect.GetClassObjectU64Value(_T("MaxVoltage"));
				info.TotalWidth = mConnect.GetClassObjectU64Value(_T("TotalWidth"));

				info.csCapacity = mConnect.GetClassObjectU64Value(_T("Capacity"));

				info.mSpeed = mConnect.GetClassObjectU64Value(_T("Speed"));
				info.ConfiguredClockSpeed = mConnect.GetClassObjectU64Value(_T("ConfiguredClockSpeed"));

				if (info.mSpeed >= 133 && info.mSpeed <= 266)
				{
					_tcscpy_s(info.csType, _countof(info.csType), _T("DDR"));
				}
				else if (info.mSpeed >= 400 && info.mSpeed <= 800)
				{
					_tcscpy_s(info.csType, _countof(info.csType), _T("DDR2"));
				}
				else if (info.mSpeed >= 1066 && info.mSpeed <= 1600)
				{
					_tcscpy_s(info.csType, _countof(info.csType), _T("DDR3"));
				}
				else if (info.mSpeed >= 1866 && info.mSpeed <= 3200)
				{
					_tcscpy_s(info.csType, _countof(info.csType), _T("DDR4"));
				}
				info.uSlotNumber = ++uIndex;


				vec.push_back(info);
			}
			return TRUE;
		}
		return FALSE;
	}
	catch (exception& e)
	{
		exce = e.what();
		return FALSE;
	}
}

BOOL WMIUtils::GetKeyBoardInfo(vector<WMIKeyboard> &vec, std::string& exce)
{
	std::unique_lock<std::mutex> lock(mLock);
	try
	{
		UINT			uIndex = 0;
		WMIKeyboard	info;
		stdstr			csBuffer;
		string ansiStr;
		DWORD ansiStrlen;

		if (mConnect.BeginEnumClassObject(_T("Win32_Keyboard")))
		{
			while (mConnect.MoveNextEnumClassObject())
			{
				ZeroMemory((void*)&info, sizeof(info));

				csBuffer = mConnect.GetClassObjectStringValue(_T("Caption"));
				_tcscpy_s(info.csCaption, _countof(info.csCaption), csBuffer.c_str());

				csBuffer = mConnect.GetClassObjectStringValue(_T("Description"));
				_tcscpy_s(info.csDescription, _countof(info.csDescription), csBuffer.c_str());

				csBuffer = mConnect.GetClassObjectStringValue(_T("Name"));
				_tcscpy_s(info.csName, _countof(info.csName), csBuffer.c_str());

				csBuffer = mConnect.GetClassObjectStringValue(_T("DeviceID"));
				_tcscpy_s(info.csDeviceID, _countof(info.csDeviceID), csBuffer.c_str());

				csBuffer = mConnect.GetClassObjectStringValue(_T("Layout"));
				_tcscpy_s(info.csLayout, _countof(info.csLayout), csBuffer.c_str());

				csBuffer = mConnect.GetClassObjectStringValue(_T("SystemName"));
				_tcscpy_s(info.SystemName, _countof(info.SystemName), csBuffer.c_str());
				vec.push_back(info);
				uIndex++;
			}
			mConnect.CloseEnumClassObject();
			return TRUE;
		}

		return FALSE;
	}
	catch (exception& e)
	{
		exce = e.what();
		return FALSE;
	}
	return TRUE;
}

BOOL WMIUtils::GetPointerInfo(vector<WMIPointer> &vec, std::string& exce)
{
	std::unique_lock<std::mutex> lock(mLock);
	try
	{
		UINT			uIndex = 0;
		WMIPointer	info;
		stdstr			csBuffer;
		string ansiStr;
		DWORD ansiStrlen;

		if (mConnect.BeginEnumClassObject(_T("Win32_PointingDevice")))
		{
			while (mConnect.MoveNextEnumClassObject())
			{
				ZeroMemory((void*)&info, sizeof(info));

				csBuffer = mConnect.GetClassObjectStringValue(_T("Caption"));
				_tcscpy_s(info.csCaption, _countof(info.csCaption), csBuffer.c_str());

				csBuffer = mConnect.GetClassObjectStringValue(_T("Description"));
				_tcscpy_s(info.csDescription, _countof(info.csDescription), csBuffer.c_str());

				csBuffer = mConnect.GetClassObjectStringValue(_T("Name"));
				_tcscpy_s(info.csName, _countof(info.csName), csBuffer.c_str());

				csBuffer = mConnect.GetClassObjectStringValue(_T("HardwareType"));
				_tcscpy_s(info.csHardwareType, _countof(info.csHardwareType), csBuffer.c_str());

				csBuffer = mConnect.GetClassObjectStringValue(_T("Manufacturer"));
				_tcscpy_s(info.csManufacturer, _countof(info.csManufacturer), csBuffer.c_str());

				csBuffer = mConnect.GetClassObjectStringValue(_T("DeviceID"));
				_tcscpy_s(info.csDeviceID, _countof(info.csDeviceID), csBuffer.c_str());

				csBuffer = mConnect.GetClassObjectStringValue(_T("SystemName"));
				_tcscpy_s(info.SystemName, _countof(info.SystemName), csBuffer.c_str());

				info.DeviceInterface = mConnect.GetClassObjectU64Value(_T("DeviceInterface"));

				info.dwType = mConnect.GetClassObjectDwordValue(_T("PointingType"));
				
				vec.push_back(info);
				uIndex++;
			}
			mConnect.CloseEnumClassObject();
			return TRUE;
		}

		return FALSE;
	}
	catch (exception& e)
	{
		exce = e.what();
		return FALSE;
	}
}

BOOL WMIUtils::GetPrinterInfo(vector<WMIPrint>& vec, std::string& exce)
{
	std::unique_lock<std::mutex> lock(mLock);
	try
	{
		UINT		uIndex = 0;
		WMIPrint	info;
		stdstr		csBuffer, csPortName;
		string ansiStr;
		DWORD ansiStrlen;

		if (mConnect.BeginEnumClassObject(_T("Win32_Printer")))
		{
			while (mConnect.MoveNextEnumClassObject())
			{
				ZeroMemory((void*)&info, sizeof(info));
				info.csName = mConnect.GetClassObjectStringValue(_T("Name"));

				info.csDeviceName = mConnect.GetClassObjectStringValue(_T("DriverName"));


				info.csPortName = mConnect.GetClassObjectStringValue(_T("PortName"));

				info.PrintProcessor = mConnect.GetClassObjectStringValue(_T("PrintProcessor"));

				info.Default = mConnect.GetBool(_T("Default"));

				info.Shared = mConnect.GetBool(_T("Shared"));
				info.WorkOffline = mConnect.GetBool(_T("WorkOffline"));

				info.Network = mConnect.GetBool(_T("Network"));
				info.Local = mConnect.GetBool(_T("Local"));

				info.Hidden = mConnect.GetBool(_T("Hidden"));

				info.Status = mConnect.GetClassObjectStringValue(_T("Status"));

				info.SystemName = mConnect.GetClassObjectStringValue(_T("SystemName"));
				vec.push_back(info);
			}
			mConnect.CloseEnumClassObject();
			return TRUE;
		}
		return FALSE;
	}
	catch (exception& e)
	{
		exce = e.what();
		return FALSE;
	}
}

BOOL WMIUtils::GetDesktop(vector<WMIDesktop>& vec, std::string& exce)
{
	std::unique_lock<std::mutex> lock(mLock);
	try
	{
		UINT		uIndex = 0;
		WMIDesktop 	info;
		CString		csBuffer, csPortName;
		string ansiStr;
		DWORD ansiStrlen;

		if (mConnect.BeginEnumClassObject(_T("Win32_Desktop")))
		{
			while (mConnect.MoveNextEnumClassObject())
			{

				info.szWallpaper = mConnect.GetString(_T("Wallpaper"));

				vec.push_back(info);
			}
			mConnect.CloseEnumClassObject();
			return TRUE;
		}
		return FALSE;
	}
	catch (exception& e)
	{
		exce = e.what();
		return FALSE;
	}
}
std::string WMIUtils::GetDesktopWallpaper(std::string& exce)
{
	try
	{
		std::string picPath;
		vector<WMIDesktop> vec;
		if (!GetDesktop(vec,exce))
		{
			return "";
		}
		else
		{
			TCHAR buffer[MAX_PATH] = { 0 };
			DWORD code = GetEnvironmentVariable(TEXT("USERNAME"), buffer, MAX_PATH);
#ifdef _UNICODE
			std::string User = Convert::WStringToString(buffer);
#else
			std::string User(buffer);
#endif
			for (auto it = vec.begin(); it != vec.end(); it++)
			{
				if (it->szWallpaper.find(User) != std::string::npos)
				{
					picPath = it->szWallpaper;
				}
				if (!picPath.empty())
				{
					continue;
				}
				if (!it->szWallpaper.empty())
				{
					picPath = it->szWallpaper;
				}
			}
		}
		return picPath;
	}
	catch (...)
	{
		return "";
	}
}
BOOL WMIUtils::GetPatch(vector<WMIPatchInfo>& vec,std::string& exce)
{
	std::unique_lock<std::mutex> lock(mLock);
	try
	{
		UINT		uIndex = 0;
		WMIPatchInfo 	info;
		stdstr		csBuffer, csPortName;
		string ansiStr;
		DWORD ansiStrlen;

		if (mConnect.BeginEnumClassObject(_T("Win32_QuickFixEngineering")))
		{
			while (mConnect.MoveNextEnumClassObject())
			{
				csBuffer = mConnect.GetClassObjectStringValue(_T("HotFixID"));
				info.Name = csBuffer;

				csBuffer = mConnect.GetClassObjectStringValue(_T("Description"));
				info.Description = csBuffer;

				std::string str = mConnect.GetString(_T("InstalledOn"));
				sscanf_s(str.c_str(), "%02d/%02d/%04d", &info.installMonth, &info.installDay, &info.installYear);

				vec.push_back(info);
			}
			mConnect.CloseEnumClassObject();
			return TRUE;
		}
		return FALSE;
	}
	catch (exception& e)
	{
		exce = e.what();
		return FALSE;
	}
}
BOOL WMIUtils::GetBIOS(vector<WMIBIOS>& vec, std::string& exce)
{
	std::unique_lock<std::mutex> lock(mLock);
	try
	{
		UINT		uIndex = 0;
		WMIBIOS 	info;
		stdstr		csBuffer, csPortName;
		string ansiStr;
		DWORD ansiStrlen;

		if (mConnect.BeginEnumClassObject(_T("Win32_BIOS")))
		{
			while (mConnect.MoveNextEnumClassObject())
			{
				csBuffer = mConnect.GetClassObjectStringValue(_T("SerialNumber"));
				info.csSerialNum = csBuffer;

				csBuffer = mConnect.GetClassObjectStringValue(_T("Description"));
				info.Description = csBuffer;

				csBuffer = mConnect.GetClassObjectStringValue(_T("Name"));
				info.Name = csBuffer;

				csBuffer = mConnect.GetClassObjectStringValue(_T("Version"));
				info.Version = csBuffer;

				csBuffer = mConnect.GetClassObjectStringValue(_T("SMBIOSBIOSVersion"));
				info.SMBIOSBIOSVersion = csBuffer;

				csBuffer = mConnect.GetClassObjectStringValue(_T("SoftwareElementID"));
				info.SoftwareElementID = csBuffer;

				info.SMBIOSMajorVersion = mConnect.GetClassObjectDwordValue(_T("SMBIOSMajorVersion"));
				info.SMBIOSMinorVersion = mConnect.GetClassObjectDwordValue(_T("SMBIOSMinorVersion"));

				vec.push_back(info);
			}
			mConnect.CloseEnumClassObject();
			return TRUE;
		}
		return FALSE;
	}
	catch (exception& e)
	{
		exce = e.what();
		return FALSE;
	}
}
BOOL WMIUtils::GetMainBoard(vector<WMIMainBoard>& vec, std::string& exce)
{
	std::unique_lock<std::mutex> lock(mLock);
	try
	{
		UINT		uIndex = 0;
		WMIMainBoard 	info;
		stdstr		csBuffer, csPortName;
		string ansiStr;
		DWORD ansiStrlen;

		if (mConnect.BeginEnumClassObject(_T("WIN32_BaseBoard")))
		{
			while (mConnect.MoveNextEnumClassObject())
			{
				csBuffer = mConnect.GetClassObjectStringValue(_T("Description"));
				info.Description = csBuffer;

				csBuffer = mConnect.GetClassObjectStringValue(_T("Manufacturer"));
				info.Manufacturer = csBuffer;

				csBuffer = mConnect.GetClassObjectStringValue(_T("Name"));
				info.Name = csBuffer;

				csBuffer = mConnect.GetClassObjectStringValue(_T("Version"));
				info.Version = csBuffer;

				csBuffer = mConnect.GetClassObjectStringValue(_T("Product"));
				info.Product = csBuffer;

				csBuffer = mConnect.GetClassObjectStringValue(_T("Tag"));
				info.Tag = csBuffer;

				vec.push_back(info);
			}
			mConnect.CloseEnumClassObject();
			return TRUE;
		}
		return FALSE;
	}
	catch (exception& e)
	{
		exce = e.what();
		return FALSE;
	}
}
BOOL WMIUtils::GetCPU(vector<WMICPU>& vec, std::string& exce)
{
	std::unique_lock<std::mutex> lock(mLock);
	try
	{
		UINT		uIndex = 0;
		WMICPU 	info;
		stdstr		csBuffer, csPortName;
		string ansiStr;
		DWORD ansiStrlen;

		if (mConnect.BeginEnumClassObject(_T("Win32_Processor")))
		{
			while (mConnect.MoveNextEnumClassObject())
			{
				info.ProcessorId = mConnect.GetString(_T("ProcessorId"));

				csBuffer = mConnect.GetClassObjectStringValue(_T("Name"));
				info.Name = csBuffer;

				csBuffer = mConnect.GetClassObjectStringValue(_T("Caption"));
				info.Caption = csBuffer;

				csBuffer = mConnect.GetClassObjectStringValue(_T("Manufacturer"));
				info.Manufacturer = csBuffer;

				 info.L2CacheSize = mConnect.GetClassObjectU64Value(_T("L2CacheSize"));
				 info.L3CacheSize = mConnect.GetClassObjectU64Value(_T("L3CacheSize"));
				 info.MaxClockSpeed = mConnect.GetClassObjectU64Value(_T("MaxClockSpeed"));
				 info.CurrentClockSpeed = mConnect.GetClassObjectU64Value(_T("CurrentClockSpeed"));
				 info.ExtClock = mConnect.GetClassObjectU64Value(_T("ExtClock"));
				 info.ThreadCount = mConnect.GetClassObjectU64Value(_T("ThreadCount"));
				 info.NumberOfCores = mConnect.GetClassObjectU64Value(_T("NumberOfCores"));
				vec.push_back(info);
			}
			mConnect.CloseEnumClassObject();
			return TRUE;
		}
		return FALSE;
	}
	catch (exception& e)
	{
		exce = e.what();
		return FALSE;
	}
}
BOOL WMIUtils::GetComputerSystem(vector<WMIComputerSystem>& vec, std::string& exce)
{
	std::unique_lock<std::mutex> lock(mLock);
	try
	{
		UINT		uIndex = 0;
		WMIComputerSystem 	info;
		stdstr		csBuffer, csPortName;
		string ansiStr;
		DWORD ansiStrlen;

		if (mConnect.BeginEnumClassObject(_T("Win32_ComputerSystem")))
		{
			while (mConnect.MoveNextEnumClassObject())
			{
				info.Name = mConnect.GetClassObjectStringValue(_T("Name"));;
				info.Manufacturer = mConnect.GetClassObjectStringValue(_T("Manufacturer"));
				info.Model = mConnect.GetClassObjectStringValue(_T("Model"));
				info.TotalPhysicalMemory = mConnect.GetClassObjectStringValue(_T("TotalPhysicalMemory"));

				vec.push_back(info);
			}
			mConnect.CloseEnumClassObject();
			return TRUE;
		}
		return FALSE;
	}
	catch (exception& e)
	{
		exce = e.what();
		return FALSE;
	}
}
BOOL WMIUtils::GetMonitors(vector<WMIMonitor> &vec, std::string& exce)
{
	std::unique_lock<std::mutex> lock(mLock);
	try
	{
		UINT		uIndex = 0;
		WMIMonitor	info;
		CString		csBuffer;

		if (mConnect.BeginEnumClassObject(_T("Win32_DesktopMonitor")))
		{
			while (mConnect.MoveNextEnumClassObject())
			{
				ZeroMemory((void*)&info, sizeof(info));
				csBuffer = mConnect.GetClassObjectStringValue(_T("PNPDeviceID")).c_str();
				csBuffer = csBuffer.Left(csBuffer.ReverseFind(L'\\'));
				_tcscpy_s(info.csModel, _countof(info.csModel), csBuffer);

				csBuffer = mConnect.GetClassObjectStringValue(_T("MonitorManufacturer")).c_str();
				_tcscpy_s(info.csManufacturer, _countof(info.csManufacturer), csBuffer);

				csBuffer = mConnect.GetClassObjectStringValue(_T("Description")).c_str();
				_tcscpy_s(info.csDescription, _countof(info.csDescription), csBuffer);

				csBuffer = mConnect.GetClassObjectStringValue(_T("Caption")).c_str();
				_tcscpy_s(info.csCaption, _countof(info.csCaption), csBuffer);

				csBuffer = mConnect.GetClassObjectStringValue(_T("MonitorType")).c_str();
				_tcscpy_s(info.csType, _countof(info.csType), csBuffer);
				vec.push_back(info);
				uIndex++;
			}
			mConnect.CloseEnumClassObject();
			return TRUE;
		}
		return FALSE;
	}
	catch (exception& e)
	{
		exce = e.what();
		return FALSE;
	}
}

BOOL WMIUtils::GetProcess(std::map<DWORD, WMIProcess>& promap, std::string& exce)
{
	std::unique_lock<std::mutex> lock(mLock);
	try
	{
		UINT		uIndex = 0;
		WMIProcess 	info;

		if (mConnect.BeginEnumClassObject(_T("Win32_Process")))
		{
			while (mConnect.MoveNextEnumClassObject())
			{
				info.Caption = mConnect.GetClassObjectStringValue(_T("Caption"));

				info.Name = mConnect.GetClassObjectStringValue(_T("Name"));

				info.CommandLine = mConnect.GetClassObjectStringValue(_T("CommandLine"));

				info.ExecutablePath = mConnect.GetClassObjectStringValue(_T("ExecutablePath"));

				info.ParentProcessId = mConnect.GetClassObjectU64Value(_T("ParentProcessId"));

				info.ProcessId = mConnect.GetClassObjectU64Value(_T("ProcessId"));

				info.SessionId = mConnect.GetClassObjectU64Value(_T("SessionId"));
				promap.insert(std::make_pair(info.ProcessId,info));
			}
			mConnect.CloseEnumClassObject();
			return TRUE;
		}
		return FALSE;
	}
	catch (exception& e)
	{
		exce = e.what();
		return FALSE;
	}
}
BOOL WMIUtils::Init()
{
	return mConnect.Init();
}
BOOL WMIUtils::Begin()
{
	std::unique_lock<std::mutex> lock(mLock);
	if (m_bConnected)
	{
		return TRUE;
	}
	//HRESULT m_hResult = CoInitializeEx(0, COINIT_APARTMENTTHREADED);

	m_bConnected = mConnect.ConnectWMI(_bstr_t(L"ROOT\\CIMV2"));
	return m_bConnected;
}
BOOL WMIUtils::End()
{
	std::unique_lock<std::mutex> lock(mLock);
	if (m_bConnected)
	{
		mConnect.DisconnectWMI();
	}
	m_bConnected = FALSE;
	return TRUE;
}
void WMIUtils::Destroy()
{
	CoUninitialize();
}
HRESULT WMIUtils::GetLastErrorWMI()
{
	return mConnect.GetLastErrorWMI();
}
