// VideoSetupDlg.h : header file
//

#if !defined(AFX_VIDEOSETUPDLG_H__2E7FAEE2_8021_4883_9C73_F69AD99145CD__INCLUDED_)
#define AFX_VIDEOSETUPDLG_H__2E7FAEE2_8021_4883_9C73_F69AD99145CD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "genesis.h"
#include "DialogExpander.h"

/////////////////////////////////////////////////////////////////////////////
// CVideoSetupDlg dialog

class CVideoSetupDlg : public CDialog
{
// Construction
public:
	CVideoSetupDlg(CWnd* pParent = NULL);	// standard constructor

	unsigned int COLOR_DEPTH;
	unsigned int ZBUFFER_DEPTH;
	int			 DRIVERTYPE;
	int			 SCREENSIZE;

// Dialog Data
	//{{AFX_DATA(CVideoSetupDlg)
	enum { IDD = IDD_VIDEOSETUP_DIALOG };
	CButton	m_advance;
	CButton	m_B32bit;
	CButton	m_B16bit;
	CButton	m_Bsizewin;
	CButton	m_Bsizevhigh;
	CButton	m_Bsizemed;
	CButton	m_Bsizelow;
	CButton	m_Bsizehigh;
	CButton	m_Bopengl;
	CButton	m_Bd3d32;
	CButton	m_Bd3d16;
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
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVideoSetupDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

	void SetColorDepth();
	void SetResolution();

	geEngine			*m_pEngine;
	geDriver			*m_pDriver;
	geDriver_Mode		*m_pMode;
	geDriver_System		*m_pDriverSystem;
	CString	m_drivername;
	bool driver[3][2];
	bool resolution16[3][5];
	bool resolution32[3][5];

// Implementation
protected:
	HICON m_hIcon;

	CExpandDialog m_ExpandDialog ;

	// Generated message map functions
	//{{AFX_MSG(CVideoSetupDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnD3d32();
	afx_msg void OnD3d16();
	afx_msg void OnOpengl();
	afx_msg void On16bit();
	afx_msg void On32bit();
	afx_msg void OnLow();
	afx_msg void OnMed();
	afx_msg void OnHigh();
	afx_msg void OnVhigh();
	afx_msg void OnWindow();
	afx_msg void OnAdvance();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIDEOSETUPDLG_H__2E7FAEE2_8021_4883_9C73_F69AD99145CD__INCLUDED_)
