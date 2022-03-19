#include "stdafx.h"
#include <WinDef.h>
#include "GetWmiInfo.h"
#include "defines.h"
#include <WinIoCtl.h>
#include <shlwapi.h>
#include <time.h>
#pragma comment(lib,"shlwapi.lib")
#include <Wtsapi32.h>
#pragma comment(lib, "WtsApi32.lib")

typedef struct _SPD_INFO
{
	char SN[8];
	char Type[20];
	ULONG year;
	ULONG week;
	int Freq;
	char Exist;
} SPD_INFO,*PSPD_INFO;

typedef struct _ALL_SPD_INFO
{
	SPD_INFO spd[4];
} ALL_SPD_INFO,*PALL_SPD_INFO;

#define FILE_DEVICE_READSPD	0x8000
#define READSPD_IOCTL_BASE	0x800
#define CTL_CODE_READSPD(i) CTL_CODE(FILE_DEVICE_READSPD, READSPD_IOCTL_BASE+i, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_READ_SPD	CTL_CODE_READSPD(0)

typedef  ULONG (*pGetSpdInfo)(PALL_SPD_INFO);

inline void StrForSQL( CString &csMyStr)
{
	csMyStr.Replace( _T( "'"), _T( "''"));
	csMyStr.Replace( _T( "\\"), _T( "/"));
	csMyStr.Replace( _T( "\n"), _T( " "));
	csMyStr.TrimLeft();
	csMyStr.TrimRight();
}

GetWmiInfo::GetWmiInfo()
{
	m_bConnected = FALSE;
}

GetWmiInfo::~GetWmiInfo()
{
	Disconnect();
}

BOOL GetWmiInfo::ParseChassisType( CString &csType)
{
	int		nPos = 0;
	CString csResult,
		csTemp,
		csData;

	if (csType.IsEmpty())
	{
		csType = NOT_AVAILABLE;
		return FALSE;
	}
	csData = csType;
	while ((nPos = csData.Find( _T( ";"))) != -1)
	{
		csTemp = csData.Left( nPos);
		if (!csTemp.IsEmpty())
		{
			switch (_ttoi( csTemp))
			{
			case 1: 
				csResult += _T( "Other;");
				break;
			case 2:  
				csResult += _T( "Unknown;");
				break;
			case 3:  
				csResult += _T( "Desktop;");
				break;
			case 4:  
				csResult += _T( "Low Profile Desktop;");
				break;
			case 5:  
				csResult += _T( "Pizza Box;");
				break;
			case 6:  
				csResult += _T( "Mini Tower;");
				break;
			case 7:  
				csResult += _T( "Tower;");
				break;
			case 8:  
				csResult += _T( "Portable;");
				break;
			case 9:  
				csResult += _T( "Laptop;");
				break;
			case 10:  
				csResult += _T( "Notebook;");
				break;
			case 11:  
				csResult += _T( "Hand Held;");
				break;
			case 12:  
				csResult += _T( "Docking Station;");
				break;
			case 13:  
				csResult += _T( "All in One;");
				break;
			case 14:  
				csResult += _T( "Sub Notebook;");
				break;
			case 15:  
				csResult += _T( "Space-Saving;");
				break;
			case 16:  
				csResult += _T( "Lunch Box;");
				break;
			case 17:  
				csResult += _T( "Main System Chassis;");
				break;
			case 18:  
				csResult += _T( "Expansion Chassis;");
				break;
			case 19:  
				csResult += _T( "SubChassis;");
				break;
			case 20:  
				csResult += _T( "Bus Expansion Chassis;");
				break;
			case 21:  
				csResult += _T( "Peripheral Chassis;");
				break;
			case 22:  
				csResult += _T( "Storage Chassis;");
				break;
			case 23:  
				csResult += _T( "Rack Mount Chassis;");
				break;
			case 24:  
				csResult += _T( "Sealed-Case PC;");
				break;
			default:
				csResult += _T( "Unknown (");
				csResult += csTemp;
				csResult += _T( ");");
				break;
			}
		}
		csTemp = csData.Right( nPos+1);
		csData = csTemp;
	}
	if (!csData.IsEmpty())
	{
		switch (_ttoi( csData))
		{
		case 1: 
			csResult += _T( "Other");
			break;
		case 2:  
			csResult += _T( "Unknown");
			break;
		case 3:  
			csResult += _T( "Desktop");
			break;
		case 4:  
			csResult += _T( "Low Profile Desktop");
			break;
		case 5:  
			csResult += _T( "Pizza Box");
			break;
		case 6:  
			csResult += _T( "Mini Tower");
			break;
		case 7:  
			csResult += _T( "Tower");
			break;
		case 8:  
			csResult += _T( "Portable");
			break;
		case 9:  
			csResult += _T( "Laptop");
			break;
		case 10:  
			csResult += _T( "Notebook");
			break;
		case 11:  
			csResult += _T( "Hand Held");
			break;
		case 12:  
			csResult += _T( "Docking Station");
			break;
		case 13:  
			csResult += _T( "All in One");
			break;
		case 14:  
			csResult += _T( "Sub Notebook");
			break;
		case 15:  
			csResult += _T( "Space-Saving");
			break;
		case 16:  
			csResult += _T( "Lunch Box");
			break;
		case 17:  
			csResult += _T( "Main System Chassis");
			break;
		case 18:  
			csResult += _T( "Expansion Chassis");
			break;
		case 19:  
			csResult += _T( "SubChassis");
			break;
		case 20:  
			csResult += _T( "Bus Expansion Chassis");
			break;
		case 21:  
			csResult += _T( "Peripheral Chassis");
			break;
		case 22:  
			csResult += _T( "Storage Chassis");
			break;
		case 23:  
			csResult += _T( "Rack Mount Chassis");
			break;
		case 24:  
			csResult += _T( "Sealed-Case PC");
			break;
		default:
			csResult += _T( "Unknown (");
			csResult += csData;
			csResult += _T( ");");
		}
	}
	csType = csResult;
	return TRUE;
}

string GetWmiInfo::ws_to_ansi(const wstring& wstr)
{
	int len = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, 0, 0, 0, 0);
	if (len == 0) 
		return string("");
	char *ansi = new char[len];
	ansi[len-1] = '\0';
	WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, ansi, len, 0, 0);
	string str(ansi);
	delete[] ansi;
	return str;
}

