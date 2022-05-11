#pragma once

#include <vector>
#include "WarInfo.h"
#include "CtrlWmi.h"

using namespace std;

class GetWmiInfo
{
public:
	GetWmiInfo(void);
	~GetWmiInfo(void);
	//////////////////////////////////
	// Connection methods
	//////////////////////////////////

	string ws_to_ansi(const wstring& wstr);
	BOOL Connect( LPCTSTR lpstrDevice = NULL);
	BOOL IsWmiConnected();
	BOOL Disconnect();

	//////////////////////////////////
	// Get attributes values
	//////////////////////////////////

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

	BOOL GetSessionCount(DWORD &dwCount);

protected: // Methods
	BOOL ParseChassisType( CString &csType);

protected: // Attributes
	CCtrlWmi m_dllWMI;	// Class to access WMI
	BOOL m_bConnected;	// Do we are already connected to WMI namespace
};
