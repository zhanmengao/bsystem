// UserWM.h : main header file for the USERWM application
//

#if !defined(AFX_USERWM_H__00CC38A5_8693_47D9_8ED1_A645E7051C14__INCLUDED_)
#define AFX_USERWM_H__00CC38A5_8693_47D9_8ED1_A645E7051C14__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CUserWMApp:
// See UserWM.cpp for the implementation of this class
//

class CUserWMApp : public CWinApp
{
public:
	CUserWMApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUserWMApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CUserWMApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_USERWM_H__00CC38A5_8693_47D9_8ED1_A645E7051C14__INCLUDED_)
