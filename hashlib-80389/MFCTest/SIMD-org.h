
#pragma once

namespace SIMDOrg
{

typedef unsigned int u32;
typedef unsigned long long u64;
typedef int fft_t;

#define C32(x)    ((u32)(x ## U))
#define T32(x)    ((x) & C32(0xFFFFFFFF))
#define ROTL32(x, n)   T32(((x) << (n)) | ((x) >> (32 - (n))))

typedef unsigned char BitSequence;
typedef u64 DataLength;
typedef enum { SUCCESS = 0, FAIL = 1, BAD_HASHBITLEN = 2} HashReturn;

struct hashState
{
  void* p1;
  void* p2;

  unsigned int hashbitlen;
  unsigned int blocksize;
  unsigned int n_feistels;
  u64 count;
  u32 *A, *B, *C, *D;
  unsigned char* buffer;

  hashState()
  {
      p1 = nullptr;
      p2 = nullptr;
  }

  ~hashState()
  {
      delete p1;
      delete p2;
  }
};

/* 
 * NIST API
 */

HashReturn Init(hashState *state, int hashbitlen);
HashReturn Update(hashState *state, const BitSequence *data, DataLength databitlen);
HashReturn Final(hashState *state, BitSequence *hashval);
HashReturn Hash(int hashbitlen, const BitSequence *data, DataLength databitlen,
                BitSequence *hashval);
}