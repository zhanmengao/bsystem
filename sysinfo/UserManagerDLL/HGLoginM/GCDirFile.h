#pragma once
class GCDirFile
{
public:
	GCDirFile(void);
	~GCDirFile(void);

	BOOL CreateMDirectory( const CString& szPath );
	BOOL CreateDesktopShotCut( CString strName, CString strSourcePath );
	BOOL DirectoryExist( CString strPath );
	BOOL DeleteDirectory( CString directory_path );
	BOOL DirectoryEmpty( CString strPath );

	CString GetPathNextComponent( CString strPath );
	CString GetFileExtName( CString strPath );
	CString GetPathFileName( CString strPath );
	BOOL FileDirExist( CString strPath );
	CString RemovePathArgs( CString pszPath );
	//void RemovePathBackslash( LPTSTR pszPath );
	CString RemovePathBackslash( CString pszPath );
	void RemovePathBlanks( LPTSTR pszPath );
	void RemoveFileExt( LPTSTR pszPath );
	CString GetPathDriveName( LPTSTR pszPath );
	CString GetFilePathStr( LPTSTR pszPath );
	
};

