#pragma once
#include<Windows.h>
#include<vector>
#include<string>
using namespace std;
class RDPServerUtils
{
public:
	//RDP Server
	static LONG  SetRDPPort(int rdpPort);
	static DWORD GetRDPPort();

	static LONG CleanProDir();
	static LONG CleanLibLink();
	static LONG ResetLibLink();

	static LONG HideThisPCPolicy();
	static LONG ShowThisPCPolicy();

	static LONG SetfSingleSessionPerUser(DWORD value);
	static LONG SetfSingleSessionPerUser2(DWORD value);
	static LONG SetfDisabledAllowList(DWORD value);
	static LONG SetLicenseServers(wchar_t* server);
	static LONG SetLicensingMode(DWORD mode);
	static LONG SetNoViewOnDrive();
	static LONG SetNoDrives();
	static LONG ClearNoViewOnDrive();
	static LONG ClearNoDrives();
	static LONG SetUserAuthentication(DWORD mode);
	static LONG SetRemoteAppLogoffTimeLimit(DWORD mode);
	static LONG SetEnableRemoteFXAdvancedRemoteApp(DWORD mode);
	static LONG SetMaxInstanceCount(DWORD mode);

	static LONG GetLicenseServers(wchar_t* server);
	static LONG GetUserAuthentication();
	static LONG GetRemoteAppLogoffTimeLimit();
	static LONG GetMaxInstanceCount();
	static LONG GetEnableRemoteFXAdvancedRemoteApp();

	static LONG SetEnableLogonScriptDelay(DWORD mode);
	static LONG SetNoControlPanel(DWORD mode);
	static LONG SetExplorerWindows(DWORD mode);
	static LONG SetNoViewContextMenu(DWORD mode);
	static LONG SetDisableCMD(DWORD mode);

	static LONG SetKOneDrive(DWORD mode);

	static LONG SetCADKetCall(DWORD mode);

	static LONG SetLockScreenImage(const wstring& bmpPath);



	static LONG DelAppInitDLLs();
	static LONG SetAppInitDLLs(const wstring& filePath_x86, const wstring& filePath_x64);
	static std::vector<wstring> GetAppInitDLLs();

	static LONG DelScriptsStartup0();
	static LONG SetScriptsStartup0(const wstring& filePath_x86, const wstring& filePath_x64);
	static std::vector<wstring> GetScriptsStartup0();
	static LONG SetScriptsLogon0(const wstring& filePath);
	static wstring GetScriptsLogon0();
	static wstring GetScriptsLogon(int id);
	static LONG SetScriptsLogon(const wstring& filePath, int id = 0);

	static BOOL GetNoControlPanel();
	static BOOL GetfSingleSessionPerUser();
	static BOOL GetfDisabledAllowList();
	static BOOL GetNoDrives();
	static BOOL GetNoViewOnDrive();
	static BOOL GetEnableLogonScriptDelay();
	static BOOL GetLibLink();
	static BOOL GetThisPCPolicy();
	static BOOL GetNoViewContextMenu();
	static LONG GetExplorerWindows();
	static LONG GetDisableCMD();
	static LONG GetCADKetCall();
	static LONG GetLockScreenImage(__out wchar_t * getbmpPath);
	static LONG GetKOneDrive();
	static LONG SetTEST();

private:

	static LONG GetRegEdit(wchar_t *lpSubKey, wchar_t* lpValueName, wchar_t* szValue, DWORD& dValue, HKEY MinhKey = HKEY_LOCAL_MACHINE);
	static LONG GetRegEditWin32(wchar_t * lpSubKey, wchar_t * lpValueName, wchar_t * szValue, DWORD& dValue, HKEY MinhKey = HKEY_LOCAL_MACHINE);
	static LONG SetRegEdit(wchar_t *lpSubKey, wchar_t* lpValueName, DWORD dwType, wchar_t* szValue, DWORD dValue);

	static LONG SetRegEditWin32(wchar_t * lpSubKey, wchar_t * lpValueName, DWORD dwType, wchar_t * szValue, DWORD dValue);

	static LONG WriteThisPCPolicyRegKey(wchar_t* wCLSID, wchar_t* wValue);
	static LONG WriteShellFolderRegKey(wchar_t* wCLSID, DWORD wValue);
	static LONG SetComputerPolicies(wchar_t* gpoKey, DWORD dwType, wchar_t* name, wchar_t* valtr, DWORD valdword);
	static LONG SetUserPolicies(wchar_t * gpoKey, DWORD dwType, wchar_t * name, wchar_t * valstr, DWORD valdword);
};