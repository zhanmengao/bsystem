#pragma once
#include"CtrlWmi.h"
#include"WMIDef.h"
#include<mutex>
class WMIUtils
{
private:
	CCtrlWmi mConnect;
	BOOL m_bConnected = FALSE;
	std::mutex mLock;
public:
	WMIUtils() :m_bConnected(0)
	{
	}
	BOOL Init();
	BOOL Begin();
	BOOL End();
	void Destroy();
	HRESULT GetLastErrorWMI();
	BOOL GetDiskInfo(vector<WMIDisk> &vec, std::string& exce);
	BOOL GetMemorySlots(vector<WMIMemory> &vec, std::string& exce);
	BOOL GetKeyBoardInfo(vector<WMIKeyboard> &vec, std::string& exce);
	BOOL GetMonitors(vector<WMIMonitor> &vec, std::string& exce);
	BOOL GetPointerInfo(vector<WMIPointer> &vec, std::string& exce);
	BOOL GetPrinterInfo(vector<WMIPrint>& vec, std::string& exce);
	BOOL GetDesktop(vector<WMIDesktop>& vec, std::string& exce);
	BOOL GetPatch(vector<WMIPatchInfo>& vec, std::string& exce);
	BOOL GetBIOS(vector<WMIBIOS>& vec, std::string& exce);
	BOOL GetMainBoard(vector<WMIMainBoard>& vec, std::string& exce);
	BOOL GetCPU(vector<WMICPU>& vec, std::string& exce);
	BOOL GetComputerSystem(vector<WMIComputerSystem>& vec, std::string& exce);
	BOOL GetProcess(std::map<DWORD,WMIProcess>& map, std::string& exce);
	std::string GetDesktopWallpaper(std::string& exce);
	
};
