
#include "stdafx.h"
#include "Skein-org.h"

namespace SkeinOrg
{
    // skein_block.c

    /***********************************************************************
    **
    ** Implementation of the Skein block functions.
    **
    ** Source code author: Doug Whiting, 2008.
    **
    ** This algorithm and source code is released to the public domain.
    **
    ** Compile-time switches:
    **
    **  SKEIN_USE_ASM  -- set bits (256/512/1024) to select which
    **                    versions use ASM code for block processing
    **                    [default: use C for all block sizes]
    **
    ************************************************************************/

#ifndef SKEIN_USE_ASM
#define SKEIN_USE_ASM   (0)                     /* default is all C code (no ASM) */
#endif

#ifndef SKEIN_LOOP
#define SKEIN_LOOP 001                          /* default: unroll 256 and 512, but not 1024 */
#endif

#define BLK_BITS        (WCNT*64)               /* some useful definitions for code here */
#define KW_TWK_BASE     (0)
#define KW_KEY_BASE     (3)
#define ks              (kw + KW_KEY_BASE)                
#define ts              (kw + KW_TWK_BASE)

#ifdef SKEIN_DEBUG
#define DebugSaveTweak(ctx) { ctx->h.T[0] = ts[0]; ctx->h.T[1] = ts[1]; }
#else
#define DebugSaveTweak(ctx)
#endif

    /*****************************  Skein_256 ******************************/
#if !(SKEIN_USE_ASM & 256)
    void Skein_256_Process_Block(Skein_256_Ctxt_t *ctx, const u08b_t *blkPtr, size_t blkCnt, size_t byteCntAdd)
    { /* do it in C */
        enum
        {
            WCNT = SKEIN_256_STATE_WORDS
        };
#undef  RCNT
#define RCNT  (SKEIN_256_ROUNDS_TOTAL/8)

#ifdef  SKEIN_LOOP                              /* configure how much to unroll the loop */
#define SKEIN_UNROLL_256 (((SKEIN_LOOP)/100)%10)
#else
#define SKEIN_UNROLL_256 (0)
#endif

#if SKEIN_UNROLL_256
#if (RCNT % SKEIN_UNROLL_256)
#error "Invalid SKEIN_UNROLL_256"               /* sanity check on unroll count */
#endif
        size_t  r;
        u64b_t  kw[WCNT + 4 + RCNT * 2];                  /* key schedule words : chaining vars + tweak + "rotation"*/
#else
        u64b_t  kw[WCNT + 4];                         /* key schedule words : chaining vars + tweak */
#endif
        u64b_t  X0, X1, X2, X3;                        /* local copy of context vars, for speed */
        u64b_t  w[WCNT];                           /* local copy of input block */
#ifdef SKEIN_DEBUG
        const u64b_t *Xptr[4];                      /* use for debugging (help compiler put Xn in registers) */
        Xptr[0] = &X0;  Xptr[1] = &X1;  Xptr[2] = &X2;  Xptr[3] = &X3;
#endif
        Skein_assert(blkCnt != 0);                  /* never call with blkCnt == 0! */
        ts[0] = ctx->h.T[0];
        ts[1] = ctx->h.T[1];
        do
        {
            /* this implementation only supports 2**64 input bytes (no carry out here) */
            ts[0] += byteCntAdd;                    /* update processed length */

            /* precompute the key schedule for this block */
            ks[0] = ctx->X[0];
            ks[1] = ctx->X[1];
            ks[2] = ctx->X[2];
            ks[3] = ctx->X[3];
            ks[4] = ks[0] ^ ks[1] ^ ks[2] ^ ks[3] ^ SKEIN_KS_PARITY;

            ts[2] = ts[0] ^ ts[1];

            Skein_Get64_LSB_First(w, blkPtr, WCNT);   /* get input block in little-endian format */
            DebugSaveTweak(ctx);
            Skein_Show_Block(BLK_BITS, &ctx->h, ctx->X, blkPtr, w, ks, ts);

            X0 = w[0] + ks[0];                      /* do the first full key injection */
            X1 = w[1] + ks[1] + ts[0];
            X2 = w[2] + ks[2] + ts[1];
            X3 = w[3] + ks[3];

            Skein_Show_R_Ptr(BLK_BITS, &ctx->h, SKEIN_RND_KEY_INITIAL, Xptr);    /* show starting state values */

            blkPtr += SKEIN_256_BLOCK_BYTES;

            /* run the rounds */

#define Round256(p0,p1,p2,p3,ROT,rNum)                              \
    X##p0 += X##p1; X##p1 = RotL_64(X##p1, ROT##_0); X##p1 ^= X##p0; \
    X##p2 += X##p3; X##p3 = RotL_64(X##p3, ROT##_1); X##p3 ^= X##p2; \

#if SKEIN_UNROLL_256 == 0                       
#define R256(p0,p1,p2,p3,ROT,rNum)           /* fully unrolled */   \
    Round256(p0, p1, p2, p3, ROT, rNum)                                  \
    Skein_Show_R_Ptr(BLK_BITS, &ctx->h, rNum, Xptr);

#define I256(R)                                                     \
    X0 += ks[((R)+1) % 5];    /* inject the key schedule value */ \
    X1 += ks[((R)+2) % 5] + ts[((R)+1) % 3];                      \
    X2 += ks[((R)+3) % 5] + ts[((R)+2) % 3];                      \
    X3 += ks[((R)+4) % 5] + (R)+1;                            \
    Skein_Show_R_Ptr(BLK_BITS, &ctx->h, SKEIN_RND_KEY_INJECT, Xptr);
#else                                       /* looping version */
#define R256(p0,p1,p2,p3,ROT,rNum)                                  \
    Round256(p0, p1, p2, p3, ROT, rNum)                                  \
    Skein_Show_R_Ptr(BLK_BITS, &ctx->h, 4 * (r - 1) + rNum, Xptr);

#define I256(R)                                                     \
    X0 += ks[r + (R)+0];        /* inject the key schedule value */ \
    X1 += ks[r + (R)+1] + ts[r + (R)+0];                              \
    X2 += ks[r + (R)+2] + ts[r + (R)+1];                              \
    X3 += ks[r + (R)+3] + r + (R);                              \
    ks[r + (R)+4] = ks[r + (R)-1];     /* rotate key schedule */\
    ts[r + (R)+2] = ts[r + (R)-1];                              \
    Skein_Show_R_Ptr(BLK_BITS, &ctx->h, SKEIN_RND_KEY_INJECT, Xptr);

            for (r = 1; r < 2 * RCNT; r += 2 * SKEIN_UNROLL_256)  /* loop thru it */
#endif  
            {
#define R256_8_rounds(R)                  \
    R256(0, 1, 2, 3, R_256_0, 8 * (R)+1);  \
    R256(0, 3, 2, 1, R_256_1, 8 * (R)+2);  \
    R256(0, 1, 2, 3, R_256_2, 8 * (R)+3);  \
    R256(0, 3, 2, 1, R_256_3, 8 * (R)+4);  \
    I256(2 * (R));                      \
    R256(0, 1, 2, 3, R_256_4, 8 * (R)+5);  \
    R256(0, 3, 2, 1, R_256_5, 8 * (R)+6);  \
    R256(0, 1, 2, 3, R_256_6, 8 * (R)+7);  \
    R256(0, 3, 2, 1, R_256_7, 8 * (R)+8);  \
    I256(2 * (R)+1);

                R256_8_rounds(0);

#define R256_Unroll_R(NN) ((SKEIN_UNROLL_256 == 0 && SKEIN_256_ROUNDS_TOTAL/8 > (NN)) || (SKEIN_UNROLL_256 > (NN)))

#if   R256_Unroll_R( 1)
                R256_8_rounds(1);
#endif
#if   R256_Unroll_R( 2)
                R256_8_rounds(2);
#endif
#if   R256_Unroll_R( 3)
                R256_8_rounds(3);
#endif
#if   R256_Unroll_R( 4)
                R256_8_rounds(4);
#endif
#if   R256_Unroll_R( 5)
                R256_8_rounds(5);
#endif
#if   R256_Unroll_R( 6)
                R256_8_rounds(6);
#endif
#if   R256_Unroll_R( 7)
                R256_8_rounds(7);
#endif
#if   R256_Unroll_R( 8)
                R256_8_rounds(8);
#endif
#if   R256_Unroll_R( 9)
                R256_8_rounds(9);
#endif
#if   R256_Unroll_R(10)
                R256_8_rounds(10);
#endif
#if   R256_Unroll_R(11)
                R256_8_rounds(11);
#endif
#if   R256_Unroll_R(12)
                R256_8_rounds(12);
#endif
#if   R256_Unroll_R(13)
                R256_8_rounds(13);
#endif
#if   R256_Unroll_R(14)
                R256_8_rounds(14);
#endif
#if  (SKEIN_UNROLL_256 > 14)
#error  "need more unrolling in Skein_256_Process_Block"
#endif
            }
            /* do the final "feedforward" xor, update context chaining vars */
            ctx->X[0] = X0 ^ w[0];
            ctx->X[1] = X1 ^ w[1];
            ctx->X[2] = X2 ^ w[2];
            ctx->X[3] = X3 ^ w[3];

            Skein_Show_Round(BLK_BITS, &ctx->h, SKEIN_RND_FEED_FWD, ctx->X);

            ts[1] &= ~SKEIN_T1_FLAG_FIRST;
        }
        while (--blkCnt);
        ctx->h.T[0] = ts[0];
        ctx->h.T[1] = ts[1];
    }

#if defined(SKEIN_CODE_SIZE) || defined(SKEIN_PERF)
    size_t Skein_256_Process_Block_CodeSize(void)
    {
        return ((u08b_t *)Skein_256_Process_Block_CodeSize) -
            ((u08b_t *)Skein_256_Process_Block);
    }
    uint_t Skein_256_Unroll_Cnt(void)
    {
        return SKEIN_UNROLL_256;
    }
#endif
#endif

