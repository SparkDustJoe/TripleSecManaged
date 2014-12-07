
#include "stdafx.h"
#include "SIMD-org.h"

namespace SIMDOrg
{

u32 IV_224[] = {
  0x33586e9f, 0x12fff033, 0xb2d9f64d, 0x6f8fea53,
  0xde943106, 0x2742e439, 0x4fbab5ac, 0x62b9ff96,
  0x22e7b0af, 0xc862b3a8, 0x33e00cdc, 0x236b86a6,
  0xf64ae77c, 0xfa373b76, 0x7dc1ee5b, 0x7fb29ce8
};

u32 IV_256[] = {
  0x4d567983, 0x07190ba9, 0x8474577b, 0x39d726e9,
  0xaaf3d925, 0x3ee20b03, 0xafd5e751, 0xc96006d3,
  0xc2c2ba14, 0x49b3bcb4, 0xf67caf46, 0x668626c9,
  0xe2eaa8d2, 0x1ff47833, 0xd0c661a5, 0x55693de1
};

u32 IV_384[] = {
  0x8a36eebc, 0x94a3bd90, 0xd1537b83, 0xb25b070b, 0xf463f1b5, 0xb6f81e20, 0x0055c339, 0xb4d144d1,
  0x7360ca61, 0x18361a03, 0x17dcb4b9, 0x3414c45a, 0xa699a9d2, 0xe39e9664, 0x468bfe77, 0x51d062f8,
  0xb9e3bfe8, 0x63bece2a, 0x8fe506b9, 0xf8cc4ac2, 0x7ae11542, 0xb1aadda1, 0x64b06794, 0x28d2f462,
  0xe64071ec, 0x1deb91a8, 0x8ac8db23, 0x3f782ab5, 0x039b5cb8, 0x71ddd962, 0xfade2cea, 0x1416df71
};

u32 IV_512[] = {
  0x0ba16b95, 0x72f999ad, 0x9fecc2ae, 0xba3264fc, 0x5e894929, 0x8e9f30e5, 0x2f1daa37, 0xf0f2c558,
  0xac506643, 0xa90635a5, 0xe25b878b, 0xaab7878f, 0x88817f7a, 0x0a02892b, 0x559a7550, 0x598f657e,
  0x7eef60a1, 0x6b70e3e8, 0x9c1714d1, 0xb958e2a8, 0xab02675e, 0xed1c014f, 0xcd8d65bb, 0xfdb7a257,
  0x09254899, 0xd699c7bc, 0x9019b6dc, 0x2b9022e4, 0x8fa14956, 0x21bf9bd3, 0xb94d0943, 0x6ffddc22
};

