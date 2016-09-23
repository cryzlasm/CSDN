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
    bIsShowFromFile = FALSE;
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
	
	//CDialog::OnOK();
}

BOOL CGetCodeDlg::ShowPicInMem()
{
    CRect rect;//��ͼƬ��ʾ��Picture�ؼ���ʱ��ȡ�ؼ�������λ��
    m_GetCodeCtl.GetWindowRect(&rect);//
    ScreenToClient(&rect);//��ȡ����Ļ����ת��Ϊ�Ի����ڲ����������

    //����ȫ�ֿռ�
    HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, m_dwDataLen);
    if(hGlobal == NULL)
        return FALSE;

    //��������pDataָ��
    void *pData = GlobalLock(hGlobal);
    if(pData == NULL)
        return FALSE;
    
    //����ͼƬ���ݵ�pData
    memcpy(pData, m_pMemData, m_dwDataLen);
    
    //����
    GlobalUnlock(hGlobal);
    //if(!GlobalUnlock(hGlobal))
    //    return FALSE;
    

    IStream *pStream = NULL;
    
    //��ȫ�ֿռ� ����Stream
    if(CreateStreamOnHGlobal(hGlobal, TRUE, &pStream) != S_OK) 
        return FALSE;

    //����DC
    CDC* pDc = m_GetCodeCtl.GetWindowDC();
    Graphics graphics(pDc->m_hDC);

    //����ͼƬ
    //Image image(L"code.jpg", FALSE);//ͼƬȫ·��+ͼƬ��ʽ
    Image imageCode(pStream);
    
    //��ͼ
    if(graphics.DrawImage(&imageCode, 0, 0, rect.Width(), rect.Height()) != Ok)
        return FALSE;

    //�ͷ�Istream
    pStream->Release();
    
    //�ͷ������ȫ�ֿռ�
    GlobalFree(hGlobal);

    //�ͷ�DC
    ReleaseDC(pDc);

    return TRUE;
}

BOOL CGetCodeDlg::ShowPicInFile()
{
    //����DC
    CDC* pDc = m_GetCodeCtl.GetWindowDC();

    
    Graphics graphics(pDc->GetSafeHdc());

    Image image((_bstr_t)m_strFile);

    if(graphics.DrawImage(&image, 10, 10) != Ok)
        return FALSE;    

    ReleaseDC(pDc);
    return TRUE;
}

BOOL CGetCodeDlg::SetPicData(LPVOID pSrc, DWORD dwLen)
{
    m_pMemData = (PCHAR)pSrc;
    m_dwDataLen = dwLen;

    bIsShowFromFile = FALSE;
    return TRUE;
}

BOOL CGetCodeDlg::SetPicData(IWinHttpRequestPtr& pObj)
{
    _variant_t varRspBody = pObj->GetResponseBody();
    m_dwDataLen = varRspBody.parray->rgsabound[0].cElements;
    char *m_pData = (char *)varRspBody.parray->pvData;

    bIsShowFromFile = FALSE;
    return TRUE;
}

BOOL CGetCodeDlg::SetPicData(CString strFileName)
{
    m_strFile = strFileName;
    bIsShowFromFile = TRUE;
    return TRUE;
}
// void CGetCodeDlg::ShowcodePic()
// {
//     CImage img;
//     //HBITMAP bitMap; 
//     //CBitmap BitMap;
//     BOOL bRet = LoadMemImage(m_pData, m_dwDataLen, img);
//     CStatic *pStaticPic = (CStatic *)GetDlgItem(IDC_PIC);
//     if (bRet && pStaticPic)
//     {
//         HBITMAP retBitmap = m_PicCodeCtl.SetBitmap();->SetBitmap(img.Detach());
//     }
// }
// 
// BOOL CGetCodeDlg::LoadMemImage(void *pMemData, ULONG nLen, CImage& imgObj)
// {
//     BOOL bRet = FALSE;
// 
//     //����ȫ�ֿռ�
//     HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, nLen);
//     
//     //��������pDataָ��
//     void *pData = GlobalLock(hGlobal);
//     
//     //����ͼƬ���ݵ�pData
//     memcpy(pData, pMemData, nLen);
//     
//     //����
//     GlobalUnlock(hGlobal);
//     
// 
//     IStream *pStream = NULL;
//     
//     //��ȫ�ֿռ� ����Stream
//     if(CreateStreamOnHGlobal(hGlobal, TRUE, &pStream) == S_OK) 
//     {
//         //����
//         if (SUCCEEDED(imgObj.Load(pStream))) 
//             bRet = TRUE;
//         
//         //�ͷ�Istream
//         pStream->Release();
//     }
// 
//     //�ͷ������ȫ�ֿռ�
//     GlobalFree(hGlobal);
//     
//     return bRet;
// } 

BOOL CGetCodeDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	//��ʼ��GDI
    //GdiplusStartup(&m_gdiplusToken, &m_StartupInput, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CGetCodeDlg::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
    Gdiplus::GdiplusShutdown(m_gdiplusToken);  
    //return __super::ExitInstance();  
	return CDialog::DestroyWindow();
}

void CGetCodeDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
    //��ʾͼƬ
    if(bIsShowFromFile)
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