    /*****************************  Skein_512 ******************************/
#if !(SKEIN_USE_ASM & 512)
    void Skein_512_Process_Block(Skein_512_Ctxt_t *ctx, const u08b_t *blkPtr, size_t blkCnt, size_t byteCntAdd)
    { /* do it in C */
        enum
        {
            WCNT = SKEIN_512_STATE_WORDS
        };
#undef  RCNT
#define RCNT  (SKEIN_512_ROUNDS_TOTAL/8)

#ifdef  SKEIN_LOOP                              /* configure how much to unroll the loop */
#define SKEIN_UNROLL_512 (((SKEIN_LOOP)/10)%10)
#else
#define SKEIN_UNROLL_512 (0)
#endif

#if SKEIN_UNROLL_512
#if (RCNT % SKEIN_UNROLL_512)
#error "Invalid SKEIN_UNROLL_512"               /* sanity check on unroll count */
#endif
        size_t  r;
        u64b_t  kw[WCNT + 4 + RCNT * 2];                  /* key schedule words : chaining vars + tweak + "rotation"*/
#else
        u64b_t  kw[WCNT + 4];                         /* key schedule words : chaining vars + tweak */
#endif
        u64b_t  X0, X1, X2, X3, X4, X5, X6, X7;            /* local copy of vars, for speed */
        u64b_t  w[WCNT];                           /* local copy of input block */
#ifdef SKEIN_DEBUG
        const u64b_t *Xptr[8];                      /* use for debugging (help compiler put Xn in registers) */
        Xptr[0] = &X0;  Xptr[1] = &X1;  Xptr[2] = &X2;  Xptr[3] = &X3;
        Xptr[4] = &X4;  Xptr[5] = &X5;  Xptr[6] = &X6;  Xptr[7] = &X7;
#endif

        Skein_assert(blkCnt != 0);                  /* never call with blkCnt == 0! */
        ts[0] = ctx->h.T[0];
        ts[1] = ctx->h.T[1];
        do
        {
            /* this implementation only supports 2**64 input bytes (no carry out here) */
            ts[0] += byteCntAdd;                    /* update processed length */

            /* precompute the key schedule for this block */
            ks[0] = ctx->X[0];
            ks[1] = ctx->X[1];
            ks[2] = ctx->X[2];
            ks[3] = ctx->X[3];
            ks[4] = ctx->X[4];
            ks[5] = ctx->X[5];
            ks[6] = ctx->X[6];
            ks[7] = ctx->X[7];
            ks[8] = ks[0] ^ ks[1] ^ ks[2] ^ ks[3] ^
                ks[4] ^ ks[5] ^ ks[6] ^ ks[7] ^ SKEIN_KS_PARITY;

            ts[2] = ts[0] ^ ts[1];

            Skein_Get64_LSB_First(w, blkPtr, WCNT); /* get input block in little-endian format */
            DebugSaveTweak(ctx);
            Skein_Show_Block(BLK_BITS, &ctx->h, ctx->X, blkPtr, w, ks, ts);

            X0 = w[0] + ks[0];                    /* do the first full key injection */
            X1 = w[1] + ks[1];
            X2 = w[2] + ks[2];
            X3 = w[3] + ks[3];
            X4 = w[4] + ks[4];
            X5 = w[5] + ks[5] + ts[0];
            X6 = w[6] + ks[6] + ts[1];
            X7 = w[7] + ks[7];

            blkPtr += SKEIN_512_BLOCK_BYTES;

            Skein_Show_R_Ptr(BLK_BITS, &ctx->h, SKEIN_RND_KEY_INITIAL, Xptr);
            /* run the rounds */
#define Round512(p0,p1,p2,p3,p4,p5,p6,p7,ROT,rNum)                  \
    X##p0 += X##p1; X##p1 = RotL_64(X##p1, ROT##_0); X##p1 ^= X##p0; \
    X##p2 += X##p3; X##p3 = RotL_64(X##p3, ROT##_1); X##p3 ^= X##p2; \
    X##p4 += X##p5; X##p5 = RotL_64(X##p5, ROT##_2); X##p5 ^= X##p4; \
    X##p6 += X##p7; X##p7 = RotL_64(X##p7, ROT##_3); X##p7 ^= X##p6; \

#if SKEIN_UNROLL_512 == 0                       
#define R512(p0,p1,p2,p3,p4,p5,p6,p7,ROT,rNum)      /* unrolled */  \
    Round512(p0, p1, p2, p3, p4, p5, p6, p7, ROT, rNum)                      \
    Skein_Show_R_Ptr(BLK_BITS, &ctx->h, rNum, Xptr);

#define I512(R)                                                     \
    X0 += ks[((R)+1) % 9];   /* inject the key schedule value */  \
    X1 += ks[((R)+2) % 9];                                        \
    X2 += ks[((R)+3) % 9];                                        \
    X3 += ks[((R)+4) % 9];                                        \
    X4 += ks[((R)+5) % 9];                                        \
    X5 += ks[((R)+6) % 9] + ts[((R)+1) % 3];                      \
    X6 += ks[((R)+7) % 9] + ts[((R)+2) % 3];                      \
    X7 += ks[((R)+8) % 9] + (R)+1;                            \
    Skein_Show_R_Ptr(BLK_BITS, &ctx->h, SKEIN_RND_KEY_INJECT, Xptr);
#else                                       /* looping version */
#define R512(p0,p1,p2,p3,p4,p5,p6,p7,ROT,rNum)                      \
    Round512(p0, p1, p2, p3, p4, p5, p6, p7, ROT, rNum)                      \
    Skein_Show_R_Ptr(BLK_BITS, &ctx->h, 4 * (r - 1) + rNum, Xptr);

#define I512(R)                                                     \
    X0 += ks[r + (R)+0];        /* inject the key schedule value */ \
    X1 += ks[r + (R)+1];                                            \
    X2 += ks[r + (R)+2];                                            \
    X3 += ks[r + (R)+3];                                            \
    X4 += ks[r + (R)+4];                                            \
    X5 += ks[r + (R)+5] + ts[r + (R)+0];                              \
    X6 += ks[r + (R)+6] + ts[r + (R)+1];                              \
    X7 += ks[r + (R)+7] + r + (R);                              \
    ks[r + (R)+8] = ks[r + (R)-1];  /* rotate key schedule */   \
    ts[r + (R)+2] = ts[r + (R)-1];                              \
    Skein_Show_R_Ptr(BLK_BITS, &ctx->h, SKEIN_RND_KEY_INJECT, Xptr);

            for (r = 1; r < 2 * RCNT; r += 2 * SKEIN_UNROLL_512)   /* loop thru it */
#endif                         /* end of looped code definitions */
            {
#define R512_8_rounds(R)  /* do 8 full rounds */  \
    R512(0, 1, 2, 3, 4, 5, 6, 7, R_512_0, 8 * (R)+1);   \
    R512(2, 1, 4, 7, 6, 5, 0, 3, R_512_1, 8 * (R)+2);   \
    R512(4, 1, 6, 3, 0, 5, 2, 7, R_512_2, 8 * (R)+3);   \
    R512(6, 1, 0, 7, 2, 5, 4, 3, R_512_3, 8 * (R)+4);   \
    I512(2 * (R));                              \
    R512(0, 1, 2, 3, 4, 5, 6, 7, R_512_4, 8 * (R)+5);   \
    R512(2, 1, 4, 7, 6, 5, 0, 3, R_512_5, 8 * (R)+6);   \
    R512(4, 1, 6, 3, 0, 5, 2, 7, R_512_6, 8 * (R)+7);   \
    R512(6, 1, 0, 7, 2, 5, 4, 3, R_512_7, 8 * (R)+8);   \
    I512(2 * (R)+1);        /* and key injection */

                R512_8_rounds(0);

#define R512_Unroll_R(NN) ((SKEIN_UNROLL_512 == 0 && SKEIN_512_ROUNDS_TOTAL/8 > (NN)) || (SKEIN_UNROLL_512 > (NN)))

#if   R512_Unroll_R( 1)
                R512_8_rounds(1);
#endif
#if   R512_Unroll_R( 2)
                R512_8_rounds(2);
#endif
#if   R512_Unroll_R( 3)
                R512_8_rounds(3);
#endif
#if   R512_Unroll_R( 4)
                R512_8_rounds(4);
#endif
#if   R512_Unroll_R( 5)
                R512_8_rounds(5);
#endif
#if   R512_Unroll_R( 6)
                R512_8_rounds(6);
#endif
#if   R512_Unroll_R( 7)
                R512_8_rounds(7);
#endif
#if   R512_Unroll_R( 8)
                R512_8_rounds(8);
#endif
#if   R512_Unroll_R( 9)
                R512_8_rounds(9);
#endif
#if   R512_Unroll_R(10)
                R512_8_rounds(10);
#endif
#if   R512_Unroll_R(11)
                R512_8_rounds(11);
#endif
#if   R512_Unroll_R(12)
                R512_8_rounds(12);
#endif
#if   R512_Unroll_R(13)
                R512_8_rounds(13);
#endif
#if   R512_Unroll_R(14)
                R512_8_rounds(14);
#endif
#if  (SKEIN_UNROLL_512 > 14)
#error  "need more unrolling in Skein_512_Process_Block"
#endif
            }

            /* do the final "feedforward" xor, update context chaining vars */
            ctx->X[0] = X0 ^ w[0];
            ctx->X[1] = X1 ^ w[1];
            ctx->X[2] = X2 ^ w[2];
            ctx->X[3] = X3 ^ w[3];
            ctx->X[4] = X4 ^ w[4];
            ctx->X[5] = X5 ^ w[5];
            ctx->X[6] = X6 ^ w[6];
            ctx->X[7] = X7 ^ w[7];
            Skein_Show_Round(BLK_BITS, &ctx->h, SKEIN_RND_FEED_FWD, ctx->X);

            ts[1] &= ~SKEIN_T1_FLAG_FIRST;
        }
        while (--blkCnt);
        ctx->h.T[0] = ts[0];
        ctx->h.T[1] = ts[1];
    }

#if defined(SKEIN_CODE_SIZE) || defined(SKEIN_PERF)
    size_t Skein_512_Process_Block_CodeSize(void)
    {
        return ((u08b_t *)Skein_512_Process_Block_CodeSize) -
            ((u08b_t *)Skein_512_Process_Block);
    }
    uint_t Skein_512_Unroll_Cnt(void)
    {
        return SKEIN_UNROLL_512;
    }
#endif
#endif