BOOL GetWmiInfo::Connect( LPCTSTR lpstrDevice)
{
	try
	{
		CString	csCimRoot;

		// Do not connect if already connected
		if (m_bConnected)
			return TRUE;
		// Load  library

		if (lpstrDevice == NULL)
			csCimRoot.Format( _T( "\\\\.\\root\\cimv2"));
		else
			csCimRoot.Format( _T( "\\\\%s\\root\\cimv2"), lpstrDevice);
		if (!m_dllWMI.ConnectWMI( csCimRoot))
		{
			// Unable to connect to WMI => no WMI support
			return FALSE;
		}
		m_bConnected = TRUE;
		return TRUE;
	}
	catch (CException *pEx)
	{
		pEx->Delete();
		return FALSE;
	}
}
BOOL GetWmiInfo::IsWmiConnected()
{
	return m_bConnected;
}
BOOL GetWmiInfo::Disconnect()
{
	if (m_bConnected)
	{
		m_dllWMI.DisconnectWMI();
	}
	m_bConnected = FALSE;
	return TRUE;
}

BOOL GetWmiInfo::GetModems( vector<Modem> &vecMyList)
{
	return TRUE;
}

BOOL GetWmiInfo::GetSystemPorts( vector<SystemPort> &vecMyList)
{
	return TRUE;
}

BOOL GetWmiInfo::GetSystemControllers( vector<SystemController> &vecMyList)
{
	return TRUE;
}

BOOL GetWmiInfo::GetSystemSlots( vector<SystemSlot> &vecMyList)
{
	return TRUE;
}

UINT GetWmiInfo::GetOS( CHAR csName[], CHAR csVersion[], CHAR csComment[], CHAR csDescription[])
{
	if (!m_bConnected)
		return FALSE;
	try
	{
		UINT	uIndex = 0,
			uType = WINDOWS_WORKSTATION;
		CString csType, csBuff;

		if (m_dllWMI.BeginEnumClassObject( _T( "Win32_OperatingSystem")))
		{
			while (m_dllWMI.MoveNextEnumClassObject())
			{
				csBuff = m_dllWMI.GetClassObjectStringValue( _T( "Name"));
				csBuff = csBuff.Left(csBuff.Find(_T("|"),0));
				StrForSQL( csBuff);
				DWORD bufflen = ws_to_ansi((LPCTSTR)csBuff).length() > MAX_PATH ? MAX_PATH : ws_to_ansi((LPCTSTR)csBuff).length();
				memcpy(csName, ws_to_ansi((LPCTSTR)csBuff).c_str(), bufflen);
				
				csBuff = m_dllWMI.GetClassObjectStringValue( _T( "Version"));
				StrForSQL( csBuff);
				bufflen = ws_to_ansi((LPCTSTR)csBuff).length() > 20 ? 20 : ws_to_ansi((LPCTSTR)csBuff).length();
				memcpy(csVersion, ws_to_ansi((LPCTSTR)csBuff).c_str(), bufflen);
				
				csBuff = m_dllWMI.GetClassObjectStringValue( _T( "CSDVersion"));
				StrForSQL( csBuff);
				bufflen = ws_to_ansi((LPCTSTR)csBuff).length() > 20 ? 20 : ws_to_ansi((LPCTSTR)csBuff).length();
				memcpy(csComment, ws_to_ansi((LPCTSTR)csBuff).c_str(), bufflen);
				csType = m_dllWMI.GetClassObjectStringValue( _T( "ProductType"));
				if (!csType.IsEmpty())
				{
					// If this key exists, it can be a workstation (1), a domain controler (2) or a server (3)
					uType = _tcstoul( csType, NULL, 10);
					uType = (uType > 1 ? WINDOWS_SERVER : WINDOWS_WORKSTATION);
				}
				else
				{
					csType = m_dllWMI.GetClassObjectStringValue( _T( "OSProductSuite"));
					if (!csType.IsEmpty())
						// If this key exists, this is a server
						uType = WINDOWS_SERVER;
				}
				csBuff = m_dllWMI.GetClassObjectStringValue( _T( "Description"));
				StrForSQL( csBuff);
				bufflen = ws_to_ansi((LPCTSTR)csBuff).length() > MAX_PATH ? MAX_PATH : ws_to_ansi((LPCTSTR)csBuff).length();
				memcpy(csDescription, ws_to_ansi((LPCTSTR)csBuff).c_str(), bufflen);
				uIndex ++;
			}
			m_dllWMI.CloseEnumClassObject();
		}
		if (uIndex > 0)
		{
			return uType;
		}
		return UNKNOWN_DEVICE;
	}
	catch (CException *pEx)
	{
		pEx->Delete();
		return UNKNOWN_DEVICE;
	}
}

BOOL GetWmiInfo::GetBiosInfo( vector<Biosdll> &vecMyBios)
{
	BOOL	bResult = FALSE;
	CString	csManufacturer = NOT_AVAILABLE,
		csModel = NOT_AVAILABLE;

	if (!m_bConnected)
		return bResult;


	
	Biosdll stBios;
	ZeroMemory((void*)&stBios, sizeof(stBios));
	DWORD bufflen = 0;
	try
	{
		if (m_dllWMI.BeginEnumClassObject( _T( "Win32_BaseBoard")))
		{
			while (m_dllWMI.MoveNextEnumClassObject())
			{
				
				csManufacturer = m_dllWMI.GetClassObjectStringValue( _T( "Manufacturer"));
				csModel = m_dllWMI.GetClassObjectStringValue( _T( "Product"));
				bufflen = ws_to_ansi((LPCTSTR)csManufacturer).length() > sizeof(stBios.csManufacturer) ? sizeof(stBios.csManufacturer) : ws_to_ansi((LPCTSTR)csManufacturer).length();
				memcpy(stBios.csManufacturer,  ws_to_ansi((LPCTSTR)csManufacturer).c_str(), bufflen);
				bufflen = ws_to_ansi((LPCTSTR)csModel).length() > sizeof(stBios.csModel) ? sizeof(stBios.csModel) : ws_to_ansi((LPCTSTR)csModel).length();
				memcpy(stBios.csModel,  ws_to_ansi((LPCTSTR)csModel).c_str(), bufflen);
			}
			m_dllWMI.CloseEnumClassObject();
		}
	}
	catch (CException *pEx)
	{
		pEx->Delete();
	}

	if(csManufacturer.IsEmpty() || csModel.IsEmpty())
	{
		try
		{
			if (m_dllWMI.BeginEnumClassObject( _T( "Win32_BaseBoard")))
			{
				while (m_dllWMI.MoveNextEnumClassObject())
				{
					//ZeroMemory((void*)&stBios, sizeof(stBios));
					// Get Manufacturer 
					csManufacturer = m_dllWMI.GetClassObjectStringValue( _T( "Manufacturer"));
					StrForSQL(csManufacturer);
					// Get Model
					csModel = m_dllWMI.GetClassObjectStringValue( _T( "Product"));
					StrForSQL(csModel);

					bufflen = ws_to_ansi((LPCTSTR)csManufacturer).length() > sizeof(stBios.csManufacturer) ? sizeof(stBios.csManufacturer) : ws_to_ansi((LPCTSTR)csManufacturer).length();
					memcpy(stBios.csManufacturer, ws_to_ansi((LPCTSTR)csManufacturer).c_str(), bufflen);
					bufflen = ws_to_ansi((LPCTSTR)csModel).length() > sizeof(stBios.csModel) ? sizeof(stBios.csModel) : ws_to_ansi((LPCTSTR)csModel).length();
					memcpy(stBios.csModel, ws_to_ansi((LPCTSTR)csModel).c_str(), bufflen);
				}
				m_dllWMI.CloseEnumClassObject();
			}
		}
		catch (CException *pEx)
		{
			pEx->Delete();
			return FALSE;
		}
	}

	vecMyBios.push_back(stBios);
	return TRUE;
}

