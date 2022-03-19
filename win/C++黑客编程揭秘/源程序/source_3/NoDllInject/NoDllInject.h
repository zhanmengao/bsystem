// NoDllInject.h : main header file for the NODLLINJECT application
//

#if !defined(AFX_NODLLINJECT_H__D51AAA01_2B5B_473B_85FE_4D462E8DBD77__INCLUDED_)
#define AFX_NODLLINJECT_H__D51AAA01_2B5B_473B_85FE_4D462E8DBD77__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CNoDllInjectApp:
// See NoDllInject.cpp for the implementation of this class
//

class CNoDllInjectApp : public CWinApp
{
public:
	CNoDllInjectApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNoDllInjectApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CNoDllInjectApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NODLLINJECT_H__D51AAA01_2B5B_473B_85FE_4D462E8DBD77__INCLUDED_)