    /*****************************  Skein1024 ******************************/
#if !(SKEIN_USE_ASM & 1024)
    void Skein1024_Process_Block(Skein1024_Ctxt_t *ctx, const u08b_t *blkPtr, size_t blkCnt, size_t byteCntAdd)
    { /* do it in C, always looping (unrolled is bigger AND slower!) */
        enum
        {
            WCNT = SKEIN1024_STATE_WORDS
        };
#undef  RCNT
#define RCNT  (SKEIN1024_ROUNDS_TOTAL/8)

#ifdef  SKEIN_LOOP                              /* configure how much to unroll the loop */
#define SKEIN_UNROLL_1024 ((SKEIN_LOOP)%10)
#else
#define SKEIN_UNROLL_1024 (0)
#endif

#if (SKEIN_UNROLL_1024 != 0)
#if (RCNT % SKEIN_UNROLL_1024)
#error "Invalid SKEIN_UNROLL_1024"              /* sanity check on unroll count */
#endif
        size_t  r;
        u64b_t  kw[WCNT + 4 + RCNT * 2];                  /* key schedule words : chaining vars + tweak + "rotation"*/
#else
        u64b_t  kw[WCNT + 4];                         /* key schedule words : chaining vars + tweak */
#endif

        u64b_t  X00, X01, X02, X03, X04, X05, X06, X07,    /* local copy of vars, for speed */
            X08, X09, X10, X11, X12, X13, X14, X15;
        u64b_t  w[WCNT];                           /* local copy of input block */
#ifdef SKEIN_DEBUG
        const u64b_t *Xptr[16];                     /* use for debugging (help compiler put Xn in registers) */
        Xptr[0] = &X00;  Xptr[1] = &X01;  Xptr[2] = &X02;  Xptr[3] = &X03;
        Xptr[4] = &X04;  Xptr[5] = &X05;  Xptr[6] = &X06;  Xptr[7] = &X07;
        Xptr[8] = &X08;  Xptr[9] = &X09;  Xptr[10] = &X10;  Xptr[11] = &X11;
        Xptr[12] = &X12;  Xptr[13] = &X13;  Xptr[14] = &X14;  Xptr[15] = &X15;
#endif

        Skein_assert(blkCnt != 0);                  /* never call with blkCnt == 0! */
        ts[0] = ctx->h.T[0];
        ts[1] = ctx->h.T[1];
        do
        {
            /* this implementation only supports 2**64 input bytes (no carry out here) */
            ts[0] += byteCntAdd;                    /* update processed length */

            /* precompute the key schedule for this block */
            ks[0] = ctx->X[0];
            ks[1] = ctx->X[1];
            ks[2] = ctx->X[2];
            ks[3] = ctx->X[3];
            ks[4] = ctx->X[4];
            ks[5] = ctx->X[5];
            ks[6] = ctx->X[6];
            ks[7] = ctx->X[7];
            ks[8] = ctx->X[8];
            ks[9] = ctx->X[9];
            ks[10] = ctx->X[10];
            ks[11] = ctx->X[11];
            ks[12] = ctx->X[12];
            ks[13] = ctx->X[13];
            ks[14] = ctx->X[14];
            ks[15] = ctx->X[15];
            ks[16] = ks[0] ^ ks[1] ^ ks[2] ^ ks[3] ^
                ks[4] ^ ks[5] ^ ks[6] ^ ks[7] ^
                ks[8] ^ ks[9] ^ ks[10] ^ ks[11] ^
                ks[12] ^ ks[13] ^ ks[14] ^ ks[15] ^ SKEIN_KS_PARITY;

            ts[2] = ts[0] ^ ts[1];

            Skein_Get64_LSB_First(w, blkPtr, WCNT); /* get input block in little-endian format */
            DebugSaveTweak(ctx);
            Skein_Show_Block(BLK_BITS, &ctx->h, ctx->X, blkPtr, w, ks, ts);

            X00 = w[0] + ks[0];                 /* do the first full key injection */
            X01 = w[1] + ks[1];
            X02 = w[2] + ks[2];
            X03 = w[3] + ks[3];
            X04 = w[4] + ks[4];
            X05 = w[5] + ks[5];
            X06 = w[6] + ks[6];
            X07 = w[7] + ks[7];
            X08 = w[8] + ks[8];
            X09 = w[9] + ks[9];
            X10 = w[10] + ks[10];
            X11 = w[11] + ks[11];
            X12 = w[12] + ks[12];
            X13 = w[13] + ks[13] + ts[0];
            X14 = w[14] + ks[14] + ts[1];
            X15 = w[15] + ks[15];

            Skein_Show_R_Ptr(BLK_BITS, &ctx->h, SKEIN_RND_KEY_INITIAL, Xptr);

#define Round1024(p0,p1,p2,p3,p4,p5,p6,p7,p8,p9,pA,pB,pC,pD,pE,pF,ROT,rNum) \
    X##p0 += X##p1; X##p1 = RotL_64(X##p1, ROT##_0); X##p1 ^= X##p0;   \
    X##p2 += X##p3; X##p3 = RotL_64(X##p3, ROT##_1); X##p3 ^= X##p2;   \
    X##p4 += X##p5; X##p5 = RotL_64(X##p5, ROT##_2); X##p5 ^= X##p4;   \
    X##p6 += X##p7; X##p7 = RotL_64(X##p7, ROT##_3); X##p7 ^= X##p6;   \
    X##p8 += X##p9; X##p9 = RotL_64(X##p9, ROT##_4); X##p9 ^= X##p8;   \
    X##pA += X##pB; X##pB = RotL_64(X##pB, ROT##_5); X##pB ^= X##pA;   \
    X##pC += X##pD; X##pD = RotL_64(X##pD, ROT##_6); X##pD ^= X##pC;   \
    X##pE += X##pF; X##pF = RotL_64(X##pF, ROT##_7); X##pF ^= X##pE;   \

#if SKEIN_UNROLL_1024 == 0                      
#define R1024(p0,p1,p2,p3,p4,p5,p6,p7,p8,p9,pA,pB,pC,pD,pE,pF,ROT,rn) \
    Round1024(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, pA, pB, pC, pD, pE, pF, ROT, rn) \
    Skein_Show_R_Ptr(BLK_BITS, &ctx->h, rn, Xptr);

#define I1024(R)                                                      \
    X00 += ks[((R)+1) % 17]; /* inject the key schedule value */   \
    X01 += ks[((R)+2) % 17];                                       \
    X02 += ks[((R)+3) % 17];                                       \
    X03 += ks[((R)+4) % 17];                                       \
    X04 += ks[((R)+5) % 17];                                       \
    X05 += ks[((R)+6) % 17];                                       \
    X06 += ks[((R)+7) % 17];                                       \
    X07 += ks[((R)+8) % 17];                                       \
    X08 += ks[((R)+9) % 17];                                       \
    X09 += ks[((R)+10) % 17];                                       \
    X10 += ks[((R)+11) % 17];                                       \
    X11 += ks[((R)+12) % 17];                                       \
    X12 += ks[((R)+13) % 17];                                       \
    X13 += ks[((R)+14) % 17] + ts[((R)+1) % 3];                     \
    X14 += ks[((R)+15) % 17] + ts[((R)+2) % 3];                     \
    X15 += ks[((R)+16) % 17] + (R)+1;                           \
    Skein_Show_R_Ptr(BLK_BITS, &ctx->h, SKEIN_RND_KEY_INJECT, Xptr);
#else                                       /* looping version */
#define R1024(p0,p1,p2,p3,p4,p5,p6,p7,p8,p9,pA,pB,pC,pD,pE,pF,ROT,rn) \
    Round1024(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, pA, pB, pC, pD, pE, pF, ROT, rn) \
    Skein_Show_R_Ptr(BLK_BITS, &ctx->h, 4 * (r - 1) + rn, Xptr);

#define I1024(R)                                                      \
    X00 += ks[r + (R)+0];    /* inject the key schedule value */     \
    X01 += ks[r + (R)+1];                                            \
    X02 += ks[r + (R)+2];                                            \
    X03 += ks[r + (R)+3];                                            \
    X04 += ks[r + (R)+4];                                            \
    X05 += ks[r + (R)+5];                                            \
    X06 += ks[r + (R)+6];                                            \
    X07 += ks[r + (R)+7];                                            \
    X08 += ks[r + (R)+8];                                            \
    X09 += ks[r + (R)+9];                                            \
    X10 += ks[r + (R)+10];                                            \
    X11 += ks[r + (R)+11];                                            \
    X12 += ks[r + (R)+12];                                            \
    X13 += ks[r + (R)+13] + ts[r + (R)+0];                              \
    X14 += ks[r + (R)+14] + ts[r + (R)+1];                              \
    X15 += ks[r + (R)+15] + r + (R);                              \
    ks[r + (R)+16] = ks[r + (R)-1];  /* rotate key schedule */   \
    ts[r + (R)+2] = ts[r + (R)-1];                              \
    Skein_Show_R_Ptr(BLK_BITS, &ctx->h, SKEIN_RND_KEY_INJECT, Xptr);

            for (r = 1; r <= 2 * RCNT; r += 2 * SKEIN_UNROLL_1024)    /* loop thru it */
#endif  
            {
#define R1024_8_rounds(R)    /* do 8 full rounds */                               \
    R1024(00, 01, 02, 03, 04, 05, 06, 07, 08, 09, 10, 11, 12, 13, 14, 15, R1024_0, 8 * (R)+1); \
    R1024(00, 09, 02, 13, 06, 11, 04, 15, 10, 07, 12, 03, 14, 05, 08, 01, R1024_1, 8 * (R)+2); \
    R1024(00, 07, 02, 05, 04, 03, 06, 01, 12, 15, 14, 13, 08, 11, 10, 09, R1024_2, 8 * (R)+3); \
    R1024(00, 15, 02, 11, 06, 13, 04, 09, 14, 01, 08, 05, 10, 03, 12, 07, R1024_3, 8 * (R)+4); \
    I1024(2 * (R));                                                             \
    R1024(00, 01, 02, 03, 04, 05, 06, 07, 08, 09, 10, 11, 12, 13, 14, 15, R1024_4, 8 * (R)+5); \
    R1024(00, 09, 02, 13, 06, 11, 04, 15, 10, 07, 12, 03, 14, 05, 08, 01, R1024_5, 8 * (R)+6); \
    R1024(00, 07, 02, 05, 04, 03, 06, 01, 12, 15, 14, 13, 08, 11, 10, 09, R1024_6, 8 * (R)+7); \
    R1024(00, 15, 02, 11, 06, 13, 04, 09, 14, 01, 08, 05, 10, 03, 12, 07, R1024_7, 8 * (R)+8); \
    I1024(2 * (R)+1);

                R1024_8_rounds(0);

#define R1024_Unroll_R(NN) ((SKEIN_UNROLL_1024 == 0 && SKEIN1024_ROUNDS_TOTAL/8 > (NN)) || (SKEIN_UNROLL_1024 > (NN)))

#if   R1024_Unroll_R( 1)
                R1024_8_rounds(1);
#endif
#if   R1024_Unroll_R( 2)
                R1024_8_rounds(2);
#endif
#if   R1024_Unroll_R( 3)
                R1024_8_rounds(3);
#endif
#if   R1024_Unroll_R( 4)
                R1024_8_rounds(4);
#endif
#if   R1024_Unroll_R( 5)
                R1024_8_rounds(5);
#endif
#if   R1024_Unroll_R( 6)
                R1024_8_rounds(6);
#endif
#if   R1024_Unroll_R( 7)
                R1024_8_rounds(7);
#endif
#if   R1024_Unroll_R( 8)
                R1024_8_rounds(8);
#endif
#if   R1024_Unroll_R( 9)
                R1024_8_rounds(9);
#endif
#if   R1024_Unroll_R(10)
                R1024_8_rounds(10);
#endif
#if   R1024_Unroll_R(11)
                R1024_8_rounds(11);
#endif
#if   R1024_Unroll_R(12)
                R1024_8_rounds(12);
#endif
#if   R1024_Unroll_R(13)
                R1024_8_rounds(13);
#endif
#if   R1024_Unroll_R(14)
                R1024_8_rounds(14);
#endif
#if  (SKEIN_UNROLL_1024 > 14)
#error  "need more unrolling in Skein_1024_Process_Block"
#endif
            }
            /* do the final "feedforward" xor, update context chaining vars */

            ctx->X[0] = X00 ^ w[0];
            ctx->X[1] = X01 ^ w[1];
            ctx->X[2] = X02 ^ w[2];
            ctx->X[3] = X03 ^ w[3];
            ctx->X[4] = X04 ^ w[4];
            ctx->X[5] = X05 ^ w[5];
            ctx->X[6] = X06 ^ w[6];
            ctx->X[7] = X07 ^ w[7];
            ctx->X[8] = X08 ^ w[8];
            ctx->X[9] = X09 ^ w[9];
            ctx->X[10] = X10 ^ w[10];
            ctx->X[11] = X11 ^ w[11];
            ctx->X[12] = X12 ^ w[12];
            ctx->X[13] = X13 ^ w[13];
            ctx->X[14] = X14 ^ w[14];
            ctx->X[15] = X15 ^ w[15];

            Skein_Show_Round(BLK_BITS, &ctx->h, SKEIN_RND_FEED_FWD, ctx->X);

            ts[1] &= ~SKEIN_T1_FLAG_FIRST;
            blkPtr += SKEIN1024_BLOCK_BYTES;
        }
        while (--blkCnt);
        ctx->h.T[0] = ts[0];
        ctx->h.T[1] = ts[1];
    }

#if defined(SKEIN_CODE_SIZE) || defined(SKEIN_PERF)
    size_t Skein1024_Process_Block_CodeSize(void)
    {
        return ((u08b_t *)Skein1024_Process_Block_CodeSize) -
            ((u08b_t *)Skein1024_Process_Block);
    }
    uint_t Skein1024_Unroll_Cnt(void)
    {
        return SKEIN_UNROLL_1024;
    }
#endif
#endif

    
    // #include "skein_iv.h"    /* get precomputed IVs */

