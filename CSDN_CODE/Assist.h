// Assist.h: interface for the CAssist class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ASSIST_H__890A911F_8712_49E4_8A06_7181638C00E2__INCLUDED_)
#define AFX_ASSIST_H__890A911F_8712_49E4_8A06_7181638C00E2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//	��Ա���������Ԥ������Ϊ
#if _MSC_VER < 1400
//	���ò��־��棨VC6.0���⣩
#pragma warning(disable: 4068)	//	VC6.0�Ա�׼ģ���֧�ֲ�������������ִ���
#pragma warning(disable: 4503) 
#pragma warning(disable: 4786) 
#else
//	���ò��־��棨VC2008���⣩
#pragma warning(disable: 4819)	//	��OpenCV�����ַ�����
#pragma warning(disable: 4996)	//	������ȫ��������
#endif

#include <boost\regex.hpp>

#include <map>
#include <string>


#define INLINE  inline

//����WinHttp
#import "C:\\Windows\\System32\\WinHttp.dll" no_namespace

#ifdef _UNICODE
#define tstring std::wstring
#else
#define tstring std::string
#endif

#ifndef MAP
#define MAP std::map
#endif


//============================================================
#ifndef __ZSTREAM_H__
#define __ZSTREAM_H__

//#define ZLIB_WINAPI
#include "zlib.h"
#include "zconf.h"
#pragma comment(lib, "zdll.lib")



// #ifdef _DEBUG
// #pragma comment(lib, "gzip/zlib/lib/zlibstat_d.lib")
// #else
// #pragma comment(lib, "gzip/zlib/lib/zlibstat_r.lib")
// #endif //#ifdef _DEBUG

#endif //#ifndef __ZSTREAM_H__
//============================================================

class CAssist  
{
public:
    CAssist(){srand((unsigned)time( NULL ));}
    virtual ~CAssist(){}

    //�ַ�����ʮ������
    INLINE BOOL StrToNum(tstring strNum, LONG& lRet);

    //����HTTPS������
    INLINE HRESULT SetIgnoreHTTPS(IWinHttpRequestPtr& pObj);

    //���ý���302��ת
    INLINE HRESULT SetHttpJmpFlase(IWinHttpRequestPtr& pObj);

    //���÷��ʴ���(������)
    INLINE HRESULT SetProxy(IWinHttpRequestPtr& pObj, _bstr_t bstrProxy = TEXT("127.0.0.1:8888"));
    

    //����ת��
    INLINE std::string GBKToUTF8(const std::string& strGBK);
    INLINE std::string UTF8ToGBK(const std::string& strUTF8);

    //�����ȡGet�����ַ���������
    INLINE BOOL Get_WinHttp_RspString(IWinHttpRequestPtr& pObj, std::string& strDst);

    //��ȡָ���ַ�����ǰ���־�м������, �����������޳��ؼ���
    //Դ���ݣ����־���ֱ�־���Ƿ������߱�־���ȣ��Ƿ�����ұ߱�־����
    INLINE std::string GetMidStrByLAndR(const std::string& strSrc, std::string strLeft, std::string strRight, BOOL bIncludeStart = FALSE, BOOL bIncludeEnd = FALSE);
    
    //���COOKIES
    INLINE tstring GetCookieString(tstring strRspHeader, MAP<tstring, tstring>& CookieMap, BOOL bAllGet = FALSE);

    //���ʱ���
    INLINE LONG GetTimer();

    //��ȡһ�������ĸ
    INLINE DWORD GetRand();

    //ת��Cookie�ַ���
    //��ϣ���ַ���
    INLINE BOOL MapToCookie(tstring& strDst, MAP<tstring, tstring>& CookieMap);
    
    //�ַ�������ϣ��
    INLINE BOOL CookieToMap(tstring strCookie, MAP<tstring, tstring>& CookieMap);

    //���ڴ��Cookie��Map
    MAP<tstring, tstring> m_CookieMap;


    //Zlib Gzip
    INLINE BOOL httpgzdecompress(BYTE *zdata, DWORD nzdata,                 
                                 BYTE *data, DWORD *ndata);
    INLINE BOOL UnGZip(BYTE *zdata, DWORD nzdata, std::string& outstr);

};

