// CookieCut.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "stdafx.h"

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

        //δ�ҵ� �˳�
        if(nEnd == string::npos)
            break;

        //���һ��Cookie
        strTmp = strCookie.substr(0, nEnd+1);
    
        //��ֵ���Cookie
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
        //��һ�ν��벻�ӿո�
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
