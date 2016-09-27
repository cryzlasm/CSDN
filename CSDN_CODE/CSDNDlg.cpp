// CSDNDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CSDN.h"
#include "CSDNDlg.h"


//#include "gzip/zstream.h"


#include <ATLBASE.H>

//BOOST命名空间与ATLBASE.H 冲突，需要放在它之下
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

    //是否使用代理
    m_bIsProxyDebug = TRUE;
    //m_bIsProxyDebug = FALSE;

    m_nTime = 0;

    m_strHostCookie = TEXT("");
    m_bIsUserActive = FALSE;
    m_strTmpBuf = TEXT("");

    m_bIsUserLogin =FALSE;
}

void CCSDNDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCSDNDlg)
	DDX_Control(pDX, IDC_BTN_ACTIVE, m_BtnActiveCtl);
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
	ON_BN_CLICKED(IDC_BTN_ACTIVE, OnBtnActive)
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
	
    CoInitialize(NULL); //COM组件的初始化

//     HRESULT hr = m_pHttpReq.CreateInstance(__uuidof(WinHttpRequest));
// 	if (FAILED(hr)) 
//     {
//         AfxMessageBox(TEXT("导入Com失败，请联系管理员!"));
//         exit(0);
//     }
// 
//     
//     //设置HTTPS忽略
//     m_Assist.SetIgnoreHTTPS(m_pHttpReq);

    GetNewRequest();
    ShowButton(BTN_REG | BTN_LOGIN);

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

//转换字符串到用户名
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
    return FALSE;
    //DisAble  所有按钮
    m_BtnRegCtl.EnableWindow(FALSE);
    m_BtnSetCtl.EnableWindow(FALSE);
    m_BtnSaveCtl.EnableWindow(FALSE);
    m_BtnLogCtl.EnableWindow(FALSE);
	m_BtnActiveCtl.EnableWindow(FALSE);

    if(dwNum & BTN_REG || !dwNum)
        m_BtnRegCtl.EnableWindow(TRUE);

    
    if(dwNum & BTN_SET)
        m_BtnSetCtl.EnableWindow(TRUE);

    
    if(dwNum & BTN_SAVE)
        m_BtnSaveCtl.EnableWindow(TRUE);

    
    if(dwNum & BTN_LOGIN)
        m_BtnLogCtl.EnableWindow(TRUE);

    if(dwNum & BTN_ACTIVE)
        m_BtnActiveCtl.EnableWindow(TRUE);
    
    

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

//邮箱后缀
#define WEB_MAIL                TEXT("@spambog.com")

//开始Request
#define BEGIN_HTTP_REQUEST  try{
    
//结束Request
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
    ShowButton(BTN_REG | BTN_LOGIN);
    try
    {
        //======================打开主页得到Cookie========================
        //GetCsdn注册连接得到COOKIE
        m_pHttpReq->Open(WEB_GET, TEXT("http://passport.csdn.net/account/register?action=registerView&service=http://www.csdn.net"));
        
        //设置本地代理
        ProxyHost();
        
        m_pHttpReq->SetRequestHeader(WEB_CACHE_CONTROL, WEB_CACHE_CONTROL_INFO);
        m_pHttpReq->SetRequestHeader(WEB_CONNECTION, WEB_CONNECTION_INFO);
        m_pHttpReq->SetRequestHeader(WEB_ACCEPT, WEB_ACCEPT_INFO);
        m_pHttpReq->SetRequestHeader(WEB_ENCODE, WEB_ENCODE_GZIP);
        m_pHttpReq->SetRequestHeader(WEB_LANGUAGE, WEB_LANGUAGE_INFO);
        m_pHttpReq->SetRequestHeader(WEB_HOST, TEXT("passport.csdn.net"));
        m_pHttpReq->SetRequestHeader(WEB_REFERER, TEXT("http://passport.csdn.net/account/mobileregister?action=mobileRegister"));
        m_pHttpReq->SetRequestHeader(WEB_UA, WEB_UA_INFO);
        
        //请求网页，得到Cookie
        m_pHttpReq->Send();

        //获得回执信息
        _variant_t varRspBody = m_pHttpReq->GetResponseBody();	
        ULONG dataLen = varRspBody.parray->rgsabound[0].cElements;
		char *pContentBuffer = (char *)varRspBody.parray->pvData;

        //Gzip解码
        std::string strOut = "";
        m_Assist.UnGZip((BYTE*)pContentBuffer, dataLen, strOut);

        //显示数据到控件
        if(!strOut.empty())
            m_HtmlCtl.SetWindowText(m_Assist.UTF8ToGBK(strOut).c_str());
        //======================打开主页得到Cookie========================

        //======================验证码====================================
        //验证流程
        CString strGetCode = TEXT("");              //用于保存验证码
        if(GetVerifyCode(strGetCode))
            m_InfoCtl.SetWindowText(TEXT("验证成功: ") + strGetCode);
        //======================验证码====================================

        //======================提交注册信息==============================
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

        //随机一个12位用户名
        RandUser();
        //CString strMail = WEB_MAIL;
        CString strPostBody = TEXT("");
        strPostBody.Format(TEXT("fromUrl=http://www.csdn.ne&userName=%s&email=%s%s&password=123456&confirmpassword=123456&validateCode=%s&agree=on"),
                            m_strUser,
                            m_strUser,
                            WEB_MAIL,
                            strGetCode);

        //发送Post数据
        m_pHttpReq->Send((_bstr_t)strPostBody);

        //获取回执信息
        long statusCode = m_pHttpReq->GetStatus();
        
        //获得回执信息
        varRspBody = m_pHttpReq->GetResponseBody();	
        dataLen = varRspBody.parray->rgsabound[0].cElements;
	    pContentBuffer = (char *)varRspBody.parray->pvData;

        if(statusCode == 200)
        {
            strOut = "";
            m_Assist.UnGZip((PBYTE)pContentBuffer, dataLen, strOut);
            
            //转换编码
            //m_Assist.UTF8ToGBK(strOut);
            
            //获得HTML 的数据
            CString strHtm = m_Assist.UTF8ToGBK(strOut).c_str();
            
            int nPos1 = strHtm.Find(TEXT("请在24小时内点击邮件中的链接继续完成注册"));
            int nPos2 = strHtm.Find(TEXT("邮件已发送到邮箱"));
            if(nPos1 != -1 && nPos2 != -1)
            {
                bIsRegOk = TRUE;
                m_HtmlCtl.SetWindowText(strHtm);
                m_InfoCtl.SetWindowText(TEXT("注册成功，接收验证邮件"));

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
                AfxMessageBox(TEXT("注册失败，请重启程序\r\n如果多次重启，均未解决\r\n请联管理员"));
            }
        }

        //======================提交注册信息==============================

        //======================打开注册邮箱提取注册验证==================


        //======================打开注册邮箱提取注册验证==================
        OnActiveReg(m_strUser);
        if(!m_bIsUserActive)
        {
            ShowButton(BTN_ACTIVE | BTN_LOGIN);
            AfxMessageBox(m_strTmpBuf);
            m_InfoCtl.SetWindowText(TEXT("自动激活失败，请手动激活"));
        }
    }
    catch (_com_error &e)
    {
        try
        {
            //打印错误信息
            AfxMessageBox((LPTSTR)e.Description());
        }
        catch (...){}
    } 
    

}

