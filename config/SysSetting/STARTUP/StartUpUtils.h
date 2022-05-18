#pragma once
#include<Windows.h>
#include"../../../File/FileManager/FileManager/PathHelper.hpp"
#include"../../../File/FileManager/FileManager/FileManager.h"
#include"../../../../C++space/StringTools/StringTools/StringHead.h"
#include"../../../../C++space/StringTools/StringTools/StrUtils.h"
#include"../../../../C++space/Convert/Convert/Convert.h"
class StartUpUtils
{
public:
	static BOOL StartUPAutoRun(const TCHAR* srcFilePath,const TCHAR* lpszDestFileName)
	{
		stdstr startDir = PathHelper::GetSpecialPath(CSIDL_STARTUP);
		TCHAR szDestFilePath[MAX_PATH] = { 0 };
		wsprintf(szDestFilePath, _T("%s\\%s"), startDir.c_str(), lpszDestFileName);											//����Ŀ��·��
		return CopyFile(srcFilePath, szDestFilePath, FALSE);																			//�����ļ�����������Ŀ¼��
	}
	static BOOL GetStartUpList(std::vector<string>& vec)
	{
		stdstr startDir = PathHelper::GetSpecialPath(CSIDL_STARTUP) + _T("\\");
		//��ȡ���������ļ�
		std::vector<_finddata_t> outFileList;
#ifdef _UNICODE
		bool bRet = FileManager::GetAllFileInfo(Convert::WStringToString(startDir), outFileList);
#else
		bool bRet  = FileManager::GetAllFileInfo(startDir, outFileList);
#endif
		for (auto it = outFileList.begin(); it != outFileList.end(); it++)
		{
			if (StrUtils::_strstr(it->name,"exe"))
			{
				vec.push_back(it->name);
			}
		}
		return bRet;
	}
};