BOOL GetWmiInfo::GetCPUInfo( vector<CPUdll> &vecMyList)
{
	/****************************
	*
	*鉴于win xp的漏洞，所以此函数无法使用，直接返回
	*http://support.microsoft.com/kb/953955
	*
	*****************************/
	return FALSE;
	/****************************/
	if (!m_bConnected)
		return FALSE;

	CString csBuffer;

	try
	{
		DWORD	dwNumber = 0;

		if (m_dllWMI.BeginEnumClassObject( _T( "Win32_Processor")))
		{
			while (m_dllWMI.MoveNextEnumClassObject())
			{
				CPUdll stCPU;
				ZeroMemory((void*)&stCPU, sizeof(stCPU));
				csBuffer = m_dllWMI.GetClassObjectStringValue( _T( "Name"));
				StrForSQL(csBuffer);
				strcpy_s(stCPU.csName,   sizeof(stCPU.csName), ws_to_ansi((LPCTSTR)csBuffer).c_str());
				csBuffer = m_dllWMI.GetClassObjectStringValue( _T( "MaxClockSpeed"));
				StrForSQL( csBuffer);
				strcpy_s(stCPU.csProcSpeed,  sizeof(stCPU.csProcSpeed), ws_to_ansi((LPCTSTR)csBuffer).c_str());
				csBuffer = m_dllWMI.GetClassObjectStringValue( _T( "Description"));
				StrForSQL(csBuffer);
				strcpy_s(stCPU.csDescription, sizeof(stCPU.csDescription),  ws_to_ansi((LPCTSTR)csBuffer).c_str());
				dwNumber ++;

				vecMyList.push_back(stCPU);
			}
			m_dllWMI.CloseEnumClassObject();
			return TRUE;
		}
		return FALSE;
	}
	catch (CException *pEx)
	{
		pEx->Delete();
		return FALSE;
	}
}

char seed[] = {'1','2','3','4','5','6','7','8','9','10','A','B','C','D','E','F'};
void GetRandomSerialNumber(char* buffer)
{
	char temp[] = "FF2047F5";
	srand((unsigned int)time(NULL));
	int n = 50;
	int w1 = rand()%8;
	int w2 = rand()%8;
	int w3 = rand()%8;
	int c1 = rand()%15;
	int c2 = rand()%15;
	int c3 = rand()%15;
	temp[w1] = seed[c1];
	temp[w2] = seed[c2];
	temp[w3] = seed[c3];
	strcpy(buffer,temp);
}