//验证码相关, 验证成功则返回True  失败则Flase
BOOL CCSDNDlg::GetVerifyCode(CString& strRetCode)
{
    BOOL bRet = FALSE;
    CString strTrue = TEXT("true");      //用于验证验证码的标志
    try
    {
        DWORD dwCount = 0;
        while(1)
        {
            //计次，防止死循环
            if(dwCount++ == 3)
            {
                AfxMessageBox(TEXT("验证码错误3次，取消获取验证码！"));
                break;
            }

            //获取时间戳
            m_nTime = m_Assist.GetTimer();
            //拼接验证码字符串
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

            //访问验证码
            m_pHttpReq->Send();


            //获取回执信息
            long statusCode = m_pHttpReq->GetStatus();

            //获得回执信息
            _variant_t varRspBody = m_pHttpReq->GetResponseBody();	
            ULONG dataLen = varRspBody.parray->rgsabound[0].cElements;
	        char *pContentBuffer = (char *)varRspBody.parray->pvData;

            //设置图片数据
            if(pContentBuffer != NULL)
            {
                m_CodeDlg.SetPicData((LPVOID)pContentBuffer, dataLen);
            }

            //显示
            m_CodeDlg.DoModal();
    
            //获取输入的验证码
            strRetCode = m_CodeDlg.GetInputCode();
    
            if(strRetCode.IsEmpty())
            {
                AfxMessageBox(TEXT("未输入验证码！"));
                continue;
            }

            //拼接验证码字符串
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
    
            //访问验证码
            m_pHttpReq->Send();

            varRspBody = m_pHttpReq->GetResponseBody();	
            dataLen = varRspBody.parray->rgsabound[0].cElements;
	        pContentBuffer = (char *)varRspBody.parray->pvData;

            CString strTrueCodeRep = pContentBuffer;

            strTrueCodeRep = strTrueCodeRep.Left(strTrue.GetLength());
            //校验验证信息
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
            //打印错误信息
            AfxMessageBox((LPTSTR)e.Description());
        }
        catch (...)
        {}
    } 
    return bRet;
}