    static const int P4[32][4] = {
{ 2, 34, 18, 50 },
{ 6, 38, 22, 54 },
{ 0, 32, 16, 48 },
{ 4, 36, 20, 52 },
{ 14, 46, 30, 62 },
{ 10, 42, 26, 58 },
{ 12, 44, 28, 60 },
{ 8, 40, 24, 56 },
{ 15, 47, 31, 63 },
{ 13, 45, 29, 61 },
{ 3, 35, 19, 51 },
{ 1, 33, 17, 49 },
{ 9, 41, 25, 57 },
{ 11, 43, 27, 59 },
{ 5, 37, 21, 53 },
{ 7, 39, 23, 55 },
{ 8, 40, 24, 56 },
{ 4, 36, 20, 52 },
{ 14, 46, 30, 62 },
{ 2, 34, 18, 50 },
{ 6, 38, 22, 54 },
{ 10, 42, 26, 58 },
{ 0, 32, 16, 48 },
{ 12, 44, 28, 60 },
{ 70, 102, 86, 118 },
{ 64, 96, 80, 112 },
{ 72, 104, 88, 120 },
{ 78, 110, 94, 126 },
{ 76, 108, 92, 124 },
{ 74, 106, 90, 122 },
{ 66, 98, 82, 114 },
{ 68, 100, 84, 116 }
};

static const int Q4[32][4] = {
{ 66, 98, 82, 114 },
{ 70, 102, 86, 118 },
{ 64, 96, 80, 112 },
{ 68, 100, 84, 116 },
{ 78, 110, 94, 126 },
{ 74, 106, 90, 122 },
{ 76, 108, 92, 124 },
{ 72, 104, 88, 120 },
{ 79, 111, 95, 127 },
{ 77, 109, 93, 125 },
{ 67, 99, 83, 115 },
{ 65, 97, 81, 113 },
{ 73, 105, 89, 121 },
{ 75, 107, 91, 123 },
{ 69, 101, 85, 117 },
{ 71, 103, 87, 119 },
{ 9, 41, 25, 57 },
{ 5, 37, 21, 53 },
{ 15, 47, 31, 63 },
{ 3, 35, 19, 51 },
{ 7, 39, 23, 55 },
{ 11, 43, 27, 59 },
{ 1, 33, 17, 49 },
{ 13, 45, 29, 61 },
{ 71, 103, 87, 119 },
{ 65, 97, 81, 113 },
{ 73, 105, 89, 121 },
{ 79, 111, 95, 127 },
{ 77, 109, 93, 125 },
{ 75, 107, 91, 123 },
{ 67, 99, 83, 115 },
{ 69, 101, 85, 117 }
};


static const int P8[32][8] = {
{ 2, 66, 34, 98, 18, 82, 50, 114 },
{ 6, 70, 38, 102, 22, 86, 54, 118 },
{ 0, 64, 32, 96, 16, 80, 48, 112 },
{ 4, 68, 36, 100, 20, 84, 52, 116 },
{ 14, 78, 46, 110, 30, 94, 62, 126 },
{ 10, 74, 42, 106, 26, 90, 58, 122 },
{ 12, 76, 44, 108, 28, 92, 60, 124 },
{ 8, 72, 40, 104, 24, 88, 56, 120 },
{ 15, 79, 47, 111, 31, 95, 63, 127 },
{ 13, 77, 45, 109, 29, 93, 61, 125 },
{ 3, 67, 35, 99, 19, 83, 51, 115 },
{ 1, 65, 33, 97, 17, 81, 49, 113 },
{ 9, 73, 41, 105, 25, 89, 57, 121 },
{ 11, 75, 43, 107, 27, 91, 59, 123 },
{ 5, 69, 37, 101, 21, 85, 53, 117 },
{ 7, 71, 39, 103, 23, 87, 55, 119 },
{ 8, 72, 40, 104, 24, 88, 56, 120 },
{ 4, 68, 36, 100, 20, 84, 52, 116 },
{ 14, 78, 46, 110, 30, 94, 62, 126 },
{ 2, 66, 34, 98, 18, 82, 50, 114 },
{ 6, 70, 38, 102, 22, 86, 54, 118 },
{ 10, 74, 42, 106, 26, 90, 58, 122 },
{ 0, 64, 32, 96, 16, 80, 48, 112 },
{ 12, 76, 44, 108, 28, 92, 60, 124 },
{ 134, 198, 166, 230, 150, 214, 182, 246 },
{ 128, 192, 160, 224, 144, 208, 176, 240 },
{ 136, 200, 168, 232, 152, 216, 184, 248 },
{ 142, 206, 174, 238, 158, 222, 190, 254 },
{ 140, 204, 172, 236, 156, 220, 188, 252 },
{ 138, 202, 170, 234, 154, 218, 186, 250 },
{ 130, 194, 162, 226, 146, 210, 178, 242 },
{ 132, 196, 164, 228, 148, 212, 180, 244 },
};

static const int Q8[32][8] = {
{ 130, 194, 162, 226, 146, 210, 178, 242 },
{ 134, 198, 166, 230, 150, 214, 182, 246 },
{ 128, 192, 160, 224, 144, 208, 176, 240 },
{ 132, 196, 164, 228, 148, 212, 180, 244 },
{ 142, 206, 174, 238, 158, 222, 190, 254 },
{ 138, 202, 170, 234, 154, 218, 186, 250 },
{ 140, 204, 172, 236, 156, 220, 188, 252 },
{ 136, 200, 168, 232, 152, 216, 184, 248 },
{ 143, 207, 175, 239, 159, 223, 191, 255 },
{ 141, 205, 173, 237, 157, 221, 189, 253 },
{ 131, 195, 163, 227, 147, 211, 179, 243 },
{ 129, 193, 161, 225, 145, 209, 177, 241 },
{ 137, 201, 169, 233, 153, 217, 185, 249 },
{ 139, 203, 171, 235, 155, 219, 187, 251 },
{ 133, 197, 165, 229, 149, 213, 181, 245 },
{ 135, 199, 167, 231, 151, 215, 183, 247 },
{ 9, 73, 41, 105, 25, 89, 57, 121 },
{ 5, 69, 37, 101, 21, 85, 53, 117 },
{ 15, 79, 47, 111, 31, 95, 63, 127 },
{ 3, 67, 35, 99, 19, 83, 51, 115 },
{ 7, 71, 39, 103, 23, 87, 55, 119 },
{ 11, 75, 43, 107, 27, 91, 59, 123 },
{ 1, 65, 33, 97, 17, 81, 49, 113 },
{ 13, 77, 45, 109, 29, 93, 61, 125 },
{ 135, 199, 167, 231, 151, 215, 183, 247 },
{ 129, 193, 161, 225, 145, 209, 177, 241 },
{ 137, 201, 169, 233, 153, 217, 185, 249 },
{ 143, 207, 175, 239, 159, 223, 191, 255 },
{ 141, 205, 173, 237, 157, 221, 189, 253 },
{ 139, 203, 171, 235, 155, 219, 187, 251 },
{ 131, 195, 163, 227, 147, 211, 179, 243 },
{ 133, 197, 165, 229, 149, 213, 181, 245 },
};

