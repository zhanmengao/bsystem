#include"DiskManager.h"
#include"PathHelper.hpp"
void DiskManager::Back()
{
	//如果当前已经是根目录，则视为进入root状态
	stdstr curDir = PathHelper::GetCurDir();
	stdstr upDir = PathHelper::GetUpFolder(curDir);
	if (curDir != upDir)
	{
		PathHelper::Back();
	}
	else
	{
		bRoot = true;
	}
}
void DiskManager::To(const stdstr& fileName)
{
	if (bRoot)
	{
		stdstr curDir = PathHelper::GetCurDir();
		//尝试进入某个盘符
		SetCurrentDirectory(fileName.c_str());
		//看是否进入成功，如未进入则还在root
		if (curDir != PathHelper::GetCurDir())
		{
			bRoot = false;
		}
	}
	else
	{
		PathHelper::To(fileName);
	}
}
void DiskManager::OpenDir(const TCHAR* path)
{
	ShellExecute(NULL, _T("open"), path, NULL, NULL, SW_SHOWNORMAL);
}
stdstr DiskManager::GetPath()
{
	if (bRoot)
	{
		return _T("root");
	}
	else
	{
		return 	PathHelper::GetCurDir();
	}
}
int DiskManager::GetAllFileInfo(std::vector<_File> &outFileList)
{
	outFileList.clear();
	if (bRoot)
	{
		auto disVec = DiskTools::GetDiskInfo();
		for (auto it = disVec.begin(); it != disVec.end(); it++)
		{
			_File nFile;
#ifdef _UNICODE
			nFile.fileName = Convert::WStringToString(it->diskName);
#else
			nFile.fileName = (it->diskName);
#endif
			nFile.attrib = it->diskType;
			nFile.fileSize = it->TotalSize - it->freeSize;
			outFileList.push_back(nFile);
		}
		return 2;
	}
	else
	{
		std::vector<FileInfo> fList;
		bool bRet = FileManager::GetAllFileInfo(fList);
		for (auto it = fList.begin(); it != fList.end(); it++)
		{
			_File nFile;
			nFile.attrib = it->attrib;
			nFile.fileName = it->name;
			nFile.fileSize = it->size;
			outFileList.push_back(nFile);
		}
		return 1;
	}
}

bool DiskManager::CreateFloder(const string& floderName)
{
	if (bRoot)
	{
		return false;
	}
	return FileManager::CreateFloder(floderName);
}

bool DiskManager::RemoveFloder(const string& floderName)
{
	if (bRoot)
	{
		return false;
	}
	return FileManager::RemoveFloder(floderName);
}

bool DiskManager::RenameFile(const string& old, const string& newName)
{
	if (bRoot)
	{
		return false;
	}
	return FileManager::RenameFile(old, newName);
}

bool DiskManager::_CreateFile(const string& filePath, char* data, unsigned int len)
{
	if (bRoot)
	{
		return false;
	}
	return FileManager::_CreateFile(filePath, data, len);
}


bool DiskManager::RemoveFile(const string& filePath)
{
	if (bRoot)
	{
		return false;
	}
	return FileManager::RemoveFile(filePath);
}

bool DiskManager::bRoot = false;

vector<Disk> DiskTools::GetDiskInfo()
{
	vector<Disk> ret;
	DWORD dwLen = GetLogicalDriveStrings(0, NULL);	//获取系统字符串长度.
	TCHAR * pszDriver = new TCHAR[dwLen];				//构建一个相应长度的数组.
	TCHAR * pHead = pszDriver;				//构建一个相应长度的数组.
	GetLogicalDriveStrings(dwLen, pszDriver);					//获取盘符字符串.
	vector<stdstr> v;
	while (*pszDriver != '\0')
	{
		v.push_back(pszDriver);
		pszDriver += lstrlen(pszDriver) + 1;
	}
	if (pHead != nullptr)
	{
		delete[] pHead;
		pHead = nullptr;
		pszDriver = nullptr;
	}
	int DType;
	BOOL fResult;
	unsigned _int64 i64FreeBytesToCaller;
	unsigned _int64 i64TotalBytes;
	unsigned _int64 i64FreeBytes;

	for (int i = 0; i < v.size(); ++i)
		//为了显示每个驱动器的状态，则通过循环输出实现，由于DStr内部保存的数据是A:\NULLB:\NULLC:\NULL，这样的信息，所以DSLength/4可以获得具体大循环范围
	{
		DType = GetDriveType(v[i].c_str());
		fResult = GetDiskFreeSpaceEx(v[i].c_str(),
			(PULARGE_INTEGER)&i64FreeBytesToCaller,
			(PULARGE_INTEGER)&i64TotalBytes,
			(PULARGE_INTEGER)&i64FreeBytes);
		if (fResult)
		{
			Disk d;
			d.diskName = v[i];
			d.diskType = DType;
			d.TotalSize = i64TotalBytes;
			d.freeSize = i64FreeBytes;
			ret.push_back(d);
		}
	}
	return ret;
}