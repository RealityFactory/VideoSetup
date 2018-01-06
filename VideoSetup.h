// VideoSetup.h : main header file for the VIDEOSETUP application
//

#if !defined(AFX_VIDEOSETUP_H__66C8C076_5A22_4A73_986A_743C1E9C6713__INCLUDED_)
#define AFX_VIDEOSETUP_H__66C8C076_5A22_4A73_986A_743C1E9C6713__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CVideoSetupApp:
// See VideoSetup.cpp for the implementation of this class
//

class CVideoSetupApp : public CWinApp
{
public:
	CVideoSetupApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVideoSetupApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CVideoSetupApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIDEOSETUP_H__66C8C076_5A22_4A73_986A_743C1E9C6713__INCLUDED_)
