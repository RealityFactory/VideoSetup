// VideoSetup.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "VideoSetup.h"
#include "VideoSetupDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVideoSetupApp

BEGIN_MESSAGE_MAP(CVideoSetupApp, CWinApp)
	//{{AFX_MSG_MAP(CVideoSetupApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVideoSetupApp construction

CVideoSetupApp::CVideoSetupApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CVideoSetupApp object

CVideoSetupApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CVideoSetupApp initialization

BOOL CVideoSetupApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	CVideoSetupDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		char szText[64];
		FILE *fd = fopen("D3D24.ini", "wb");
		sprintf(szText, "%d", dlg.COLOR_DEPTH);
		fputs(szText, fd); fputs("\n", fd);
		sprintf(szText, "%d", dlg.ZBUFFER_DEPTH);
		fputs(szText, fd); fputs("\n", fd);
		fclose(fd);

		char szInputString[1024] = {""};
		char szOutputString[1024] = {""};
		char *szAtom = NULL;
		FILE *fdInput = NULL, *fdOutput = NULL;

		fdInput = fopen("RealityFactory.ini", "rt");
		fdOutput = fopen("RealityFactory.temp", "wt");

		while(fgets(szInputString, 512, fdInput) != NULL)
		{
			if(szInputString[0] == ';' || strlen(szInputString) <= 5)
			{
				fputs(szInputString, fdOutput);
				continue;
			}
			strcpy(szOutputString, szInputString);
			szAtom = strtok(szOutputString, "=");
			if(szAtom == NULL)
			{
				fputs(szInputString, fdOutput);
				continue;
			}
			if(!stricmp(szAtom, "Renderer"))
			{
				fputs("Renderer=hardware\n\n", fdOutput);
				continue;
			}
			if(!stricmp(szAtom, "Width"))
			{
				if(dlg.SCREENSIZE==0)
					fputs("Width=640\n\n", fdOutput);
				else if(dlg.SCREENSIZE==1 || dlg.SCREENSIZE==4)
					fputs("Width=800\n\n", fdOutput);
				else if(dlg.SCREENSIZE==2)
					fputs("Width=1024\n\n", fdOutput);
				else if(dlg.SCREENSIZE==3)
					fputs("Width=1280\n\n", fdOutput);
				continue;
			}
			if(!stricmp(szAtom, "Height"))
			{
				if(dlg.SCREENSIZE==0)
					fputs("Height=480\n\n", fdOutput);
				else if(dlg.SCREENSIZE==1 || dlg.SCREENSIZE==4)
					fputs("Height=600\n\n", fdOutput);
				else if(dlg.SCREENSIZE==2)
					fputs("Height=768\n\n", fdOutput);
				else if(dlg.SCREENSIZE==3)
					fputs("Height=1024\n\n", fdOutput);
				continue;
			}
			if(!stricmp(szAtom, "FullScreen"))
			{
				if(dlg.SCREENSIZE==4)
					fputs("FullScreen=false\n\n", fdOutput);
				else
					fputs("FullScreen=true\n\n", fdOutput);
				continue;
			}
			if(!stricmp(szAtom, "Driver"))
			{
				if(dlg.DRIVERTYPE==0)
					fputs("Driver=d3d\n\n", fdOutput);
				if(dlg.DRIVERTYPE==1)
					fputs("Driver=d3d16\n\n", fdOutput);
				if(dlg.DRIVERTYPE==2)
					fputs("Driver=opengl\n\n", fdOutput);
				continue;
			}
			fputs(szInputString, fdOutput);
		}

		fclose(fdInput); fclose(fdOutput);
		remove("RealityFactory.ini");
		rename("RealityFactory.temp", "RealityFactory.ini");
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
