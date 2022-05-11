#include "stdafx.h"
#include "7zfileTool.h"
#include <Windows.h>
#include <iostream>
#include "GStringClass.h"
#include "md5.h"
#include "json\json.h"

unsigned long long WINAPI GetTimeTickFrom19700101()
{
	SYSTEMTIME  systime;
	::GetLocalTime(&systime);
	time_t curr_t = time(NULL);
	unsigned long long tld = (unsigned long long)curr_t;
	unsigned long millSec = (unsigned long)(systime.wMilliseconds);
	tld = tld * 1000 + millSec;
	return tld;
}


char* wide_Char_To_Multi_Byte(wchar_t* pWCStrKey)
{
	//第一次调用确认转换后单字节字符串的长度，用于开辟空间
	int pSize = WideCharToMultiByte(CP_OEMCP, 0, pWCStrKey, wcslen(pWCStrKey), NULL, 0, NULL, NULL);
	char* pCStrKey = new char[pSize + 1];

	//第二次调用将双字节字符串转换成单字节字符串
	WideCharToMultiByte(CP_OEMCP, 0, pWCStrKey, wcslen(pWCStrKey), pCStrKey, pSize, NULL, NULL);
	pCStrKey[pSize] = '\0';
	return pCStrKey;

	//如果想要转换成string，直接赋值即可
	//string pKey = pCStrKey;
}

wchar_t* multi_Byte_To_Wide_Char(string pKey)
{
	//string 转 char*
	char* pCStrKey = (char*)pKey.c_str();

	//第一次调用返回转换后的字符串长度，用于确认为wchar_t*开辟多大的内存空间
	int pSize = MultiByteToWideChar(CP_OEMCP, 0, pCStrKey, strlen(pCStrKey) + 1, NULL, 0);
	wchar_t *pWCStrKey = new wchar_t[pSize];

	//第二次调用将单字节字符串转换成双字节字符串
	MultiByteToWideChar(CP_OEMCP, 0, pCStrKey, strlen(pCStrKey) + 1, pWCStrKey, pSize);

	return pWCStrKey;
}


int EFiles( int ClientSocket, string filelist ,string& zipsavePath   )
{
	
	//string fjStr = GStringClass::replace_all(filelist, "\\\\", "\\");
	//fjStr = GStringClass::replace_all(filelist, "\\", "/");

	Json::Reader reader;
	Json::Value  resp;
	
	string filelistS = "";
	string saveZipFile = "";
	if (reader.parse(filelist, resp, false))
	{
		Json::Value filePaths = resp["filePaths"];
		int sz = filePaths.size();		
		for (int i = 0; i < sz; ++i)
		{

			string tmpfilename = filePaths[i].asString();
			
			
			if ( PathFileExists(multi_Byte_To_Wide_Char(tmpfilename)) )
			{
				filelistS += "\""+ tmpfilename + "\"" + " ";
				printf("Append: %s\n", tmpfilename.c_str());
			}
			else
			{
				printf( "PathFileExists Error: %s \n", tmpfilename.c_str());
			}
			
		}

		saveZipFile = resp["targetPaths"].asString();

	}
	else
	{
		printf("JSON From Error\n");
	}

	if (filelistS.length() == 0)
	{
		printf("all files exists\n");
		return -2;
	}
	
	
	HMODULE module = GetModuleHandle(0);
	wchar_t pFileName[MAX_PATH];
	GetModuleFileName(module, pFileName, MAX_PATH);

	string csFullPath  = wide_Char_To_Multi_Byte(pFileName);
	int nPos = csFullPath.rfind('\\');
	csFullPath = csFullPath.substr(0,nPos);

	unsigned long long tmpTimeK = GetTimeTickFrom19700101();
	char itc[20] = {0};
	sprintf(itc, "%llu.zip", tmpTimeK );
	string zipTimeFile = itc;

	saveZipFile = saveZipFile + zipTimeFile;


	string  szCommandLine = csFullPath + "\\7z.exe a -tzip "+
		+" "+ saveZipFile + " "+
		filelistS;
	

	wchar_t *WszCommandLine = multi_Byte_To_Wide_Char(szCommandLine);
	
	STARTUPINFO        SI;
	PROCESS_INFORMATION PI;

	memset(&SI, 0, sizeof(SI));
	SI.cb = sizeof(SI);
	memset(&PI, 0, sizeof(PI));

	BOOL bSuccess = CreateProcess(NULL, // No module name (use command line). 
		WszCommandLine,      // Command line. 
		NULL,               // Process handle not inheritable. 
		NULL,               // Thread handle not inheritable. 
		FALSE,              // Set handle inheritance to FALSE. 
		0,                  // No creation flags. 
		NULL,               // Use parent's environment block. 
		NULL,               // Use parent's starting directory. 
		&SI,                // Pointer to STARTUPINFO structure.
		&PI);               // Pointer to PROCESS_INFORMATION structure.

	DWORD dwExitCode = 0;

	if (!bSuccess)
	{
		DWORD ERROR0 = GetLastError();
	}
	else
	{
		//DWORD t2 = GetTickCount();

		WaitForSingleObject(PI.hProcess, INFINITE);
		
		if (GetExitCodeProcess(PI.hProcess, &dwExitCode))
		{
			if ( dwExitCode == 0 )
			{

				zipsavePath = saveZipFile;
			}
			else
			{

				zipsavePath = "Error:-1";
			}

		}
		DWORD t1 = GetTickCount();
		//cout << "convert consume " << t2 - t1 << "ms" << endl;	
	}

	CloseHandle(PI.hThread);
	CloseHandle(PI.hProcess);
	

	return dwExitCode;
}




