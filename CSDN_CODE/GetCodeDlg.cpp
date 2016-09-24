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

    //��ʼ��GDI
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
        AfxMessageBox(TEXT("��������֤��"));
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
    
    //���ļ�������ļ�����Ч��   
    //�����ݣ������ݳ��Ȳ�Ϊ0
    if (m_pMemData != NULL && m_dwDataLen > 0)     
    {        
        //����ȫ�ֿռ�
        HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, m_dwDataLen);        
        LPVOID pvData = NULL;         
        if (hGlobal != NULL)        
        {            
            //����ȫ�ֿռ�
            pvData = GlobalLock(hGlobal);            
            if (pvData != NULL)           
            {          
                //MemCpy ����ȫ�ֿռ�
                memcpy(pvData, m_pMemData, m_dwDataLen);    
                
                //����ȫ�ֿռ�
                GlobalUnlock(hGlobal);           
                
                //ͨ��ȫ�ֿռ䴴��һ��Stream
                CreateStreamOnHGlobal(hGlobal, TRUE, &pStream);     
            }     
        } 
    }    
    else   
    {   
        return FALSE;  
    }
    
    //���DC
    //CDC* pDC = m_GetCodeCtl.GetDC();
    CWnd* pWnd = GetDlgItem(IDC_STATIC_GET_CODE);
	CDC* pDC = pWnd->GetDC();

    //��ʾJPEGͼƬ 
    IPicture *pPic = NULL;    
    
    //����һ��Image ͨ��Stream
    //Stream�����ݳ��ȣ��棬 GUID�� IPicture
    if(SUCCEEDED(OleLoadPicture(pStream, fstatus.m_size, TRUE,IID_IPicture, (LPVOID*)&pPic)))   
    {    
        if (m_bOriginalSize) //ͼƬԭʼ��С��ʾ
        {
            // ��ߣ�MM_HIMETRIC ģʽ����λ��0.01���� 
            long nWidth = 0, nHeight = 0; 

            // ��
            pPic->get_Width( &nWidth );  
            // ��
            pPic->get_Height( &nHeight );  
            
            CSize sz( nWidth, nHeight ); 

            // ת�� MM_HIMETRIC ģʽ��λΪ MM_TEXT ���ص�λ 
            pDC->HIMETRICtoDP( &sz ); 

            //��
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
            
            //��ȡͼƬ�Ŀ�͸�  
            double fX,fY;
            fX = (double)pDC->GetDeviceCaps(HORZRES)  * (double)hmWidth /
                ((double)pDC->GetDeviceCaps(HORZSIZE) * 100.0); 
            fY = (double)pDC->GetDeviceCaps(VERTRES)  * (double)hmHeight /
                ((double)pDC->GetDeviceCaps(VERTSIZE) * 100.0);  
            
            //��ͼƬԭʼ��С��ʾ
            if(FAILED(pPic->Render(*pDC,        //DC
                                    0,          //X      ��ʾλ��X
                                    0,          //Y      ��ʾλ��
                                    (DWORD)fX,  //Width  Ŀ���
                                    (DWORD)fY,  //Height Ŀ���
                                    0,          //ˮƽƫ������ʼ���Ƶ�Դͼ���С�
                                    hmHeight,   //ԴͼƬ�Ĵ�ֱƫ������ʼ���ơ�
                                    hmWidth,    //ˮƽ�̶ȴ�Դ����ͼƬ��
                                    -hmHeight,  //��ֱ�̶ȴ�Դ����ͼƬ��
                                    NULL)       //
                                    ))         
            {
                pPic->Release();  
                return FALSE;        
            }
        }       
        
        if ( pStream ) pStream->Release(); // �ͷ� IStream ָ��
        if ( pPic ) pPic->Release(); // �ͷ� IPicture ָ��  
    }   
    else    
    {      
        return FALSE;   
    }    
    
    ReleaseDC(pDC);
    return TRUE;
}

