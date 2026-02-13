// Аппроксимация с графикой.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CGrApp:
// See Аппроксимация с графикой.cpp for the implementation of this class
//

class CGrApp : public CWinApp
{
public:
	CGrApp();

	// Overrides
public:
	virtual BOOL InitInstance();

	// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CGrApp theApp;