#pragma once

#include <stdio.h>
#include <vector>
#include <atlstr.h>
#include <vector>
#include "CommonFiles.h"
#include "GStringClass.h"

class GCDirFile
{
private:
	GStringClass cstringClass;

public:
	GCDirFile(void);
	~GCDirFile(void);

public:
	BOOL CreateMDirectory(string szPath );	
	
	BOOL DirectoryExist( CString strPath );
	BOOL DeleteDirectory( CString directory_path );
	BOOL DirectoryEmpty( CString strPath );

	CString GetPathNextComponent( CString strPath );
	CString GetFileExtName( CString strPath );
	CString GetPathFileName( CString strPath );
	
	BOOL FileDirExist( CString strPath );
	
	CString RemovePathArgs( CString pszPath );	
	CString RemovePathBackslash( CString pszPath );
	void RemovePathBlanks( LPTSTR pszPath );
	void RemoveFileExt( LPTSTR pszPath );

	CString GetPathDriveName( LPTSTR pszPath );
	CString GetFilePathStr( LPTSTR pszPath );
	int moveFile(const char *oldPath, const char *newPath);

	int ReadDirPathList(string DirPath, vector<Filepath> &filespath);
	int SendFile(SOCKET sock, const char *path);
	int RecvFile(const char *url, string diskpath);
	int copyFile(const char *oldPath, const char *newPath);
	int deleteFile(const char *filePath, const char *filePath1);
	int CreateFolder(const char *folderPath);
	int FolderAddOrDelGroup( char *lpszFileName, char *lpszAccountName, DWORD dwAccessMask, UINT status);
	int renameFile(const char *oldName, const char *newName);
	int SetUserFolderSize(string path, string size);
	int ModifyUserFolderSize(string path, string size);
	int GCDirFile::DelUserFolderSize(string path);

private:
	int GetFiles(string path, vector<Filepath> &filespath);
	
};

