// CSDNDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CSDN.h"
#include "CSDNDlg.h"


//#include "gzip/zstream.h"


#include <ATLBASE.H>

//BOOST�����ռ���ATLBASE.H ��ͻ����Ҫ������֮��
using namespace std;
using namespace boost;

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
// CCSDNDlg dialog

CCSDNDlg::CCSDNDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCSDNDlg::IDD, pParent), m_Assist()
{
	//{{AFX_DATA_INIT(CCSDNDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

    m_pHttpReq = NULL;

    m_Thread = NULL;
    m_bIsExitThread = FALSE;
    m_strUser = TEXT("");

    //�Ƿ�ʹ�ô���
    m_bIsProxyDebug = TRUE;
    //m_bIsProxyDebug = FALSE;

    m_nTime = 0;
}

void CCSDNDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCSDNDlg)
	DDX_Control(pDX, IDC_BTN_SET, m_BtnSetCtl);
	DDX_Control(pDX, IDC_BTN_SAVE, m_BtnSaveCtl);
	DDX_Control(pDX, IDC_BTN_REG, m_BtnRegCtl);
	DDX_Control(pDX, IDC_BTN_LOGIN, m_BtnLogCtl);
	DDX_Control(pDX, IDC_EDIT_REG_USER, m_RegUserCtl);
	DDX_Control(pDX, IDC_EDIT_TEST, m_TestCtl);
	DDX_Control(pDX, IDC_EDIT_HTML, m_HtmlCtl);
	DDX_Control(pDX, IDC_STATIC_INFO, m_InfoCtl);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCSDNDlg, CDialog)
	//{{AFX_MSG_MAP(CCSDNDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_REG, OnBtnReg)
	ON_BN_CLICKED(IDC_BTN_SAVE, OnBtnSave)
	ON_BN_CLICKED(IDC_BTN_LOGIN, OnBtnLogin)
	ON_BN_CLICKED(IDC_BTN_SET, OnBtnSet)
	ON_BN_CLICKED(IDC_BTN_TEST, OnBtnTest)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCSDNDlg message handlers

BOOL CCSDNDlg::OnInitDialog()
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
	
    CoInitialize(NULL); //COM����ĳ�ʼ��

    HRESULT hr = m_pHttpReq.CreateInstance(__uuidof(WinHttpRequest));
	if (FAILED(hr)) 
    {
        AfxMessageBox(TEXT("����Comʧ�ܣ�����ϵ����Ա!"));
        exit(0);
    }

    
    //����HTTPS����
    m_Assist.SetIgnoreHTTPS(m_pHttpReq);
    ShowButton(BTN_REG);

    return TRUE;  // return TRUE  unless you set the focus to a control
}

void CCSDNDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CCSDNDlg::OnPaint() 
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
HCURSOR CCSDNDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

//ת���ַ������û���
BOOL CCSDNDlg::ConvertUserFromMail(CString strSrcUser)
{
    if(strSrcUser.IsEmpty())
        return FALSE;

    int nPos = strSrcUser.Find(TEXT("@"));
    if(nPos == -1)
    {
        m_strUser = strSrcUser;
        return FALSE;
    }

    m_strUser = strSrcUser.Left(nPos);
    return TRUE;
}

BOOL CCSDNDlg::ShowButton(DWORD dwNum)
{
    //DisAble  ���а�ť
    m_BtnRegCtl.EnableWindow(FALSE);
    m_BtnSetCtl.EnableWindow(FALSE);
    m_BtnSaveCtl.EnableWindow(FALSE);
	m_BtnLogCtl.EnableWindow(FALSE);

    switch(dwNum)
    {
    case BTN_REG:
        m_BtnRegCtl.EnableWindow(TRUE);
        break;
        
    case BTN_SET:
        m_BtnSetCtl.EnableWindow(TRUE);
        break;
        
    case BTN_SAVE:
        m_BtnSaveCtl.EnableWindow(TRUE);
        break;
        
    case BTN_LOGIN:
	    m_BtnLogCtl.EnableWindow(TRUE);
        break;

    default:
        m_BtnRegCtl.EnableWindow(TRUE);
        break;
    }
    return TRUE;
}

void CCSDNDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	//CDialog::OnOK();
}

#define WEB_GET                 TEXT("GET")
#define WEB_POST                TEXT("POST")    

#define WEB_UA                  TEXT("User-Agent")
#define WEB_UA_INFO             TEXT("Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/50.0.2661.102 Safari/537.36")

#define WEB_ENCODE              TEXT("Accept-Encoding")
#define WEB_ENCODE_GZIP         TEXT("gzip, deflate")

#define WEB_LANGUAGE            TEXT("Accept-Language")
#define WEB_LANGUAGE_INFO       TEXT("zh-CN,zh;q=0.8")

#define WEB_CACHE_CONTROL       TEXT("Cache-Control")
#define WEB_CACHE_CONTROL_INFO  TEXT("max-age=0")

