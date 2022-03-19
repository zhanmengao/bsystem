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

//����1
std::wstring GetProcessInfo(HANDLE hProcess, WCHAR* processName)
{
	PROCESSENTRY32* pinfo = new PROCESSENTRY32; //������Ϣ ��pinfo->dwSize = sizeof(PROCESSENTRY32);��
	MODULEENTRY32* minfo = new MODULEENTRY32; //ģ����Ϣ ��minfo->dwSize = sizeof(MODULEENTRY32);��
	WCHAR shortpath[MAX_PATH];				//����·������

	int flag = Process32First(hProcess, pinfo);	// �ӵ�һ�����̿�ʼ
	while (flag) 
	{

		// ����� QQMusic.exe �������
		if (lstrcmp(pinfo->szExeFile, processName) == 0) 
		{
			// �������̿���
			HANDLE hModule = CreateToolhelp32Snapshot(
				TH32CS_SNAPMODULE,		//��DWORD�� ���շ��صĶ���TH32CS_SNAPMODULE ��ָ "�ض����̵�ʹ��ģ����б�"
				pinfo->th32ProcessID	//��DWORD�� Ҫ��ȡ���ս��̵�PID����ǰ����/ϵͳ�б� ����ʱ��Ϊ0
			);

			// �ѵ�һ��ģ����Ϣ�� minfo
			Module32First(
				hModule,  //��HANDLE�� CreateToolhelp32Snapshot �ķ��ؾ��
				minfo	 // ��LPMODULEENTRY32��  ����ģ����Ϣ
			);

			// ���ļ�·���� shortpath
			GetShortPathName(
				minfo->szExePath,	//  �ļ�·��������ò�Ҫ���������Ϊ����������Ļ�������룩
				shortpath,		// �������� minfo->szExePath �������ĵ�ֵ
				256			// ��������С
			);

			return shortpath;
		}

		// ��һ������
		flag = Process32Next(hProcess, pinfo);
	}

	return NULL;
}

int main22()
{
	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0); // �������̿���
	PROCESSENTRY32 process = { sizeof(PROCESSENTRY32) };	// �������� hProcessSnap ����Ϣ

	// �������̿���
	while (Process32Next(hProcessSnap, &process))
	{
		// �ҵ� QQMusic.exe ����
		std::wstring processName = process.szExeFile; // char* ת string
		if (processName == L"QQMusic.exe") 
		{
			std::wstring s_exePath = GetProcessInfo(hProcessSnap, L"QQMusic.exe"); // ���̵�ȫ·�� 
			std::wcout << s_exePath << std::endl;
			break;
		}
	}

	return 0;
}

//����2
void main2222()
{
	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0); // �������̿���
	PROCESSENTRY32 process = { sizeof(PROCESSENTRY32) };	// �������� hProcessSnap ����Ϣ

															// �������̿���
	while (Process32Next(hProcessSnap, &process))
	{
		// ��ȡ������
		wstring processName = process.szExeFile;
		wcout << processName;

		// ��ȡȫ·��
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


//����3
BOOL DosPathToNtPath(LPTSTR pszDosPath, LPTSTR pszNtPath)
{
	TCHAR            szDriveStr[500];
	TCHAR            szDrive[3];
	TCHAR            szDevName[100];
	INT                cchDevName;
	INT                i;

	//������
	if (!pszDosPath || !pszNtPath)
		return FALSE;

	//��ȡ���ش����ַ���
	if (GetLogicalDriveStrings(sizeof(szDriveStr), szDriveStr))
	{
		for (i = 0; szDriveStr[i]; i += 4)
		{
			if (!lstrcmpi(&(szDriveStr[i]), _T("A:\\")) || !lstrcmpi(&(szDriveStr[i]), _T("B:\\")))
				continue;

			szDrive[0] = szDriveStr[i];
			szDrive[1] = szDriveStr[i + 1];
			szDrive[2] = '\0';
			if (!QueryDosDevice(szDrive, szDevName, 100))//��ѯ Dos �豸��
				return FALSE;

			cchDevName = lstrlen(szDevName);
			if (_tcsnicmp(pszDosPath, szDevName, cchDevName) == 0)//����
			{
				lstrcpy(pszNtPath, szDrive);//����������
				lstrcat(pszNtPath, pszDosPath + cchDevName);//����·��
				return TRUE;
			}
		}
	}

	lstrcpy(pszNtPath, pszDosPath);

	return FALSE;
}
//��ȡ��������·��
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