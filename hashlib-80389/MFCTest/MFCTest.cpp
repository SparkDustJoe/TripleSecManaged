
#include "stdafx.h"
#include "JHTest.h"
#include "BlakeTest.h"
#include "BlueMidnightWishTest.h"
#include "CubeHashTest.h"
#include "EchoTest.h"
#include "FugueTest.h"
#include "GroestlTest.h"
#include "HamsiTest.h"
#include "KeccakTest.h"
#include "LuffaTest.h"
#include "ShabalTest.h"
#include "SHAvite3Test.h"
#include "SIMDTest.h"
#include "SkeinTest.h"
#include "SipHashTest.h"
#include "Murmur3Test.h"

using namespace MFCTest;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); 
    COORD bufferSize = { 200, 2000 }; 
    SetConsoleScreenBufferSize(hConsole, bufferSize);

    TestBaseCrypto::Init();

    BlakeOrgTest().Test();
    BlakeTest().Test();
    
    BlueMidnightWishOrgTest().Test();
    BlueMidnightWishTest().Test();
    
    CubeHashOrgTest().Test();
    CubeHashTest().Test();
    
    EchoOrgTest().Test();
    EchoTest().Test();
    
    FugueOrgTest().Test();
    FugueTest().Test();
    
    GroestlOrgTest().Test();
    GroestlTest().Test();
    
    HamsiOrgTest().Test();
    HamsiTest().Test();
    
    JHOrgTest().Test(); 
    JHTest().Test(); 
    
    KeccakOrgTest().Test();
    KeccakTest().Test();
    
    LuffaOrgTest().Test();
    LuffaTest().Test();
    
    ShabalOrgTest().Test();
    ShabalTest().Test();
    
    SHAvite3OrgTest().Test(); 
    SHAvite3Test().Test();
    
    SIMDOrgTest().Test();
    SIMDTest().Test(); 
    
    SkeinOrgTest().Test();
    SkeinTest().Test();
    
    SipHashOrgTest();
    SipHashTest().Test();
    
    Murmur3_32_OrgTest().Test();
    Murmur3_128_OrgTest().Test();
    Murmur3_32_Test().Test();
    Murmur3_128_Test().Test();
    
    printf("Done \n");
    getchar();

    return 0;
}