#define WEB_CACHE_PRAGMA        TEXT("Pragma")
#define WEB_NO_CACHE            TEXT("no-cache")

#define WEB_ACCEPT              TEXT("Accept")
#define WEB_ACCEPT_INFO         TEXT("text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8")

#define WEB_UPGRADE             TEXT("Upgrade-Insecure-Requests")
#define WEB_UPGRADE_INFO        TEXT("1")


#define WEB_CONNECTION          TEXT("Connection")
#define WEB_CONNECTION_INFO     TEXT("keep-alive")
#define WEB_HOST                TEXT("Host")
#define WEB_REFERER             TEXT("Referer")
#define WEB_COOKIE              TEXT("Cookie")

//�����׺
#define WEB_MAIL                TEXT("@spambog.com")

//��ʼRequest
#define BEGIN_HTTP_REQUEST  try{
    
//����Request
#define END_HTTP_REQUEST    }catch (_com_error &e){try{AfxMessageBox((LPTSTR)e.Description());}catch(...){}};


BOOL CCSDNDlg::ProxyHost()
{
    if(m_bIsProxyDebug)
    {
        m_Assist.SetProxy(m_pHttpReq);
    }
    return TRUE;
}


void CCSDNDlg::OnBtnReg() 
{
    ShowButton(BTN_REG);
    try
    {
        //======================����ҳ�õ�Cookie========================
        //GetCsdnע�����ӵõ�COOKIE
        m_pHttpReq->Open(WEB_GET, TEXT("http://passport.csdn.net/account/register?action=registerView&service=http://www.csdn.net"));
        
        //���ñ��ش���
        ProxyHost();
        
        m_pHttpReq->SetRequestHeader(WEB_CACHE_CONTROL, WEB_CACHE_CONTROL_INFO);
        m_pHttpReq->SetRequestHeader(WEB_CONNECTION, WEB_CONNECTION_INFO);
        m_pHttpReq->SetRequestHeader(WEB_ACCEPT, WEB_ACCEPT_INFO);
        m_pHttpReq->SetRequestHeader(WEB_ENCODE, WEB_ENCODE_GZIP);
        m_pHttpReq->SetRequestHeader(WEB_LANGUAGE, WEB_LANGUAGE_INFO);
        m_pHttpReq->SetRequestHeader(WEB_HOST, TEXT("passport.csdn.net"));
        m_pHttpReq->SetRequestHeader(WEB_REFERER, TEXT("http://passport.csdn.net/account/mobileregister?action=mobileRegister"));
        m_pHttpReq->SetRequestHeader(WEB_UA, WEB_UA_INFO);
        
        //������ҳ���õ�Cookie
        m_pHttpReq->Send();

        //��û�ִ��Ϣ
        _variant_t varRspBody = m_pHttpReq->GetResponseBody();	
        ULONG dataLen = varRspBody.parray->rgsabound[0].cElements;
		char *pContentBuffer = (char *)varRspBody.parray->pvData;

        //Gzip����
        std::string strOut = "";
        m_Assist.UnGZip((BYTE*)pContentBuffer, dataLen, strOut);

        //��ʾ���ݵ��ؼ�
        if(!strOut.empty())
            m_HtmlCtl.SetWindowText(m_Assist.UTF8ToGBK(strOut).c_str());
        //======================����ҳ�õ�Cookie========================

        //======================��֤��====================================
        //��֤����
        CString strGetCode = TEXT("");              //���ڱ�����֤��
        if(GetVerifyCode(strGetCode))
            m_InfoCtl.SetWindowText(TEXT("��֤�ɹ�: ") + strGetCode);
        //======================��֤��====================================

        //======================�ύע����Ϣ==============================
        BOOL bIsRegOk = FALSE;

        m_pHttpReq->Open(WEB_POST, TEXT("http://passport.csdn.net/account/register?action=saveUser&isFrom=false"));
        
        m_pHttpReq->SetRequestHeader(WEB_ACCEPT, TEXT("text/html, application/xhtml+xml, */*"));
        m_pHttpReq->SetRequestHeader(WEB_REFERER, TEXT("http://passport.csdn.net/account/register?action=registerView&service=http://www.csdn.net"));
        m_pHttpReq->SetRequestHeader(WEB_LANGUAGE, WEB_LANGUAGE_INFO);
        m_pHttpReq->SetRequestHeader(WEB_UA, WEB_UA_INFO);
        m_pHttpReq->SetRequestHeader(TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded"));
        m_pHttpReq->SetRequestHeader(WEB_ENCODE, WEB_ENCODE_GZIP);
        m_pHttpReq->SetRequestHeader(WEB_HOST, TEXT("passport.csdn.net"));
        m_pHttpReq->SetRequestHeader(WEB_CONNECTION, WEB_CONNECTION_INFO);
        m_pHttpReq->SetRequestHeader(WEB_CACHE_PRAGMA, WEB_NO_CACHE);

        //���һ��12λ�û���
        RandUser();
        //CString strMail = WEB_MAIL;
        CString strPostBody = TEXT("");
        strPostBody.Format(TEXT("fromUrl=http://www.csdn.ne&userName=%s&email=%s%s&password=123456&confirmpassword=123456&validateCode=%s&agree=on"),
                            m_strUser,
                            m_strUser,
                            WEB_MAIL,
                            strGetCode);

        //����Post����
        m_pHttpReq->Send((_bstr_t)strPostBody);

        //��ȡ��ִ��Ϣ
        long statusCode = m_pHttpReq->GetStatus();
        
        //��û�ִ��Ϣ
        varRspBody = m_pHttpReq->GetResponseBody();	
        dataLen = varRspBody.parray->rgsabound[0].cElements;
	    pContentBuffer = (char *)varRspBody.parray->pvData;

        if(statusCode == 200)
        {
            strOut = "";
            m_Assist.UnGZip((PBYTE)pContentBuffer, dataLen, strOut);
            
            //ת������
            //m_Assist.UTF8ToGBK(strOut);
            
            //���HTML ������
            CString strHtm = m_Assist.UTF8ToGBK(strOut).c_str();
            
            int nPos1 = strHtm.Find(TEXT("����24Сʱ�ڵ���ʼ��е����Ӽ������ע��"));
            int nPos2 = strHtm.Find(TEXT("�ʼ��ѷ��͵�����"));
            if(nPos1 != -1 && nPos2 != -1)
            {
                bIsRegOk = TRUE;
                m_HtmlCtl.SetWindowText(strHtm);
                m_InfoCtl.SetWindowText(TEXT("ע��ɹ���������֤�ʼ�"));

                CString strWrite = m_strUser + WEB_MAIL;
                m_RegUserCtl.SetWindowText(strWrite);
//                 CFile file("c:\\User.txt", CFile::modeCreate | CFile::modeWrite);
//                 file.Write(strWrite, strWrite.GetLength());
//                 file.Flush();
// 
//                 try
//                 {
//                     file.Close();
//                 }
//                 catch(...)
//                 {}
            }
            else
            {
                AfxMessageBox(TEXT("ע��ʧ�ܣ�����������\r\n��������������δ���\r\n��������Ա"));
            }
        }

        //======================�ύע����Ϣ==============================

        //======================��ע��������ȡע����֤==================


        //======================��ע��������ȡע����֤==================
        OnActiveReg(m_strUser);
    }
    catch (_com_error &e)
    {
        try
        {
            //��ӡ������Ϣ
            AfxMessageBox((LPTSTR)e.Description());
        }
        catch (...){}
    } 
    

}

//��֤�����, ��֤�ɹ��򷵻�True  ʧ����Flase
BOOL CCSDNDlg::GetVerifyCode(CString& strRetCode)
{
    BOOL bRet = FALSE;
    CString strTrue = TEXT("true");      //������֤��֤��ı�־
    try
    {
        DWORD dwCount = 0;
        while(1)
        {
            //�ƴΣ���ֹ��ѭ��
            if(dwCount++ == 3)
            {
                AfxMessageBox(TEXT("��֤�����3�Σ�ȡ����ȡ��֤�룡"));
                break;
            }

            //��ȡʱ���
            m_nTime = m_Assist.GetTimer();
            //ƴ����֤���ַ���
            CString strVerify = TEXT("");
            strVerify.Format(TEXT("http://passport.csdn.net/ajax/verifyhandler.ashx?rand=%d"), m_nTime);
            //strVerify = TEXT("http://passport.csdn.net/ajax/verifyhandler.ashx");

            m_pHttpReq->Open(WEB_GET, (_bstr_t)strVerify);

            m_pHttpReq->SetRequestHeader(WEB_ACCEPT, TEXT("image/png, image/svg+xml, image/*;q=0.8, */*;q=0.5"));
            m_pHttpReq->SetRequestHeader(WEB_REFERER, TEXT("http://passport.csdn.net/account/register?action=registerView&service=http://www.csdn.net/"));
            m_pHttpReq->SetRequestHeader(WEB_LANGUAGE, TEXT("zh-CN"));
            m_pHttpReq->SetRequestHeader(WEB_UA, WEB_UA_INFO);
            m_pHttpReq->SetRequestHeader(WEB_ENCODE, WEB_ENCODE_GZIP);
            m_pHttpReq->SetRequestHeader(WEB_HOST, TEXT("passport.csdn.net"));
            m_pHttpReq->SetRequestHeader(WEB_CONNECTION, WEB_CONNECTION_INFO);

            //������֤��
            m_pHttpReq->Send();


            //��ȡ��ִ��Ϣ
            long statusCode = m_pHttpReq->GetStatus();

            //��û�ִ��Ϣ
            _variant_t varRspBody = m_pHttpReq->GetResponseBody();	
            ULONG dataLen = varRspBody.parray->rgsabound[0].cElements;
	        char *pContentBuffer = (char *)varRspBody.parray->pvData;

            //����ͼƬ����
            if(pContentBuffer != NULL)
            {
                m_CodeDlg.SetPicData((LPVOID)pContentBuffer, dataLen);
            }

            //��ʾ
            m_CodeDlg.DoModal();
    
            //��ȡ�������֤��
            strRetCode = m_CodeDlg.GetInputCode();
    
            if(strRetCode.IsEmpty())
            {
                AfxMessageBox(TEXT("δ������֤�룡"));
                continue;
            }

            //ƴ����֤���ַ���
            strVerify = TEXT("");
            strVerify.Format(TEXT("http://passport.csdn.net/account/register?action=validateCode&validateCode=%s"), strRetCode);
    
            m_pHttpReq->Open(WEB_GET, (_bstr_t)strVerify);
    
            m_pHttpReq->SetRequestHeader(WEB_HOST, TEXT("passport.csdn.net"));
            m_pHttpReq->SetRequestHeader(WEB_CONNECTION, WEB_CONNECTION_INFO);
            m_pHttpReq->SetRequestHeader(WEB_ACCEPT, TEXT("*/*"));
            m_pHttpReq->SetRequestHeader(TEXT("X-Requested-With"), TEXT("XMLHttpRequest"));
            m_pHttpReq->SetRequestHeader(WEB_UA, WEB_UA_INFO);
            m_pHttpReq->SetRequestHeader(WEB_REFERER, TEXT("http://passport.csdn.net/account/register?action=registerView&service=http://www.csdn.net/"));
            m_pHttpReq->SetRequestHeader(WEB_ENCODE, WEB_ENCODE_GZIP);
            m_pHttpReq->SetRequestHeader(WEB_LANGUAGE, TEXT("zh-CN"));
    
            //������֤��
            m_pHttpReq->Send();

            varRspBody = m_pHttpReq->GetResponseBody();	
            dataLen = varRspBody.parray->rgsabound[0].cElements;
	        pContentBuffer = (char *)varRspBody.parray->pvData;

            CString strTrueCodeRep = pContentBuffer;

            strTrueCodeRep = strTrueCodeRep.Left(strTrue.GetLength());
            //У����֤��Ϣ
            if(!strTrueCodeRep.CompareNoCase(strTrue))
            {
                bRet = TRUE;
                break;
            }
        }//End While()
    
    }
    catch (_com_error &e)
    {
        try
        {
            //��ӡ������Ϣ
            AfxMessageBox((LPTSTR)e.Description());
        }
        catch (...)
        {}
    } 
    return bRet;
}



//�û�����12����λ�����
#define USER_MAX_LEN 6
BOOL CCSDNDlg::RandUser()
{
    //��ʼ���û���
    m_strUser = TEXT("");
    
    for(int i = 0; i < USER_MAX_LEN; i++)
    {
        m_strUser += m_Assist.GetRand();
    }
    

    return TRUE;
}


void CCSDNDlg::OnBtnSave() 
{
	// TODO: Add your control notification handler code here
	
}

void CCSDNDlg::OnBtnLogin() 
{
	// TODO: Add your control notification handler code here
    if(!GetNewRequest())
    {
        AfxMessageBox("��ʼ��Comʧ�ܣ�����ϵ����Ա");
        return;
    }

    //��ȡ�û���
    char szBuf[MAXBYTE] = {0};
    m_TestCtl.GetWindowText(szBuf, MAXBYTE);
    ConvertUserFromMail(szBuf);
    OnActiveReg(m_strUser);

    try
    {
        
    }
    catch (_com_error &e)
    {
        try
        {
            //��ӡ������Ϣ
            AfxMessageBox((LPTSTR)e.Description());
        }
        catch (...)
        {}
    } 
}

void CCSDNDlg::OnBtnSet() 
{
	// TODO: Add your control notification handler code here
	
}

BOOL CCSDNDlg::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
    if(m_pHttpReq != NULL)
    {
        try
        {
            m_pHttpReq.Release();
        }
        catch(...)
        {}
        
	}

    CoUninitialize();   //ж��COM���
	return CDialog::DestroyWindow();
}

