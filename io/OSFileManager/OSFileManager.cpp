// OSFileManager.cpp: 定义应用程序的入口点。
//

#include "OSFileManager.h"
using namespace std;


bool OSFileManager::GetAllFileInfo(const char* nPath, std::vector<OSFileInfo>& dir)
{
#ifdef _WIN32
	std::vector<FileInfo> outFileList;
	bool bRet = FileManager::GetAllFileInfo(nPath, outFileList);
	dir.resize(outFileList.size());
	for (auto it = outFileList.begin(); it != outFileList.end(); it++)
	{
		dir.push_back(*it);
	}
	return bRet;
#else
#endif
 }
