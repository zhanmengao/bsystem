// WarInfo.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "WarInfo.h"
#include "HardInfo.h"

//#include "cpuid_asm.h"

//BOOL APIENTRY DllMain( HANDLE hModule, 
//                       DWORD  ul_reason_for_call, 
//                       LPVOID lpReserved
//					 )
//{
//    return TRUE;
//}

WORD  GetCpuInfo(LPSTR lppszCpu,int len)
{
#ifndef _WIN64
	unsigned long DBaseIndex, DFeInfo, DFeInfo2, DCPUBaseInfo; 
	unsigned long DFeIndex, DCPUExInfo;
	char cCom[13]; 
	char cProStr[49]; 

	_asm 
	{ 
		xor eax, eax 
			cpuid 
			mov DBaseIndex      ,eax 
			mov dword ptr cCom    ,ebx 
			mov dword ptr cCom+4  ,edx //AMD CPU要把ecx改为edx 
			mov dword ptr cCom+8  ,ecx //AMD CPU要把edx改为ecx 

			mov eax, 1 
			cpuid 
			mov DCPUBaseInfo, eax 
			mov DFeInfo, ebx 
			mov DFeInfo2, edx 

			mov eax, 0x80000000 
			cpuid 
			mov DFeIndex, eax 

			mov eax, 0x80000001 
			cpuid 
			mov DCPUExInfo, eax 

			mov eax, 0x80000002 
			cpuid 
			mov dword ptr cProStr    , eax 
			mov dword ptr cProStr + 4  , ebx 
			mov dword ptr cProStr + 8  , ecx 
			mov dword ptr cProStr + 12  ,edx 

			mov eax, 0x80000003 
			cpuid 
			mov dword ptr cProStr + 16  , eax 
			mov dword ptr cProStr + 20  , ebx 
			mov dword ptr cProStr + 24  , ecx 
			mov dword ptr cProStr + 28  , edx 

			mov eax, 0x80000004 
			cpuid 
			mov dword ptr cProStr + 32  , eax 
			mov dword ptr cProStr + 36  , ebx 
			mov dword ptr cProStr + 40  , ecx 
			mov dword ptr cProStr + 44  , edx 
	} 
#else
	//ULONGLONG DBaseIndex, DFeInfo, DFeInfo2, DCPUBaseInfo; 
	//ULONGLONG DFeIndex, DCPUExInfo;

	char cCom[13*2] = {0};
	char cProStr[49*2] = {0}; 
	//get_cpuid_info(cCom, cProStr);
#endif

	if(lppszCpu && len>0)
	{
		memset(lppszCpu,0,len);
		memcpy(lppszCpu,cProStr,strlen(cProStr));
		_snprintf(lppszCpu,len,"%s",cProStr);
		return S_OK;
	}
	return S_FALSE;
}

HardInfo *g_hardInfo = NULL;

