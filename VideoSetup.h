// VideoSetup.h : main header file for the VIDEOSETUP application
//

#if !defined(__VIDEOSETUP_H__)
#define __VIDEOSETUP_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"		// main symbols

class CVideoSetupDlg;
/////////////////////////////////////////////////////////////////////////////
// CVideoSetupApp:
// See VideoSetup.cpp for the implementation of this class
//

class CVideoSetupApp
{
public:
	CVideoSetupApp();
	~CVideoSetupApp();

public:
	virtual BOOL InitInstance(HINSTANCE hInstance);

	HWND GetMainHWnd(void)		{ return m_MainHWnd; }
	HINSTANCE GetHInstance(void){ return m_hInstance; }
	CVideoSetupDlg *GetDlg(void)	{ return m_pDlg; }

private:
	HWND m_MainHWnd;
	HINSTANCE m_hInstance;
	CVideoSetupDlg *m_pDlg;
};


#ifdef _THE_MASTER_MODULE_
// The one and only Cttf2pcxApp object
CVideoSetupApp theApp;
#else
extern CVideoSetupApp theApp;
#endif
/////////////////////////////////////////////////////////////////////////////

#endif // !defined(__VIDEOSETUP_H__)
