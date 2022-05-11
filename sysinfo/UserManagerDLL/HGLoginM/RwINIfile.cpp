#include "stdafx.h"
#include "RwINIfile.h"

#define MAX_ALLSection   65535

CRwINIfile::CRwINIfile( CString INIfileName  )
{

	pINIfile = INIfileName;
}


CRwINIfile::~CRwINIfile(void)
{
}


CString CRwINIfile::ReadString( CString Section ,CString KeyName ,CString DefaultV )
{
	WCHAR GetV[ MAX_PATH ];
	::GetPrivateProfileString( Section ,KeyName ,DefaultV, GetV,MAX_PATH,pINIfile );

	return (CString)GetV;
}

int CRwINIfile::ReadInt( CString Section ,CString KeyName ,int DefaultV )
{

	return ::GetPrivateProfileInt(  Section ,KeyName ,DefaultV, pINIfile);
	 
}

BOOL CRwINIfile::WriteString( CString Section ,CString KeyName ,CString newV )
{

	return  ::WritePrivateProfileString( Section ,KeyName ,newV , pINIfile );
}


BOOL CRwINIfile::WriteInt( CString Section ,CString KeyName ,int newV )
{
	CString tmpToStr;
	tmpToStr.Format(_T("%d"), newV );

	return  ::WritePrivateProfileString( Section ,KeyName ,tmpToStr , pINIfile );
}


BOOL CRwINIfile::DeleteSection( CString Section )
{
	return ::WritePrivateProfileString( Section, NULL,NULL, pINIfile );
}

BOOL CRwINIfile::DeleteApp( CString Section ,CString AppName )
{
	return ::WritePrivateProfileString( Section, AppName ,NULL, pINIfile );
}


int  CRwINIfile::GetAllSectionName( CStringList* SectionList )
{
	WCHAR Section_s[MAX_ALLSection];
	ZeroMemory(Section_s,MAX_ALLSection);
	int count =  ::GetPrivateProfileSectionNames( Section_s, MAX_ALLSection, pINIfile ); 

	WCHAR Section_z[MAX_PATH];
	ZeroMemory(Section_z,MAX_PATH  );
	int j = 0;
    for( int i =0 ;i < count ;i++   )
	{

	    if( Section_s[i] != '\0'  )
		{
			Section_z[j] = Section_s[i]  ;
		    j++;
		}
		else
		{

			SectionList->AddTail( (CString)Section_z );
			j = 0;
			ZeroMemory(Section_z,MAX_PATH  );
		}	
	
	}

	return SectionList->GetCount();
}


int CRwINIfile::GetKeyValues( CStringList* AppNameList, CStringList* KeyNameList, CString Section )
{

	int i;  
	int iPos=0;
	CString strKeyValue;
	int iMaxCount;
	WCHAR AppNameS[MAX_ALLSection]={0}; //总的提出来的字符串
	WCHAR KeyNameS[MAX_PATH]={0}; //提出来的一个键名

	iMaxCount = ::GetPrivateProfileSection( Section ,AppNameS,MAX_ALLSection, pINIfile );
	/*
	for(i=0;i<MAX_ALLSection;i++)
	{
	  if ( AppNameS[i]==0 )
	     if ( AppNameS[i] == AppNameS[i+1] )
		      break;
	}

	iMaxCount=i+1; //要多一个0号元素。即找出全部字符串的结束部分。
	*/

	AppNameList->RemoveAll();//清空原数组
	KeyNameList->RemoveAll();

	for(i=0;i<iMaxCount;i++)
	{

	  KeyNameS[ iPos++ ] = AppNameS[i];

	  if( AppNameS[i]==0 )
	  {
		  strKeyValue=KeyNameS;
		  AppNameList->AddTail( strKeyValue.Left( strKeyValue.Find(_T("="))) );
		  KeyNameList->AddTail( strKeyValue.Mid( strKeyValue.Find(_T("="))+1) );
		  ZeroMemory(KeyNameS,MAX_PATH);
		  iPos=0;
	  }

	}

	return (int)AppNameList->GetCount();
}


CString CRwINIfile::GetFileName()
{
	//
	return pINIfile;
}

void CRwINIfile::SetFileName( CString iniFileName )
{
	//
	pINIfile = iniFileName;
}

BOOL CRwINIfile::DeleteAllSection()
{
	CStringList* del_T = new CStringList;

	int count = GetAllSectionName(del_T);

	if (  count != 0  )
	{
		POSITION P;
		P = del_T->GetHeadPosition();

		while( P!= NULL  )
		{
			DeleteSection( del_T->GetNext(P) );
		}
	}

	
	delete del_T;

	return TRUE;
}