BOOL GetWmiInfo::GetMemorySlots(vector<MemorySlotdll> &vecMyList)
{
	BOOL bDLLExist = FALSE;
	if (!m_bConnected)
		return FALSE;

	
	wchar_t strCurrent[MAX_PATH];
	::GetModuleFileNameW(NULL, strCurrent, sizeof(strCurrent));
	::PathRemoveFileSpecW(strCurrent);
	lstrcat(strCurrent, L"\\VEMSFUNCDLL.dll");
	HINSTANCE hdll  = LoadLibrary(strCurrent);

	ALL_SPD_INFO AllSpdInfo;
	ZeroMemory(&AllSpdInfo, sizeof(AllSpdInfo));
	long bit7_4[16]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
	double bit3_0[14]={0.0,0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,0.25,0.33,0.66,0.75};

	if(hdll)
	{
		pGetSpdInfo GetSpdInfo;
		GetSpdInfo = (pGetSpdInfo)GetProcAddress(hdll, "GetSpdInfo");
		if(GetSpdInfo)
		{
			bDLLExist = TRUE;
			GetSpdInfo(&AllSpdInfo);
		}

		FreeLibrary(hdll);
	}

	//MessageBox(NULL, L"Enum memory device ...", L"memory", 0);
	unsigned __int64	u64Value = 0;
	UINT				uIndex = 0;
	DWORD			dwType = 0;
	CString		csBuffer;
	try
	{
		//	UINT nbFilled = 0;
		//	DWORD dwValue;
		//	MemorySlotdll			myObject;
		//	uIndex = 0;
		//	
		//	if (m_dllWMI.BeginEnumClassObject( _T( "Win32_PhysicalMemoryArray")))
		//	{
		//		// One Physical Memory Array object may include one or more Physical Memory objects
		//		while (m_dllWMI.MoveNextEnumClassObject())
		//		{
		//			// The value MemoryDevices indicates the number of Physical Memory object in the array
		//			//dwValue = m_dllWMI.GetClassObjectDwordValue( _T( "MemoryDevices"));
		//			//while( nbFilled < dwValue ) {
		//			//	/*myObject.SetCaption("");
		//			//	myObject.SetDescription("");
		//			//	myObject.SetCapacity( "0");
		//			//	myObject.SetSlotNumber( nbFilled+1 );
		//			//	myObject.SetSpeed( "N/A");
		//			//	myObject.SetType( "Empty slot");
		//			//	pMyList->AddTail( myObject);*/
		//			//	nbFilled++;
		//			//}
		//			DWORD dwUse = m_dllWMI.GetClassObjectDwordValue( _T( "Use"));
		//			DWORD dwECC = m_dllWMI.GetClassObjectDwordValue( _T( "MemoryErrorCorrection"));
		//			csBuffer = m_dllWMI.GetClassObjectStringValue( _T( "Description"));
		//			CString csCaption = m_dllWMI.GetClassObjectStringValue( _T( "Caption"));
		//			csCaption = m_dllWMI.GetClassObjectStringValue( _T( "Name"));
		//			csCaption = m_dllWMI.GetClassObjectStringValue( _T( "Manufacturer"));
		//			csCaption = m_dllWMI.GetClassObjectStringValue( _T( "Model"));
		//			csCaption = m_dllWMI.GetClassObjectStringValue( _T( "OtherIdentifyingInfo"));
		//			csCaption = m_dllWMI.GetClassObjectStringValue( _T( "PartNumber"));
		//			csCaption = m_dllWMI.GetClassObjectStringValue( _T( "SerialNumber"));
		//			csCaption = m_dllWMI.GetClassObjectStringValue( _T( "SKU"));
		//			csCaption = m_dllWMI.GetClassObjectStringValue( _T( "Tag"));
		//			csCaption = m_dllWMI.GetClassObjectStringValue( _T( "Version"));

		//			// Each properties of Physical Memory Array have to be set in MemorySlot object 
		//			// corresponding to current array
		//	
		//			//uIndex += dwValue;
		//		}
		//		m_dllWMI.CloseEnumClassObject();
		//	}
		//	return TRUE;
		//}
		//catch (CException *pEx)
		//{
		//	pEx->Delete();
		//	return FALSE;
		//}

		///////////////////////
		if (m_dllWMI.BeginEnumClassObject( _T( "Win32_PhysicalMemory")))
		{
			int i = 0;
			while (m_dllWMI.MoveNextEnumClassObject())
			{
				MemorySlotdll stMemSlot;
				ZeroMemory((void*)&stMemSlot, sizeof(stMemSlot));

				string ansiStr = ws_to_ansi((LPCTSTR)m_dllWMI.GetClassObjectStringValue(_T("Name")));
				DWORD ansiStrlen = ansiStr.length() > sizeof(stMemSlot.csName) ? sizeof(stMemSlot.csName): ansiStr.length();
				memcpy(stMemSlot.csName, ansiStr.c_str(), ansiStrlen);

				ansiStr = ws_to_ansi((LPCTSTR)m_dllWMI.GetClassObjectStringValue(_T("Caption")));
				ansiStrlen = ansiStr.length() > sizeof(stMemSlot.csCaption) ? sizeof(stMemSlot.csCaption) : ansiStr.length();
				memcpy(stMemSlot.csCaption, ansiStr.c_str(), ansiStrlen);

				ansiStr = ws_to_ansi((LPCTSTR)m_dllWMI.GetClassObjectStringValue(_T("Description")));
				ansiStrlen = ansiStr.length() > sizeof(stMemSlot.csDescription) ? sizeof(stMemSlot.csDescription) : ansiStr.length();
				memcpy(stMemSlot.csDescription, ansiStr.c_str(), ansiStrlen);
				//MessageBox(NULL, L"Enum memory device memcpy...", L"memory", 0);

				CString csDeviceLocator = m_dllWMI.GetClassObjectStringValue( _T( "DeviceLocator"));
				if (csDeviceLocator.IsEmpty())
					csDeviceLocator = m_dllWMI.GetClassObjectStringValue( _T( "BankLabel"));

				//MessageBox(NULL, L"Enum memory device memcpy1...", L"memory", 0);
				ansiStr = ws_to_ansi((LPCTSTR)csDeviceLocator);
				ansiStrlen = ansiStr.length() > sizeof(stMemSlot.csDeviceLocator) ? sizeof(stMemSlot.csDeviceLocator) : ansiStr.length();
				memcpy(stMemSlot.csDeviceLocator, ansiStr.c_str(), ansiStrlen);

				//MessageBox(NULL, L"Enum memory device memcpy2...", L"memory", 0);
				u64Value = m_dllWMI.GetClassObjectU64Value(  _T( "Capacity"));
				sprintf_s(stMemSlot.csCapacity, sizeof(stMemSlot.csCapacity), "%I64u", u64Value / ONE_MEGABYTE);

				//MessageBox(NULL, L"Enum memory device memcpy3...", L"memory", 0);
				stMemSlot.uSlotNumber =  uIndex+1;

				//MessageBox(NULL, L"Enum memory device memcpy4...", L"memory", 0);
				if( (csDeviceLocator.Find(L"DIMM0")!=-1) && bDLLExist)
				{
					ZeroMemory(stMemSlot.csSerialNum,10);
					memcpy(stMemSlot.csSerialNum,AllSpdInfo.spd[i].SN,8);
					sprintf_s(stMemSlot.csSpeed, sizeof(stMemSlot.csSpeed),"%d", AllSpdInfo.spd[i].Freq);
					sprintf_s(stMemSlot.csProductDate, sizeof(stMemSlot.csProductDate),"%d年%d周", AllSpdInfo.spd[i].year,AllSpdInfo.spd[i].week);
					memcpy(stMemSlot.csType, AllSpdInfo.spd[i].Type, min(sizeof(stMemSlot.csType), sizeof(AllSpdInfo.spd[i].Type)));
					i++;
				}
				else if((csDeviceLocator.Find(L"DIMM1") != -1) && bDLLExist)
				{
					//MessageBox(NULL, L"Enum memory device memcpy5...", L"memory", 0);
					ZeroMemory(stMemSlot.csSerialNum,10);
					memcpy(stMemSlot.csSerialNum,AllSpdInfo.spd[i].SN,8);
					//MessageBox(NULL, L"Enum memory device memcpy6...", L"memory", 0);
					sprintf_s(stMemSlot.csSpeed, sizeof(stMemSlot.csSpeed), "%d", AllSpdInfo.spd[i].Freq);
					//MessageBoxA(NULL, stMemSlot.csSpeed, "memory", 0);
					sprintf_s(stMemSlot.csProductDate, sizeof(stMemSlot.csProductDate),"%d年%d周", AllSpdInfo.spd[i].year,AllSpdInfo.spd[i].week);
					//MessageBox(NULL, L"Enum memory device memcpy8...", L"memory", 0);
					memcpy(stMemSlot.csType, AllSpdInfo.spd[i].Type, min(sizeof(stMemSlot.csType), sizeof(AllSpdInfo.spd[i].Type)));
					//MessageBox(NULL, L"Enum memory device memcpy9...", L"memory", 0);
					i++;
				}
				else if((csDeviceLocator.Find(L"DIMM2")!= -1) && bDLLExist)
				{
					ZeroMemory(stMemSlot.csSerialNum,10);
					memcpy(stMemSlot.csSerialNum,AllSpdInfo.spd[i].SN,8);
					sprintf_s(stMemSlot.csSpeed, sizeof(stMemSlot.csSpeed),"%d", AllSpdInfo.spd[i].Freq);
					sprintf_s(stMemSlot.csProductDate, sizeof(stMemSlot.csProductDate),"%d年%d周", AllSpdInfo.spd[i].year,AllSpdInfo.spd[i].week);
					memcpy(stMemSlot.csType, AllSpdInfo.spd[i].Type, min(sizeof(stMemSlot.csType), sizeof(AllSpdInfo.spd[i].Type)));
					i++;
				}
				else if((csDeviceLocator.Find(L"DIMM4")!=-1) && bDLLExist)
				{
					ZeroMemory(stMemSlot.csSerialNum,10);
					memcpy(stMemSlot.csSerialNum,AllSpdInfo.spd[i].SN,8);
					sprintf_s(stMemSlot.csSpeed, sizeof(stMemSlot.csSpeed),"%d", AllSpdInfo.spd[i].Freq);
					sprintf_s(stMemSlot.csProductDate, sizeof(stMemSlot.csProductDate),"%d年%d周", AllSpdInfo.spd[i].year,AllSpdInfo.spd[i].week);
					memcpy(stMemSlot.csType, AllSpdInfo.spd[i].Type, min(sizeof(stMemSlot.csType), sizeof(AllSpdInfo.spd[i].Type)));
					i++;
				}
				else if((csDeviceLocator.Find(L"DIMM8")!=-1) && bDLLExist)
				{
					ZeroMemory(stMemSlot.csSerialNum,10);
					memcpy(stMemSlot.csSerialNum,AllSpdInfo.spd[i].SN,8);
					sprintf_s(stMemSlot.csSpeed, sizeof(stMemSlot.csSpeed),"%d", AllSpdInfo.spd[i].Freq);
					sprintf_s(stMemSlot.csProductDate, sizeof(stMemSlot.csProductDate),"%d年%d周", AllSpdInfo.spd[i].year,AllSpdInfo.spd[i].week);
					memcpy(stMemSlot.csType, AllSpdInfo.spd[i].Type, min(sizeof(stMemSlot.csType), sizeof(AllSpdInfo.spd[i].Type)));
					i++;
				}

				///////////////////////////////////以下的数据为伪造的数据//////////////////
				if(!strcmp(stMemSlot.csType, ""))
				{
					strcpy(stMemSlot.csType, "DDR3");
				}
				//MessageBox(NULL, L"Enum memory device memcpy10...", L"memory", 0);
				if(!strcmp(stMemSlot.csSpeed, ""))
				{
					strcpy(stMemSlot.csSpeed, "800");
				}
				//MessageBox(NULL, L"Enum memory device memcpy11...", L"memory", 0);
				if(!strcmp(stMemSlot.csProductDate, ""))
				{
					strcpy(stMemSlot.csProductDate, "2008年3月");
				}
				//MessageBox(NULL, L"Enum memory device memcpy12...", L"memory", 0);
				if(!strcmp(stMemSlot.csSerialNum, ""))
				{
					srand((unsigned)time(NULL));
					char serialNumber[10] = {0};
					GetRandomSerialNumber(serialNumber);
					memcpy(stMemSlot.csSerialNum, serialNumber, min(sizeof(stMemSlot.csSerialNum), sizeof(serialNumber)));
				}
				////////////////////////////////////以上的数据为伪造的数据//////////////////
				vecMyList.push_back(stMemSlot);
				//MessageBox(NULL, L"Enum memory device end", L"memory", 0);
				uIndex ++;
			}
			return TRUE;
		}
		return FALSE;
	}
	catch (CException *pEx)
	{
		pEx->Delete();
		return FALSE;
	}
}
BOOL GetWmiInfo::GetMonitors( vector<Monitordll> &vecMyList)
{
	if (!m_bConnected)
		return FALSE;

	
	try
	{
		UINT		uIndex = 0;
		Monitordll	myObject;
		CString		csBuffer;

		if (m_dllWMI.BeginEnumClassObject( _T( "Win32_DesktopMonitor")))
		{
			while (m_dllWMI.MoveNextEnumClassObject())
			{
				ZeroMemory((void*)&myObject, sizeof(myObject));
				csBuffer = m_dllWMI.GetClassObjectStringValue( _T( "PNPDeviceID"));
				csBuffer = csBuffer.Left(csBuffer.ReverseFind(L'\\'));
				string ansiStr = ws_to_ansi((LPCTSTR)csBuffer);
				DWORD ansiStrlen = ansiStr.length() > sizeof(myObject.csModel)? sizeof(myObject.csModel): ansiStr.length();
				memcpy(myObject.csModel, ansiStr.c_str(), ansiStrlen);

				csBuffer = m_dllWMI.GetClassObjectStringValue( _T( "MonitorManufacturer"));
				ansiStr = ws_to_ansi((LPCTSTR)csBuffer);
				ansiStrlen = ansiStr.length() > sizeof(myObject.csManufacturer) ? sizeof(myObject.csManufacturer) : ansiStr.length();
				memcpy(myObject.csManufacturer,   ansiStr.c_str(), ansiStrlen);

				csBuffer = m_dllWMI.GetClassObjectStringValue( _T( "Description"));
				ansiStr = ws_to_ansi((LPCTSTR)csBuffer);
				ansiStrlen = ansiStr.length() > sizeof(myObject.csDescription) ? sizeof(myObject.csDescription) : ansiStr.length();
				memcpy(myObject.csDescription,   ansiStr.c_str(), ansiStrlen);

				csBuffer = m_dllWMI.GetClassObjectStringValue( _T( "Caption"));
				ansiStr = ws_to_ansi((LPCTSTR)csBuffer);
				ansiStrlen = ansiStr.length() > sizeof(myObject.csCaption) ? sizeof(myObject.csCaption) : ansiStr.length();
				memcpy(myObject.csCaption,   ansiStr.c_str(), ansiStrlen);

				csBuffer = m_dllWMI.GetClassObjectStringValue( _T( "MonitorType"));
				ansiStr = ws_to_ansi((LPCTSTR)csBuffer);
				ansiStrlen = ansiStr.length() > sizeof(myObject.csType) ? sizeof(myObject.csType) : ansiStr.length();
				memcpy(myObject.csType,   ansiStr.c_str(), ansiStrlen);
				vecMyList.push_back( myObject);
				uIndex ++;
			}
			m_dllWMI.CloseEnumClassObject();
			return TRUE;
		}
		return FALSE;
	}
	catch (CException *pEx)
	{
		pEx->Delete();
		return FALSE;
	}
}
BOOL GetWmiInfo::GetNetworkAdapters( vector<NetworkAdaptertdll> &vecMyList)
{
	if (!m_bConnected)
		return FALSE;

	
	try
	{
		CString			csBuffer,
			csMacAddress;
		UINT			uIndex = 0,
			uTotalIndex = 0;
		NetworkAdaptertdll	myObject;
		unsigned __int64	u64Value = 0;

		if (m_dllWMI.BeginEnumClassObject( _T( "Win32_NetworkAdapter")))
		{
			while (m_dllWMI.MoveNextEnumClassObject())
			{
				ZeroMemory((void*)&myObject, sizeof(myObject));
				/*csBuffer = m_dllWMI.GetRefElementClassObjectStringValue( _T( "Element"), _T( "AdapterType"));
				myObject.csType = csBuffer;
				csBuffer = m_dllWMI.GetRefElementClassObjectStringValue( _T( "Element"), _T( "ProductName"));
				myObject.csName = csBuffer;
				csBuffer = m_dllWMI.GetRefElementClassObjectStringValue( _T( "Element"), _T( "Speed"));
				myObject.csSpeed = csBuffer;

				csMacAddress = m_dllWMI.GetRefElementClassObjectStringValue( _T( "Setting"), _T( "MACAddress"));*/
				string ansiStr = ws_to_ansi((LPCTSTR)m_dllWMI.GetClassObjectStringValue(_T("Name")));
				DWORD ansiStrlen = ansiStr.length() > sizeof(myObject.csName) ? sizeof(myObject.csName) : ansiStr.length();
				memcpy(myObject.csName,  ansiStr.c_str(), ansiStrlen);

				ansiStr = ws_to_ansi((LPCTSTR)m_dllWMI.GetClassObjectStringValue(_T("Description")));
				ansiStrlen = ansiStr.length() > sizeof(myObject.csDescription) ? sizeof(myObject.csDescription) : ansiStr.length();
				memcpy(myObject.csDescription,   ansiStr.c_str(), ansiStrlen);

				ansiStr = ws_to_ansi((LPCTSTR)m_dllWMI.GetClassObjectStringValue(_T("Manufacturer")));
				ansiStrlen = ansiStr.length() > sizeof(myObject.csManufacturer) ? sizeof(myObject.csManufacturer) : ansiStr.length();
				memcpy(myObject.csManufacturer,   ansiStr.c_str(), ansiStrlen);

				ansiStr = ws_to_ansi((LPCTSTR)m_dllWMI.GetClassObjectStringValue(_T("ProductName")));
				ansiStrlen = ansiStr.length() > sizeof(myObject.csProductName) ? sizeof(myObject.csProductName) : ansiStr.length();
				memcpy(myObject.csProductName,   ansiStr.c_str(), ansiStrlen);

				CString strMac = m_dllWMI.GetClassObjectStringValue( _T( "MACAddress"));
				CString strPNPDeviceID = m_dllWMI.GetClassObjectStringValue( _T( "PNPDeviceID"));
				if (strMac && strPNPDeviceID != "" && 
					strPNPDeviceID.Find(_T("ROOT\\MS_"), 0) == -1&&
					strPNPDeviceID.Find(_T("ROOT\\VMWARE"), 0) == -1)
				{
					// Skip adapters without MAC Address
					vecMyList.push_back(myObject);
					uIndex ++;
				}
			}
			m_dllWMI.CloseEnumClassObject();
			return TRUE;
		}
		return FALSE;
	}
	catch (CException *pEx)
	{
		pEx->Delete();
		return FALSE;
	}
}
BOOL GetWmiInfo::GetPrinters( vector<Printerdll> &vecMyList)
{
	if (!m_bConnected)
		return FALSE;


	try
	{
		UINT		uIndex = 0;
		Printerdll	myObject;
		CString		csBuffer, csPortName;
		string ansiStr;
		DWORD ansiStrlen;

		if (m_dllWMI.BeginEnumClassObject( _T( "Win32_Printer")))
		{
			while (m_dllWMI.MoveNextEnumClassObject())
			{
				ZeroMemory((void*)&myObject, sizeof(myObject));
				DWORD dwLocal = m_dllWMI.GetClassObjectDwordValue( _T( "Local"));
				DWORD dwNetWork = m_dllWMI.GetClassObjectDwordValue( _T( "Network"));
				csPortName = m_dllWMI.GetClassObjectStringValue( _T( "PortName"));
				ansiStr = ws_to_ansi((LPCTSTR)csPortName);
				ansiStrlen = ansiStr.length() > sizeof(myObject.csPortName)? sizeof(myObject.csPortName): ansiStr.length();
				memcpy(myObject.csPortName,  ansiStr.c_str(), ansiStrlen);
				
				csBuffer = m_dllWMI.GetClassObjectStringValue( _T( "Name"));
				ansiStr = ws_to_ansi((LPCTSTR)csBuffer);
				ansiStrlen = ansiStr.length() > sizeof(myObject.csName) ? sizeof(myObject.csName) : ansiStr.length();
				memcpy(myObject.csName, ansiStr.c_str(), ansiStrlen);

				csBuffer = m_dllWMI.GetClassObjectStringValue( _T( "DriverName"));
				ansiStr = ws_to_ansi((LPCTSTR)csBuffer);
				ansiStrlen = ansiStr.length() > sizeof(myObject.csDeviceName) ? sizeof(myObject.csDeviceName) : ansiStr.length();
				memcpy(myObject.csDeviceName,  ansiStr.c_str(), ansiStrlen);

				if(csPortName.Find(_T("COM")) != -1 || 
					csPortName.Find(_T("LTP")) != -1 || 
					csPortName.Find(_T("USB")) != -1 ||
					csPortName.Find(_T("RJ-45")) != -1||
					csPortName.Find(_T("DB-25")) != -1 ||
					csPortName.Find(_T("IEEE")) != -1 ||
					csPortName.Find(_T("MFC-250C")) != -1 ||
					csPortName.Find(_T("RS-232")) != -1)
					if(dwLocal && !dwNetWork)
					{
						vecMyList.push_back(myObject);
						uIndex ++;
					}
			}
			m_dllWMI.CloseEnumClassObject();
			return TRUE;
		}
		return FALSE;
	}
	catch (CException *pEx)
	{
		pEx->Delete();
		return FALSE;
	}
}
BOOL GetWmiInfo::GetSoundDevices( vector<SoundDevicedll>& vecMyList)
{
	if (!m_bConnected)
		return FALSE;
	SoundDevicedll	myObject;
	try
	{
		UINT			uIndex = 0;
		CString			csBuffer;
		string ansiStr;
		DWORD ansiStrlen;

		if (m_dllWMI.BeginEnumClassObject( _T( "Win32_SoundDevice")))
		{
			while (m_dllWMI.MoveNextEnumClassObject())
			{
				ZeroMemory((void*)&myObject, sizeof(myObject));
				csBuffer = m_dllWMI.GetClassObjectStringValue( _T( "Manufacturer"));
				ansiStr = ws_to_ansi((LPCTSTR)csBuffer);
				ansiStrlen = ansiStr.length() > sizeof(myObject.csManufacturer) ? sizeof(myObject.csManufacturer) : ansiStr.length();
				memcpy(myObject.csManufacturer,  ansiStr.c_str(), ansiStrlen);

				csBuffer = m_dllWMI.GetClassObjectStringValue( _T( "ProductName"));
				ansiStr = ws_to_ansi((LPCTSTR)csBuffer);
				ansiStrlen = ansiStr.length() > sizeof(myObject.csName) ? sizeof(myObject.csName) : ansiStr.length();
				memcpy(myObject.csName,   ansiStr.c_str(), ansiStrlen);

				csBuffer = m_dllWMI.GetClassObjectStringValue( _T( "Description"));
				ansiStr = ws_to_ansi((LPCTSTR)csBuffer);
				ansiStrlen = ansiStr.length() > sizeof(myObject.csDescription) ? sizeof(myObject.csDescription) : ansiStr.length();
				memcpy(myObject.csDescription,  ansiStr.c_str(), ansiStrlen);

				vecMyList.push_back(myObject);
				uIndex ++;
			}
			m_dllWMI.CloseEnumClassObject();
			return TRUE;
		}

		return FALSE;
	}
	catch (CException *pEx)
	{
		pEx->Delete();
		return FALSE;
	}

}
BOOL GetWmiInfo::GetStoragePeripherals( vector<StoragePeripheraldll> &vecMyList)
{
	if (!m_bConnected)
		return FALSE;

	UINT				uIndex = 0;
	StoragePeripheraldll	myObject;
	CString				csBuffer;
	string ansiStr;
	DWORD ansiStrlen;
	
	try
	{
		uIndex = 0;
		if (m_dllWMI.BeginEnumClassObject( _T( "Win32_DiskDrive")))
		{
			while (m_dllWMI.MoveNextEnumClassObject())
			{
				ZeroMemory((void*)&myObject, sizeof(myObject));
				memcpy(myObject.csType,  "DiskDrive", sizeof("DiskDrive"));

				csBuffer = m_dllWMI.GetClassObjectStringValue( _T( "InterfaceType"));
				ansiStr = ws_to_ansi((LPCTSTR)csBuffer);
				ansiStrlen = ansiStr.length() > sizeof(myObject.csInterfaceType) ? sizeof(myObject.csInterfaceType) : ansiStr.length();
				memcpy(myObject.csInterfaceType, ansiStr.c_str(), ansiStrlen);
				
				csBuffer = m_dllWMI.GetClassObjectStringValue( _T( "Manufacturer"));
				ansiStr = ws_to_ansi((LPCTSTR)csBuffer);
				ansiStrlen = ansiStr.length() > sizeof(myObject.csManufacturer) ? sizeof(myObject.csManufacturer) : ansiStr.length();
				memcpy(myObject.csManufacturer,  ansiStr.c_str(), ansiStrlen);
				
				csBuffer = m_dllWMI.GetClassObjectStringValue( _T( "Model"));
				ansiStr = ws_to_ansi((LPCTSTR)csBuffer);
				ansiStrlen = ansiStr.length() > sizeof(myObject.csModel) ? sizeof(myObject.csModel) : ansiStr.length();
				memcpy(myObject.csModel, ansiStr.c_str(), ansiStrlen);
				
				csBuffer = m_dllWMI.GetClassObjectStringValue( _T( "Description"));
				ansiStr = ws_to_ansi((LPCTSTR)csBuffer);
				ansiStrlen = ansiStr.length() > sizeof(myObject.csDescription) ? sizeof(myObject.csDescription) : ansiStr.length();
				memcpy(myObject.csDescription,  ansiStr.c_str(), ansiStrlen);
				
				csBuffer = m_dllWMI.GetClassObjectStringValue( _T( "Name"));
				ansiStr = ws_to_ansi((LPCTSTR)csBuffer);
				ansiStrlen = ansiStr.length() > sizeof(myObject.csName) ? sizeof(myObject.csName) : ansiStr.length();
				memcpy(myObject.csName,   ansiStr.c_str(), ansiStrlen);
				unsigned __int64 u64Size = m_dllWMI.GetClassObjectU64Value( _T( "Size")) / ONE_MEGABYTE;
				sprintf_s(myObject.csSize, sizeof(myObject.csSize), "%ld", u64Size);

				vecMyList.push_back(myObject);
				uIndex ++;
			}
			m_dllWMI.CloseEnumClassObject();
			return TRUE;
		}
		return FALSE;
	}
	catch (CException *pEx)
	{
		pEx->Delete();
		return FALSE;
	}
	return FALSE;
}
BOOL GetWmiInfo::GetVideoAdapters( vector<VideoAdapterdll> &vecMyList)
{
	if (!m_bConnected)
		return FALSE;


	try
	{
		UINT			uIndex = 0;
		VideoAdapterdll	myObject;
		CString			csBuffer;
		string ansiStr;
		DWORD ansiStrlen;

		if (m_dllWMI.BeginEnumClassObject( _T( "Win32_VideoController")))
		{
			while (m_dllWMI.MoveNextEnumClassObject())
			{
				ZeroMemory((void*)&myObject, sizeof(myObject));
				csBuffer = m_dllWMI.GetClassObjectStringValue( _T( "Description"));
				ansiStr = ws_to_ansi((LPCTSTR)csBuffer);
				ansiStrlen = ansiStr.length() > sizeof(myObject.csDescription) ? sizeof(myObject.csDescription) : ansiStr.length();
				memcpy(myObject.csDescription,  ansiStr.c_str(), ansiStrlen);
				
				csBuffer = m_dllWMI.GetClassObjectStringValue( _T( "VideoProcessor"));
				ansiStr = ws_to_ansi((LPCTSTR)csBuffer);
				ansiStrlen = ansiStr.length() > sizeof(myObject.csProcessor) ? sizeof(myObject.csProcessor) : ansiStr.length();
				memcpy(myObject.csProcessor,   ansiStr.c_str(), ansiStrlen);
				
				csBuffer = m_dllWMI.GetClassObjectStringValue( _T( "Name"));
				ansiStr = ws_to_ansi((LPCTSTR)csBuffer);
				ansiStrlen = ansiStr.length() > sizeof(myObject.csName) ? sizeof(myObject.csName) : ansiStr.length();
				memcpy(myObject.csName, ansiStr.c_str(), ansiStrlen);
				
				csBuffer.Format( _T( "%lu"), m_dllWMI.GetClassObjectDwordValue( _T( "AdapterRAM")) / ONE_MEGABYTE);
				ansiStr = ws_to_ansi((LPCTSTR)csBuffer);
				ansiStrlen = ansiStr.length() > sizeof(myObject.csMemory) ? sizeof(myObject.csMemory) : ansiStr.length();
				memcpy(myObject.csMemory, ansiStr.c_str(), ansiStrlen);

				vecMyList.push_back(myObject);
				uIndex ++;
			}
			m_dllWMI.CloseEnumClassObject();
			return TRUE;
		}

		return FALSE;
	}
	catch (CException *pEx)
	{
		pEx->Delete();
		return FALSE;
	}
	return TRUE;
}

