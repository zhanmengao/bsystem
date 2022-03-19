#include "stdafx.h"
#include "GCDirFile.h"
#include <vector>


GCDirFile::GCDirFile(void)
{
}


GCDirFile::~GCDirFile(void)
{
}

BOOL GCDirFile::CreateMDirectory( const CString& szPath )
{

	CString strDir(szPath);//存放要创建的目录字符串
	
	//确保以'\'结尾以创建最后一个目录
	if (strDir.GetAt(strDir.GetLength()-1)!=_T('\\'))
	{
		strDir.AppendChar(_T('\\'));
	}

	std::vector<CString> vPath;//存放每一层目录字符串
	CString strTemp;//一个临时变量,存放目录字符串
	
	bool bSuccess = false;//成功标志
	
	//遍历要创建的字符串
	for (int i=0;i<strDir.GetLength();++i)
	{
		if (strDir.GetAt(i) != _T('\\')) 
		{//如果当前字符不是'\\'
			strTemp.AppendChar(strDir.GetAt(i));
		}
		else 
		{//如果当前字符是'\\'
			vPath.push_back(strTemp);//将当前层的字符串添加到数组中
			strTemp.AppendChar(_T('\\'));
		}
	}

	//遍历存放目录的数组,创建每层目录
	std::vector<CString>::const_iterator vIter;
	for (vIter = vPath.begin(); vIter != vPath.end(); vIter++) 
	{
		//如果CreateDirectory执行成功,返回true,否则返回false
		bSuccess = CreateDirectory(*vIter, NULL) ? true : false;    
	}

	return bSuccess;
}

BOOL GCDirFile::DeleteDirectory( CString directory_path )   //删除一个文件夹下的所有内容
{   
	CFileFind finder;
	CString path;
	path.Format( _T("%s/*.*"),directory_path );
	BOOL bWorking = finder.FindFile( path );

	while(bWorking){
		bWorking = finder.FindNextFile();
		if( finder.IsDirectory() && !finder.IsDots() ){//处理文件夹
			DeleteDirectory(finder.GetFilePath()); //递归删除文件夹
			RemoveDirectory(finder.GetFilePath());
		}
		else{//处理文件
			DeleteFile( finder.GetFilePath() );
		}
	}

	if ( ! DirectoryExist( directory_path ) )
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}


BOOL GCDirFile::DirectoryExist( CString strPath )
{
	//CString strPath = lpszPath;
	WIN32_FIND_DATA wfd;
	BOOL rValue = FALSE;
	HANDLE hFind = FindFirstFile( strPath, &wfd );
	if ((hFind!=INVALID_HANDLE_VALUE) && ( wfd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY) )
	{
		rValue = TRUE;
	}
	FindClose(hFind);

	return rValue;
}

//
BOOL GCDirFile::DirectoryEmpty( CString strPath )
{
	return PathIsDirectoryEmpty( strPath );
}

BOOL GCDirFile::FileDirExist( CString strPath )
{
	//功能：检查文件/路径是否存在
	return PathFileExists( strPath );
		
}

CString GCDirFile::GetPathFileName( CString strPath )
{
	
	//功能：获得路径中的文件名
	//例如： PathFileFileName( “c:\\Program Files\\File.txt” ) = “File.txt”。
	//如果路径中不包含文件名，则返回值是最下面的目录名。

	return PathFindFileName( strPath );

}

CString GCDirFile::GetFileExtName( CString strPath )
{
	
	//功能：获取路径或文件名中的文件扩展名
	//例如：PathFindExtension( “File.txt” ) = “.txt”
	return PathFindExtension( strPath );
}


CString GCDirFile::GetPathNextComponent( CString strPath )
{
	
	//例如：PathFindNextComponent( “c:\\Program Files\\Directory” ) = “Program Files\\Directory”
	//PathFindNextComponent( “c:\\Program Files” ) = “Program Files”
	return PathFindNextComponent( strPath );
}


CString GCDirFile::RemovePathArgs( CString pszPath )
{
	//功能：删除路径中带有的参数

	CString tmp = pszPath;
	
	PathRemoveArgs( tmp.GetBuffer() );
	tmp.ReleaseBuffer();
	
	return tmp;
	
}

CString GCDirFile::RemovePathBackslash( CString pszPath )
{
	//功能：删除路径结尾多余的’\’符号
	 PathRemoveBackslash( pszPath.GetBuffer() );
	 pszPath.ReleaseBuffer();

	 return pszPath;
}

