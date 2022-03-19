// OSFileManager.h: 标准系统包含文件的包含文件
// 或项目特定的包含文件。

#pragma once

#include <iostream>
#include<string>
#include<vector>
#ifdef _WIN32
#include"../FileManager/FileManager/FileManager.h"
#else
#include"../FileManager/LFileManager/FIleManager.h"
#endif
class OSFileInfo
{
public:
	std::string fileName;
	size_t fileSize;
	bool bDir;
#ifdef _WIN32
	OSFileInfo(const FileInfo& fileInfo):fileName(fileInfo.name)
	{	
	}
	OSFileInfo& operator=(const FileInfo& fileInfo)
	{
		fileName = fileInfo.name;
	}
#else
#endif
};
class OSFileManager
{
public:
	static bool GetAllFileInfo(const char* nPath, std::vector<OSFileInfo>& dir);
};