void CCSDNDlg::OnActiveReg(CString strUser)
{
    try
    {
        //���ú���HTTPS
        m_Assist.SetIgnoreHTTPS(m_pHttpReq);
        m_Assist.SetHttpJmpFlase(m_pHttpReq);

        ProxyHost();
        //����ҳ���Cookie================================================
        m_pHttpReq->Open(WEB_GET, TEXT("https://discard.email/"));

        m_pHttpReq->SetRequestHeader(WEB_HOST, TEXT("discard.email"));
        m_pHttpReq->SetRequestHeader(WEB_CONNECTION, WEB_CONNECTION_INFO);
        m_pHttpReq->SetRequestHeader(WEB_ENCODE, WEB_ENCODE_GZIP);
        m_pHttpReq->SetRequestHeader(WEB_UA, WEB_UA_INFO);
        m_pHttpReq->SetRequestHeader(WEB_LANGUAGE, TEXT("zh-CN"));
        m_pHttpReq->SetRequestHeader(WEB_ACCEPT, TEXT("text/html, application/xhtml+xml, */*"));

        m_pHttpReq->Send();

        
        //POST�����ʱ����===================================================
        m_pHttpReq->Open(WEB_POST, TEXT("https://discard.email/"));

        m_pHttpReq->SetRequestHeader(WEB_ACCEPT, TEXT("text/html, application/xhtml+xml, */*"));
        m_pHttpReq->SetRequestHeader(WEB_REFERER, TEXT("https://discard.email/"));
        m_pHttpReq->SetRequestHeader(WEB_LANGUAGE, TEXT("zh-CN"));
        m_pHttpReq->SetRequestHeader(WEB_UA, WEB_UA_INFO);
        m_pHttpReq->SetRequestHeader(TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded"));
        m_pHttpReq->SetRequestHeader(WEB_ENCODE, WEB_ENCODE_GZIP);
        m_pHttpReq->SetRequestHeader(WEB_HOST, TEXT("discard.email"));
        m_pHttpReq->SetRequestHeader(WEB_CONNECTION, WEB_CONNECTION_INFO);
        m_pHttpReq->SetRequestHeader(WEB_CACHE_CONTROL, WEB_NO_CACHE);

        CString strPostBody = TEXT("");
        strPostBody.Format(TEXT("LocalPart=%s&DomainType=public&DomainId=101&PrivateDomain=&Password=&LoginButton=Postfach+abrufen&CopyAndPaste=laksjdikosajo@servermaps.net+"), 
                            strUser);

        m_pHttpReq->Send((_bstr_t)strPostBody);
    
        //��ȡ��ִ��Ϣ
        long statusCode = m_pHttpReq->GetStatus();

        BOOL bIsGetNext = FALSE;
        if(statusCode == 302)
                bIsGetNext = TRUE;

        if(!bIsGetNext)
        {
            OutputDebugString("Mail 302֮ǰ���˳�");
            return;
        }

        //ע��̫�죬�ʼ�û�е���
        Sleep(5000);

        //����302֮�����ת��ҳ�棬�õ��ʼ���ַ��Ϣ===========================
        m_pHttpReq->Open(WEB_GET, TEXT("https://discard.email/inbox.htm"));

        m_pHttpReq->SetRequestHeader(WEB_ACCEPT, TEXT("text/html, application/xhtml+xml, */*"));
        m_pHttpReq->SetRequestHeader(WEB_REFERER, TEXT("https://discard.email/"));
        m_pHttpReq->SetRequestHeader(WEB_LANGUAGE, TEXT("zh-CN"));
        m_pHttpReq->SetRequestHeader(WEB_UA, WEB_UA_INFO);
        m_pHttpReq->SetRequestHeader(TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded"));
        m_pHttpReq->SetRequestHeader(WEB_ENCODE, WEB_ENCODE_GZIP);
        m_pHttpReq->SetRequestHeader(WEB_HOST, TEXT("discard.email"));
        m_pHttpReq->SetRequestHeader(WEB_CONNECTION, WEB_CONNECTION_INFO);
        m_pHttpReq->SetRequestHeader(WEB_CACHE_CONTROL, WEB_NO_CACHE);

        m_pHttpReq->Send();
    
        //��ȡ��ִ��Ϣ
        statusCode = m_pHttpReq->GetStatus();

        //��û�ִ��Ϣ
        _variant_t varRspBody = m_pHttpReq->GetResponseBody();	
        ULONG dataLen = varRspBody.parray->rgsabound[0].cElements;
	    char *pContentBuffer = (char *)varRspBody.parray->pvData;

        std::string strOut = pContentBuffer;
        m_Assist.UnGZip((PBYTE)pContentBuffer, dataLen, strOut);

        strOut = m_Assist.UTF8ToGBK(strOut);

        CString strMailHtm = strOut.c_str();

        CString strNextHost = "";
        bIsGetNext = FALSE;
        if(statusCode == 200)
        {
            if(strMailHtm.Find("<div class=\"Head\"><a href=\"https://discard.email/message") != -1)
            {
                strNextHost = m_Assist.GetMidStrByLAndR(strOut, "<div class=\"Head\"><a href=\"", "\">service").c_str();
                bIsGetNext = TRUE;
            }

        }

        if(!bIsGetNext)
        {
            OutputDebugString("��ȡ�ʼ�����֮ǰ�˳�");
            return;
        }

        //��ȡ�ʼ�����======================================================
        m_pHttpReq->Open(WEB_GET, (_bstr_t)strNextHost);

        m_pHttpReq->SetRequestHeader(WEB_ACCEPT, TEXT("text/html, application/xhtml+xml, */*"));
        m_pHttpReq->SetRequestHeader(WEB_REFERER, TEXT("https://discard.email/inbox.htm"));
        m_pHttpReq->SetRequestHeader(WEB_LANGUAGE, TEXT("zh-CN"));
        m_pHttpReq->SetRequestHeader(WEB_UA, WEB_UA_INFO);
        m_pHttpReq->SetRequestHeader(WEB_ENCODE, WEB_ENCODE_GZIP);
        m_pHttpReq->SetRequestHeader(WEB_HOST, TEXT("discard.email"));
        m_pHttpReq->SetRequestHeader(WEB_CONNECTION, WEB_CONNECTION_INFO);
        
        m_pHttpReq->Send();

        //��ȡ��ִ��Ϣ
        statusCode = m_pHttpReq->GetStatus();
        
        //��û�ִ��Ϣ
        varRspBody = m_pHttpReq->GetResponseBody();	
        dataLen = varRspBody.parray->rgsabound[0].cElements;
        pContentBuffer = (char *)varRspBody.parray->pvData;
        
        strOut = "";
        m_Assist.UnGZip((PBYTE)pContentBuffer, dataLen, strOut);
        
        strOut = m_Assist.UTF8ToGBK(strOut);
        
        strMailHtm = strOut.c_str();

        CString strDstUrl = "";
        bIsGetNext = FALSE;
        if(statusCode == 200)
        {
            CString strLeft = "";
            strLeft.Format("username=%s&activeCode=", strUser);

            if(strMailHtm.Find("https://passport.csdn.net/account/login?username=") != -1)
            {
                //��ȡ�м�� ������
                strDstUrl = m_Assist.GetMidStrByLAndR(strOut, (LPCTSTR)strLeft, "&service=https:").c_str();
                
                //�ϳɼ�������
                strLeft.Format("https://passport.csdn.net/account/login?username=%s&activeCode=%s&service=https://passport.csdn.net/account/register?action=userInfoView",
                                    strUser,                
                                    strDstUrl);

                strDstUrl = strLeft;

                m_HtmlCtl.SetWindowText(strDstUrl);

                bIsGetNext = TRUE;
            }
        }

        if(!bIsGetNext)
        {
            OutputDebugString("�����˻�֮ǰ�˳�");
            return;
        }
        //�����˻�
        m_pHttpReq->Open(WEB_GET, (_bstr_t)strDstUrl);

        m_pHttpReq->SetRequestHeader(WEB_ACCEPT, TEXT("text/html, application/xhtml+xml, */*"));
        m_pHttpReq->SetRequestHeader(WEB_LANGUAGE, TEXT("zh-CN"));
        m_pHttpReq->SetRequestHeader(WEB_UA, WEB_UA_INFO);
        m_pHttpReq->SetRequestHeader(WEB_ENCODE, WEB_ENCODE_GZIP);
        m_pHttpReq->SetRequestHeader(WEB_HOST, TEXT("passport.csdn.net"));
        m_pHttpReq->SetRequestHeader(WEB_CONNECTION, WEB_CONNECTION_INFO);

        m_pHttpReq->Send();

        
        //��ȡ��ִ��Ϣ
        statusCode = m_pHttpReq->GetStatus();

        CString strAllHeader = (LPTSTR)m_pHttpReq->GetAllResponseHeaders();

        if(statusCode == 302)
        {
            if(strAllHeader.Find(TEXT("Set-Cookie")) != -1)
            {
                m_InfoCtl.SetWindowText(TEXT("�˻�����ɹ�\r\n�ɵ�¼\r\nҲ��������Ϣ��û���"));

                ShowButton(BTN_LOGIN);
            }
        }

    }//end TRy
    catch (_com_error &e)
    {
        try
        {
            //��ӡ������Ϣ
            AfxMessageBox((LPTSTR)e.Description());
        }
        catch (...)
        {}
    } 
}

void CCSDNDlg::OnBtnTest() 
{
	// TODO: Add your control notification handler code here
    if(!GetNewRequest())
    {
        AfxMessageBox("��ʼ��Comʧ�ܣ�����ϵ����Ա");
        return;
    }
    
    //��ȡ�û���
    char szBuf[MAXBYTE] = {0};
    m_TestCtl.GetWindowText(szBuf, MAXBYTE);
    ConvertUserFromMail(szBuf);
    //OnActiveReg(m_strUser);
    int nFirst = 0;
    int nSecond = 0;
    int nFail = 0;

    //��ʼRequest
    BEGIN_HTTP_REQUEST
    
//     for(int i = 0; i < 50; i++)
//     {
    //����ҳ���Cookie��ؼ��ַ���
    m_pHttpReq->Open(WEB_GET, TEXT("https://passport.csdn.net/account/login?from=http://my.csdn.net/my/mycsdn"));

    m_pHttpReq->SetRequestHeader(WEB_HOST, TEXT("passport.csdn.net"));
    m_pHttpReq->SetRequestHeader(WEB_CONNECTION, WEB_CONNECTION_INFO);
    m_pHttpReq->SetRequestHeader(WEB_ACCEPT, WEB_ACCEPT_INFO);
    m_pHttpReq->SetRequestHeader(WEB_UPGRADE, WEB_UPGRADE_INFO);
    m_pHttpReq->SetRequestHeader(WEB_UA, WEB_UA_INFO);
    m_pHttpReq->SetRequestHeader(WEB_REFERER, TEXT("http://www.csdn.net/"));
    m_pHttpReq->SetRequestHeader(WEB_LANGUAGE, WEB_LANGUAGE_INFO);


    Sleep(800);
    m_pHttpReq->Send();

    //��ȡ��ִ��Ϣ
    LONG statusCode = m_pHttpReq->GetStatus();
    
    //��û�ִ��Ϣ
    _variant_t varRspBody = m_pHttpReq->GetResponseBody();	
    DWORD dwDataLen = varRspBody.parray->rgsabound[0].cElements;
    PCHAR pContentBuffer = (PCHAR)varRspBody.parray->pvData;
    
    string strOut = pContentBuffer;
    //m_Assist.UnGZip((PBYTE)pContentBuffer, dwDataLen, strOut);
    
    //strOut = m_Assist.UTF8ToGBK(strOut);

    string strLt = "";
    string strExecution = "";
    string strEventId = "";
    //ƥ��LT
    {
        regex expressionLT("<input type=\"hidden\" name=\"lt\" value=\"(LT-[0-9a-zA-Z]{0,9}-[0-9a-zA-Z]{0,35})\"");
        regex expressionLT2("\"(LT-[0-9a-zA-Z]{0,9}-[0-9a-zA-Z]{0,30})\""); // ע��ת�巽ʽ

        cmatch what;
        // ����� regex_match ��������Ҫ��ȫƥ�䣬
        // �������ַ�������Ѱģʽ����������֤����
        if (!regex_search(strOut.c_str(), what, expressionLT))
        {
            OutputDebugString("��һ��δƥ��, ���ڶ���");
            if(!regex_search(strOut.c_str(), what, expressionLT))
            {
                OutputDebugString("���־�Ϊƥ����");
                nFail++;
                //continue;
            }
            nSecond ++;
        }

        nFirst++;
        strLt = what[1];
        OutputDebugString(strLt.c_str());
    }

    //ƥ��Execution
    {
        regex expressionLT("<input type=\"hidden\" name=\"execution\" value=\"([0-9a-zA-Z]{0,9})\"");
        regex expressionLT2("\"([0-9a-zA-Z]{0,9})\""); // ע��ת�巽ʽ
        
        cmatch what;
        // ����� regex_match ��������Ҫ��ȫƥ�䣬
        // �������ַ�������Ѱģʽ����������֤����
        if (!regex_search(strOut.c_str(), what, expressionLT))
        {
            OutputDebugString("��һ��δƥ��, ���ڶ���");
            if(!regex_search(strOut.c_str(), what, expressionLT))
            {
                OutputDebugString("���־�Ϊƥ����");
                nFail++;
                //continue;
            }
            nSecond ++;
        }
        
        nFirst++;
        strExecution = what[1];
        OutputDebugString(strExecution.c_str());
    }

    //ƥ��_eventId
    {
        regex expressionLT("<input type=\"hidden\" name=\"_eventId\" value=\"([0-9a-zA-Z]{0,9})\"");
        regex expressionLT2("\"([0-9a-zA-Z]{0,9})\""); // ע��ת�巽ʽ
        
        cmatch what;
        // ����� regex_match ��������Ҫ��ȫƥ�䣬
        // �������ַ�������Ѱģʽ����������֤����
        if (!regex_search(strOut.c_str(), what, expressionLT))
        {
            OutputDebugString("��һ��δƥ��, ���ڶ���");
            if(!regex_search(strOut.c_str(), what, expressionLT))
            {
                OutputDebugString("���־�Ϊƥ����");
                nFail++;
                //continue;
            }
            nSecond ++;
        }
        
        nFirst++;
        strEventId = what[1];
        OutputDebugString(strEventId.c_str());
    }
    //}//End FOR

    //��ȡ��֤��
    CString strValidateCode = TEXT("");
    if(!GetVerifyCode(strValidateCode))
        return;

    m_pHttpReq->Open(WEB_POST, TEXT("https://passport.csdn.net/account/login?from=http://my.csdn.net/my/mycsdn"));

    m_pHttpReq->SetRequestHeader(WEB_CACHE_CONTROL, TEXT("max-age=0"));
    m_pHttpReq->SetRequestHeader(TEXT("Origin"), TEXT("https://passport.csdn.net"));
    m_pHttpReq->SetRequestHeader(TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded"));
    m_pHttpReq->SetRequestHeader(WEB_REFERER, TEXT("https://passport.csdn.net/account/login?from=http://my.csdn.net/my/mycsdn"));

    CString strPostBody = TEXT("");

    //�û����� ���룬 ��֤�룬 LT, Execution, EventID
    strPostBody.Format(TEXT("username=%s&password=123456&validateCode=%s&lt=%s&execution=%s&_eventId=%s"),
                        m_strUser,
                        strValidateCode,
                        strLt.c_str(),
                        strExecution.c_str(),
                        strEventId.c_str());

    m_pHttpReq->Send((_bstr_t)strPostBody);

    BSTR bstrResponse = NULL;

    m_pHttpReq->GetResponseHeader(TEXT("Cookie"), &bstrResponse);
    CString strCookie = bstrResponse;

    //����Cookie
    try
    {
        CFile file(TEXT("C:\\") + m_strUser, CFile::modeCreate | CFile::modeWrite);
        file.Write(strCookie, strCookie.GetLength());

        file.Flush();
        file.Close();
    }
    catch(...){}


    //����Request
    END_HTTP_REQUEST

    CString strOver = "";
    strOver.Format("��һ��ƥ��: %d�Σ��ڶ���ƥ��: %d�Σ�δƥ����: %d.", nFirst, nSecond, nFail);
    OutputDebugString(strOver);
}

BOOL CCSDNDlg::GetNewRequest()
{
    //�ͷ�ԭ��
    if(m_pHttpReq != NULL)
        m_pHttpReq.Release();

    //�����µ�
    HRESULT hr = m_pHttpReq.CreateInstance(__uuidof(WinHttpRequest));
    if (FAILED(hr)) 
    {
        AfxMessageBox(TEXT("����Comʧ�ܣ�����ϵ����Ա!"));
        return FALSE;
    }

    m_Assist.SetIgnoreHTTPS(m_pHttpReq);
    m_Assist.SetHttpJmpFlase(m_pHttpReq);
    m_Assist.SetProxy(m_pHttpReq);

    return TRUE;
}

void CCSDNDlg::TestBaidu()
{

    try
    {
        //�������Ƿ�ʵ��
        if(m_pHttpReq == NULL)
        {
            AfxMessageBox(TEXT("���Ӷ���δ��ʼ����"));
            return;
        }
        
        //�������ַ
        m_pHttpReq->Open(WEB_GET, TEXT("http://passport.csdn.net/account/register?action=registerView&service=http://www.csdn.net"));
        
        //���ó�ʱ
        m_pHttpReq->SetTimeouts(15000, 15000, 15000, 15000);

        //�����߱��ش���
        HRESULT hr = m_Assist.SetProxy(m_pHttpReq);
        if (FAILED(hr))
        {
            AfxMessageBox(TEXT("���ô���ʧ��"));
            return;
        }

        //����HTTPͷ
        m_pHttpReq->SetRequestHeader(WEB_HOST, TEXT("passport.csdn.net"));
        m_pHttpReq->SetRequestHeader(WEB_CONNECTION, WEB_CONNECTION_INFO);
        m_pHttpReq->SetRequestHeader(WEB_CACHE_CONTROL, TEXT("max-age=0"));
        m_pHttpReq->SetRequestHeader(WEB_ACCEPT, WEB_ACCEPT_INFO);
        m_pHttpReq->SetRequestHeader(WEB_LANGUAGE, WEB_LANGUAGE_INFO);
        m_pHttpReq->SetRequestHeader(WEB_REFERER, TEXT("http://passport.csdn.net/account/mobileregister?action=mobileRegister"));
        m_pHttpReq->SetRequestHeader(WEB_UA, WEB_UA_INFO);
        

        //Get���� ����HTTP��Ϣ
        hr = m_pHttpReq->Send();
        if (FAILED(hr)) 
            return;

        //��û�ִ��Ϣ��Body
        _bstr_t bstrRep = m_pHttpReq->ResponseText;

        //��ȡ��ִ��Ϣ���е�ͷ��Ϣ
        _bstr_t bstrAllHeader = m_pHttpReq->GetAllResponseHeaders();

        //ת������ΪCString
        CString strHeader = (LPTSTR)bstrAllHeader;

        //ͨ��Httpͷ���COOKIES
        m_Assist.GetCookieString((LPCTSTR)bstrAllHeader, m_Assist.m_CookieMap).c_str();
        
        tstring strTmp = TEXT("");
        m_Assist.MapToCookie(strTmp, m_Assist.m_CookieMap);
        CString strCookie = strTmp.c_str();
        CString strRep = (LPTSTR)bstrRep;

        //AfxMessageBox(strHeader);
        //AfxMessageBox(strCookie);
        //AfxMessageBox(strRep);
        
        m_pHttpReq->Open(WEB_GET, TEXT("http://passport.csdn.net/account/register?action=registerView&service=http://www.csdn.net"));
        m_pHttpReq->Send();

        m_pHttpReq->Open(WEB_GET, TEXT("http://passport.csdn.net/account/register?action=registerView&service=http://www.csdn.net"));
        m_pHttpReq->SetRequestHeader(WEB_COOKIE, (_bstr_t)strCookie);

        m_pHttpReq->Send();

    }
    catch (_com_error &e)
    {
        try
        {
            //��ӡ������Ϣ
            AfxMessageBox((LPTSTR)e.Description());
        }
        catch (...){}
    } 
}
