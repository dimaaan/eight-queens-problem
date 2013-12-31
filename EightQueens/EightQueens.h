// EightQueens.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols


// CEightQueensApp:
// See EightQueens.cpp for the implementation of this class
//

class CEightQueensApp : public CWinApp
{
public:
	CEightQueensApp();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CEightQueensApp theApp;