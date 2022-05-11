#if !defined(WAR_INFO_H)
#define WAR_INFO_H
#include <windows.h>
#include <vector>
using namespace std;
/*
*	CPUdll, ����win xp��©�������Ըýṹ�޷�ʹ��
*/
struct CPUdll
{
	CHAR csName[MAX_PATH];
	CHAR csProcSpeed[10];//ת��(MHz)
	CHAR csDescription[MAX_PATH];
};
/*
*	BIOS
*/
struct Biosdll
{
	CHAR csManufacturer[MAX_PATH];
	CHAR csModel[MAX_PATH]; //�ͺ�
};
/*
*	�ڴ�
*/
struct MemorySlotdll
{
	CHAR csCaption[MAX_PATH];
	CHAR csDeviceLocator[MAX_PATH];//�ڴ�λ��
	CHAR csCapacity[50]; //�ڴ��С
	CHAR csType[50];	//�ڴ�����
	UINT	uSlotNumber;//�ڴ�����
	CHAR csSpeed[50];	//�ڴ�����ٶ� ��MHz��
	CHAR csDescription[MAX_PATH];
	CHAR csName[MAX_PATH];
	CHAR csSerialNum[10];
	CHAR csProductDate[40]; //��������
};
/*
*	���ƽ���� ����ʱ������δʵ�֣�
*/
struct Modem
{
	CHAR csName[MAX_PATH];
	CHAR csModel[MAX_PATH];	
	CHAR csDescription[MAX_PATH];
};
/*
*	��ʾ��
*/
struct Monitordll
{
	CHAR csManufacturer[MAX_PATH]; //������
	CHAR csType[50];	//����
	CHAR csModel[50];
	CHAR csCaption[MAX_PATH];
	CHAR csDescription[MAX_PATH];
	CHAR csSerialNum[50];
	CHAR csMonitorSize[50]; //��ʾ���ߴ�
};
/*
*	����
*/
struct NetworkAdaptertdll
{
	CHAR csName[MAX_PATH];
	CHAR csDescription[MAX_PATH];
	CHAR csManufacturer[MAX_PATH];
	CHAR csProductName[MAX_PATH];
};
/*
*	�������ڣ���ʱ������δʵ�֣�
*/
struct SystemPort
{
	CHAR csName[MAX_PATH];
	CHAR csType[10];	//����(serial, parallel...)
	CHAR csCaption[MAX_PATH];
	CHAR csDescription[MAX_PATH];
};
/*
*	��ӡ��
*/
struct Printerdll
{
	CHAR csName[MAX_PATH];
	CHAR csDeviceName[MAX_PATH];
	CHAR csPortName[MAX_PATH];
};
/*
*	����
*/
struct SoundDevicedll
{
	CHAR csManufacturer[MAX_PATH]; //������
	CHAR csName[MAX_PATH];
	CHAR csDescription[MAX_PATH];
};
/*
*	Ӳ��,�������豸
*/
struct StoragePeripheraldll
{
	CHAR csType[20];		//����
	CHAR csManufacturer[MAX_PATH]; //������
	CHAR csInterfaceType[10];
	CHAR csName[MAX_PATH];
	CHAR csModel[MAX_PATH];
	CHAR csDescription[MAX_PATH];
	CHAR csSize[15]; //��С��MB��
};
/*
*	ϵͳ����������ʱ������δʵ�֣�
*/
struct SystemController
{
	CHAR csType[10];				//����(Floppy, IDE, SCSI, PCMCIA...)
	CHAR csManufacturer[MAX_PATH]; //������
	CHAR csCaption[MAX_PATH];
	CHAR csName[MAX_PATH];
	CHAR csDescription[MAX_PATH];
};
/*
*	ϵͳ��ۣ���ʱ������δʵ�֣�
*/
struct SystemSlot
{
	CHAR csName[MAX_PATH];
	CHAR csDescription[MAX_PATH];
	CHAR csDesignation[10];//�ƺ�(ISA1, PCI1, PCI2, AGP1...)
};
/*
*	�Կ�
*/
struct VideoAdapterdll
{
	CHAR csName[MAX_PATH];
	CHAR csMemory[10];//���� (��λ MB)
	CHAR csDescription[MAX_PATH];
	CHAR csProcessor[MAX_PATH];
};
/*
*	OS
*/
struct OSdll
{
	CHAR csName[MAX_PATH];
	CHAR csVersion[20];
	CHAR csComment[20];
	CHAR csDescription[MAX_PATH];
};
/*
*	���� 
*/
struct KeyBoarddll
{
	CHAR csCaption[MAX_PATH];
	CHAR csDescription[MAX_PATH];
	CHAR csDeviceID[MAX_PATH];
	CHAR csName[MAX_PATH];
	CHAR csLayout[MAX_PATH];
};
/*
*	��������豸���������
*/
struct PointingDevicedll
{
	CHAR csCaption[MAX_PATH];
	CHAR csDescription[MAX_PATH];
	CHAR csHardwareType[MAX_PATH];
	CHAR csManufacturer[MAX_PATH];
	CHAR csName[MAX_PATH];
	CHAR csPointingType[20];
};
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

	BOOL  BeginGetHardInfo();
	BOOL  EndGetHardInfo();
	WORD  GetCpuInfo(LPSTR lppszCpu,int len);
	UINT  GetOS(OSdll &stOS);
	BOOL  GetBiosInfo(Biosdll arr[], IN OUT int* nCountRet);
	BOOL  GetMemorySlots(MemorySlotdll arr[], IN OUT int* nCountRet);
	BOOL  GetMonitors(Monitordll arr[], IN OUT int* nCountRet);
	BOOL  GetNetworkAdapters(NetworkAdaptertdll arr[], IN OUT int* nCountRet);
	BOOL  GetPrinters(Printerdll arr[], IN OUT int* nCountRet);
	BOOL  GetSoundDevices(SoundDevicedll arr[], IN OUT int* nCountRet);
	BOOL  GetStoragePeripherals(StoragePeripheraldll arr[], IN OUT int* nCountRet);
	BOOL  GetVideoAdapters(VideoAdapterdll arr[], IN OUT int* nCountRet);
	BOOL  GetKeyBoards(KeyBoarddll arr[], IN OUT int* nCountRet);
	BOOL  GetPointingDevices(PointingDevicedll arr[], IN OUT int* nCountRet);
	/****************************
	*
	*���º�����������δʵ��
	*
	*****************************/
	BOOL  GetModems(Modem arr[], IN OUT int* nCountRet);
	BOOL  GetSystemPorts(SystemPort arr[], IN OUT int* nCountRet);
	BOOL  GetSystemControllers(SystemController arr[], IN OUT int* nCountRet);
	BOOL  GetSystemSlots(SystemSlot arr[], IN OUT int* nCountRet);
	/****************************
	*
	*����win xp��©�����������к����޷�ʹ��
	*http://support.microsoft.com/kb/953955
	*
	*****************************/
	BOOL  GetCPUInfo(CPUdll arr[], IN OUT int* nCountRet);