    /*
    ***************** Pre-computed Skein IVs *******************
    **
    ** NOTE: these values are not "magic" constants, but
    ** are generated using the Threefish block function.
    ** They are pre-computed here only for speed; i.e., to
    ** avoid the need for a Threefish call during Init().
    **
    ** The IV for any fixed hash length may be pre-computed.
    ** Only the most common values are included here.
    **
    ************************************************************
    **/

    #define MK_64 SKEIN_MK_64

    /* blkSize =  256 bits. hashSize =  128 bits */
    const u64b_t SKEIN_256_IV_128[] =
    {
        MK_64(0x302F7EA2, 0x3D7FE2E1),
        MK_64(0xADE4683A, 0x6913752B),
        MK_64(0x975CFABE, 0xF208AB0A),
        MK_64(0x2AF4BA95, 0xF831F55B)
    };

    /* blkSize =  256 bits. hashSize =  160 bits */
    const u64b_t SKEIN_256_IV_160[] =
    {
        MK_64(0xA38A0D80, 0xA3687723),
        MK_64(0xB73CDB6A, 0x5963FFC9),
        MK_64(0x9633E8EA, 0x07A1B447),
        MK_64(0xCA0ED09E, 0xC9529C22)
    };

    /* blkSize =  256 bits. hashSize =  224 bits */
    const u64b_t SKEIN_256_IV_224[] =
    {
        MK_64(0xB8092969, 0x9AE0F431),
        MK_64(0xD340DC14, 0xA06929DC),
        MK_64(0xAE866594, 0xBDE4DC5A),
        MK_64(0x339767C2, 0x5A60EA1D)
    };

    /* blkSize =  256 bits. hashSize =  256 bits */
    const u64b_t SKEIN_256_IV_256[] =
    {
        MK_64(0x38851268, 0x0E660046),
        MK_64(0x4B72D5DE, 0xC5A8FF01),
        MK_64(0x281A9298, 0xCA5EB3A5),
        MK_64(0x54CA5249, 0xF46070C4)
    };

    /* blkSize =  512 bits. hashSize =  128 bits */
    const u64b_t SKEIN_512_IV_128[] =
    {
        MK_64(0x477DF9EF, 0xAFC4F08A),
        MK_64(0x7A64D342, 0x33660E14),
        MK_64(0x71653C44, 0xCEBC89C5),
        MK_64(0x63D2A36D, 0x65B0AB91),
        MK_64(0x52B93FB0, 0x9782EA89),
        MK_64(0x20F36980, 0x8B960829),
        MK_64(0xE8DF80FB, 0x30303B9B),
        MK_64(0xB89D3902, 0x1A476D1F)
    };

    /* blkSize =  512 bits. hashSize =  160 bits */
    const u64b_t SKEIN_512_IV_160[] =
    {
        MK_64(0x0045FA2C, 0xAD913A2C),
        MK_64(0xF45C9A76, 0xBF75CE81),
        MK_64(0x0ED758A9, 0x3D1F266B),
        MK_64(0xC0E65E85, 0x1EDCD67A),
        MK_64(0x1E024D51, 0xF5E7583E),
        MK_64(0xA271F855, 0x4E52B0E1),
        MK_64(0x5292867D, 0x8AC674F9),
        MK_64(0xADA325FA, 0x60C3B226)
    };

    /* blkSize =  512 bits. hashSize =  224 bits */
    const u64b_t SKEIN_512_IV_224[] =
    {
        MK_64(0xF2DAA169, 0x8216CC98),
        MK_64(0x00E06A48, 0x8983AE05),
        MK_64(0xC080CEA9, 0x5948958F),
        MK_64(0x2A8F314B, 0x57F4ADD1),
        MK_64(0xBCD06591, 0x360A405A),
        MK_64(0xF81A11A1, 0x02D91F70),
        MK_64(0x85C6FFA5, 0x4810A739),
        MK_64(0x1E07AFE0, 0x1802CE74)
    };

    /* blkSize =  512 bits. hashSize =  256 bits */
    const u64b_t SKEIN_512_IV_256[] =
    {
        MK_64(0x88C07F38, 0xD4F95AD4),
        MK_64(0x3DF0D33A, 0x8610E240),
        MK_64(0x3E243F6E, 0xDB6FAC74),
        MK_64(0xBAC4F4CD, 0xD7A90A24),
        MK_64(0xDF90FD1F, 0xDEEEBA04),
        MK_64(0xA4F5796B, 0xDB7FDDA8),
        MK_64(0xDA182FD2, 0x964BC923),
        MK_64(0x55F76677, 0xEF6961F9)
    };

    /* blkSize =  512 bits. hashSize =  384 bits */
    const u64b_t SKEIN_512_IV_384[] =
    {
        MK_64(0xE5BF4D02, 0xBA62494C),
        MK_64(0x7AA1EABC, 0xC3E6FC68),
        MK_64(0xBBE5FC26, 0xE1038C5A),
        MK_64(0x53C9903E, 0x8F88E9FA),
        MK_64(0xF30D8DDD, 0xFB940C83),
        MK_64(0x500FDA3C, 0x4865ABEC),
        MK_64(0x2226C67F, 0x745BC5E7),
        MK_64(0x015DA800, 0x77C639F7)
    };

    /* blkSize =  512 bits. hashSize =  512 bits */
    const u64b_t SKEIN_512_IV_512[] =
    {
        MK_64(0xA8D47980, 0x544A6E32),
        MK_64(0x84751153, 0x3E9B1A8A),
        MK_64(0x6FAEE870, 0xD8E81A00),
        MK_64(0x58B0D9D6, 0xCB557F92),
        MK_64(0x9BBC0051, 0xDAC1D4E9),
        MK_64(0xB744E2B1, 0xD189E7CA),
        MK_64(0x979350FA, 0x709C5EF3),
        MK_64(0x0350125A, 0x92067BCD)
    };

    /* blkSize = 1024 bits. hashSize =  384 bits */
    const u64b_t SKEIN1024_IV_384[] =
    {
        MK_64(0x7600B2E3, 0x9FC73E48),
        MK_64(0x7A4543BB, 0xECCD60E4),
        MK_64(0x8AB879D6, 0x2F53E192),
        MK_64(0x14847919, 0xA7F0AC6E),
        MK_64(0x4F774735, 0xAA99CB7F),
        MK_64(0x607CF3C2, 0x41760EE1),
        MK_64(0xC0BF6D7B, 0xFF9F27DB),
        MK_64(0x7D321485, 0x99342254),
        MK_64(0xE7231BB0, 0xCDF9DD49),
        MK_64(0x641DE8E6, 0x464DB3F4),
        MK_64(0x05613046, 0xA01CF005),
        MK_64(0x7347EE0B, 0xB09E8BCC),
        MK_64(0x5103A256, 0x161A26FF),
        MK_64(0x8161EAC4, 0x3A1176C2),
        MK_64(0xB9607373, 0xCF92C2CC),
        MK_64(0xFDE8D4AD, 0xD376300D)
    };

    /* blkSize = 1024 bits. hashSize =  512 bits */
    const u64b_t SKEIN1024_IV_512[] =
    {
        MK_64(0x8CF63BE5, 0xE1EDF4B7),
        MK_64(0x256FD425, 0xCBDE61EB),
        MK_64(0x497B412D, 0xEBA3EF9D),
        MK_64(0x3CBD412A, 0xD8293FBA),
        MK_64(0xD5AE34D6, 0xF26F646E),
        MK_64(0x72879C01, 0x0DA17B79),
        MK_64(0x61BD8F18, 0x05AFF621),
        MK_64(0x75CB3C94, 0x9CE0E463),
        MK_64(0xAF27329D, 0x2CD71E37),
        MK_64(0x7DB5EC5E, 0x1141CE9F),
        MK_64(0x76484C13, 0x20CAB67B),
        MK_64(0x57EB52A6, 0x561BE8C5),
        MK_64(0x51161125, 0xE681412D),
        MK_64(0xF510D937, 0x5439A9BC),
        MK_64(0xD18AF77C, 0xFC425E3C),
        MK_64(0xEB05160C, 0x3FEBB037)
    };

    /* blkSize = 1024 bits. hashSize = 1024 bits */
    const u64b_t SKEIN1024_IV_1024[] =
    {
        MK_64(0x5A4352BE, 0x62092156),
        MK_64(0x5F6E8B1A, 0x72F001CA),
        MK_64(0xFFCBFE9C, 0xA1A2CE26),
        MK_64(0x6C23C396, 0x67038BCA),
        MK_64(0x583A8BFC, 0xCE34EB6C),
        MK_64(0x3FDBFB11, 0xD4A46A3E),
        MK_64(0x3304ACFC, 0xA8300998),
        MK_64(0xB2F6675F, 0xA17F0FD2),
        MK_64(0x9D259973, 0x0EF7AB6B),
        MK_64(0x0914A20D, 0x3DFEA9E4),
        MK_64(0xCC1A9CAF, 0xA494DBD3),
        MK_64(0x9828030D, 0xA0A6388C),
        MK_64(0x0D339D5D, 0xAADEE3DC),
        MK_64(0xFC46DE35, 0xC4E2A086),
        MK_64(0x53D6E4F5, 0x2E19A6D1),
        MK_64(0x5663952F, 0x715D1DDD)
    };

    // Skein.c

    /***********************************************************************
    **
    ** Implementation of the Skein hash function.
    **
    ** Source code author: Doug Whiting, 2008.
    **
    ** This algorithm and source code is released to the public domain.
    **
    ************************************************************************/

    #define  SKEIN_PORT_CODE /* instantiate any code in skein_port.h */

    /*****************************************************************/
    /* External function to process blkCnt (nonzero) full block(s) of data. */
    void    Skein_256_Process_Block(Skein_256_Ctxt_t *ctx, const u08b_t *blkPtr, size_t blkCnt, size_t byteCntAdd);
    void    Skein_512_Process_Block(Skein_512_Ctxt_t *ctx, const u08b_t *blkPtr, size_t blkCnt, size_t byteCntAdd);
    void    Skein1024_Process_Block(Skein1024_Ctxt_t *ctx, const u08b_t *blkPtr, size_t blkCnt, size_t byteCntAdd);

    /*****************************************************************/
    /*     256-bit Skein                                             */
    /*****************************************************************/