BOOL  BeginGetHardInfo()
{
	if(g_hardInfo == NULL)
	{
		g_hardInfo=new HardInfo();
	}
	
	return TRUE;
}
BOOL  EndGetHardInfo()
{

	if(g_hardInfo != NULL)
	{
		delete g_hardInfo;
		g_hardInfo = NULL;
	}
	
	return TRUE;
}
/*
BOOL  GetCPUInfo(vector<CPUdll> &vecCPU)
{
	if(!g_hardInfo)
	{
		return FALSE;
	}
	return g_hardInfo->GetCPUInfo(vecCPU);
}
*/
BOOL  GetCPUInfo(CPUdll arr[], int* pCountRet)
{
	vector<CPUdll> vec;
	if(!g_hardInfo || !g_hardInfo->GetCPUInfo(vec))
	{
		return FALSE;
	}
	for (int i = 0; i < (int)vec.size(); i++)
	{
		if (i < *pCountRet)
			arr[i] = vec[i];
	}
	*pCountRet = (int)vec.size();
	return TRUE;
}
UINT   GetOS(OSdll &stOS)
{
	if(!g_hardInfo)
	{
		return FALSE;
	}
	return g_hardInfo->GetOS(stOS.csName, stOS.csVersion, stOS.csComment, stOS.csDescription);
}
/*
BOOL  GetBiosInfo( vector<Biosdll> &vecMyBios)
{
	if(!g_hardInfo)
	{
		return FALSE;
	}
	return g_hardInfo->GetBiosInfo(vecMyBios);
}
*/
BOOL GetBiosInfo(Biosdll arr[], int* pCountRet)
{
	vector<Biosdll> vec;
	if(!g_hardInfo || !g_hardInfo->GetBiosInfo(vec))
	{
		return FALSE;
	}
	for (int i = 0; i < (int)vec.size(); i++)
	{
		if (i < *pCountRet)
			arr[i] = vec[i];
	}
	*pCountRet = (int)vec.size();
	return TRUE;
}
/*
BOOL  GetMemorySlots(vector<MemorySlotdll> &vecMemSlot)
{
	if(!g_hardInfo)
	{
		return FALSE;
	}
	return g_hardInfo->GetMemorySlots(vecMemSlot);
}
*/
BOOL GetMemorySlots(MemorySlotdll arr[], int* pCountRet)
{
	vector<MemorySlotdll> vec;
	if(!g_hardInfo || !g_hardInfo->GetMemorySlots(vec))
	{
		return FALSE;
	}
	for (int i = 0; i < (int)vec.size(); i++)
	{
		if (i < *pCountRet)
			arr[i] = vec[i];
	}
	*pCountRet = (int)vec.size();
	return TRUE;
}
/*
BOOL  GetMonitors( vector<Monitordll>& vecMonitors)
{
	if(!g_hardInfo)
	{
		return FALSE;
	}
	return g_hardInfo->GetMonitors(vecMonitors);
}
*/
BOOL GetMonitors(Monitordll arr[], int* pCountRet)
{
	vector<Monitordll> vec;
	if(!g_hardInfo || !g_hardInfo->GetMonitors(vec))
	{
		return FALSE;
	}
	for (int i = 0; i < (int)vec.size(); i++)
	{
		if (i < *pCountRet)
			arr[i] = vec[i];
	}
	*pCountRet = (int)vec.size();
	return TRUE;
}
/*
BOOL  GetNetworkAdapters( vector<NetworkAdaptertdll> &vecNetAdapters)
{
	if(!g_hardInfo)
	{
		return FALSE;
	}
	return g_hardInfo->GetNetworkAdapters(vecNetAdapters);
}
*/
BOOL GetNetworkAdapters(NetworkAdaptertdll arr[], int* pCountRet)
{
	vector<NetworkAdaptertdll> vec;
	if(!g_hardInfo || !g_hardInfo->GetNetworkAdapters(vec))
	{
		return FALSE;
	}
	for (int i = 0; i < (int)vec.size(); i++)
	{
		if (i < *pCountRet)
			arr[i] = vec[i];
	}
	*pCountRet = (int)vec.size();
	return TRUE;
}
/*
BOOL  GetPrinters( vector<Printerdll> &vecPrinters)
{
	if(!g_hardInfo)
	{
		return FALSE;
	}
	return g_hardInfo->GetPrinters(vecPrinters);
}
*/
BOOL GetPrinters(Printerdll arr[], int* pCountRet)
{
	vector<Printerdll> vec;
	if(!g_hardInfo || !g_hardInfo->GetPrinters(vec))
	{
		return FALSE;
	}
	for (int i = 0; i < (int)vec.size(); i++)
	{
		if (i < *pCountRet)
			arr[i] = vec[i];
	}
	*pCountRet = (int)vec.size();
	return TRUE;
}
/*
BOOL  GetSoundDevices( vector<SoundDevicedll>& vecSoundDevices)
{
	if(!g_hardInfo)
	{
		return FALSE;
	}
	return g_hardInfo->GetSoundDevices(vecSoundDevices);
}
*/
BOOL GetSoundDevices(SoundDevicedll arr[], int* pCountRet)
{
	vector<SoundDevicedll> vec;
	if(!g_hardInfo || !g_hardInfo->GetSoundDevices(vec))
	{
		return FALSE;
	}
	for (int i = 0; i < (int)vec.size(); i++)
	{
		if (i < *pCountRet)
			arr[i] = vec[i];
	}
	*pCountRet = (int)vec.size();
	return TRUE;
}
/*
BOOL  GetStoragePeripherals( vector<StoragePeripheraldll> &vecDisk)
{
	if(!g_hardInfo)
	{
		return FALSE;
	}
	return g_hardInfo->GetStoragePeripherals(vecDisk);
}
*/
BOOL GetStoragePeripherals(StoragePeripheraldll arr[], int* pCountRet)
{
	vector<StoragePeripheraldll> vec;
	if(!g_hardInfo || !g_hardInfo->GetStoragePeripherals(vec))
	{
		return FALSE;
	}
	for (int i = 0; i < (int)vec.size(); i++)
	{
		if (i < *pCountRet)
			arr[i] = vec[i];
	}
	*pCountRet = (int)vec.size();
	return TRUE;
}
/*
BOOL  GetVideoAdapters( vector<VideoAdapterdll> &vecVideoAdapters)
{
	if(!g_hardInfo)
	{
		return FALSE;
	}
	return g_hardInfo->GetVideoAdapters(vecVideoAdapters);
}
*/
BOOL GetVideoAdapters(VideoAdapterdll arr[], int* pCountRet)
{
	vector<VideoAdapterdll> vec;
	if(!g_hardInfo || !g_hardInfo->GetVideoAdapters(vec))
	{
		return FALSE;
	}
	for (int i = 0; i < (int)vec.size(); i++)
	{
		if (i < *pCountRet)
			arr[i] = vec[i];
	}
	*pCountRet = (int)vec.size();
	return TRUE;
}
/*
BOOL  GetKeyBoards( vector<KeyBoarddll> &vecKeyBoards)
{
	if(!g_hardInfo)
	{
		return FALSE;
	}
	return g_hardInfo->GetKeyBoards(vecKeyBoards);
}
*/
BOOL GetKeyBoards(KeyBoarddll arr[], int* pCountRet)
{
	vector<KeyBoarddll> vec;
	if(!g_hardInfo || !g_hardInfo->GetKeyBoards(vec))
	{
		return FALSE;
	}
	for (int i = 0; i < (int)vec.size(); i++)
	{
		if (i < *pCountRet)
			arr[i] = vec[i];
	}
	*pCountRet = (int)vec.size();
	return TRUE;
}
/*
BOOL  GetPointingDevices( vector<PointingDevicedll> &vecPointingDevices)
{
	if(!g_hardInfo)
	{
		return FALSE;
	}
	return g_hardInfo->GetPointingDevices(vecPointingDevices);
}
*/
BOOL GetPointingDevices(PointingDevicedll arr[], int* pCountRet)
{
	vector<PointingDevicedll> vec;
	if(!g_hardInfo || !g_hardInfo->GetPointingDevices(vec))
	{
		return FALSE;
	}
	for (int i = 0; i < (int)vec.size(); i++)
	{
		if (i < *pCountRet)
			arr[i] = vec[i];
	}
	*pCountRet = (int)vec.size();
	return TRUE;
}
/*
BOOL  GetModems( vector<Modem> &vecModems)
{
	if(!g_hardInfo)
	{
		return FALSE;
	}
	return g_hardInfo->GetModems(vecModems);
}
*/
BOOL GetModems(Modem arr[], int* pCountRet)
{
	vector<Modem> vec;
	if(!g_hardInfo || !g_hardInfo->GetModems(vec))
	{
		return FALSE;
	}
	for (int i = 0; i < (int)vec.size(); i++)
	{
		if (i < *pCountRet)
			arr[i] = vec[i];
	}
	*pCountRet = (int)vec.size();
	return TRUE;
}
/*
BOOL  GetSystemPorts( vector<SystemPort> &vecSysPorts)
{
	if(!g_hardInfo)
	{
		return FALSE;
	}
	return g_hardInfo->GetSystemPorts(vecSysPorts);
}
*/
BOOL GetSystemPorts(SystemPort arr[], int* pCountRet)
{
	vector<SystemPort> vec;
	if(!g_hardInfo || !g_hardInfo->GetSystemPorts(vec))
	{
		return FALSE;
	}
	for (int i = 0; i < (int)vec.size(); i++)
	{
		if (i < *pCountRet)
			arr[i] = vec[i];
	}
	*pCountRet = (int)vec.size();
	return TRUE;
}
/*
BOOL  GetSystemControllers( vector<SystemController> &vecSysCtrls)
{
	if(!g_hardInfo)
	{
		return FALSE;
	}
	return g_hardInfo->GetSystemControllers(vecSysCtrls);
}
*/
BOOL GetSystemControllers(SystemController arr[], int* pCountRet)
{
	vector<SystemController> vec;
	if(!g_hardInfo || !g_hardInfo->GetSystemControllers(vec))
	{
		return FALSE;
	}
	for (int i = 0; i < (int)vec.size(); i++)
	{
		if (i < *pCountRet)
			arr[i] = vec[i];
	}
	*pCountRet = (int)vec.size();
	return TRUE;
}
/*
BOOL  GetSystemSlots( vector<SystemSlot> &vecSysSlots)
{
	if(!g_hardInfo)
	{
		return FALSE;
	}
	return g_hardInfo->GetSystemSlots(vecSysSlots);
}
*/
BOOL GetSystemSlots(SystemSlot arr[], int* pCountRet)
{
	vector<SystemSlot> vec;
	if(!g_hardInfo || !g_hardInfo->GetSystemSlots(vec))
	{
		return FALSE;
	}
	for (int i = 0; i < (int)vec.size(); i++)
	{
		if (i < *pCountRet)
			arr[i] = vec[i];
	}
	*pCountRet = (int)vec.size();
	return TRUE;
}
