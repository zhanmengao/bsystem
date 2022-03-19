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

	CString strDir(szPath);//���Ҫ������Ŀ¼�ַ���
	
	//ȷ����'\'��β�Դ������һ��Ŀ¼
	if (strDir.GetAt(strDir.GetLength()-1)!=_T('\\'))
	{
		strDir.AppendChar(_T('\\'));
	}

	std::vector<CString> vPath;//���ÿһ��Ŀ¼�ַ���
	CString strTemp;//һ����ʱ����,���Ŀ¼�ַ���
	
	bool bSuccess = false;//�ɹ���־
	
	//����Ҫ�������ַ���
	for (int i=0;i<strDir.GetLength();++i)
	{
		if (strDir.GetAt(i) != _T('\\')) 
		{//�����ǰ�ַ�����'\\'
			strTemp.AppendChar(strDir.GetAt(i));
		}
		else 
		{//�����ǰ�ַ���'\\'
			vPath.push_back(strTemp);//����ǰ����ַ�����ӵ�������
			strTemp.AppendChar(_T('\\'));
		}
	}

	//�������Ŀ¼������,����ÿ��Ŀ¼
	std::vector<CString>::const_iterator vIter;
	for (vIter = vPath.begin(); vIter != vPath.end(); vIter++) 
	{
		//���CreateDirectoryִ�гɹ�,����true,���򷵻�false
		bSuccess = CreateDirectory(*vIter, NULL) ? true : false;    
	}

	return bSuccess;
}

BOOL GCDirFile::DeleteDirectory( CString directory_path )   //ɾ��һ���ļ����µ���������
{   
	CFileFind finder;
	CString path;
	path.Format( _T("%s/*.*"),directory_path );
	BOOL bWorking = finder.FindFile( path );

	while(bWorking){
		bWorking = finder.FindNextFile();
		if( finder.IsDirectory() && !finder.IsDots() ){//�����ļ���
			DeleteDirectory(finder.GetFilePath()); //�ݹ�ɾ���ļ���
			RemoveDirectory(finder.GetFilePath());
		}
		else{//�����ļ�
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
	//���ܣ�����ļ�/·���Ƿ����
	return PathFileExists( strPath );
		
}

CString GCDirFile::GetPathFileName( CString strPath )
{
	
	//���ܣ����·���е��ļ���
	//���磺 PathFileFileName( ��c:\\Program Files\\File.txt�� ) = ��File.txt����
	//���·���в������ļ������򷵻�ֵ���������Ŀ¼����

	return PathFindFileName( strPath );

}

CString GCDirFile::GetFileExtName( CString strPath )
{
	
	//���ܣ���ȡ·�����ļ����е��ļ���չ��
	//���磺PathFindExtension( ��File.txt�� ) = ��.txt��
	return PathFindExtension( strPath );
}


CString GCDirFile::GetPathNextComponent( CString strPath )
{
	
	//���磺PathFindNextComponent( ��c:\\Program Files\\Directory�� ) = ��Program Files\\Directory��
	//PathFindNextComponent( ��c:\\Program Files�� ) = ��Program Files��
	return PathFindNextComponent( strPath );
}


CString GCDirFile::RemovePathArgs( CString pszPath )
{
	//���ܣ�ɾ��·���д��еĲ���

	CString tmp = pszPath;
	
	PathRemoveArgs( tmp.GetBuffer() );
	tmp.ReleaseBuffer();
	
	return tmp;
	
}

CString GCDirFile::RemovePathBackslash( CString pszPath )
{
	//���ܣ�ɾ��·����β����ġ�\������
	 PathRemoveBackslash( pszPath.GetBuffer() );
	 pszPath.ReleaseBuffer();

	 return pszPath;
}

void GCDirFile::RemovePathBlanks( LPTSTR pszPath )
{
	//���ܣ�ɾ���ַ���ͷβ�Ŀո�
	PathRemoveBlanks( pszPath );
}

void GCDirFile::RemoveFileExt( LPTSTR pszPath )
{
	//���ܣ�ɾ��·��������ļ���չ��
	PathRemoveExtension( pszPath );

}

CString GCDirFile::GetFilePathStr( LPTSTR pszPath )
{
	//���ܣ�ɾ��·��������ļ����͡�\�����š��ú������Է�����һ���ļ���·����
	//����szPath = ��c:\\windows\\system32\\nt.dll�� ;
	//����PathRemoveFileSpec( szPath ) ��szPath = ��c:\\windows\\system32�� 

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
	//���ܣ���·���з������̷�

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
    //���ܣ��Ӵ����ŵ�·����ȡ��·��
	//����szPath = ��\��c:\\Program Files\���� 
	//PathUnquoteSpaces( szPath ) => szPath = ��c:\\Program Files��
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
			strDestDir+=strName;//���������ݷ�ʽ������
			strDestDir+=".lnk";
			IShellLink* psl;
			if(SUCCEEDED(CoCreateInstance
				(CLSID_ShellLink,NULL,CLSCTX_INPROC_SERVER,IID_IShellLink,(LPVOID*)&psl)))
			{
				psl->SetPath(strSourcePath);//���ÿ�ݷ�ʽ��Ŀ��λ��
				//����Ŀ��λ��ΪC��\windows\a.txt ��ʼλ�þ�Ӧ������ΪC��\windows����ᵼ�²���Ԥ�ϵĴ���
				//������ļ��еĿ�ݷ�ʽ��ʼλ�ú�Ŀ��λ�ÿ�������Ϊһ��
				psl->SetWorkingDirectory(strSourcePath);   //���ÿ�ݷ�ʽ����ʼλ��
				IPersistFile* ppf;

				if(SUCCEEDED(psl->QueryInterface(IID_IPersistFile,(LPVOID*)
					&ppf)))
				{

					
				//	WCHAR wsz[MAX_PATH];
				//	MultiByteToWideChar
				//		(CP_THREAD_ACP,MB_PRECOMPOSED,strDestDir.GetBuffer(),-1,wsz,MAX_PATH);//���������ݷ�ʽ������
					CString wsz = strDestDir;
					if(SUCCEEDED(ppf->Save(wsz,TRUE)))//�����ݷ�ʽ������
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
