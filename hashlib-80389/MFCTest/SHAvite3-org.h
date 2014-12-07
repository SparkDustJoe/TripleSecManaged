
#pragma once 

namespace SHAvite3Org
{
#define NESSIE_LITTLE_ENDIAN
#define NATIVE64
#define NATIVE_WORD_SIZE_B 8
#define BIG_ENDIAN 4321
#define LITTLE_ENDIAN 1234
#define BYTE_ORDER 1234

#define i8_t char
    typedef signed char s8;
    typedef unsigned char u8;

#define i16_t short
#define i32_t int
    typedef signed short s16;
    typedef signed int s32;
    typedef unsigned short u16;
    typedef unsigned int u32;

#define ONE32   0xFFFFFFFFU

#define ONE8    0xFFU
#define ONE16   0xFFFFU

#define T8(x)   ((x) & ONE8)
#define T16(x)  ((x) & ONE16)
#define T32(x)  ((x) & ONE32)

#define i64_t __int64
    typedef unsigned __int64 u64;
    typedef signed __int64 s64;
#define LL(v)   (v##i64)

#define ONE64   LL(0xFFFFFFFFFFFFFFFF)
#define T64(x)  ((x) & ONE64)

    typedef u64 unative;

#define U8TO16_BIG(c)  (((u16)T8(*((u8*)(c))) << 8) | ((u16)T8(*(((u8*)(c)) + 1))))
#define U8TO16_LITTLE(c)  (((u16*)(c))[0])

#define U16TO8_BIG(c, v)    do { \
    u16 tmp_portable_h_x = (v); \
    u8 *tmp_portable_h_d = (c); \
    tmp_portable_h_d[0] = T8(tmp_portable_h_x >> 8); \
    tmp_portable_h_d[1] = T8(tmp_portable_h_x); \
    } while (0)

#define U16TO8_LITTLE(c, v) ((u16*)(c))[0]=v

#define U8TO32_BIG(c)  (((u32)T8(*((u8*)(c))) << 24) | \
    ((u32)T8(*(((u8*)(c)) + 1)) << 16) | \
    ((u32)T8(*(((u8*)(c)) + 2)) << 8) | \
    ((u32)T8(*(((u8*)(c)) + 3))))

#define U8TO32_LITTLE(c)  (((u32*)(c))[0])

#define U32TO8_BIG(c, v)    do { \
    u32 tmp_portable_h_x = (v); \
    u8 *tmp_portable_h_d = (c); \
    tmp_portable_h_d[0] = T8(tmp_portable_h_x >> 24); \
    tmp_portable_h_d[1] = T8(tmp_portable_h_x >> 16); \
    tmp_portable_h_d[2] = T8(tmp_portable_h_x >> 8); \
    tmp_portable_h_d[3] = T8(tmp_portable_h_x); \
    } while (0)

#define U32TO8_LITTLE(c, v) ((u32*)(c))[0]=v

#define U8TO64_BIG(c)  (((u64)T8(*(c)) << 56) | ((u64)T8(*((c) + 1)) << 48) | \
    ((u64)T8(*((c) + 2)) << 40) | ((u64)T8(*((c) + 3)) << 32) | \
    ((u64)T8(*((c) + 4)) << 24) | ((u64)T8(*((c) + 5)) << 16) | \
    ((u64)T8(*((c) + 6)) <<  8) | ((u64)T8(*((c) + 7))))

#define U8TO64_LITTLE(c)  (((u64*)(c))[0])

#define U64TO8_BIG(c, v)    do { \
    u64 tmp_portable_h_x = (v); \
    u8 *tmp_portable_h_d = (c); \
    tmp_portable_h_d[0] = T8(tmp_portable_h_x >> 56); \
    tmp_portable_h_d[1] = T8(tmp_portable_h_x >> 48); \
    tmp_portable_h_d[2] = T8(tmp_portable_h_x >> 40); \
    tmp_portable_h_d[3] = T8(tmp_portable_h_x >> 32); \
    tmp_portable_h_d[4] = T8(tmp_portable_h_x >> 24); \
    tmp_portable_h_d[5] = T8(tmp_portable_h_x >> 16); \
    tmp_portable_h_d[6] = T8(tmp_portable_h_x >> 8); \
    tmp_portable_h_d[7] = T8(tmp_portable_h_x); \
    } while (0)

#define U64TO8_LITTLE(c, v) ((u64*)(c))[0]=v

#define ROTL32(v, n)   (T32((v) << ((n)&0x1F)) | \
    T32((v) >> (32 - ((n)&0x1F))))
#define ROTR32(v, n)   (T32((v) >> ((n)&0x1F)) | \
    T32((v) << (32 - ((n)&0x1F))))

#define ROTL16(v, n)   (T16((v) << ((n)&0xF)) | \
    T16((v) >> (16 - ((n)&0xF))))
#define ROTR16(v, n)   (T16((v) >> ((n)&0xF)) | \
    T16((v) << (16 - ((n)&0xF))))

#define ROTL64(v, n)   (T64((v) << ((n)&0x3F)) | \
    T64((v) >> (64 - ((n)&0x3F))))
#define ROTR64(v, n)   (T64((v) >> ((n)&0x3F)) | \
    T64((v) << (64 - ((n)&0x3F))))

#define bzero(p,l)    memset((p), 0x00, (l))
#define bcopy(s,d,l) memcpy((d), (s), (l))

    /* This function cannot be a macro, as in some of it's calls,
    /*  "x++" is passed as an argument, thus increasing x 4 times instead of one */
    static inline u32 swap32(u32 x) 
    { 
        return ((((x)&    0xFF)<<24) | (((x)&    0xFF00)<<8) | \
            (((x)&0xFF0000)>> 8) | (((x)&0xFF000000)>>24)); 
    }

#define ntohl(x) swap32(x)
#define htonl(x) swap32(x)

    typedef unsigned char BitSequence;

    typedef unsigned long long DataLength;

    typedef enum {SUCCESS=0, FAIL=1, BAD_HASHBITLEN=2} HashReturn;

    /* SHAvite-3 definition */

    typedef struct {
        DataLength bitcount;            /* The number of bits compressed so far   */
        BitSequence chaining_value[64]; /* An array containing the chaining value */
        BitSequence buffer[128];        /* A buffer storing bytes until they are  */
        /* compressed			     */
        BitSequence partial_byte;       /* A byte to store a fraction of a byte   */
        /* in case the input is not fully byte    */
        /* alligned				     */
        BitSequence salt[64];           /* The salt used in the hash function     */ 
        int DigestSize;		   /* The requested digest size              */
        int BlockSize;		   /* The message block size                 */
    } hashState;

    /* Function calls imposed by the API */

    HashReturn Init (hashState *state, int hashbitlen);

    HashReturn Update (hashState *state, const BitSequence *data, DataLength
        databitlen);

    HashReturn Final (hashState *state, BitSequence *hashval);

    HashReturn Hash (int hashbitlen, const BitSequence *data, 
        DataLength databitlen, BitSequence *hashval);

}