#ifdef __cplusplus
}
#endif  /* __cplusplus */



#ifdef __cplusplus


//////////////////////////////////////////////////////////////////////////
// ��̬��ת���ӿ�

#include <vector>
using namespace std;

inline BOOL  GetBiosInfo( vector<Biosdll> &vec)
{
	int ncount = 0;
	GetBiosInfo(NULL, &ncount);
	Biosdll *arr = new Biosdll[ncount];
	if (!arr)
		return FALSE;
	BOOL ret = GetBiosInfo(arr, &ncount);
	if (ret)
	{
		for (int i = 0; i<ncount; i++)
			vec.push_back(arr[i]);
	}
	delete [] arr;
	return ret;
}
inline BOOL  GetMemorySlots(vector<MemorySlotdll> &vec)
{
	int ncount = 0;
	GetMemorySlots(NULL, &ncount);
	//MessageBox(NULL, L"Enum memory device num", L"memory", 0);
	MemorySlotdll *arr = new MemorySlotdll[ncount];
	if (!arr)
		return FALSE;
	BOOL ret = GetMemorySlots(arr, &ncount);
	if (ret)
	{
		for (int i = 0; i<ncount; i++)
			vec.push_back(arr[i]);
	}
	delete [] arr;
	return ret;
}
inline BOOL  GetMonitors( vector<Monitordll> &vec)
{
	int ncount = 0;
	GetMonitors(NULL, &ncount);
	Monitordll *arr = new Monitordll[ncount];
	if (!arr)
		return FALSE;
	BOOL ret = GetMonitors(arr, &ncount);
	if (ret)
	{
		for (int i = 0; i<ncount; i++)
			vec.push_back(arr[i]);
	}
	delete [] arr;
	return ret;
}
inline BOOL  GetNetworkAdapters( vector<NetworkAdaptertdll> &vec)
{
	int ncount = 0;
	GetNetworkAdapters(NULL, &ncount);
	NetworkAdaptertdll *arr = new NetworkAdaptertdll[ncount];
	if (!arr)
		return FALSE;
	BOOL ret = GetNetworkAdapters(arr, &ncount);
	if (ret)
	{
		for (int i = 0; i<ncount; i++)
			vec.push_back(arr[i]);
	}
	delete [] arr;
	return ret;
}
inline BOOL  GetPrinters( vector<Printerdll> &vec)
{
	int ncount = 0;
	GetPrinters(NULL, &ncount);
	Printerdll *arr = new Printerdll[ncount];
	if (!arr)
		return FALSE;
	BOOL ret = GetPrinters(arr, &ncount);
	if (ret)
	{
		for (int i = 0; i<ncount; i++)
			vec.push_back(arr[i]);
	}
	delete [] arr;
	return ret;
}
inline BOOL  GetSoundDevices( vector<SoundDevicedll>& vec)
{
	int ncount = 0;
	GetSoundDevices(NULL, &ncount);
	SoundDevicedll *arr = new SoundDevicedll[ncount];
	if (!arr)
		return FALSE;
	BOOL ret = GetSoundDevices(arr, &ncount);
	if (ret)
	{
		for (int i = 0; i<ncount; i++)
			vec.push_back(arr[i]);
	}
	delete [] arr;
	return ret;
}
inline BOOL  GetStoragePeripherals( vector<StoragePeripheraldll> &vec)
{
	int ncount = 0;
	GetStoragePeripherals(NULL, &ncount);
	StoragePeripheraldll *arr = new StoragePeripheraldll[ncount];
	if (!arr)
		return FALSE;
	BOOL ret = GetStoragePeripherals(arr, &ncount);
	if (ret)
	{
		for (int i = 0; i<ncount; i++)
			vec.push_back(arr[i]);
	}
	delete [] arr;
	return ret;
}
inline BOOL  GetVideoAdapters( vector<VideoAdapterdll> &vec)
{
	int ncount = 0;
	GetVideoAdapters(NULL, &ncount);
	VideoAdapterdll *arr = new VideoAdapterdll[ncount];
	if (!arr)
		return FALSE;
	BOOL ret = GetVideoAdapters(arr, &ncount);
	if (ret)
	{
		for (int i = 0; i<ncount; i++)
			vec.push_back(arr[i]);
	}
	delete [] arr;
	return ret;
}
inline BOOL  GetKeyBoards( vector<KeyBoarddll> &vec)
{
	int ncount = 0;
	GetKeyBoards(NULL, &ncount);
	KeyBoarddll *arr = new KeyBoarddll[ncount];
	if (!arr)
		return FALSE;
	BOOL ret = GetKeyBoards(arr, &ncount);
	if (ret)
	{
		for (int i = 0; i<ncount; i++)
			vec.push_back(arr[i]);
	}
	delete [] arr;
	return ret;
}
inline BOOL  GetPointingDevices( vector<PointingDevicedll> &vec)
{
	int ncount = 0;
	GetPointingDevices(NULL, &ncount);
	PointingDevicedll *arr = new PointingDevicedll[ncount];
	if (!arr)
		return FALSE;
	BOOL ret = GetPointingDevices(arr, &ncount);
	if (ret)
	{
		for (int i = 0; i<ncount; i++)
			vec.push_back(arr[i]);
	}
	delete [] arr;
	return ret;
}
inline BOOL  GetModems( vector<Modem> &vec)
{
	int ncount = 0;
	GetModems(NULL, &ncount);
	Modem *arr = new Modem[ncount];
	if (!arr)
		return FALSE;
	BOOL ret = GetModems(arr, &ncount);
	if (ret)
	{
		for (int i = 0; i<ncount; i++)
			vec.push_back(arr[i]);
	}
	delete [] arr;
	return ret;
}
inline BOOL  GetSystemPorts( vector<SystemPort> &vec)
{
	int ncount = 0;
	GetSystemPorts(NULL, &ncount);
	SystemPort *arr = new SystemPort[ncount];
	if (!arr)
		return FALSE;
	BOOL ret = GetSystemPorts(arr, &ncount);
	if (ret)
	{
		for (int i = 0; i<ncount; i++)
			vec.push_back(arr[i]);
	}
	delete [] arr;
	return ret;
}
inline BOOL  GetSystemControllers( vector<SystemController> &vec)
{
	int ncount = 0;
	GetSystemControllers(NULL, &ncount);
	SystemController *arr = new SystemController[ncount];
	if (!arr)
		return FALSE;
	BOOL ret = GetSystemControllers(arr, &ncount);
	if (ret)
	{
		for (int i = 0; i<ncount; i++)
			vec.push_back(arr[i]);
	}
	delete [] arr;
	return ret;
}
inline BOOL  GetSystemSlots( vector<SystemSlot> &vec)
{
	int ncount = 0;
	GetSystemSlots(NULL, &ncount);
	SystemSlot *arr = new SystemSlot[ncount];
	if (!arr)
		return FALSE;
	BOOL ret = GetSystemSlots(arr, &ncount);
	if (ret)
	{
		for (int i = 0; i<ncount; i++)
			vec.push_back(arr[i]);
	}
	delete [] arr;
	return ret;
}
inline BOOL  GetCPUInfo(vector<CPUdll> &vec)
{
	int ncount = 0;
	GetCPUInfo(NULL, &ncount);
	CPUdll *arr = new CPUdll[ncount];
	if (!arr)
		return FALSE;
	BOOL ret = GetCPUInfo(arr, &ncount);
	if (ret)
	{
		for (int i = 0; i<ncount; i++)
			vec.push_back(arr[i]);
	}
	delete [] arr;
	return ret;
}

//////////////////////////////////////////////////////////////////////////


#endif





#endif
