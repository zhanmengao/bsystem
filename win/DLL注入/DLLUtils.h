#pragma once
#include<Windows.h>
#include "tlhelp32.h"
#include<stdio.h>
#include"../../../C++space/StringTools/StringTools/StringHead.h"

#define LOAD "LoadLibraryA"
#define FREE "FreeLibrary"
#include<tchar.h>
#include"ExpDefine.h"



class DLLUtils
{
public:
	static void InjectDLLThread(DWORD pid, const char* dllPath);								//��DLLע��ָ������
	static void InjectDLLThread(DWORD pid, RemoteThreadProc proc);						//��dll ֱ��ע������
	static void InjectDLLAPC(DWORD pid, const char* dllPath);								//��DLLע��ָ������
	static void FreeAllDLL(const TCHAR* inDllPath);
	static DWORD FreeDLL(DWORD pid, const TCHAR* inDllPath);							//��DLL��ָ����������
	// ʹ�� ZwCreateThreadEx ʵ��Զ�߳�ע��
	static BOOL ZwCreateThreadExInjectDll(DWORD dwProcessId, char *pszDllFileName);

	//����DLL���޳���
	static BOOL CreateDLL(const HANDLE& hToken, const stdstr& dllPath,STARTUPINFO& si, PROCESS_INFORMATION& pi);
	//DLL
	static LPVOID Malloc(HANDLE hProcess, SIZE_T nSize);
	static BOOL WriteMemory(HANDLE hProcess, LPVOID to, LPVOID from, SIZE_T nSize);
	static HMODULE LoadDLL(const TCHAR* dPath);
	static FARPROC GetProc(HMODULE hDLL, const char* procName);
	static HMODULE GetModule(const TCHAR* mName);						//��ȡģ������ͨ��ģ������
	static HMODULE GetModuleEx(PCTSTR func);														//��ȡ��ǰ����ģ����
	static stdstr _GetModuleFileName(HMODULE hModule);					//��ȡģ�������·��
public:
	//�޸�PEB�е�·������������Ϣ��ʵ�ֽ���αװ
	static BOOL DisguiseProcess(DWORD dwProcessID, TCHAR* lpwszPath, TCHAR* lpwszCmd);
	//�������ܽ���
	static BOOL ReplaceProcess(const TCHAR *pszFilePath, PVOID pReplaceData, 
		DWORD dwReplaceDataSize, DWORD dwRunOffset);
};