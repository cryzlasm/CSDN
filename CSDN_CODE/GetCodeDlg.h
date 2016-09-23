#if !defined(AFX_GETCODEDLG_H__40CC5F0A_E521_4E72_9B4A_2AA79971C3C3__INCLUDED_)
#define AFX_GETCODEDLG_H__40CC5F0A_E521_4E72_9B4A_2AA79971C3C3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GetCodeDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGetCodeDlg dialog

#pragma comment(lib, "GdiPlus.lib")
#define ULONG_PTR ULONG

#include <GdiPlus.h>
using namespace Gdiplus;

#include "Assist.h"

class CGetCodeDlg : public CDialog
{
// Construction
public:
	CGetCodeDlg(CWnd* pParent = NULL);   // standard constructor

    //图片缓冲区
    PCHAR   m_pMemData;

    //图片缓冲区长度
    DWORD   m_dwDataLen;

    //设置图片信息
    BOOL SetPicData(IWinHttpRequestPtr& pObj);
    
    BOOL SetPicData(LPVOID pSrc, DWORD dwLen);

    BOOL SetPicData(CString strFileName);


    //文件名
    CString m_strFile;

    //显示图片
    BOOL ShowPicInMem();

    BOOL ShowPicInFile();

    BOOL bIsShowFromFile;
    ULONG_PTR m_gdiplusToken;
// Dialog Data
	//{{AFX_DATA(CGetCodeDlg)
	enum { IDD = IDD_DLG_GET_CODE };
	CStatic	m_PicCodeCtl;
	CEdit	m_GetCodeCtl;
	CString	m_strGetCode;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGetCodeDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CGetCodeDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
    //ULONG_PTR m_gdiplusToken;
    //Gdiplus::GdiplusStartupInput m_StartupInput;  
    
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GETCODEDLG_H__40CC5F0A_E521_4E72_9B4A_2AA79971C3C3__INCLUDED_)
