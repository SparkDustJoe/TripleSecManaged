
#pragma once

#define ONE_CORE 0
#define SPEED_TEST 0
#define TEST_SHORT_HASHES 1
#define TEST_EXTREMELY_LONG_HASHES 1
#define GENERATE_EXTREMELY_LONG_HASHES 0
#define CPU_UNDERLOAD 0
#define GENERATE_SHORT_HASHES 0

#include <afx.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stddef.h>
#include <limits>
#include <time.h>

typedef unsigned char byte;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned __int64 ulong;

#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

#ifndef DEBUG_NEW
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif
#define new DEBUG_NEW

#endif

#include "Stopwatch.h"
#include "TestBase.h"
#include "TestBaseBlock.h"
#include "TestBaseCrypto.h"
#include "TestBaseNonCryptoWithKey.h"
#include "TestBaseNonCryptoNonBlock.h"