INLINE BOOL CAssist::httpgzdecompress(BYTE *zdata, DWORD nzdata,                 
                                  BYTE *data, DWORD *ndata)
{
    int err = 0;
    z_stream d_stream = {0}; /* decompression stream */
    static char dummy_head[2] = 
    {
        0x8 + 0x7 * 0x10,
            (((0x8 + 0x7 * 0x10) * 0x100 + 30) / 31 * 31) & 0xFF,
    };
    d_stream.zalloc = (alloc_func)0;
    d_stream.zfree = (free_func)0;
    d_stream.opaque = (voidpf)0;
    d_stream.next_in  = zdata;
    d_stream.avail_in = 0;
    d_stream.next_out = data;
    if(inflateInit2(&d_stream, 47) != Z_OK) 
        return FALSE;

    while (d_stream.total_out < *ndata && d_stream.total_in < nzdata) 
    {
        d_stream.avail_in = d_stream.avail_out = 1; /* force small buffers */
        if((err = inflate(&d_stream, Z_NO_FLUSH)) == Z_STREAM_END) 
            break;

        if(err != Z_OK )
        {
            if(err == Z_DATA_ERROR)
            {
                d_stream.next_in = (Bytef*) dummy_head;
                d_stream.avail_in = sizeof(dummy_head);
                if((err = inflate(&d_stream, Z_NO_FLUSH)) != Z_OK) 
                {
                    return FALSE;
                }
            }
            else return FALSE;
        }
    }
    if(inflateEnd(&d_stream) != Z_OK) 
        return FALSE;
    *ndata = d_stream.total_out;
    return TRUE;
}

INLINE BOOL CAssist::UnGZip(BYTE *zdata, DWORD nzdata, std::string& outstr)
{
    int nTimes = 4;
    uLong uReqLen = nzdata, realLen = 0;
    int ret = -1;
    char *pDstBuffer = NULL;
    do 
    {
        uReqLen *= nTimes;
        if (pDstBuffer) 
            delete [] pDstBuffer;

        pDstBuffer = new char[uReqLen];
        if (!pDstBuffer)
        {
            ret = FALSE;
            break;
        }
        ZeroMemory(pDstBuffer, uReqLen);
        realLen = uReqLen;
        
        ret = httpgzdecompress(zdata, nzdata, (BYTE*)pDstBuffer, &realLen);
        nTimes += 2;
        
    } while (ret >= 0 && realLen >= uReqLen);
    
    if (ret < 0 || realLen <= 0)
    {
        if (pDstBuffer) delete [] pDstBuffer;
        return FALSE;
    }
    
    pDstBuffer[realLen] = 0;
    outstr = pDstBuffer;
    return TRUE;
}

//���Cookie�����ص�Header���Body
INLINE tstring CAssist::GetCookieString(tstring strRspHeader, MAP<tstring, tstring>& CookieMap, BOOL bAllGet)
{
    //����Cookie
    tstring retCookie;
    
    //Cookie��־
    tstring strKey = _T("Set-Cookie: ");
    
    int iBegin = 0, iEnd = 0;
    while(iBegin < strRspHeader.length())
    {
        //�õ�Cookie��λ��
        iBegin = strRspHeader.find(strKey, iBegin);
        if (iBegin >= 0)
        {
            //Cookie��β��Ϊ\r\n
            iEnd = strRspHeader.find(_T("\r\n"), iBegin);
            if (iEnd > 0)
            {
                //��ȡCookie���ڷ���
                //�ж��Ƿ���Ҫԭ����ȡ�ַ���
                if(!bAllGet)
                {
                    //��ȡһ��Cookie
                    tstring strTmp = strRspHeader.substr(iBegin, iEnd-iBegin+2);
                    tstring strDst = TEXT("");
                    
                    //��ȡSetCookie��Ϣ  ��ͷ �� �ֺ�
                    strDst = GetMidStrByLAndR(strTmp, strKey, TEXT(";"), FALSE, TRUE);

                    CookieToMap(strDst, CookieMap);
                    //retCookie += strDst + TEXT(" ");
                }
                else
                    retCookie += strRspHeader.substr(iBegin, iEnd-iBegin+2);
                
                iBegin = iEnd;
            }
        }
        else
        {
            break;
        }
    }
    
    return retCookie;
}