void GCDirFile::RemovePathBlanks( LPTSTR pszPath )
{
	//功能：删除字符串头尾的空格
	PathRemoveBlanks( pszPath );
}

void GCDirFile::RemoveFileExt( LPTSTR pszPath )
{
	//功能：删除路径后面的文件扩展名
	PathRemoveExtension( pszPath );

}

CString GCDirFile::GetFilePathStr( LPTSTR pszPath )
{
	//功能：删除路径后面的文件名和’\’符号。该函数可以分析出一个文件的路径。
	//例：szPath = “c:\\windows\\system32\\nt.dll” ;
	//调用PathRemoveFileSpec( szPath ) 后，szPath = “c:\\windows\\system32” 

	 if ( PathRemoveFileSpec( pszPath ) )
	 {
		 return (CString)pszPath;
	 }  
	 else
	 {
	     return _T("Error:Path");
	 }

}

CString GCDirFile::GetPathDriveName( LPTSTR pszPath )
{
	//功能：从路径中分析出盘符

	if( PathStripToRoot(  pszPath ) )
	{
		return (CString)pszPath;

	}
	else
	{
		return _T("Error:Path");
	}
    
}

/*
void GCDirFile::RemoveFileExt( LPTSTR pszPath )
{
	//void PathUnquoteSpaces( LPTSTR lpsz );
    //功能：从带引号的路径中取出路径
	//例：szPath = “\”c:\\Program Files\”” 
	//PathUnquoteSpaces( szPath ) => szPath = “c:\\Program Files”
}
*/

BOOL GCDirFile::CreateDesktopShotCut( CString strName, CString strSourcePath )
{
	if(FAILED(CoInitialize(NULL)))
	{
		return FALSE;
	}
	int i;
	WCHAR Path[MAX_PATH+1];
	CString strDestDir;
	i=CSIDL_DESKTOPDIRECTORY;
	LPITEMIDLIST pidl;
	LPMALLOC pShell;
	if(SUCCEEDED(SHGetMalloc(&pShell)))
	{
		if(SUCCEEDED(SHGetSpecialFolderLocation(NULL,i,&pidl)))
		{
			if(!SHGetPathFromIDList(pidl,Path))
			{
				pShell->Free(pidl);
				::CoUninitialize();
				return FALSE;
			}
			pShell->Release();
			strDestDir.Format( _T("%s"),Path );
			strDestDir+="\\";
			strDestDir+=strName;//设置桌面快捷方式的名字
			strDestDir+=".lnk";
			IShellLink* psl;
			if(SUCCEEDED(CoCreateInstance
				(CLSID_ShellLink,NULL,CLSCTX_INPROC_SERVER,IID_IShellLink,(LPVOID*)&psl)))
			{
				psl->SetPath(strSourcePath);//设置快捷方式的目标位置
				//比如目标位置为C：\windows\a.txt 起始位置就应该设置为C：\windows否则会导致不可预料的错误
				//如果是文件夹的快捷方式起始位置和目标位置可以设置为一样
				psl->SetWorkingDirectory(strSourcePath);   //设置快捷方式的起始位置
				IPersistFile* ppf;

				if(SUCCEEDED(psl->QueryInterface(IID_IPersistFile,(LPVOID*)
					&ppf)))
				{

					
				//	WCHAR wsz[MAX_PATH];
				//	MultiByteToWideChar
				//		(CP_THREAD_ACP,MB_PRECOMPOSED,strDestDir.GetBuffer(),-1,wsz,MAX_PATH);//设置桌面快捷方式的名字
					CString wsz = strDestDir;
					if(SUCCEEDED(ppf->Save(wsz,TRUE)))//保存快捷方式到桌面
					{
						ppf->Release();
						psl->Release();
						::CoUninitialize();
						return TRUE;
					}else
					{
						ppf->Release();
						psl->Release();
						::CoUninitialize();
						return FALSE;
					}
				}else
				{
					ppf->Release();
					psl->Release();
					::CoUninitialize();
					return FALSE;
				}
			}else
			{
				::CoUninitialize();
				return FALSE;
			}
		}else
		{
			::CoUninitialize();
			return FALSE;
		}
	}else
	{
		::CoUninitialize();
		return FALSE;
	}
}