BOOL GetWmiInfo::GetKeyBoards( vector<KeyBoarddll> &vecMyList)
{
	DWORD dwSessionCount = 0;
	if(!GetSessionCount(dwSessionCount) || dwSessionCount > 1)
	{
		return FALSE;
	}
	if (!m_bConnected)
		return FALSE;

	
	try
	{
		UINT			uIndex = 0;
		KeyBoarddll	myObject;
		CString			csBuffer;
		string ansiStr;
		DWORD ansiStrlen;

		if (m_dllWMI.BeginEnumClassObject( _T( "Win32_Keyboard")))
		{
			while (m_dllWMI.MoveNextEnumClassObject())
			{
				ZeroMemory((void*)&myObject, sizeof(myObject));
				csBuffer = m_dllWMI.GetClassObjectStringValue( _T( "Caption"));
				ansiStr = ws_to_ansi((LPCTSTR)csBuffer);
				ansiStrlen = ansiStr.length() > sizeof(myObject.csCaption) ? sizeof(myObject.csCaption) : ansiStr.length();
				memcpy(myObject.csCaption,  ansiStr.c_str(), ansiStrlen);
				
				csBuffer = m_dllWMI.GetClassObjectStringValue( _T( "Description"));
				ansiStr = ws_to_ansi((LPCTSTR)csBuffer);
				ansiStrlen = ansiStr.length() > sizeof(myObject.csDescription) ? sizeof(myObject.csDescription) : ansiStr.length();
				memcpy(myObject.csDescription, ansiStr.c_str(), ansiStrlen);
				
				csBuffer = m_dllWMI.GetClassObjectStringValue( _T( "Name"));
				ansiStr = ws_to_ansi((LPCTSTR)csBuffer);
				ansiStrlen = ansiStr.length() > sizeof(myObject.csName) ? sizeof(myObject.csName) : ansiStr.length();
				memcpy(myObject.csName, ansiStr.c_str(), ansiStrlen);
				
				csBuffer = m_dllWMI.GetClassObjectStringValue( _T( "DeviceID"));
				ansiStr = ws_to_ansi((LPCTSTR)csBuffer);
				ansiStrlen = ansiStr.length() > sizeof(myObject.csDeviceID) ? sizeof(myObject.csDeviceID) : ansiStr.length();
				memcpy(myObject.csDeviceID, ansiStr.c_str(), ansiStrlen);
				
				csBuffer = m_dllWMI.GetClassObjectStringValue( _T( "Layout"));
				ansiStr = ws_to_ansi((LPCTSTR)csBuffer);
				ansiStrlen = ansiStr.length() > sizeof(myObject.csLayout) ? sizeof(myObject.csLayout) : ansiStr.length();
				memcpy(myObject.csLayout, ansiStr.c_str(), ansiStrlen);

				vecMyList.push_back(myObject);
				uIndex ++;
			}
			m_dllWMI.CloseEnumClassObject();
			return TRUE;
		}

		return FALSE;
	}
	catch (CException *pEx)
	{
		pEx->Delete();
		return FALSE;
	}
	return TRUE;
}

