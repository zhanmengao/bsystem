
// TestRemoteConnection.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CTestRemoteConnectionApp: 
// �йش����ʵ�֣������ TestRemoteConnection.cpp
//

class CTestRemoteConnectionApp : public CWinApp
{
public:
	CTestRemoteConnectionApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CTestRemoteConnectionApp theApp;