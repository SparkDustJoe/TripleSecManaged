/*******************************/
/* luffa_for_64.h              */
/* Version 2.0 (Sep 15th 2009) */
/* Made by Hitachi Ltd.        */
/*******************************/

#pragma once

namespace LuffaOrg
{

/* The length of digests*/
#define DIGEST_BIT_LEN_224 224
#define DIGEST_BIT_LEN_256 256
#define DIGEST_BIT_LEN_384 384
#define DIGEST_BIT_LEN_512 512

/*********************************/
/* The parameters of Luffa       */
#define MSG_BLOCK_BIT_LEN 256  /*The bit length of a message block*/
#define MSG_BLOCK_BYTE_LEN (MSG_BLOCK_BIT_LEN >> 3) /* The byte length
                                                     * of a message block*/

/* The number of blocks in Luffa */
#define WIDTH_224 3
#define WIDTH_256 3
#define WIDTH_384 4
#define WIDTH_512 5

/* The limit of the length of message */
#define LIMIT_224 64
#define LIMIT_256 64
#define LIMIT_384 128
#define LIMIT_512 128
/*********************************/


typedef unsigned char BitSequence;
typedef unsigned long long DataLength;
typedef enum { SUCCESS = 0, FAIL = 1, BAD_HASHBITLEN = 2} HashReturn;

typedef unsigned char uint8;
typedef unsigned int uint32;
typedef unsigned long long ulong;

typedef struct {
    int hashbitlen;
    ulong bitlen[2]; /* Message length in bits */
    uint32 rembitlen; /* Length of buffer data to be hashed */
    uint32 buffer[8]; /* Buffer to be hashed */
    uint32 chainv[40];   /* Chaining values */
} hashState;

HashReturn Init(hashState *state, int hashbitlen);
HashReturn Update(hashState *state, const BitSequence *data, DataLength databitlen);
HashReturn Final(hashState *state, BitSequence *hashval);
HashReturn Hash(int hashbitlen, const BitSequence *data, DataLength databitlen, BitSequence *hashval);

}