    /*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
    /* init the context for a straight hashing operation  */
    int Skein_256_Init(Skein_256_Ctxt_t *ctx, size_t hashBitLen)
    {
        union
        {
            u08b_t  b[SKEIN_256_STATE_BYTES];
            u64b_t  w[SKEIN_256_STATE_WORDS];
        } cfg;                              /* config block */

        Skein_Assert(hashBitLen > 0, SKEIN_BAD_HASHLEN);
        ctx->h.hashBitLen = hashBitLen;         /* output hash bit count */

        switch (hashBitLen)
        {             /* use pre-computed values, where available */
#ifndef SKEIN_NO_PRECOMP
            case  256: memcpy(ctx->X, SKEIN_256_IV_256, sizeof(ctx->X));  break;
            case  224: memcpy(ctx->X, SKEIN_256_IV_224, sizeof(ctx->X));  break;
            case  160: memcpy(ctx->X, SKEIN_256_IV_160, sizeof(ctx->X));  break;
            case  128: memcpy(ctx->X, SKEIN_256_IV_128, sizeof(ctx->X));  break;
#endif
            default:
                /* here if there is no precomputed IV value available */
                /* build/process the config block, type == CONFIG (could be precomputed) */
                Skein_Start_New_Type(ctx, CFG_FINAL);        /* set tweaks: T0=0; T1=CFG | FINAL */

                cfg.w[0] = Skein_Swap64(SKEIN_SCHEMA_VER);  /* set the schema, version */
                cfg.w[1] = Skein_Swap64(hashBitLen);        /* hash result length in bits */
                cfg.w[2] = Skein_Swap64(SKEIN_CFG_TREE_INFO_SEQUENTIAL);
                memset(&cfg.w[3], 0, sizeof(cfg)-3 * sizeof(cfg.w[0])); /* zero pad config block */

                /* compute the initial chaining values from config block */
                memset(ctx->X, 0, sizeof(ctx->X));            /* zero the chaining variables */
                Skein_256_Process_Block(ctx, cfg.b, 1, SKEIN_CFG_STR_LEN);
                break;
        }
        /* The chaining vars ctx->X are now initialized for the given hashBitLen. */
        /* Set up to process the data message portion of the hash (default) */
        Skein_Start_New_Type(ctx, MSG);              /* T0=0, T1= MSG type */

        return SKEIN_SUCCESS;
    }

    /*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
    /* init the context for a MAC and/or tree hash operation */
    /* [identical to Skein_256_Init() when keyBytes == 0 && treeInfo == SKEIN_CFG_TREE_INFO_SEQUENTIAL] */
    int Skein_256_InitExt(Skein_256_Ctxt_t *ctx, size_t hashBitLen, u64b_t treeInfo, const u08b_t *key, size_t keyBytes)
    {
        union
        {
            u08b_t  b[SKEIN_256_STATE_BYTES];
            u64b_t  w[SKEIN_256_STATE_WORDS];
        } cfg;                              /* config block */

        Skein_Assert(hashBitLen > 0, SKEIN_BAD_HASHLEN);
        Skein_Assert(keyBytes == 0 || key != NULL, SKEIN_FAIL);

        /* compute the initial chaining values ctx->X[], based on key */
        if (keyBytes == 0)                          /* is there a key? */
        {
            memset(ctx->X, 0, sizeof(ctx->X));        /* no key: use all zeroes as key for config block */
        }
        else                                        /* here to pre-process a key */
        {
            Skein_assert(sizeof(cfg.b) >= sizeof(ctx->X));
            /* do a mini-Init right here */
            ctx->h.hashBitLen = 8 * sizeof(ctx->X);     /* set output hash bit count = state size */
            Skein_Start_New_Type(ctx, KEY);          /* set tweaks: T0 = 0; T1 = KEY type */
            memset(ctx->X, 0, sizeof(ctx->X));        /* zero the initial chaining variables */
            Skein_256_Update(ctx, key, keyBytes);     /* hash the key */
            Skein_256_Final_Pad(ctx, cfg.b);         /* put result into cfg.b[] */
            memcpy(ctx->X, cfg.b, sizeof(cfg.b));     /* copy over into ctx->X[] */
#if SKEIN_NEED_SWAP
            {
                uint_t i;
                for (i = 0; i<SKEIN_256_STATE_WORDS; i++)   /* convert key bytes to context words */
                    ctx->X[i] = Skein_Swap64(ctx->X[i]);
            }
#endif
        }
        /* build/process the config block, type == CONFIG (could be precomputed for each key) */
        ctx->h.hashBitLen = hashBitLen;             /* output hash bit count */
        Skein_Start_New_Type(ctx, CFG_FINAL);

        memset(&cfg.w, 0, sizeof(cfg.w));             /* pre-pad cfg.w[] with zeroes */
        cfg.w[0] = Skein_Swap64(SKEIN_SCHEMA_VER);
        cfg.w[1] = Skein_Swap64(hashBitLen);        /* hash result length in bits */
        cfg.w[2] = Skein_Swap64(treeInfo);          /* tree hash config info (or SKEIN_CFG_TREE_INFO_SEQUENTIAL) */

        Skein_Show_Key(256, &ctx->h, key, keyBytes);

        /* compute the initial chaining values from config block */
        Skein_256_Process_Block(ctx, cfg.b, 1, SKEIN_CFG_STR_LEN);

        /* The chaining vars ctx->X are now initialized */
        /* Set up to process the data message portion of the hash (default) */
        ctx->h.bCnt = 0;                            /* buffer b[] starts out empty */
        Skein_Start_New_Type(ctx, MSG);

        return SKEIN_SUCCESS;
    }

    /*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
    /* process the input bytes */
    int Skein_256_Update(Skein_256_Ctxt_t *ctx, const u08b_t *msg, size_t msgByteCnt)
    {
        size_t n;

        Skein_Assert(ctx->h.bCnt <= SKEIN_256_BLOCK_BYTES, SKEIN_FAIL);    /* catch uninitialized context */

        /* process full blocks, if any */
        if (msgByteCnt + ctx->h.bCnt > SKEIN_256_BLOCK_BYTES)
        {
            if (ctx->h.bCnt)                              /* finish up any buffered message data */
            {
                n = SKEIN_256_BLOCK_BYTES - ctx->h.bCnt;  /* # bytes free in buffer b[] */
                if (n)
                {
                    Skein_assert(n < msgByteCnt);         /* check on our logic here */
                    memcpy(&ctx->b[ctx->h.bCnt], msg, n);
                    msgByteCnt -= n;
                    msg += n;
                    ctx->h.bCnt += n;
                }
                Skein_assert(ctx->h.bCnt == SKEIN_256_BLOCK_BYTES);
                Skein_256_Process_Block(ctx, ctx->b, 1, SKEIN_256_BLOCK_BYTES);
                ctx->h.bCnt = 0;
            }
            /* now process any remaining full blocks, directly from input message data */
            if (msgByteCnt > SKEIN_256_BLOCK_BYTES)
            {
                n = (msgByteCnt - 1) / SKEIN_256_BLOCK_BYTES;   /* number of full blocks to process */
                Skein_256_Process_Block(ctx, msg, n, SKEIN_256_BLOCK_BYTES);
                msgByteCnt -= n * SKEIN_256_BLOCK_BYTES;
                msg += n * SKEIN_256_BLOCK_BYTES;
            }
            Skein_assert(ctx->h.bCnt == 0);
        }

        /* copy any remaining source message data bytes into b[] */
        if (msgByteCnt)
        {
            Skein_assert(msgByteCnt + ctx->h.bCnt <= SKEIN_256_BLOCK_BYTES);
            memcpy(&ctx->b[ctx->h.bCnt], msg, msgByteCnt);
            ctx->h.bCnt += msgByteCnt;
        }

        return SKEIN_SUCCESS;
    }

    /*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
    /* finalize the hash computation and output the result */
    int Skein_256_Final(Skein_256_Ctxt_t *ctx, u08b_t *hashVal)
    {
        size_t i, n, byteCnt;
        u64b_t X[SKEIN_256_STATE_WORDS];
        Skein_Assert(ctx->h.bCnt <= SKEIN_256_BLOCK_BYTES, SKEIN_FAIL);    /* catch uninitialized context */

        ctx->h.T[1] |= SKEIN_T1_FLAG_FINAL;                 /* tag as the final block */
        if (ctx->h.bCnt < SKEIN_256_BLOCK_BYTES)            /* zero pad b[] if necessary */
            memset(&ctx->b[ctx->h.bCnt], 0, SKEIN_256_BLOCK_BYTES - ctx->h.bCnt);

        Skein_256_Process_Block(ctx, ctx->b, 1, ctx->h.bCnt);  /* process the final block */

        /* now output the result */
        byteCnt = (ctx->h.hashBitLen + 7) >> 3;             /* total number of output bytes */

        /* run Threefish in "counter mode" to generate output */
        memset(ctx->b, 0, sizeof(ctx->b));  /* zero out b[], so it can hold the counter */
        memcpy(X, ctx->X, sizeof(X));       /* keep a local copy of counter mode "key" */
        for (i = 0; i*SKEIN_256_BLOCK_BYTES < byteCnt; i++)
        {
            ((u64b_t *)ctx->b)[0] = Skein_Swap64((u64b_t)i); /* build the counter block */
            Skein_Start_New_Type(ctx, OUT_FINAL);
            Skein_256_Process_Block(ctx, ctx->b, 1, sizeof(u64b_t)); /* run "counter mode" */
            n = byteCnt - i*SKEIN_256_BLOCK_BYTES;   /* number of output bytes left to go */
            if (n >= SKEIN_256_BLOCK_BYTES)
                n = SKEIN_256_BLOCK_BYTES;
            Skein_Put64_LSB_First(hashVal + i*SKEIN_256_BLOCK_BYTES, ctx->X, n);   /* "output" the ctr mode bytes */
            Skein_Show_Final(256, &ctx->h, n, hashVal + i*SKEIN_256_BLOCK_BYTES);
            memcpy(ctx->X, X, sizeof(X));   /* restore the counter mode key for next time */
        }
        return SKEIN_SUCCESS;
    }

#if defined(SKEIN_CODE_SIZE) || defined(SKEIN_PERF)
    size_t Skein_256_API_CodeSize(void)
    {
        return ((u08b_t *)Skein_256_API_CodeSize) -
            ((u08b_t *)Skein_256_Init);
    }
#endif

    /*****************************************************************/
    /*     512-bit Skein                                             */
    /*****************************************************************/