BOOL GetWmiInfo::GetPointingDevices( vector<PointingDevicedll> &vecMyList)
{
	if (!m_bConnected)
		return FALSE;

	
	try
	{
		UINT			uIndex = 0;
		PointingDevicedll	myObject;
		CString			csBuffer;
		string ansiStr;
		DWORD ansiStrlen;

		if (m_dllWMI.BeginEnumClassObject( _T( "Win32_PointingDevice")))
		{
			while (m_dllWMI.MoveNextEnumClassObject())
			{
				ZeroMemory((void*)&myObject, sizeof(myObject));
				csBuffer = m_dllWMI.GetClassObjectStringValue( _T( "Caption"));
				ansiStr = ws_to_ansi((LPCTSTR)csBuffer);
				ansiStrlen = ansiStr.length() > sizeof(myObject.csCaption) ? sizeof(myObject.csCaption) : ansiStr.length();
				memcpy(myObject.csCaption,  ansiStr.c_str(), ansiStrlen);

				csBuffer = m_dllWMI.GetClassObjectStringValue( _T( "Description"));
				ansiStr = ws_to_ansi((LPCTSTR)csBuffer);
				ansiStrlen = ansiStr.length() > sizeof(myObject.csDescription) ? sizeof(myObject.csDescription) : ansiStr.length();
				memcpy(myObject.csDescription,ansiStr.c_str(), ansiStrlen);

				csBuffer = m_dllWMI.GetClassObjectStringValue( _T( "Name"));
				ansiStr = ws_to_ansi((LPCTSTR)csBuffer);
				ansiStrlen = ansiStr.length() > sizeof(myObject.csName) ? sizeof(myObject.csName) : ansiStr.length();
				memcpy(myObject.csName,ansiStr.c_str(), ansiStrlen);

				csBuffer = m_dllWMI.GetClassObjectStringValue( _T( "HardwareType"));
				ansiStr = ws_to_ansi((LPCTSTR)csBuffer);
				ansiStrlen = ansiStr.length() > sizeof(myObject.csHardwareType) ? sizeof(myObject.csHardwareType) : ansiStr.length();
				memcpy(myObject.csHardwareType,ansiStr.c_str(), ansiStrlen);
				
				csBuffer = m_dllWMI.GetClassObjectStringValue( _T( "Manufacturer"));
				ansiStr = ws_to_ansi((LPCTSTR)csBuffer);
				ansiStrlen = ansiStr.length() > sizeof(myObject.csManufacturer) ? sizeof(myObject.csManufacturer) : ansiStr.length();
				memcpy(myObject.csManufacturer,ansiStr.c_str(), ansiStrlen);

				DWORD dwType = m_dllWMI.GetClassObjectDwordValue( _T( "PointingType"));
				switch(dwType)
				{
				case 1:
					memcpy(myObject.csPointingType, "Other", sizeof("Other"));
					break;
				case 2:
					memcpy(myObject.csPointingType, "Unknown", sizeof("Unknown"));
					break;
				case 3:
					memcpy(myObject.csPointingType, "Mouse",sizeof("Mouse"));
					break;
				case 4:
					memcpy(myObject.csPointingType,"Track Ball", sizeof("Track Ball"));
					break;
				case 5:
					memcpy(myObject.csPointingType,"Track Point", sizeof("Track Point"));
					break;
				case 6:
					memcpy(myObject.csPointingType,"Glide Point", sizeof("Glide Point"));
					break;
				case 7:
					memcpy(myObject.csPointingType,"Touch Pad", sizeof("Touch Pad"));
					break;
				case 8:
					memcpy(myObject.csPointingType,"Touch Screen", sizeof("Touch Screen"));
					break;
				case 9:
					memcpy(myObject.csPointingType,"Mouse - Optical Sensor", sizeof(myObject.csPointingType));
					break;
				default:
					break;
				}

				vecMyList.push_back(myObject);
				uIndex ++;
			}
			m_dllWMI.CloseEnumClassObject();
			return TRUE;
		}

		return FALSE;
	}
	catch (CException *pEx)
	{
		pEx->Delete();
		return FALSE;
	}
	return TRUE;
}

BOOL GetWmiInfo::GetSessionCount(DWORD &dwCount)
{
	WTS_SESSION_INFO* pSessionInfo = NULL;
	BOOL ret = WTSEnumerateSessions(WTS_CURRENT_SERVER_HANDLE, 0, 1, &pSessionInfo, &dwCount);
	DWORD err = GetLastError();
	return ret;
}