#include"DiskManager.h"
#include"PathHelper.hpp"
void DiskManager::Back()
{
	//�����ǰ�Ѿ��Ǹ�Ŀ¼������Ϊ����root״̬
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
		//���Խ���ĳ���̷�
		SetCurrentDirectory(fileName.c_str());
		//���Ƿ����ɹ�����δ��������root
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
	DWORD dwLen = GetLogicalDriveStrings(0, NULL);	//��ȡϵͳ�ַ�������.
	TCHAR * pszDriver = new TCHAR[dwLen];				//����һ����Ӧ���ȵ�����.
	TCHAR * pHead = pszDriver;				//����һ����Ӧ���ȵ�����.
	GetLogicalDriveStrings(dwLen, pszDriver);					//��ȡ�̷��ַ���.
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
		//Ϊ����ʾÿ����������״̬����ͨ��ѭ�����ʵ�֣�����DStr�ڲ������������A:\NULLB:\NULLC:\NULL����������Ϣ������DSLength/4���Ի�þ����ѭ����Χ
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