// VideoSetupDlg.cpp : implementation file
//

#include <windows.h>
#include "VideoSetup.h"
#include "VideoSetupDlg.h"


static LRESULT wm_Command(
		HWND hwnd,
		WORD /* wNotifyCode */,
		WORD wID,
		HWND /* hwndCtl */)
{
	switch(wID)
	{
	case IDOK:
		theApp.GetDlg()->OnOk();
		return 0;
	case IDCANCEL:
		DestroyWindow(hwnd);
		return 0;
	case IDC_D3D32:
		theApp.GetDlg()->OnD3d32();
		return 0;
	case IDC_D3D16:
		theApp.GetDlg()->OnD3d16();
		return 0;
	case IDC_OPENGL:
		theApp.GetDlg()->OnOpengl();
		return 0;
	case IDC_16BIT:
		theApp.GetDlg()->On16bit();
		return 0;
	case IDC_32BIT:
		theApp.GetDlg()->On32bit();
		return 0;
	case IDC_LOW:
		theApp.GetDlg()->OnLow();
		return 0;
	case IDC_MED:
		theApp.GetDlg()->OnMed();
		return 0;
	case IDC_HIGH:
		theApp.GetDlg()->OnHigh();
		return 0;
	case IDC_VHIGH:
		theApp.GetDlg()->OnVhigh();
		return 0;
	case IDC_WINDOW:
		theApp.GetDlg()->OnWindow();
		return 0;
	case IDC_ADVANCE:
		theApp.GetDlg()->OnAdvance();
		return 0;
	}

	return 0;
}

static BOOL CALLBACK VideoSetup_DlgProc(
		HWND hwndDlg,	// handle to dialog box
		UINT uMsg,		// message
		WPARAM wParam,	// first message parameter
		LPARAM lParam)	// second message parameter
{
	switch(uMsg)
	{
	case WM_INITDIALOG:
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_COMMAND:
		{
			WORD wNotifyCode = HIWORD(wParam);
			WORD wID = LOWORD(wParam);
			HWND hwndCtl = (HWND)lParam;

			return wm_Command(hwndDlg, wNotifyCode, wID, hwndCtl);
		}
	case WM_QUERYDRAGICON:
		{
			theApp.GetDlg()->OnQueryDragIcon();
			return 0;
		}
	case WM_PAINT:
		{
			theApp.GetDlg()->OnPaint();
			return 0;
		}
	}

	return FALSE;
}
/////////////////////////////////////////////////////////////////////////////
// CVideoSetupDlg dialog

CVideoSetupDlg::CVideoSetupDlg(HWND pParent /*=NULL*/)
{
	m_d3d16 = 0;
	m_d3d32 = 0;
	m_opengl = 0;
	m_sizehigh = 0;
	m_sizelow = 0;
	m_sizemed = 0;
	m_sizevhigh = 0;
	m_sizewin = 0;
	m_16bit = 0;
	m_32bit = 0;
	m_TripleBuffering = FALSE;
	m_TextureCompression = FALSE;
	m_FSAA = FALSE;
	m_ExtraTextures = FALSE;
	m_NoVSync = FALSE;
	m_ASync = TRUE;
	m_DoNotWait = FALSE;

	hLogo = NULL;
	m_bInit = FALSE;

	m_hIcon = LoadIcon(theApp.GetHInstance(), MAKEINTRESOURCE(IDR_MAINFRAME));
}

CVideoSetupDlg::~CVideoSetupDlg()
{
	if(hLogo)
		DeleteObject(hLogo);
}

BOOL CVideoSetupDlg::Create()
{
	m_hWnd = CreateDialog(
		theApp.GetHInstance(),
		MAKEINTRESOURCE(IDD_VIDEOSETUP_DIALOG),
		NULL,
		VideoSetup_DlgProc);

	if(m_hWnd == NULL)
		return FALSE;

	m_bInit = OnInitDialog();

	return m_bInit;
}


/////////////////////////////////////////////////////////////////////////////
// CVideoSetupDlg message handlers