    /*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
    /* init the context for a straight hashing operation  */
    int Skein_512_Init(Skein_512_Ctxt_t *ctx, size_t hashBitLen)
    {
        union
        {
            u08b_t  b[SKEIN_512_STATE_BYTES];
            u64b_t  w[SKEIN_512_STATE_WORDS];
        } cfg;                              /* config block */

        Skein_Assert(hashBitLen > 0, SKEIN_BAD_HASHLEN);
        ctx->h.hashBitLen = hashBitLen;         /* output hash bit count */

        switch (hashBitLen)
        {             /* use pre-computed values, where available */
#ifndef SKEIN_NO_PRECOMP
            case  512: memcpy(ctx->X, SKEIN_512_IV_512, sizeof(ctx->X));  break;
            case  384: memcpy(ctx->X, SKEIN_512_IV_384, sizeof(ctx->X));  break;
            case  256: memcpy(ctx->X, SKEIN_512_IV_256, sizeof(ctx->X));  break;
            case  224: memcpy(ctx->X, SKEIN_512_IV_224, sizeof(ctx->X));  break;
#endif
            default:
                /* here if there is no precomputed IV value available */
                /* build/process the config block, type == CONFIG (could be precomputed) */
                Skein_Start_New_Type(ctx, CFG_FINAL);        /* set tweaks: T0=0; T1=CFG | FINAL */

                cfg.w[0] = Skein_Swap64(SKEIN_SCHEMA_VER);  /* set the schema, version */
                cfg.w[1] = Skein_Swap64(hashBitLen);        /* hash result length in bits */
                cfg.w[2] = Skein_Swap64(SKEIN_CFG_TREE_INFO_SEQUENTIAL);
                memset(&cfg.w[3], 0, sizeof(cfg)-3 * sizeof(cfg.w[0])); /* zero pad config block */

                /* compute the initial chaining values from config block */
                memset(ctx->X, 0, sizeof(ctx->X));            /* zero the chaining variables */
                Skein_512_Process_Block(ctx, cfg.b, 1, SKEIN_CFG_STR_LEN);
                break;
        }

        /* The chaining vars ctx->X are now initialized for the given hashBitLen. */
        /* Set up to process the data message portion of the hash (default) */
        Skein_Start_New_Type(ctx, MSG);              /* T0=0, T1= MSG type */

        return SKEIN_SUCCESS;
    }

    /*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
    /* init the context for a MAC and/or tree hash operation */
    /* [identical to Skein_512_Init() when keyBytes == 0 && treeInfo == SKEIN_CFG_TREE_INFO_SEQUENTIAL] */
    int Skein_512_InitExt(Skein_512_Ctxt_t *ctx, size_t hashBitLen, u64b_t treeInfo, const u08b_t *key, size_t keyBytes)
    {
        union
        {
            u08b_t  b[SKEIN_512_STATE_BYTES];
            u64b_t  w[SKEIN_512_STATE_WORDS];
        } cfg;                              /* config block */

        Skein_Assert(hashBitLen > 0, SKEIN_BAD_HASHLEN);
        Skein_Assert(keyBytes == 0 || key != NULL, SKEIN_FAIL);

        /* compute the initial chaining values ctx->X[], based on key */
        if (keyBytes == 0)                          /* is there a key? */
        {
            memset(ctx->X, 0, sizeof(ctx->X));        /* no key: use all zeroes as key for config block */
        }
        else                                        /* here to pre-process a key */
        {
            Skein_assert(sizeof(cfg.b) >= sizeof(ctx->X));
            /* do a mini-Init right here */
            ctx->h.hashBitLen = 8 * sizeof(ctx->X);     /* set output hash bit count = state size */
            Skein_Start_New_Type(ctx, KEY);          /* set tweaks: T0 = 0; T1 = KEY type */
            memset(ctx->X, 0, sizeof(ctx->X));        /* zero the initial chaining variables */
            Skein_512_Update(ctx, key, keyBytes);     /* hash the key */
            Skein_512_Final_Pad(ctx, cfg.b);         /* put result into cfg.b[] */
            memcpy(ctx->X, cfg.b, sizeof(cfg.b));     /* copy over into ctx->X[] */
#if SKEIN_NEED_SWAP
            {
                uint_t i;
                for (i = 0; i<SKEIN_512_STATE_WORDS; i++)   /* convert key bytes to context words */
                    ctx->X[i] = Skein_Swap64(ctx->X[i]);
            }
#endif
        }
        /* build/process the config block, type == CONFIG (could be precomputed for each key) */
        ctx->h.hashBitLen = hashBitLen;             /* output hash bit count */
        Skein_Start_New_Type(ctx, CFG_FINAL);

        memset(&cfg.w, 0, sizeof(cfg.w));             /* pre-pad cfg.w[] with zeroes */
        cfg.w[0] = Skein_Swap64(SKEIN_SCHEMA_VER);
        cfg.w[1] = Skein_Swap64(hashBitLen);        /* hash result length in bits */
        cfg.w[2] = Skein_Swap64(treeInfo);          /* tree hash config info (or SKEIN_CFG_TREE_INFO_SEQUENTIAL) */

        Skein_Show_Key(512, &ctx->h, key, keyBytes);

        /* compute the initial chaining values from config block */
        Skein_512_Process_Block(ctx, cfg.b, 1, SKEIN_CFG_STR_LEN);

        /* The chaining vars ctx->X are now initialized */
        /* Set up to process the data message portion of the hash (default) */
        ctx->h.bCnt = 0;                            /* buffer b[] starts out empty */
        Skein_Start_New_Type(ctx, MSG);

        return SKEIN_SUCCESS;
    }

    /*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
    /* process the input bytes */
    int Skein_512_Update(Skein_512_Ctxt_t *ctx, const u08b_t *msg, size_t msgByteCnt)
    {
        size_t n;

        Skein_Assert(ctx->h.bCnt <= SKEIN_512_BLOCK_BYTES, SKEIN_FAIL);    /* catch uninitialized context */

        /* process full blocks, if any */
        if (msgByteCnt + ctx->h.bCnt > SKEIN_512_BLOCK_BYTES)
        {
            if (ctx->h.bCnt)                              /* finish up any buffered message data */
            {
                n = SKEIN_512_BLOCK_BYTES - ctx->h.bCnt;  /* # bytes free in buffer b[] */
                if (n)
                {
                    Skein_assert(n < msgByteCnt);         /* check on our logic here */
                    memcpy(&ctx->b[ctx->h.bCnt], msg, n);
                    msgByteCnt -= n;
                    msg += n;
                    ctx->h.bCnt += n;
                }
                Skein_assert(ctx->h.bCnt == SKEIN_512_BLOCK_BYTES);
                Skein_512_Process_Block(ctx, ctx->b, 1, SKEIN_512_BLOCK_BYTES);
                ctx->h.bCnt = 0;
            }
            /* now process any remaining full blocks, directly from input message data */
            if (msgByteCnt > SKEIN_512_BLOCK_BYTES)
            {
                n = (msgByteCnt - 1) / SKEIN_512_BLOCK_BYTES;   /* number of full blocks to process */
                Skein_512_Process_Block(ctx, msg, n, SKEIN_512_BLOCK_BYTES);
                msgByteCnt -= n * SKEIN_512_BLOCK_BYTES;
                msg += n * SKEIN_512_BLOCK_BYTES;
            }
            Skein_assert(ctx->h.bCnt == 0);
        }

        /* copy any remaining source message data bytes into b[] */
        if (msgByteCnt)
        {
            Skein_assert(msgByteCnt + ctx->h.bCnt <= SKEIN_512_BLOCK_BYTES);
            memcpy(&ctx->b[ctx->h.bCnt], msg, msgByteCnt);
            ctx->h.bCnt += msgByteCnt;
        }

        return SKEIN_SUCCESS;
    }

    /*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
    /* finalize the hash computation and output the result */
    int Skein_512_Final(Skein_512_Ctxt_t *ctx, u08b_t *hashVal)
    {
        size_t i, n, byteCnt;
        u64b_t X[SKEIN_512_STATE_WORDS];
        Skein_Assert(ctx->h.bCnt <= SKEIN_512_BLOCK_BYTES, SKEIN_FAIL);    /* catch uninitialized context */

        ctx->h.T[1] |= SKEIN_T1_FLAG_FINAL;                 /* tag as the final block */
        if (ctx->h.bCnt < SKEIN_512_BLOCK_BYTES)            /* zero pad b[] if necessary */
            memset(&ctx->b[ctx->h.bCnt], 0, SKEIN_512_BLOCK_BYTES - ctx->h.bCnt);

        Skein_512_Process_Block(ctx, ctx->b, 1, ctx->h.bCnt);  /* process the final block */

        /* now output the result */
        byteCnt = (ctx->h.hashBitLen + 7) >> 3;             /* total number of output bytes */

        /* run Threefish in "counter mode" to generate output */
        memset(ctx->b, 0, sizeof(ctx->b));  /* zero out b[], so it can hold the counter */
        memcpy(X, ctx->X, sizeof(X));       /* keep a local copy of counter mode "key" */
        for (i = 0; i*SKEIN_512_BLOCK_BYTES < byteCnt; i++)
        {
            ((u64b_t *)ctx->b)[0] = Skein_Swap64((u64b_t)i); /* build the counter block */
            Skein_Start_New_Type(ctx, OUT_FINAL);
            Skein_512_Process_Block(ctx, ctx->b, 1, sizeof(u64b_t)); /* run "counter mode" */
            n = byteCnt - i*SKEIN_512_BLOCK_BYTES;   /* number of output bytes left to go */
            if (n >= SKEIN_512_BLOCK_BYTES)
                n = SKEIN_512_BLOCK_BYTES;
            Skein_Put64_LSB_First(hashVal + i*SKEIN_512_BLOCK_BYTES, ctx->X, n);   /* "output" the ctr mode bytes */
            Skein_Show_Final(512, &ctx->h, n, hashVal + i*SKEIN_512_BLOCK_BYTES);
            memcpy(ctx->X, X, sizeof(X));   /* restore the counter mode key for next time */
        }
        return SKEIN_SUCCESS;
    }

#if defined(SKEIN_CODE_SIZE) || defined(SKEIN_PERF)
    size_t Skein_512_API_CodeSize(void)
    {
        return ((u08b_t *)Skein_512_API_CodeSize) -
            ((u08b_t *)Skein_512_Init);
    }
#endif

    /*****************************************************************/
    /*    1024-bit Skein                                             */
    /*****************************************************************/

    /*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
    /* init the context for a straight hashing operation  */
    int Skein1024_Init(Skein1024_Ctxt_t *ctx, size_t hashBitLen)
    {
        union
        {
            u08b_t  b[SKEIN1024_STATE_BYTES];
            u64b_t  w[SKEIN1024_STATE_WORDS];
        } cfg;                              /* config block */

        Skein_Assert(hashBitLen > 0, SKEIN_BAD_HASHLEN);
        ctx->h.hashBitLen = hashBitLen;         /* output hash bit count */

        switch (hashBitLen)
        {              /* use pre-computed values, where available */
#ifndef SKEIN_NO_PRECOMP
            case  512: memcpy(ctx->X, SKEIN1024_IV_512, sizeof(ctx->X)); break;
            case  384: memcpy(ctx->X, SKEIN1024_IV_384, sizeof(ctx->X)); break;
            case 1024: memcpy(ctx->X, SKEIN1024_IV_1024, sizeof(ctx->X)); break;
#endif
            default:
                /* here if there is no precomputed IV value available */
                /* build/process the config block, type == CONFIG (could be precomputed) */
                Skein_Start_New_Type(ctx, CFG_FINAL);        /* set tweaks: T0=0; T1=CFG | FINAL */

                cfg.w[0] = Skein_Swap64(SKEIN_SCHEMA_VER);  /* set the schema, version */
                cfg.w[1] = Skein_Swap64(hashBitLen);        /* hash result length in bits */
                cfg.w[2] = Skein_Swap64(SKEIN_CFG_TREE_INFO_SEQUENTIAL);
                memset(&cfg.w[3], 0, sizeof(cfg)-3 * sizeof(cfg.w[0])); /* zero pad config block */

                /* compute the initial chaining values from config block */
                memset(ctx->X, 0, sizeof(ctx->X));            /* zero the chaining variables */
                Skein1024_Process_Block(ctx, cfg.b, 1, SKEIN_CFG_STR_LEN);
                break;
        }

        /* The chaining vars ctx->X are now initialized for the given hashBitLen. */
        /* Set up to process the data message portion of the hash (default) */
        Skein_Start_New_Type(ctx, MSG);              /* T0=0, T1= MSG type */

        return SKEIN_SUCCESS;
    }

