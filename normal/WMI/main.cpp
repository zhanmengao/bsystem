#include"WMI.h"
#include"WMIUtils.h"

#define CHECK_ARRAY(name,i,arr) if(arr.size() <=0)\
{\
printf("%s %d error \n",name,i);\
}\

int _tmain(int argc, _TCHAR* argv[])
{
	CWMI wmi;
	wmi.InitWMI();
	wmi.QuerySystemInfo();
	wmi.ClearWMI();

	vector<WMIQueryData> vquery;
	CWMI::GetDiskInfo(vquery);
	printf("\nGetDiskInfo--------------------------------------------------\n");
	CWMI::PrintWMIVec(vquery);

	CWMI::GetWin32_LogicDisk(vquery);
	printf("\nGetWin32_LogicDisk-------------------------------------------------\n");
	CWMI::PrintWMIVec(vquery);
	

	CWMI::GetMemoryInfo(vquery);
	printf("\nmoney--------------------------------------------------\n");
	CWMI::PrintWMIVec(vquery);

	CWMI::GetDesktop(vquery);
	printf("\nDesktop--------------------------------------------------\n");
	//CWMI::PrintWMIVec(vquery);

	CWMI::GetMonitor(vquery);
	printf("\nMonitor--------------------------------------------------\n");
	CWMI::PrintWMIVec(vquery);

	CWMI::GetKeyBoard(vquery);
	printf("\nKeyboard--------------------------------------------------\n");
	CWMI::PrintWMIVec(vquery);

	CWMI::GetMouse(vquery);
	printf("\nMouse--------------------------------------------------\n");
	CWMI::PrintWMIVec(vquery);

	CWMI::GetPrint(vquery);
	printf("\nPrint--------------------------------------------------\n");
	CWMI::PrintWMIVec(vquery);

	CWMI::GetDesktop(vquery);
	printf("\nDesktop--------------------------------------------------\n");
	//CWMI::PrintWMIVec(vquery);
	
	CWMI::GetWin32_Process(vquery);
	printf("\GetWin32_Process--------------------------------------------------\n");
	//CWMI::PrintWMIVec(vquery);

	CWMI::GetWin32_BIOS(vquery);
	printf("\nGetWin32_BIOS--------------------------------------------------\n");
	CWMI::PrintWMIVec(vquery);

	CWMI::GetWin32_BaseBoard(vquery);
	printf("\nGetWin32_BaseBoard--------------------------------------------------\n");
	CWMI::PrintWMIVec(vquery);

	CWMI::GetWin32_CPU(vquery);
	printf("\GetWin32_CPU--------------------------------------------------\n");
	CWMI::PrintWMIVec(vquery);
	
	CWMI::GetWin32_ComputerSystem(vquery);
	printf("\nGetWin32_ComputerSystem--------------------------------------------------\n");
	CWMI::PrintWMIVec(vquery);

	CWMI::GetWin32_OS(vquery);
	printf("\nGetWin32_OS--------------------------------------------------\n");
	CWMI::PrintWMIVec(vquery);

	CWMI::GetWin32_Fan(vquery);
	printf("\nGetWin32_Fan--------------------------------------------------\n");
	//CWMI::PrintWMIVec(vquery);
	
	CWMI::GetWin32_TemperatureProbe(vquery);
	printf("\nGetWin32_TemperatureProbe --------------------------------------------------\n");
	//CWMI::PrintWMIVec(vquery);

	
	WMIUtils utils;
	utils.Init();
	utils.Begin();
	for (int i = 0; i < 100; i++)
	{
		string err;
		vector<WMIDisk> vecdisk;
		vector<WMIMemory> vecmemory;
		vector<WMIKeyboard> veckey;
		vector<WMIPointer> vecpointer;
		vector<WMIPrint> vecprint;
		vector<WMIDesktop> vecDesktop;
		vector<WMIMonitor> vecMonitor;
		vector<WMIPatchInfo> vecPatch;
		vector<WMIBIOS> vecBIOS;
		vector<WMIMainBoard> vecMainBoard;
		vector<WMICPU> vecCPU;
		std::map<DWORD, WMIProcess> promap;
		utils.GetDiskInfo(vecdisk, err);
		CHECK_ARRAY("GetDiskInfo", i, vecdisk);
		utils.GetMemorySlots(vecmemory, err);
		CHECK_ARRAY("GetMemorySlots", i, vecmemory);
		utils.GetKeyBoardInfo(veckey, err);
		CHECK_ARRAY("GetKeyBoardInfo", i, veckey);
		utils.GetPointerInfo(vecpointer, err);
		CHECK_ARRAY("GetPointerInfo", i, vecpointer);
		utils.GetPrinterInfo(vecprint, err);
		CHECK_ARRAY("GetPrinterInfo", i, vecprint);
		utils.GetDesktop(vecDesktop, err);
		CHECK_ARRAY("GetDesktop", i, vecDesktop);
		utils.GetMonitors(vecMonitor, err);
		CHECK_ARRAY("GetMonitors", i, vecMonitor);
		utils.GetPatch(vecPatch, err);
		CHECK_ARRAY("GetPatch", i, vecPatch);
		utils.GetBIOS(vecBIOS, err);
		CHECK_ARRAY("GetBIOS", i, vecBIOS);
		utils.GetMainBoard(vecMainBoard, err);
		CHECK_ARRAY("GetMainBoard", i, vecMainBoard);
		utils.GetCPU(vecCPU, err);
		CHECK_ARRAY("GetCPU", i, vecCPU);
		utils.GetProcess(promap, err);
		CHECK_ARRAY("GetProcess", i, promap);
		//cout << utils.GetDesktopWallpaper() << endl;
	}
	utils.End();
	utils.Destroy();
	char buf[1024] = { 0 };
	while (1)
	{
		cin.getline(buf,_countof(buf));
		CWMI::Query(buf, vquery);
		CWMI::PrintWMIVec(vquery);
	}
	system("pause");
	return 0;
}