// GetCodeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CSDN.h"
#include "GetCodeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGetCodeDlg dialog


CGetCodeDlg::CGetCodeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGetCodeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGetCodeDlg)
	m_strGetCode = _T("");
	//}}AFX_DATA_INIT

    m_pMemData = NULL;
    m_dwDataLen = 0;
    m_strFile = TEXT("");
    m_bIsShowFromFile = FALSE;

    //初始化GDI
    //GdiplusStartup(&m_gdiplusToken, &m_StartupInput, NULL);
    
    m_bOriginalSize = FALSE;
}


void CGetCodeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGetCodeDlg)
	DDX_Control(pDX, IDC_STATIC_GET_CODE, m_PicCodeCtl);
	DDX_Control(pDX, IDC_EDIT_GET_CODE, m_GetCodeCtl);
	DDX_Text(pDX, IDC_EDIT_GET_CODE, m_strGetCode);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGetCodeDlg, CDialog)
	//{{AFX_MSG_MAP(CGetCodeDlg)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGetCodeDlg message handlers

void CGetCodeDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);

    if(m_strGetCode.IsEmpty())
    {
        AfxMessageBox(TEXT("请输入验证码"));
        return;
    }
    else
	    CDialog::OnOK();
}

BOOL CGetCodeDlg::ShowPicInMem(int x, int y)
{
    IStream *pStream = NULL; 
    CFileStatus fstatus;
    CFile file;
    
    //打开文件并检测文件的有效性   
    //有数据，且数据长度不为0
    if (m_pMemData != NULL && m_dwDataLen > 0)     
    {        
        //申请全局空间
        HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, m_dwDataLen);        
        LPVOID pvData = NULL;         
        if (hGlobal != NULL)        
        {            
            //加锁全局空间
            pvData = GlobalLock(hGlobal);            
            if (pvData != NULL)           
            {          
                //MemCpy 操作全局空间
                memcpy(pvData, m_pMemData, m_dwDataLen);    
                
                //解锁全局空间
                GlobalUnlock(hGlobal);           
                
                //通过全局空间创建一个Stream
                CreateStreamOnHGlobal(hGlobal, TRUE, &pStream);     
            }     
        } 
    }    
    else   
    {   
        return FALSE;  
    }
    
    //获得DC
    //CDC* pDC = m_GetCodeCtl.GetDC();
    CWnd* pWnd = GetDlgItem(IDC_STATIC_GET_CODE);
	CDC* pDC = pWnd->GetDC();

    //显示JPEG图片 
    IPicture *pPic = NULL;    
    
    //加载一个Image 通过Stream
    //Stream，数据长度，真， GUID， IPicture
    if(SUCCEEDED(OleLoadPicture(pStream, fstatus.m_size, TRUE,IID_IPicture, (LPVOID*)&pPic)))   
    {    
        if (m_bOriginalSize) //图片原始大小显示
        {
            // 宽高，MM_HIMETRIC 模式，单位是0.01毫米 
            long nWidth = 0, nHeight = 0; 

            // 宽
            pPic->get_Width( &nWidth );  
            // 高
            pPic->get_Height( &nHeight );  
            
            CSize sz( nWidth, nHeight ); 

            // 转换 MM_HIMETRIC 模式单位为 MM_TEXT 像素单位 
            pDC->HIMETRICtoDP( &sz ); 

            //画
            if(FAILED(pPic->Render(pDC->m_hDC, 0, 0, sz.cx, sz.cy, 
                        0, nHeight, nWidth, -nHeight, NULL)))  
            {
                pPic->Release();  
                return FALSE;        
            }
        }   
        else
        {
            OLE_XSIZE_HIMETRIC hmWidth;      
            OLE_YSIZE_HIMETRIC hmHeight;    
            pPic->get_Width(&hmWidth);
            pPic->get_Height(&hmHeight);   
            
            //获取图片的宽和高  
            double fX,fY;
            fX = (double)pDC->GetDeviceCaps(HORZRES)  * (double)hmWidth /
                ((double)pDC->GetDeviceCaps(HORZSIZE) * 100.0); 
            fY = (double)pDC->GetDeviceCaps(VERTRES)  * (double)hmHeight /
                ((double)pDC->GetDeviceCaps(VERTSIZE) * 100.0);  
            
            //按图片原始大小显示
            if(FAILED(pPic->Render(*pDC,        //DC
                                    0,          //X      显示位置X
                                    0,          //Y      显示位置
                                    (DWORD)fX,  //Width  目标宽
                                    (DWORD)fY,  //Height 目标高
                                    0,          //水平偏移量开始复制的源图像中。
                                    hmHeight,   //源图片的垂直偏移量开始复制。
                                    hmWidth,    //水平程度从源复制图片。
                                    -hmHeight,  //垂直程度从源复制图片。
                                    NULL)       //
                                    ))         
            {
                pPic->Release();  
                return FALSE;        
            }
        }       
        
        if ( pStream ) pStream->Release(); // 释放 IStream 指针
        if ( pPic ) pPic->Release(); // 释放 IPicture 指针  
    }   
    else    
    {      
        return FALSE;   
    }    
    
    ReleaseDC(pDC);
    return TRUE;
}