//Cookie String �� Map
INLINE BOOL CAssist::CookieToMap(tstring strCookie, MAP<tstring, tstring>& CookieMap)
{
    tstring strTmp = TEXT("");
    tstring strHead = TEXT("");
    tstring strBody = TEXT("");
    
    while(TRUE)
    {
        //�ҵ��ֺŵ�һ�γ��ֵ�λ��
        int nEnd = strCookie.find_first_of(TEXT(";"));
        
        //δ�ҵ� �˳�
        if(nEnd == tstring::npos)
            break;
        
        //���һ��Cookie
        strTmp = strCookie.substr(0, nEnd+1);
        
        //��ֵ���Cookie
        int nSplit = strTmp.find(TEXT("="));
        
        //�õ��Ⱥ�ǰ�ߵ��ֶΣ�
        strHead = strTmp.substr(0, nSplit);
        
        //�õ��Ǻź�ߵ��ֶ�
        strBody = strTmp.substr(nSplit + 1, strTmp.length() - nSplit - 2);
        
        //���ӹ�ϣ��
        CookieMap[strHead] = strBody;
        
        //ȥ���Ѿ�������ַ���
        strCookie = strCookie.substr(nEnd + 1, strCookie.length() - nEnd - 1);
        
        //ɾ��ǰ�ÿո�
        strCookie.erase(0, strCookie.find_first_not_of(TEXT(" ")));
    }
    
    return TRUE;
}

//cookie Map �� String
INLINE BOOL CAssist::MapToCookie(tstring& strDst, MAP<tstring, tstring>& CookieMap)
{
    //�ж��Ƿ��һ�ν��룬��һ�ν���ͷǰ���ӿո񣬷ǵ�һ����ӿո����
    static BOOL bIsFirstInfo = TRUE;
    char* pszTmp = TEXT("");
    
    MAP<tstring, tstring>::iterator  iter;
    for(iter = CookieMap.begin(); iter != CookieMap.end(); iter++)
    {
        //�ǵ�һ�ν��������ո�
        if(!bIsFirstInfo)
        {
            pszTmp = TEXT(" ");
        }
        
        //ƴ�ַ���      LocalPart=csdntest;
        strDst = strDst + pszTmp + iter->first + "=" + iter->second + ";";
        
        //��һ�ν���֮������
        bIsFirstInfo = FALSE;
    }

    strDst = strDst.substr(0, strDst.find_last_of(TEXT(";")));
    
    CookieMap.clear();
    return TRUE;
}

//�ַ��� ת  ����
INLINE BOOL CAssist::StrToNum(tstring strNum, LONG& lRet)
{
    #define HEX 16
    
    LPTSTR pEnd = NULL;
    LPTSTR pStart = &strNum[0];
    
    //�õ����ݰ���DeCode Len
    lRet = _tcstol(strNum.c_str(), &pEnd, HEX);
    
    if(pEnd - pStart != strNum.length())
    {
        return FALSE;
    }

    return TRUE;
}

//��ȡ0~32�������
INLINE DWORD CAssist::GetRand()
{
    
    //�õ�һ�������Сд��ĸ
    DWORD dwRand = rand() % 26;

    //�����Сд
    DWORD dwLower = rand() % 32;

    DWORD dwRet = 0;

    //�õ�����0 �� 1
    if(dwLower % 2 == 0)
    {
        dwRet = dwRand + TEXT('a');
    }
    else    //dwLower % 2 == 1
    {
        dwRet = dwRand + TEXT('A');
    }

    return dwRet;
}

//��ȡʱ���
INLINE LONG CAssist::GetTimer()
{
    //��ȡʱ���
    return time(0);
}

//��ȡBody�ַ���
INLINE BOOL CAssist::Get_WinHttp_RspString(IWinHttpRequestPtr& pObj, std::string& strDst)
{				
    _variant_t varRspBody = pObj->GetResponseBody();		
    ULONG dataLen = varRspBody.parray->rgsabound[0].cElements;				
    char *pContentBuffer = (char *)varRspBody.parray->pvData;				
    strDst = pContentBuffer;
    return TRUE;
}

//�õ���־�м���ַ���
INLINE std::string CAssist::GetMidStrByLAndR(const std::string& strSrc, std::string strLeft, std::string strRight, BOOL bIncludeStart, BOOL bIncludeEnd)
{
    std::string strRet = TEXT("");

    //��ȡ���־�ĵ�һ��λ��
    int eIdxBegin = strSrc.find(strLeft);

    if(eIdxBegin != -1)
    {
        //����ֵ�Ƿ����ǰ��־
        if (!bIncludeStart) 
        {
            eIdxBegin += strLeft.length();
        }
        
        int eIdxEnd = strSrc.find(strRight, eIdxBegin + 1);
        if (eIdxEnd != -1)
        {
            //����ֵ�Ƿ�������־
            if (!bIncludeEnd) 
            {
                strRet = strSrc.substr(eIdxBegin, eIdxEnd - eIdxBegin);
            }
            else
            {
                eIdxEnd += strRight.length();
                strRet = strSrc.substr(eIdxBegin, eIdxEnd - eIdxBegin);
            }
        }
    }
    
    return strRet;
}

