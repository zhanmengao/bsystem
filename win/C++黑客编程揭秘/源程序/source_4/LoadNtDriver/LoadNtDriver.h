// LoadNtDriver.h : main header file for the LOADNTDRIVER application
//

#if !defined(AFX_LOADNTDRIVER_H__3465217C_4D37_4DB9_B14D_EBD4ED1E482F__INCLUDED_)
#define AFX_LOADNTDRIVER_H__3465217C_4D37_4DB9_B14D_EBD4ED1E482F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CLoadNtDriverApp:
// See LoadNtDriver.cpp for the implementation of this class
//

class CLoadNtDriverApp : public CWinApp
{
public:
	CLoadNtDriverApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLoadNtDriverApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CLoadNtDriverApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOADNTDRIVER_H__3465217C_4D37_4DB9_B14D_EBD4ED1E482F__INCLUDED_)
