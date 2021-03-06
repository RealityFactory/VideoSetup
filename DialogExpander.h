// Expanding and collapsing dialogs
//
// Michael Walz (walz@epsitec.ch)
// Dec. 99
//

#ifndef _INC_EXPANDDLG_H_
#define _INC_EXPANDDLG_H_

class CExpandDialog
{
	BOOL m_bIsInitialized;
	int m_IdExpandButton;
	int m_IdCollapsedMark;
	int m_IdCollapsedText;

	HWND m_pDialog;
	TCHAR m_sTextMore[128];
	TCHAR m_sTextLess[128];
	RECT m_dialogrect;

public:
	CExpandDialog();
	BOOL m_bIsExpanded;
	// call this function from the OnInitDialog() function
	void Initialize(HWND pDialog,	// pointer to dialog
		BOOL bInitiallyExpanded,	// TRUE => dialog is initially expanded
		int IdExpandButton,			// id of " More " / " Less " button
		int IdCollapsedMark,		// id of static that indicates position of
									// the bottom of the collapsed dialog
		int m_IdCollapsedText);		// id of static that cntains the text
									// "Less" ore something similar

	void OnExpandButton() ;			// call this in the handler of the
									// "More"/"Less" button
} ;


#endif /* _INC_EXPANDDLG_H_ */