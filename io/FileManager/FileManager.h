#pragma once
#include<io.h>
#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include <Windows.h>
#include"../../../../C++space/StringTools/StringTools/StringHead.h"
#include<Shlwapi.h>
#include"FileAttrHelper.h"
using namespace std;

#define TEST_ROOT "D:\\Space"
class FileInfo
{
public:
	unsigned    attrib;
	__time64_t  time_create;    // -1 for FAT file systems
	__time64_t  time_access;    // -1 for FAT file systems
	__time64_t  time_write;
	_fsize_t    size;
	std::string name;
	FileInfo()
	{
	}
	FileInfo(const _finddata_t& obj):name(obj.name),attrib(obj.attrib),
		time_create(obj.time_create), time_access(obj.time_access), time_write(obj.time_write), size(obj.size)
	{
	}
	FileInfo& operator=(const _finddata_t& obj)
	{
		name = obj.name;
		attrib = obj.attrib;
		time_create = obj.time_create;
		time_access = obj.time_access;
		time_write = obj.time_write;
		size = obj.size;
	}
};
struct FolderInfo
{
	std::vector<FileInfo> infolist;                 //file info
	std::vector<FolderInfo> folderList;             //folder info
	FileInfo info;                                  //my info
};
class FileManager
{
public:
	static bool GetAllFileInfo(std::vector<FileInfo> &outFileList);
	static bool GetAllFileInfo(const string& path,std::vector<FileInfo> &outFileList);
	static bool GetAllFileInfo(const string& path, FolderInfo& dir);          //Recursion Call Get All Folder File

	static bool CopyDirectory(const std::string& src,const std::string& dst);
	static bool CopyDirectory(const FolderInfo& folder, const std::string& dst);
	static BOOL CreatedMultipleDirectory(const std::string& direct);					//创建多级目录
	static int CreatDir(const TCHAR *pDir);
	static bool CreateFloder(const string& floderName);

	static bool  RemoveFloder(const string& floderName);
	static bool RemoveEmptyFloder(const string& floderName);

	static bool RenameFile(const string& old, const string& newName);

	static bool _CreateFile(const string& filePath, 
		char* data,unsigned int len);

	static bool RemoveFile(const string& filePath);

	static BOOL CpFile(const std::string& src, const std::string& dest);
public:
	//A_ARCH（存档）、 _A_HIDDEN（隐藏）、
	//_A_NORMAL（正常）、_A_RDONLY（只读）、
	//_A_SUBDIR（文件夹）、_A_SYSTEM（系统）

	static bool IsFileExist(const TCHAR* path)
	{
		return PathFileExists(path);
	}
};


/*
SetFileAttributes(文件名， FILE_ATTRIBUTE_READONLY); // 设定为只读
SetFileAttributes(文件名， FILE_ATTRIBUTE_HIDDEN );       //设定为隐藏
SetFileAttributes(文件名， FILE_ATTRIBUTE_SYSTEM);      //设定为系统
SetFileAttributes(文件名， FILE_ATTRIBUTE_ARCHIVE);   //设定为保存
SetFileAttributes(文件名， FILE_ATTRIBUTE_NORMAL);     //设定为一般 (取消前四种属性)
设定二种以上的属性：
设定为只读 + 隐藏
SetFileAttributes(文件名， FILE_ATTRIBUTE_READONLY | FILE_ATTRIBUTE_HIDDEN);
设定为只读 + 隐藏 + 系统 + 保存
SetFileAttributes(文件名， FILE_ATTRIBUTE_READONLY | FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM | FILE_ATTRIBUTE_ARCHIVE);
取消所有设定
SetFileAttributes(文件名， FILE_ATTRIBUTE_NORMAL);
*/
