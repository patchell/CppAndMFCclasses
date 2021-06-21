
// ComboxWithLB.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CComboxWithLBApp:
// See ComboxWithLB.cpp for the implementation of this class
//

class CComboxWithLBApp : public CWinApp
{
public:
	CComboxWithLBApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CComboxWithLBApp theApp;