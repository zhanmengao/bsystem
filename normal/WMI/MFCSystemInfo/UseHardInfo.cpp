// UseHardInfo.cpp : Defines the entry point for the console application.
//

#include "../EasyWarInfo/WarInfo.h"
#pragma comment(lib,"EasyWarInfo.lib")
#pragma comment(lib, "Ole32.lib")
#define VC_EXTRALEAN        // 从 Windows 头中排除极少使用的资料
#define MINNUM 5

int main(int argc, TCHAR* argv[])
{
	HRESULT m_hResult = CoInitializeEx(0, COINIT_MULTITHREADED);
	m_hResult = CoInitializeSecurity(NULL, -1, NULL, NULL,
		RPC_C_AUTHN_LEVEL_PKT_PRIVACY,
		RPC_C_IMP_LEVEL_IMPERSONATE,
		NULL,
		EOAC_SECURE_REFS, //change to EOAC_NONE if you change dwAuthnLevel to RPC_C_AUTHN_LEVEL_NONE
		NULL);
	BeginGetHardInfo();
	OSdll stOS;
	memset(&stOS, 0, sizeof(OSdll));
	GetOS(stOS);
	//MessageBox(0, L"OS", 0, 0);
	
	printf("---------------------OS-----------------------\n");
	printf("OS:\n");
	printf("\tcsName:  %s",stOS.csName);printf("\n");
	printf("\tcsComment:  %s",stOS.csComment);printf("\n");
	printf("\tcsVersion:  %s",stOS.csVersion);printf("\n");
	printf("\tcsDescription:  %s",stOS.csDescription);printf("\n");
	printf("\n");

	//cpu info
	printf("---------------------CPU-----------------------\n");
	char szCpu[128];
	if(GetCpuInfo(szCpu,sizeof(szCpu)) == S_OK)
	{
		printf("CPU:\n");
		printf("\t%s",szCpu);
	}
	printf("\n");
	//MessageBox(0, L"CPU", 0, 0);
	

	vector<SoundDevicedll> stSoundDevice;
	stSoundDevice.clear();
	stSoundDevice.reserve(MINNUM);
	GetSoundDevices(stSoundDevice);
	//MessageBox(0, L"SoundDevice", 0, 0);
	vector<SoundDevicedll>::iterator citorSoundDevice;
	printf("---------------------SoundDevice-----------------------\n");
	for(citorSoundDevice = stSoundDevice.begin(); citorSoundDevice != stSoundDevice.end(); citorSoundDevice++)
	{
		printf("SoundDevice:\n");
		printf("\tcsManufacturer:  %s.",citorSoundDevice->csManufacturer);printf("\n");
		printf("\tcsName:  %s",citorSoundDevice->csName);printf("\n");
		printf("\tcsDescription:  %s",citorSoundDevice->csDescription);printf("\n");
	}
	printf("\n");
	stSoundDevice.clear();

	vector<Biosdll> stBios;
	stBios.clear();
	stBios.reserve(MINNUM);
	GetBiosInfo(stBios);
	//MessageBox(0, L"Bios", 0, 0);
	vector<Biosdll>::iterator citorBios;
	printf("---------------------Bios-----------------------\n");
	for(citorBios = stBios.begin(); citorBios != stBios.end(); citorBios++)
	{
		printf("Bios:\n");
		printf("\tcsManufacturer:  %s.",citorBios->csManufacturer);printf("\n");
		printf("\tcsModel:  %s",citorBios->csModel);printf("\n");
	}
	printf("\n");
	stBios.clear();

	vector<MemorySlotdll> stMemorySlot;
	stMemorySlot.clear();
	stMemorySlot.reserve(MINNUM);
	GetMemorySlots(stMemorySlot);
	//MessageBox(0, L"MemorySlot", 0, 0);
	vector<MemorySlotdll>::iterator citorMemorySlot;
	printf("---------------------MemorySlot-----------------------\n");
	for(citorMemorySlot = stMemorySlot.begin(); citorMemorySlot != stMemorySlot.end(); citorMemorySlot++)
	{
		printf("MemorySlot:\n");
		printf("\tcsCaption:  %s",citorMemorySlot->csCaption);printf("\n");
		printf("\tcsDeviceLocator:  %s",citorMemorySlot->csDeviceLocator);printf("\n");
		printf("\tcsCapacity:  %s",citorMemorySlot->csCapacity);printf("\n");
		printf("\tcsType:  %s",citorMemorySlot->csType);printf("\n");
		printf("\tuSlotNumber:  %d",citorMemorySlot->uSlotNumber);printf("\n");
		printf("\tcsSpeed:  %sMHz",citorMemorySlot->csSpeed);printf("\n");
		printf("\tcsDescription:  %s",citorMemorySlot->csDescription);printf("\n");
		printf("\tcsName:  %s",citorMemorySlot->csName);printf("\n");
		printf("\tcsProductDate:  %s",citorMemorySlot->csProductDate);printf("\n");
		printf("\tcsSerialNum:  %s",citorMemorySlot->csSerialNum);printf("\n");
	}
	printf("\n");
	stMemorySlot.clear();

	vector<Monitordll> stMonitor;
	stMonitor.clear();
	stMonitor.reserve(MINNUM);
	GetMonitors(stMonitor);
	//MessageBox(0, L"Monitor", 0, 0);
	vector<Monitordll>::iterator citorMonitor;
	printf("---------------------Monitor-----------------------\n");
	for(citorMonitor = stMonitor.begin(); citorMonitor != stMonitor.end(); citorMonitor++)
	{
		printf("Monitor:\n");
		printf("\tcsManufacturer:  %s",citorMonitor->csManufacturer);printf("\n");
		printf("\tcsModel:  %s",citorMonitor->csModel);printf("\n");
		printf("\tcsType:  %s",citorMonitor->csType);printf("\n");
		printf("\tcsCaption:  %s",citorMonitor->csCaption);printf("\n");
		printf("\tcsDescription:  %s",citorMonitor->csDescription);printf("\n");
		printf("\tcsSerialNum:  %s",citorMonitor->csSerialNum);printf("\n");
		printf("\tcsMonitorSize:  %s",citorMonitor->csMonitorSize);printf("\n");
	}
	printf("\n");
	stMonitor.clear();

	//Sleep(20000);
	vector<NetworkAdaptertdll> stNetworkAdaptert;
	stNetworkAdaptert.clear();
	stNetworkAdaptert.reserve(MINNUM);
	GetNetworkAdapters(stNetworkAdaptert);
	//MessageBox(0, L"NetworkAdaptert", 0, 0);
	vector<NetworkAdaptertdll>::iterator citorNetworkAdaptert;
	printf("---------------------NetworkAdaptert-----------------------\n");
	for(citorNetworkAdaptert = stNetworkAdaptert.begin(); citorNetworkAdaptert != stNetworkAdaptert.end(); citorNetworkAdaptert++)
	{
		printf("NetworkAdaptert:\n");
		printf("\tcsName:  %s",citorNetworkAdaptert->csName);printf("\n");
		printf("\tcsDescription:  %s",citorNetworkAdaptert->csDescription);printf("\n");
		printf("\tcsManufacturer:  %s",citorNetworkAdaptert->csManufacturer);printf("\n");
		printf("\tcsProductName:  %s",citorNetworkAdaptert->csProductName);printf("\n");
	}
	printf("\n");
	stNetworkAdaptert.clear();

	vector<Printerdll> stPrinter;
	stPrinter.clear();
	stPrinter.reserve(MINNUM);
	GetPrinters(stPrinter);
	//MessageBox(0, L"Printer", 0, 0);
	vector<Printerdll>::iterator citorPrinter;
	printf("---------------------Printer-----------------------\n");
	for(citorPrinter = stPrinter.begin(); citorPrinter != stPrinter.end(); citorPrinter++)
	{
		printf("Printer:\n");
		printf("\tcsName:  %s",citorPrinter->csName);printf("\n");
		printf("\tcsDeviceName:  %s",citorPrinter->csDeviceName);printf("\n");
		printf("\tcsPortName:  %s",citorPrinter->csPortName);printf("\n");
	}
	printf("\n");
	stPrinter.clear();

	vector<StoragePeripheraldll> stStoragePeripheral;
	stStoragePeripheral.clear();
	stStoragePeripheral.reserve(MINNUM);
	GetStoragePeripherals(stStoragePeripheral);
	//MessageBox(0, L"StoragePeripheral", 0, 0);
	vector<StoragePeripheraldll>::iterator citorStoragePeripheral;
	printf("---------------------StoragePeripheral-----------------------\n");
	for(citorStoragePeripheral = stStoragePeripheral.begin(); citorStoragePeripheral != stStoragePeripheral.end(); citorStoragePeripheral++)
	{
		printf("StoragePeripheral:\n");
		printf("\tcsType:  %s",citorStoragePeripheral->csType);printf("\n");
		printf("\tcsManufacturer:  %s",citorStoragePeripheral->csManufacturer);printf("\n");
		printf("\tcsInterfaceType:  %s",citorStoragePeripheral->csInterfaceType);printf("\n");
		printf("\tcsName:  %s",citorStoragePeripheral->csName);printf("\n");
		printf("\tcsModel:  %s",citorStoragePeripheral->csModel);printf("\n");
		printf("\tcsSize:  %s MB",citorStoragePeripheral->csSize);printf("\n");
		printf("\tcsDescription:  %s",citorStoragePeripheral->csDescription);printf("\n");
	}
	printf("\n");
	stStoragePeripheral.clear();

	vector<VideoAdapterdll> stVideoAdapter;
	stVideoAdapter.clear();
	stVideoAdapter.reserve(MINNUM);
	GetVideoAdapters(stVideoAdapter);
	//MessageBox(0, L"VideoAdapter", 0, 0);
	vector<VideoAdapterdll>::iterator citorVideoAdapter;
	printf("---------------------VideoAdapter-----------------------\n");
	for(citorVideoAdapter = stVideoAdapter.begin(); citorVideoAdapter != stVideoAdapter.end(); citorVideoAdapter++)
	{
		printf("VideoAdapter:\n");
		printf("\tcsName:  %s",citorVideoAdapter->csName);printf("\n");
		printf("\tcsMemory:  %s",citorVideoAdapter->csMemory);printf("\n");
		printf("\tcsProcessor:  %s",citorVideoAdapter->csProcessor);printf("\n");
		printf("\tcsDescription:  %s",citorVideoAdapter->csDescription);printf("\n");
	}
	printf("\n");
	stVideoAdapter.clear();

	vector<KeyBoarddll> stKeyBoard;
	stKeyBoard.clear();
	stKeyBoard.reserve(MINNUM);
	GetKeyBoards(stKeyBoard);
	//MessageBox(0, L"KeyBoard", 0, 0);
	vector<KeyBoarddll>::iterator citorKeyBoard;
	printf("---------------------KeyBoard-----------------------\n");
	for(citorKeyBoard = stKeyBoard.begin(); citorKeyBoard != stKeyBoard.end(); citorKeyBoard++)
	{
		printf("KeyBoard:\n");
		printf("\tcsName:  %s",citorKeyBoard->csName);printf("\n");
		printf("\tcsDescription:  %s",citorKeyBoard->csDescription);printf("\n");
		printf("\tcsDeviceID:  %s",citorKeyBoard->csDeviceID);printf("\n");
		printf("\tcsName:  %s",citorKeyBoard->csName);printf("\n");
		printf("\tcsLayout:  %s",citorKeyBoard->csLayout);printf("\n");
	}
	printf("\n");
	stKeyBoard.clear();

	vector<PointingDevicedll> stPointingDevice;
	stPointingDevice.clear();
	stPointingDevice.reserve(MINNUM);
	GetPointingDevices(stPointingDevice);
	//MessageBox(0, L"PointingDevice", 0, 0);
	vector<PointingDevicedll>::iterator citorPointingDevice;
	printf("---------------------PointingDevice-----------------------\n");
	for(citorPointingDevice = stPointingDevice.begin(); citorPointingDevice != stPointingDevice.end(); citorPointingDevice++)
	{
		printf("PointingDevice:\n");
		printf("\tcsName:  %s",citorPointingDevice->csName);printf("\n");
		printf("\tcsDescription:  %s",citorPointingDevice->csDescription);printf("\n");
		printf("\tcsHardwareType:  %s",citorPointingDevice->csHardwareType);printf("\n");
		printf("\tcsName:  %s",citorPointingDevice->csName);printf("\n");
		printf("\tcsManufacturer:  %s",citorPointingDevice->csManufacturer);printf("\n");
		printf("\tcsPointingType:  %s",citorPointingDevice->csPointingType);printf("\n");
	}
	printf("\n");
	stPointingDevice.clear();

	EndGetHardInfo();
	system("pause");

	return 0;
}

