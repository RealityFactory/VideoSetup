// VideoSetupDlg.cpp : implementation file
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
// CVideoSetupDlg dialog

CVideoSetupDlg::CVideoSetupDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CVideoSetupDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CVideoSetupDlg)
	m_d3d16 = -1;
	m_d3d32 = -1;
	m_opengl = -1;
	m_sizehigh = -1;
	m_sizelow = -1;
	m_sizemed = -1;
	m_sizevhigh = -1;
	m_sizewin = -1;
	m_16bit = -1;
	m_32bit = -1;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CVideoSetupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CVideoSetupDlg)
	DDX_Control(pDX, IDC_ADVANCE, m_advance);
	DDX_Control(pDX, IDC_32BIT, m_B32bit);
	DDX_Control(pDX, IDC_16BIT, m_B16bit);
	DDX_Control(pDX, IDC_WINDOW, m_Bsizewin);
	DDX_Control(pDX, IDC_VHIGH, m_Bsizevhigh);
	DDX_Control(pDX, IDC_MED, m_Bsizemed);
	DDX_Control(pDX, IDC_LOW, m_Bsizelow);
	DDX_Control(pDX, IDC_HIGH, m_Bsizehigh);
	DDX_Control(pDX, IDC_OPENGL, m_Bopengl);
	DDX_Control(pDX, IDC_D3D32, m_Bd3d32);
	DDX_Control(pDX, IDC_D3D16, m_Bd3d16);
	DDX_Radio(pDX, IDC_D3D16, m_d3d16);
	DDX_Radio(pDX, IDC_D3D32, m_d3d32);
	DDX_Radio(pDX, IDC_OPENGL, m_opengl);
	DDX_Radio(pDX, IDC_HIGH, m_sizehigh);
	DDX_Radio(pDX, IDC_LOW, m_sizelow);
	DDX_Radio(pDX, IDC_MED, m_sizemed);
	DDX_Radio(pDX, IDC_VHIGH, m_sizevhigh);
	DDX_Radio(pDX, IDC_WINDOW, m_sizewin);
	DDX_Radio(pDX, IDC_16BIT, m_16bit);
	DDX_Radio(pDX, IDC_32BIT, m_32bit);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CVideoSetupDlg, CDialog)
	//{{AFX_MSG_MAP(CVideoSetupDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_D3D32, OnD3d32)
	ON_BN_CLICKED(IDC_D3D16, OnD3d16)
	ON_BN_CLICKED(IDC_OPENGL, OnOpengl)
	ON_BN_CLICKED(IDC_16BIT, On16bit)
	ON_BN_CLICKED(IDC_32BIT, On32bit)
	ON_BN_CLICKED(IDC_LOW, OnLow)
	ON_BN_CLICKED(IDC_MED, OnMed)
	ON_BN_CLICKED(IDC_HIGH, OnHigh)
	ON_BN_CLICKED(IDC_VHIGH, OnVhigh)
	ON_BN_CLICKED(IDC_WINDOW, OnWindow)
	ON_BN_CLICKED(IDC_ADVANCE, OnAdvance)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVideoSetupDlg message handlers

