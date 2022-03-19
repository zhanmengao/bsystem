// UserWMC.h : main header file for the USERWMC application
//

#if !defined(AFX_USERWMC_H__BD8EB822_2F85_4C42_B17F_B2DFA07E05A3__INCLUDED_)
#define AFX_USERWMC_H__BD8EB822_2F85_4C42_B17F_B2DFA07E05A3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CUserWMCApp:
// See UserWMC.cpp for the implementation of this class
//

class CUserWMCApp : public CWinApp
{
public:
	CUserWMCApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUserWMCApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CUserWMCApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_USERWMC_H__BD8EB822_2F85_4C42_B17F_B2DFA07E05A3__INCLUDED_)