//ת������ UTF8 �� GBK
INLINE std::string CAssist::UTF8ToGBK(const std::string& strUTF8)
{
    //�õ�����ת��֮��ĳ���
    int len = MultiByteToWideChar(CP_UTF8, 0, strUTF8.c_str(), -1, NULL, 0);
    
    //���뻺����
    wchar_t *wszGBK = new wchar_t[len + 1];
    memset(wszGBK, 0, (len + 1)*sizeof(wchar_t));
    
    //ת������  ���ֽڵ����ֽ�
    MultiByteToWideChar(CP_UTF8, 0, strUTF8.c_str(), -1, wszGBK, len);
    
    //�õ�����ת��֮��ĳ���
    len = WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, NULL, 0, NULL, NULL);
    
    //���뻺����
    char *szGBK = new char[len + 1];
    memset(szGBK, 0, len + 1);
    
    //ת�����룬 ���ֽڵ����ֽ�
    WideCharToMultiByte(CP_ACP,0, wszGBK, -1, szGBK, len, NULL, NULL);
    //strUTF8 = szGBK;
    std::string strTemp(szGBK);
    
    //�ͷŻ�����
    delete[]szGBK;
    delete[]wszGBK;
    
    return strTemp;
}

//ת������ GBK �� UTF8
INLINE std::string CAssist::GBKToUTF8(const std::string& strGBK)
{
    //�����ִ������
    std::string strOutUTF8 = "";
    
    //����ת�������õ�ת����Ĵ�С
    int n = MultiByteToWideChar(CP_ACP, 0, strGBK.c_str(), -1, NULL, 0);
    
    //���뻺����
    WCHAR* str1 = new WCHAR[n];
    ZeroMemory(str1, n*sizeof(WCHAR));
    
    //ת������  ���ֽڵ����ֽ�
    MultiByteToWideChar(CP_ACP, 0, strGBK.c_str(), -1, str1, n);

    //�õ�ת����Ĵ�С 
    n = WideCharToMultiByte(CP_UTF8, 0, str1, -1, NULL, 0, NULL, NULL);
    
    //���뻺����
    char *str2 = new char[n];
    ZeroMemory(str2, n);
    
    //ת�����룬���ֽڵ����ֽ�
    WideCharToMultiByte(CP_UTF8, 0, str1, -1, str2, n, NULL, NULL);
    
    strOutUTF8 = str2;
    
    //ɾ��������
    delete [] str1;
    delete[]str2;
    
    return strOutUTF8;
}

//Ĭ��
#define HTTPREQUEST_PROXYSETTING_DEFAULT    0
#define HTTPREQUEST_PROXYSETTING_PRECONFIG  0

//ע���
#define HTTPREQUEST_PROXYSETTING_DIRECT     1

//��дIP:PROT
#define HTTPREQUEST_PROXYSETTING_PROXY      2

//���ô���Ĭ��Ϊ����
INLINE HRESULT CAssist::SetProxy(IWinHttpRequestPtr& pObj, _bstr_t bstrProxy)
{
    return pObj->SetProxy(HTTPREQUEST_PROXYSETTING_PROXY, bstrProxy);
}

//���ú���Https ����
INLINE HRESULT CAssist::SetIgnoreHTTPS(IWinHttpRequestPtr& pObj)
{
    //���ú���HTTPS������
    try
    {
        _variant_t val = (long)0x3300;
        pObj->PutOption(WinHttpRequestOption_SslErrorIgnoreFlags, val);
    }
    catch (_com_error e)
    {
        
    }
    return S_OK;
}

//���ý���302��ת
INLINE HRESULT CAssist::SetHttpJmpFlase(IWinHttpRequestPtr& pObj)
{
    //302�Զ���ת���ƣ�Ĭ��Ϊ����������Ϊ������
    //COleVariant varFalse = VARIANT_FALSE;
    //pHttpReq->put_Option(WinHttpRequestOption_EnableRedirects, varFalse);
	pObj->Option[WinHttpRequestOption_EnableRedirects] = VARIANT_FALSE;


    return S_OK;
}

#endif // !defined(AFX_ASSIST_H__890A911F_8712_49E4_8A06_7181638C00E2__INCLUDED_)