    /*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
    /* init the context for a MAC and/or tree hash operation */
    /* [identical to Skein1024_Init() when keyBytes == 0 && treeInfo == SKEIN_CFG_TREE_INFO_SEQUENTIAL] */
    int Skein1024_InitExt(Skein1024_Ctxt_t *ctx, size_t hashBitLen, u64b_t treeInfo, const u08b_t *key, size_t keyBytes)
    {
        union
        {
            u08b_t  b[SKEIN1024_STATE_BYTES];
            u64b_t  w[SKEIN1024_STATE_WORDS];
        } cfg;                              /* config block */

        Skein_Assert(hashBitLen > 0, SKEIN_BAD_HASHLEN);
        Skein_Assert(keyBytes == 0 || key != NULL, SKEIN_FAIL);

        /* compute the initial chaining values ctx->X[], based on key */
        if (keyBytes == 0)                          /* is there a key? */
        {
            memset(ctx->X, 0, sizeof(ctx->X));        /* no key: use all zeroes as key for config block */
        }
        else                                        /* here to pre-process a key */
        {
            Skein_assert(sizeof(cfg.b) >= sizeof(ctx->X));
            /* do a mini-Init right here */
            ctx->h.hashBitLen = 8 * sizeof(ctx->X);     /* set output hash bit count = state size */
            Skein_Start_New_Type(ctx, KEY);          /* set tweaks: T0 = 0; T1 = KEY type */
            memset(ctx->X, 0, sizeof(ctx->X));        /* zero the initial chaining variables */
            Skein1024_Update(ctx, key, keyBytes);     /* hash the key */
            Skein1024_Final_Pad(ctx, cfg.b);         /* put result into cfg.b[] */
            memcpy(ctx->X, cfg.b, sizeof(cfg.b));     /* copy over into ctx->X[] */
#if SKEIN_NEED_SWAP
            {
                uint_t i;
                for (i = 0; i<SKEIN1024_STATE_WORDS; i++)   /* convert key bytes to context words */
                    ctx->X[i] = Skein_Swap64(ctx->X[i]);
            }
#endif
        }
        /* build/process the config block, type == CONFIG (could be precomputed for each key) */
        ctx->h.hashBitLen = hashBitLen;             /* output hash bit count */
        Skein_Start_New_Type(ctx, CFG_FINAL);

        memset(&cfg.w, 0, sizeof(cfg.w));             /* pre-pad cfg.w[] with zeroes */
        cfg.w[0] = Skein_Swap64(SKEIN_SCHEMA_VER);
        cfg.w[1] = Skein_Swap64(hashBitLen);        /* hash result length in bits */
        cfg.w[2] = Skein_Swap64(treeInfo);          /* tree hash config info (or SKEIN_CFG_TREE_INFO_SEQUENTIAL) */

        Skein_Show_Key(1024, &ctx->h, key, keyBytes);

        /* compute the initial chaining values from config block */
        Skein1024_Process_Block(ctx, cfg.b, 1, SKEIN_CFG_STR_LEN);

        /* The chaining vars ctx->X are now initialized */
        /* Set up to process the data message portion of the hash (default) */
        ctx->h.bCnt = 0;                            /* buffer b[] starts out empty */
        Skein_Start_New_Type(ctx, MSG);

        return SKEIN_SUCCESS;
    }

    /*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
    /* process the input bytes */
    int Skein1024_Update(Skein1024_Ctxt_t *ctx, const u08b_t *msg, size_t msgByteCnt)
    {
        size_t n;

        Skein_Assert(ctx->h.bCnt <= SKEIN1024_BLOCK_BYTES, SKEIN_FAIL);    /* catch uninitialized context */

        /* process full blocks, if any */
        if (msgByteCnt + ctx->h.bCnt > SKEIN1024_BLOCK_BYTES)
        {
            if (ctx->h.bCnt)                              /* finish up any buffered message data */
            {
                n = SKEIN1024_BLOCK_BYTES - ctx->h.bCnt;  /* # bytes free in buffer b[] */
                if (n)
                {
                    Skein_assert(n < msgByteCnt);         /* check on our logic here */
                    memcpy(&ctx->b[ctx->h.bCnt], msg, n);
                    msgByteCnt -= n;
                    msg += n;
                    ctx->h.bCnt += n;
                }
                Skein_assert(ctx->h.bCnt == SKEIN1024_BLOCK_BYTES);
                Skein1024_Process_Block(ctx, ctx->b, 1, SKEIN1024_BLOCK_BYTES);
                ctx->h.bCnt = 0;
            }
            /* now process any remaining full blocks, directly from input message data */
            if (msgByteCnt > SKEIN1024_BLOCK_BYTES)
            {
                n = (msgByteCnt - 1) / SKEIN1024_BLOCK_BYTES;   /* number of full blocks to process */
                Skein1024_Process_Block(ctx, msg, n, SKEIN1024_BLOCK_BYTES);
                msgByteCnt -= n * SKEIN1024_BLOCK_BYTES;
                msg += n * SKEIN1024_BLOCK_BYTES;
            }
            Skein_assert(ctx->h.bCnt == 0);
        }

        /* copy any remaining source message data bytes into b[] */
        if (msgByteCnt)
        {
            Skein_assert(msgByteCnt + ctx->h.bCnt <= SKEIN1024_BLOCK_BYTES);
            memcpy(&ctx->b[ctx->h.bCnt], msg, msgByteCnt);
            ctx->h.bCnt += msgByteCnt;
        }

        return SKEIN_SUCCESS;
    }

    /*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
    /* finalize the hash computation and output the result */
    int Skein1024_Final(Skein1024_Ctxt_t *ctx, u08b_t *hashVal)
    {
        size_t i, n, byteCnt;
        u64b_t X[SKEIN1024_STATE_WORDS];
        Skein_Assert(ctx->h.bCnt <= SKEIN1024_BLOCK_BYTES, SKEIN_FAIL);    /* catch uninitialized context */

        ctx->h.T[1] |= SKEIN_T1_FLAG_FINAL;                 /* tag as the final block */
        if (ctx->h.bCnt < SKEIN1024_BLOCK_BYTES)            /* zero pad b[] if necessary */
            memset(&ctx->b[ctx->h.bCnt], 0, SKEIN1024_BLOCK_BYTES - ctx->h.bCnt);

        Skein1024_Process_Block(ctx, ctx->b, 1, ctx->h.bCnt);  /* process the final block */

        /* now output the result */
        byteCnt = (ctx->h.hashBitLen + 7) >> 3;             /* total number of output bytes */

        /* run Threefish in "counter mode" to generate output */
        memset(ctx->b, 0, sizeof(ctx->b));  /* zero out b[], so it can hold the counter */
        memcpy(X, ctx->X, sizeof(X));       /* keep a local copy of counter mode "key" */
        for (i = 0; i*SKEIN1024_BLOCK_BYTES < byteCnt; i++)
        {
            ((u64b_t *)ctx->b)[0] = Skein_Swap64((u64b_t)i); /* build the counter block */
            Skein_Start_New_Type(ctx, OUT_FINAL);
            Skein1024_Process_Block(ctx, ctx->b, 1, sizeof(u64b_t)); /* run "counter mode" */
            n = byteCnt - i*SKEIN1024_BLOCK_BYTES;   /* number of output bytes left to go */
            if (n >= SKEIN1024_BLOCK_BYTES)
                n = SKEIN1024_BLOCK_BYTES;
            Skein_Put64_LSB_First(hashVal + i*SKEIN1024_BLOCK_BYTES, ctx->X, n);   /* "output" the ctr mode bytes */
            Skein_Show_Final(1024, &ctx->h, n, hashVal + i*SKEIN1024_BLOCK_BYTES);
            memcpy(ctx->X, X, sizeof(X));   /* restore the counter mode key for next time */
        }
        return SKEIN_SUCCESS;
    }

#if defined(SKEIN_CODE_SIZE) || defined(SKEIN_PERF)
    size_t Skein1024_API_CodeSize(void)
    {
        return ((u08b_t *)Skein1024_API_CodeSize) -
            ((u08b_t *)Skein1024_Init);
    }
#endif

    /**************** Functions to support MAC/tree hashing ***************/
    /*   (this code is identical for Optimized and Reference versions)    */

    /*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
    /* finalize the hash computation and output the block, no OUTPUT stage */
    int Skein_256_Final_Pad(Skein_256_Ctxt_t *ctx, u08b_t *hashVal)
    {
        Skein_Assert(ctx->h.bCnt <= SKEIN_256_BLOCK_BYTES, SKEIN_FAIL);    /* catch uninitialized context */

        ctx->h.T[1] |= SKEIN_T1_FLAG_FINAL;        /* tag as the final block */
        if (ctx->h.bCnt < SKEIN_256_BLOCK_BYTES)   /* zero pad b[] if necessary */
            memset(&ctx->b[ctx->h.bCnt], 0, SKEIN_256_BLOCK_BYTES - ctx->h.bCnt);
        Skein_256_Process_Block(ctx, ctx->b, 1, ctx->h.bCnt);    /* process the final block */

        Skein_Put64_LSB_First(hashVal, ctx->X, SKEIN_256_BLOCK_BYTES);   /* "output" the state bytes */

        return SKEIN_SUCCESS;
    }

    /*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
    /* finalize the hash computation and output the block, no OUTPUT stage */
    int Skein_512_Final_Pad(Skein_512_Ctxt_t *ctx, u08b_t *hashVal)
    {
        Skein_Assert(ctx->h.bCnt <= SKEIN_512_BLOCK_BYTES, SKEIN_FAIL);    /* catch uninitialized context */

        ctx->h.T[1] |= SKEIN_T1_FLAG_FINAL;        /* tag as the final block */
        if (ctx->h.bCnt < SKEIN_512_BLOCK_BYTES)   /* zero pad b[] if necessary */
            memset(&ctx->b[ctx->h.bCnt], 0, SKEIN_512_BLOCK_BYTES - ctx->h.bCnt);
        Skein_512_Process_Block(ctx, ctx->b, 1, ctx->h.bCnt);    /* process the final block */

        Skein_Put64_LSB_First(hashVal, ctx->X, SKEIN_512_BLOCK_BYTES);   /* "output" the state bytes */

        return SKEIN_SUCCESS;
    }

