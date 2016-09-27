// PicFromBase64.h : main header file for the PICFROMBASE64 application
//

#if !defined(AFX_PICFROMBASE64_H__8945FEB2_0EFC_482D_90D2_9377651FF66B__INCLUDED_)
#define AFX_PICFROMBASE64_H__8945FEB2_0EFC_482D_90D2_9377651FF66B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CPicFromBase64App:
// See PicFromBase64.cpp for the implementation of this class
//

class CPicFromBase64App : public CWinApp
{
public:
	CPicFromBase64App();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPicFromBase64App)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CPicFromBase64App)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PICFROMBASE64_H__8945FEB2_0EFC_482D_90D2_9377651FF66B__INCLUDED_)
