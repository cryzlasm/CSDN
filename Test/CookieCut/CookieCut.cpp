// CookieCut.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "stdafx.h"

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
#include <IOSTREAM>
using namespace std;


long CookieToMap(string strCookie, map<string, string>& CookieMap)
{
    string strTmp = "";
    string strHead = "";
    string strBody = "";
    while(1)
    {
        int nEnd = strCookie.find_first_of(';');

        //未找到 退出
        if(nEnd == string::npos)
            break;

        //获得一条Cookie
        strTmp = strCookie.substr(0, nEnd+1);
    
        //拆分单条Cookie
        int nSplit = strTmp.find("=");

        strHead = strTmp.substr(0, nSplit);
        strBody = strTmp.substr(nSplit + 1, strTmp.length() - nSplit - 2);

        CookieMap[strHead] = strBody;

        strCookie = strCookie.substr(nEnd + 1, strCookie.length() - nEnd - 1);

        strCookie.erase(0, strCookie.find_first_not_of(" "));
    }

    return 1;
}

long MapToCookie(string& strDst, map<string, string>& CookieMap)
{
    static bool bIsFirstInfo = true;
    char* pszTmp = "";

    map<string, string>::iterator  iter;
    for(iter = CookieMap.begin(); iter != CookieMap.end(); iter++)
    {
        //第一次进入不加空格
        if(!bIsFirstInfo)
        {
            pszTmp = " ";
        }

        strDst = strDst + pszTmp + iter->first + "=" + iter->second + ";";

        bIsFirstInfo = false;
    }
    
    CookieMap.clear();
    return 1;
}

#define STRING ("UserName=xeludukuzefo; UserInfo=%2ByxI6DjDNSRJwUDKmDIEPB4wcKl7EBoAwWGWC7fuSypDScxmeHcYP8QpHEk1b1o1wurfMGPzmvh1ehxEgoGHTuPD4QlbUM24DNzSgSEBmsUAeNCYlCrV4cK%2FQrJoFvUEFmChEe0cBrhc%2BoLQxGx%2FEA%3D%3D; UserNick=xeludukuzefo; Domain=.csdn.net; AU=5EF; N=xeludukuzefo; UE=\"xeludukuzefo@spambog.com\"; BT=1474548483142; access-token=2d90ed4d-799b-4b70-9aab-82ad2a8ef0ae;")

int main(int argc, char* argv[])
{
    map<string, string> CookieMap;

    CookieToMap(STRING, CookieMap);

    map<string, string>::iterator  iter;
    for(iter = CookieMap.begin(); iter != CookieMap.end(); iter++)
    {
        cout << iter->first << "   "<< iter->second << endl << endl;
    }

    string strDst = "";
    MapToCookie(strDst, CookieMap);

    cout << strDst << endl;
	return 0;
}