BOOL CVideoSetupDlg::OnInitDialog()
{
	m_ExpandDialog.Initialize(m_hWnd, FALSE, IDC_ADVANCE,
					IDC_SHRINKMARK, IDC_LESSTEXT);

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SendMessage(m_hWnd, WM_SETICON, TRUE, (LPARAM)m_hIcon);		// Set big icon
	SendMessage(m_hWnd, WM_SETICON, FALSE, (LPARAM)m_hIcon);	// Set small icon

	FILE *fLogo = fopen("logo.bmp", "r");
	if(fLogo)
	{
		fclose(fLogo);
		hLogo = LoadImage(theApp.GetHInstance(), "logo.bmp", IMAGE_BITMAP, 96, 96, LR_LOADFROMFILE);
		SendDlgItemMessage(m_hWnd, IDC_LOGO, STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)hLogo);
	}

	// center dialog on desktop
	RECT WndRect, ScreenRect;
	int Width, Height;

	GetWindowRect(m_hWnd, &WndRect);
	Width = WndRect.right - WndRect.left;
	Height = WndRect.bottom - WndRect.top;

	GetWindowRect(GetDesktopWindow(), &ScreenRect);
	SetWindowPos(m_hWnd, 0, (((ScreenRect.right - ScreenRect.left) / 2) - (Width / 2)),
				 (((ScreenRect.bottom - ScreenRect.top) / 2) - (Height / 2)),
				  Width, Height, SWP_NOZORDER | SWP_NOCOPYBITS| SWP_NOSIZE);


	m_advance	= GetDlgItem(m_hWnd, IDC_ADVANCE);
	m_B32bit	= GetDlgItem(m_hWnd, IDC_32BIT);
	m_B16bit	= GetDlgItem(m_hWnd, IDC_16BIT);
	m_Bsizewin	= GetDlgItem(m_hWnd, IDC_WINDOW);
	m_Bsizevhigh= GetDlgItem(m_hWnd, IDC_VHIGH);
	m_Bsizemed	= GetDlgItem(m_hWnd, IDC_MED);
	m_Bsizelow	= GetDlgItem(m_hWnd, IDC_LOW);
	m_Bsizehigh = GetDlgItem(m_hWnd, IDC_HIGH);
	m_Bopengl	= GetDlgItem(m_hWnd, IDC_OPENGL);
	m_Bd3d32	= GetDlgItem(m_hWnd, IDC_D3D32);
	m_Bd3d16	= GetDlgItem(m_hWnd, IDC_D3D16);

	const char* driverName;
	std::string tmp;
	int i, j, index;

	for(i=0;i<3;i++)
	{
		for(j=0;j<2;j++)
		{
			driver[i][j] = false;
		}
	}

	for(i=0;i<3;i++)
	{
		for(j=0;j<5;j++)
		{
			resolution16[i][j] = false;
			resolution32[i][j] = false;
		}
	}

	// save original color depth/zbuffer
	BOOL d3dexist = FALSE;
	COLOR_DEPTH = 16;
	ZBUFFER_DEPTH = 16;
	DRIVERTYPE = 1;
	SCREENSIZE = 0;

	d3dexist = ReadIni();
	UpdateData(FALSE);

	// test drivers at 16/16 level
	WriteIni(16, 16);

	m_pEngine = geEngine_Create(m_hWnd, "", ".");
	m_pDriverSystem = geEngine_GetDriverSystem(m_pEngine);
	m_pDriver = geDriver_SystemGetNextDriver(m_pDriverSystem, NULL);

	while(m_pDriver != NULL)
	{
		// Loop through drivers and set flag is avalible
		geDriver_GetName(m_pDriver, &driverName);
		tmp = driverName;
		index = -1;

		if(tmp.find("(D3D") != -1)
		{
			index = 0;
			driver[0][0] = true;
		}
		else if(tmp.find("D3D 16") != -1)
		{
			index = 1;
			driver[1][0] = true;
		}
		else if(tmp.find("Open") != -1)
		{
			index = 2;
			driver[2][0] = true;
		}

		if(index != -1)
		{
			long w, h;
			m_pMode = geDriver_GetNextMode(m_pDriver, NULL);

			while(m_pMode != NULL)
			{
				geDriver_ModeGetWidthHeight(m_pMode, &w, &h);

				if((w == -1) && (h == -1))
				{
					resolution16[index][4] = true;
				}
				else if(w < 640 || w > 1280)
				{
					;
				}
				else if((w == 640) && (h == 480))
				{
					resolution16[index][0] = true;
				}
				else if((w == 800) && (h == 600))
				{
					resolution16[index][1] = true;
				}
				else if((w == 1024) && (h == 768))
				{
					resolution16[index][2] = true;
				}
				else if((w == 1280) && (h == 1024))
				{
					resolution16[index][3] = true;
				}

				m_pMode = geDriver_GetNextMode(m_pDriver, m_pMode);
			}
		}

		m_pDriver = geDriver_SystemGetNextDriver(m_pDriverSystem, m_pDriver);
	}

	// test drivers at 32/16 level
	WritePrivateProfileInt("D3D24", "BPP", 32, ".\\D3D24.INI");

	m_pDriverSystem = geEngine_GetDriverSystem(m_pEngine);
	m_pDriver = geDriver_SystemGetNextDriver(m_pDriverSystem, NULL);

	while(m_pDriver != NULL)
	{
		// Loop through drivers and set flag if avalible
		geDriver_GetName(m_pDriver, &driverName);
		tmp = driverName;
		index = -1;

		if(tmp.find("(D3D") != -1)
		{
			index = 0;
			driver[0][1] = true;
		}
		else if(tmp.find("Open") != -1)
		{
			index = 2;
			driver[2][1] = true;
		}

		if(index != -1)
		{
			long w, h;
			m_pMode = geDriver_GetNextMode(m_pDriver, NULL);

			while(m_pMode != NULL)
			{
				geDriver_ModeGetWidthHeight(m_pMode, &w, &h);

				if((w == -1) && (h == -1))
				{
					resolution32[index][4] = true;
				}
				else if(w < 640 || w > 1280)
				{
					;
				}
				else if((w == 640) && (h == 480))
				{
					resolution32[index][0] = true;
				}
				else if((w == 800) && (h == 600))
				{
					resolution32[index][1] = true;
				}
				else if((w == 1024) && (h == 768))
				{
					resolution32[index][2] = true;
				}
				else if((w == 1280) && (h == 1024))
				{
					resolution32[index][3] = true;
				}

				m_pMode = geDriver_GetNextMode(m_pDriver, m_pMode);
			}
		}

		m_pDriver = geDriver_SystemGetNextDriver(m_pDriverSystem, m_pDriver);
	}

	geEngine_Free(m_pEngine);

	// restore d3d24.ini
	if(d3dexist)
	{
		WritePrivateProfileInt("D3D24", "BPP", COLOR_DEPTH, ".\\D3D24.INI");
		WritePrivateProfileInt("D3D24", "ZBufferD", ZBUFFER_DEPTH, ".\\D3D24.INI");
	}
	else
	{
		WritePrivateProfileInt("D3D24", "BPP", 16, ".\\D3D24.INI");
		WritePrivateProfileInt("D3D24", "ZBufferD", 16, ".\\D3D24.INI");
	}

	// disable options not avalible
	if(!driver[0][0])
		EnableWindow(m_Bd3d32, FALSE);
	if(!driver[1][0])
		EnableWindow(m_Bd3d16, FALSE);
	if(!driver[2][0])
		EnableWindow(m_Bopengl, FALSE);

	FILE *fdInput = NULL;
	char szInputString[520] = {""};
	char szOutputString[520] = {""};
	char *szAtom = NULL;
	int width = 640;
	int height = 480;
	bool fullscreen = true;

	if((fdInput = fopen("RealityFactory.ini", "rt"))!=NULL)
	{
		while(fgets(szInputString, 512, fdInput) != NULL)
		{
			if(szInputString[0] == ';' || strlen(szInputString) <= 5)
				continue;
			strcpy(szOutputString, szInputString);
			szAtom = strtok(szOutputString, "=");
			if(szAtom == NULL)
				continue;
			if(!stricmp(szAtom, "Width"))
			{
				szAtom = strtok(NULL, " \n");
				width = atoi(szAtom);
				continue;
			}
			else if(!stricmp(szAtom, "Height"))
			{
				szAtom = strtok(NULL, " \n");
				height = atoi(szAtom);
				continue;
			}
			else if(!stricmp(szAtom, "FullScreen"))
			{
				szAtom = strtok(NULL, " \n");
				if(!stricmp(szAtom, "false"))
					fullscreen = false;
				continue;
			}
			else if(!stricmp(szAtom, "Driver"))
			{
				szAtom = strtok(NULL, " \n");
				if(!stricmp(szAtom, "d3d"))
					DRIVERTYPE = 0;
				else if(!stricmp(szAtom, "d3d16"))
					DRIVERTYPE = 1;
				else if(!stricmp(szAtom, "opengl"))
					DRIVERTYPE = 2;
				continue;
			}
		}

		fclose(fdInput);

		if(fullscreen)
		{
			if(width==640 && height==480)
				SCREENSIZE = 0;
			else if(width==800 && height==600)
				SCREENSIZE = 1;
			else if(width==1024 && height==768)
				SCREENSIZE = 2;
			else if(width==1280 && height==1024)
				SCREENSIZE = 3;
		}
		else
		{
			SCREENSIZE = 4;
		}
	}

	if(DRIVERTYPE == 0)
	{
		m_d3d32 = 1;
		SendMessage(m_Bd3d32, BM_SETCHECK, (WPARAM)m_d3d32, 0);
		EnableWindow(m_advance, TRUE);
	}
	else if(DRIVERTYPE == 1)
	{
		m_d3d16 = 1;
		SendMessage(m_Bd3d16, BM_SETCHECK, (WPARAM)m_d3d16, 0);
		EnableWindow(m_advance, FALSE);
	}
	else if(DRIVERTYPE == 2)
	{
		m_opengl = 1;
		SendMessage(m_Bopengl, BM_SETCHECK, (WPARAM)m_opengl, 0);
		EnableWindow(m_advance, TRUE);
	}

	SetColorDepth();
	SetResolution();

	UpdateData();

	ShowWindow(m_hWnd, SW_SHOWNORMAL);

	return TRUE;
}


