
// SetRDPPort.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CSetRDPPortApp: 
// �йش����ʵ�֣������ SetRDPPort.cpp
//

class CSetRDPPortApp : public CWinApp
{
public:
	CSetRDPPortApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CSetRDPPortApp theApp;