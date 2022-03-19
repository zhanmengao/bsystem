// ImmunityU.h : main header file for the IMMUNITYU application
//

#if !defined(AFX_IMMUNITYU_H__4228DA2D_EFD6_46D3_ACD5_DBB682B171F6__INCLUDED_)
#define AFX_IMMUNITYU_H__4228DA2D_EFD6_46D3_ACD5_DBB682B171F6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CImmunityUApp:
// See ImmunityU.cpp for the implementation of this class
//

class CImmunityUApp : public CWinApp
{
public:
	CImmunityUApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImmunityUApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CImmunityUApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMMUNITYU_H__4228DA2D_EFD6_46D3_ACD5_DBB682B171F6__INCLUDED_)
