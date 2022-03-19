/*
提供简化的路径操作：如exe路径、系统路径、上级目录
*/

#ifndef PATH_HELPER_H  
#define PATH_HELPER_H  
#pragma once
#include <stdio.h>
#include <shlobj.h>
#include <string>
#include<wchar.h>
#include<Windows.h>
#include<tchar.h>
#include"../../../../C++space/Convert/Convert/Convert.h"
#include"../../../../C++space/StringTools/StringTools/StringHead.h"
#include <Psapi.h>
#pragma comment (lib,"Psapi.lib")
using namespace std;
#pragma   comment(lib,   "shell32.lib")

class PathHelper
{
public:
	//得到上一级目录  
	static stdstr GetUpFolder(const stdstr& strPath)
	{
		stdstr ret;
		int nPos = strPath.find_last_of('\\');
		ret = stdstr(strPath, 0, nPos);
		if (nPos == strPath.size() - 1)
		{
			nPos = stdstr(strPath, 0, nPos - 1).find_last_of(_T("\\"));
			ret = stdstr(strPath, 0, nPos);
		}
		//如果已经退到root
		if (ret.size() <= 2)
		{
			ret += _T("\\");
		}
		return ret;
	}

	//获取运行目录(exe所在目录)  
	static stdstr GetModuleFolder(HMODULE handle = NULL)
	{
		TCHAR   szPath[MAX_PATH] = { 0 };
		GetModuleFileName(handle, szPath, MAX_PATH);
		ZeroMemory(_tcsrchr(szPath, _T('\\')), _tcslen(_tcsrchr(szPath, _T('\\'))) * sizeof(TCHAR));
		stdstr ws(szPath);
		return ws + _T("\\");
	}

	static stdstr GetCurDir()
	{
		TCHAR wChar[1024] = _T("\n");
		GetCurrentDirectory(1024, wChar);
		stdstr ws(wChar);
		return ws +_T("\\");
	}
	static void Back()
	{
		stdstr curDir = PathHelper::GetCurDir();
		stdstr upDir = PathHelper::GetUpFolder(curDir);
		if (curDir != upDir)
		{
			SetCurrentDirectory(upDir.c_str());
		}
	}
	static void To(const stdstr& fileName)
	{
		//绝对路径
		if (fileName.find(_T(':')) != stdstr::npos)
		{
			SetCurrentDirectory(fileName.c_str());
		}
		//相对路径
		else
		{
			stdstr tFilePath = PathHelper::GetCurDir() + fileName;
			SetCurrentDirectory(tFilePath.c_str());
		}
	}
	//获取运行模块文件全路径  
	static stdstr GetModuleFilePath(HMODULE handle = NULL)
	{
		TCHAR   szPath[MAX_PATH] = { 0 };
		GetModuleFileName(handle, szPath, MAX_PATH);
		return szPath;
	}
	//获取运行模块文件全路径  
	static stdstr GetModuleFilePath(HANDLE nHandle)
	{
		TCHAR   szPath[MAX_PATH] = { 0 };
		GetModuleFileNameEx(nHandle, NULL, szPath, MAX_PATH);
		return szPath;
	}
	//①获取系统system32路径：（c:\windows\system32)
	static stdstr GetSysFolder()
	{
		TCHAR szPath[100] = { 0 };
		GetSystemDirectory(szPath, 100);
		return szPath;
	}

	//获取系统windows路径：（c:\windows）
	static stdstr GetWindowsFolder()
	{
		TCHAR szPath[100] = { 0 };
		GetWindowsDirectory(szPath, 100);
		stdstr ws(szPath);
		return ws + _T("\\");
	}
	static stdstr GetStarUp()
	{
		TCHAR szChar[MAX_PATH] = { 0 };
		SHGetSpecialFolderPath(NULL, szChar, CSIDL_STARTUP, FALSE);
		return stdstr(szChar) + _T("\\");
	}

