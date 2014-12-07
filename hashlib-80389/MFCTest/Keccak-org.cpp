
#include "stdafx.h"
#include "Keccak-org.h"

namespace KeccakOrg
{
    /*
    The Keccak sponge function, designed by Guido Bertoni, Joan Daemen,
    Michaël Peeters and Gilles Van Assche. For more information, feedback or
    questions, please refer to our website: http://keccak.noekeon.org/

    Implementation by the designers,
    hereby denoted as "the implementer".

    To the extent possible under law, the implementer has waived all copyright
    and related or neighboring rights to the source code in this file.
    http://creativecommons.org/publicdomain/zero/1.0/
    */

    /**
    * Function to initialize the state of the Keccak[r, c] sponge function.
    * The sponge function is set to the absorbing phase.
    * @param  state       Pointer to the state of the sponge function to be initialized.
    * @param  rate        The value of the rate r.
    * @param  capacity    The value of the capacity c.
    * @pre    One must have r+c=1600 and the rate a multiple of 64 bits in this implementation.
    * @return Zero if successful, 1 otherwise.
    */
    int InitSponge(hashState *state, unsigned int rate, unsigned int capacity);
    /**
    * Function to give input data for the sponge function to absorb.
    * @param  state       Pointer to the state of the sponge function initialized by InitSponge().
    * @param  data        Pointer to the input data.
    *                     When @a databitLen is not a multiple of 8, the last bits of data must be
    *                     in the least significant bits of the last byte.
    * @param  databitLen  The number of input bits provided in the input data.
    * @pre    In the previous call to Absorb(), databitLen was a multiple of 8.
    * @pre    The sponge function must be in the absorbing phase,
    *         i.e., Squeeze() must not have been called before.
    * @return Zero if successful, 1 otherwise.
    */
    int Absorb(hashState *state, const unsigned char *data, unsigned long long databitlen);

    void KeccakInitialize();
    void KeccakInitializeState(unsigned char *state);

    int InitSponge(hashState *state, unsigned int rate, unsigned int capacity)
    {
        if (rate + capacity != 1600)
            return 1;
        if ((rate <= 0) || (rate >= 1600) || ((rate % 64) != 0))
            return 1;
        KeccakInitialize();
        state->rate = rate;
        state->capacity = capacity;
        state->hashbitlen = 0;
        KeccakInitializeState(state->state);
        memset(state->dataQueue, 0, KeccakMaximumRateInBytes);
        state->bitsInQueue = 0;
        state->squeezing = 0;
        state->bitsAvailableForSqueezing = 0;

        return 0;
    }

    void KeccakAbsorb(unsigned char *state, const unsigned char *data, unsigned int laneCount);

    void AbsorbQueue(hashState *state)
    {
        // state->bitsInQueue is assumed to be equal to state->rate
#ifdef KeccakReference
        displayBytes(1, "Block to be absorbed", state->dataQueue, state->rate / 8);
#endif
#ifdef ProvideFast576
        if (state->rate == 576)
            KeccakAbsorb576bits(state->state, state->dataQueue);
        else
#endif
#ifdef ProvideFast832
        if (state->rate == 832)
            KeccakAbsorb832bits(state->state, state->dataQueue);
        else
#endif
#ifdef ProvideFast1024
        if (state->rate == 1024)
            KeccakAbsorb1024bits(state->state, state->dataQueue);
        else
#endif
#ifdef ProvideFast1088
        if (state->rate == 1088)
            KeccakAbsorb1088bits(state->state, state->dataQueue);
        else
#endif
#ifdef ProvideFast1152
        if (state->rate == 1152)
            KeccakAbsorb1152bits(state->state, state->dataQueue);
        else
#endif
#ifdef ProvideFast1344
        if (state->rate == 1344)
            KeccakAbsorb1344bits(state->state, state->dataQueue);
        else
#endif
            KeccakAbsorb(state->state, state->dataQueue, state->rate / 64);
        state->bitsInQueue = 0;
    }

    int Absorb(hashState *state, const unsigned char *data, unsigned long long databitlen)
    {
        unsigned long long i, j, wholeBlocks;
        unsigned int partialBlock, partialByte;
        const unsigned char *curData;

        if ((state->bitsInQueue % 8) != 0)
            return 1; // Only the last call may contain a partial byte
        if (state->squeezing)
            return 1; // Too late for additional input

        i = 0;
        while (i < databitlen)
        {
            if ((state->bitsInQueue == 0) && (databitlen >= state->rate) && (i <= (databitlen - state->rate)))
            {
                wholeBlocks = (databitlen - i) / state->rate;
                curData = data + i / 8;
#ifdef ProvideFast576
                if (state->rate == 576)
                {
                    for (j = 0; j<wholeBlocks; j++, curData += 576 / 8)
                    {
#ifdef KeccakReference
                        displayBytes(1, "Block to be absorbed", curData, state->rate / 8);
#endif
                        KeccakAbsorb576bits(state->state, curData);
                    }
                }
                else
#endif
#ifdef ProvideFast832
                if (state->rate == 832)
                {
                    for (j = 0; j<wholeBlocks; j++, curData += 832 / 8)
                    {
#ifdef KeccakReference
                        displayBytes(1, "Block to be absorbed", curData, state->rate / 8);
#endif
                        KeccakAbsorb832bits(state->state, curData);
                    }
                }
                else
#endif
#ifdef ProvideFast1024
                if (state->rate == 1024)
                {
                    for (j = 0; j<wholeBlocks; j++, curData += 1024 / 8)
                    {
#ifdef KeccakReference
                        displayBytes(1, "Block to be absorbed", curData, state->rate / 8);
#endif
                        KeccakAbsorb1024bits(state->state, curData);
                    }
                }
                else
#endif
#ifdef ProvideFast1088
                if (state->rate == 1088)
                {
                    for (j = 0; j<wholeBlocks; j++, curData += 1088 / 8)
                    {
#ifdef KeccakReference
                        displayBytes(1, "Block to be absorbed", curData, state->rate / 8);
#endif
                        KeccakAbsorb1088bits(state->state, curData);
                    }
                }
                else
#endif
#ifdef ProvideFast1152
                if (state->rate == 1152)
                {
                    for (j = 0; j<wholeBlocks; j++, curData += 1152 / 8)
                    {
#ifdef KeccakReference
                        displayBytes(1, "Block to be absorbed", curData, state->rate / 8);
#endif
                        KeccakAbsorb1152bits(state->state, curData);
                    }
                }
                else
#endif
#ifdef ProvideFast1344
                if (state->rate == 1344)
                {
                    for (j = 0; j<wholeBlocks; j++, curData += 1344 / 8)
                    {
#ifdef KeccakReference
                        displayBytes(1, "Block to be absorbed", curData, state->rate / 8);
#endif
                        KeccakAbsorb1344bits(state->state, curData);
                    }
                }
                else
#endif
                {
                    for (j = 0; j<wholeBlocks; j++, curData += state->rate / 8)
                    {
#ifdef KeccakReference
                        displayBytes(1, "Block to be absorbed", curData, state->rate / 8);
#endif
                        KeccakAbsorb(state->state, curData, state->rate / 64);
                    }
                }
                i += wholeBlocks*state->rate;
            }
            else
            {
                partialBlock = (unsigned int)(databitlen - i);
                if (partialBlock + state->bitsInQueue > state->rate)
                    partialBlock = state->rate - state->bitsInQueue;
                partialByte = partialBlock % 8;
                partialBlock -= partialByte;
                memcpy(state->dataQueue + state->bitsInQueue / 8, data + i / 8, partialBlock / 8);
                state->bitsInQueue += partialBlock;
                i += partialBlock;
                if (state->bitsInQueue == state->rate)
                    AbsorbQueue(state);
                if (partialByte > 0)
                {
                    unsigned char mask = (1 << partialByte) - 1;
                    state->dataQueue[state->bitsInQueue / 8] = data[i / 8] & mask;
                    state->bitsInQueue += partialByte;
                    i += partialByte;
                }
            }
        }
        return 0;
    }

    void KeccakExtract(const unsigned char *state, unsigned char *data, unsigned int laneCount);

    void PadAndSwitchToSqueezingPhase(hashState *state)
    {
        // Note: the bits are numbered from 0=LSB to 7=MSB
        if (state->bitsInQueue + 1 == state->rate)
        {
            state->dataQueue[state->bitsInQueue / 8] |= 1 << (state->bitsInQueue % 8);
            AbsorbQueue(state);
            memset(state->dataQueue, 0, state->rate / 8);
        }
        else
        {
            memset(state->dataQueue + (state->bitsInQueue + 7) / 8, 0, state->rate / 8 - (state->bitsInQueue + 7) / 8);
            state->dataQueue[state->bitsInQueue / 8] |= 1 << (state->bitsInQueue % 8);
        }
        state->dataQueue[(state->rate - 1) / 8] |= 1 << ((state->rate - 1) % 8);
        AbsorbQueue(state);

#ifdef KeccakReference
        displayText(1, "--- Switching to squeezing phase ---");
#endif
#ifdef ProvideFast1024
        if (state->rate == 1024)
        {
            KeccakExtract1024bits(state->state, state->dataQueue);
            state->bitsAvailableForSqueezing = 1024;
        }
        else
#endif
        {
            KeccakExtract(state->state, state->dataQueue, state->rate / 64);
            state->bitsAvailableForSqueezing = state->rate;
        }
#ifdef KeccakReference
        displayBytes(1, "Block available for squeezing", state->dataQueue, state->bitsAvailableForSqueezing / 8);
#endif
        state->squeezing = 1;
    }

    void KeccakPermutation(unsigned char *state);

