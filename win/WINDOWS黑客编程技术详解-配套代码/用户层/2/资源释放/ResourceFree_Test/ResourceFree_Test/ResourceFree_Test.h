
// ResourceFree_Test.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CResourceFree_TestApp: 
// �йش����ʵ�֣������ ResourceFree_Test.cpp
//

class CResourceFree_TestApp : public CWinApp
{
public:
	CResourceFree_TestApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CResourceFree_TestApp theApp;