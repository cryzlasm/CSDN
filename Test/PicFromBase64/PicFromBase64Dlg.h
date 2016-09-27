// PicFromBase64Dlg.h : header file
//

#if !defined(AFX_PICFROMBASE64DLG_H__6130BA21_F84B_4470_934C_8269CC118185__INCLUDED_)
#define AFX_PICFROMBASE64DLG_H__6130BA21_F84B_4470_934C_8269CC118185__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CPicFromBase64Dlg dialog

#include "Base64.h"

class CPicFromBase64Dlg : public CDialog
{
// Construction
public:
	CPicFromBase64Dlg(CWnd* pParent = NULL);	// standard constructor

    CBase64 Base64;

    CString m_strEnPic;
    PCHAR m_pData;
    PCHAR m_pEnData;
// Dialog Data
	//{{AFX_DATA(CPicFromBase64Dlg)
	enum { IDD = IDD_PICFROMBASE64_DIALOG };
	CEdit	m_EditCtl;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPicFromBase64Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CPicFromBase64Dlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	afx_msg void OnButton1();
	afx_msg void OnButton2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PICFROMBASE64DLG_H__6130BA21_F84B_4470_934C_8269CC118185__INCLUDED_)
