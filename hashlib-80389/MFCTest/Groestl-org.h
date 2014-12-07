
#pragma once

namespace GroestlOrg
{

    typedef unsigned int u32;
    typedef unsigned long long u64;
    typedef unsigned char BitSequence;
    typedef unsigned char u8;
    typedef unsigned long long DataLength;

    /* some sizes (number of bytes) */
#define ROWS 8
#define LENGTHFIELDLEN ROWS
#define COLS512 8
#define COLS1024 16
#define SIZE512 (ROWS*COLS512)
#define SIZE1024 (ROWS*COLS1024)

#define ROUNDS512 10
#define ROUNDS1024 14

#define ROTL64(a,n) ((((a)<<(n))|((a)>>(64-(n))))&(0xffffffffffffffffULL))

#define EXT_BYTE(var,n) ((u8)((u64)(var) >> (8*n)))
#define U64BIG(a) \
    ((ROTL64(a, 8) & (0x000000FF000000FFULL)) | \
    (ROTL64(a, 24) & (0x0000FF000000FF00ULL)) | \
    (ROTL64(a, 40) & (0x00FF000000FF0000ULL)) | \
    (ROTL64(a, 56) & (0xFF000000FF000000ULL)))

    enum { LONG = SIZE1024, SHORT = SIZE512 };


    /* NIST API begin */
    typedef unsigned char BitSequence;
    typedef unsigned long long DataLength;
    typedef enum { SUCCESS = 0, FAIL = 1, BAD_HASHLEN = 2 } HashReturn;
    typedef struct {
        u64 *chaining;            /* actual state */
        BitSequence *buffer;      /* data buffer */
        u64 block_counter;        /* message block counter */
        int buf_ptr;              /* data buffer pointer */
        int bits_in_last_byte;    /* no. of message bits in last byte of
                         data buffer */
        int hashbitlen;           /* output length in bits */
        int size;                 /* LONG or SHORT */
    } hashState;

    HashReturn Init(hashState*, int);
    HashReturn Update(hashState*, const BitSequence*, DataLength);
    HashReturn Final(hashState*, BitSequence*);
    HashReturn Hash(int, const BitSequence*, DataLength, BitSequence*);
    /* NIST API end   */

    /* helper functions */
    void PrintHash(const BitSequence*, int);

}