BOOL CVideoSetupDlg::UpdateData(BOOL bSaveAndValidate /*= TRUE*/)
{
	if(bSaveAndValidate) // retrieve data
	{
		m_d3d16		= SendMessage(m_Bd3d16,		BM_GETCHECK, 0, 0);
		m_d3d32		= SendMessage(m_Bd3d32,		BM_GETCHECK, 0, 0);
		m_opengl	= SendMessage(m_Bopengl,	BM_GETCHECK, 0, 0);
		m_sizehigh	= SendMessage(m_Bsizehigh,	BM_GETCHECK, 0, 0);
		m_sizelow	= SendMessage(m_Bsizelow,	BM_GETCHECK, 0, 0);
		m_sizemed	= SendMessage(m_Bsizemed,	BM_GETCHECK, 0, 0);
		m_sizevhigh	= SendMessage(m_Bsizevhigh,	BM_GETCHECK, 0, 0);
		m_sizewin	= SendMessage(m_Bsizewin,	BM_GETCHECK, 0, 0);
		m_16bit		= SendMessage(m_B16bit,		BM_GETCHECK, 0, 0);
		m_32bit		= SendMessage(m_B32bit,		BM_GETCHECK, 0, 0);

		m_TripleBuffering	= (SendDlgItemMessage(m_hWnd, IDC_TRIPLEBUFFERING,		BM_GETCHECK, 0, 0) == BST_CHECKED) ? TRUE : FALSE;
		m_TextureCompression= (SendDlgItemMessage(m_hWnd, IDC_TEXTURECOMPRESSION,	BM_GETCHECK, 0, 0) == BST_CHECKED) ? TRUE : FALSE;
		m_FSAA				= (SendDlgItemMessage(m_hWnd, IDC_FSAA,					BM_GETCHECK, 0, 0) == BST_CHECKED) ? TRUE : FALSE;
		m_ExtraTextures		= (SendDlgItemMessage(m_hWnd, IDC_EXTRATEXTURES,		BM_GETCHECK, 0, 0) == BST_CHECKED) ? TRUE : FALSE;
		m_NoVSync			= (SendDlgItemMessage(m_hWnd, IDC_NOVSYNC,				BM_GETCHECK, 0, 0) == BST_CHECKED) ? TRUE : FALSE;
		m_ASync				= (SendDlgItemMessage(m_hWnd, IDC_ASYNC,				BM_GETCHECK, 0, 0) == BST_CHECKED) ? TRUE : FALSE;
		m_DoNotWait			= (SendDlgItemMessage(m_hWnd, IDC_DONOTWAIT,			BM_GETCHECK, 0, 0) == BST_CHECKED) ? TRUE : FALSE;

		return TRUE;
	}
	else // initialize data
	{
		SendMessage(m_Bd3d16,		BM_SETCHECK, (WPARAM)m_d3d16,		0);
		SendMessage(m_Bd3d32,		BM_SETCHECK, (WPARAM)m_d3d32,		0);
		SendMessage(m_Bopengl,		BM_SETCHECK, (WPARAM)m_opengl,		0);
		SendMessage(m_Bsizehigh, 	BM_SETCHECK, (WPARAM)m_sizehigh,	0);
		SendMessage(m_Bsizelow, 	BM_SETCHECK, (WPARAM)m_sizelow,		0);
		SendMessage(m_Bsizemed,		BM_SETCHECK, (WPARAM)m_sizemed,		0);
		SendMessage(m_Bsizevhigh,	BM_SETCHECK, (WPARAM)m_sizevhigh,	0);
		SendMessage(m_Bsizewin, 	BM_SETCHECK, (WPARAM)m_sizewin,		0);
		SendMessage(m_B16bit,		BM_SETCHECK, (WPARAM)m_16bit,		0);
		SendMessage(m_B32bit,		BM_SETCHECK, (WPARAM)m_32bit,		0);

		SendDlgItemMessage(m_hWnd, IDC_TRIPLEBUFFERING,		BM_SETCHECK, (WPARAM)(m_TripleBuffering ?	BST_CHECKED : BST_UNCHECKED), 0);
		SendDlgItemMessage(m_hWnd, IDC_TEXTURECOMPRESSION,	BM_SETCHECK, (WPARAM)(m_TextureCompression? BST_CHECKED : BST_UNCHECKED), 0);
		SendDlgItemMessage(m_hWnd, IDC_FSAA,				BM_SETCHECK, (WPARAM)(m_FSAA ?				BST_CHECKED : BST_UNCHECKED), 0);
		SendDlgItemMessage(m_hWnd, IDC_EXTRATEXTURES,		BM_SETCHECK, (WPARAM)(m_ExtraTextures ?		BST_CHECKED : BST_UNCHECKED), 0);
		SendDlgItemMessage(m_hWnd, IDC_NOVSYNC,				BM_SETCHECK, (WPARAM)(m_NoVSync ?			BST_CHECKED : BST_UNCHECKED), 0);
		SendDlgItemMessage(m_hWnd, IDC_ASYNC,				BM_SETCHECK, (WPARAM)(m_ASync ?				BST_CHECKED : BST_UNCHECKED), 0);
		SendDlgItemMessage(m_hWnd, IDC_DONOTWAIT,			BM_SETCHECK, (WPARAM)(m_DoNotWait ?			BST_CHECKED : BST_UNCHECKED), 0);

		return TRUE;
	}
}

