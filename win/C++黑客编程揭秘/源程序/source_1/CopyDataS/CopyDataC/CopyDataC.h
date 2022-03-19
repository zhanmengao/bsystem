// CopyDataC.h : main header file for the COPYDATAC application
//

#if !defined(AFX_COPYDATAC_H__8521E99C_B5E5_487D_87B6_9C106E94D94C__INCLUDED_)
#define AFX_COPYDATAC_H__8521E99C_B5E5_487D_87B6_9C106E94D94C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CCopyDataCApp:
// See CopyDataC.cpp for the implementation of this class
//

class CCopyDataCApp : public CWinApp
{
public:
	CCopyDataCApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCopyDataCApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CCopyDataCApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COPYDATAC_H__8521E99C_B5E5_487D_87B6_9C106E94D94C__INCLUDED_)
