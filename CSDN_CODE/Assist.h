// Assist.h: interface for the CAssist class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ASSIST_H__890A911F_8712_49E4_8A06_7181638C00E2__INCLUDED_)
#define AFX_ASSIST_H__890A911F_8712_49E4_8A06_7181638C00E2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//	针对编译器设置预编译行为
#if _MSC_VER < 1400
//	禁用部分警告（VC6.0问题）
#pragma warning(disable: 4068)	//	VC6.0对标准模板库支持不良，报各种奇怪错误
#pragma warning(disable: 4503) 
#pragma warning(disable: 4786) 
#else
//	禁用部分警告（VC2008问题）
#pragma warning(disable: 4819)	//	报OpenCV代码字符警告
#pragma warning(disable: 4996)	//	报不安全函数警告
#endif


#include <map>
#include <string>

#define INLINE  inline

//导入WinHttp
#import "C:\\Windows\\System32\\WinHttp.dll" no_namespace

#ifdef _UNICODE
#define tstring std::wstring
#else
#define tstring std::string
#endif

#ifndef MAP
#define MAP std::map
#endif

class CAssist  
{
public:
    CAssist(){srand((unsigned)time( NULL ));}
    virtual ~CAssist(){}

    //字符串到十六进制
    INLINE BOOL StrToNum(tstring strNum, LONG& lRet);

    //忽略HTTPS不信任
    INLINE HRESULT IgnoreHTTPS(IWinHttpRequestPtr& pObj);

    //设置访问代理(调试用)
    INLINE HRESULT SetProxy(IWinHttpRequestPtr& pObj, _bstr_t bstrProxy = TEXT("127.0.0.1:8888"));
    

    //编码转换
    INLINE std::string GBKToUTF8(const std::string& strGBK);
    INLINE std::string UTF8ToGBK(const std::string& strUTF8);

    //定义获取Get返回字符串的数据
    INLINE BOOL Get_WinHttp_RspString(IWinHttpRequestPtr& pObj, std::string& strDst);

    //获取指定字符串，前后标志中间的数据
    //源数据，左标志，又标志，是否包含左边标志长度，是否包含右边标志长度
    INLINE std::string GetMidStrByLAndR(const std::string& strSrc, std::string strLeft, std::string strRight, BOOL bIncludeStart = FALSE, BOOL bIncludeEnd = FALSE);
    
    //获得COOKIES
    INLINE tstring GetCookieString(tstring strRspHeader, MAP<tstring, tstring>& CookieMap, BOOL bAllGet = FALSE);

    //获得时间戳
    INLINE LONG GetTimer();

    //获取一个随机字母
    INLINE DWORD GetRand();

    //转换Cookie字符串
    //哈希表到字符串
    INLINE BOOL MapToCookie(tstring& strDst, MAP<tstring, tstring>& CookieMap);
    
    //字符串到哈希表
    INLINE BOOL CookieToMap(tstring strCookie, MAP<tstring, tstring>& CookieMap);

    MAP<tstring, tstring> m_CookieMap;
};