  static const fft_t FFT64_8_8_Twiddle[] = {
    1,    1,    1,    1,    1,    1,    1,    1,
    1,    2,    4,    8,   16,   32,   64,  128,
    1,   60,    2,  120,    4,  -17,    8,  -34,
    1,  120,    8,  -68,   64,  -30,   -2,   17,
    1,   46,   60,  -67,    2,   92,  120,  123,
    1,   92,  -17,  -22,   32,  117,  -30,   67,
    1,  -67,  120,  -73,    8,  -22,  -68,  -70,
    1,  123,  -34,  -70,  128,   67,   17,   35,
  };

 static const fft_t FFT128_2_64_Twiddle[] =  {
    1, -118,   46,  -31,   60,  116,  -67,  -61,
    2,   21,   92,  -62,  120,  -25,  123, -122,
    4,   42,  -73, -124,  -17,  -50,  -11,   13,
    8,   84,  111,    9,  -34, -100,  -22,   26,
   16,  -89,  -35,   18,  -68,   57,  -44,   52,
   32,   79,  -70,   36,  121,  114,  -88,  104,
   64,  -99,  117,   72,  -15,  -29,   81,  -49,
  128,   59,  -23, -113,  -30,  -58,  -95,  -98
  };

static const fft_t FFT128_16_8_Twiddle[] =  {
1, 1, 1, 1, 1, 1, 1, 1, 
1, 2, 4, 8, 16, 32, 64, 128, 
1, 60, 2, 120, 4, -17, 8, -34, 
1, 120, 8, -68, 64, -30, -2, 17, 
1, 46, 60, -67, 2, 92, 120, 123, 
1, 92, -17, -22, 32, 117, -30, 67, 
1, -67, 120, -73, 8, -22, -68, -70, 
1, 123, -34, -70, 128, 67, 17, 35, 
1, -118, 46, -31, 60, 116, -67, -61, 
1, 21, -73, 9, -68, 114, 81, -98, 
1, 116, 92, -122, -17, 84, -22, 18, 
1, -25, 111, 52, -15, 118, -123, -9, 
1, -31, -67, 21, 120, -122, -73, -50, 
1, -62, -11, -89, 121, -49, -46, 25, 
1, -61, 123, -50, -34, 18, -70, -99, 
1, -122, -22, 114, -30, 62, -111, -79 };

