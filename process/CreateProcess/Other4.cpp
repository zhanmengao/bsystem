#include <stdio.h>
#include <windows.h>
#include <tlhelp32.h>
#include <string>
#include <tchar.h>
#include <Psapi.h>
#include<string>
#include<iostream>
using namespace std;
#pragma comment (lib,"Psapi.lib")

//方法1
std::wstring GetProcessInfo(HANDLE hProcess, WCHAR* processName)
{
	PROCESSENTRY32* pinfo = new PROCESSENTRY32; //进程信息 （pinfo->dwSize = sizeof(PROCESSENTRY32);）
	MODULEENTRY32* minfo = new MODULEENTRY32; //模块信息 （minfo->dwSize = sizeof(MODULEENTRY32);）
	WCHAR shortpath[MAX_PATH];				//保存路径变量

	int flag = Process32First(hProcess, pinfo);	// 从第一个进程开始
	while (flag) 
	{

		// 如果是 QQMusic.exe 这个进程
		if (lstrcmp(pinfo->szExeFile, processName) == 0) 
		{
			// 创建进程快照
			HANDLE hModule = CreateToolhelp32Snapshot(
				TH32CS_SNAPMODULE,		//（DWORD） 快照返回的对象，TH32CS_SNAPMODULE 是指 "特定进程的使用模块的列表"
				pinfo->th32ProcessID	//（DWORD） 要获取快照进程的PID，当前进程/系统列表 快照时设为0
			);

			// 把第一个模块信息给 minfo
			Module32First(
				hModule,  //（HANDLE） CreateToolhelp32Snapshot 的返回句柄
				minfo	 // （LPMODULEENTRY32）  接收模块信息
			);

			// 把文件路径给 shortpath
			GetShortPathName(
				minfo->szExePath,	//  文件路径（但最好不要用这个，因为这个碰到中文会出现乱码）
				shortpath,		// 用来接收 minfo->szExePath 兼容中文的值
				256			// 缓冲区大小
			);

			return shortpath;
		}

		// 下一个进程
		flag = Process32Next(hProcess, pinfo);
	}

	return NULL;
}

int main22()
{
	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0); // 创建进程快照
	PROCESSENTRY32 process = { sizeof(PROCESSENTRY32) };	// 用来接收 hProcessSnap 的信息

	// 遍历进程快照
	while (Process32Next(hProcessSnap, &process))
	{
		// 找到 QQMusic.exe 进程
		std::wstring processName = process.szExeFile; // char* 转 string
		if (processName == L"QQMusic.exe") 
		{
			std::wstring s_exePath = GetProcessInfo(hProcessSnap, L"QQMusic.exe"); // 进程的全路径 
			std::wcout << s_exePath << std::endl;
			break;
		}
	}

	return 0;
}

//方法2
void main2222()
{
	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0); // 创建进程快照
	PROCESSENTRY32 process = { sizeof(PROCESSENTRY32) };	// 用来接收 hProcessSnap 的信息

															// 遍历进程快照
	while (Process32Next(hProcessSnap, &process))
	{
		// 获取进程名
		wstring processName = process.szExeFile;
		wcout << processName;

		// 获取全路径
		WCHAR chpath[MAX_PATH];
		HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, process.th32ProcessID);
		if (!hProcess)
		{
			continue;
		}
		GetModuleFileNameEx(hProcess, NULL, chpath, sizeof(chpath));
		wcout <<"	"<<chpath << endl;
		CloseHandle(hProcess);
		//std::cout << "-------------------" << endl;
	}
}


//方法3
BOOL DosPathToNtPath(LPTSTR pszDosPath, LPTSTR pszNtPath)
{
	TCHAR            szDriveStr[500];
	TCHAR            szDrive[3];
	TCHAR            szDevName[100];
	INT                cchDevName;
	INT                i;

	//检查参数
	if (!pszDosPath || !pszNtPath)
		return FALSE;

	//获取本地磁盘字符串
	if (GetLogicalDriveStrings(sizeof(szDriveStr), szDriveStr))
	{
		for (i = 0; szDriveStr[i]; i += 4)
		{
			if (!lstrcmpi(&(szDriveStr[i]), _T("A:\\")) || !lstrcmpi(&(szDriveStr[i]), _T("B:\\")))
				continue;

			szDrive[0] = szDriveStr[i];
			szDrive[1] = szDriveStr[i + 1];
			szDrive[2] = '\0';
			if (!QueryDosDevice(szDrive, szDevName, 100))//查询 Dos 设备名
				return FALSE;

			cchDevName = lstrlen(szDevName);
			if (_tcsnicmp(pszDosPath, szDevName, cchDevName) == 0)//命中
			{
				lstrcpy(pszNtPath, szDrive);//复制驱动器
				lstrcat(pszNtPath, pszDosPath + cchDevName);//复制路径
				return TRUE;
			}
		}
	}

	lstrcpy(pszNtPath, pszDosPath);

	return FALSE;
}
//获取进程完整路径
BOOL GetProcessFullPath(DWORD dwPID, TCHAR pszFullPath[MAX_PATH])
{
	TCHAR        szImagePath[MAX_PATH];
	HANDLE        hProcess;
	if (!pszFullPath)
		return FALSE;
	pszFullPath[0] = '\0';
	hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, 0, dwPID);
	if (!hProcess)
		return FALSE;
	if (!GetProcessImageFileName(hProcess, szImagePath, MAX_PATH))
	{
		CloseHandle(hProcess);
		return FALSE;
	}
	if (!DosPathToNtPath(szImagePath, pszFullPath))
	{
		CloseHandle(hProcess);
		return FALSE;
	}
	CloseHandle(hProcess);
	_tprintf(_T("%d,%s \r\n"), dwPID, pszFullPath);
	return TRUE;
}
int main333(int argc, char* argv[])
{
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (INVALID_HANDLE_VALUE == hSnapshot)
	{
		return NULL;
	}
	PROCESSENTRY32 pe = { 0 };
	pe.dwSize = sizeof(PROCESSENTRY32);

	BOOL fOk;
	for (fOk = Process32First(hSnapshot, &pe); fOk; fOk = Process32Next(hSnapshot, &pe))
	{
		TCHAR szProcessName[MAX_PATH] = { 0 };
		GetProcessFullPath(pe.th32ProcessID, szProcessName);
	}
	return 0;
}