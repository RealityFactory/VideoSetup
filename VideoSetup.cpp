// VideoSetup.cpp : Defines the class behaviors for the application.
//

#define _THE_MASTER_MODULE_

#include <windows.h>
#include <commctrl.h>
#include "VideoSetup.h"
#include "VideoSetupDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CVideoSetupApp construction

CVideoSetupApp::CVideoSetupApp()
{
	m_MainHWnd = NULL;
	m_pDlg = NULL;
}

CVideoSetupApp::~CVideoSetupApp()
{
	if(m_pDlg)
		delete m_pDlg;
}

// CVideoSetupApp initialization

BOOL CVideoSetupApp::InitInstance(HINSTANCE hInstance)
{
	m_hInstance = hInstance;
	m_pDlg = new CVideoSetupDlg();
	if(!m_pDlg->Create())
		return FALSE;

	m_MainHWnd = m_pDlg->GetHWnd();

	return TRUE;
}


int WINAPI WinMain(
		HINSTANCE hInstance,		// handle to current instance
		HINSTANCE /*hPrevInstance*/,// handle to previous instance
		LPSTR /*lpCmdLine*/,		// pointer to command line
		int /*nCmdShow*/)			// show state of window
{
	// check for previous instances
	CreateMutex(NULL, TRUE, "VideoSetup");

	if(GetLastError() == ERROR_ALREADY_EXISTS)
	{
		return 0;
	}

	InitCommonControls ();

	if(!theApp.InitInstance(hInstance))
		return 0;

	MSG msg;

	// enter the Main message loop:
	while(GetMessage(&msg, NULL, 0, 0))
	{
		if(!IsDialogMessage(theApp.GetMainHWnd(), &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return 0;
}