BOOL CVideoSetupDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_ExpandDialog.Initialize(this, FALSE, IDC_ADVANCE,
					IDC_SHRINKMARK, IDC_LESSTEXT) ;

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	FILE *fd;
	const char* driverName;
	CString tmp;
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
	bool d3dexist = false;
	COLOR_DEPTH = 16;
	ZBUFFER_DEPTH = 16;
	DRIVERTYPE = 1;
	SCREENSIZE = 0;

	fd = fopen("D3D24.ini","r");
	if(fd)
	{
		fscanf(fd,"%d",&COLOR_DEPTH);
		fscanf(fd,"%d",&ZBUFFER_DEPTH);
		fclose(fd);
		d3dexist = true;
	}

	// test drivers at 16/16 level
	fd = fopen("D3D24.ini", "wb");
	fputs("16", fd); fputs("\n", fd);
	fputs("16", fd); fputs("\n", fd);
	fclose(fd);

	m_pEngine = geEngine_Create(GetSafeHwnd(), "", ".");
	m_pDriverSystem = geEngine_GetDriverSystem(m_pEngine);
	m_pDriver = geDriver_SystemGetNextDriver(m_pDriverSystem, NULL);
	while (m_pDriver != NULL)
	{
		// Loop through drivers and set flag is avalible
		geDriver_GetName(m_pDriver, &driverName);
		tmp = driverName;
		index = -1;
		if(tmp.Find("(D3D")!=-1)
		{
			index = 0;
			driver[0][0] = true;
		}
		else if(tmp.Find("D3D 16")!=-1)
		{
			index = 1;
			driver[1][0] = true;
		}
		else if(tmp.Find("Open")!=-1)
		{
			index = 2;
			driver[2][0] = true;
		}
		if(index!=-1)
		{
			long w, h;
			m_pMode = geDriver_GetNextMode(m_pDriver, NULL);
			while (m_pMode != NULL)
			{
				geDriver_ModeGetWidthHeight(m_pMode, &w, &h);
				if ((w == -1) && (h == -1))
				{
					resolution16[index][4] = true;
				}
				else if ((w == 640) && (h == 480))
				{
					resolution16[index][0] = true;
				}
				else if ((w == 800) && (h == 600))
				{
					resolution16[index][1] = true;
				}
				else if ((w == 1024) && (h == 768))
				{
					resolution16[index][2] = true;
				}
				else if ((w == 1280) && (h == 1024))
				{
					resolution16[index][3] = true;
				}
				m_pMode = geDriver_GetNextMode(m_pDriver, m_pMode);
			}
		}
		m_pDriver = geDriver_SystemGetNextDriver(m_pDriverSystem, m_pDriver);
	}

	// test drivers at 32/16 level
	fd = fopen("D3D24.ini", "wb");
	fputs("32", fd); fputs("\n", fd);
	fputs("16", fd); fputs("\n", fd);
	fclose(fd);

	m_pDriverSystem = geEngine_GetDriverSystem(m_pEngine);
	m_pDriver = geDriver_SystemGetNextDriver(m_pDriverSystem, NULL);
	while (m_pDriver != NULL)
	{
		// Loop through drivers and set flag if avalible
		geDriver_GetName(m_pDriver, &driverName);
		tmp = driverName;
		index = -1;
		if(tmp.Find("(D3D")!=-1)
		{
			index = 0;
			driver[0][1] = true;
		}
		else if(tmp.Find("Open")!=-1)
		{
			index = 2;
			driver[2][1] = true;
		}
		if(index!=-1)
		{
			long w, h;
			m_pMode = geDriver_GetNextMode(m_pDriver, NULL);
			while (m_pMode != NULL)
			{
				geDriver_ModeGetWidthHeight(m_pMode, &w, &h);
				if ((w == -1) && (h == -1))
				{
					resolution32[index][4] = true;
				}
				else if ((w == 640) && (h == 480))
				{
					resolution32[index][0] = true;
				}
				else if ((w == 800) && (h == 600))
				{
					resolution32[index][1] = true;
				}
				else if ((w == 1024) && (h == 768))
				{
					resolution32[index][2] = true;
				}
				else if ((w == 1280) && (h == 1024))
				{
					resolution32[index][3] = true;
				}
				m_pMode = geDriver_GetNextMode(m_pDriver, m_pMode);
			}
		}
		m_pDriver = geDriver_SystemGetNextDriver(m_pDriverSystem, m_pDriver);
	}

	geEngine_Free(m_pEngine);
	geVFile_CloseAPI();

	// restore d3d24.ini
	if(d3dexist)
	{
		char szText[64];
		fd = fopen("D3D24.ini", "wb");
		sprintf(szText, "%d", COLOR_DEPTH);
		fputs(szText, fd); fputs("\n", fd);
		sprintf(szText, "%d", ZBUFFER_DEPTH);
		fputs(szText, fd); fputs("\n", fd);
		fclose(fd);
	}
	else
	{
		fd = fopen("D3D24.ini", "wb");
		fputs("16", fd); fputs("\n", fd);
		fputs("16", fd); fputs("\n", fd);
		fclose(fd);
	}

	// disable options not avalible
	if(!driver[0][0])
		m_Bd3d32.EnableWindow(FALSE);
	if(!driver[1][0])
		m_Bd3d16.EnableWindow(FALSE);
	if(!driver[2][0])
		m_Bopengl.EnableWindow(FALSE);

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
			if(!stricmp(szAtom, "Height"))
			{
				szAtom = strtok(NULL, " \n");
				height = atoi(szAtom);
				continue;
			}
			if(!stricmp(szAtom, "FullScreen"))
			{
				szAtom = strtok(NULL, " \n");
				if(!stricmp(szAtom, "false"))
					fullscreen = false;
			}
			if(!stricmp(szAtom, "Driver"))
			{
				szAtom = strtok(NULL, " \n");
				if(!stricmp(szAtom, "d3d"))
					DRIVERTYPE = 0;
				if(!stricmp(szAtom, "d3d16"))
					DRIVERTYPE = 1;
				if(!stricmp(szAtom, "opengl"))
					DRIVERTYPE = 2;
			}
		}
		fclose(fdInput);
		if(fullscreen)
		{
			if(width==640 && height==480)
				SCREENSIZE = 0;
			if(width==800 && height==600)
				SCREENSIZE = 1;
			if(width==1024 && height==768)
				SCREENSIZE = 2;
			if(width==1280 && height==1024)
				SCREENSIZE = 3;
		}
		else
			SCREENSIZE = 4;
	}

	if(DRIVERTYPE==0)
	{
		m_d3d32 = 1;
		m_Bd3d32.SetCheck(1);
		m_advance.EnableWindow(TRUE);
	}
	else if(DRIVERTYPE==1)
	{
		m_d3d16 = 1;
		m_Bd3d16.SetCheck(1);
		m_advance.EnableWindow(FALSE);
	}
	else if(DRIVERTYPE==2)
	{
		m_opengl = 1;
		m_Bopengl.SetCheck(1);
		m_advance.EnableWindow(TRUE);
	}

	SetColorDepth();
	SetResolution();

	UpdateData(TRUE);

	return TRUE;  // return TRUE  unless you set the focus to a control
}