    HashReturn Squeeze(hashState *state, BitSequence *output, DataLength outputLength)
    {
        unsigned long long i;
        unsigned int partialBlock;

        if (!state->squeezing)
            PadAndSwitchToSqueezingPhase(state);
        if ((outputLength % 8) != 0)
            return FAIL; // Only multiple of 8 bits are allowed, truncation can be done at user level

        i = 0;
        while (i < outputLength)
        {
            if (state->bitsAvailableForSqueezing == 0)
            {
                KeccakPermutation(state->state);
#ifdef ProvideFast1024
                if (state->rate == 1024)
                {
                    KeccakExtract1024bits(state->state, state->dataQueue);
                    state->bitsAvailableForSqueezing = 1024;
                }
                else
#endif
                {
                    KeccakExtract(state->state, state->dataQueue, state->rate / 64);
                    state->bitsAvailableForSqueezing = state->rate;
                }
#ifdef KeccakReference
                displayBytes(1, "Block available for squeezing", state->dataQueue, state->bitsAvailableForSqueezing / 8);
#endif
            }
            partialBlock = state->bitsAvailableForSqueezing;
            if ((unsigned long long)partialBlock > outputLength - i)
                partialBlock = (unsigned int)(outputLength - i);
            memcpy(output + i / 8, state->dataQueue + (state->rate - state->bitsAvailableForSqueezing) / 8, partialBlock / 8);
            state->bitsAvailableForSqueezing -= partialBlock;
            i += partialBlock;
        }
        return SUCCESS;
    }

    /*
    ---------------------------------------------------------------------------
    Copyright (c) 1998-2008, Brian Gladman, Worcester, UK. All rights reserved.

    LICENSE TERMS

    The redistribution and use of this software (with or without changes)
    is allowed without the payment of fees or royalties provided that:

    1. source code distributions include the above copyright notice, this
    list of conditions and the following disclaimer;

    2. binary distributions include the above copyright notice, this list
    of conditions and the following disclaimer in their documentation;

    3. the name of the copyright holder is not used to endorse products
    built using this software without specific written permission.

    DISCLAIMER

    This software is provided 'as is' with no explicit or implied warranties
    in respect of its properties, including, but not limited to, correctness
    and/or fitness for purpose.
    ---------------------------------------------------------------------------
    Issue Date: 20/12/2007
    Changes for ARM 9/9/2010
    */

#ifndef _BRG_ENDIAN_H
#define _BRG_ENDIAN_H

#define IS_BIG_ENDIAN      4321 /* byte 0 is most significant (mc68k) */
#define IS_LITTLE_ENDIAN   1234 /* byte 0 is least significant (i386) */

#if 0
    /* Include files where endian defines and byteswap functions may reside */
#if defined( __sun )
#  include <sys/isa_defs.h>
#elif defined( __FreeBSD__ ) || defined( __OpenBSD__ ) || defined( __NetBSD__ )
#  include <sys/endian.h>
#elif defined( BSD ) && ( BSD >= 199103 ) || defined( __APPLE__ ) || \
    defined(__CYGWIN32__) || defined(__DJGPP__) || defined(__osf__)
#  include <machine/endian.h>
#elif defined( __linux__ ) || defined( __GNUC__ ) || defined( __GNU_LIBRARY__ )
#  if !defined( __MINGW32__ ) && !defined( _AIX )
#    include <endian.h>
#    if !defined( __BEOS__ )
#      include <byteswap.h>
#    endif
#  endif
#endif
#endif

    /* Now attempt to set the define for platform byte order using any  */
    /* of the four forms SYMBOL, _SYMBOL, __SYMBOL & __SYMBOL__, which  */
    /* seem to encompass most endian symbol definitions                 */

#if defined( BIG_ENDIAN ) && defined( LITTLE_ENDIAN )
#  if defined( BYTE_ORDER ) && BYTE_ORDER == BIG_ENDIAN
#    define PLATFORM_BYTE_ORDER IS_BIG_ENDIAN
#  elif defined( BYTE_ORDER ) && BYTE_ORDER == LITTLE_ENDIAN
#    define PLATFORM_BYTE_ORDER IS_LITTLE_ENDIAN
#  endif
#elif defined( BIG_ENDIAN )
#  define PLATFORM_BYTE_ORDER IS_BIG_ENDIAN
#elif defined( LITTLE_ENDIAN )
#  define PLATFORM_BYTE_ORDER IS_LITTLE_ENDIAN
#endif

#if defined( _BIG_ENDIAN ) && defined( _LITTLE_ENDIAN )
#  if defined( _BYTE_ORDER ) && _BYTE_ORDER == _BIG_ENDIAN
#    define PLATFORM_BYTE_ORDER IS_BIG_ENDIAN
#  elif defined( _BYTE_ORDER ) && _BYTE_ORDER == _LITTLE_ENDIAN
#    define PLATFORM_BYTE_ORDER IS_LITTLE_ENDIAN
#  endif
#elif defined( _BIG_ENDIAN )
#  define PLATFORM_BYTE_ORDER IS_BIG_ENDIAN
#elif defined( _LITTLE_ENDIAN )
#  define PLATFORM_BYTE_ORDER IS_LITTLE_ENDIAN
#endif

#if defined( __BIG_ENDIAN ) && defined( __LITTLE_ENDIAN )
#  if defined( __BYTE_ORDER ) && __BYTE_ORDER == __BIG_ENDIAN
#    define PLATFORM_BYTE_ORDER IS_BIG_ENDIAN
#  elif defined( __BYTE_ORDER ) && __BYTE_ORDER == __LITTLE_ENDIAN
#    define PLATFORM_BYTE_ORDER IS_LITTLE_ENDIAN
#  endif
#elif defined( __BIG_ENDIAN )
#  define PLATFORM_BYTE_ORDER IS_BIG_ENDIAN
#elif defined( __LITTLE_ENDIAN )
#  define PLATFORM_BYTE_ORDER IS_LITTLE_ENDIAN
#endif

#if defined( __BIG_ENDIAN__ ) && defined( __LITTLE_ENDIAN__ )
#  if defined( __BYTE_ORDER__ ) && __BYTE_ORDER__ == __BIG_ENDIAN__
#    define PLATFORM_BYTE_ORDER IS_BIG_ENDIAN
#  elif defined( __BYTE_ORDER__ ) && __BYTE_ORDER__ == __LITTLE_ENDIAN__
#    define PLATFORM_BYTE_ORDER IS_LITTLE_ENDIAN
#  endif
#elif defined( __BIG_ENDIAN__ )
#  define PLATFORM_BYTE_ORDER IS_BIG_ENDIAN
#elif defined( __LITTLE_ENDIAN__ )
#  define PLATFORM_BYTE_ORDER IS_LITTLE_ENDIAN
#endif

