#pragma once
#include "stdafx.h"
#include <vector>
#include "WarInfo.h"
#include "GetWmiInfo.h"
#include "OtherWay.h"

using namespace std;

class HardInfo
{
public:
	HardInfo(void);
	~HardInfo(void);

	// OS information
	UINT GetOS( CHAR csName[], CHAR csVersion[], CHAR csComment[], CHAR csDescription[]);
	// Hardware informations
	BOOL GetBiosInfo( vector<Biosdll> &vecMyBios);
	BOOL GetCPUInfo( vector<CPUdll> &vecMyList);
	BOOL GetMemorySlots(vector<MemorySlotdll> &vecMyList);
	BOOL GetModems( vector<Modem> &vecMyList);
	BOOL GetMonitors( vector<Monitordll> &vecMyList);
	BOOL GetNetworkAdapters( vector<NetworkAdaptertdll> &vecMyList);
	BOOL GetSystemPorts( vector<SystemPort> &vecMyList);
	BOOL GetPrinters( vector<Printerdll> &vecMyList);
	BOOL GetSoundDevices( vector<SoundDevicedll>& vecMyList);
	BOOL GetStoragePeripherals( vector<StoragePeripheraldll> &vecMyList);
	BOOL GetSystemControllers( vector<SystemController> &vecMyList);
	BOOL GetSystemSlots( vector<SystemSlot> &vecMyList);
	BOOL GetVideoAdapters( vector<VideoAdapterdll> &vecMyList);
	BOOL GetKeyBoards( vector<KeyBoarddll> &vecMyList);
	BOOL GetPointingDevices( vector<PointingDevicedll> &vecMyList);

	GetWmiInfo		m_wmiInfo;		// Class to get informations from WMI
	COtherWay m_othWay;
};
