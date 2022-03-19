// EasyCrackMe.h : main header file for the EASYCRACKME application
//

#if !defined(AFX_EASYCRACKME_H__80289F08_3511_40A7_A3F7_9F4D085FF75F__INCLUDED_)
#define AFX_EASYCRACKME_H__80289F08_3511_40A7_A3F7_9F4D085FF75F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CEasyCrackMeApp:
// See EasyCrackMe.cpp for the implementation of this class
//

class CEasyCrackMeApp : public CWinApp
{
public:
	CEasyCrackMeApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEasyCrackMeApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CEasyCrackMeApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EASYCRACKME_H__80289F08_3511_40A7_A3F7_9F4D085FF75F__INCLUDED_)
