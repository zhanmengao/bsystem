// MultiPortScan.h : main header file for the MULTIPORTSCAN application
//

#if !defined(AFX_MULTIPORTSCAN_H__02C47405_E7C9_419B_A5B6_3799D2641ABF__INCLUDED_)
#define AFX_MULTIPORTSCAN_H__02C47405_E7C9_419B_A5B6_3799D2641ABF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CMultiPortScanApp:
// See MultiPortScan.cpp for the implementation of this class
//

class CMultiPortScanApp : public CWinApp
{
public:
	CMultiPortScanApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMultiPortScanApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CMultiPortScanApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MULTIPORTSCAN_H__02C47405_E7C9_419B_A5B6_3799D2641ABF__INCLUDED_)