    /*  if the platform byte order could not be determined, then try to */
    /*  set this define using common machine defines                    */
#if !defined(PLATFORM_BYTE_ORDER)

#if   defined( __alpha__ ) || defined( __alpha ) || defined( i386 )       || \
    defined(__i386__) || defined(_M_I86) || defined(_M_IX86) || \
    defined(__OS2__) || defined(sun386) || defined(__TURBOC__) || \
    defined(vax) || defined(vms) || defined(VMS) || \
    defined(__VMS) || defined(_M_X64)
#  define PLATFORM_BYTE_ORDER IS_LITTLE_ENDIAN

#elif defined( AMIGA )   || defined( applec )    || defined( __AS400__ )  || \
    defined(_CRAY) || defined(__hppa) || defined(__hp9000) || \
    defined(ibm370) || defined(mc68000) || defined(m68k) || \
    defined(__MRC__) || defined(__MVS__) || defined(__MWERKS__) || \
    defined(sparc) || defined(__sparc) || defined(SYMANTEC_C) || \
    defined(__VOS__) || defined(__TIGCC__) || defined(__TANDEM) || \
    defined(THINK_C) || defined(__VMCMS__) || defined(_AIX)
#  define PLATFORM_BYTE_ORDER IS_BIG_ENDIAN

#elif defined(__arm__)
# ifdef __BIG_ENDIAN
#  define PLATFORM_BYTE_ORDER IS_BIG_ENDIAN
# else
#  define PLATFORM_BYTE_ORDER IS_LITTLE_ENDIAN
# endif
#elif 1     /* **** EDIT HERE IF NECESSARY **** */
#  define PLATFORM_BYTE_ORDER IS_LITTLE_ENDIAN
#elif 0     /* **** EDIT HERE IF NECESSARY **** */
#  define PLATFORM_BYTE_ORDER IS_BIG_ENDIAN
#else
#  error Please edit lines 132 or 134 in brg_endian.h to set the platform byte order
#endif

#endif

#endif
/*
 ---------------------------------------------------------------------------
 Copyright (c) 1998-2008, Brian Gladman, Worcester, UK. All rights reserved.

 LICENSE TERMS

 The redistribution and use of this software (with or without changes)
 is allowed without the payment of fees or royalties provided that:

  1. source code distributions include the above copyright notice, this
     list of conditions and the following disclaimer;

  2. binary distributions include the above copyright notice, this list
     of conditions and the following disclaimer in their documentation;

  3. the name of the copyright holder is not used to endorse products
     built using this software without specific written permission.

 DISCLAIMER

 This software is provided 'as is' with no explicit or implied warranties
 in respect of its properties, including, but not limited to, correctness
 and/or fitness for purpose.
 ---------------------------------------------------------------------------
 Issue Date: 20/12/2007
 Changes for ARM 9/9/2010
*/

#define IS_BIG_ENDIAN      4321 /* byte 0 is most significant (mc68k) */
#define IS_LITTLE_ENDIAN   1234 /* byte 0 is least significant (i386) */

#if 0
/* Include files where endian defines and byteswap functions may reside */
#if defined( __sun )
#  include <sys/isa_defs.h>
#elif defined( __FreeBSD__ ) || defined( __OpenBSD__ ) || defined( __NetBSD__ )
#  include <sys/endian.h>
#elif defined( BSD ) && ( BSD >= 199103 ) || defined( __APPLE__ ) || \
      defined( __CYGWIN32__ ) || defined( __DJGPP__ ) || defined( __osf__ )
#  include <machine/endian.h>
#elif defined( __linux__ ) || defined( __GNUC__ ) || defined( __GNU_LIBRARY__ )
#  if !defined( __MINGW32__ ) && !defined( _AIX )
#    include <endian.h>
#    if !defined( __BEOS__ )
#      include <byteswap.h>
#    endif
#  endif
#endif
#endif

/* Now attempt to set the define for platform byte order using any  */
/* of the four forms SYMBOL, _SYMBOL, __SYMBOL & __SYMBOL__, which  */
/* seem to encompass most endian symbol definitions                 */

#if defined( BIG_ENDIAN ) && defined( LITTLE_ENDIAN )
#  if defined( BYTE_ORDER ) && BYTE_ORDER == BIG_ENDIAN
#    define PLATFORM_BYTE_ORDER IS_BIG_ENDIAN
#  elif defined( BYTE_ORDER ) && BYTE_ORDER == LITTLE_ENDIAN
#    define PLATFORM_BYTE_ORDER IS_LITTLE_ENDIAN
#  endif
#elif defined( BIG_ENDIAN )
#  define PLATFORM_BYTE_ORDER IS_BIG_ENDIAN
#elif defined( LITTLE_ENDIAN )
#  define PLATFORM_BYTE_ORDER IS_LITTLE_ENDIAN
#endif

#if defined( _BIG_ENDIAN ) && defined( _LITTLE_ENDIAN )
#  if defined( _BYTE_ORDER ) && _BYTE_ORDER == _BIG_ENDIAN
#    define PLATFORM_BYTE_ORDER IS_BIG_ENDIAN
#  elif defined( _BYTE_ORDER ) && _BYTE_ORDER == _LITTLE_ENDIAN
#    define PLATFORM_BYTE_ORDER IS_LITTLE_ENDIAN
#  endif
#elif defined( _BIG_ENDIAN )
#  define PLATFORM_BYTE_ORDER IS_BIG_ENDIAN
#elif defined( _LITTLE_ENDIAN )
#  define PLATFORM_BYTE_ORDER IS_LITTLE_ENDIAN
#endif

#if defined( __BIG_ENDIAN ) && defined( __LITTLE_ENDIAN )
#  if defined( __BYTE_ORDER ) && __BYTE_ORDER == __BIG_ENDIAN
#    define PLATFORM_BYTE_ORDER IS_BIG_ENDIAN
#  elif defined( __BYTE_ORDER ) && __BYTE_ORDER == __LITTLE_ENDIAN
#    define PLATFORM_BYTE_ORDER IS_LITTLE_ENDIAN
#  endif
#elif defined( __BIG_ENDIAN )
#  define PLATFORM_BYTE_ORDER IS_BIG_ENDIAN
#elif defined( __LITTLE_ENDIAN )
#  define PLATFORM_BYTE_ORDER IS_LITTLE_ENDIAN
#endif

#if defined( __BIG_ENDIAN__ ) && defined( __LITTLE_ENDIAN__ )
#  if defined( __BYTE_ORDER__ ) && __BYTE_ORDER__ == __BIG_ENDIAN__
#    define PLATFORM_BYTE_ORDER IS_BIG_ENDIAN
#  elif defined( __BYTE_ORDER__ ) && __BYTE_ORDER__ == __LITTLE_ENDIAN__
#    define PLATFORM_BYTE_ORDER IS_LITTLE_ENDIAN
#  endif
#elif defined( __BIG_ENDIAN__ )
#  define PLATFORM_BYTE_ORDER IS_BIG_ENDIAN
#elif defined( __LITTLE_ENDIAN__ )
#  define PLATFORM_BYTE_ORDER IS_LITTLE_ENDIAN
#endif

/*  if the platform byte order could not be determined, then try to */
/*  set this define using common machine defines                    */
#if !defined(PLATFORM_BYTE_ORDER)

#if   defined( __alpha__ ) || defined( __alpha ) || defined( i386 )       || \
      defined( __i386__ )  || defined( _M_I86 )  || defined( _M_IX86 )    || \
      defined( __OS2__ )   || defined( sun386 )  || defined( __TURBOC__ ) || \
      defined( vax )       || defined( vms )     || defined( VMS )        || \
      defined( __VMS )     || defined( _M_X64 )
#  define PLATFORM_BYTE_ORDER IS_LITTLE_ENDIAN

#elif defined( AMIGA )   || defined( applec )    || defined( __AS400__ )  || \
      defined( _CRAY )   || defined( __hppa )    || defined( __hp9000 )   || \
      defined( ibm370 )  || defined( mc68000 )   || defined( m68k )       || \
      defined( __MRC__ ) || defined( __MVS__ )   || defined( __MWERKS__ ) || \
      defined( sparc )   || defined( __sparc)    || defined( SYMANTEC_C ) || \
      defined( __VOS__ ) || defined( __TIGCC__ ) || defined( __TANDEM )   || \
      defined( THINK_C ) || defined( __VMCMS__ ) || defined( _AIX )
#  define PLATFORM_BYTE_ORDER IS_BIG_ENDIAN

#elif defined(__arm__)
# ifdef __BIG_ENDIAN
#  define PLATFORM_BYTE_ORDER IS_BIG_ENDIAN
# else
#  define PLATFORM_BYTE_ORDER IS_LITTLE_ENDIAN
# endif
#elif 1     /* **** EDIT HERE IF NECESSARY **** */
#  define PLATFORM_BYTE_ORDER IS_LITTLE_ENDIAN
#elif 0     /* **** EDIT HERE IF NECESSARY **** */
#  define PLATFORM_BYTE_ORDER IS_BIG_ENDIAN
#else
#  error Please edit lines 132 or 134 in brg_endian.h to set the platform byte order
#endif

#endif


    /*
    The Keccak sponge function, designed by Guido Bertoni, Joan Daemen,
    Michaël Peeters and Gilles Van Assche. For more information, feedback or
    questions, please refer to our website: http://keccak.noekeon.org/

    Implementation by the designers,
    hereby denoted as "the implementer".

    To the extent possible under law, the implementer has waived all copyright
    and related or neighboring rights to the source code in this file.
    http://creativecommons.org/publicdomain/zero/1.0/
    */

#define Unrolling 24
#define UseBebigokimisa
    //#define UseSSE
    //#define UseOnlySIMD64
    //#define UseMMX