	static stdstr GetAppData()
	{
		TCHAR szChar[MAX_PATH] = { 0 };
		SHGetSpecialFolderPath(NULL, szChar, CSIDL_APPDATA, FALSE);
		return stdstr(szChar) + _T("\\");
	}
	static stdstr GetDesktop()
	{
		TCHAR szChar[MAX_PATH] = { 0 };
		SHGetSpecialFolderPath(NULL, szChar, CSIDL_DESKTOPDIRECTORY, FALSE);
		return stdstr(szChar) + _T("\\");
	}
	static stdstr GetSendTo()
	{
		TCHAR szChar[MAX_PATH] = { 0 };
		SHGetSpecialFolderPath(NULL, szChar, CSIDL_SENDTO, FALSE);
		return stdstr(szChar) + _T("\\");
	}
	static stdstr GetDocumentation()
	{
		TCHAR szChar[MAX_PATH] = { 0 };
		SHGetSpecialFolderPath(NULL, szChar, CSIDL_PERSONAL, FALSE);
		return stdstr(szChar) + _T("\\");
	}
	static stdstr GetProgram()
	{
		TCHAR szChar[MAX_PATH] = { 0 };
		SHGetSpecialFolderPath(NULL, szChar, CSIDL_PROGRAM_FILES, FALSE);
		return stdstr(szChar) + _T("\\");
	}

	static stdstr GetCommnAppData()
	{
		TCHAR szChar[MAX_PATH] = { 0 };
		SHGetSpecialFolderPath(NULL, szChar, CSIDL_COMMON_APPDATA, FALSE);
		return stdstr(szChar) + _T("\\");
	}
	
	static stdstr GetCommnProgram()
	{
		TCHAR szChar[MAX_PATH] = { 0 };
		SHGetSpecialFolderPath(NULL, szChar, CSIDL_PROGRAM_FILES_COMMON, FALSE);
		return stdstr(szChar) + _T("\\");
	}
	


	static stdstr GetUserFolder()
	{
		TCHAR szChar[MAX_PATH] = { 0 };
		GetEnvironmentVariable(_T("USERPROFILE"), szChar, MAX_PATH);
		return stdstr(szChar) + _T("\\");
	}

	static stdstr GetTemp()
	{
		TCHAR szChar[MAX_PATH] = { 0 };
		GetEnvironmentVariable(_T("TMP"), szChar, MAX_PATH);
		return szChar;
	}
	//%HOMEDRIVE%{ 当前用户根目录 - C: }
	//	%USERPROFILE%{ 当前用户目录 - C:\Documents and Settings\wy }
	//	%HOMEPATH%{ 当前用户路径 - \Documents and Settings\wy }
	//	%TMP%{ 当前用户临时文件夹 - C:\DOCUME~1\wy\LOCALS~1\Temp }
	//	%TEMP%{ 当前用户临时文件夹 - C:\DOCUME~1\wy\LOCALS~1\Temp }
	//	%APPDATA%{ 当前用户数据文件夹 - C:\Documents and Settings\wy\Application Data }
	//	%PROGRAMFILES%{ 程序默认安装目录 - C:\Program Files }
	//	%COMMONPROGRAMFILES%{ 文件通用目录 - C:\Program Files\Common Files }
	//	%ALLUSERSPROFILE%{ 所有用户文件目录 - C:\Documents and Settings\All Users }

	static stdstr GetEnvironment(const stdstr& lpName)
	{
		TCHAR szChar[MAX_PATH] = { 0 };
		GetEnvironmentVariable(lpName.c_str(), szChar, MAX_PATH);
		return szChar;
	}
	static stdstr GetSpecialPath(int fileType)
	{
		TCHAR   szPath[MAX_PATH] = { 0 };
		SHGetSpecialFolderPath(NULL, szPath, fileType, false);
		return szPath;
	}
	static stdstr GetDriveDir(const stdstr& drive)
	{
		TCHAR szChar[MAX_PATH] = { 0 };
		DWORD dwRet = GetFullPathName(drive.c_str(), MAX_PATH, szChar, NULL);
		return szChar;
	}
};


#endif