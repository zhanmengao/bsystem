
// MFCUserClient.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CMFCUserClientApp: 
// �йش����ʵ�֣������ MFCUserClient.cpp
//

class CMFCUserClientApp : public CWinApp
{
public:
	CMFCUserClientApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CMFCUserClientApp theApp;