//��֤��Ĭ����ʾ��0, 0��λ��
BOOL CGetCodeDlg::ShowPicInFile(int x, int y)
{
    IStream *pStream = NULL; 
    CFileStatus fstatus;
    CFile file;
    
    //���ļ�������ļ�����Ч��   
    //CFile���ļ�
    //��ȡ�ļ���Ϣ file.GetStatus(strPath,fstatus)
    //��ȡ�ļ���С cb = fstatus.m_size
    if (file.Open(m_strFile, CFile::modeRead) && 
        file.GetStatus(m_strFile,fstatus) &&
        ((m_dwDataLen = fstatus.m_size) != -1))     
    {        
        //����ȫ�ֿռ�
        HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, m_dwDataLen);        
        LPVOID pvData = NULL;         
        if (hGlobal != NULL)        
        {            
            //����ȫ�ֿռ�
            pvData = GlobalLock(hGlobal);            
            if (pvData != NULL)           
            {          
                //MemCpy ����ȫ�ֿռ�
                file.ReadHuge(pvData, m_dwDataLen);      
                
                //����ȫ�ֿռ�
                GlobalUnlock(hGlobal);           
                
                //ͨ��ȫ�ֿռ䴴��һ��Stream
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
    //���ļ�����  
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
    
    //���DC
    //CDC* pDC = m_GetCodeCtl.GetDC();
    CWnd* pWnd = GetDlgItem(IDC_STATIC_GET_CODE);
	CDC* pDC = pWnd->GetDC();

    //��ʾJPEGͼƬ 
    IPicture *pPic = NULL;    
    
    //����һ��Image ͨ��Stream
    //Stream�����ݳ��ȣ��棬 GUID�� IPicture
    if(SUCCEEDED(OleLoadPicture(pStream, fstatus.m_size, TRUE,IID_IPicture, (LPVOID*)&pPic)))   
    {    
        if (m_bOriginalSize) //ͼƬԭʼ��С��ʾ
        {
            // ��ߣ�MM_HIMETRIC ģʽ����λ��0.01���� 
            long nWidth = 0, nHeight = 0; 

            // ��
            pPic->get_Width( &nWidth );  
            // ��
            pPic->get_Height( &nHeight );  
            
            CSize sz( nWidth, nHeight ); 

            // ת�� MM_HIMETRIC ģʽ��λΪ MM_TEXT ���ص�λ 
            pDC->HIMETRICtoDP( &sz ); 

            //��
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
            
            //��ȡͼƬ�Ŀ�͸�  
            double fX,fY;
            fX = (double)pDC->GetDeviceCaps(HORZRES)  * (double)hmWidth /
                ((double)pDC->GetDeviceCaps(HORZSIZE) * 100.0); 
            fY = (double)pDC->GetDeviceCaps(VERTRES)  * (double)hmHeight /
                ((double)pDC->GetDeviceCaps(VERTSIZE) * 100.0);  
            
            //��ͼƬԭʼ��С��ʾ
            if(FAILED(pPic->Render(*pDC,        //DC
                                    0,          //X      ��ʾλ��X
                                    0,          //Y      ��ʾλ��
                                    (DWORD)fX,  //Width  Ŀ���
                                    (DWORD)fY,  //Height Ŀ���
                                    0,          //ˮƽƫ������ʼ���Ƶ�Դͼ���С�
                                    hmHeight,   //ԴͼƬ�Ĵ�ֱƫ������ʼ���ơ�
                                    hmWidth,    //ˮƽ�̶ȴ�Դ����ͼƬ��
                                    -hmHeight,  //��ֱ�̶ȴ�Դ����ͼƬ��
                                    NULL)       //
                                    ))         
            {
                pPic->Release();  
                return FALSE;        
            }
        }       
        
        if ( pStream ) pStream->Release(); // �ͷ� IStream ָ��
        if ( pPic ) pPic->Release(); // �ͷ� IPicture ָ��  
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
    //��ȡRequestͷ
    _variant_t varRspBody = pObj->GetResponseBody();

    //�õ����ݳ��Ⱥ�����
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
    //��ʾͼƬ
    if(m_bIsShowFromFile)
    {
        if(!ShowPicInFile())
            AfxMessageBox(TEXT("��ʾͼƬ����!"));
    }
    else
    {
        if(!ShowPicInMem())
            AfxMessageBox(TEXT("��ʾͼƬ����!"));
    }
    // Do not call CDialog::OnPaint() for painting messages
}

CString CGetCodeDlg::GetInputCode() CONST
{
    return m_strGetCode;
}