void CVideoSetupDlg::SetColorDepth()
{
	switch(COLOR_DEPTH)
	{
	case 16:
		{
			m_16bit = 1;
			SendMessage(m_B16bit, BM_SETCHECK, (WPARAM)m_16bit, 0);
			m_32bit = 0;
			SendMessage(m_B32bit, BM_SETCHECK, (WPARAM)m_32bit, 0);
			if(!driver[DRIVERTYPE][1])
				EnableWindow(m_B32bit, FALSE);
			else
				EnableWindow(m_B32bit, TRUE);

			break;
		}
	case 32:
		{
			if(driver[DRIVERTYPE][1])
			{
				m_32bit = 1;
				SendMessage(m_B32bit, BM_SETCHECK, (WPARAM)m_32bit, 0);
				m_16bit = 0;
				SendMessage(m_B16bit, BM_SETCHECK, (WPARAM)m_16bit, 0);
			}
			else
			{
				COLOR_DEPTH = 16;
				ZBUFFER_DEPTH = 16;
				m_16bit = 1;
				SendMessage(m_B16bit, BM_SETCHECK, (WPARAM)m_16bit, 0);
				m_32bit = 0;
				SendMessage(m_B32bit, BM_SETCHECK, (WPARAM)m_32bit, 0);
				EnableWindow(m_B32bit, FALSE);
			}
		}
	}
}

