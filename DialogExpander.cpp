// Expanding and collapsing dialogs
//
// Michael Walz (walz@epsitec.ch)
// Dec. 99
//

#include <windows.h>
#include "DialogExpander.h"


void ShrinkDialog(HWND pDlg, int idmark)
{
	HWND pWndMark = GetDlgItem(pDlg, idmark);
	RECT markrect;
	RECT dlgrect;
	RECT clientrect;
	HWND pParentWnd = GetParent(pDlg);
	int offset;

	GetClientRect(pDlg, &clientrect);  // clientrect of the dialog
	GetWindowRect(pDlg, &dlgrect);	  // rectangle of the dialog window

	// get height of the title bar
	offset = (dlgrect.bottom - dlgrect.top) - clientrect.bottom;

	GetWindowRect(pWndMark, &markrect);
	ScreenToClient(pDlg, (LPPOINT)&markrect);
	ScreenToClient(pDlg, ((LPPOINT)&markrect)+1);

	// calculate the new rectangle of the dialog window
	dlgrect.bottom = dlgrect.top + markrect.bottom + offset;

	MoveWindow(pDlg, dlgrect.left, dlgrect.top, (dlgrect.right-dlgrect.left), (dlgrect.bottom-dlgrect.top), TRUE);
}


CExpandDialog::CExpandDialog()
{
	m_bIsInitialized = FALSE;
}


void CExpandDialog::Initialize(
	HWND pDialog,
	BOOL bInitiallyExpanded,
	int IdExpandButton,
	int IdCollapsedMark,
	int IdCollapsedText)
{
	m_IdExpandButton = IdExpandButton;
	m_IdCollapsedMark = IdCollapsedMark;
	m_IdCollapsedText = IdCollapsedText;
	m_bIsInitialized = TRUE;
	m_pDialog = pDialog;
	m_bIsExpanded = TRUE;

	GetWindowRect(m_pDialog, &m_dialogrect);	  // rectangle of the dialog window

	GetDlgItemText(m_pDialog, IdExpandButton, m_sTextMore, 128);
	GetDlgItemText(m_pDialog, IdCollapsedText, m_sTextLess, 128);

	HWND pWndMark = GetDlgItem(m_pDialog, m_IdCollapsedText);
	ShowWindow(pWndMark, SW_HIDE);	// hide the "delimiting" control

	pWndMark = GetDlgItem(m_pDialog, m_IdCollapsedMark);
	ShowWindow(pWndMark, SW_HIDE);	// hide the "delimiting" control

	if(bInitiallyExpanded)
	{
		HWND pButton = GetDlgItem(m_pDialog, m_IdExpandButton);
		SetWindowText(pButton, m_sTextLess);
	}
	else
	{
		OnExpandButton();
	}
}


void CExpandDialog::OnExpandButton()
{
	HWND pButton;
	m_bIsExpanded = !m_bIsExpanded;

	if(m_bIsExpanded)
	{
		RECT dlgrect;
		GetWindowRect(m_pDialog, &dlgrect);
		dlgrect.right = dlgrect.left + (m_dialogrect.right - m_dialogrect.left);
		dlgrect.bottom = dlgrect.top + (m_dialogrect.bottom - m_dialogrect.top);
		MoveWindow(m_pDialog, dlgrect.left, dlgrect.top, dlgrect.right-dlgrect.left, dlgrect.bottom-dlgrect.top, TRUE);
	}
	else
	{
		ShrinkDialog(m_pDialog, m_IdCollapsedMark);
	}

	pButton = GetDlgItem(m_pDialog, m_IdExpandButton);
	SetWindowText(pButton, !m_bIsExpanded ? m_sTextMore : m_sTextLess);
}


