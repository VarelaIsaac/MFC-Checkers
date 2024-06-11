
// MFC Checkers.h : main header file for the MFC Checkers application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CMFCCheckersApp:
// See MFC Checkers.cpp for the implementation of this class
//

class CMFCCheckersApp : public CWinApp
{
public:
	CMFCCheckersApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CMFCCheckersApp theApp;
