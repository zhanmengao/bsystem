
// WM_DEVICECHANGE_MFC_Test.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CWM_DEVICECHANGE_MFC_TestApp: 
// �йش����ʵ�֣������ WM_DEVICECHANGE_MFC_Test.cpp
//

class CWM_DEVICECHANGE_MFC_TestApp : public CWinApp
{


public:
	CWM_DEVICECHANGE_MFC_TestApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CWM_DEVICECHANGE_MFC_TestApp theApp;