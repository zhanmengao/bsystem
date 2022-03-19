// MsgTest.h : main header file for the MSGTEST application
//

#if !defined(AFX_MSGTEST_H__059B9596_254C_4635_96C8_54FA2178A9C2__INCLUDED_)
#define AFX_MSGTEST_H__059B9596_254C_4635_96C8_54FA2178A9C2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CMsgTestApp:
// See MsgTest.cpp for the implementation of this class
//

class CMsgTestApp : public CWinApp
{
public:
	CMsgTestApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMsgTestApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CMsgTestApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MSGTEST_H__059B9596_254C_4635_96C8_54FA2178A9C2__INCLUDED_)