void CVideoSetupDlg::SetResolution()
{
	switch(COLOR_DEPTH)
	{
	case 16:
		{
			if(resolution16[DRIVERTYPE][0])
			{
				EnableWindow(m_Bsizelow, TRUE);
			}
			else
			{
				EnableWindow(m_Bsizelow, FALSE);
			}

			if(resolution16[DRIVERTYPE][1])
			{
				EnableWindow(m_Bsizemed, TRUE);
			}
			else
			{
				EnableWindow(m_Bsizemed, FALSE);
				if(SCREENSIZE == 1)
					SCREENSIZE = 0;
			}

			if(resolution16[DRIVERTYPE][2])
			{
				EnableWindow(m_Bsizehigh, TRUE);
			}
			else
			{
				EnableWindow(m_Bsizehigh, FALSE);
				if(SCREENSIZE == 2)
					SCREENSIZE = 0;
			}

			if(resolution16[DRIVERTYPE][3])
			{
				EnableWindow(m_Bsizevhigh, TRUE);
			}
			else
			{
				EnableWindow(m_Bsizevhigh, FALSE);
				if(SCREENSIZE == 3)
					SCREENSIZE = 0;
			}

			if(resolution16[DRIVERTYPE][4])
			{
				EnableWindow(m_Bsizewin, TRUE);
			}
			else
			{
				EnableWindow(m_Bsizewin, FALSE);
				if(SCREENSIZE == 4)
					SCREENSIZE = 0;
			}
			break;
		}

	case 32:
		{
			if(resolution32[DRIVERTYPE][0])
			{
				EnableWindow(m_Bsizelow, TRUE);
			}
			else
			{
				EnableWindow(m_Bsizelow, FALSE);
			}

			if(resolution32[DRIVERTYPE][1])
			{
				EnableWindow(m_Bsizemed, TRUE);
			}
			else
			{
				EnableWindow(m_Bsizemed, FALSE);
				if(SCREENSIZE == 1)
					SCREENSIZE = 0;
			}

			if(resolution32[DRIVERTYPE][2])
			{
				EnableWindow(m_Bsizehigh, TRUE);
			}
			else
			{
				EnableWindow(m_Bsizehigh, FALSE);
				if(SCREENSIZE == 2)
					SCREENSIZE = 0;
			}

			if(resolution32[DRIVERTYPE][3])
			{
				EnableWindow(m_Bsizevhigh, TRUE);
			}
			else
			{
				EnableWindow(m_Bsizevhigh, FALSE);
				if(SCREENSIZE == 3)
					SCREENSIZE = 0;
			}

			if(resolution32[DRIVERTYPE][4])
			{
				EnableWindow(m_Bsizewin, TRUE);
			}
			else
			{
				EnableWindow(m_Bsizewin, FALSE);
				if(SCREENSIZE == 4)
					SCREENSIZE = 0;
			}
			break;
		}
	}
	switch(SCREENSIZE)
	{
		case 0:
		{
			m_sizelow = 1;
			SendMessage(m_Bsizelow, BM_SETCHECK, (WPARAM)m_sizelow, 0);
			m_sizemed = 0;
			SendMessage(m_Bsizemed, BM_SETCHECK, (WPARAM)m_sizemed, 0);
			m_sizehigh = 0;
			SendMessage(m_Bsizehigh, BM_SETCHECK, (WPARAM)m_sizehigh, 0);
			m_sizevhigh = 0;
			SendMessage(m_Bsizevhigh, BM_SETCHECK, (WPARAM)m_sizevhigh, 0);
			m_sizewin = 0;
			SendMessage(m_Bsizewin, BM_SETCHECK, (WPARAM)m_sizewin, 0);
			break;
		}
	case 1:
		{
			m_sizelow = 0;
			SendMessage(m_Bsizelow, BM_SETCHECK, (WPARAM)m_sizelow, 0);
			m_sizemed = 1;
			SendMessage(m_Bsizemed, BM_SETCHECK, (WPARAM)m_sizemed, 0);
			m_sizehigh = 0;
			SendMessage(m_Bsizehigh, BM_SETCHECK, (WPARAM)m_sizehigh, 0);
			m_sizevhigh = 0;
			SendMessage(m_Bsizevhigh, BM_SETCHECK, (WPARAM)m_sizevhigh, 0);
			m_sizewin = 0;
			SendMessage(m_Bsizewin, BM_SETCHECK, (WPARAM)m_sizewin, 0);
			break;
		}
	case 2:
		{
			m_sizelow = 0;
			SendMessage(m_Bsizelow, BM_SETCHECK, (WPARAM)m_sizelow, 0);
			m_sizemed = 0;
			SendMessage(m_Bsizemed, BM_SETCHECK, (WPARAM)m_sizemed, 0);
			m_sizehigh = 1;
			SendMessage(m_Bsizehigh, BM_SETCHECK, (WPARAM)m_sizehigh, 0);
			m_sizevhigh = 0;
			SendMessage(m_Bsizevhigh, BM_SETCHECK, (WPARAM)m_sizevhigh, 0);
			m_sizewin = 0;
			SendMessage(m_Bsizewin, BM_SETCHECK, (WPARAM)m_sizewin, 0);
			break;
		}
	case 3:
		{
			m_sizelow = 0;
			SendMessage(m_Bsizelow, BM_SETCHECK, (WPARAM)m_sizelow, 0);
			m_sizemed = 0;
			SendMessage(m_Bsizemed, BM_SETCHECK, (WPARAM)m_sizemed, 0);
			m_sizehigh = 0;
			SendMessage(m_Bsizehigh, BM_SETCHECK, (WPARAM)m_sizehigh, 0);
			m_sizevhigh = 1;
			SendMessage(m_Bsizevhigh, BM_SETCHECK, (WPARAM)m_sizevhigh, 0);
			m_sizewin = 0;
			SendMessage(m_Bsizewin, BM_SETCHECK, (WPARAM)m_sizewin, 0);
			break;
		}
	case 4:
		{
			m_sizelow = 0;
			SendMessage(m_Bsizelow, BM_SETCHECK, (WPARAM)m_sizelow, 0);
			m_sizemed = 0;
			SendMessage(m_Bsizemed, BM_SETCHECK, (WPARAM)m_sizemed, 0);
			m_sizehigh = 0;
			SendMessage(m_Bsizehigh, BM_SETCHECK, (WPARAM)m_sizehigh, 0);
			m_sizevhigh = 0;
			SendMessage(m_Bsizevhigh, BM_SETCHECK, (WPARAM)m_sizevhigh, 0);
			m_sizewin = 1;
			SendMessage(m_Bsizewin, BM_SETCHECK, (WPARAM)m_sizewin, 0);
			break;
		}
	}
}

