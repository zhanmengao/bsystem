
// MFCUserServer.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CMFCUserServerApp: 
// �йش����ʵ�֣������ MFCUserServer.cpp
//

class CMFCUserServerApp : public CWinApp
{
public:
	CMFCUserServerApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CMFCUserServerApp theApp;