 static const fft_t FFT128_8_16_Twiddle[] =  {
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
1, 60, 2, 120, 4, -17, 8, -34, 16, -68, 32, 121, 64, -15, 128, -30, 
1, 46, 60, -67, 2, 92, 120, 123, 4, -73, -17, -11, 8, 111, -34, -22, 
1, -67, 120, -73, 8, -22, -68, -70, 64, 81, -30, -46, -2, -123, 17, -111, 
1, -118, 46, -31, 60, 116, -67, -61, 2, 21, 92, -62, 120, -25, 123, -122, 
1, 116, 92, -122, -17, 84, -22, 18, 32, 114, 117, -49, -30, 118, 67, 62, 
1, -31, -67, 21, 120, -122, -73, -50, 8, 9, -22, -89, -68, 52, -70, 114, 
1, -61, 123, -50, -34, 18, -70, -99, 128, -98, 67, 25, 17, -9, 35, -79};


static const fft_t FFT256_2_128_Twiddle[] =  {
   1,   41, -118,   45,   46,   87,  -31,   14, 
  60, -110,  116, -127,  -67,   80,  -61,   69, 
   2,   82,   21,   90,   92,  -83,  -62,   28, 
 120,   37,  -25,    3,  123,  -97, -122, -119, 
   4,  -93,   42,  -77,  -73,   91, -124,   56, 
 -17,   74,  -50,    6,  -11,   63,   13,   19, 
   8,   71,   84,  103,  111,  -75,    9,  112, 
 -34, -109, -100,   12,  -22,  126,   26,   38, 
  16, -115,  -89,  -51,  -35,  107,   18,  -33, 
 -68,   39,   57,   24,  -44,   -5,   52,   76, 
  32,   27,   79, -102,  -70,  -43,   36,  -66, 
 121,   78,  114,   48,  -88,  -10,  104, -105, 
  64,   54,  -99,   53,  117,  -86,   72,  125, 
 -15, -101,  -29,   96,   81,  -20,  -49,   47, 
 128,  108,   59,  106,  -23,   85, -113,   -7, 
 -30,   55,  -58,  -65,  -95,  -40,  -98,   94};


char* VERSION(void) {
  return "OPTIMIZED, NON-VECTORIZED";
}

int SupportedLength(int hashbitlen) {
  if (hashbitlen <= 0 || hashbitlen > 512)
    return 0;
  else
    return 1;
}

/************* the round function ****************/

#define  IF(x, y, z)  ((((y) ^ (z)) & (x)) ^ (z))

#define MAJ(x, y, z)  (((z) & (y)) | (((z) | (y)) & (x)))

#define STEP4(state, w, i, r, s, A, B, C, D, F)				\
  {                                                                     \
    int j;                                                              \
    u32 R[4];                                                           \
                                \
    for(j=0; j<4; j++) {                                                \
      R[j] = ROTL32(state->A[j], r);                                    \
    }                                                                   \
    for(j=0; j<4; j++) {                                                \
      state->D[j] = state->D[j] + w[j] +                                \
        F(state->A[j], state->B[j], state->C[j]);                       \
      state->D[j] = T32(ROTL32(T32(state->D[j]), s) + R[(j^(((i)%3)+1))]); \
      state->A[j] = R[j];                                               \
    }                                                                   \
  }
  
const int p8_xor [] = {1, 6, 2, 3, 5, 7, 4};

#define tabsize(t) (sizeof(t)/sizeof(t[0]))

#define STEP8(state, w, i, r, s, A, B, C, D, F)				\
  {                                                                     \
    int j;                                                              \
    u32 R[8];                                                           \
                                    \
    for(j=0; j<8; j++) {                                                \
      R[j] = ROTL32(state->A[j], r);                                    \
    }                                                                   \
    for(j=0; j<8; j++) {                                                \
      state->D[j] = state->D[j] + w[j] +                                \
        F(state->A[j], state->B[j], state->C[j]);                       \
      state->D[j] = T32(ROTL32(T32(state->D[j]), s) +                   \
                        R[j^p8_xor[(i) % tabsize(p8_xor)]]);            \
      state->A[j] = R[j];                                               \
    }                                                                   \
  }

void Round4(hashState * state, fft_t y[128], int i,
           int r, int s, int t, int u) {
  fft_t code = i<2? 185: 233;
  u32 w[8][4];
  int a, b;

  /*
   * The FFT output y is in revbin permuted order,
   * but this is included in the tables P and Q
   */

  for(a=0; a<8; a++)
    for(b=0; b<4; b++)
      w[a][b] = (((u32) (y[Q4[8*i+a][b]] * code)) << 16) |
                (((u32) (y[P4[8*i+a][b]] * code)) & 0xffff);

  STEP4(state, w[0], 8*i+0, r, s, A, B, C, D, IF);
  STEP4(state, w[1], 8*i+1, s, t, D, A, B, C, IF);
  STEP4(state, w[2], 8*i+2, t, u, C, D, A, B, IF);
  STEP4(state, w[3], 8*i+3, u, r, B, C, D, A, IF);

  STEP4(state, w[4], 8*i+4, r, s, A, B, C, D, MAJ);
  STEP4(state, w[5], 8*i+5, s, t, D, A, B, C, MAJ);
  STEP4(state, w[6], 8*i+6, t, u, C, D, A, B, MAJ);
  STEP4(state, w[7], 8*i+7, u, r, B, C, D, A, MAJ);
}

void Round8(hashState * state, fft_t y[128], int i,
           int r, int s, int t, int u) {
  fft_t code = i<2? 185: 233;
  u32 w[8][8];
  int a, b;

  /*
   * The FFT output y is in revbin permuted order,
   * but this is included in the tables P and Q
   */

  for(a=0; a<8; a++)
    for(b=0; b<8; b++)
      w[a][b] = (((u32) (y[Q8[8*i+a][b]] * code)) << 16) |
                (((u32) (y[P8[8*i+a][b]] * code)) & 0xffff);

  STEP8(state, w[0], 8*i+0, r, s, A, B, C, D, IF);
  STEP8(state, w[1], 8*i+1, s, t, D, A, B, C, IF);
  STEP8(state, w[2], 8*i+2, t, u, C, D, A, B, IF);
  STEP8(state, w[3], 8*i+3, u, r, B, C, D, A, IF);

  STEP8(state, w[4], 8*i+4, r, s, A, B, C, D, MAJ);
  STEP8(state, w[5], 8*i+5, s, t, D, A, B, C, MAJ);
  STEP8(state, w[6], 8*i+6, t, u, C, D, A, B, MAJ);
  STEP8(state, w[7], 8*i+7, u, r, B, C, D, A, MAJ);
}


/********************* Message expansion ************************/

/*
 * Reduce modulo 257; result is in [-127; 383]
 * REDUCE(x) := (x&255) - (x>>8)
 */
#define REDUCE(x) (((x)&255) - ((x)>>8))

/*
 * Reduce from [-127; 383] to [-128; 128]
 * EXTRA_REDUCE_S(x) := x<=128 ? x : x-257
 */
#define EXTRA_REDUCE_S(x)                       \
  ((x)<=128 ? (x) : (x)-257)
 
/*
 * Reduce modulo 257; result is in [-128; 128]
 */
#define REDUCE_FULL_S(x)                        \
  EXTRA_REDUCE_S(REDUCE(x))

void FFT_8(fft_t *y, int stripe) {

  /*
   * FFT_8 using w=4 as 8th root of unity
   *  Unrolled decimation in frequency (DIF) radix-2 NTT.
   *  Output data is in revbin_permuted order.
   */



#define X(i) y[stripe*i]

#define DO_REDUCE(i)                            \
  X(i) = REDUCE(X(i))

#define DO_REDUCE_FULL_S(i)                     \
  do {                                          \
    X(i) = REDUCE(X(i));                        \
    X(i) = EXTRA_REDUCE_S(X(i));                \
  } while(0)

#define BUTTERFLY(i,j,n)                        \
  do {                                          \
    fft_t u= X(i);                              \
    fft_t v= X(j);                              \
    X(i) =  u+v;				\
    X(j) = (u-v) << (2*n);			\
  } while(0)

    BUTTERFLY(0, 4, 0);
    BUTTERFLY(1, 5, 1);
    BUTTERFLY(2, 6, 2);
    BUTTERFLY(3, 7, 3);

    DO_REDUCE(6);
    DO_REDUCE(7);

    BUTTERFLY(0, 2, 0);
    BUTTERFLY(4, 6, 0);
    BUTTERFLY(1, 3, 2);
    BUTTERFLY(5, 7, 2);

    DO_REDUCE(7);
    
    BUTTERFLY(0, 1, 0);
    BUTTERFLY(2, 3, 0);
    BUTTERFLY(4, 5, 0);
    BUTTERFLY(6, 7, 0);

    DO_REDUCE_FULL_S(0);
    DO_REDUCE_FULL_S(1);
    DO_REDUCE_FULL_S(2);
    DO_REDUCE_FULL_S(3);
    DO_REDUCE_FULL_S(4);
    DO_REDUCE_FULL_S(5);
    DO_REDUCE_FULL_S(6);
    DO_REDUCE_FULL_S(7);

#undef X
#undef DO_REDUCE
#undef DO_REDUCE_FULL_S
#undef BUTTERFLY
}

void FFT_16(fft_t *y, int stripe) {

  /*
   * FFT_16 using w=2 as 16th root of unity
   *  Unrolled decimation in frequency (DIF) radix-2 NTT.
   *  Output data is in revbin_permuted order.
   */

#define X(i) y[stripe*i]

#define DO_REDUCE(i)                            \
  X(i) = REDUCE(X(i))

#define DO_REDUCE_FULL_S(i)                     \
  do {                                          \
    X(i) = REDUCE(X(i));                        \
    X(i) = EXTRA_REDUCE_S(X(i));                \
  } while(0)

#define BUTTERFLY(i,j,n)                        \
  do {                                          \
    fft_t u= X(i);                              \
    fft_t v= X(j);                              \
    X(i) =  u+v;				\
    X(j) = (u-v) << n;                          \
  } while(0)

    BUTTERFLY(0,  8, 0);
    BUTTERFLY(1,  9, 1);
    BUTTERFLY(2, 10, 2);
    BUTTERFLY(3, 11, 3);
    BUTTERFLY(4, 12, 4);
    BUTTERFLY(5, 13, 5);
    BUTTERFLY(6, 14, 6);
    BUTTERFLY(7, 15, 7);

    DO_REDUCE(11);
    DO_REDUCE(12);
    DO_REDUCE(13);
    DO_REDUCE(14);
    DO_REDUCE(15);

    BUTTERFLY( 0,  4, 0);
    BUTTERFLY( 8, 12, 0);
    BUTTERFLY( 1,  5, 2);
    BUTTERFLY( 9, 13, 2);
    BUTTERFLY( 2,  6, 4);
    BUTTERFLY(10, 14, 4);
    BUTTERFLY( 3,  7, 6);
    BUTTERFLY(11, 15, 6);

    DO_REDUCE(5);
    DO_REDUCE(7);
    DO_REDUCE(13);
    DO_REDUCE(15);
    
    BUTTERFLY( 0,  2, 0);
    BUTTERFLY( 4,  6, 0);
    BUTTERFLY( 8, 10, 0);
    BUTTERFLY(12, 14, 0);
    BUTTERFLY( 1,  3, 4);
    BUTTERFLY( 5,  7, 4);
    BUTTERFLY( 9, 11, 4);
    BUTTERFLY(13, 15, 4);

    BUTTERFLY( 0,  1, 0);
    BUTTERFLY( 2,  3, 0);
    BUTTERFLY( 4,  5, 0);
    BUTTERFLY( 6,  7, 0);
    BUTTERFLY( 8,  9, 0);
    BUTTERFLY(10, 11, 0);
    BUTTERFLY(12, 13, 0);
    BUTTERFLY(14, 15, 0);

    DO_REDUCE_FULL_S( 0);
    DO_REDUCE_FULL_S( 1);
    DO_REDUCE_FULL_S( 2);
    DO_REDUCE_FULL_S( 3);
    DO_REDUCE_FULL_S( 4);
    DO_REDUCE_FULL_S( 5);
    DO_REDUCE_FULL_S( 6);
    DO_REDUCE_FULL_S( 7);
    DO_REDUCE_FULL_S( 8);
    DO_REDUCE_FULL_S( 9);
    DO_REDUCE_FULL_S(10);
    DO_REDUCE_FULL_S(11);
    DO_REDUCE_FULL_S(12);
    DO_REDUCE_FULL_S(13);
    DO_REDUCE_FULL_S(14);
    DO_REDUCE_FULL_S(15);

#undef X
#undef DO_REDUCE
#undef DO_REDUCE_FULL_S
#undef BUTTERFLY
}

void FFT_64(fft_t *y) {
  
  /*
   * FFT_64 using w=46 as 64th root of unity
   *  decimation in frequency (DIF) radix-8 NTT.
   *  Output data is in revbin_permuted order.
   */

  int i;
 
  /*
   * Begin with 8 parallels DIF FFT_8.
   */
  for (i=0; i<8; i++) {
    FFT_8(y+i,8);
  }

  /*
   * Multiply by twiddle factors
   */
  for (i=8; i<64; i++)
    if (i & 7) y[i] = REDUCE(y[i]*FFT64_8_8_Twiddle[i]);

  /*
   * Finish with 8 paralles DIF FFT_8.
   */
  for (i=0; i<8; i++) {
    FFT_8(y+8*i,1);
  }

}

void FFT_128_halfzero(fft_t *y, int final) {

  /*
   * FFT_128 using w=139 as 128th root of unity.
   *  Decimation in frequency (DIF) NTT.
   *  Output data is in revbin_permuted order.
   *  In place.
   */

  const fft_t tmp = y[63];
  int i;
 
  for (i=0; i<63; i++)
    y[64+i] = REDUCE(y[i] * FFT128_2_64_Twiddle[i]);

  if (final) {
  /*   handle X^125 with an additionnal butterfly */
    const fft_t tmp2 = y[61];
    y[61]  = REDUCE(tmp2 + 1);
    y[125] = REDUCE((tmp2 - 1) * FFT128_2_64_Twiddle[61]);
  }

  /* handle X^127 */
  y[63] = REDUCE(tmp + 1);
  y[127] = REDUCE((tmp - 1) * FFT128_2_64_Twiddle[63]);

  FFT_64(y);
  FFT_64(y+64);
}

void FFT_128_full(fft_t *y) {
  int i;
 
  for (i=0; i<16; i++) {
    FFT_8(y+i,16);
  }

  for (i=0; i<128; i++)
    /*if (i & 7)*/  y[i] = REDUCE(y[i]*FFT128_8_16_Twiddle[i]);

  for (i=0; i<8; i++) {
    FFT_16(y+16*i,1);
  }
}


void FFT_256_halfzero(fft_t *y, int final) {

  /*
   * FFT_256 using w=41 as 256th root of unity.
   *  Decimation in frequency (DIF) NTT.
   *  Output data is in revbin_permuted order.
   *  In place.
   */

  const fft_t tmp = y[127];
  int i;

  for (i=0; i<127; i++)
    y[128+i] = REDUCE(y[i] * FFT256_2_128_Twiddle[i]);

  if (final) {
  /*   handle X^253 with an additionnal butterfly */
    const fft_t tmp2 = y[125];
    y[125] = REDUCE(tmp2 + 1);
    y[253] = REDUCE((tmp2 - 1) * FFT256_2_128_Twiddle[125]);
  }

  /*   handle X^255 with an additionnal butterfly */
  y[127] = REDUCE(tmp + 1);
  y[255] = REDUCE((tmp - 1) * FFT256_2_128_Twiddle[127]);

  FFT_128_full(y);
  FFT_128_full(y+128);
}


void SIMD_Compress(hashState * state, const unsigned char *M, int final) {
  
  fft_t y[256];
  u32 IV[4][8];
  int i;
  int n = state->n_feistels;
 
  if (n == 4) {
    for(i=0; i<64; i++)
      y[i] = M[i];

    FFT_128_halfzero(y, final);
  } else {
    for(i=0; i<128; i++)
      y[i] = M[i];

    FFT_256_halfzero(y, final);
  }  

  /* Save the chaining value for the feed-forward */
  
  for(i=0; i<n; i++) {
    IV[0][i] = state->A[i];
    IV[1][i] = state->B[i];
    IV[2][i] = state->C[i];
    IV[3][i] = state->D[i];
  }

  /* XOR the message to the chaining value */
  /* we can XOR word-by-word */
    u32 * message = (u32 *) M;
    
    for(i=0; i<n; i++) {
        state->A[i] ^= message[i];
        state->B[i] ^= message[n+i];
        state->C[i] ^= message[2*n+i];
        state->D[i] ^= message[3*n+i];
    }

  /* Run the feistel ladders */
  if (n == 4) {
    Round4(state, y, 0, 3,  23, 17, 27);
    Round4(state, y, 1, 28, 19, 22,  7);
    Round4(state, y, 2, 29,  9, 15,  5);
    Round4(state, y, 3,  4, 13, 10, 25);

    STEP4(state, IV[0], 32, 4,  13, A, B, C, D, IF);
    STEP4(state, IV[1], 33, 13, 10, D, A, B, C, IF);
    STEP4(state, IV[2], 34, 10, 25, C, D, A, B, IF);
    STEP4(state, IV[3], 35, 25,  4, B, C, D, A, IF);
  }
  else
 {
    Round8(state, y, 0, 3,  23, 17, 27);
    Round8(state, y, 1, 28, 19, 22,  7);
    Round8(state, y, 2, 29,  9, 15,  5);
    Round8(state, y, 3,  4, 13, 10, 25);

    STEP8(state, IV[0], 32, 4,  13, A, B, C, D, IF);
    STEP8(state, IV[1], 33, 13, 10, D, A, B, C, IF);
    STEP8(state, IV[2], 34, 10, 25, C, D, A, B, IF);
    STEP8(state, IV[3], 35, 25,  4, B, C, D, A, IF);
  }
}


/* 
 * Gives the FFT output in the regular order for consistency checks
 */
int revbin(int k, int i) {
  int acc = 0;
  int j;
  for(j=0; j<k; j++) {
    acc = acc << 1;
    acc ^= (i & 1);
    i = i >> 1;
  }
  return acc;
}


void fft128_natural(fft_t *a, unsigned char *x) {
  fft_t A[128];
  int i;

  for (i=0; i<64; i++)
    A[i] = x[i];

  FFT_128_halfzero(A,0);
  
  for (i=0; i<128; i++)
  a[i] = A[revbin(7,i)];
  
}

void fft256_natural(fft_t *a, unsigned char *x) {
  fft_t A[256];
  int i;

  for (i=0; i<128; i++)
    A[i] = x[i];

  FFT_256_halfzero(A,0);
  
  for (i=0; i<256; i++)
    a[i] = A[revbin(8,i)];
}


/* #define NO_PRECOMPUTED_IV */


/* 
 * Increase the counter.
 */
void IncreaseCounter(hashState *state, DataLength databitlen) {
      state->count += databitlen;
}


/* 
 * Initialize the hashState with a given IV.
 * If the IV is NULL, initialize with zeros.
 */
HashReturn InitIV(hashState *state, int hashbitlen, const u32 *IV) {

  int n;

  if (!SupportedLength(hashbitlen))
    return BAD_HASHBITLEN;

  n = (hashbitlen <= 256) ? 4 : 8;

  state->hashbitlen = hashbitlen;
  state->n_feistels = n;
  state->blocksize = 128*n;
  
  state->count = 0;

  delete state->p1;
  state->buffer = (byte*)malloc(16*n + 16);
  state->p1 = state->buffer;
  /*
   * Align the buffer to a 128 bit boundary.
   */
  state->buffer += ((unsigned char*)NULL - state->buffer)&15;

  delete state->p2;
  state->A = (u32*)malloc((4*n+4)*sizeof(u32));
  state->p2 = state->A;
  /*
   * Align the buffer to a 128 bit boundary.
   */
  state->A += ((u32*)NULL - state->A)&3;

  state->B = state->A+n;
  state->C = state->B+n;
  state->D = state->C+n;

  if (IV)
    memcpy(state->A, IV, 4*n*sizeof(u32));
  else
    memset(state->A, 0, 4*n*sizeof(u32));

  return SUCCESS;
}

/* 
 * Initialize the hashState.
 */
HashReturn Init(hashState *state, int hashbitlen) {
  HashReturn r;
  char *init;

#ifndef NO_PRECOMPUTED_IV
  if (hashbitlen == 224)
    r=InitIV(state, hashbitlen, IV_224);
  else if (hashbitlen == 256)
    r=InitIV(state, hashbitlen, IV_256);
  else if (hashbitlen == 384)
    r=InitIV(state, hashbitlen, IV_384);
  else if (hashbitlen == 512)
    r=InitIV(state, hashbitlen, IV_512);
  else
#endif
    {
      /* 
       * Nonstandart length: IV is not precomputed.
       */
      r=InitIV(state, hashbitlen, NULL);
      if (r != SUCCESS)
        return r;
      
      init = (char*)malloc(state->blocksize);
      memset(init, 0, state->blocksize);
#if defined __STDC__ && __STDC_VERSION__ >= 199901L
      snprintf(init, state->blocksize, "SIMD-%i v1.1", hashbitlen);
#else
      sprintf(init, "SIMD-%i v1.1", hashbitlen);
#endif
      SIMD_Compress(state, (unsigned char*) init, 0);
      free(init);
    }
  return r;
}



HashReturn Update(hashState *state, const BitSequence *data, DataLength databitlen) {
  unsigned current;
  unsigned int bs = state->blocksize;

  current = state->count & (bs - 1);
  
  if (current & 7) {
    /*
     * The number of hashed bits is not a multiple of 8.
     * Very painfull to implement and not required by the NIST API.
     */
    return FAIL;
  }

  while (databitlen > 0) 
  {
    if (current == 0 && databitlen >= bs) 
    {
      /* 
       * We can hash the data directly from the input buffer.
       */
      SIMD_Compress(state, data, 0);
      databitlen -= bs;
      data += bs/8;
      IncreaseCounter(state, bs);
    } 
    else 
    {
      /* 
       * Copy a chunk of data to the buffer
       */
      unsigned int len = bs - current;
      if (databitlen < len) {
        memcpy(state->buffer+current/8, data, (size_t)((databitlen+7)/8));
        IncreaseCounter(state, databitlen);        
        return SUCCESS;
      } else {
        memcpy(state->buffer+current/8, data, len/8);
        IncreaseCounter(state,len);
        databitlen -= len;
        data += len/8;
        current = 0;
        SIMD_Compress(state, state->buffer, 0);
      }
    }
  }
  return SUCCESS;
}

HashReturn Final(hashState *state, BitSequence *hashval) 
{
  u64 l;
  int current = state->count & (state->blocksize - 1);
  unsigned int i;
  BitSequence bs[64];
  int isshort = 1;

  /* 
   * If there is still some data in the buffer, hash it
   */
  if (current) {
    /* 
     * We first need to zero out the end of the buffer.
     */
    if (current & 7) {
      BitSequence mask = 0xff >> (current&7);
      state->buffer[current/8] &= ~mask;
    }
    current = (current+7)/8;
    memset(state->buffer+current, 0, state->blocksize/8 - current);
    SIMD_Compress(state, state->buffer, 0);
  }

  /* 
   * Input the message length as the last block
   */
  memset(state->buffer, 0, state->blocksize/8);

  l = state->count;
  for (i=0; i<8; i++) {
    state->buffer[i] = l & 0xff;
    l >>= 8;
  }
  if (state->count < 16384)
    isshort = 2;

  SIMD_Compress(state, state->buffer, isshort);
    

  /*
   * Decode the 32-bit words into a BitSequence
   */
  for (i=0; i<2*state->n_feistels; i++) {
    u32 x = state->A[i];
    bs[4*i  ] = x&0xff;
    x >>= 8;
    bs[4*i+1] = x&0xff;
    x >>= 8;
    bs[4*i+2] = x&0xff;
    x >>= 8;
    bs[4*i+3] = x&0xff;
  }

  memcpy(hashval, bs, state->hashbitlen/8);
  if (state->hashbitlen%8) {
    BitSequence mask = 0xff << (8 - (state->hashbitlen%8));
    hashval[state->hashbitlen/8 + 1] = bs[state->hashbitlen/8 + 1] & mask;
  }

  return SUCCESS;
}



HashReturn Hash(int hashbitlen, const BitSequence *data, DataLength databitlen,
                BitSequence *hashval) {
  hashState s;
  HashReturn r;
  r = Init(&s, hashbitlen);
  if (r != SUCCESS)
    return r;
  r = Update(&s, data, databitlen);
  if (r != SUCCESS)
    return r;
  r = Final(&s, hashval);
  return r;
}


}