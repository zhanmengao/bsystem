#pragma once
class CRwINIfile
{

private:
	CString pINIfile;

public:
	CRwINIfile( CString INIfileName  );
	~CRwINIfile(void);

public:
	CString ReadString( CString Section ,CString KeyName ,CString DefaultV );
	int ReadInt( CString Section ,CString KeyName ,int DefaultV );

	BOOL WriteString( CString Section ,CString KeyName ,CString newV );
	BOOL WriteInt( CString Section ,CString KeyName ,int newV );

	BOOL DeleteSection( CString Section );
	BOOL DeleteApp( CString Section ,CString AppName );

	int GetAllSectionName( CStringList* SectionList );

	int GetKeyValues( CStringList* AppNameList, CStringList* KeyNameList, CString Section );

	void SetFileName( CString iniFileName );
	CString GetFileName();
	BOOL DeleteAllSection();
};