//验证码默认显示在0, 0的位置
BOOL CGetCodeDlg::ShowPicInFile(int x, int y)
{
    IStream *pStream = NULL; 
    CFileStatus fstatus;
    CFile file;
    
    //打开文件并检测文件的有效性   
    //CFile打开文件
    //获取文件信息 file.GetStatus(strPath,fstatus)
    //获取文件大小 cb = fstatus.m_size
    if (file.Open(m_strFile, CFile::modeRead) && 
        file.GetStatus(m_strFile,fstatus) &&
        ((m_dwDataLen = fstatus.m_size) != -1))     
    {        
        //申请全局空间
        HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, m_dwDataLen);        
        LPVOID pvData = NULL;         
        if (hGlobal != NULL)        
        {            
            //加锁全局空间
            pvData = GlobalLock(hGlobal);            
            if (pvData != NULL)           
            {          
                //MemCpy 操作全局空间
                file.ReadHuge(pvData, m_dwDataLen);      
                
                //解锁全局空间
                GlobalUnlock(hGlobal);           
                
                //通过全局空间创建一个Stream
                CreateStreamOnHGlobal(hGlobal, TRUE, &pStream);     
            }     
        } 
    }    
    else   
    {   
        try
        {
            file.Close();
        }
        catch (CFileException &e)
        {
            TCHAR   szCause[MAXBYTE] = {0};
            e.GetErrorMessage(szCause, MAXBYTE);
        	AfxMessageBox(szCause);
        }
        
        
        return FALSE;  
    }
    //打开文件结束  
    try
    {
        file.Close();
    }
    catch (CFileException &e)
    {
        TCHAR   szCause[MAXBYTE] = {0};
        e.GetErrorMessage(szCause, MAXBYTE);
        AfxMessageBox(szCause);
    }
    
    //获得DC
    //CDC* pDC = m_GetCodeCtl.GetDC();
    CWnd* pWnd = GetDlgItem(IDC_STATIC_GET_CODE);
	CDC* pDC = pWnd->GetDC();

    //显示JPEG图片 
    IPicture *pPic = NULL;    
    
    //加载一个Image 通过Stream
    //Stream，数据长度，真， GUID， IPicture
    if(SUCCEEDED(OleLoadPicture(pStream, fstatus.m_size, TRUE,IID_IPicture, (LPVOID*)&pPic)))   
    {    
        if (m_bOriginalSize) //图片原始大小显示
        {
            // 宽高，MM_HIMETRIC 模式，单位是0.01毫米 
            long nWidth = 0, nHeight = 0; 

            // 宽
            pPic->get_Width( &nWidth );  
            // 高
            pPic->get_Height( &nHeight );  
            
            CSize sz( nWidth, nHeight ); 

            // 转换 MM_HIMETRIC 模式单位为 MM_TEXT 像素单位 
            pDC->HIMETRICtoDP( &sz ); 

            //画
            if(FAILED(pPic->Render(pDC->m_hDC, 0, 0, sz.cx, sz.cy, 
                        0, nHeight, nWidth, -nHeight, NULL)))  
            {
                pPic->Release();  
                return FALSE;        
            }
        }   
        else
        {
            OLE_XSIZE_HIMETRIC hmWidth;      
            OLE_YSIZE_HIMETRIC hmHeight;    
            pPic->get_Width(&hmWidth);
            pPic->get_Height(&hmHeight);   
            
            //获取图片的宽和高  
            double fX,fY;
            fX = (double)pDC->GetDeviceCaps(HORZRES)  * (double)hmWidth /
                ((double)pDC->GetDeviceCaps(HORZSIZE) * 100.0); 
            fY = (double)pDC->GetDeviceCaps(VERTRES)  * (double)hmHeight /
                ((double)pDC->GetDeviceCaps(VERTSIZE) * 100.0);  
            
            //按图片原始大小显示
            if(FAILED(pPic->Render(*pDC,        //DC
                                    0,          //X      显示位置X
                                    0,          //Y      显示位置
                                    (DWORD)fX,  //Width  目标宽
                                    (DWORD)fY,  //Height 目标高
                                    0,          //水平偏移量开始复制的源图像中。
                                    hmHeight,   //源图片的垂直偏移量开始复制。
                                    hmWidth,    //水平程度从源复制图片。
                                    -hmHeight,  //垂直程度从源复制图片。
                                    NULL)       //
                                    ))         
            {
                pPic->Release();  
                return FALSE;        
            }
        }       
        
        if ( pStream ) pStream->Release(); // 释放 IStream 指针
        if ( pPic ) pPic->Release(); // 释放 IPicture 指针  
    }   
    else    
    {      
        return FALSE;   
    }    
    
    ReleaseDC(pDC);
    return TRUE;
}

