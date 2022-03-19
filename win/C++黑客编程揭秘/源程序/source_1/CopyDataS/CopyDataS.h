// CopyDataS.h : main header file for the COPYDATAS application
//

#if !defined(AFX_COPYDATAS_H__CA4D31F8_CE34_4A25_B1D6_F020BFB63149__INCLUDED_)
#define AFX_COPYDATAS_H__CA4D31F8_CE34_4A25_B1D6_F020BFB63149__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CCopyDataSApp:
// See CopyDataS.cpp for the implementation of this class
//

class CCopyDataSApp : public CWinApp
{
public:
	CCopyDataSApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCopyDataSApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CCopyDataSApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COPYDATAS_H__CA4D31F8_CE34_4A25_B1D6_F020BFB63149__INCLUDED_)
