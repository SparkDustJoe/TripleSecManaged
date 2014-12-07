
#include "stdafx.h"
#include "SIMD.h"

#define ROTL32(x, n) ((x << n) | (x >> (32 - n)))

namespace SIMD
{
    uint IV_224[] = 
    {
        0x33586e9f, 0x12fff033, 0xb2d9f64d, 0x6f8fea53,
        0xde943106, 0x2742e439, 0x4fbab5ac, 0x62b9ff96,
        0x22e7b0af, 0xc862b3a8, 0x33e00cdc, 0x236b86a6,
        0xf64ae77c, 0xfa373b76, 0x7dc1ee5b, 0x7fb29ce8
    };

    uint IV_256[] = 
    {
        0x4d567983, 0x07190ba9, 0x8474577b, 0x39d726e9,
        0xaaf3d925, 0x3ee20b03, 0xafd5e751, 0xc96006d3,
        0xc2c2ba14, 0x49b3bcb4, 0xf67caf46, 0x668626c9,
        0xe2eaa8d2, 0x1ff47833, 0xd0c661a5, 0x55693de1
    };

    uint IV_384[] = 
    {
        0x8a36eebc, 0x94a3bd90, 0xd1537b83, 0xb25b070b, 0xf463f1b5, 0xb6f81e20, 0x0055c339, 0xb4d144d1,
        0x7360ca61, 0x18361a03, 0x17dcb4b9, 0x3414c45a, 0xa699a9d2, 0xe39e9664, 0x468bfe77, 0x51d062f8,
        0xb9e3bfe8, 0x63bece2a, 0x8fe506b9, 0xf8cc4ac2, 0x7ae11542, 0xb1aadda1, 0x64b06794, 0x28d2f462,
        0xe64071ec, 0x1deb91a8, 0x8ac8db23, 0x3f782ab5, 0x039b5cb8, 0x71ddd962, 0xfade2cea, 0x1416df71
    };

    uint IV_512[] = 
    {
        0x0ba16b95, 0x72f999ad, 0x9fecc2ae, 0xba3264fc, 0x5e894929, 0x8e9f30e5, 0x2f1daa37, 0xf0f2c558,
        0xac506643, 0xa90635a5, 0xe25b878b, 0xaab7878f, 0x88817f7a, 0x0a02892b, 0x559a7550, 0x598f657e,
        0x7eef60a1, 0x6b70e3e8, 0x9c1714d1, 0xb958e2a8, 0xab02675e, 0xed1c014f, 0xcd8d65bb, 0xfdb7a257,
        0x09254899, 0xd699c7bc, 0x9019b6dc, 0x2b9022e4, 0x8fa14956, 0x21bf9bd3, 0xb94d0943, 0x6ffddc22
    };