void CVideoSetupDlg::SetColorDepth()
{
	if(COLOR_DEPTH==16)
	{
		m_16bit = 1;
		m_B16bit.SetCheck(1);
		m_32bit = -1;
		m_B32bit.SetCheck(0);
		if(!driver[DRIVERTYPE][1])
			m_B32bit.EnableWindow(FALSE);
		else
			m_B32bit.EnableWindow(TRUE);
	}
	else if(COLOR_DEPTH==32)
	{
		if(driver[DRIVERTYPE][1])
		{
			m_32bit = 1;
			m_B32bit.SetCheck(1);
			m_16bit = -1;
			m_B16bit.SetCheck(0);
		}
		else
		{
			COLOR_DEPTH=16;
			ZBUFFER_DEPTH = 16;
			m_16bit = 1;
			m_B16bit.SetCheck(1);
			m_32bit = -1;
			m_B32bit.SetCheck(0);
			m_B32bit.EnableWindow(FALSE);
		}
	}
}

void CVideoSetupDlg::SetResolution()
{
	if(COLOR_DEPTH==16)
	{
		if(resolution16[DRIVERTYPE][0])
		{
			m_Bsizelow.EnableWindow(TRUE);
		}
		else
		{
			m_Bsizelow.EnableWindow(FALSE);
		}
		if(resolution16[DRIVERTYPE][1])
		{
			m_Bsizemed.EnableWindow(TRUE);
		}
		else
		{
			m_Bsizemed.EnableWindow(FALSE);
			if(SCREENSIZE==1)
				SCREENSIZE = 0;
		}
		if(resolution16[DRIVERTYPE][2])
		{
			m_Bsizehigh.EnableWindow(TRUE);
		}
		else
		{
			m_Bsizehigh.EnableWindow(FALSE);
			if(SCREENSIZE==2)
				SCREENSIZE = 0;
		}
		if(resolution16[DRIVERTYPE][3])
		{
			m_Bsizevhigh.EnableWindow(TRUE);
		}
		else
		{
			m_Bsizevhigh.EnableWindow(FALSE);
			if(SCREENSIZE==3)
				SCREENSIZE = 0;
		}
		if(resolution16[DRIVERTYPE][4])
		{
			m_Bsizewin.EnableWindow(TRUE);
		}
		else
		{
			m_Bsizewin.EnableWindow(FALSE);
			if(SCREENSIZE==4)
				SCREENSIZE = 0;
		}
	}
	else
	{
		if(resolution32[DRIVERTYPE][0])
		{
			m_Bsizelow.EnableWindow(TRUE);
		}
		else
		{
			m_Bsizelow.EnableWindow(FALSE);
		}
		if(resolution32[DRIVERTYPE][1])
		{
			m_Bsizemed.EnableWindow(TRUE);
		}
		else
		{
			m_Bsizemed.EnableWindow(FALSE);
			if(SCREENSIZE==1)
				SCREENSIZE = 0;
		}
		if(resolution32[DRIVERTYPE][2])
		{
			m_Bsizehigh.EnableWindow(TRUE);
		}
		else
		{
			m_Bsizehigh.EnableWindow(FALSE);
			if(SCREENSIZE==2)
				SCREENSIZE = 0;
		}
		if(resolution32[DRIVERTYPE][3])
		{
			m_Bsizevhigh.EnableWindow(TRUE);
		}
		else
		{
			m_Bsizevhigh.EnableWindow(FALSE);
			if(SCREENSIZE==3)
				SCREENSIZE = 0;
		}
		if(resolution32[DRIVERTYPE][4])
		{
			m_Bsizewin.EnableWindow(TRUE);
		}
		else
		{
			m_Bsizewin.EnableWindow(FALSE);
			if(SCREENSIZE==4)
				SCREENSIZE = 0;
		}
	}
	if(SCREENSIZE==0)
	{
		m_sizelow = 1;
		m_Bsizelow.SetCheck(1);
		m_sizemed = -1;
		m_Bsizemed.SetCheck(0);
		m_sizehigh = -1;
		m_Bsizehigh.SetCheck(0);
		m_sizevhigh = -1;
		m_Bsizevhigh.SetCheck(0);
		m_sizewin = -1;
		m_Bsizewin.SetCheck(0);
	}
	else if(SCREENSIZE==1)
	{
		m_sizelow = -1;
		m_Bsizelow.SetCheck(0);
		m_sizemed = 1;
		m_Bsizemed.SetCheck(1);
		m_sizehigh = -1;
		m_Bsizehigh.SetCheck(0);
		m_sizevhigh = -1;
		m_Bsizevhigh.SetCheck(0);
		m_sizewin = -1;
		m_Bsizewin.SetCheck(0);
	}
	else if(SCREENSIZE==2)
	{
		m_sizelow = -1;
		m_Bsizelow.SetCheck(0);
		m_sizemed = -1;
		m_Bsizemed.SetCheck(0);
		m_sizehigh = 1;
		m_Bsizehigh.SetCheck(1);
		m_sizevhigh = -1;
		m_Bsizevhigh.SetCheck(0);
		m_sizewin = -1;
		m_Bsizewin.SetCheck(0);
	}
	else if(SCREENSIZE==3)
	{
		m_sizelow = -1;
		m_Bsizelow.SetCheck(0);
		m_sizemed = -1;
		m_Bsizemed.SetCheck(0);
		m_sizehigh = -1;
		m_Bsizehigh.SetCheck(0);
		m_sizevhigh = 1;
		m_Bsizevhigh.SetCheck(1);
		m_sizewin = -1;
		m_Bsizewin.SetCheck(0);
	}
	else if(SCREENSIZE==4)
	{
		m_sizelow = -1;
		m_Bsizelow.SetCheck(0);
		m_sizemed = -1;
		m_Bsizemed.SetCheck(0);
		m_sizehigh = -1;
		m_Bsizehigh.SetCheck(0);
		m_sizevhigh = -1;
		m_Bsizevhigh.SetCheck(0);
		m_sizewin = 1;
		m_Bsizewin.SetCheck(1);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CVideoSetupDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
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
	m_d3d16 = -1;
	m_Bd3d16.SetCheck(0);
	m_d3d32 = 1;
	m_Bd3d32.SetCheck(1);
	m_opengl = -1;
	m_Bopengl.SetCheck(0);
	SetColorDepth();
	SetResolution();
	m_advance.EnableWindow(TRUE);
	UpdateData(TRUE);

}

void CVideoSetupDlg::OnD3d16()
{
	DRIVERTYPE = 1;
	m_d3d16 = 1;
	m_Bd3d16.SetCheck(1);
	m_d3d32 = -1;
	m_Bd3d32.SetCheck(0);
	m_opengl = -1;
	m_Bopengl.SetCheck(0);
	SetColorDepth();
	SetResolution();
	if(m_ExpandDialog.m_bIsExpanded)
		m_ExpandDialog.OnExpandButton();
	m_advance.EnableWindow(FALSE);
	UpdateData(TRUE);
}

void CVideoSetupDlg::OnOpengl()
{
	DRIVERTYPE = 2;
	m_d3d16 = -1;
	m_Bd3d16.SetCheck(0);
	m_d3d32 = -1;
	m_Bd3d32.SetCheck(0);
	m_opengl = 1;
	m_Bopengl.SetCheck(1);
	SetColorDepth();
	SetResolution();
	m_advance.EnableWindow(TRUE);
	UpdateData(TRUE);
}

void CVideoSetupDlg::On16bit()
{
	COLOR_DEPTH=16;
	ZBUFFER_DEPTH = 16;
	m_16bit = 1;
	m_B16bit.SetCheck(1);
	m_32bit = -1;
	m_B32bit.SetCheck(0);
	SetResolution();
	UpdateData(TRUE);
}

void CVideoSetupDlg::On32bit()
{
	COLOR_DEPTH=32;
	ZBUFFER_DEPTH = 16;
	m_16bit = -1;
	m_B16bit.SetCheck(0);
	m_32bit = 1;
	m_B32bit.SetCheck(1);
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
