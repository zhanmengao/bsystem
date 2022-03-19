#pragma once
#include"FileManager.h"
#include <iostream>
#include <windows.h>
#include <string>
#include<vector>
#include"PathHelper.hpp"
#include"../../../../C++space/Convert/Convert/Convert.h"
#include"DiskManager.h"
#include"FileAttrHelper.h"
using namespace std;
void PrinfFileInfo(const _File& rFile)
{
	if (FileAttrHelper::IsFolder(rFile.attrib))
	{
		cout << "FileName:" << rFile.fileName << endl;
	}
	else
	{
		cout << "FileName:" << rFile.fileName << "	Size:" << rFile.fileSize << endl;
	}
}
void PrintFIlesInfo()
{
	wstring curDir = PathHelper::GetCurDir();
	wcout << L"Current Path:" << curDir << endl;
	std::vector<_File> outFileList;
	DiskManager::GetAllFileInfo(outFileList);
	for (int i = 0; i < outFileList.size(); i++)
	{
		PrinfFileInfo(outFileList[i]);
	}
}

//ºóÍË
void Back()
{
	DiskManager::Back();
	PrintFIlesInfo();
}

void To(const wstring& fileName)
{
	DiskManager::To(fileName);
	PrintFIlesInfo();
}