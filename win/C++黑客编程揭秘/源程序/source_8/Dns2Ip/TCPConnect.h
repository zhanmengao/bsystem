// TCPConnect.h : main header file for the TCPCONNECT application
//

#if !defined(AFX_TCPCONNECT_H__E2FDFA51_B1FC_4900_B6FC_EC2C73D19791__INCLUDED_)
#define AFX_TCPCONNECT_H__E2FDFA51_B1FC_4900_B6FC_EC2C73D19791__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CTCPConnectApp:
// See TCPConnect.cpp for the implementation of this class
//

class CTCPConnectApp : public CWinApp
{
public:
	CTCPConnectApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTCPConnectApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CTCPConnectApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TCPCONNECT_H__E2FDFA51_B1FC_4900_B6FC_EC2C73D19791__INCLUDED_)