    typedef unsigned char UINT8;
    typedef unsigned long long int UINT64;

#if defined(UseSSE)
#include <emmintrin.h>
    typedef __m128i V64;
    typedef __m128i V128;
    typedef union
    {
        V128 v128;
        UINT64 v64[2];
    } V6464;

#define ANDnu64(a, b)       _mm_andnot_si128(a, b)
#define LOAD64(a)           _mm_loadl_epi64((const V64 *)&(a))
#define CONST64(a)          _mm_loadl_epi64((const V64 *)&(a))
#define ROL64(a, o)         _mm_or_si128(_mm_slli_epi64(a, o), _mm_srli_epi64(a, 64-(o)))
#define STORE64(a, b)       _mm_storel_epi64((V64 *)&(a), b)
#define XOR64(a, b)         _mm_xor_si128(a, b)
#define XOReq64(a, b)       a = _mm_xor_si128(a, b)

#define ANDnu128(a, b)      _mm_andnot_si128(a, b)
#define LOAD6464(a, b)      _mm_set_epi64((__m64)(a), (__m64)(b))
#define LOAD128(a)          _mm_load_si128((const V128 *)&(a))
#define LOAD128u(a)         _mm_loadu_si128((const V128 *)&(a))
#define ROL64in128(a, o)    _mm_or_si128(_mm_slli_epi64(a, o), _mm_srli_epi64(a, 64-(o)))
#define STORE128(a, b)      _mm_store_si128((V128 *)&(a), b)
#define XOR128(a, b)        _mm_xor_si128(a, b)
#define XOReq128(a, b)      a = _mm_xor_si128(a, b)
#define GET64LO(a, b)       _mm_unpacklo_epi64(a, b)
#define GET64HI(a, b)       _mm_unpackhi_epi64(a, b)
#define COPY64HI2LO(a)      _mm_shuffle_epi32(a, 0xEE)
#define COPY64LO2HI(a)      _mm_shuffle_epi32(a, 0x44)
#define ZERO128()           _mm_setzero_si128()

#ifdef UseOnlySIMD64
#include "KeccakF-1600-simd64.macros"
#else
#include "KeccakF-1600-simd128.macros"
#endif

#ifdef UseBebigokimisa
#error "UseBebigokimisa cannot be used in combination with UseSSE"
#endif
#elif defined(UseMMX)
#include <mmintrin.h>
    typedef __m64 V64;
#define ANDnu64(a, b)       _mm_andnot_si64(a, b)

#if (defined(_MSC_VER) || defined (__INTEL_COMPILER))
#define LOAD64(a)       *(V64*)&(a)
#define CONST64(a)      *(V64*)&(a)
#define STORE64(a, b)   *(V64*)&(a) = b
#else
#define LOAD64(a)       (V64)a
#define CONST64(a)      (V64)a
#define STORE64(a, b)   a = (UINT64)b
#endif
#define ROL64(a, o)         _mm_or_si64(_mm_slli_si64(a, o), _mm_srli_si64(a, 64-(o)))
#define XOR64(a, b)         _mm_xor_si64(a, b)
#define XOReq64(a, b)       a = _mm_xor_si64(a, b)

#include "KeccakF-1600-simd64.macros"

#ifdef UseBebigokimisa
#error "UseBebigokimisa cannot be used in combination with UseMMX"
#endif
#else
#if defined(_MSC_VER)
#define ROL64(a, offset) _rotl64(a, offset)
#else
#define ROL64(a, offset) ((((UINT64)a) << offset) ^ (((UINT64)a) >> (64-offset)))
#endif

const UINT64 KeccakF1600RoundConstants[24] = {
        0x0000000000000001ULL,
        0x0000000000008082ULL,
        0x800000000000808aULL,
        0x8000000080008000ULL,
        0x000000000000808bULL,
        0x0000000080000001ULL,
        0x8000000080008081ULL,
        0x8000000000008009ULL,
        0x000000000000008aULL,
        0x0000000000000088ULL,
        0x0000000080008009ULL,
        0x000000008000000aULL,
        0x000000008000808bULL,
        0x800000000000008bULL,
        0x8000000000008089ULL,
        0x8000000000008003ULL,
        0x8000000000008002ULL,
        0x8000000000000080ULL,
        0x000000000000800aULL,
        0x800000008000000aULL,
        0x8000000080008081ULL,
        0x8000000000008080ULL,
        0x0000000080000001ULL,
        0x8000000080008008ULL };

#define declareABCDE \
    UINT64 Aba, Abe, Abi, Abo, Abu; \
    UINT64 Aga, Age, Agi, Ago, Agu; \
    UINT64 Aka, Ake, Aki, Ako, Aku; \
    UINT64 Ama, Ame, Ami, Amo, Amu; \
    UINT64 Asa, Ase, Asi, Aso, Asu; \
    UINT64 Bba, Bbe, Bbi, Bbo, Bbu; \
    UINT64 Bga, Bge, Bgi, Bgo, Bgu; \
    UINT64 Bka, Bke, Bki, Bko, Bku; \
    UINT64 Bma, Bme, Bmi, Bmo, Bmu; \
    UINT64 Bsa, Bse, Bsi, Bso, Bsu; \
    UINT64 Ca, Ce, Ci, Co, Cu; \
    UINT64 Da, De, Di, Do, Du; \
    UINT64 Eba, Ebe, Ebi, Ebo, Ebu; \
    UINT64 Ega, Ege, Egi, Ego, Egu; \
    UINT64 Eka, Eke, Eki, Eko, Eku; \
    UINT64 Ema, Eme, Emi, Emo, Emu; \
    UINT64 Esa, Ese, Esi, Eso, Esu; \

#define prepareTheta \
    Ca = Aba^Aga^Aka^Ama^Asa; \
    Ce = Abe^Age^Ake^Ame^Ase; \
    Ci = Abi^Agi^Aki^Ami^Asi; \
    Co = Abo^Ago^Ako^Amo^Aso; \
    Cu = Abu^Agu^Aku^Amu^Asu; \

#ifdef UseBebigokimisa
    // --- Code for round, with prepare-theta (lane complementing pattern 'bebigokimisa')
    // --- 64-bit lanes mapped to 64-bit words
#define thetaRhoPiChiIotaPrepareTheta(i, A, E) \
    Da = Cu^ROL64(Ce, 1); \
    De = Ca^ROL64(Ci, 1); \
    Di = Ce^ROL64(Co, 1); \
    Do = Ci^ROL64(Cu, 1); \
    Du = Co^ROL64(Ca, 1); \
    \
    A##ba ^= Da; \
    Bba = A##ba; \
    A##ge ^= De; \
    Bbe = ROL64(A##ge, 44); \
    A##ki ^= Di; \
    Bbi = ROL64(A##ki, 43); \
    A##mo ^= Do; \
    Bbo = ROL64(A##mo, 21); \
    A##su ^= Du; \
    Bbu = ROL64(A##su, 14); \
    E##ba = Bba ^ (Bbe | Bbi); \
    E##ba ^= KeccakF1600RoundConstants[i]; \
    Ca = E##ba; \
    E##be = Bbe ^ ((~Bbi) | Bbo); \
    Ce = E##be; \
    E##bi = Bbi ^ (Bbo &  Bbu); \
    Ci = E##bi; \
    E##bo = Bbo ^ (Bbu | Bba); \
    Co = E##bo; \
    E##bu = Bbu ^ (Bba &  Bbe); \
    Cu = E##bu; \
    \
    A##bo ^= Do; \
    Bga = ROL64(A##bo, 28); \
    A##gu ^= Du; \
    Bge = ROL64(A##gu, 20); \
    A##ka ^= Da; \
    Bgi = ROL64(A##ka, 3); \
    A##me ^= De; \
    Bgo = ROL64(A##me, 45); \
    A##si ^= Di; \
    Bgu = ROL64(A##si, 61); \
    E##ga = Bga ^ (Bge | Bgi); \
    Ca ^= E##ga; \
    E##ge = Bge ^ (Bgi &  Bgo); \
    Ce ^= E##ge; \
    E##gi = Bgi ^ (Bgo | (~Bgu)); \
    Ci ^= E##gi; \
    E##go = Bgo ^ (Bgu | Bga); \
    Co ^= E##go; \
    E##gu = Bgu ^ (Bga &  Bge); \
    Cu ^= E##gu; \
    \
    A##be ^= De; \
    Bka = ROL64(A##be, 1); \
    A##gi ^= Di; \
    Bke = ROL64(A##gi, 6); \
    A##ko ^= Do; \
    Bki = ROL64(A##ko, 25); \
    A##mu ^= Du; \
    Bko = ROL64(A##mu, 8); \
    A##sa ^= Da; \
    Bku = ROL64(A##sa, 18); \
    E##ka = Bka ^ (Bke | Bki); \
    Ca ^= E##ka; \
    E##ke = Bke ^ (Bki &  Bko); \
    Ce ^= E##ke; \
    E##ki = Bki ^ ((~Bko)&  Bku); \
    Ci ^= E##ki; \
    E##ko = (~Bko) ^ (Bku | Bka); \
    Co ^= E##ko; \
    E##ku = Bku ^ (Bka &  Bke); \
    Cu ^= E##ku; \
    \
    A##bu ^= Du; \
    Bma = ROL64(A##bu, 27); \
    A##ga ^= Da; \
    Bme = ROL64(A##ga, 36); \
    A##ke ^= De; \
    Bmi = ROL64(A##ke, 10); \
    A##mi ^= Di; \
    Bmo = ROL64(A##mi, 15); \
    A##so ^= Do; \
    Bmu = ROL64(A##so, 56); \
    E##ma = Bma ^ (Bme &  Bmi); \
    Ca ^= E##ma; \
    E##me = Bme ^ (Bmi | Bmo); \
    Ce ^= E##me; \
    E##mi = Bmi ^ ((~Bmo) | Bmu); \
    Ci ^= E##mi; \
    E##mo = (~Bmo) ^ (Bmu &  Bma); \
    Co ^= E##mo; \
    E##mu = Bmu ^ (Bma | Bme); \
    Cu ^= E##mu; \
    \
    A##bi ^= Di; \
    Bsa = ROL64(A##bi, 62); \
    A##go ^= Do; \
    Bse = ROL64(A##go, 55); \
    A##ku ^= Du; \
    Bsi = ROL64(A##ku, 39); \
    A##ma ^= Da; \
    Bso = ROL64(A##ma, 41); \
    A##se ^= De; \
    Bsu = ROL64(A##se, 2); \
    E##sa = Bsa ^ ((~Bse)&  Bsi); \
    Ca ^= E##sa; \
    E##se = (~Bse) ^ (Bsi | Bso); \
    Ce ^= E##se; \
    E##si = Bsi ^ (Bso &  Bsu); \
    Ci ^= E##si; \
    E##so = Bso ^ (Bsu | Bsa); \
    Co ^= E##so; \
    E##su = Bsu ^ (Bsa &  Bse); \
    Cu ^= E##su; \
    \

    // --- Code for round (lane complementing pattern 'bebigokimisa')
    // --- 64-bit lanes mapped to 64-bit words
#define thetaRhoPiChiIota(i, A, E) \
    Da = Cu^ROL64(Ce, 1); \
    De = Ca^ROL64(Ci, 1); \
    Di = Ce^ROL64(Co, 1); \
    Do = Ci^ROL64(Cu, 1); \
    Du = Co^ROL64(Ca, 1); \
    \
    A##ba ^= Da; \
    Bba = A##ba; \
    A##ge ^= De; \
    Bbe = ROL64(A##ge, 44); \
    A##ki ^= Di; \
    Bbi = ROL64(A##ki, 43); \
    A##mo ^= Do; \
    Bbo = ROL64(A##mo, 21); \
    A##su ^= Du; \
    Bbu = ROL64(A##su, 14); \
    E##ba = Bba ^ (Bbe | Bbi); \
    E##ba ^= KeccakF1600RoundConstants[i]; \
    E##be = Bbe ^ ((~Bbi) | Bbo); \
    E##bi = Bbi ^ (Bbo &  Bbu); \
    E##bo = Bbo ^ (Bbu | Bba); \
    E##bu = Bbu ^ (Bba &  Bbe); \
    \
    A##bo ^= Do; \
    Bga = ROL64(A##bo, 28); \
    A##gu ^= Du; \
    Bge = ROL64(A##gu, 20); \
    A##ka ^= Da; \
    Bgi = ROL64(A##ka, 3); \
    A##me ^= De; \
    Bgo = ROL64(A##me, 45); \
    A##si ^= Di; \
    Bgu = ROL64(A##si, 61); \
    E##ga = Bga ^ (Bge | Bgi); \
    E##ge = Bge ^ (Bgi &  Bgo); \
    E##gi = Bgi ^ (Bgo | (~Bgu)); \
    E##go = Bgo ^ (Bgu | Bga); \
    E##gu = Bgu ^ (Bga &  Bge); \
    \
    A##be ^= De; \
    Bka = ROL64(A##be, 1); \
    A##gi ^= Di; \
    Bke = ROL64(A##gi, 6); \
    A##ko ^= Do; \
    Bki = ROL64(A##ko, 25); \
    A##mu ^= Du; \
    Bko = ROL64(A##mu, 8); \
    A##sa ^= Da; \
    Bku = ROL64(A##sa, 18); \
    E##ka = Bka ^ (Bke | Bki); \
    E##ke = Bke ^ (Bki &  Bko); \
    E##ki = Bki ^ ((~Bko)&  Bku); \
    E##ko = (~Bko) ^ (Bku | Bka); \
    E##ku = Bku ^ (Bka &  Bke); \
    \
    A##bu ^= Du; \
    Bma = ROL64(A##bu, 27); \
    A##ga ^= Da; \
    Bme = ROL64(A##ga, 36); \
    A##ke ^= De; \
    Bmi = ROL64(A##ke, 10); \
    A##mi ^= Di; \
    Bmo = ROL64(A##mi, 15); \
    A##so ^= Do; \
    Bmu = ROL64(A##so, 56); \
    E##ma = Bma ^ (Bme &  Bmi); \
    E##me = Bme ^ (Bmi | Bmo); \
    E##mi = Bmi ^ ((~Bmo) | Bmu); \
    E##mo = (~Bmo) ^ (Bmu &  Bma); \
    E##mu = Bmu ^ (Bma | Bme); \
    \
    A##bi ^= Di; \
    Bsa = ROL64(A##bi, 62); \
    A##go ^= Do; \
    Bse = ROL64(A##go, 55); \
    A##ku ^= Du; \
    Bsi = ROL64(A##ku, 39); \
    A##ma ^= Da; \
    Bso = ROL64(A##ma, 41); \
    A##se ^= De; \
    Bsu = ROL64(A##se, 2); \
    E##sa = Bsa ^ ((~Bse)&  Bsi); \
    E##se = (~Bse) ^ (Bsi | Bso); \
    E##si = Bsi ^ (Bso &  Bsu); \
    E##so = Bso ^ (Bsu | Bsa); \
    E##su = Bsu ^ (Bsa &  Bse); \
    \

#else // UseBebigokimisa
    // --- Code for round, with prepare-theta
    // --- 64-bit lanes mapped to 64-bit words
#define thetaRhoPiChiIotaPrepareTheta(i, A, E) \
    Da = Cu^ROL64(Ce, 1); \
    De = Ca^ROL64(Ci, 1); \
    Di = Ce^ROL64(Co, 1); \
    Do = Ci^ROL64(Cu, 1); \
    Du = Co^ROL64(Ca, 1); \
    \
    A##ba ^= Da; \
    Bba = A##ba; \
    A##ge ^= De; \
    Bbe = ROL64(A##ge, 44); \
    A##ki ^= Di; \
    Bbi = ROL64(A##ki, 43); \
    A##mo ^= Do; \
    Bbo = ROL64(A##mo, 21); \
    A##su ^= Du; \
    Bbu = ROL64(A##su, 14); \
    E##ba = Bba ^ ((~Bbe)&  Bbi); \
    E##ba ^= KeccakF1600RoundConstants[i]; \
    Ca = E##ba; \
    E##be = Bbe ^ ((~Bbi)&  Bbo); \
    Ce = E##be; \
    E##bi = Bbi ^ ((~Bbo)&  Bbu); \
    Ci = E##bi; \
    E##bo = Bbo ^ ((~Bbu)&  Bba); \
    Co = E##bo; \
    E##bu = Bbu ^ ((~Bba)&  Bbe); \
    Cu = E##bu; \
    \
    A##bo ^= Do; \
    Bga = ROL64(A##bo, 28); \
    A##gu ^= Du; \
    Bge = ROL64(A##gu, 20); \
    A##ka ^= Da; \
    Bgi = ROL64(A##ka, 3); \
    A##me ^= De; \
    Bgo = ROL64(A##me, 45); \
    A##si ^= Di; \
    Bgu = ROL64(A##si, 61); \
    E##ga = Bga ^ ((~Bge)&  Bgi); \
    Ca ^= E##ga; \
    E##ge = Bge ^ ((~Bgi)&  Bgo); \
    Ce ^= E##ge; \
    E##gi = Bgi ^ ((~Bgo)&  Bgu); \
    Ci ^= E##gi; \
    E##go = Bgo ^ ((~Bgu)&  Bga); \
    Co ^= E##go; \
    E##gu = Bgu ^ ((~Bga)&  Bge); \
    Cu ^= E##gu; \
    \
    A##be ^= De; \
    Bka = ROL64(A##be, 1); \
    A##gi ^= Di; \
    Bke = ROL64(A##gi, 6); \
    A##ko ^= Do; \
    Bki = ROL64(A##ko, 25); \
    A##mu ^= Du; \
    Bko = ROL64(A##mu, 8); \
    A##sa ^= Da; \
    Bku = ROL64(A##sa, 18); \
    E##ka = Bka ^ ((~Bke)&  Bki); \
    Ca ^= E##ka; \
    E##ke = Bke ^ ((~Bki)&  Bko); \
    Ce ^= E##ke; \
    E##ki = Bki ^ ((~Bko)&  Bku); \
    Ci ^= E##ki; \
    E##ko = Bko ^ ((~Bku)&  Bka); \
    Co ^= E##ko; \
    E##ku = Bku ^ ((~Bka)&  Bke); \
    Cu ^= E##ku; \
    \
    A##bu ^= Du; \
    Bma = ROL64(A##bu, 27); \
    A##ga ^= Da; \
    Bme = ROL64(A##ga, 36); \
    A##ke ^= De; \
    Bmi = ROL64(A##ke, 10); \
    A##mi ^= Di; \
    Bmo = ROL64(A##mi, 15); \
    A##so ^= Do; \
    Bmu = ROL64(A##so, 56); \
    E##ma = Bma ^ ((~Bme)&  Bmi); \
    Ca ^= E##ma; \
    E##me = Bme ^ ((~Bmi)&  Bmo); \
    Ce ^= E##me; \
    E##mi = Bmi ^ ((~Bmo)&  Bmu); \
    Ci ^= E##mi; \
    E##mo = Bmo ^ ((~Bmu)&  Bma); \
    Co ^= E##mo; \
    E##mu = Bmu ^ ((~Bma)&  Bme); \
    Cu ^= E##mu; \
    \
    A##bi ^= Di; \
    Bsa = ROL64(A##bi, 62); \
    A##go ^= Do; \
    Bse = ROL64(A##go, 55); \
    A##ku ^= Du; \
    Bsi = ROL64(A##ku, 39); \
    A##ma ^= Da; \
    Bso = ROL64(A##ma, 41); \
    A##se ^= De; \
    Bsu = ROL64(A##se, 2); \
    E##sa = Bsa ^ ((~Bse)&  Bsi); \
    Ca ^= E##sa; \
    E##se = Bse ^ ((~Bsi)&  Bso); \
    Ce ^= E##se; \
    E##si = Bsi ^ ((~Bso)&  Bsu); \
    Ci ^= E##si; \
    E##so = Bso ^ ((~Bsu)&  Bsa); \
    Co ^= E##so; \
    E##su = Bsu ^ ((~Bsa)&  Bse); \
    Cu ^= E##su; \
    \

    // --- Code for round
    // --- 64-bit lanes mapped to 64-bit words
#define thetaRhoPiChiIota(i, A, E) \
    Da = Cu^ROL64(Ce, 1); \
    De = Ca^ROL64(Ci, 1); \
    Di = Ce^ROL64(Co, 1); \
    Do = Ci^ROL64(Cu, 1); \
    Du = Co^ROL64(Ca, 1); \
    \
    A##ba ^= Da; \
    Bba = A##ba; \
    A##ge ^= De; \
    Bbe = ROL64(A##ge, 44); \
    A##ki ^= Di; \
    Bbi = ROL64(A##ki, 43); \
    A##mo ^= Do; \
    Bbo = ROL64(A##mo, 21); \
    A##su ^= Du; \
    Bbu = ROL64(A##su, 14); \
    E##ba = Bba ^ ((~Bbe)&  Bbi); \
    E##ba ^= KeccakF1600RoundConstants[i]; \
    E##be = Bbe ^ ((~Bbi)&  Bbo); \
    E##bi = Bbi ^ ((~Bbo)&  Bbu); \
    E##bo = Bbo ^ ((~Bbu)&  Bba); \
    E##bu = Bbu ^ ((~Bba)&  Bbe); \
    \
    A##bo ^= Do; \
    Bga = ROL64(A##bo, 28); \
    A##gu ^= Du; \
    Bge = ROL64(A##gu, 20); \
    A##ka ^= Da; \
    Bgi = ROL64(A##ka, 3); \
    A##me ^= De; \
    Bgo = ROL64(A##me, 45); \
    A##si ^= Di; \
    Bgu = ROL64(A##si, 61); \
    E##ga = Bga ^ ((~Bge)&  Bgi); \
    E##ge = Bge ^ ((~Bgi)&  Bgo); \
    E##gi = Bgi ^ ((~Bgo)&  Bgu); \
    E##go = Bgo ^ ((~Bgu)&  Bga); \
    E##gu = Bgu ^ ((~Bga)&  Bge); \
    \
    A##be ^= De; \
    Bka = ROL64(A##be, 1); \
    A##gi ^= Di; \
    Bke = ROL64(A##gi, 6); \
    A##ko ^= Do; \
    Bki = ROL64(A##ko, 25); \
    A##mu ^= Du; \
    Bko = ROL64(A##mu, 8); \
    A##sa ^= Da; \
    Bku = ROL64(A##sa, 18); \
    E##ka = Bka ^ ((~Bke)&  Bki); \
    E##ke = Bke ^ ((~Bki)&  Bko); \
    E##ki = Bki ^ ((~Bko)&  Bku); \
    E##ko = Bko ^ ((~Bku)&  Bka); \
    E##ku = Bku ^ ((~Bka)&  Bke); \
    \
    A##bu ^= Du; \
    Bma = ROL64(A##bu, 27); \
    A##ga ^= Da; \
    Bme = ROL64(A##ga, 36); \
    A##ke ^= De; \
    Bmi = ROL64(A##ke, 10); \
    A##mi ^= Di; \
    Bmo = ROL64(A##mi, 15); \
    A##so ^= Do; \
    Bmu = ROL64(A##so, 56); \
    E##ma = Bma ^ ((~Bme)&  Bmi); \
    E##me = Bme ^ ((~Bmi)&  Bmo); \
    E##mi = Bmi ^ ((~Bmo)&  Bmu); \
    E##mo = Bmo ^ ((~Bmu)&  Bma); \
    E##mu = Bmu ^ ((~Bma)&  Bme); \
    \
    A##bi ^= Di; \
    Bsa = ROL64(A##bi, 62); \
    A##go ^= Do; \
    Bse = ROL64(A##go, 55); \
    A##ku ^= Du; \
    Bsi = ROL64(A##ku, 39); \
    A##ma ^= Da; \
    Bso = ROL64(A##ma, 41); \
    A##se ^= De; \
    Bsu = ROL64(A##se, 2); \
    E##sa = Bsa ^ ((~Bse)&  Bsi); \
    E##se = Bse ^ ((~Bsi)&  Bso); \
    E##si = Bsi ^ ((~Bso)&  Bsu); \
    E##so = Bso ^ ((~Bsu)&  Bsa); \
    E##su = Bsu ^ ((~Bsa)&  Bse); \
    \

#endif // UseBebigokimisa

#define copyFromStateAndXor576bits(X, state, input) \
    X##ba = state[0] ^ input[0]; \
    X##be = state[1] ^ input[1]; \
    X##bi = state[2] ^ input[2]; \
    X##bo = state[3] ^ input[3]; \
    X##bu = state[4] ^ input[4]; \
    X##ga = state[5] ^ input[5]; \
    X##ge = state[6] ^ input[6]; \
    X##gi = state[7] ^ input[7]; \
    X##go = state[8] ^ input[8]; \
    X##gu = state[9]; \
    X##ka = state[10]; \
    X##ke = state[11]; \
    X##ki = state[12]; \
    X##ko = state[13]; \
    X##ku = state[14]; \
    X##ma = state[15]; \
    X##me = state[16]; \
    X##mi = state[17]; \
    X##mo = state[18]; \
    X##mu = state[19]; \
    X##sa = state[20]; \
    X##se = state[21]; \
    X##si = state[22]; \
    X##so = state[23]; \
    X##su = state[24]; \

#define copyFromStateAndXor832bits(X, state, input) \
    X##ba = state[0] ^ input[0]; \
    X##be = state[1] ^ input[1]; \
    X##bi = state[2] ^ input[2]; \
    X##bo = state[3] ^ input[3]; \
    X##bu = state[4] ^ input[4]; \
    X##ga = state[5] ^ input[5]; \
    X##ge = state[6] ^ input[6]; \
    X##gi = state[7] ^ input[7]; \
    X##go = state[8] ^ input[8]; \
    X##gu = state[9] ^ input[9]; \
    X##ka = state[10] ^ input[10]; \
    X##ke = state[11] ^ input[11]; \
    X##ki = state[12] ^ input[12]; \
    X##ko = state[13]; \
    X##ku = state[14]; \
    X##ma = state[15]; \
    X##me = state[16]; \
    X##mi = state[17]; \
    X##mo = state[18]; \
    X##mu = state[19]; \
    X##sa = state[20]; \
    X##se = state[21]; \
    X##si = state[22]; \
    X##so = state[23]; \
    X##su = state[24]; \

#define copyFromStateAndXor1024bits(X, state, input) \
    X##ba = state[0] ^ input[0]; \
    X##be = state[1] ^ input[1]; \
    X##bi = state[2] ^ input[2]; \
    X##bo = state[3] ^ input[3]; \
    X##bu = state[4] ^ input[4]; \
    X##ga = state[5] ^ input[5]; \
    X##ge = state[6] ^ input[6]; \
    X##gi = state[7] ^ input[7]; \
    X##go = state[8] ^ input[8]; \
    X##gu = state[9] ^ input[9]; \
    X##ka = state[10] ^ input[10]; \
    X##ke = state[11] ^ input[11]; \
    X##ki = state[12] ^ input[12]; \
    X##ko = state[13] ^ input[13]; \
    X##ku = state[14] ^ input[14]; \
    X##ma = state[15] ^ input[15]; \
    X##me = state[16]; \
    X##mi = state[17]; \
    X##mo = state[18]; \
    X##mu = state[19]; \
    X##sa = state[20]; \
    X##se = state[21]; \
    X##si = state[22]; \
    X##so = state[23]; \
    X##su = state[24]; \

#define copyFromStateAndXor1088bits(X, state, input) \
    X##ba = state[0] ^ input[0]; \
    X##be = state[1] ^ input[1]; \
    X##bi = state[2] ^ input[2]; \
    X##bo = state[3] ^ input[3]; \
    X##bu = state[4] ^ input[4]; \
    X##ga = state[5] ^ input[5]; \
    X##ge = state[6] ^ input[6]; \
    X##gi = state[7] ^ input[7]; \
    X##go = state[8] ^ input[8]; \
    X##gu = state[9] ^ input[9]; \
    X##ka = state[10] ^ input[10]; \
    X##ke = state[11] ^ input[11]; \
    X##ki = state[12] ^ input[12]; \
    X##ko = state[13] ^ input[13]; \
    X##ku = state[14] ^ input[14]; \
    X##ma = state[15] ^ input[15]; \
    X##me = state[16] ^ input[16]; \
    X##mi = state[17]; \
    X##mo = state[18]; \
    X##mu = state[19]; \
    X##sa = state[20]; \
    X##se = state[21]; \
    X##si = state[22]; \
    X##so = state[23]; \
    X##su = state[24]; \

#define copyFromStateAndXor1152bits(X, state, input) \
    X##ba = state[0] ^ input[0]; \
    X##be = state[1] ^ input[1]; \
    X##bi = state[2] ^ input[2]; \
    X##bo = state[3] ^ input[3]; \
    X##bu = state[4] ^ input[4]; \
    X##ga = state[5] ^ input[5]; \
    X##ge = state[6] ^ input[6]; \
    X##gi = state[7] ^ input[7]; \
    X##go = state[8] ^ input[8]; \
    X##gu = state[9] ^ input[9]; \
    X##ka = state[10] ^ input[10]; \
    X##ke = state[11] ^ input[11]; \
    X##ki = state[12] ^ input[12]; \
    X##ko = state[13] ^ input[13]; \
    X##ku = state[14] ^ input[14]; \
    X##ma = state[15] ^ input[15]; \
    X##me = state[16] ^ input[16]; \
    X##mi = state[17] ^ input[17]; \
    X##mo = state[18]; \
    X##mu = state[19]; \
    X##sa = state[20]; \
    X##se = state[21]; \
    X##si = state[22]; \
    X##so = state[23]; \
    X##su = state[24]; \

#define copyFromStateAndXor1344bits(X, state, input) \
    X##ba = state[0] ^ input[0]; \
    X##be = state[1] ^ input[1]; \
    X##bi = state[2] ^ input[2]; \
    X##bo = state[3] ^ input[3]; \
    X##bu = state[4] ^ input[4]; \
    X##ga = state[5] ^ input[5]; \
    X##ge = state[6] ^ input[6]; \
    X##gi = state[7] ^ input[7]; \
    X##go = state[8] ^ input[8]; \
    X##gu = state[9] ^ input[9]; \
    X##ka = state[10] ^ input[10]; \
    X##ke = state[11] ^ input[11]; \
    X##ki = state[12] ^ input[12]; \
    X##ko = state[13] ^ input[13]; \
    X##ku = state[14] ^ input[14]; \
    X##ma = state[15] ^ input[15]; \
    X##me = state[16] ^ input[16]; \
    X##mi = state[17] ^ input[17]; \
    X##mo = state[18] ^ input[18]; \
    X##mu = state[19] ^ input[19]; \
    X##sa = state[20] ^ input[20]; \
    X##se = state[21]; \
    X##si = state[22]; \
    X##so = state[23]; \
    X##su = state[24]; \

#define copyFromState(X, state) \
    X##ba = state[0]; \
    X##be = state[1]; \
    X##bi = state[2]; \
    X##bo = state[3]; \
    X##bu = state[4]; \
    X##ga = state[5]; \
    X##ge = state[6]; \
    X##gi = state[7]; \
    X##go = state[8]; \
    X##gu = state[9]; \
    X##ka = state[10]; \
    X##ke = state[11]; \
    X##ki = state[12]; \
    X##ko = state[13]; \
    X##ku = state[14]; \
    X##ma = state[15]; \
    X##me = state[16]; \
    X##mi = state[17]; \
    X##mo = state[18]; \
    X##mu = state[19]; \
    X##sa = state[20]; \
    X##se = state[21]; \
    X##si = state[22]; \
    X##so = state[23]; \
    X##su = state[24]; \

#define copyToState(state, X) \
    state[0] = X##ba; \
    state[1] = X##be; \
    state[2] = X##bi; \
    state[3] = X##bo; \
    state[4] = X##bu; \
    state[5] = X##ga; \
    state[6] = X##ge; \
    state[7] = X##gi; \
    state[8] = X##go; \
    state[9] = X##gu; \
    state[10] = X##ka; \
    state[11] = X##ke; \
    state[12] = X##ki; \
    state[13] = X##ko; \
    state[14] = X##ku; \
    state[15] = X##ma; \
    state[16] = X##me; \
    state[17] = X##mi; \
    state[18] = X##mo; \
    state[19] = X##mu; \
    state[20] = X##sa; \
    state[21] = X##se; \
    state[22] = X##si; \
    state[23] = X##so; \
    state[24] = X##su; \

#define copyStateVariables(X, Y) \
    X##ba = Y##ba; \
    X##be = Y##be; \
    X##bi = Y##bi; \
    X##bo = Y##bo; \
    X##bu = Y##bu; \
    X##ga = Y##ga; \
    X##ge = Y##ge; \
    X##gi = Y##gi; \
    X##go = Y##go; \
    X##gu = Y##gu; \
    X##ka = Y##ka; \
    X##ke = Y##ke; \
    X##ki = Y##ki; \
    X##ko = Y##ko; \
    X##ku = Y##ku; \
    X##ma = Y##ma; \
    X##me = Y##me; \
    X##mi = Y##mi; \
    X##mo = Y##mo; \
    X##mu = Y##mu; \
    X##sa = Y##sa; \
    X##se = Y##se; \
    X##si = Y##si; \
    X##so = Y##so; \
    X##su = Y##su; \

#endif

/*
The Keccak sponge function, designed by Guido Bertoni, Joan Daemen,
Michaël Peeters and Gilles Van Assche. For more information, feedback or
questions, please refer to our website: http://keccak.noekeon.org/

Implementation by the designers,
hereby denoted as "the implementer".

To the extent possible under law, the implementer has waived all copyright
and related or neighboring rights to the source code in this file.
http://creativecommons.org/publicdomain/zero/1.0/
*/

#if (Unrolling == 24)
#define rounds \
    prepareTheta \
    thetaRhoPiChiIotaPrepareTheta(0, A, E) \
    thetaRhoPiChiIotaPrepareTheta(1, E, A) \
    thetaRhoPiChiIotaPrepareTheta(2, A, E) \
    thetaRhoPiChiIotaPrepareTheta(3, E, A) \
    thetaRhoPiChiIotaPrepareTheta(4, A, E) \
    thetaRhoPiChiIotaPrepareTheta(5, E, A) \
    thetaRhoPiChiIotaPrepareTheta(6, A, E) \
    thetaRhoPiChiIotaPrepareTheta(7, E, A) \
    thetaRhoPiChiIotaPrepareTheta(8, A, E) \
    thetaRhoPiChiIotaPrepareTheta(9, E, A) \
    thetaRhoPiChiIotaPrepareTheta(10, A, E) \
    thetaRhoPiChiIotaPrepareTheta(11, E, A) \
    thetaRhoPiChiIotaPrepareTheta(12, A, E) \
    thetaRhoPiChiIotaPrepareTheta(13, E, A) \
    thetaRhoPiChiIotaPrepareTheta(14, A, E) \
    thetaRhoPiChiIotaPrepareTheta(15, E, A) \
    thetaRhoPiChiIotaPrepareTheta(16, A, E) \
    thetaRhoPiChiIotaPrepareTheta(17, E, A) \
    thetaRhoPiChiIotaPrepareTheta(18, A, E) \
    thetaRhoPiChiIotaPrepareTheta(19, E, A) \
    thetaRhoPiChiIotaPrepareTheta(20, A, E) \
    thetaRhoPiChiIotaPrepareTheta(21, E, A) \
    thetaRhoPiChiIotaPrepareTheta(22, A, E) \
    thetaRhoPiChiIota(23, E, A) \
    copyToState(state, A)
#elif (Unrolling == 12)
#define rounds \
    prepareTheta \
for (i = 0; i<24; i += 12)
 {
    \
    thetaRhoPiChiIotaPrepareTheta(i, A, E) \
    thetaRhoPiChiIotaPrepareTheta(i + 1, E, A) \
    thetaRhoPiChiIotaPrepareTheta(i + 2, A, E) \
    thetaRhoPiChiIotaPrepareTheta(i + 3, E, A) \
    thetaRhoPiChiIotaPrepareTheta(i + 4, A, E) \
    thetaRhoPiChiIotaPrepareTheta(i + 5, E, A) \
    thetaRhoPiChiIotaPrepareTheta(i + 6, A, E) \
    thetaRhoPiChiIotaPrepareTheta(i + 7, E, A) \
    thetaRhoPiChiIotaPrepareTheta(i + 8, A, E) \
    thetaRhoPiChiIotaPrepareTheta(i + 9, E, A) \
    thetaRhoPiChiIotaPrepareTheta(i + 10, A, E) \
    thetaRhoPiChiIotaPrepareTheta(i + 11, E, A) \
} \
    copyToState(state, A)
#elif (Unrolling == 8)
#define rounds \
    prepareTheta \
for (i = 0; i<24; i += 8)
 {
    \
    thetaRhoPiChiIotaPrepareTheta(i, A, E) \
    thetaRhoPiChiIotaPrepareTheta(i + 1, E, A) \
    thetaRhoPiChiIotaPrepareTheta(i + 2, A, E) \
    thetaRhoPiChiIotaPrepareTheta(i + 3, E, A) \
    thetaRhoPiChiIotaPrepareTheta(i + 4, A, E) \
    thetaRhoPiChiIotaPrepareTheta(i + 5, E, A) \
    thetaRhoPiChiIotaPrepareTheta(i + 6, A, E) \
    thetaRhoPiChiIotaPrepareTheta(i + 7, E, A) \
} \
    copyToState(state, A)
#elif (Unrolling == 6)
#define rounds \
    prepareTheta \
for (i = 0; i<24; i += 6)
 {
    \
    thetaRhoPiChiIotaPrepareTheta(i, A, E) \
    thetaRhoPiChiIotaPrepareTheta(i + 1, E, A) \
    thetaRhoPiChiIotaPrepareTheta(i + 2, A, E) \
    thetaRhoPiChiIotaPrepareTheta(i + 3, E, A) \
    thetaRhoPiChiIotaPrepareTheta(i + 4, A, E) \
    thetaRhoPiChiIotaPrepareTheta(i + 5, E, A) \
} \
    copyToState(state, A)
#elif (Unrolling == 4)
#define rounds \
    prepareTheta \
for (i = 0; i<24; i += 4)
 {
    \
    thetaRhoPiChiIotaPrepareTheta(i, A, E) \
    thetaRhoPiChiIotaPrepareTheta(i + 1, E, A) \
    thetaRhoPiChiIotaPrepareTheta(i + 2, A, E) \
    thetaRhoPiChiIotaPrepareTheta(i + 3, E, A) \
} \
    copyToState(state, A)
#elif (Unrolling == 3)
#define rounds \
    prepareTheta \
for (i = 0; i<24; i += 3)
 {
    \
    thetaRhoPiChiIotaPrepareTheta(i, A, E) \
    thetaRhoPiChiIotaPrepareTheta(i + 1, E, A) \
    thetaRhoPiChiIotaPrepareTheta(i + 2, A, E) \
    copyStateVariables(A, E) \
} \
    copyToState(state, A)
#elif (Unrolling == 2)
#define rounds \
    prepareTheta \
for (i = 0; i<24; i += 2)
 {
    \
    thetaRhoPiChiIotaPrepareTheta(i, A, E) \
    thetaRhoPiChiIotaPrepareTheta(i + 1, E, A) \
} \
    copyToState(state, A)
#elif (Unrolling == 1)
#define rounds \
    prepareTheta \
for (i = 0; i<24; i++)
 {
    \
    thetaRhoPiChiIotaPrepareTheta(i, A, E) \
    copyStateVariables(A, E) \
} \
    copyToState(state, A)
#else
#error "Unrolling is not correctly specified!"
#endif


    void KeccakPermutationOnWords(UINT64 *state)
    {
        declareABCDE
#if (Unrolling != 24)
            unsigned int i;
#endif

        copyFromState(A, state)
            rounds
#if defined(UseMMX)
            _mm_empty();
#endif
    }

    void KeccakPermutationOnWordsAfterXoring(UINT64 *state, const UINT64 *input, unsigned int laneCount)
    {
        declareABCDE
#if (Unrolling != 24)
            unsigned int i;
#endif
        unsigned int j;

        for (j = 0; j<laneCount; j++)
            state[j] ^= input[j];
        copyFromState(A, state)
            rounds
#if defined(UseMMX)
            _mm_empty();
#endif
    }

#ifdef ProvideFast576
    void KeccakPermutationOnWordsAfterXoring576bits(UINT64 *state, const UINT64 *input)
    {
        declareABCDE
#if (Unrolling != 24)
            unsigned int i;
#endif

        copyFromStateAndXor576bits(A, state, input)
            rounds
#if defined(UseMMX)
            _mm_empty();
#endif
    }
#endif

#ifdef ProvideFast832
    void KeccakPermutationOnWordsAfterXoring832bits(UINT64 *state, const UINT64 *input)
    {
        declareABCDE
#if (Unrolling != 24)
            unsigned int i;
#endif

        copyFromStateAndXor832bits(A, state, input)
            rounds
#if defined(UseMMX)
            _mm_empty();
#endif
    }
#endif

#ifdef ProvideFast1024
    void KeccakPermutationOnWordsAfterXoring1024bits(UINT64 *state, const UINT64 *input)
    {
        declareABCDE
#if (Unrolling != 24)
            unsigned int i;
#endif

        copyFromStateAndXor1024bits(A, state, input)
            rounds
#if defined(UseMMX)
            _mm_empty();
#endif
    }
#endif

#ifdef ProvideFast1088
    void KeccakPermutationOnWordsAfterXoring1088bits(UINT64 *state, const UINT64 *input)
    {
        declareABCDE
#if (Unrolling != 24)
            unsigned int i;
#endif

        copyFromStateAndXor1088bits(A, state, input)
            rounds
#if defined(UseMMX)
            _mm_empty();
#endif
    }
#endif

#ifdef ProvideFast1152
    void KeccakPermutationOnWordsAfterXoring1152bits(UINT64 *state, const UINT64 *input)
    {
        declareABCDE
#if (Unrolling != 24)
            unsigned int i;
#endif

        copyFromStateAndXor1152bits(A, state, input)
            rounds
#if defined(UseMMX)
            _mm_empty();
#endif
    }
#endif

#ifdef ProvideFast1344
    void KeccakPermutationOnWordsAfterXoring1344bits(UINT64 *state, const UINT64 *input)
    {
        declareABCDE
#if (Unrolling != 24)
            unsigned int i;
#endif

        copyFromStateAndXor1344bits(A, state, input)
            rounds
#if defined(UseMMX)
            _mm_empty();
#endif
    }
#endif

    void KeccakInitialize()
    {
    }

    void KeccakInitializeState(unsigned char *state)
    {
        memset(state, 0, 200);
#ifdef UseBebigokimisa
        ((UINT64*)state)[1] = ~(UINT64)0;
        ((UINT64*)state)[2] = ~(UINT64)0;
        ((UINT64*)state)[8] = ~(UINT64)0;
        ((UINT64*)state)[12] = ~(UINT64)0;
        ((UINT64*)state)[17] = ~(UINT64)0;
        ((UINT64*)state)[20] = ~(UINT64)0;
#endif
    }

    void KeccakPermutation(unsigned char *state)
    {
        // We assume the state is always stored as words
        KeccakPermutationOnWords((UINT64*)state);
    }

    void fromBytesToWord(UINT64 *word, const UINT8 *bytes)
    {
        unsigned int i;

        *word = 0;
        for (i = 0; i<(64 / 8); i++)
            *word |= (UINT64)(bytes[i]) << (8 * i);
    }

#ifdef ProvideFast576
    void KeccakAbsorb576bits(unsigned char *state, const unsigned char *data)
    {
#if (PLATFORM_BYTE_ORDER == IS_LITTLE_ENDIAN)
        KeccakPermutationOnWordsAfterXoring576bits((UINT64*)state, (const UINT64*)data);
#else
        UINT64 dataAsWords[9];
        unsigned int i;

        for (i = 0; i<9; i++)
            fromBytesToWord(dataAsWords + i, data + (i * 8));
        KeccakPermutationOnWordsAfterXoring576bits((UINT64*)state, dataAsWords);
#endif
    }
#endif

#ifdef ProvideFast832
    void KeccakAbsorb832bits(unsigned char *state, const unsigned char *data)
    {
#if (PLATFORM_BYTE_ORDER == IS_LITTLE_ENDIAN)
        KeccakPermutationOnWordsAfterXoring832bits((UINT64*)state, (const UINT64*)data);
#else
        UINT64 dataAsWords[13];
        unsigned int i;

        for (i = 0; i<13; i++)
            fromBytesToWord(dataAsWords + i, data + (i * 8));
        KeccakPermutationOnWordsAfterXoring832bits((UINT64*)state, dataAsWords);
#endif
    }
#endif

#ifdef ProvideFast1024
    void KeccakAbsorb1024bits(unsigned char *state, const unsigned char *data)
    {
#if (PLATFORM_BYTE_ORDER == IS_LITTLE_ENDIAN)
        KeccakPermutationOnWordsAfterXoring1024bits((UINT64*)state, (const UINT64*)data);
#else
        UINT64 dataAsWords[16];
        unsigned int i;

        for (i = 0; i<16; i++)
            fromBytesToWord(dataAsWords + i, data + (i * 8));
        KeccakPermutationOnWordsAfterXoring1024bits((UINT64*)state, dataAsWords);
#endif
    }
#endif

#ifdef ProvideFast1088
    void KeccakAbsorb1088bits(unsigned char *state, const unsigned char *data)
    {
#if (PLATFORM_BYTE_ORDER == IS_LITTLE_ENDIAN)
        KeccakPermutationOnWordsAfterXoring1088bits((UINT64*)state, (const UINT64*)data);
#else
        UINT64 dataAsWords[17];
        unsigned int i;

        for (i = 0; i<17; i++)
            fromBytesToWord(dataAsWords + i, data + (i * 8));
        KeccakPermutationOnWordsAfterXoring1088bits((UINT64*)state, dataAsWords);
#endif
    }
#endif

#ifdef ProvideFast1152
    void KeccakAbsorb1152bits(unsigned char *state, const unsigned char *data)
    {
#if (PLATFORM_BYTE_ORDER == IS_LITTLE_ENDIAN)
        KeccakPermutationOnWordsAfterXoring1152bits((UINT64*)state, (const UINT64*)data);
#else
        UINT64 dataAsWords[18];
        unsigned int i;

        for (i = 0; i<18; i++)
            fromBytesToWord(dataAsWords + i, data + (i * 8));
        KeccakPermutationOnWordsAfterXoring1152bits((UINT64*)state, dataAsWords);
#endif
    }
#endif

#ifdef ProvideFast1344
    void KeccakAbsorb1344bits(unsigned char *state, const unsigned char *data)
    {
#if (PLATFORM_BYTE_ORDER == IS_LITTLE_ENDIAN)
        KeccakPermutationOnWordsAfterXoring1344bits((UINT64*)state, (const UINT64*)data);
#else
        UINT64 dataAsWords[21];
        unsigned int i;

        for (i = 0; i<21; i++)
            fromBytesToWord(dataAsWords + i, data + (i * 8));
        KeccakPermutationOnWordsAfterXoring1344bits((UINT64*)state, dataAsWords);
#endif
    }
#endif

    void KeccakAbsorb(unsigned char *state, const unsigned char *data, unsigned int laneCount)
    {
#if (PLATFORM_BYTE_ORDER == IS_LITTLE_ENDIAN)
        KeccakPermutationOnWordsAfterXoring((UINT64*)state, (const UINT64*)data, laneCount);
#else
        UINT64 dataAsWords[25];
        unsigned int i;

        for (i = 0; i<laneCount; i++)
            fromBytesToWord(dataAsWords + i, data + (i * 8));
        KeccakPermutationOnWordsAfterXoring((UINT64*)state, dataAsWords, laneCount);
#endif
    }

    void fromWordToBytes(UINT8 *bytes, const UINT64 word)
    {
        unsigned int i;

        for (i = 0; i<(64 / 8); i++)
            bytes[i] = (word >> (8 * i)) & 0xFF;
    }

#ifdef ProvideFast1024
    void KeccakExtract1024bits(const unsigned char *state, unsigned char *data)
    {
#if (PLATFORM_BYTE_ORDER == IS_LITTLE_ENDIAN)
        memcpy(data, state, 128);
#else
        unsigned int i;

        for (i = 0; i<16; i++)
            fromWordToBytes(data + (i * 8), ((const UINT64*)state)[i]);
#endif
#ifdef UseBebigokimisa
        ((UINT64*)data)[1] = ~((UINT64*)data)[1];
        ((UINT64*)data)[2] = ~((UINT64*)data)[2];
        ((UINT64*)data)[8] = ~((UINT64*)data)[8];
        ((UINT64*)data)[12] = ~((UINT64*)data)[12];
#endif
    }
#endif

    void KeccakExtract(const unsigned char *state, unsigned char *data, unsigned int laneCount)
    {
#if (PLATFORM_BYTE_ORDER == IS_LITTLE_ENDIAN)
        memcpy(data, state, laneCount * 8);
#else
        unsigned int i;

        for (i = 0; i<laneCount; i++)
            fromWordToBytes(data + (i * 8), ((const UINT64*)state)[i]);
#endif
#ifdef UseBebigokimisa
        if (laneCount > 1)
        {
            ((UINT64*)data)[1] = ~((UINT64*)data)[1];
            if (laneCount > 2)
            {
                ((UINT64*)data)[2] = ~((UINT64*)data)[2];
                if (laneCount > 8)
                {
                    ((UINT64*)data)[8] = ~((UINT64*)data)[8];
                    if (laneCount > 12)
                    {
                        ((UINT64*)data)[12] = ~((UINT64*)data)[12];
                        if (laneCount > 17)
                        {
                            ((UINT64*)data)[17] = ~((UINT64*)data)[17];
                            if (laneCount > 20)
                            {
                                ((UINT64*)data)[20] = ~((UINT64*)data)[20];
                            }
                        }
                    }
                }
            }
        }
#endif
    }

    /*
    The Keccak sponge function, designed by Guido Bertoni, Joan Daemen,
    Michaël Peeters and Gilles Van Assche. For more information, feedback or
    questions, please refer to our website: http://keccak.noekeon.org/

    Implementation by the designers,
    hereby denoted as "the implementer".

    To the extent possible under law, the implementer has waived all copyright
    and related or neighboring rights to the source code in this file.
    http://creativecommons.org/publicdomain/zero/1.0/
    */

#define ProvideFast576
#define ProvideFast832
#define ProvideFast1024
#define ProvideFast1088
#define ProvideFast1152
#define ProvideFast1344

    void KeccakInitialize(void);
    void KeccakInitializeState(unsigned char *state);
    void KeccakPermutation(unsigned char *state);
#ifdef ProvideFast576
    void KeccakAbsorb576bits(unsigned char *state, const unsigned char *data);
#endif
#ifdef ProvideFast832
    void KeccakAbsorb832bits(unsigned char *state, const unsigned char *data);
#endif
#ifdef ProvideFast1024
    void KeccakAbsorb1024bits(unsigned char *state, const unsigned char *data);
#endif
#ifdef ProvideFast1088
    void KeccakAbsorb1088bits(unsigned char *state, const unsigned char *data);
#endif
#ifdef ProvideFast1152
    void KeccakAbsorb1152bits(unsigned char *state, const unsigned char *data);
#endif
#ifdef ProvideFast1344
    void KeccakAbsorb1344bits(unsigned char *state, const unsigned char *data);
#endif
    void KeccakAbsorb(unsigned char *state, const unsigned char *data, unsigned int laneCount);
#ifdef ProvideFast1024
    void KeccakExtract1024bits(const unsigned char *state, unsigned char *data);
#endif
    void KeccakExtract(const unsigned char *state, unsigned char *data, unsigned int laneCount);

    HashReturn Init(hashState *state, int hashbitlen)
    {
        switch (hashbitlen)
        {
            case 0: // Default parameters, arbitrary length output
                InitSponge((hashState*)state, 1024, 576);
                break;
            case 224:
                InitSponge((hashState*)state, 1152, 448);
                break;
            case 256:
                InitSponge((hashState*)state, 1088, 512);
                break;
            case 384:
                InitSponge((hashState*)state, 832, 768);
                break;
            case 512:
                InitSponge((hashState*)state, 576, 1024);
                break;
            default:
                return BAD_HASHLEN;
        }
        state->hashbitlen = hashbitlen;
        return SUCCESS;
    }

    HashReturn Update(hashState *state, const BitSequence *data, DataLength databitlen)
    {
        if ((databitlen % 8) == 0)
            return (HashReturn)Absorb((hashState*)state, data, databitlen);
        else
        {
            HashReturn ret = (HashReturn)Absorb((hashState*)state, data, databitlen - (databitlen % 8));
            if (ret == SUCCESS)
            {
                unsigned char lastByte;
                // Align the last partial byte to the least significant bits
                lastByte = data[databitlen / 8] >> (8 - (databitlen % 8));
                return (HashReturn)Absorb((hashState*)state, &lastByte, databitlen % 8);
            }
            else
                return ret;
        }
    }

    HashReturn Final(hashState *state, BitSequence *hashval)
    {
        return (HashReturn)Squeeze(state, hashval, state->hashbitlen);
    }

    HashReturn Hash(int hashbitlen, const BitSequence *data, DataLength databitlen, BitSequence *hashval)
    {
        hashState state;
        HashReturn result;

        if ((hashbitlen != 224) && (hashbitlen != 256) && (hashbitlen != 384) && (hashbitlen != 512))
            return BAD_HASHLEN; // Only the four fixed output lengths available through this API
        result = Init(&state, hashbitlen);
        if (result != SUCCESS)
            return result;
        result = Update(&state, data, databitlen);
        if (result != SUCCESS)
            return result;
        result = Final(&state, hashval);
        return result;
    }
}