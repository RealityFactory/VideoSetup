// VideoSetupDlg.h : header file
//

#if !defined(__VIDEOSETUPDLG_H__)
#define __VIDEOSETUPDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>
#include "genesis.h"
#include "DialogExpander.h"

/////////////////////////////////////////////////////////////////////////////
// CVideoSetupDlg dialog

class CVideoSetupDlg
{
	// Construction
public:
	CVideoSetupDlg(HWND pParent = NULL);	// standard constructor
	~CVideoSetupDlg();

	BOOL Create();
	BOOL IsInitialized()	{ return m_bInit; }
	HWND GetHWnd()			{ return m_hWnd; }

	unsigned int COLOR_DEPTH;
	unsigned int ZBUFFER_DEPTH;
	int			 DRIVERTYPE;
	int			 SCREENSIZE;

	enum { IDD = IDD_VIDEOSETUP_DIALOG };
	HWND	m_advance;
	HWND	m_B32bit;
	HWND	m_B16bit;
	HWND	m_Bsizewin;
	HWND	m_Bsizevhigh;
	HWND	m_Bsizemed;
	HWND	m_Bsizelow;
	HWND	m_Bsizehigh;
	HWND	m_Bopengl;
	HWND	m_Bd3d32;
	HWND	m_Bd3d16;
	int		m_d3d16;
	int		m_d3d32;
	int		m_opengl;
	int		m_sizehigh;
	int		m_sizelow;
	int		m_sizemed;
	int		m_sizevhigh;
	int		m_sizewin;
	int		m_16bit;
	int		m_32bit;
	BOOL	m_TripleBuffering;
	BOOL	m_TextureCompression;
	BOOL	m_FSAA;
	BOOL	m_ExtraTextures;
	BOOL	m_NoVSync;
	BOOL	m_ASync;
	BOOL	m_DoNotWait;


protected:
	void SetColorDepth();
	void SetResolution();
	BOOL ReadIni();
	void WriteIni(unsigned int ColorDepth, unsigned int ZDepth);
	BOOL UpdateData(BOOL bSaveAndValidate = TRUE);

	geEngine		*m_pEngine;
	geDriver		*m_pDriver;
	geDriver_Mode	*m_pMode;
	geDriver_System	*m_pDriverSystem;
	std::string	m_drivername;
	bool driver[3][2];
	bool resolution16[3][5];
	bool resolution32[3][5];
	HWND m_hWnd;
	BOOL m_bInit;
	HANDLE hLogo;

// Implementation
public:
	HICON m_hIcon;

	CExpandDialog m_ExpandDialog;
	BOOL WritePrivateProfileInt(LPCTSTR lpAppName, LPCTSTR lpKeyName, INT Value, LPCTSTR lpFileName);

	BOOL OnInitDialog();
	void OnPaint();
	HCURSOR OnQueryDragIcon();
	void OnD3d32();
	void OnD3d16();
	void OnOpengl();
	void On16bit();
	void On32bit();
	void OnLow();
	void OnMed();
	void OnHigh();
	void OnVhigh();
	void OnWindow();
	void OnAdvance();
	void OnOk();
};

#endif // !defined(__VIDEOSETUPDLG_H__)
