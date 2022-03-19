#pragma once
#include<Windows.h>
#include<string>
#include"../../../../C++space/StringTools/StringTools/StringHead.h"
class FileAttrHelper
{
public:
	static BOOL SetFileHidden(const stdstr& filePath, bool bHidden);
	static BOOL SetFileOnlyRead(const stdstr& filePath, bool bOnlyRead);
	static BOOL SetFileNormal(const stdstr& filePath);
	static bool IsFolder(unsigned attr);
	static bool IsHidden(unsigned attr);
	static bool IsOnlyRead(unsigned attr);
	static bool IsBinary(const std::string& nPath);
};
/*
#define GENERIC_READ                     (0x80000000L)
#define GENERIC_WRITE                    (0x40000000L)
#define GENERIC_EXECUTE                  (0x20000000L)
#define GENERIC_ALL                      (0x10000000L)
*/
