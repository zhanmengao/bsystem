
// SQLInjectTools.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CSQLInjectToolsApp:
// �йش����ʵ�֣������ SQLInjectTools.cpp
//

class CSQLInjectToolsApp : public CWinApp
{
public:
	CSQLInjectToolsApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CSQLInjectToolsApp theApp;