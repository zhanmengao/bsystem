// ManageServices.h : main header file for the MANAGESERVICES application
//

#if !defined(AFX_MANAGESERVICES_H__45086DEB_7F14_4954_AB1F_5776ECF3EE78__INCLUDED_)
#define AFX_MANAGESERVICES_H__45086DEB_7F14_4954_AB1F_5776ECF3EE78__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CManageServicesApp:
// See ManageServices.cpp for the implementation of this class
//

class CManageServicesApp : public CWinApp
{
public:
	CManageServicesApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CManageServicesApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CManageServicesApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MANAGESERVICES_H__45086DEB_7F14_4954_AB1F_5776ECF3EE78__INCLUDED_)
