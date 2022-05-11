#pragma once
#include"SysDef.h"


class SysInfoUtils
{
public:
	static SYSTEM_INFO GetSystemInfo();														//��ȡϵͳ��Ϣ								
	static stdstr GetUser();																					//��ȡ�û���
	static stdstr GetHostName();																		//��ȡ������
	static stdstr _GetComputerName();																		//��ȡ������
	static stdstr GetPCName();																			//��ȡ������
	static struct WindowsInfo GetWindowsInfo();											//��ȡwindows��Ϣ
	static struct WindowsVersion GetWindowsVersion();										//��ȡ�汾��Ϣ
	static net_info GetNetInfo();																		//��ȡ������Ϣ
	static BOOL GetAdapterInfo(vector<AdapterInfo>& vec);						//��ȡ������Ϣ
	static std::string GetMac(const char* ipaddr);												//����ip��ȡmac
	static net_info GetNetGateInfo();
	static int GetTCPportlist(std::vector<TCPPort>& vec);								//ɨ��TCP�˿�
	static int GetUDPportlist(std::vector<UDPPort>& vec);								//ɨ��UDP�˿�
	static int GetEventLog(std::vector<EventLog>& vec);								//��ȡWindows��־
public:
	inline static stdstr GetRegisteredOwner()												//��ȡע��������
	{
		return OsInfo(_T("RegisteredOwner"));
	}
	inline static stdstr GetProductName()															//��ȡ��Ʒ����
	{
		return OsProductInfo(_T("SystemProductName"));
	}
	inline static stdstr GetProductID()															//��ȡ��ƷID
	{
		return OsProductInfo(_T("ProductId"));
	}
	inline static stdstr GetCPU()																		//��ȡCPU����
	{
		return OsCpuInfo(_T("ProcessorNameString"));
	}
	inline static stdstr GetOSName()																	//��ȡ����ϵͳ����
	{
		return OsInfo(_T("ProductName"));
	}
	inline static stdstr GetOSVersion()																//����ϵͳ�汾
	{
		return OsInfo(_T("CurrentVersion"));
	}
	inline static stdstr GetOSReleaseID()																//����ϵͳ����ID
	{
		return OsInfo(_T("ReleaseId"));
	}
	inline static stdstr GetOSCurrBuild()													//����ϵͳ�ڲ��汾
	{
		return OsInfo(_T("CurrentBuild"));
	}
	inline static DWORD GetOSInstallDate()													//ϵͳ��װ����
	{
		return DWOsInfo(_T("InstallDate"));
	}
private:
	static stdstr OsProductInfo(const TCHAR* str);
	static stdstr OsCpuInfo(const TCHAR* str);
	static stdstr OsInfo(const TCHAR* str);
	static DWORD DWOsInfo(const TCHAR* str);
	static ULONG GetHostOrder(const std::string& strIp);
	static BOOL RegGetIP(AdapterInfo *pAI, const char* lpszAdapterName, int nIndex = 0);
};