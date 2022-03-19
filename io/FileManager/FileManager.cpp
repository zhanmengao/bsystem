#include"FileManager.h"
#include<direct.h>
#include"PathHelper.hpp"
#include"../../../../C++space/Convert/Convert/CppConvert.h"


bool FileManager::GetAllFileInfo(std::vector<FileInfo> &outFileList)
{
#ifdef _UNICODE
	return GetAllFileInfo(CppConvert::UnicodeToUTF8(PathHelper::GetCurDir()), outFileList);
#else
	return GetAllFileInfo(PathHelper::GetCurDir(), outFileList);
#endif
}
bool FileManager::GetAllFileInfo(const string& nPath, std::vector<FileInfo> &outFileList)
{
	string path = nPath;
	_finddata_t fileInfo;
	intptr_t handle = _findfirst(path.assign(nPath).append(("\\*")).c_str(), &fileInfo);
	if (handle == -1L)
	{
		cerr << "GetAllFileInfo fail,Path = " << path << endl;
		DWORD dwRet = GetLastError();
		return false;
	}
	do
	{
		if (strcmp(fileInfo.name, ".") == 0 || strcmp(fileInfo.name, "..") == 0)
		{
		}
		else
		{
			outFileList.push_back(fileInfo);
		}
	} while (_findnext(handle, &fileInfo) == 0);
	_findclose(handle);
	return true;
}
bool FileManager::GetAllFileInfo(const string& nPath, FolderInfo& dir)          //Recursion Call Get All Folder File
{
	dir.info.name = nPath;
	std::vector<FileInfo> outFileList;
	bool bRet = GetAllFileInfo(nPath,outFileList);
	for (auto it = outFileList.begin(); it != outFileList.end(); it++)
	{
		if (FileAttrHelper::IsFolder(it->attrib))
		{
			//递归
			FolderInfo sub;
			sub.info = *it;
			GetAllFileInfo(nPath+"\\" + sub.info.name, sub);
			dir.folderList.push_back(sub);
		}
		else
		{
			dir.infolist.push_back(*it);
		}
	}
	return bRet;
}
bool FileManager::CreateFloder(const string& floderName)
{
	int ret = _mkdir(floderName.c_str());
	if (ret == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}
int FileManager::CreatDir(const TCHAR *pDir)
{
	int i = 0;
	int iRet;
	int iLen;
	if (NULL == pDir)
	{
		return 0;
	}
	TCHAR szDir[MAX_PATH] = { 0 };
	_tcscpy_s(szDir, _countof(szDir), pDir);
	TCHAR* pszDir;

	pszDir = _tcsdup(szDir);
	
	iLen = _tcslen(pszDir);

	
	if (_taccess_s(pszDir, 0) == 0)
	{
		return 0;
	}

	// 创建中间目录
	for (i = 0; i < iLen; i++)
	{
		if (pszDir[i] == '\\' || pszDir[i] == '/')
		{
			pszDir[i] = '\0';

			//如果不存在,创建
			iRet = _taccess_s(pszDir, 0);
			if (iRet != 0)
			{		
				iRet = _tmkdir(pszDir);
				if (iRet != 0)
				{
					return -1;
				}
			}
			//支持linux,将所有\换成/
			pszDir[i] = '/';
		}
	}

	iRet = _tmkdir(pszDir);
	free(pszDir);

	return iRet;
}
bool FileManager::RemoveEmptyFloder(const string& floderName)
{
	if (_rmdir(floderName.c_str()) == 0)
	{
		return true;
	}
	return false;
}
bool FileManager::CopyDirectory(const std::string& src, const std::string& dst)
{
	FolderInfo folder;
	bool bRet = GetAllFileInfo(src, folder);
	if (!bRet)
	{
		return false;
	}
	return CopyDirectory(folder, dst);
}
bool FileManager::CopyDirectory(const FolderInfo& folder, const std::string& dst)
{
	//创建目录CP
	CreatedMultipleDirectory(dst);
	for (auto it = folder.infolist.begin(); it != folder.infolist.end(); it++)
	{
		CpFile(folder.info.name + "\\" + it->name, dst + "\\" + it->name);
	}
	for (auto it = folder.folderList.begin(); it != folder.folderList.end(); it++)
	{
		//拿到子目录的相对路径
		std::string path = it->info.name;
		size_t nPos = path.find(folder.info.name);
		std::string sub = path.substr(nPos + folder.info.name.size(), path.size());
		path = dst + sub;
		CopyDirectory(*it, path);
	}
	return true;
}
BOOL FileManager::CpFile(const std::string& src, const std::string& dest)
{
#ifdef _WIN32
	return CopyFileA(src.c_str(), dest.c_str(), FALSE);
#endif
}
bool  FileManager::RemoveFloder(const string& floderName)
{
	std::vector<FileInfo> outFileLis;
	FileManager::GetAllFileInfo(floderName, outFileLis);
	for (auto it = outFileLis.begin(); it != outFileLis.end(); it++)
	{
		char sTempName[200];
		sprintf(sTempName, "%s\\%s", floderName.c_str(), it->name.c_str());
		if (FileAttrHelper::IsFolder(it->attrib))
		{
			RemoveFloder(sTempName);
		}
		else
		{
			FileManager::RemoveFile(sTempName);
		}
	}
	return FileManager::RemoveEmptyFloder(floderName);
}
bool FileManager::RenameFile(const string& old, const string& newName)
{
	int ret = rename(old.c_str(), newName.c_str());
	if (ret == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}
bool FileManager::RemoveFile(const string& filePath)
{
	int ret = remove(filePath.c_str());
	if (ret == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool FileManager::_CreateFile(const string& filePath,
	char* data, unsigned int len)
{
	int ret = -1;
	ofstream ofs;
	ofs.open(filePath, ios::out);
	if (!ofs.is_open())
	{
		return ret;
	}
	else
	{
		ofs.write(data, len);
		ofs.close();
	}
	return true;
}

BOOL FileManager::CreatedMultipleDirectory(const std::string& direct)
{
	std::string Directoryname = direct;
	if (Directoryname[Directoryname.length() - 1] != '\\')
	{
		Directoryname.append(1, '\\');
	}
	std::vector< std::string> vpath;
	std::string strtemp;
	BOOL  bSuccess = FALSE;
	for (int i = 0; i < Directoryname.length(); i++)
	{
		if (Directoryname[i] != '\\')
		{
			strtemp.append(1, Directoryname[i]);
		}
		else
		{
			vpath.push_back(strtemp);
			strtemp.append(1, '\\');
		}
	}
	std::vector< std::string>::iterator vIter = vpath.begin();
	for (; vIter != vpath.end(); vIter++)
	{
		bSuccess = CreateDirectoryA(vIter->c_str(), NULL) ? TRUE : FALSE;
	}
	return bSuccess;
}