    static const int P4[32][4] = 
    {
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

    static const int Q4[32][4] = 
    {
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

    static const int P8[32][8] = 
    {
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

    static const int Q8[32][8] = 
    {
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

    static const int FFT64_8_8_Twiddle[] = 
    {
        1,    1,    1,    1,    1,    1,    1,    1,
        1,    2,    4,    8,   16,   32,   64,  128,
        1,   60,    2,  120,    4,  -17,    8,  -34,
        1,  120,    8,  -68,   64,  -30,   -2,   17,
        1,   46,   60,  -67,    2,   92,  120,  123,
        1,   92,  -17,  -22,   32,  117,  -30,   67,
        1,  -67,  120,  -73,    8,  -22,  -68,  -70,
        1,  123,  -34,  -70,  128,   67,   17,   35,
    };

    static const int FFT128_2_64_Twiddle[] =  
    {
        1, -118,   46,  -31,   60,  116,  -67,  -61,
        2,   21,   92,  -62,  120,  -25,  123, -122,
        4,   42,  -73, -124,  -17,  -50,  -11,   13,
        8,   84,  111,    9,  -34, -100,  -22,   26,
        16,  -89,  -35,   18,  -68,   57,  -44,   52,
        32,   79,  -70,   36,  121,  114,  -88,  104,
        64,  -99,  117,   72,  -15,  -29,   81,  -49,
        128,   59,  -23, -113,  -30,  -58,  -95,  -98
    };

    static const int FFT128_16_8_Twiddle[] =  
    {
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
        1, -122, -22, 114, -30, 62, -111, -79 
    };

    static const int FFT128_8_16_Twiddle[] =  
    {
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
        1, 60, 2, 120, 4, -17, 8, -34, 16, -68, 32, 121, 64, -15, 128, -30, 
        1, 46, 60, -67, 2, 92, 120, 123, 4, -73, -17, -11, 8, 111, -34, -22, 
        1, -67, 120, -73, 8, -22, -68, -70, 64, 81, -30, -46, -2, -123, 17, -111, 
        1, -118, 46, -31, 60, 116, -67, -61, 2, 21, 92, -62, 120, -25, 123, -122, 
        1, 116, 92, -122, -17, 84, -22, 18, 32, 114, 117, -49, -30, 118, 67, 62, 
        1, -31, -67, 21, 120, -122, -73, -50, 8, 9, -22, -89, -68, 52, -70, 114, 
        1, -61, 123, -50, -34, 18, -70, -99, 128, -98, 67, 25, 17, -9, 35, -79
    };

    static const int FFT256_2_128_Twiddle[] =  
    {
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
        -30,   55,  -58,  -65,  -95,  -40,  -98,   94
    };

    const int p8_xor [] = {1, 6, 2, 3, 5, 7, 4};

    void SIMD256Base::Round4(int* y, int i, int r, int s, int t, int u) 
    {
        int code = i<2 ? 185 : 233;
        uint* w = new uint[32];

        for(int a=0; a<8; a++)
        {
            for(int b=0; b<4; b++)
            {
                w[a*4+b] = (((uint) (y[Q4[8*i+a][b]] * code)) << 16) |
                    (((uint) (y[P4[8*i+a][b]] * code)) & 0xffff);
            }
        }

        uint* R = new uint[4];

        for(int j=0; j<4; j++) 
            R[j] = ((m_state[0+j] << r) | (m_state[0+j] >> (32 - r))); 
        for(int j=0; j<4; j++) 
        { 
            m_state[12+j] = m_state[12+j] + w[0+j] + ((((m_state[4+j]) ^ (m_state[8+j])) & (m_state[0+j])) ^ (m_state[8+j])); 
            m_state[12+j] = ((m_state[12+j] << s) | (m_state[12+j] >> (32 - s))) + R[(j^(((8*i+0)%3)+1))]; 
            m_state[0+j] = R[j]; 
        }

        for(int j=0; j<4; j++) R[j] = ((m_state[12+j] << s) | (m_state[12+j] >> (32 - s))); for(int j=0; j<4; j++) { m_state[8+j] = m_state[8+j] + w[4+j] + ((((m_state[0+j]) ^ (m_state[4+j])) & (m_state[12+j])) ^ (m_state[4+j])); m_state[8+j] = ((m_state[8+j] << t) | (m_state[8+j] >> (32 - t))) + R[(j^(((8*i+1)%3)+1))]; m_state[12+j] = R[j]; };

        for(int j=0; j<4; j++) R[j] = ((m_state[8+j] << t) | (m_state[8+j] >> (32 - t))); for(int j=0; j<4; j++) { m_state[4+j] = m_state[4+j] + w[8+j] + ((((m_state[12+j]) ^ (m_state[0+j])) & (m_state[8+j])) ^ (m_state[0+j])); m_state[4+j] = ((m_state[4+j] << u) | (m_state[4+j] >> (32 - u))) + R[(j^(((8*i+2)%3)+1))]; m_state[8+j] = R[j]; };

        for(int j=0; j<4; j++) R[j] = ((m_state[4+j] << u) | (m_state[4+j] >> (32 - u))); for(int j=0; j<4; j++) { m_state[0+j] = m_state[0+j] + w[12+j] + ((((m_state[8+j]) ^ (m_state[12+j])) & (m_state[4+j])) ^ (m_state[12+j])); m_state[0+j] = ((m_state[0+j] << r) | (m_state[0+j] >> (32 - r))) + R[(j^(((8*i+3)%3)+1))]; m_state[4+j] = R[j]; };


        for(int j=0; j<4; j++) R[j] = ((m_state[0+j] << r) | (m_state[0+j] >> (32 - r))); for(int j=0; j<4; j++) { m_state[12+j] = m_state[12+j] + w[16+j] + (((m_state[8+j]) & (m_state[4+j])) | (((m_state[8+j]) | (m_state[4+j])) & (m_state[0+j]))); m_state[12+j] = ((m_state[12+j] << s) | (m_state[12+j] >> (32 - s))) + R[(j^(((8*i+4)%3)+1))]; m_state[0+j] = R[j]; };

        for(int j=0; j<4; j++) R[j] = ((m_state[12+j] << s) | (m_state[12+j] >> (32 - s))); for(int j=0; j<4; j++) { m_state[8+j] = m_state[8+j] + w[20+j] + (((m_state[4+j]) & (m_state[0+j])) | (((m_state[4+j]) | (m_state[0+j])) & (m_state[12+j]))); m_state[8+j] = ((m_state[8+j] << t) | (m_state[8+j] >> (32 - t))) + R[(j^(((8*i+5)%3)+1))]; m_state[12+j] = R[j]; };

        for(int j=0; j<4; j++) R[j] = ((m_state[8+j] << t) | (m_state[8+j] >> (32 - t))); for(int j=0; j<4; j++) { m_state[4+j] = m_state[4+j] + w[24+j] + (((m_state[0+j]) & (m_state[12+j])) | (((m_state[0+j]) | (m_state[12+j])) & (m_state[8+j]))); m_state[4+j] = ((m_state[4+j] << u) | (m_state[4+j] >> (32 - u))) + R[(j^(((8*i+6)%3)+1))]; m_state[8+j] = R[j]; };

        for(int j=0; j<4; j++) R[j] = ((m_state[4+j] << u) | (m_state[4+j] >> (32 - u))); for(int j=0; j<4; j++) { m_state[0+j] = m_state[0+j] + w[28+j] + (((m_state[12+j]) & (m_state[8+j])) | (((m_state[12+j]) | (m_state[8+j])) & (m_state[4+j]))); m_state[0+j] = ((m_state[0+j] << r) | (m_state[0+j] >> (32 - r))) + R[(j^(((8*i+7)%3)+1))]; m_state[4+j] = R[j]; };

        delete R;
        delete w;
    }

    void SIMD512Base::Round8(int* y, int i, int r, int s, int t, int u) 
    {
        int code = i<2 ? 185 : 233;
        uint* w = new uint[64];

        for(int a=0; a<8; a++)
        {
            for(int b=0; b<8; b++)
            {
                w[a*8+b] = (((uint) (y[Q8[8*i+a][b]] * code)) << 16) |
                    (((uint) (y[P8[8*i+a][b]] * code)) & 0xffff);
            }
        }

        uint* R = new uint[8];

        for(int j=0; j<8; j++) R[j] = ((m_state[0+j] << r) | (m_state[0+j] >> (32 - r))); for(int j=0; j<8; j++) { m_state[24+j] = m_state[24+j] + w[0+j] + ((((m_state[8+j]) ^ (m_state[16+j])) & (m_state[0+j])) ^ (m_state[16+j])); m_state[24+j] = ((m_state[24+j] << s) | (m_state[24+j] >> (32 - s))) + R[j^p8_xor[(8*i+0) % 7]]; m_state[0+j] = R[j]; };

        for(int j=0; j<8; j++) R[j] = ((m_state[24+j] << s) | (m_state[24+j] >> (32 - s))); for(int j=0; j<8; j++) { m_state[16+j] = m_state[16+j] + w[8+j] + ((((m_state[0+j]) ^ (m_state[8+j])) & (m_state[24+j])) ^ (m_state[8+j])); m_state[16+j] = ((m_state[16+j] << t) | (m_state[16+j] >> (32 - t))) + R[j^p8_xor[(8*i+1) % 7]]; m_state[24+j] = R[j]; };

        for(int j=0; j<8; j++) R[j] = ((m_state[16+j] << t) | (m_state[16+j] >> (32 - t))); for(int j=0; j<8; j++) { m_state[8+j] = m_state[8+j] + w[16+j] + ((((m_state[24+j]) ^ (m_state[0+j])) & (m_state[16+j])) ^ (m_state[0+j])); m_state[8+j] = ((m_state[8+j] << u) | (m_state[8+j] >> (32 - u))) + R[j^p8_xor[(8*i+2) % 7]]; m_state[16+j] = R[j]; };

        for(int j=0; j<8; j++) R[j] = ((m_state[8+j] << u) | (m_state[8+j] >> (32 - u))); for(int j=0; j<8; j++) { m_state[0+j] = m_state[0+j] + w[24+j] + ((((m_state[16+j]) ^ (m_state[24+j])) & (m_state[8+j])) ^ (m_state[24+j])); m_state[0+j] = ((m_state[0+j] << r) | (m_state[0+j] >> (32 - r))) + R[j^p8_xor[(8*i+3) % 7]]; m_state[8+j] = R[j]; };

        for(int j=0; j<8; j++) R[j] = ((m_state[0+j] << r) | (m_state[0+j] >> (32 - r))); for(int j=0; j<8; j++) { m_state[24+j] = m_state[24+j] + w[32+j] + (((m_state[16+j]) & (m_state[8+j])) | (((m_state[16+j]) | (m_state[8+j])) & (m_state[0+j]))); m_state[24+j] = ((m_state[24+j] << s) | (m_state[24+j] >> (32 - s))) + R[j^p8_xor[(8*i+4) % 7]]; m_state[0+j] = R[j]; };

        for(int j=0; j<8; j++) R[j] = ((m_state[24+j] << s) | (m_state[24+j] >> (32 - s))); for(int j=0; j<8; j++) { m_state[16+j] = m_state[16+j] + w[40+j] + (((m_state[8+j]) & (m_state[0+j])) | (((m_state[8+j]) | (m_state[0+j])) & (m_state[24+j]))); m_state[16+j] = ((m_state[16+j] << t) | (m_state[16+j] >> (32 - t))) + R[j^p8_xor[(8*i+5) % 7]]; m_state[24+j] = R[j]; };

        for(int j=0; j<8; j++) R[j] = ((m_state[16+j] << t) | (m_state[16+j] >> (32 - t))); for(int j=0; j<8; j++) { m_state[8+j] = m_state[8+j] + w[48+j] + (((m_state[0+j]) & (m_state[24+j])) | (((m_state[0+j]) | (m_state[24+j])) & (m_state[16+j]))); m_state[8+j] = ((m_state[8+j] << u) | (m_state[8+j] >> (32 - u))) + R[j^p8_xor[(8*i+6) % 7]]; m_state[16+j] = R[j]; };

        for(int j=0; j<8; j++) R[j] = ((m_state[8+j] << u) | (m_state[8+j] >> (32 - u))); for(int j=0; j<8; j++) { m_state[0+j] = m_state[0+j] + w[56+j] + (((m_state[24+j]) & (m_state[16+j])) | (((m_state[24+j]) | (m_state[16+j])) & (m_state[8+j]))); m_state[0+j] = ((m_state[0+j] << r) | (m_state[0+j] >> (32 - r))) + R[j^p8_xor[(8*i+7) % 7]]; m_state[8+j] = R[j]; };

        delete R;
        delete w;
    }

    void SIMDBase::FFT_8(int *y, int stripe) 
    {
        int u;
        int v;

        u= y[stripe*0]; v= y[stripe*4]; y[stripe*0] = u+v; y[stripe*4] = (u-v) << (2*0);;
        u= y[stripe*1]; v= y[stripe*5]; y[stripe*1] = u+v; y[stripe*5] = (u-v) << (2*1);;
        u= y[stripe*2]; v= y[stripe*6]; y[stripe*2] = u+v; y[stripe*6] = (u-v) << (2*2);;
        u= y[stripe*3]; v= y[stripe*7]; y[stripe*3] = u+v; y[stripe*7] = (u-v) << (2*3);;

        y[stripe*6] = ((y[stripe*6] & 255) - (y[stripe*6] >> 8));
        y[stripe*7] = ((y[stripe*7] & 255) - (y[stripe*7] >> 8));

        u= y[stripe*0]; v= y[stripe*2]; y[stripe*0] = u+v; y[stripe*2] = (u-v) << (2*0);;
        u= y[stripe*4]; v= y[stripe*6]; y[stripe*4] = u+v; y[stripe*6] = (u-v) << (2*0);;
        u= y[stripe*1]; v= y[stripe*3]; y[stripe*1] = u+v; y[stripe*3] = (u-v) << (2*2);;
        u= y[stripe*5]; v= y[stripe*7]; y[stripe*5] = u+v; y[stripe*7] = (u-v) << (2*2);;

        y[stripe*7] = ((y[stripe*7] & 255) - (y[stripe*7] >> 8));

        u= y[stripe*0]; v= y[stripe*1]; y[stripe*0] = u+v; y[stripe*1] = (u-v) << (2*0);;
        u= y[stripe*2]; v= y[stripe*3]; y[stripe*2] = u+v; y[stripe*3] = (u-v) << (2*0);;
        u= y[stripe*4]; v= y[stripe*5]; y[stripe*4] = u+v; y[stripe*5] = (u-v) << (2*0);;
        u= y[stripe*6]; v= y[stripe*7]; y[stripe*6] = u+v; y[stripe*7] = (u-v) << (2*0);;

        y[stripe*0] = ((y[stripe*0] & 255) - (y[stripe*0] >> 8)); y[stripe*0] = (y[stripe*0] <= 128 ? y[stripe*0] : y[stripe*0]-257);;
        y[stripe*1] = ((y[stripe*1] & 255) - (y[stripe*1] >> 8)); y[stripe*1] = (y[stripe*1] <= 128 ? y[stripe*1] : y[stripe*1]-257);;
        y[stripe*2] = ((y[stripe*2] & 255) - (y[stripe*2] >> 8)); y[stripe*2] = (y[stripe*2] <= 128 ? y[stripe*2] : y[stripe*2]-257);;
        y[stripe*3] = ((y[stripe*3] & 255) - (y[stripe*3] >> 8)); y[stripe*3] = (y[stripe*3] <= 128 ? y[stripe*3] : y[stripe*3]-257);;
        y[stripe*4] = ((y[stripe*4] & 255) - (y[stripe*4] >> 8)); y[stripe*4] = (y[stripe*4] <= 128 ? y[stripe*4] : y[stripe*4]-257);;
        y[stripe*5] = ((y[stripe*5] & 255) - (y[stripe*5] >> 8)); y[stripe*5] = (y[stripe*5] <= 128 ? y[stripe*5] : y[stripe*5]-257);;
        y[stripe*6] = ((y[stripe*6] & 255) - (y[stripe*6] >> 8)); y[stripe*6] = (y[stripe*6] <= 128 ? y[stripe*6] : y[stripe*6]-257);;
        y[stripe*7] = ((y[stripe*7] & 255) - (y[stripe*7] >> 8)); y[stripe*7] = (y[stripe*7] <= 128 ? y[stripe*7] : y[stripe*7]-257);;
    }

    void SIMD512Base::FFT_16(int *y, int stripe) 
    {
        int u;
        int v;

        u= y[stripe*0]; v= y[stripe*8]; y[stripe*0] = u+v; y[stripe*8] = (u-v) << 0;;
        u= y[stripe*1]; v= y[stripe*9]; y[stripe*1] = u+v; y[stripe*9] = (u-v) << 1;;
        u= y[stripe*2]; v= y[stripe*10]; y[stripe*2] = u+v; y[stripe*10] = (u-v) << 2;;
        u= y[stripe*3]; v= y[stripe*11]; y[stripe*3] = u+v; y[stripe*11] = (u-v) << 3;;
        u= y[stripe*4]; v= y[stripe*12]; y[stripe*4] = u+v; y[stripe*12] = (u-v) << 4;;
        u= y[stripe*5]; v= y[stripe*13]; y[stripe*5] = u+v; y[stripe*13] = (u-v) << 5;;
        u= y[stripe*6]; v= y[stripe*14]; y[stripe*6] = u+v; y[stripe*14] = (u-v) << 6;;
        u= y[stripe*7]; v= y[stripe*15]; y[stripe*7] = u+v; y[stripe*15] = (u-v) << 7;;

        y[stripe*11] = ((y[stripe*11] & 255) - (y[stripe*11] >> 8));
        y[stripe*12] = ((y[stripe*12] & 255) - (y[stripe*12] >> 8));
        y[stripe*13] = ((y[stripe*13] & 255) - (y[stripe*13] >> 8));
        y[stripe*14] = ((y[stripe*14] & 255) - (y[stripe*14] >> 8));
        y[stripe*15] = ((y[stripe*15] & 255) - (y[stripe*15] >> 8));

        u= y[stripe*0]; v= y[stripe*4]; y[stripe*0] = u+v; y[stripe*4] = (u-v) << 0;;
        u= y[stripe*8]; v= y[stripe*12]; y[stripe*8] = u+v; y[stripe*12] = (u-v) << 0;;
        u= y[stripe*1]; v= y[stripe*5]; y[stripe*1] = u+v; y[stripe*5] = (u-v) << 2;;
        u= y[stripe*9]; v= y[stripe*13]; y[stripe*9] = u+v; y[stripe*13] = (u-v) << 2;;
        u= y[stripe*2]; v= y[stripe*6]; y[stripe*2] = u+v; y[stripe*6] = (u-v) << 4;;
        u= y[stripe*10]; v= y[stripe*14]; y[stripe*10] = u+v; y[stripe*14] = (u-v) << 4;;
        u= y[stripe*3]; v= y[stripe*7]; y[stripe*3] = u+v; y[stripe*7] = (u-v) << 6;;
        u= y[stripe*11]; v= y[stripe*15]; y[stripe*11] = u+v; y[stripe*15] = (u-v) << 6;;

        y[stripe*5] = ((y[stripe*5] & 255) - (y[stripe*5] >> 8));
        y[stripe*7] = ((y[stripe*7] & 255) - (y[stripe*7] >> 8));
        y[stripe*13] = ((y[stripe*13] & 255) - (y[stripe*13] >> 8));
        y[stripe*15] = ((y[stripe*15] & 255) - (y[stripe*15] >> 8));

        u= y[stripe*0]; v= y[stripe*2]; y[stripe*0] = u+v; y[stripe*2] = (u-v) << 0;;
        u= y[stripe*4]; v= y[stripe*6]; y[stripe*4] = u+v; y[stripe*6] = (u-v) << 0;;
        u= y[stripe*8]; v= y[stripe*10]; y[stripe*8] = u+v; y[stripe*10] = (u-v) << 0;;
        u= y[stripe*12]; v= y[stripe*14]; y[stripe*12] = u+v; y[stripe*14] = (u-v) << 0;;
        u= y[stripe*1]; v= y[stripe*3]; y[stripe*1] = u+v; y[stripe*3] = (u-v) << 4;;
        u= y[stripe*5]; v= y[stripe*7]; y[stripe*5] = u+v; y[stripe*7] = (u-v) << 4;;
        u= y[stripe*9]; v= y[stripe*11]; y[stripe*9] = u+v; y[stripe*11] = (u-v) << 4;;
        u= y[stripe*13]; v= y[stripe*15]; y[stripe*13] = u+v; y[stripe*15] = (u-v) << 4;;

        u= y[stripe*0]; v= y[stripe*1]; y[stripe*0] = u+v; y[stripe*1] = (u-v) << 0;;
        u= y[stripe*2]; v= y[stripe*3]; y[stripe*2] = u+v; y[stripe*3] = (u-v) << 0;;
        u= y[stripe*4]; v= y[stripe*5]; y[stripe*4] = u+v; y[stripe*5] = (u-v) << 0;;
        u= y[stripe*6]; v= y[stripe*7]; y[stripe*6] = u+v; y[stripe*7] = (u-v) << 0;;
        u= y[stripe*8]; v= y[stripe*9]; y[stripe*8] = u+v; y[stripe*9] = (u-v) << 0;;
        u= y[stripe*10]; v= y[stripe*11]; y[stripe*10] = u+v; y[stripe*11] = (u-v) << 0;;
        u= y[stripe*12]; v= y[stripe*13]; y[stripe*12] = u+v; y[stripe*13] = (u-v) << 0;;
        u= y[stripe*14]; v= y[stripe*15]; y[stripe*14] = u+v; y[stripe*15] = (u-v) << 0;;

        y[stripe*0] = ((y[stripe*0] & 255) - (y[stripe*0] >> 8)); y[stripe*0] = (y[stripe*0] <= 128 ? y[stripe*0] : y[stripe*0]-257);;
        y[stripe*1] = ((y[stripe*1] & 255) - (y[stripe*1] >> 8)); y[stripe*1] = (y[stripe*1] <= 128 ? y[stripe*1] : y[stripe*1]-257);;
        y[stripe*2] = ((y[stripe*2] & 255) - (y[stripe*2] >> 8)); y[stripe*2] = (y[stripe*2] <= 128 ? y[stripe*2] : y[stripe*2]-257);;
        y[stripe*3] = ((y[stripe*3] & 255) - (y[stripe*3] >> 8)); y[stripe*3] = (y[stripe*3] <= 128 ? y[stripe*3] : y[stripe*3]-257);;
        y[stripe*4] = ((y[stripe*4] & 255) - (y[stripe*4] >> 8)); y[stripe*4] = (y[stripe*4] <= 128 ? y[stripe*4] : y[stripe*4]-257);;
        y[stripe*5] = ((y[stripe*5] & 255) - (y[stripe*5] >> 8)); y[stripe*5] = (y[stripe*5] <= 128 ? y[stripe*5] : y[stripe*5]-257);;
        y[stripe*6] = ((y[stripe*6] & 255) - (y[stripe*6] >> 8)); y[stripe*6] = (y[stripe*6] <= 128 ? y[stripe*6] : y[stripe*6]-257);;
        y[stripe*7] = ((y[stripe*7] & 255) - (y[stripe*7] >> 8)); y[stripe*7] = (y[stripe*7] <= 128 ? y[stripe*7] : y[stripe*7]-257);;
        y[stripe*8] = ((y[stripe*8] & 255) - (y[stripe*8] >> 8)); y[stripe*8] = (y[stripe*8] <= 128 ? y[stripe*8] : y[stripe*8]-257);;
        y[stripe*9] = ((y[stripe*9] & 255) - (y[stripe*9] >> 8)); y[stripe*9] = (y[stripe*9] <= 128 ? y[stripe*9] : y[stripe*9]-257);;
        y[stripe*10] = ((y[stripe*10] & 255) - (y[stripe*10] >> 8)); y[stripe*10] = (y[stripe*10] <= 128 ? y[stripe*10] : y[stripe*10]-257);;
        y[stripe*11] = ((y[stripe*11] & 255) - (y[stripe*11] >> 8)); y[stripe*11] = (y[stripe*11] <= 128 ? y[stripe*11] : y[stripe*11]-257);;
        y[stripe*12] = ((y[stripe*12] & 255) - (y[stripe*12] >> 8)); y[stripe*12] = (y[stripe*12] <= 128 ? y[stripe*12] : y[stripe*12]-257);;
        y[stripe*13] = ((y[stripe*13] & 255) - (y[stripe*13] >> 8)); y[stripe*13] = (y[stripe*13] <= 128 ? y[stripe*13] : y[stripe*13]-257);;
        y[stripe*14] = ((y[stripe*14] & 255) - (y[stripe*14] >> 8)); y[stripe*14] = (y[stripe*14] <= 128 ? y[stripe*14] : y[stripe*14]-257);;
        y[stripe*15] = ((y[stripe*15] & 255) - (y[stripe*15] >> 8)); y[stripe*15] = (y[stripe*15] <= 128 ? y[stripe*15] : y[stripe*15]-257);;
    }

    void SIMD256Base::FFT_64(int* y) 
    {
        for (int i=0; i<8; i++) 
            FFT_8(y+i,8);

        for (int i=8; i<64; i++)
        {
            if (i & 7) 
                y[i] = ((y[i]*FFT64_8_8_Twiddle[i] & 255) - (y[i]*FFT64_8_8_Twiddle[i] >> 8));
        }

        for (int i=0; i<8; i++) 
            FFT_8(y+8*i,1);
    }

    void SIMD512Base::FFT_128_full(int* y) 
    {
        for (int i=0; i<16; i++) 
            FFT_8(y+i,16);

        for (int i=0; i<128; i++)
            y[i] = ((y[i]*FFT128_8_16_Twiddle[i] & 255) - (y[i]*FFT128_8_16_Twiddle[i] >> 8));

        for (int i=0; i<8; i++) 
            FFT_16(y+16*i,1);
    }
    
    void SIMD256Base::TransformBlock(byte* a_data, int a_index)
    {
        TransformBlock(a_data, a_index, 0);
    }

    void SIMD256Base::TransformBlock(byte* a_data, int a_index, int a_final) 
    {
        a_data += a_index;
        int* y = new int[128];

        for(int i=0; i<64; i++)
            y[i] = a_data[i];

        int tmp = y[63];

        for (int i=0; i<63; i++)
            y[64+i] = ((y[i] * FFT128_2_64_Twiddle[i] & 255) - (y[i] * FFT128_2_64_Twiddle[i] >> 8));

        if (a_final) 
        {
            int tmp2 = y[61];
            y[61]  = (((tmp2 + 1) & 255) - ((tmp2 + 1) >> 8));
            y[125] = (((tmp2 - 1) * FFT128_2_64_Twiddle[61] & 255) - ((tmp2 - 1) * FFT128_2_64_Twiddle[61] >> 8));
        }

        y[63] = (((tmp + 1) & 255) - ((tmp + 1) >> 8));
        y[127] = (((tmp - 1) * FFT128_2_64_Twiddle[63] & 255) - ((tmp - 1) * FFT128_2_64_Twiddle[63] >> 8));

        FFT_64(y);
        FFT_64(y+64);

        uint* state = new uint[16];
        memcpy(state, m_state, 64);

        uint* message = (uint *)a_data;

        for(int i=0; i<4; i++) 
        {
            m_state[i] ^= message[i];
            m_state[4+i] ^= message[4+i];
            m_state[8+i] ^= message[8+i];
            m_state[12+i] ^= message[12+i];
        }

        Round4(y, 0, 3,  23, 17, 27);
        Round4(y, 1, 28, 19, 22,  7);
        Round4(y, 2, 29,  9, 15,  5);
        Round4(y, 3,  4, 13, 10, 25);

        uint* R = new uint[4];

        for(int j=0; j<4; j++) R[j] = ((m_state[0+j] << 4) | (m_state[0+j] >> (32 - 4))); for(int j=0; j<4; j++) { m_state[12+j] = m_state[12+j] + state[0+j] + ((((m_state[4+j]) ^ (m_state[8+j])) & (m_state[0+j])) ^ (m_state[8+j])); m_state[12+j] = ((m_state[12+j] << 13) | (m_state[12+j] >> (32 - 13))) + R[(j^(((32)%3)+1))]; m_state[0+j] = R[j]; };

        for(int j=0; j<4; j++) R[j] = ((m_state[12+j] << 13) | (m_state[12+j] >> (32 - 13))); for(int j=0; j<4; j++) { m_state[8+j] = m_state[8+j] + state[4+j] + ((((m_state[0+j]) ^ (m_state[4+j])) & (m_state[12+j])) ^ (m_state[4+j])); m_state[8+j] = ((m_state[8+j] << 10) | (m_state[8+j] >> (32 - 10))) + R[(j^(((33)%3)+1))]; m_state[12+j] = R[j]; };

        for(int j=0; j<4; j++) R[j] = ((m_state[8+j] << 10) | (m_state[8+j] >> (32 - 10))); for(int j=0; j<4; j++) { m_state[4+j] = m_state[4+j] + state[8+j] + ((((m_state[12+j]) ^ (m_state[0+j])) & (m_state[8+j])) ^ (m_state[0+j])); m_state[4+j] = ((m_state[4+j] << 25) | (m_state[4+j] >> (32 - 25))) + R[(j^(((34)%3)+1))]; m_state[8+j] = R[j]; };

        for(int j=0; j<4; j++) R[j] = ((m_state[4+j] << 25) | (m_state[4+j] >> (32 - 25))); for(int j=0; j<4; j++) { m_state[0+j] = m_state[0+j] + state[12+j] + ((((m_state[8+j]) ^ (m_state[12+j])) & (m_state[4+j])) ^ (m_state[12+j])); m_state[0+j] = ((m_state[0+j] << 4) | (m_state[0+j] >> (32 - 4))) + R[(j^(((35)%3)+1))]; m_state[4+j] = R[j]; };

        delete R;
        delete state;
        delete y;
    }

    void SIMD512Base::TransformBlock(byte* a_data, int a_index)
    {
        TransformBlock(a_data, a_index, 0);
    }

    void SIMD512Base::TransformBlock(byte* a_data, int a_index, int a_final) 
    {
        a_data += a_index;

        int* y = new int[256];

        for(int i=0; i<128; i++)
            y[i] = a_data[i];

        int tmp = y[127];

        for (int i=0; i<127; i++)
            y[128+i] = ((y[i] * FFT256_2_128_Twiddle[i] & 255) - (y[i] * FFT256_2_128_Twiddle[i] >> 8));

        if (a_final) 
        {
            int tmp2 = y[125];
            y[125] = (((tmp2 + 1) & 255) - ((tmp2 + 1) >> 8));
            y[253] = (((tmp2 - 1) * FFT256_2_128_Twiddle[125] & 255) - ((tmp2 - 1) * FFT256_2_128_Twiddle[125] >> 8));
        }

        y[127] = (((tmp + 1) & 255) - ((tmp + 1) >> 8));
        y[255] = (((tmp - 1) * FFT256_2_128_Twiddle[127] & 255) - ((tmp - 1) * FFT256_2_128_Twiddle[127] >> 8));

        FFT_128_full(y);
        FFT_128_full(y+128);

        uint* state = new uint[32];
        memcpy(state, m_state, 128);

        uint* message = (uint *)a_data;

        for(int i=0; i<8; i++) 
        {
            m_state[i] ^= message[i];
            m_state[8+i] ^= message[8+i];
            m_state[16+i] ^= message[16+i];
            m_state[24+i] ^= message[24+i];
        }

        Round8(y, 0, 3,  23, 17, 27);
        Round8(y, 1, 28, 19, 22,  7);
        Round8(y, 2, 29,  9, 15,  5);
        Round8(y, 3,  4, 13, 10, 25);

        uint* R = new uint[8];

        for(int j=0; j<8; j++) R[j] = ((m_state[0+j] << 4) | (m_state[0+j] >> (32 - 4))); for(int j=0; j<8; j++) { m_state[24+j] = m_state[24+j] + state[0+j] + ((((m_state[8+j]) ^ (m_state[16+j])) & (m_state[0+j])) ^ (m_state[16+j])); m_state[24+j] = ((m_state[24+j] << 13) | (m_state[24+j] >> (32 - 13))) + R[j^p8_xor[(32) % 7]]; m_state[0+j] = R[j]; };

        for(int j=0; j<8; j++) R[j] = ((m_state[24+j] << 13) | (m_state[24+j] >> (32 - 13))); for(int j=0; j<8; j++) { m_state[16+j] = m_state[16+j] + state[8+j] + ((((m_state[0+j]) ^ (m_state[8+j])) & (m_state[24+j])) ^ (m_state[8+j])); m_state[16+j] = ((m_state[16+j] << 10) | (m_state[16+j] >> (32 - 10))) + R[j^p8_xor[(33) % 7]]; m_state[24+j] = R[j]; };

        for(int j=0; j<8; j++) R[j] = ((m_state[16+j] << 10) | (m_state[16+j] >> (32 - 10))); for(int j=0; j<8; j++) { m_state[8+j] = m_state[8+j] + state[16+j] + ((((m_state[24+j]) ^ (m_state[0+j])) & (m_state[16+j])) ^ (m_state[0+j])); m_state[8+j] = ((m_state[8+j] << 25) | (m_state[8+j] >> (32 - 25))) + R[j^p8_xor[(34) % 7]]; m_state[16+j] = R[j]; };

        for(int j=0; j<8; j++) R[j] = ((m_state[8+j] << 25) | (m_state[8+j] >> (32 - 25))); for(int j=0; j<8; j++) { m_state[0+j] = m_state[0+j] + state[24+j] + ((((m_state[16+j]) ^ (m_state[24+j])) & (m_state[8+j])) ^ (m_state[24+j])); m_state[0+j] = ((m_state[0+j] << 4) | (m_state[0+j] >> (32 - 4))) + R[j^p8_xor[(35) % 7]]; m_state[8+j] = R[j]; };

        delete R;
        delete state;
        delete y;
    }

    SIMDBase::SIMDBase(int a_hashSize, int a_blockSize)
    {
        HashSize = a_hashSize;
        BlockSize = a_blockSize;

        m_buffer = new byte[BlockSize];
        m_state = new uint[BlockSize / 4];
    }

    void SIMDBase::Initialize() 
    {
        m_processedBytes = 0;
        m_buffer_pos = 0;
    }

    void SIMD256Base::Initialize() 
    {
        if (HashSize == 28)
            memcpy(m_state, IV_224, BlockSize);
        else 
            memcpy(m_state, IV_256, BlockSize);

        SIMDBase::Initialize();
    }

    void SIMD512Base::Initialize() 
    {
        if (HashSize == 48)
            memcpy(m_state, IV_384, BlockSize);
        else 
            memcpy(m_state, IV_512, BlockSize);

        SIMDBase::Initialize();
    }

    void SIMDBase::TransformBytes(byte* a_data, int a_index, int a_length) 
    {
        if (m_buffer_pos > 0) 
        {
            int left = BlockSize - m_buffer_pos;
            if (left > a_length)
                left = a_length;

            memcpy(&m_buffer[m_buffer_pos], &a_data[a_index], left);
            m_buffer_pos += left;
            a_index += left;
            a_length -= left;
            m_processedBytes += left;

            if (m_buffer_pos == BlockSize)
            {
                TransformBlock(m_buffer, 0);
                m_buffer_pos = 0;
            } 
        }

        while (a_length >= BlockSize)
        {
            m_processedBytes += BlockSize;
            TransformBlock(a_data, a_index);
            a_index += BlockSize;
            a_length -= BlockSize;
        }

        if (a_length > 0)
        {
            memcpy(&m_buffer[m_buffer_pos], &a_data[a_index], a_length);
            m_processedBytes += a_length;
            m_buffer_pos += a_length;
        }
    }

    void SIMDBase::Finish() 
    {
        if (m_buffer_pos > 0) 
        {
            memset(&m_buffer[m_buffer_pos], 0, BlockSize - m_buffer_pos);
            TransformBlock(m_buffer, 0);
        }

        memset(m_buffer, 0, BlockSize);

        ulong bits = m_processedBytes * 8;

        int padindex = 0;
        m_buffer[padindex++] = (byte)bits;
        m_buffer[padindex++] = (byte)(bits >> 8);
        m_buffer[padindex++] = (byte)(bits >> 16);
        m_buffer[padindex++] = (byte)(bits >> 24);
        m_buffer[padindex++] = (byte)(bits >> 32);
        m_buffer[padindex++] = (byte)(bits >> 40);
        m_buffer[padindex++] = (byte)(bits >> 48);
        m_buffer[padindex++] = (byte)(bits >> 56);
    }

    void SIMD256Base::Finish() 
    {
        SIMDBase::Finish();
    
        TransformBlock(m_buffer, 0, (m_processedBytes  < 2048) ? 2 : 1);
    }

    void SIMD512Base::Finish() 
    {
        SIMDBase::Finish();
    
        TransformBlock(m_buffer, 0, (m_processedBytes  < 2048) ? 2 : 1);
    }
}