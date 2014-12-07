
#include "stdafx.h"
#include "BlakeTest.h"
#include "BlueMidnightWishTest.h"
#include "CubeHashTest.h"
#include "EchoTest.h"
#include "FugueTest.h"
#include "GroestlTest.h"
#include "HamsiTest.h"
#include "JHTest.h"
#include "KeccakTest.h"
#include "LuffaTest.h"
#include "ShabalTest.h"
#include "SHAvite3Test.h"
#include "SIMDTest.h"
#include "SkeinTest.h"
#include "SipHashTest.h"
#include "Murmur3Test.h"

using namespace CLRTest;

void main()
{
    System::Console::SetBufferSize(200, 2000);

    TestBase::Init();

    BlakeTest::DoTest();
    BlakeCSharpTest::DoTest();
    
    BlueMidnightWishTest::DoTest();
    BlueMidnightWishCSharpTest::DoTest();
    
    CubeHashTest::DoTest();
    CubeHashCSharpTest::DoTest();
    
    EchoTest::DoTest();
    EchoCSharpTest::DoTest();
    
    FugueTest::DoTest();
    FugueCSharpTest::DoTest();
    
    GroestlTest::DoTest();
    GroestlCSharpTest::DoTest();
    
    HamsiTest::DoTest();
    HamsiCSharpTest::DoTest();
    
    JHTest::DoTest();
    JHCSharpTest::DoTest();
    
    KeccakTest::DoTest();
    KeccakCSharpTest::DoTest();
    
    LuffaTest::DoTest();
    LuffaCSharpTest::DoTest();
    
    ShabalTest::DoTest();
    ShabalCSharpTest::DoTest();
    
    SHAvite3Test::DoTest();
    SHAvite3CSharpTest::DoTest();
    
    SIMDTest::DoTest();
    SIMDCSharpTest::DoTest();
    
    SkeinTest::DoTest();
    SkeinCSharpTest::DoTest();
    
    SipHashTest::DoTest();
    SipHashCSharpTest::DoTest();
    
    Murmur3CSharpTest::DoTest();
    Murmur3_128_CSharpTest::DoTest();

    System::Console::WriteLine("\nDone \n");
    System::Console::ReadKey();
}