BOOL CVideoSetupDlg::ReadIni()
{
	FILE *fd = fopen("D3D24.ini","rt");
	if(fd)
	{
		UINT uRetVal;

		COLOR_DEPTH = GetPrivateProfileInt("D3D24", "BPP", 16, ".\\D3D24.INI");
		if(!COLOR_DEPTH) COLOR_DEPTH = 16;

		ZBUFFER_DEPTH = GetPrivateProfileInt("D3D24", "ZBufferD", 16, ".\\D3D24.INI");
		if(!ZBUFFER_DEPTH) ZBUFFER_DEPTH = 16;

		uRetVal = GetPrivateProfileInt("D3D24", "BBufferCount", 2, ".\\D3D24.INI");
		if(uRetVal > 1) m_TripleBuffering = TRUE;

		uRetVal = GetPrivateProfileInt("D3D24", "CompressTextures", 0, ".\\D3D24.INI");
		if(uRetVal) m_TextureCompression = TRUE;

		uRetVal = GetPrivateProfileInt("D3D24", "FSAntiAliasing", 0, ".\\D3D24.INI");
		if(uRetVal) m_FSAA = TRUE;

		uRetVal = GetPrivateProfileInt("D3D24", "ExtraTextures", 0, ".\\D3D24.INI");
		if(uRetVal) m_ExtraTextures = TRUE;

		uRetVal = GetPrivateProfileInt("D3D24", "NoVsync", 0, ".\\D3D24.INI");
		if(uRetVal) m_NoVSync = TRUE;

		uRetVal = GetPrivateProfileInt("D3D24", "Async", 1, ".\\D3D24.INI");
		if(!uRetVal) m_ASync = FALSE;

		uRetVal = GetPrivateProfileInt("D3D24", "DoNotWait", 0, ".\\D3D24.INI");
		if(uRetVal) m_DoNotWait = TRUE;

		fclose(fd);
		return TRUE;
	}

	return FALSE;
}

