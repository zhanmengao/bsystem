// KeyBoard.h : main header file for the KEYBOARD application
//

#if !defined(AFX_KEYBOARD_H__1F3BC1A0_F35E_40EA_B28A_9551CDA824E5__INCLUDED_)
#define AFX_KEYBOARD_H__1F3BC1A0_F35E_40EA_B28A_9551CDA824E5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CKeyBoardApp:
// See KeyBoard.cpp for the implementation of this class
//

class CKeyBoardApp : public CWinApp
{
public:
	CKeyBoardApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKeyBoardApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CKeyBoardApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KEYBOARD_H__1F3BC1A0_F35E_40EA_B28A_9551CDA824E5__INCLUDED_)
