
// DelayLoadDll_Test.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CDelayLoadDll_TestApp: 
// �йش����ʵ�֣������ DelayLoadDll_Test.cpp
//

class CDelayLoadDll_TestApp : public CWinApp
{
public:
	CDelayLoadDll_TestApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CDelayLoadDll_TestApp theApp;