//获得Cookie，返回的Header后的Body
INLINE tstring CAssist::GetCookieString(tstring strRspHeader, MAP<tstring, tstring>& CookieMap, BOOL bAllGet)
{
    //返回Cookie
    tstring retCookie;
    
    //Cookie标志
    tstring strKey = _T("Set-Cookie: ");
    
    int iBegin = 0, iEnd = 0;
    while(iBegin < strRspHeader.length())
    {
        //得到Cookie的位置
        iBegin = strRspHeader.find(strKey, iBegin);
        if (iBegin >= 0)
        {
            //Cookie的尾部为\r\n
            iEnd = strRspHeader.find(_T("\r\n"), iBegin);
            if (iEnd > 0)
            {
                //截取Cookie用于返回
                //判断是否需要原样获取字符串
                if(!bAllGet)
                {
                    //截取一行Cookie
                    tstring strTmp = strRspHeader.substr(iBegin, iEnd-iBegin+2);
                    tstring strDst = TEXT("");
                    
                    //获取SetCookie信息  从头 到 分号
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

//Cookie String 到 Map
INLINE BOOL CAssist::CookieToMap(tstring strCookie, MAP<tstring, tstring>& CookieMap)
{
    tstring strTmp = TEXT("");
    tstring strHead = TEXT("");
    tstring strBody = TEXT("");
    
    while(TRUE)
    {
        //找到分号第一次出现的位置
        int nEnd = strCookie.find_first_of(TEXT(";"));
        
        //未找到 退出
        if(nEnd == tstring::npos)
            break;
        
        //获得一条Cookie
        strTmp = strCookie.substr(0, nEnd+1);
        
        //拆分单条Cookie
        int nSplit = strTmp.find(TEXT("="));
        
        //得到等号前边的字段，
        strHead = strTmp.substr(0, nSplit);
        
        //得到登号后边的字段
        strBody = strTmp.substr(nSplit + 1, strTmp.length() - nSplit - 2);
        
        //增加哈希表
        CookieMap[strHead] = strBody;
        
        //去除已经处理的字符串
        strCookie = strCookie.substr(nEnd + 1, strCookie.length() - nEnd - 1);
        
        //删除前置空格
        strCookie.erase(0, strCookie.find_first_not_of(TEXT(" ")));
    }
    
    return TRUE;
}

//cookie Map 到 String
INLINE BOOL CAssist::MapToCookie(tstring& strDst, MAP<tstring, tstring>& CookieMap)
{
    //判断是否第一次进入，第一次进入头前不加空格，非第一次则加空格隔开
    static BOOL bIsFirstInfo = TRUE;
    char* pszTmp = TEXT("");
    
    MAP<tstring, tstring>::iterator  iter;
    for(iter = CookieMap.begin(); iter != CookieMap.end(); iter++)
    {
        //非第一次进入则加入空格
        if(!bIsFirstInfo)
        {
            pszTmp = TEXT(" ");
        }
        
        //拼字符串      LocalPart=csdntest;
        strDst = strDst + pszTmp + iter->first + "=" + iter->second + ";";
        
        //第一次进入之后设置
        bIsFirstInfo = FALSE;
    }

    strDst = strDst.substr(0, strDst.find_last_of(TEXT(";")));
    
    CookieMap.clear();
    return TRUE;
}

//字符串 转  整型
INLINE BOOL CAssist::StrToNum(tstring strNum, LONG& lRet)
{
    #define HEX 16
    
    LPTSTR pEnd = NULL;
    LPTSTR pStart = &strNum[0];
    
    //得到数据包中DeCode Len
    lRet = _tcstol(strNum.c_str(), &pEnd, HEX);
    
    if(pEnd - pStart != strNum.length())
    {
        return FALSE;
    }

    return TRUE;
}

//获取0~32的随机数
INLINE DWORD CAssist::GetRand()
{
    
    //得到一个随机大小写字母
    DWORD dwRand = rand() % 26;

    //随机大小写
    DWORD dwLower = rand() % 32;

    DWORD dwRet = 0;

    //得到余数0 或 1
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

//获取时间戳
INLINE LONG CAssist::GetTimer()
{
    //获取时间戳
    return time(0);
}

//获取Body字符串
INLINE BOOL CAssist::Get_WinHttp_RspString(IWinHttpRequestPtr& pObj, std::string& strDst)
{				
    _variant_t varRspBody = pObj->GetResponseBody();		
    ULONG dataLen = varRspBody.parray->rgsabound[0].cElements;				
    char *pContentBuffer = (char *)varRspBody.parray->pvData;				
    strDst = pContentBuffer;
}

//得到标志中间的字符串
INLINE std::string CAssist::GetMidStrByLAndR(const std::string& strSrc, std::string strLeft, std::string strRight, BOOL bIncludeStart, BOOL bIncludeEnd)
{
    std::string strRet = TEXT("");

    //获取左标志的第一个位置
    int eIdxBegin = strSrc.find(strLeft);

    if(eIdxBegin != -1)
    {
        //返回值是否包含前标志
        if (!bIncludeStart) 
        {
            eIdxBegin += strLeft.length();
        }
        
        int eIdxEnd = strSrc.find(strRight, eIdxBegin + 1);
        if (eIdxEnd != -1)
        {
            //返回值是否包含后标志
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

//转换编码 UTF8 到 GBK
INLINE std::string CAssist::UTF8ToGBK(const std::string& strUTF8)
{
    //得到编码转换之后的长度
    int len = MultiByteToWideChar(CP_UTF8, 0, strUTF8.c_str(), -1, NULL, 0);
    
    //申请缓冲区
    wchar_t *wszGBK = new wchar_t[len + 1];
    memset(wszGBK, 0, (len + 1)*sizeof(wchar_t));
    
    //转换编码  多字节到宽字节
    MultiByteToWideChar(CP_UTF8, 0, strUTF8.c_str(), -1, wszGBK, len);
    
    //得到编码转换之后的长度
    len = WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, NULL, 0, NULL, NULL);
    
    //申请缓冲区
    char *szGBK = new char[len + 1];
    memset(szGBK, 0, len + 1);
    
    //转换编码， 宽字节到多字节
    WideCharToMultiByte(CP_ACP,0, wszGBK, -1, szGBK, len, NULL, NULL);
    //strUTF8 = szGBK;
    std::string strTemp(szGBK);
    
    //释放缓冲区
    delete[]szGBK;
    delete[]wszGBK;
    
    return strTemp;
}

//转换编码 GBK 到 UTF8
INLINE std::string CAssist::GBKToUTF8(const std::string& strGBK)
{
    //定义回执缓冲区
    std::string strOutUTF8 = "";
    
    //调用转换函数得到转换后的大小
    int n = MultiByteToWideChar(CP_ACP, 0, strGBK.c_str(), -1, NULL, 0);
    
    //申请缓冲区
    WCHAR* str1 = new WCHAR[n];
    ZeroMemory(str1, n*sizeof(WCHAR));
    
    //转换编码  多字节到宽字节
    MultiByteToWideChar(CP_ACP, 0, strGBK.c_str(), -1, str1, n);

    //得到转换后的大小 
    n = WideCharToMultiByte(CP_UTF8, 0, str1, -1, NULL, 0, NULL, NULL);
    
    //申请缓冲区
    char *str2 = new char[n];
    ZeroMemory(str2, n);
    
    //转换编码，宽字节到多字节
    WideCharToMultiByte(CP_UTF8, 0, str1, -1, str2, n, NULL, NULL);
    
    strOutUTF8 = str2;
    
    //删除缓冲区
    delete [] str1;
    delete[]str2;
    
    return strOutUTF8;
}

//默认
#define HTTPREQUEST_PROXYSETTING_DEFAULT    0
#define HTTPREQUEST_PROXYSETTING_PRECONFIG  0

//注册表
#define HTTPREQUEST_PROXYSETTING_DIRECT     1

//填写IP:PROT
#define HTTPREQUEST_PROXYSETTING_PROXY      2

//设置代理，默认为本地
INLINE HRESULT CAssist::SetProxy(IWinHttpRequestPtr& pObj, _bstr_t bstrProxy)
{
    return pObj->SetProxy(HTTPREQUEST_PROXYSETTING_PROXY, bstrProxy);
}

//设置忽略Https 警告
INLINE HRESULT CAssist::IgnoreHTTPS(IWinHttpRequestPtr& pObj)
{
    //设置忽略HTTPS不信任
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

#endif // !defined(AFX_ASSIST_H__890A911F_8712_49E4_8A06_7181638C00E2__INCLUDED_)
