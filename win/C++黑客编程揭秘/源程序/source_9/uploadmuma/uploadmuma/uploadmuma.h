
// uploadmuma.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CuploadmumaApp:
// �йش����ʵ�֣������ uploadmuma.cpp
//

class CuploadmumaApp : public CWinApp
{
public:
	CuploadmumaApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CuploadmumaApp theApp;