BOOL CGetCodeDlg::SetPicData(LPVOID pSrc, DWORD dwLen, BOOL OriginalSize)
{
    m_pMemData = (PCHAR)pSrc;
    m_dwDataLen = dwLen;
    
    m_bOriginalSize = OriginalSize;
    m_bIsShowFromFile = FALSE;
    return TRUE;
}

BOOL CGetCodeDlg::SetPicData(IWinHttpRequestPtr& pObj, BOOL OriginalSize)
{
    //获取Request头
    _variant_t varRspBody = pObj->GetResponseBody();

    //得到数据长度和数据
    m_dwDataLen = varRspBody.parray->rgsabound[0].cElements;
    m_pMemData = (char *)varRspBody.parray->pvData;

    m_bOriginalSize = OriginalSize;
    m_bIsShowFromFile = FALSE;
    return TRUE;
}

BOOL CGetCodeDlg::SetPicData(CString strFileName, BOOL OriginalSize)
{
    m_strFile = strFileName;

    m_bOriginalSize = OriginalSize;
    m_bIsShowFromFile = TRUE;
    return TRUE;
}

BOOL CGetCodeDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
    
    m_strGetCode = TEXT("");
    UpdateData(FALSE);
    
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CGetCodeDlg::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
    //Gdiplus::GdiplusShutdown(m_gdiplusToken);  
    //return __super::ExitInstance();  
	return CDialog::DestroyWindow();
}

void CGetCodeDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
    //显示图片
    if(m_bIsShowFromFile)
    {
        if(!ShowPicInFile())
            AfxMessageBox(TEXT("显示图片错误!"));
    }
    else
    {
        if(!ShowPicInMem())
            AfxMessageBox(TEXT("显示图片错误!"));
    }
    // Do not call CDialog::OnPaint() for painting messages
}

CString CGetCodeDlg::GetInputCode() CONST
{
    return m_strGetCode;
}
