#include "HardInfo.h"
#include "defines.h"
#include "edid.h"

HardInfo::HardInfo(void)
{
	m_wmiInfo.Connect();
}

HardInfo::~HardInfo(void)
{
	m_wmiInfo.Disconnect();
}

UINT HardInfo::GetOS( CHAR csName[], CHAR csVersion[], CHAR csComment[], CHAR csDescription[])
{
	UINT uReturn;
	if (m_wmiInfo.IsWmiConnected())
	{
		uReturn = m_wmiInfo.GetOS( csName, csVersion, csComment, csDescription);
		if (uReturn != UNKNOWN_DEVICE)
			return uReturn;
	}
	return FALSE;//m_othWay.GetOS(csName, csVersion);
}

BOOL HardInfo::GetBiosInfo( vector<Biosdll> &vecMyBios)
{
	if (m_wmiInfo.IsWmiConnected() && m_wmiInfo.GetBiosInfo( vecMyBios))
		return TRUE;
	return FALSE;
}

BOOL HardInfo::GetCPUInfo( vector<CPUdll> &vecMyList)
{
	if (m_wmiInfo.IsWmiConnected() && m_wmiInfo.GetCPUInfo( vecMyList))
		return TRUE;
	return FALSE;
}

BOOL HardInfo::GetMemorySlots(vector<MemorySlotdll> &vecMyList)
{
	if (m_wmiInfo.IsWmiConnected() && m_wmiInfo.GetMemorySlots( vecMyList))
		return TRUE;
	return FALSE;
}

BOOL HardInfo::GetModems( vector<Modem> &vecMyList)
{
	if (m_wmiInfo.IsWmiConnected() && m_wmiInfo.GetModems( vecMyList))
		return TRUE;
	return FALSE;
}

BOOL HardInfo::GetMonitors( vector<Monitordll> &vecMyList)
{
	CEdid edidInfo;		// Class to get information for EDID enabled display

	// First try EDID
	edidInfo.Connect();
	if (edidInfo.IsConnected() && edidInfo.GetMonitors(vecMyList))
	{
		edidInfo.Disconnect();
		return TRUE;
	}
	edidInfo.Disconnect();
	//Next, try WMI
	if (m_wmiInfo.IsWmiConnected() && m_wmiInfo.GetMonitors( vecMyList))
		return TRUE;
	return FALSE;
}

BOOL HardInfo::GetNetworkAdapters( vector<NetworkAdaptertdll> &vecMyList)
{
	if (m_wmiInfo.IsWmiConnected() && m_wmiInfo.GetNetworkAdapters( vecMyList))
		return TRUE;
	if (m_othWay.GetNetworkAdaptersByIPHelper( vecMyList))
		return TRUE;
	return m_othWay.GetNetworkAdaptersBySNMP( vecMyList);
}

BOOL HardInfo::GetSystemPorts( vector<SystemPort> &vecMyList)
{
	if (m_wmiInfo.IsWmiConnected() && m_wmiInfo.GetSystemPorts( vecMyList))
		return TRUE;
	return FALSE;
}

BOOL HardInfo::GetPrinters( vector<Printerdll> &vecMyList)
{
	if (m_wmiInfo.IsWmiConnected() && m_wmiInfo.GetPrinters( vecMyList))
		return TRUE;
	return FALSE;
}

BOOL HardInfo::GetSoundDevices( vector<SoundDevicedll>& vecMyList)
{
	if (m_wmiInfo.IsWmiConnected() && m_wmiInfo.GetSoundDevices( vecMyList))
		return TRUE;
	return FALSE;
}

BOOL HardInfo::GetStoragePeripherals( vector<StoragePeripheraldll> &vecMyList)
{
	if (m_wmiInfo.IsWmiConnected() && m_wmiInfo.GetStoragePeripherals( vecMyList))
		return TRUE;
	return TRUE;
}

BOOL HardInfo::GetSystemControllers( vector<SystemController> &vecMyList)
{
	if (m_wmiInfo.IsWmiConnected() && m_wmiInfo.GetSystemControllers( vecMyList))
		return TRUE;
	return FALSE;
}

BOOL HardInfo::GetSystemSlots( vector<SystemSlot> &vecMyList)
{
	if (m_wmiInfo.IsWmiConnected() && m_wmiInfo.GetSystemSlots( vecMyList))
		return TRUE;
	return FALSE;
}

BOOL HardInfo::GetVideoAdapters( vector<VideoAdapterdll> &vecMyList)
{
	if (m_wmiInfo.IsWmiConnected() && m_wmiInfo.GetVideoAdapters( vecMyList))
		return TRUE;
	return FALSE;
}

BOOL HardInfo::GetKeyBoards( vector<KeyBoarddll> &vecMyList)
{
	if (m_wmiInfo.IsWmiConnected() && m_wmiInfo.GetKeyBoards( vecMyList))
		return TRUE;
	return FALSE;
}

BOOL HardInfo::GetPointingDevices( vector<PointingDevicedll> &vecMyList)
{
	if (m_wmiInfo.IsWmiConnected() && m_wmiInfo.GetPointingDevices( vecMyList))
		return TRUE;
	return FALSE;
}