// BaseTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "Base64.h"

#include <IOSTREAM>

int main(int argc, char* argv[])
{
    CBase64 tBase;
    
    int Len = 0;



    #define pString "Q1N0cmluZyBDU2NhbkRsZzo6RW5jb2RlSW1hZ2UoKQ=="

    const char* pStr = pString;

    string strOut = tBase.Decode(pStr, strlen(pStr), Len);

    cout << strOut << endl;
    

    cout << pString << endl;
    
    pStr = strOut.c_str();
    cout << tBase.Encode(pStr, strOut.length()) << endl;


    return 0;
}