BOOL CVideoSetupDlg::WritePrivateProfileInt(LPCTSTR lpAppName, LPCTSTR lpKeyName, INT Value, LPCTSTR lpFileName)
{
	TCHAR ValBuf[16];
	wsprintf(ValBuf, TEXT("%i"), Value);
	return (WritePrivateProfileString(lpAppName, lpKeyName, ValBuf, lpFileName));
}

void CVideoSetupDlg::WriteIni(unsigned int ColorDepth, unsigned int ZDepth)
{
	UpdateData();

	WritePrivateProfileInt("D3D24", "BPP", ColorDepth, ".\\D3D24.INI");
	WritePrivateProfileInt("D3D24", "ZBufferD", ZDepth, ".\\D3D24.INI");

	if(m_TripleBuffering)
		WritePrivateProfileInt("D3D24", "BBufferCount", 2, ".\\D3D24.INI");
	else
		WritePrivateProfileInt("D3D24", "BBufferCount", 1, ".\\D3D24.INI");

	if(m_TextureCompression)
		WritePrivateProfileInt("D3D24", "CompressTextures", 1, ".\\D3D24.INI");
	else
		WritePrivateProfileInt("D3D24", "CompressTextures", 0, ".\\D3D24.INI");

	if(m_FSAA)
		WritePrivateProfileInt("D3D24", "FSAntiAliasing", 1, ".\\D3D24.INI");
	else
		WritePrivateProfileInt("D3D24", "FSAntiAliasing", 0, ".\\D3D24.INI");

	if(m_ExtraTextures)
		WritePrivateProfileInt("D3D24", "ExtraTextures", 1, ".\\D3D24.INI");
	else
		WritePrivateProfileInt("D3D24", "ExtraTextures", 0, ".\\D3D24.INI");

	if(m_NoVSync)
		WritePrivateProfileInt("D3D24", "NoVsync", 1, ".\\D3D24.INI");
	else
		WritePrivateProfileInt("D3D24", "NoVsync", 0, ".\\D3D24.INI");

	if(m_ASync)
		WritePrivateProfileInt("D3D24", "Async", 1, ".\\D3D24.INI");
	else
		WritePrivateProfileInt("D3D24", "Async", 0, ".\\D3D24.INI");

	if(m_DoNotWait)
		WritePrivateProfileInt("D3D24", "DoNotWait", 1, ".\\D3D24.INI");
	else
		WritePrivateProfileInt("D3D24", "DoNotWait", 0, ".\\D3D24.INI");
}


// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CVideoSetupDlg::OnPaint()
{
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CVideoSetupDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CVideoSetupDlg::OnD3d32()
{
	DRIVERTYPE = 0;
	m_d3d16 = 0;
	SendMessage(m_Bd3d16, BM_SETCHECK, (WPARAM)m_d3d16, 0);
	m_d3d32 = 1;
	SendMessage(m_Bd3d32, BM_SETCHECK, (WPARAM)m_d3d32, 0);
	m_opengl = 0;
	SendMessage(m_Bopengl, BM_SETCHECK, (WPARAM)m_opengl, 0);
	SetColorDepth();
	SetResolution();
	EnableWindow(m_advance, TRUE);
	UpdateData(TRUE);
}

void CVideoSetupDlg::OnD3d16()
{
	DRIVERTYPE = 1;
	m_d3d16 = 1;
	SendMessage(m_Bd3d16, BM_SETCHECK, (WPARAM)m_d3d16, 0);
	m_d3d32 = 0;
	SendMessage(m_Bd3d32, BM_SETCHECK, (WPARAM)m_d3d32, 0);
	m_opengl = 0;
	SendMessage(m_Bopengl, BM_SETCHECK, (WPARAM)m_opengl, 0);
	SetColorDepth();
	SetResolution();
	if(m_ExpandDialog.m_bIsExpanded)
		m_ExpandDialog.OnExpandButton();
	EnableWindow(m_advance, FALSE);
	UpdateData(TRUE);
}

void CVideoSetupDlg::OnOpengl()
{
	DRIVERTYPE = 2;
	m_d3d16 = 0;
	SendMessage(m_Bd3d16, BM_SETCHECK, (WPARAM)m_d3d16, 0);
	m_d3d32 = 0;
	SendMessage(m_Bd3d32, BM_SETCHECK, (WPARAM)m_d3d32, 0);
	m_opengl = 1;
	SendMessage(m_Bopengl, BM_SETCHECK, (WPARAM)m_opengl, 0);
	SetColorDepth();
	SetResolution();
	EnableWindow(m_advance, TRUE);
	UpdateData(TRUE);
}

void CVideoSetupDlg::On16bit()
{
	COLOR_DEPTH = 16;
	ZBUFFER_DEPTH = 16;
	m_16bit = 1;
	SendMessage(m_B16bit, BM_SETCHECK, (WPARAM)m_16bit, 0);
	m_32bit = 0;
	SendMessage(m_B32bit, BM_SETCHECK, (WPARAM)m_32bit, 0);
	SetResolution();
	UpdateData(TRUE);
}

void CVideoSetupDlg::On32bit()
{
	COLOR_DEPTH=32;
	ZBUFFER_DEPTH = 16;
	m_16bit = 0;
	SendMessage(m_B16bit, BM_SETCHECK, (WPARAM)m_16bit, 0);
	m_32bit = 1;
	SendMessage(m_B32bit, BM_SETCHECK, (WPARAM)m_32bit, 0);
	SetResolution();
	UpdateData(TRUE);
}

void CVideoSetupDlg::OnLow()
{
	SCREENSIZE = 0;
	SetResolution();
	UpdateData(TRUE);
}

void CVideoSetupDlg::OnMed()
{
	SCREENSIZE = 1;
	SetResolution();
	UpdateData(TRUE);
}

void CVideoSetupDlg::OnHigh()
{
	SCREENSIZE = 2;
	SetResolution();
	UpdateData(TRUE);
}

void CVideoSetupDlg::OnVhigh()
{
	SCREENSIZE = 3;
	SetResolution();
	UpdateData(TRUE);
}

void CVideoSetupDlg::OnWindow()
{
	SCREENSIZE = 4;
	SetResolution();
	UpdateData(TRUE);
}

void CVideoSetupDlg::OnAdvance()
{
	m_ExpandDialog.OnExpandButton();
}

void CVideoSetupDlg::OnOk()
{
	UpdateData();

	{
		WriteIni(COLOR_DEPTH, ZBUFFER_DEPTH);

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
				fputs("Renderer=hardware\n", fdOutput);
				continue;
			}
			if(!stricmp(szAtom, "Width"))
			{
				if(SCREENSIZE == 0)
					fputs("Width=640\n", fdOutput);
				else if(SCREENSIZE == 1 || SCREENSIZE == 4)
					fputs("Width=800\n", fdOutput);
				else if(SCREENSIZE == 2)
					fputs("Width=1024\n", fdOutput);
				else if(SCREENSIZE == 3)
					fputs("Width=1280\n", fdOutput);
				continue;
			}
			if(!stricmp(szAtom, "Height"))
			{
				if(SCREENSIZE == 0)
					fputs("Height=480\n", fdOutput);
				else if(SCREENSIZE == 1 || SCREENSIZE == 4)
					fputs("Height=600\n", fdOutput);
				else if(SCREENSIZE == 2)
					fputs("Height=768\n", fdOutput);
				else if(SCREENSIZE == 3)
					fputs("Height=1024\n", fdOutput);
				continue;
			}
			if(!stricmp(szAtom, "FullScreen"))
			{
				if(SCREENSIZE == 4)
					fputs("FullScreen=false\n", fdOutput);
				else
					fputs("FullScreen=true\n", fdOutput);
				continue;
			}
			if(!stricmp(szAtom, "Driver"))
			{
				if(DRIVERTYPE == 0)
					fputs("Driver=d3d\n", fdOutput);
				else if(DRIVERTYPE == 1)
					fputs("Driver=d3d16\n", fdOutput);
				else if(DRIVERTYPE == 2)
					fputs("Driver=opengl\n", fdOutput);
				continue;
			}
			fputs(szInputString, fdOutput);
		}

		fclose(fdInput); fclose(fdOutput);
		remove("RealityFactory.ini");
		rename("RealityFactory.temp", "RealityFactory.ini");
	}

	DestroyWindow(m_hWnd);
}
