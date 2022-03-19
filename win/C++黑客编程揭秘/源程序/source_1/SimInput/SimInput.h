// SimInput.h : main header file for the SIMINPUT application
//

#if !defined(AFX_SIMINPUT_H__97C06611_BC4A_48BC_9BCB_27E2BF87E5BE__INCLUDED_)
#define AFX_SIMINPUT_H__97C06611_BC4A_48BC_9BCB_27E2BF87E5BE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSimInputApp:
// See SimInput.cpp for the implementation of this class
//

class CSimInputApp : public CWinApp
{
public:
	CSimInputApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSimInputApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CSimInputApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SIMINPUT_H__97C06611_BC4A_48BC_9BCB_27E2BF87E5BE__INCLUDED_)
