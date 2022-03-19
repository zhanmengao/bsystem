#pragma once
#include"FileManager.h"
#include"PathHelper.hpp"

struct _File
{
	std::string fileName;
	unsigned long long fileSize;
	unsigned    attrib;
};

struct Disk
{
public:
	stdstr diskName;
	unsigned long long TotalSize;
	unsigned long long freeSize;
	int diskType;
};
class DiskTools
{
public:
	static vector<Disk> GetDiskInfo();
};

//  ≈‰∆˜
class DiskManager
{
public:
	static stdstr GetPath();
	static int GetAllFileInfo(std::vector<_File> &outFileList);

	static bool CreateFloder(const string& floderName);

	static bool RemoveFloder(const string& floderName);

	static bool RenameFile(const string& old, const string& newName);

	static bool _CreateFile(const string& filePath,
		char* data, unsigned int len);

	static bool RemoveFile(const string& filePath);
	static void Back();
	static void To(const stdstr& fileName);
	static void OpenDir(const TCHAR* path);
private:
	static bool bRoot;
};