//用户名在12个的位置随机
#define USER_MAX_LEN 6
BOOL CCSDNDlg::RandUser()
{
    //初始化用户名
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
    m_bIsUserLogin = FALSE;
	// TODO: Add your control notification handler code here
    if(!GetNewRequest())
    {
        AfxMessageBox("初始化Com失败，请联系管理员");
        return;
    }
    
    //获取用户名================================================
    char szBuf[MAXBYTE] = {0};
    m_RegUserCtl.GetWindowText(szBuf, MAXBYTE);
    ConvertUserFromMail(szBuf);
    //OnActiveReg(m_strUser);
    int nFirst = 0;
    int nSecond = 0;
    int nFail = 0;

    //开始Request================================================
    BEGIN_HTTP_REQUEST

    //打开主页获得Cookie与关键字符串================================================
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

    //获取回执信息
    LONG statusCode = m_pHttpReq->GetStatus();
    
    //获得回执信息
    _variant_t varRspBody = m_pHttpReq->GetResponseBody();	
    DWORD dwDataLen = varRspBody.parray->rgsabound[0].cElements;
    PCHAR pContentBuffer = (PCHAR)varRspBody.parray->pvData;
    
    string strOut = pContentBuffer;
    //m_Assist.UnGZip((PBYTE)pContentBuffer, dwDataLen, strOut);
    
    //strOut = m_Assist.UTF8ToGBK(strOut);

    string strLt = "";
    string strExecution = "";
    string strEventId = "";
    //匹配LT
    {
        regex expressionLT("<input type=\"hidden\" name=\"lt\" value=\"(LT-[0-9a-zA-Z]{0,9}-[0-9a-zA-Z]{0,35})\"");
        regex expressionLT2("\"(LT-[0-9a-zA-Z]{0,9}-[0-9a-zA-Z]{0,30})\""); // 注意转义方式

        cmatch what;
        // 如果用 regex_match 方法将需要完全匹配，
        // 不能在字符串中找寻模式，可用于验证输入
        if (!regex_search(strOut.c_str(), what, expressionLT))
        {
            OutputDebugString("第一种未匹配, 换第二种");
            if(!regex_search(strOut.c_str(), what, expressionLT))
            {
                OutputDebugString("两种均为匹配上");
                nFail++;
                //continue;
            }
            nSecond ++;
        }

        nFirst++;
        strLt = what[1];
        OutputDebugString(strLt.c_str());
    }

    //匹配Execution
    {
        regex expressionLT("<input type=\"hidden\" name=\"execution\" value=\"([0-9a-zA-Z]{0,9})\"");
        regex expressionLT2("\"([0-9a-zA-Z]{0,9})\""); // 注意转义方式
        
        cmatch what;
        // 如果用 regex_match 方法将需要完全匹配，
        // 不能在字符串中找寻模式，可用于验证输入
        if (!regex_search(strOut.c_str(), what, expressionLT))
        {
            OutputDebugString("第一种未匹配, 换第二种");
            if(!regex_search(strOut.c_str(), what, expressionLT))
            {
                OutputDebugString("两种均为匹配上");
                nFail++;
                //continue;
            }
            nSecond ++;
        }
        
        nFirst++;
        strExecution = what[1];
        OutputDebugString(strExecution.c_str());
    }

    //匹配_eventId
    {
        regex expressionLT("<input type=\"hidden\" name=\"_eventId\" value=\"([0-9a-zA-Z]{0,9})\"");
        regex expressionLT2("\"([0-9a-zA-Z]{0,9})\""); // 注意转义方式
        
        cmatch what;
        // 如果用 regex_match 方法将需要完全匹配，
        // 不能在字符串中找寻模式，可用于验证输入
        if (!regex_search(strOut.c_str(), what, expressionLT))
        {
            OutputDebugString("第一种未匹配, 换第二种");
            if(!regex_search(strOut.c_str(), what, expressionLT))
            {
                OutputDebugString("两种均为匹配上");
                nFail++;
                //continue;
            }
            nSecond ++;
        }
        
        nFirst++;
        strEventId = what[1];
        OutputDebugString(strEventId.c_str());
    }

    //获取验证码================================================
    CString strValidateCode = TEXT("");
    if(!GetVerifyCode(strValidateCode))
        return;

    //登录
    m_pHttpReq->Open(WEB_POST, TEXT("https://passport.csdn.net/account/login?from=http://my.csdn.net/my/mycsdn"));

    m_pHttpReq->SetRequestHeader(WEB_CACHE_CONTROL, TEXT("max-age=0"));
    m_pHttpReq->SetRequestHeader(TEXT("Origin"), TEXT("https://passport.csdn.net"));
    m_pHttpReq->SetRequestHeader(TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded"));
    m_pHttpReq->SetRequestHeader(WEB_REFERER, TEXT("https://passport.csdn.net/account/login?from=http://my.csdn.net/my/mycsdn"));

    CString strPostBody = TEXT("");

    //用户名， 密码， 验证码， LT, Execution, EventID
    strPostBody.Format(TEXT("username=%s&password=123456&validateCode=%s&lt=%s&execution=%s&_eventId=%s"),
                        m_strUser,
                        strValidateCode,
                        strLt.c_str(),
                        strExecution.c_str(),
                        strEventId.c_str());

    m_pHttpReq->Send((_bstr_t)strPostBody);

    //获取回执信息
    statusCode = m_pHttpReq->GetStatus();
    
    //获得回执信息
    varRspBody = m_pHttpReq->GetResponseBody();	
    dwDataLen = varRspBody.parray->rgsabound[0].cElements;
    pContentBuffer = (PCHAR)varRspBody.parray->pvData;

    //POST Data 在 m_pHttpReq "8C"偏移上

    if(statusCode != 200)
    {
        AfxMessageBox("登录失败，请查看网络连接是否不正常！");
        return;
    }

    CString strHtm = pContentBuffer;
    if(strHtm.Find(TEXT("=\"/content/loginbox/loginapi.js")) == 1)
    {
        AfxMessageBox(TEXT("请检查账户是否存在，是否已经激活"));
        return;
    }

    //获取登录Cookie================================================
    m_strHostCookie = TEXT("");
    CString strLoginCookie = (LPCTSTR)m_pHttpReq->GetAllResponseHeaders();

    tstring strTmpCookie = strLoginCookie;
    m_Assist.GetCookieString(strTmpCookie, m_Assist.m_CookieMap);


    strTmpCookie = TEXT("");
    if(!m_Assist.MapToCookie(strTmpCookie, m_Assist.m_CookieMap))
    {
        AfxMessageBox(TEXT("获取登录 Cookie 失败"));
        return;
    }

    m_strHostCookie = strTmpCookie.c_str();

    m_HtmlCtl.SetWindowText(m_strHostCookie);
    m_bIsUserLogin = TRUE;
    m_InfoCtl.SetWindowText(TEXT("登录成功！"));


    //按用户名保存Cookie================================================
    try
    {
        CString strPath = TEXT("c:\\CSDN_Cookie\\");

        //未创建目录，则返回
        if(!PathIsOk(strPath))
            return;
        
        CFile file(strPath + m_strUser, CFile::modeCreate | CFile::modeWrite);
        file.Write(m_strHostCookie, m_strHostCookie.GetLength());

        file.Flush();
        file.Close();
    }
    catch(...){}


    //结束Request================================================
    END_HTTP_REQUEST

    CString strOver = "";
    strOver.Format("第一种匹配: %d次，第二种匹配: %d次，未匹配上: %d.", nFirst, nSecond, nFail);
    OutputDebugString(strOver);
}

void CCSDNDlg::OnBtnSet() 
{
	//判断是否已经登录，因为需要登录的Cookie
    if(!m_bIsUserLogin)
    {
        AfxMessageBox(TEXT("请先登录，再设置账户信息！"));
        return;
    }

    //获取用户名================================================
    char szBuf[MAXBYTE] = {0};
    m_RegUserCtl.GetWindowText(szBuf, MAXBYTE);
    ConvertUserFromMail(szBuf);
    
    //m_strTmpBuf = szBuf;

    BEGIN_HTTP_REQUEST

    #define REF_TOOLBAR TEXT("http://my.csdn.net/?ref=toolbar")
    #define REP_SUCEED  TEXT("\"err\":0,\"msg\":\"ok\"")
    //提交个人信息================================================
    //设置熟悉领域---------------------------
    {
        m_pHttpReq->Open(WEB_GET, TEXT("http://my.csdn.net/service/main/do_add_knownarea?areaname=Python"));
    
        m_pHttpReq->SetRequestHeader(WEB_HOST, TEXT("my.csdn.net"));
        m_pHttpReq->SetRequestHeader(WEB_CONNECTION, WEB_CONNECTION_INFO);
        m_pHttpReq->SetRequestHeader(WEB_ACCEPT, TEXT("application/json, text/javascript, */*; q=0.01"));
        m_pHttpReq->SetRequestHeader(TEXT("X-Requested-With"), TEXT("XMLHttpRequest"));
        m_pHttpReq->SetRequestHeader(WEB_UA, WEB_UA_INFO);
        m_pHttpReq->SetRequestHeader(WEB_REFERER, REF_TOOLBAR);
        //m_pHttpReq->SetRequestHeader(WEB_ENCODE, WEB_ENCODE_GZIP);
        m_pHttpReq->SetRequestHeader(WEB_LANGUAGE, WEB_LANGUAGE_INFO);

        m_pHttpReq->Send();

        //获取回执信息
        LONG statusCode = m_pHttpReq->GetStatus();

        //取出Body
        string strOut;
        m_Assist.Get_WinHttp_RspString(m_pHttpReq, strOut);

    
        CString strHtm = m_Assist.UTF8ToGBK(strOut).c_str();
    
        //判断回执标志
        if(statusCode != 200)
        {
            m_InfoCtl.SetWindowText(strHtm);
            AfxMessageBox(TEXT("网络错误或其他原因"));
            return;
        }

        //判断是否成功
        if(strHtm.Find(REP_SUCEED) == -1)
        {
            m_InfoCtl.SetWindowText(strHtm);
            AfxMessageBox(TEXT("设置表单出错，很大概率放弃此号！"));
            return;
        }

        m_InfoCtl.SetWindowText(TEXT("设置熟悉领域成功!"));
    }//End 设置熟悉领域---------------------------
    
    //设置专业技能---------------------------
    {
        m_pHttpReq->Open(WEB_POST, TEXT("http://my.csdn.net/service/main/uc"));

        m_pHttpReq->SetRequestHeader(WEB_HOST, TEXT("my.csdn.net"));
        m_pHttpReq->SetRequestHeader(WEB_CONNECTION, WEB_CONNECTION_INFO);
        m_pHttpReq->SetRequestHeader(WEB_ACCEPT, TEXT("application/json, text/javascript, */*; q=0.01"));
        m_pHttpReq->SetRequestHeader(TEXT("Origin"), TEXT("http://my.csdn.net"));
        m_pHttpReq->SetRequestHeader(TEXT("X-Requested-With"), TEXT("XMLHttpRequest"));
        m_pHttpReq->SetRequestHeader(WEB_UA, WEB_UA_INFO);
        m_pHttpReq->SetRequestHeader(TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded; charset=UTF-8"));
        m_pHttpReq->SetRequestHeader(WEB_REFERER, REF_TOOLBAR);
        //m_pHttpReq->SetRequestHeader(WEB_ENCODE, WEB_ENCODE_GZIP);
        m_pHttpReq->SetRequestHeader(WEB_LANGUAGE, WEB_LANGUAGE_INFO);

        CString strPostBody = TEXT("");

        strPostBody = TEXT("params=%7B%22skillname%22%3A%22Python%22%2C%22skilllevel%22%3A%221%22%2C%22method%22%3A%22saveSkill%22%2C%22skillid%22%3A0%7D");

        m_pHttpReq->Send((_bstr_t)strPostBody);

        //获取回执信息
        LONG statusCode = m_pHttpReq->GetStatus();

        //取出Body
        string strOut;
        m_Assist.Get_WinHttp_RspString(m_pHttpReq, strOut);

    
        CString strHtm = m_Assist.UTF8ToGBK(strOut).c_str();
    
        //判断回执标志
        if(statusCode != 200)
        {
            m_InfoCtl.SetWindowText(strHtm);
            AfxMessageBox(TEXT("网络错误或其他原因"));
            return;
        }

        //判断是否成功
        if(strHtm.Find(REP_SUCEED) == -1)
        {
            m_InfoCtl.SetWindowText(strHtm);
            AfxMessageBox(TEXT("设置表单出错，很大概率放弃此号！"));
            return;
        }
        m_InfoCtl.SetWindowText(TEXT("设置专业技能成功!"));
    }//end 设置专业技能---------------------------

    //设置联系方式---------------------------
    {
        m_pHttpReq->Open(WEB_POST, TEXT("http://my.csdn.net/service/main/uc"));

        m_pHttpReq->SetRequestHeader(WEB_HOST, TEXT("my.csdn.net"));
        m_pHttpReq->SetRequestHeader(WEB_CONNECTION, WEB_CONNECTION_INFO);
        m_pHttpReq->SetRequestHeader(WEB_ACCEPT, TEXT("application/json, text/javascript, */*; q=0.01"));
        m_pHttpReq->SetRequestHeader(TEXT("Origin"), TEXT("http://my.csdn.net"));
        m_pHttpReq->SetRequestHeader(TEXT("X-Requested-With"), TEXT("XMLHttpRequest"));
        m_pHttpReq->SetRequestHeader(WEB_UA, WEB_UA_INFO);
        m_pHttpReq->SetRequestHeader(TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded; charset=UTF-8"));
        m_pHttpReq->SetRequestHeader(WEB_REFERER, REF_TOOLBAR);
        //m_pHttpReq->SetRequestHeader(WEB_ENCODE, WEB_ENCODE_GZIP);
        m_pHttpReq->SetRequestHeader(WEB_LANGUAGE, WEB_LANGUAGE_INFO);

        CString strPostBody = TEXT("");

        strPostBody = (TEXT("params=%7B%22method%22%3A%22saveUserContact%22%2C%22username%22%3A%22") + m_strUser + TEXT("%22%2C%22contactinfo%22%3A%5B%7B%22contactid%22%3A0%2C%22value%22%3A%22123456%22%2C%22type%22%3A70%7D%2C%7B%22contactid%22%3A0%2C%22value%22%3A%22123456%22%2C%22type%22%3A110%7D%5D%2C%22pubemail%22%3A%22123456%40qq.com%22%2C%22submobile%22%3A%2213888888888%22%7D"));

        m_pHttpReq->Send((_bstr_t)strPostBody);

        //获取回执信息
        LONG statusCode = m_pHttpReq->GetStatus();

        //取出Body
        string strOut;
        m_Assist.Get_WinHttp_RspString(m_pHttpReq, strOut);

    
        CString strHtm = m_Assist.UTF8ToGBK(strOut).c_str();
    
        //判断回执标志
        if(statusCode != 200)
        {
            m_InfoCtl.SetWindowText(strHtm);
            AfxMessageBox(TEXT("网络错误或其他原因"));
            return;
        }

        //判断是否成功
        if(strHtm.Find(REP_SUCEED) == -1)
        {
            m_InfoCtl.SetWindowText(strHtm);
            AfxMessageBox(TEXT("设置表单出错，很大概率放弃此号！"));
            return;
        }
        m_InfoCtl.SetWindowText(TEXT("设置联系方式成功!"));
    }//end 设置联系方式---------------------------

    //设置教育经历---------------------------
    {
        m_pHttpReq->Open(WEB_POST, TEXT("http://my.csdn.net/service/main/uc"));

        m_pHttpReq->SetRequestHeader(WEB_HOST, TEXT("my.csdn.net"));
        m_pHttpReq->SetRequestHeader(WEB_CONNECTION, WEB_CONNECTION_INFO);
        m_pHttpReq->SetRequestHeader(WEB_ACCEPT, TEXT("application/json, text/javascript, */*; q=0.01"));
        m_pHttpReq->SetRequestHeader(TEXT("Origin"), TEXT("http://my.csdn.net"));
        m_pHttpReq->SetRequestHeader(TEXT("X-Requested-With"), TEXT("XMLHttpRequest"));
        m_pHttpReq->SetRequestHeader(WEB_UA, WEB_UA_INFO);
        m_pHttpReq->SetRequestHeader(TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded; charset=UTF-8"));
        m_pHttpReq->SetRequestHeader(WEB_REFERER, REF_TOOLBAR);
        //m_pHttpReq->SetRequestHeader(WEB_ENCODE, WEB_ENCODE_GZIP);
        m_pHttpReq->SetRequestHeader(WEB_LANGUAGE, WEB_LANGUAGE_INFO);

        CString strPostBody = TEXT("");

        strPostBody = (TEXT("params=%7B%22schoolname%22%3A%22%E5%AE%B6%E9%87%8C%E8%B9%B2%22%2C%22majorstr%22%3A%22%E6%97%A0%E6%97%A0%22%2C%22degree%22%3A%221%22%2C%22edustartdate%22%3A%222016-09-02%22%2C%22eduenddate%22%3A%22%22%2C%22eduid%22%3A%220%22%2C%22method%22%3A%22saveEduExp%22%2C%22username%22%3A%22") + m_strUser + TEXT("%22%7D"));

        m_pHttpReq->Send((_bstr_t)strPostBody);

        //获取回执信息
        LONG statusCode = m_pHttpReq->GetStatus();

        //取出Body
        string strOut;
        m_Assist.Get_WinHttp_RspString(m_pHttpReq, strOut);

    
        CString strHtm = m_Assist.UTF8ToGBK(strOut).c_str();
    
        //判断回执标志
        if(statusCode != 200)
        {
            m_InfoCtl.SetWindowText(strHtm);
            AfxMessageBox(TEXT("网络错误或其他原因"));
            return;
        }

        //判断是否成功
        if(strHtm.Find(REP_SUCEED) == -1)
        {
            m_InfoCtl.SetWindowText(strHtm);
            AfxMessageBox(TEXT("设置表单出错，很大概率放弃此号！"));
            return;
        }
        m_InfoCtl.SetWindowText(TEXT("设置教育经历成功!"));
    }//end 设置教育经历---------------------------

    //设置工作经历---------------------------
    {
        m_pHttpReq->Open(WEB_POST, TEXT("http://my.csdn.net/service/main/uc"));

        m_pHttpReq->SetRequestHeader(WEB_HOST, TEXT("my.csdn.net"));
        m_pHttpReq->SetRequestHeader(WEB_CONNECTION, WEB_CONNECTION_INFO);
        m_pHttpReq->SetRequestHeader(WEB_ACCEPT, TEXT("application/json, text/javascript, */*; q=0.01"));
        m_pHttpReq->SetRequestHeader(TEXT("Origin"), TEXT("http://my.csdn.net"));
        m_pHttpReq->SetRequestHeader(TEXT("X-Requested-With"), TEXT("XMLHttpRequest"));
        m_pHttpReq->SetRequestHeader(WEB_UA, WEB_UA_INFO);
        m_pHttpReq->SetRequestHeader(TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded; charset=UTF-8"));
        m_pHttpReq->SetRequestHeader(WEB_REFERER, REF_TOOLBAR);
        //m_pHttpReq->SetRequestHeader(WEB_ENCODE, WEB_ENCODE_GZIP);
        m_pHttpReq->SetRequestHeader(WEB_LANGUAGE, WEB_LANGUAGE_INFO);

        CString strPostBody = TEXT("");

        strPostBody = TEXT("params=%7B%22orgname%22%3A%22%E6%97%A0%E6%97%A0%22%2C%22job%22%3A%22%E6%97%A0%E6%97%A0%22%2C%22workbegindate%22%3A%222016-09-01%22%2C%22workenddate%22%3A%22%22%2C%22workdesc%22%3A%22%E6%97%A0%E6%97%A0%22%2C%22workid%22%3A%220%22%2C%22method%22%3A%22saveWorkExp%22%2C%22username%22%3A%22") + m_strUser + TEXT("%22%7D");

        m_pHttpReq->Send((_bstr_t)strPostBody);

        //获取回执信息
        LONG statusCode = m_pHttpReq->GetStatus();

        //取出Body
        string strOut;
        m_Assist.Get_WinHttp_RspString(m_pHttpReq, strOut);

    
        CString strHtm = m_Assist.UTF8ToGBK(strOut).c_str();
    
        //判断回执标志
        if(statusCode != 200)
        {
            m_InfoCtl.SetWindowText(strHtm);
            AfxMessageBox(TEXT("网络错误或其他原因"));
            return;
        }

        //判断是否成功
        if(strHtm.Find(REP_SUCEED) == -1)
        {
            m_InfoCtl.SetWindowText(strHtm);
            AfxMessageBox(TEXT("设置表单出错，很大概率放弃此号！"));
            return;
        }
        m_InfoCtl.SetWindowText(TEXT("设置工作经历成功!"));
    }//end 设置工作经历---------------------------

    //设置基本信息---------------------------
    {
        m_pHttpReq->Open(WEB_POST, TEXT("http://my.csdn.net/service/main/uc"));

        m_pHttpReq->SetRequestHeader(WEB_HOST, TEXT("my.csdn.net"));
        m_pHttpReq->SetRequestHeader(WEB_CONNECTION, WEB_CONNECTION_INFO);
        m_pHttpReq->SetRequestHeader(WEB_ACCEPT, TEXT("application/json, text/javascript, */*; q=0.01"));
        m_pHttpReq->SetRequestHeader(TEXT("Origin"), TEXT("http://my.csdn.net"));
        m_pHttpReq->SetRequestHeader(TEXT("X-Requested-With"), TEXT("XMLHttpRequest"));
        m_pHttpReq->SetRequestHeader(WEB_UA, WEB_UA_INFO);
        m_pHttpReq->SetRequestHeader(TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded; charset=UTF-8"));
        m_pHttpReq->SetRequestHeader(WEB_REFERER, REF_TOOLBAR);
        //m_pHttpReq->SetRequestHeader(WEB_ENCODE, WEB_ENCODE_GZIP);
        m_pHttpReq->SetRequestHeader(WEB_LANGUAGE, WEB_LANGUAGE_INFO);

        CString strPostBody = TEXT("");

        strPostBody = TEXT("params=%7B%22nickname%22%3A%22") + m_strUser + 
                      TEXT("%22%2C%22realname%22%3A%22%E5%8C%BF%E5%90%8D%22%2C%22curjob%22%3A%22%E6%97%A0%E6%97%A0%22%2C%22gender%22%3A%221%22%2C%22birthday%22%3A%222016-9-1%22%2C%22industrytype%22%3A%22400%22%2C%22country%22%3A%221%22%2C%22province%22%3A%22110000%22%2C%22district%22%3A%22110101%22%2C%22city%22%3A%22110100%22%2C%22selfdesc%22%3A%22%E6%B2%A1%E5%95%A5%E8%AF%B4%E7%9A%84%22%2C%22method%22%3A%22saveDetailInfo%22%2C%22username%22%3A%22") +
                      m_strUser + TEXT("%22%7D");

        m_pHttpReq->Send((_bstr_t)strPostBody);

        //获取回执信息
        LONG statusCode = m_pHttpReq->GetStatus();

        //取出Body
        string strOut;
        m_Assist.Get_WinHttp_RspString(m_pHttpReq, strOut);

    
        CString strHtm = m_Assist.UTF8ToGBK(strOut).c_str();
    
        //判断回执标志
        if(statusCode != 200)
        {
            m_InfoCtl.SetWindowText(strHtm);
            AfxMessageBox(TEXT("网络错误或其他原因"));
            return;
        }

        //判断是否成功
        if(strHtm.Find(REP_SUCEED) == -1)
        {
            m_InfoCtl.SetWindowText(strHtm);
            AfxMessageBox(TEXT("设置表单出错，很大概率放弃此号！"));
            return;
        }
        m_InfoCtl.SetWindowText(TEXT("设置基本信息成功!"));
    }//end 设置基本信息---------------------------

    //设置头像---------------------------
    {
        m_pHttpReq->Open(WEB_POST, TEXT("http://my.csdn.net/service/main/do_base64_avatar"));

        m_pHttpReq->SetRequestHeader(WEB_HOST, TEXT("my.csdn.net"));
        m_pHttpReq->SetRequestHeader(WEB_CONNECTION, WEB_CONNECTION_INFO);
        m_pHttpReq->SetRequestHeader(WEB_ACCEPT, TEXT("application/json, text/javascript, */*; q=0.01"));
        m_pHttpReq->SetRequestHeader(TEXT("Origin"), TEXT("http://my.csdn.net"));
        m_pHttpReq->SetRequestHeader(TEXT("X-Requested-With"), TEXT("XMLHttpRequest"));
        m_pHttpReq->SetRequestHeader(WEB_UA, WEB_UA_INFO);
        //m_pHttpReq->SetRequestHeader(TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded; charset=UTF-8"));

        //说明多个节 在 数据中， 分割点为WebKitFormBoundaryhENPilH3pE2DZv8u
        m_pHttpReq->SetRequestHeader(TEXT("Content-Type"), TEXT("multipart/form-data; boundary=----WebKitFormBoundaryhENPilH3pE2DZv8u"));
        
        m_pHttpReq->SetRequestHeader(WEB_REFERER, REF_TOOLBAR);
        //m_pHttpReq->SetRequestHeader(WEB_ENCODE, WEB_ENCODE_GZIP);
        m_pHttpReq->SetRequestHeader(WEB_LANGUAGE, WEB_LANGUAGE_INFO);

        CString strPostBody = TEXT("");
        
        //从文件读数据
        try
        {
            //读取Post信息
            CFile File(TEXT("PicData.dat"), CFile::modeRead);

            DWORD dwFileLen = File.GetLength();
            
            PCHAR pBuf = NULL;
            pBuf = new CHAR[dwFileLen + 1];
            if(pBuf == NULL)
            {
                m_InfoCtl.SetWindowText(TEXT("申请缓冲区失败，设置头像失败，请手动设置头像！"));
                AfxMessageBox(TEXT("申请缓冲区失败，设置头像失败，请手动设置头像！"));
                return;
            }

            if(File.Read(pBuf, dwFileLen) != dwFileLen)
            {
                m_InfoCtl.SetWindowText(TEXT("读取数据文件失败，设置头像失败，请手动设置头像！"));
                AfxMessageBox(TEXT("读取数据文件失败，设置头像失败，请手动设置头像！"));
                return;
            }

            //文件数据末尾补'\0'
            pBuf[dwFileLen] = TEXT('\0');

            strPostBody = pBuf;
            delete pBuf;
            pBuf = NULL;

            File.Close();
        	
        }
        catch (CFileException  &e)
        {
            TCHAR szBuf[MAXBYTE] = {0};

            e.GetErrorMessage(szBuf, MAXBYTE - 1);
            AfxMessageBox(szBuf);
        } 

        //Post 提交头像信息
        m_pHttpReq->Send((_bstr_t)strPostBody);

        //获取回执信息
        LONG statusCode = m_pHttpReq->GetStatus();

        //取出Body
        string strOut;
        m_Assist.Get_WinHttp_RspString(m_pHttpReq, strOut);

        CString strHtm = m_Assist.UTF8ToGBK(strOut).c_str();
    
        //判断回执标志
        if(statusCode != 200)
        {
            m_InfoCtl.SetWindowText(strHtm);
            AfxMessageBox(TEXT("网络错误或其他原因"));
            return;
        }

        //判断是否成功
        if(strHtm.Find(REP_SUCEED) == -1)
        {
            m_InfoCtl.SetWindowText(strHtm);
            AfxMessageBox(TEXT("设置表单出错，很大概率放弃此号！"));
            return;
        }
        m_InfoCtl.SetWindowText(TEXT("设置头像成功!"));
    }//end 设置头像---------------------------
    
    //获取填写资料的金币
    {
        m_pHttpReq->Open(WEB_GET, TEXT("http://my.csdn.net/service/main/do_send_coin"));

        m_pHttpReq->SetRequestHeader(WEB_HOST, TEXT("my.csdn.net"));
        m_pHttpReq->SetRequestHeader(WEB_CONNECTION, WEB_CONNECTION_INFO);
        m_pHttpReq->SetRequestHeader(WEB_ACCEPT, TEXT("application/json, text/javascript, */*; q=0.01"));
        m_pHttpReq->SetRequestHeader(TEXT("X-Requested-With"), TEXT("XMLHttpRequest"));
        m_pHttpReq->SetRequestHeader(WEB_UA, WEB_UA_INFO);
        m_pHttpReq->SetRequestHeader(WEB_REFERER, REF_TOOLBAR);
        //m_pHttpReq->SetRequestHeader(WEB_ENCODE, WEB_ENCODE_GZIP);
        m_pHttpReq->SetRequestHeader(WEB_LANGUAGE, WEB_LANGUAGE_INFO);

        m_pHttpReq->Send();
        
        //获取回执信息
        LONG statusCode = m_pHttpReq->GetStatus();
        
        //取出Body
        string strOut;
        m_Assist.Get_WinHttp_RspString(m_pHttpReq, strOut);
        
        
        CString strHtm = m_Assist.UTF8ToGBK(strOut).c_str();
        
        //判断回执标志
        if(statusCode != 200)
        {
            m_InfoCtl.SetWindowText(strHtm);
            AfxMessageBox(TEXT("网络错误或其他原因"));
            return;
        }
        
        //判断是否成功
        if(strHtm.Find(REP_SUCEED) == -1)
        {
            m_InfoCtl.SetWindowText(strHtm);
            AfxMessageBox(TEXT("设置表单出错，很大概率放弃此号！"));
            return;
        }

        m_InfoCtl.SetWindowText(TEXT("金币领取成功！"));
    }

    //Http访问结束
    END_HTTP_REQUEST
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

    CoUninitialize();   //卸载COM组件
	return CDialog::DestroyWindow();
}

void CCSDNDlg::OnActiveReg(CString strUser)
{
    m_bIsUserActive = FALSE;
    try
    {
        //设置忽略HTTPS
        m_Assist.SetIgnoreHTTPS(m_pHttpReq);
        m_Assist.SetHttpJmpFlase(m_pHttpReq);

        ProxyHost();
        //打开主页获得Cookie================================================
        m_pHttpReq->Open(WEB_GET, TEXT("https://discard.email/"));

        m_pHttpReq->SetRequestHeader(WEB_HOST, TEXT("discard.email"));
        m_pHttpReq->SetRequestHeader(WEB_CONNECTION, WEB_CONNECTION_INFO);
        m_pHttpReq->SetRequestHeader(WEB_ENCODE, WEB_ENCODE_GZIP);
        m_pHttpReq->SetRequestHeader(WEB_UA, WEB_UA_INFO);
        m_pHttpReq->SetRequestHeader(WEB_LANGUAGE, TEXT("zh-CN"));
        m_pHttpReq->SetRequestHeader(WEB_ACCEPT, TEXT("text/html, application/xhtml+xml, */*"));

        m_pHttpReq->Send();

        
        //POST获得临时邮箱===================================================
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
    
        //获取回执信息
        long statusCode = m_pHttpReq->GetStatus();

        BOOL bIsGetNext = FALSE;
        if(statusCode == 302)
                bIsGetNext = TRUE;

        if(!bIsGetNext)
        {
            m_strTmpBuf = TEXT("打开邮箱失败， 请联系管理员！");
            OutputDebugString(TEXT("Mail 302之前，退出"));
            return;
        }

        //注册太快，邮件没有到达
        Sleep(5000);

        //访问302之后的跳转主页面，得到邮件地址信息===========================
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
    
        //获取回执信息
        statusCode = m_pHttpReq->GetStatus();

        //获得回执信息
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
            m_strTmpBuf = TEXT("获取激活信息失败，可能邮件还没到达邮箱");
            OutputDebugString("获取邮件内容之前退出");
            return;
        }

        //获取邮件内容======================================================
        m_pHttpReq->Open(WEB_GET, (_bstr_t)strNextHost);

        m_pHttpReq->SetRequestHeader(WEB_ACCEPT, TEXT("text/html, application/xhtml+xml, */*"));
        m_pHttpReq->SetRequestHeader(WEB_REFERER, TEXT("https://discard.email/inbox.htm"));
        m_pHttpReq->SetRequestHeader(WEB_LANGUAGE, TEXT("zh-CN"));
        m_pHttpReq->SetRequestHeader(WEB_UA, WEB_UA_INFO);
        m_pHttpReq->SetRequestHeader(WEB_ENCODE, WEB_ENCODE_GZIP);
        m_pHttpReq->SetRequestHeader(WEB_HOST, TEXT("discard.email"));
        m_pHttpReq->SetRequestHeader(WEB_CONNECTION, WEB_CONNECTION_INFO);
        
        m_pHttpReq->Send();

        //获取回执信息
        statusCode = m_pHttpReq->GetStatus();
        
        //获得回执信息
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
                //获取中间的 激活码
                strDstUrl = m_Assist.GetMidStrByLAndR(strOut, (LPCTSTR)strLeft, "&service=https:").c_str();
                
                //合成激活连接
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
            m_strTmpBuf = TEXT("账户激活失败，请重试，多次均未成功，请联系管理员");
            OutputDebugString("激活账户之前退出");
            return;
        }
        //激活账户
        m_pHttpReq->Open(WEB_GET, (_bstr_t)strDstUrl);

        m_pHttpReq->SetRequestHeader(WEB_ACCEPT, TEXT("text/html, application/xhtml+xml, */*"));
        m_pHttpReq->SetRequestHeader(WEB_LANGUAGE, TEXT("zh-CN"));
        m_pHttpReq->SetRequestHeader(WEB_UA, WEB_UA_INFO);
        m_pHttpReq->SetRequestHeader(WEB_ENCODE, WEB_ENCODE_GZIP);
        m_pHttpReq->SetRequestHeader(WEB_HOST, TEXT("passport.csdn.net"));
        m_pHttpReq->SetRequestHeader(WEB_CONNECTION, WEB_CONNECTION_INFO);

        m_pHttpReq->Send();

        
        //获取回执信息
        statusCode = m_pHttpReq->GetStatus();

        CString strAllHeader = (LPTSTR)m_pHttpReq->GetAllResponseHeaders();

        if(statusCode == 302)
        {
            if(strAllHeader.Find(TEXT("Set-Cookie")) != -1)
            {
                m_InfoCtl.SetWindowText(TEXT("账户激活成功\r\n可登录\r\n也可设置信息获得积分"));

                ShowButton(BTN_LOGIN);
            }
        }


    }//end TRy
    catch (_com_error &e)
    {
        try
        {
            //打印错误信息
            AfxMessageBox((LPTSTR)e.Description());
        }
        catch (...)
        {}
    } 

    m_bIsUserActive = TRUE;
}

void CCSDNDlg::OnBtnTest() 
{
    //判断是否已经登录，因为需要登录的Cookie
    if(!m_bIsUserLogin)
    {
        AfxMessageBox(TEXT("请先登录，再设置账户信息！"));
        return;
    }
    
    //获取用户名================================================
    char szBuf[MAXBYTE] = {0};
    m_RegUserCtl.GetWindowText(szBuf, MAXBYTE);
    ConvertUserFromMail(szBuf);
    
    //m_strTmpBuf = szBuf;
    BEGIN_HTTP_REQUEST
    
    //查看是否有5个金币===============================
    {
        m_pHttpReq->Open(WEB_GET, TEXT("http://mall.csdn.net/cbuy/buy_download_coin"));

        m_pHttpReq->SetRequestHeader(WEB_HOST, TEXT("mall.csdn.net"));
        m_pHttpReq->SetRequestHeader(WEB_CONNECTION, WEB_CONNECTION_INFO);
        m_pHttpReq->SetRequestHeader(WEB_CACHE_CONTROL, TEXT("max-age=0"));
        m_pHttpReq->SetRequestHeader(WEB_ACCEPT, WEB_ACCEPT_INFO);
        m_pHttpReq->SetRequestHeader(WEB_UPGRADE, WEB_UPGRADE_INFO);
        m_pHttpReq->SetRequestHeader(WEB_UA, WEB_UA_INFO);
        m_pHttpReq->SetRequestHeader(WEB_REFERER, TEXT("http://mall.csdn.net/cbuy"));
        m_pHttpReq->SetRequestHeader(WEB_ENCODE, WEB_ENCODE_GZIP);
        m_pHttpReq->SetRequestHeader(WEB_LANGUAGE, WEB_LANGUAGE_INFO);

        m_pHttpReq->Send();

        //获取回执信息
        LONG statusCode = m_pHttpReq->GetStatus();
        
        //判断回执标志
        if(statusCode != 200)
        {
            //m_InfoCtl.SetWindowText(strHtm);
            AfxMessageBox(TEXT("网络错误或其他原因"));
            return;
        }

        //获得回执信息
        _variant_t varRspBody = m_pHttpReq->GetResponseBody();	
        ULONG dataLen = varRspBody.parray->rgsabound[0].cElements;
        char *pContentBuffer = (char *)varRspBody.parray->pvData;
        
        std::string strOut = "";
        m_Assist.UnGZip((PBYTE)pContentBuffer, dataLen, strOut);
        
        strOut = m_Assist.UTF8ToGBK(strOut);
        
        //获取金币数量
        CString strNum = TEXT("");
        //正则取数据
        {
            regex expressionLT("</label><span><i>([0-9]{0,2})</i></span>");
            regex expressionLT2("<i>([0-9]{0,2})</i>"); // 注意转义方式
            
            cmatch what;
            // 如果用 regex_match 方法将需要完全匹配，
            // 不能在字符串中找寻模式，可用于验证输入
            if (!regex_search(strOut.c_str(), what, expressionLT))
            {
                OutputDebugString("第一种未匹配, 换第二种");
                if(!regex_search(strOut.c_str(), what, expressionLT))
                {
                    OutputDebugString("两种均为匹配上");
                    //continue;
                }
            }
            else
            {
                strNum = std::string(what[1]).c_str();
                OutputDebugString(strNum);
            }

        }//End 正则

        if(strNum.IsEmpty())
        {
            AfxMessageBox(TEXT("正则获取Post链接失败，请通知管理员"));
            m_InfoCtl.SetWindowText(TEXT("正则获取Post链接失败，请通知管理员"));
            return;
        }

        //判断是否有5个金币
        if(strNum != TEXT("5"))
        {
            AfxMessageBox(TEXT("账户没有5个金币，请检查是否没有设置资料或已兑换"));
            m_InfoCtl.SetWindowText(TEXT("账户没有5个金币，请检查是否没有设置资料或已兑换"));
            return;
        }

        m_InfoCtl.SetWindowText(TEXT("获取兑换积分页面成功!"));

    }//查看是否有5个金币===============================


    //最后Post用的Url
    CString strPostUrl = TEXT("");
    #define REF_BUY_COIN TEXT("http://mall.csdn.net/cbuy/buy_download_coin")

    //获取兑换积分页面===============================
    {
        
        CString strUrl = TEXT("");
        strUrl.Format(TEXT("http://mall.csdn.net/cbuy/get_c_ajax?time=%d&post_number=5&c_type=download_coin"),
                        m_Assist.GetTimer());

        m_pHttpReq->Open(WEB_GET, (_bstr_t)strUrl);
    
        m_pHttpReq->SetRequestHeader(WEB_HOST, TEXT("mall.csdn.net"));
        m_pHttpReq->SetRequestHeader(WEB_CONNECTION, WEB_CONNECTION_INFO);
        m_pHttpReq->SetRequestHeader(WEB_ACCEPT, TEXT("application/json, text/javascript, */*; q=0.01"));
        m_pHttpReq->SetRequestHeader(TEXT("X-Requested-With"), TEXT("XMLHttpRequest"));
        m_pHttpReq->SetRequestHeader(WEB_UA, WEB_UA_INFO);
        m_pHttpReq->SetRequestHeader(TEXT("Content-Type"), TEXT("application/json; charset=utf-8"));
        m_pHttpReq->SetRequestHeader(WEB_REFERER, REF_BUY_COIN);
        //m_pHttpReq->SetRequestHeader(WEB_ENCODE, WEB_ENCODE_GZIP);
        m_pHttpReq->SetRequestHeader(WEB_LANGUAGE, WEB_LANGUAGE_INFO);

        m_pHttpReq->Send();

        //获取回执信息
        LONG statusCode = m_pHttpReq->GetStatus();
        
        //判断回执标志
        if(statusCode != 200)
        {
            //m_InfoCtl.SetWindowText(strHtm);
            AfxMessageBox(TEXT("网络错误或其他原因"));
            return;
        }

        //取出Body
        string strOut;
        m_Assist.Get_WinHttp_RspString(m_pHttpReq, strOut);
        
        CString strHtm = m_Assist.UTF8ToGBK(strOut).c_str();
        
        //获取32位数据码
        CString strCode = TEXT("");
        //正则取数据
        {
            regex expressionLT("c_type=download_coin&code=([0-9a-zA-Z]{30,35})&post_number");
            regex expressionLT2("([0-9a-zA-Z]{30,35})"); // 注意转义方式
            
            cmatch what;
            // 如果用 regex_match 方法将需要完全匹配，
            // 不能在字符串中找寻模式，可用于验证输入
            if (!regex_search(strOut.c_str(), what, expressionLT))
            {
                OutputDebugString("第一种未匹配, 换第二种");
                if(!regex_search(strOut.c_str(), what, expressionLT))
                {
                    OutputDebugString("两种均为匹配上");
                    //continue;
                }
            }
            else
            {
                strCode = std::string(what[1]).c_str();
                OutputDebugString(strCode);
            }

        }//End 正则

        if(strCode.IsEmpty())
        {
            AfxMessageBox(TEXT("正则获取Post链接失败，请通知管理员"));
            m_InfoCtl.SetWindowText(TEXT("正则获取Post链接失败，请通知管理员"));
            return;
        }

        strPostUrl.Format(TEXT("http://mall.csdn.net/cbuy/submit_cbuy_do?c_type=download_coin&code=%s&post_number=5"),
                            (LPCTSTR)strCode);

        
        m_InfoCtl.SetWindowText(TEXT("获取兑换积分页面成功!"));

    }//获取兑换积分页面===============================

    //获取兑换积分验证码===============================
    {
        
        CString strUrl = TEXT("");
        strUrl.Format(TEXT("http://mall.csdn.net/cbuy/get_v_code?r=%d"),
                        m_Assist.GetTimer());

        m_pHttpReq->Open(WEB_GET, (_bstr_t)strUrl);
    
        m_pHttpReq->SetRequestHeader(WEB_HOST, TEXT("mall.csdn.net"));
        m_pHttpReq->SetRequestHeader(WEB_CONNECTION, WEB_CONNECTION_INFO);
        m_pHttpReq->SetRequestHeader(WEB_ACCEPT, TEXT("image/webp,image/*,*/*;q=0.8"));
        m_pHttpReq->SetRequestHeader(WEB_UA, WEB_UA_INFO);
        m_pHttpReq->SetRequestHeader(WEB_REFERER, REF_BUY_COIN);
        //m_pHttpReq->SetRequestHeader(WEB_ENCODE, WEB_ENCODE_GZIP);
        m_pHttpReq->SetRequestHeader(WEB_LANGUAGE, WEB_LANGUAGE_INFO);

        m_pHttpReq->Send();

        //获取回执信息
        LONG statusCode = m_pHttpReq->GetStatus();
        
        //判断回执标志
        if(statusCode != 200)
        {
            //m_InfoCtl.SetWindowText(strHtm);
            AfxMessageBox(TEXT("网络错误或其他原因"));
            return;
        }

//         //取出Body
//         string strOut;
//         m_Assist.Get_WinHttp_RspString(m_pHttpReq, strOut);
//         
//         CString strHtm = m_Assist.UTF8ToGBK(strOut).c_str();
        
        //获取登录Cookie================================================
        if(!m_Assist.CookieToMap((LPCTSTR)m_strHostCookie, m_Assist.m_CookieMap))
        {
            AfxMessageBox(TEXT("转换Cookie失败"));
            return;
        }

        //获取Cookie
        CString strLoginCookie = (LPCTSTR)m_pHttpReq->GetAllResponseHeaders();
        
        tstring strTmpCookie = strLoginCookie;
        m_Assist.GetCookieString(strTmpCookie, m_Assist.m_CookieMap);
        
        
        strTmpCookie = TEXT("");
        if(!m_Assist.MapToCookie(strTmpCookie, m_Assist.m_CookieMap))
        {
            AfxMessageBox(TEXT("获取登录 Cookie 失败"));
            return;
        }
        
        m_strHostCookie = strTmpCookie.c_str();
        
        m_HtmlCtl.SetWindowText(m_strHostCookie);

        
        //获取验证码数据
        CString strGetCode = TEXT("");
        std::string strOut = strLoginCookie;
        //正则取数据
        {
            regex expressionLT("%22verify_code%22%3Bs%3A4%3A%22([a-zA-Z0-9]{3,6})%22%3B%7D");
            regex expressionLT2("%22([a-zA-Z0-9]{3,6})%22"); // 注意转义方式
            
            cmatch what;
            // 如果用 regex_match 方法将需要完全匹配，
            // 不能在字符串中找寻模式，可用于验证输入
            if (!regex_search(strOut.c_str(), what, expressionLT))
            {
                OutputDebugString("第一种未匹配, 换第二种");
                if(!regex_search(strOut.c_str(), what, expressionLT))
                {
                    OutputDebugString("两种均为匹配上");
                    //continue;
                }
            }
            else
            {
                strGetCode = std::string(what[1]).c_str();
                OutputDebugString(strGetCode);
            }

        }//End 正则

        if(strGetCode.IsEmpty())
        {
            AfxMessageBox(TEXT("正则获取兑换验证码，请通知管理员"));
            m_InfoCtl.SetWindowText(TEXT("正则获取兑换验证码，请通知管理员"));
            return;
        }

        //http://mall.csdn.net/cbuy/submit_cbuy_do?c_type=download_coin&code=cf33d4da8010a4d802e9f326e691beb3&post_number=5&v_code=z755
        strPostUrl = strPostUrl + TEXT("&v_code=") + strGetCode;

        m_InfoCtl.SetWindowText(TEXT("获取积分验证码成功"));

    }//获取兑换积分验证码===============================
    
    //兑换积分===============================
    {
        m_pHttpReq->Open(WEB_GET, (_bstr_t)strPostUrl);

        m_pHttpReq->SetRequestHeader(WEB_HOST, TEXT("mall.csdn.net"));
        m_pHttpReq->SetRequestHeader(WEB_CONNECTION, WEB_CONNECTION_INFO);
        m_pHttpReq->SetRequestHeader(WEB_ACCEPT, WEB_ACCEPT_INFO);
        m_pHttpReq->SetRequestHeader(WEB_UPGRADE, WEB_UPGRADE_INFO);
        m_pHttpReq->SetRequestHeader(WEB_UA, WEB_UA_INFO);
        m_pHttpReq->SetRequestHeader(WEB_REFERER, REF_BUY_COIN);
        //m_pHttpReq->SetRequestHeader(WEB_ENCODE, WEB_ENCODE_GZIP);
        //m_pHttpReq->SetRequestHeader(WEB_LANGUAGE, WEB_LANGUAGE_INFO);

        m_pHttpReq->Send();

        //获取回执信息
        LONG statusCode = m_pHttpReq->GetStatus();
        
        //判断回执标志
        if(statusCode != 500)
        {
            //m_InfoCtl.SetWindowText(strHtm);
            AfxMessageBox(TEXT("网络错误或其他原因"));
            return;
        }

        //取出Body
        string strOut;
        m_Assist.Get_WinHttp_RspString(m_pHttpReq, strOut);
        
        CString strHtm = m_Assist.UTF8ToGBK(strOut).c_str();
        
        //判断是否成功
        if(strHtm.Find(TEXT("兑换成功!")) == -1)
        {
            m_InfoCtl.SetWindowText(strHtm);
            AfxMessageBox(TEXT("兑换金币到下载积分失败!"));
            return;
        }
        
        m_InfoCtl.SetWindowText(TEXT("兑换金币到下载积分成功!"));

    }//兑换积分===============================

    //Request结束
    END_HTTP_REQUEST
}

#include <shlwapi.h>
#pragma comment(lib,"Shlwapi.lib") //如果没有这行，会出现link错误
//判断路径是否存在
BOOL CCSDNDlg::PathIsOk(CString& strFolderPath)
{
    //判断路径是否存在
    if (!PathIsDirectory(strFolderPath) )
    {
        //不存在，则创建一个
        if(!CreateDirectory(strFolderPath, NULL) )
        {
            CString strMsg = TEXT("");
            strMsg.Format(TEXT("创建路径\"%s\"失败！是否继续?"), strFolderPath);
            if (AfxMessageBox(strMsg, MB_YESNO) == IDYES)
                return FALSE;
        }
    }

    return TRUE;
}

BOOL CCSDNDlg::GetNewRequest()
{
    //释放原有
    if(m_pHttpReq != NULL)
        m_pHttpReq.Release();

    //申请新的
    HRESULT hr = m_pHttpReq.CreateInstance(__uuidof(WinHttpRequest));
    if (FAILED(hr)) 
    {
        AfxMessageBox(TEXT("导入Com失败，请联系管理员!"));
        return FALSE;
    }

    m_Assist.SetIgnoreHTTPS(m_pHttpReq);
    m_Assist.SetHttpJmpFlase(m_pHttpReq);
    m_Assist.SetProxy(m_pHttpReq);

    //设置超时
    m_pHttpReq->SetTimeouts(15000, 15000, 15000, 15000);

    return TRUE;
}

void CCSDNDlg::TestBaidu()
{

    try
    {
        //检测对象是否实例
        if(m_pHttpReq == NULL)
        {
            AfxMessageBox(TEXT("连接对象未初始化！"));
            return;
        }
        
        //对象打开网址
        m_pHttpReq->Open(WEB_GET, TEXT("http://passport.csdn.net/account/register?action=registerView&service=http://www.csdn.net"));

        //设置走本地代理
        HRESULT hr = m_Assist.SetProxy(m_pHttpReq);
        if (FAILED(hr))
        {
            AfxMessageBox(TEXT("设置代理失败"));
            return;
        }

        //设置HTTP头
        m_pHttpReq->SetRequestHeader(WEB_HOST, TEXT("passport.csdn.net"));
        m_pHttpReq->SetRequestHeader(WEB_CONNECTION, WEB_CONNECTION_INFO);
        m_pHttpReq->SetRequestHeader(WEB_CACHE_CONTROL, TEXT("max-age=0"));
        m_pHttpReq->SetRequestHeader(WEB_ACCEPT, WEB_ACCEPT_INFO);
        m_pHttpReq->SetRequestHeader(WEB_LANGUAGE, WEB_LANGUAGE_INFO);
        m_pHttpReq->SetRequestHeader(WEB_REFERER, TEXT("http://passport.csdn.net/account/mobileregister?action=mobileRegister"));
        m_pHttpReq->SetRequestHeader(WEB_UA, WEB_UA_INFO);
        

        //Get方法 发送HTTP信息
        hr = m_pHttpReq->Send();
        if (FAILED(hr)) 
            return;

        //获得回执信息的Body
        _bstr_t bstrRep = m_pHttpReq->ResponseText;

        //获取回执信息所有的头信息
        _bstr_t bstrAllHeader = m_pHttpReq->GetAllResponseHeaders();

        //转换数据为CString
        CString strHeader = (LPTSTR)bstrAllHeader;

        //通过Http头获得COOKIES
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
            //打印错误信息
            AfxMessageBox((LPTSTR)e.Description());
        }
        catch (...){}
    } 
}

void CCSDNDlg::OnBtnActive() 
{
	// TODO: Add your control notification handler code here
    if(!GetNewRequest())
    {
        AfxMessageBox("初始化Com失败，请联系管理员");
        return;
    }
    
    //获取用户名================================================
    char szBuf[MAXBYTE] = {0};
    m_RegUserCtl.GetWindowText(szBuf, MAXBYTE);
    ConvertUserFromMail(szBuf);
    
    m_strTmpBuf = szBuf;

    //激活账户================================================
    OnActiveReg(m_strTmpBuf);
    if(!m_bIsUserActive)
    {
        AfxMessageBox(m_strTmpBuf);
        m_InfoCtl.SetWindowText(TEXT("激活失败，请重试，多次重试失败，请联系管理员！"));
    }

}