    /*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
    /* finalize the hash computation and output the block, no OUTPUT stage */
    int Skein1024_Final_Pad(Skein1024_Ctxt_t *ctx, u08b_t *hashVal)
    {
        Skein_Assert(ctx->h.bCnt <= SKEIN1024_BLOCK_BYTES, SKEIN_FAIL);    /* catch uninitialized context */

        ctx->h.T[1] |= SKEIN_T1_FLAG_FINAL;        /* tag as the final block */
        if (ctx->h.bCnt < SKEIN1024_BLOCK_BYTES)   /* zero pad b[] if necessary */
            memset(&ctx->b[ctx->h.bCnt], 0, SKEIN1024_BLOCK_BYTES - ctx->h.bCnt);
        Skein1024_Process_Block(ctx, ctx->b, 1, ctx->h.bCnt);    /* process the final block */

        Skein_Put64_LSB_First(hashVal, ctx->X, SKEIN1024_BLOCK_BYTES);   /* "output" the state bytes */

        return SKEIN_SUCCESS;
    }

#if SKEIN_TREE_HASH
    /*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
    /* just do the OUTPUT stage                                       */
    int Skein_256_Output(Skein_256_Ctxt_t *ctx, u08b_t *hashVal)
    {
        size_t i, n, byteCnt;
        u64b_t X[SKEIN_256_STATE_WORDS];
        Skein_Assert(ctx->h.bCnt <= SKEIN_256_BLOCK_BYTES, SKEIN_FAIL);    /* catch uninitialized context */

        /* now output the result */
        byteCnt = (ctx->h.hashBitLen + 7) >> 3;    /* total number of output bytes */

        /* run Threefish in "counter mode" to generate output */
        memset(ctx->b, 0, sizeof(ctx->b));  /* zero out b[], so it can hold the counter */
        memcpy(X, ctx->X, sizeof(X));       /* keep a local copy of counter mode "key" */
        for (i = 0; i*SKEIN_256_BLOCK_BYTES < byteCnt; i++)
        {
            ((u64b_t *)ctx->b)[0] = Skein_Swap64((u64b_t)i); /* build the counter block */
            Skein_Start_New_Type(ctx, OUT_FINAL);
            Skein_256_Process_Block(ctx, ctx->b, 1, sizeof(u64b_t)); /* run "counter mode" */
            n = byteCnt - i*SKEIN_256_BLOCK_BYTES;   /* number of output bytes left to go */
            if (n >= SKEIN_256_BLOCK_BYTES)
                n = SKEIN_256_BLOCK_BYTES;
            Skein_Put64_LSB_First(hashVal + i*SKEIN_256_BLOCK_BYTES, ctx->X, n);   /* "output" the ctr mode bytes */
            Skein_Show_Final(256, &ctx->h, n, hashVal + i*SKEIN_256_BLOCK_BYTES);
            memcpy(ctx->X, X, sizeof(X));   /* restore the counter mode key for next time */
        }
        return SKEIN_SUCCESS;
    }

    /*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
    /* just do the OUTPUT stage                                       */
    int Skein_512_Output(Skein_512_Ctxt_t *ctx, u08b_t *hashVal)
    {
        size_t i, n, byteCnt;
        u64b_t X[SKEIN_512_STATE_WORDS];
        Skein_Assert(ctx->h.bCnt <= SKEIN_512_BLOCK_BYTES, SKEIN_FAIL);    /* catch uninitialized context */

        /* now output the result */
        byteCnt = (ctx->h.hashBitLen + 7) >> 3;    /* total number of output bytes */

        /* run Threefish in "counter mode" to generate output */
        memset(ctx->b, 0, sizeof(ctx->b));  /* zero out b[], so it can hold the counter */
        memcpy(X, ctx->X, sizeof(X));       /* keep a local copy of counter mode "key" */
        for (i = 0; i*SKEIN_512_BLOCK_BYTES < byteCnt; i++)
        {
            ((u64b_t *)ctx->b)[0] = Skein_Swap64((u64b_t)i); /* build the counter block */
            Skein_Start_New_Type(ctx, OUT_FINAL);
            Skein_512_Process_Block(ctx, ctx->b, 1, sizeof(u64b_t)); /* run "counter mode" */
            n = byteCnt - i*SKEIN_512_BLOCK_BYTES;   /* number of output bytes left to go */
            if (n >= SKEIN_512_BLOCK_BYTES)
                n = SKEIN_512_BLOCK_BYTES;
            Skein_Put64_LSB_First(hashVal + i*SKEIN_512_BLOCK_BYTES, ctx->X, n);   /* "output" the ctr mode bytes */
            Skein_Show_Final(256, &ctx->h, n, hashVal + i*SKEIN_512_BLOCK_BYTES);
            memcpy(ctx->X, X, sizeof(X));   /* restore the counter mode key for next time */
        }
        return SKEIN_SUCCESS;
    }

    /*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
    /* just do the OUTPUT stage                                       */
    int Skein1024_Output(Skein1024_Ctxt_t *ctx, u08b_t *hashVal)
    {
        size_t i, n, byteCnt;
        u64b_t X[SKEIN1024_STATE_WORDS];
        Skein_Assert(ctx->h.bCnt <= SKEIN1024_BLOCK_BYTES, SKEIN_FAIL);    /* catch uninitialized context */

        /* now output the result */
        byteCnt = (ctx->h.hashBitLen + 7) >> 3;    /* total number of output bytes */

        /* run Threefish in "counter mode" to generate output */
        memset(ctx->b, 0, sizeof(ctx->b));  /* zero out b[], so it can hold the counter */
        memcpy(X, ctx->X, sizeof(X));       /* keep a local copy of counter mode "key" */
        for (i = 0; i*SKEIN1024_BLOCK_BYTES < byteCnt; i++)
        {
            ((u64b_t *)ctx->b)[0] = Skein_Swap64((u64b_t)i); /* build the counter block */
            Skein_Start_New_Type(ctx, OUT_FINAL);
            Skein1024_Process_Block(ctx, ctx->b, 1, sizeof(u64b_t)); /* run "counter mode" */
            n = byteCnt - i*SKEIN1024_BLOCK_BYTES;   /* number of output bytes left to go */
            if (n >= SKEIN1024_BLOCK_BYTES)
                n = SKEIN1024_BLOCK_BYTES;
            Skein_Put64_LSB_First(hashVal + i*SKEIN1024_BLOCK_BYTES, ctx->X, n);   /* "output" the ctr mode bytes */
            Skein_Show_Final(256, &ctx->h, n, hashVal + i*SKEIN1024_BLOCK_BYTES);
            memcpy(ctx->X, X, sizeof(X));   /* restore the counter mode key for next time */
        }
        return SKEIN_SUCCESS;
    }
#endif

    // SHA3api_ref.c

    /***********************************************************************
    **
    ** Implementation of the AHS API using the Skein hash function.
    **
    ** Source code author: Doug Whiting, 2008.
    **
    ** This algorithm and source code is released to the public domain.
    **
    ************************************************************************/

    /******************************************************************/
    /*     AHS API code                                               */
    /******************************************************************/

    /*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
    /* select the context size and init the context */
    HashReturn Init(hashState *state, int hashbitlen)
    {
        if (hashbitlen <= SKEIN_256_NIST_MAX_HASHBITS)
        {
            Skein_Assert(hashbitlen > 0, BAD_HASHLEN);
            state->statebits = 64 * SKEIN_256_STATE_WORDS;
            return (HashReturn)Skein_256_Init(&state->u.ctx_256, (size_t)hashbitlen);
        }
        if (hashbitlen <= SKEIN_512_NIST_MAX_HASHBITS)
        {
            state->statebits = 64 * SKEIN_512_STATE_WORDS;
            return (HashReturn)Skein_512_Init(&state->u.ctx_512, (size_t)hashbitlen);
        }
        else
        {
            state->statebits = 64 * SKEIN1024_STATE_WORDS;
            return (HashReturn)Skein1024_Init(&state->u.ctx1024, (size_t)hashbitlen);
        }
    }

    /*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
    /* process data to be hashed */
    HashReturn Update(hashState *state, const BitSequence *data, DataLength databitlen)
    {
        /* only the final Update() call is allowed do partial bytes, else assert an error */
        Skein_Assert((state->u.h.T[1] & SKEIN_T1_FLAG_BIT_PAD) == 0 || databitlen == 0, FAIL);

        Skein_Assert(state->statebits % 256 == 0 && (state->statebits - 256) < 1024, FAIL);
        if ((databitlen & 7) == 0)  /* partial bytes? */
        {
            switch ((state->statebits >> 8) & 3)
            {
                case 2:  return (HashReturn)Skein_512_Update(&state->u.ctx_512, data, databitlen >> 3);
                case 1:  return (HashReturn)Skein_256_Update(&state->u.ctx_256, data, databitlen >> 3);
                case 0:  return (HashReturn)Skein1024_Update(&state->u.ctx1024, data, databitlen >> 3);
                default: return FAIL;
            }
        }
        else
        {   /* handle partial final byte */
            size_t bCnt = (databitlen >> 3) + 1;                  /* number of bytes to handle (nonzero here!) */
            u08b_t b, mask;

            mask = (u08b_t)(1u << (7 - (databitlen & 7)));       /* partial byte bit mask */
            b = (u08b_t)((data[bCnt - 1] & (0 - mask)) | mask);   /* apply bit padding on final byte */

            switch ((state->statebits >> 8) & 3)
            {
                case 2:  Skein_512_Update(&state->u.ctx_512, data, bCnt - 1); /* process all but the final byte    */
                    Skein_512_Update(&state->u.ctx_512, &b, 1); /* process the (masked) partial byte */
                    break;
                case 1:  Skein_256_Update(&state->u.ctx_256, data, bCnt - 1); /* process all but the final byte    */
                    Skein_256_Update(&state->u.ctx_256, &b, 1); /* process the (masked) partial byte */
                    break;
                case 0:  Skein1024_Update(&state->u.ctx1024, data, bCnt - 1); /* process all but the final byte    */
                    Skein1024_Update(&state->u.ctx1024, &b, 1); /* process the (masked) partial byte */
                    break;
                default: return FAIL;
            }
            Skein_Set_Bit_Pad_Flag(state->u.h);                    /* set tweak flag for the final call */

            return SUCCESS;
        }
    }

    /*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
    /* finalize hash computation and output the result (hashbitlen bits) */
    HashReturn Final(hashState *state, BitSequence *hashval)
    {
        Skein_Assert(state->statebits % 256 == 0 && (state->statebits - 256) < 1024, FAIL);
        switch ((state->statebits >> 8) & 3)
        {
            case 2:  return (HashReturn)Skein_512_Final(&state->u.ctx_512, hashval);
            case 1:  return (HashReturn)Skein_256_Final(&state->u.ctx_256, hashval);
            case 0:  return (HashReturn)Skein1024_Final(&state->u.ctx1024, hashval);
            default: return FAIL;
        }
    }

    /*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
    /* all-in-one hash function */
    HashReturn Hash(int hashbitlen, const BitSequence *data, /* all-in-one call */
        DataLength databitlen, BitSequence *hashval)
    {
        hashState  state;
        HashReturn r = Init(&state, hashbitlen);
        if (r == SUCCESS)
        { /* these calls do not fail when called properly */
            r = Update(&state, data, databitlen);
            Final(&state, hashval);
        }
        return r;
    }

}