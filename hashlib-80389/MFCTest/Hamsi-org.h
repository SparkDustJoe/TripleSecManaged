
#pragma once

namespace HamsiOrg
{

typedef unsigned char  Byte;            //  8 bits
typedef unsigned int   Word;            // 32 bits
typedef unsigned long long int U64;     // 64 bits

typedef unsigned char BitSequence;
typedef unsigned long long DataLength;
typedef enum { SUCCESS=0, FAIL=1, BAD_HASHBITLEN=2 } HashReturn;

typedef struct {
    int hashbitlen;
    int leftbits;
    Byte leftdata[8];
    Word state[16];     // chain value
    Word counter;

    int cvsize;
    // int blocksize;
    int ROUNDS;
    int PFROUNDS;
} hashState;

HashReturn   Init(hashState *state, int hashbitlen);
HashReturn Update(hashState *state, const BitSequence* data, DataLength databitlen);
HashReturn  Final(hashState *state, BitSequence* hashval);
HashReturn   Hash(int hashbitlen, const BitSequence *data, DataLength databitlen, BitSequence *hashval);

}