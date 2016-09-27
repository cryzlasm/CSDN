// PicFromBase64Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "PicFromBase64.h"
#include "PicFromBase64Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPicFromBase64Dlg dialog

CPicFromBase64Dlg::CPicFromBase64Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPicFromBase64Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPicFromBase64Dlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

    m_pData = NULL;
    m_pEnData = NULL;
}

void CPicFromBase64Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPicFromBase64Dlg)
	DDX_Control(pDX, IDC_EDIT1, m_EditCtl);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPicFromBase64Dlg, CDialog)
	//{{AFX_MSG_MAP(CPicFromBase64Dlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPicFromBase64Dlg message handlers

BOOL CPicFromBase64Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CPicFromBase64Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CPicFromBase64Dlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CPicFromBase64Dlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CPicFromBase64Dlg::OnOK() 
{
	// TODO: Add extra validation here
	
	//CDialog::OnOK();
}

void CPicFromBase64Dlg::OnButton1() 
{
    try
    {
	    
        CFile File("c:\\test.dat", CFile::modeRead);

        DWORD dwFileLen = File.GetLength();

        m_pData = NULL;
        m_pData = new CHAR[dwFileLen];

        File.Read(m_pData, dwFileLen);

        m_strEnPic = Base64.Encode(m_pData, dwFileLen).c_str();

        m_EditCtl.SetWindowText(m_strEnPic);

        File.Close();
    }
    catch(...){}
}

void CPicFromBase64Dlg::OnButton2() 
{
	// TODO: Add your control notification handler code here
    try
    {
        CFile Pic("c:\\Pic.dat", CFile::modeRead);
        
        char szBuf[MAXWORD] = {0};

        DWORD dwLen = Pic.GetLength();
        Pic.Read(szBuf, dwLen);


        int nLen = 0;
        CFile File("C:\\OutDeCodePic.png", CFile::modeCreate | CFile::modeWrite);

        PCHAR pData = Base64.Decode(szBuf, dwLen, nLen);

        File.Write(pData, nLen);

        m_EditCtl.SetWindowText("Êä³ö³É¹¦ c:\\OutDeCodePic.png");
        File.Flush();
        File.Close();
        
        delete [] pData;
        pData = NULL;
    }
    catch (std::string& str)
    {
        AfxMessageBox(str.c_str());
    } 
    

    
}
