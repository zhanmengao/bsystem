
// SetWindowsInfo.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CSetWindowsInfoApp: 
// �йش����ʵ�֣������ SetWindowsInfo.cpp
//

class CSetWindowsInfoApp : public CWinApp
{
public:
	CSetWindowsInfoApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CSetWindowsInfoApp theApp;