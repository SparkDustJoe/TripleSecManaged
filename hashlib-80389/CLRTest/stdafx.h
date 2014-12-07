
#pragma once 

#define ONE_CORE 0
#define SPEED_TEST 0
#define TEST_SHORT_HASHES 1
#define TEST_EXTREMELY_LONG_HASHES 1
#define CPU_UNDERLOAD 0

typedef unsigned char byte;
typedef unsigned int uint;
typedef unsigned __int64 ulong;

using namespace System;
using namespace System::Collections::Generic;
using namespace System::Diagnostics;
using namespace System::IO;
using namespace HashLib;

#include "Trans.h"
#include "TestBase.h"
#include "TestBaseCrypto.h"
#include "TestBaseNonCryptoNonBlock.h"
#include "TestBaseNonCryptoWithKey.h"
#include "HashBuffer.h"