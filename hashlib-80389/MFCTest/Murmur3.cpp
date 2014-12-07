#include "stdafx.h"
#include "Murmur3.h"

namespace Murmur3
{

byte* Murmur3::MurmurHash3_x86_32(byte* a_data, int a_length)
{
    const uint C1 = 0xCC9E2D51;
    const uint C2 = 0x1B873593;
    const uint C3 = 0xE6546B64;
    const uint C4 = 0x85EBCA6B;
    const uint C5 = 0xC2B2AE35;

    uint h1 = SEED;

    int length = a_length;
    int current_index = 0;

    while (length >= 4)
    {
        uint k1 = (uint)a_data[current_index] |
                  ((uint)a_data[current_index+1] << 8) |
                  ((uint)a_data[current_index+2] << 16) |
                  ((uint)a_data[current_index+3] << 24);

        k1 *= C1;
        k1 = (k1 << 15) | (k1 >> 17);
        k1 *= C2;

        h1 ^= k1;
        h1 = (h1 << 13) | (h1 >> 19);
        h1 = h1 * 5 + C3;

        current_index += 4;
        length -= 4;
    }

    switch (length)
    {
        case 3:
        {
            uint k1 = a_data[current_index + 2] << 16 | (a_data[current_index + 1] << 8) | (a_data[current_index]);
            k1 *= C1;
            k1 = (k1 << 15) | (k1 >> 17);
            k1 *= C2;
            h1 ^= k1;
            break;
        }
        case 2:
        {
            uint k1 = (a_data[current_index + 1] << 8) | a_data[current_index];
            k1 *= C1;
            k1 = (k1 << 15) | (k1 >> 17);
            k1 *= C2;
            h1 ^= k1;
            break;
        }
        case 1:
        {
            uint k1 = a_data[current_index];
            k1 *= C1; 
            k1 = (k1 << 15) | (k1 >> 17);
            k1 *= C2; 
            h1 ^= k1;
            break;
        }
    };

    h1 ^= a_length;

    h1 ^= h1 >> 16;
    h1 *= C4;
    h1 ^= h1 >> 13;
    h1 *= C5;
    h1 ^= h1 >> 16;

    byte* out = new byte[4];

    out[0] = (byte)h1;
    out[1] = (byte)(h1 >> 8);
    out[2] = (byte)(h1 >> 16);
    out[3] = (byte)(h1 >> 24);

    return out;
}

byte* Murmur3::MurmurHash3_x64_128(byte* a_data, int a_length)
{
    const ulong C1 = 0x87C37B91114253D5;
    const ulong C2 = 0x4CF5AD432745937F;
    const uint C3 = 0x52DCE729;
    const uint C4 = 0x38495AB5;
    const ulong C5 = 0xFF51AFD7ED558CCD;
    const ulong C6 = 0xC4CEB9FE1A85EC53;

    ulong h1 = SEED;
    ulong h2 = SEED;

    int length = a_length;
    int current_index = 0;

    while (length >= 16)
    {
        ulong k1 = (ulong)a_data[current_index+0] | (ulong)a_data[current_index+1] << 8 |
                   (ulong)a_data[current_index+2] << 16 | (ulong)a_data[current_index+3] << 24 |
                   (ulong)a_data[current_index+4] << 32 | (ulong)a_data[current_index+5] << 40 |
                   (ulong)a_data[current_index+6] << 48 | (ulong)a_data[current_index+7] << 56;

        k1 *= C1; 
        k1 = (k1 << 31) | (k1 >> 33);
        k1 *= C2; 
        h1 ^= k1;

        h1 = (h1 << 27) | (h1 >> 37);
        h1 += h2; 
        h1 = h1 * 5 + C3;

        ulong k2 = (ulong)a_data[current_index + 8] | (ulong)a_data[current_index + 9] << 8 |
                   (ulong)a_data[current_index + 10] << 16 | (ulong)a_data[current_index + 11] << 24 |
                   (ulong)a_data[current_index + 12] << 32 | (ulong)a_data[current_index + 13] << 40 |
                   (ulong)a_data[current_index + 14] << 48 | (ulong)a_data[current_index + 15] << 56;

        k2 *= C2; 
        k2 = (k2 << 33) | (k2 >> 31);
        k2 *= C1; 
        h2 ^= k2;

        h2 = (h2 << 31) | (h2 >> 33);
        h2 += h1; 
        h2 = h2 * 5 + C4;

        current_index += 16;
        length -= 16;
    }

    ulong k1 = 0;
    ulong k2 = 0;

    switch (length)
    {
        case 15:
        {
                   k2 ^= ulong(a_data[current_index + 14]) << 48;
                   k2 ^= ulong(a_data[current_index + 13]) << 40;
                   k2 ^= ulong(a_data[current_index + 12]) << 32; 
                   k2 ^= ulong(a_data[current_index + 11]) << 24;
                   k2 ^= ulong(a_data[current_index + 10]) << 16;
                   k2 ^= ulong(a_data[current_index +  9]) << 8;
                   k2 ^= ulong(a_data[current_index +  8]) << 0;
                   k2 *= C2;
                   k2 = (k2 << 33) | (k2 >> 31);
                   k2 *= C1;
                   h2 ^= k2;
                   break;
        }
        case 14:
        {
                   k2 ^= ulong(a_data[current_index + 13]) << 40;
                   k2 ^= ulong(a_data[current_index + 12]) << 32; 
                   k2 ^= ulong(a_data[current_index + 11]) << 24;
                   k2 ^= ulong(a_data[current_index + 10]) << 16;
                   k2 ^= ulong(a_data[current_index + 9]) << 8;
                   k2 ^= ulong(a_data[current_index + 8]) << 0;
                   k2 *= C2;
                   k2 = (k2 << 33) | (k2 >> 31);
                   k2 *= C1;
                   h2 ^= k2;
                   break;
        }
        case 13:
        {
                   k2 ^= ulong(a_data[current_index + 12]) << 32;
                   k2 ^= ulong(a_data[current_index + 11]) << 24;
                   k2 ^= ulong(a_data[current_index + 10]) << 16;
                   k2 ^= ulong(a_data[current_index + 9]) << 8;
                   k2 ^= ulong(a_data[current_index + 8]) << 0;
                   k2 *= C2;
                   k2 = (k2 << 33) | (k2 >> 31);
                   k2 *= C1;
                   h2 ^= k2;
                   break;
        }
        case 12:
        {
                   k2 ^= ulong(a_data[current_index + 11]) << 24;
                   k2 ^= ulong(a_data[current_index + 10]) << 16;
                   k2 ^= ulong(a_data[current_index + 9]) << 8;
                   k2 ^= ulong(a_data[current_index + 8]) << 0;
                   k2 *= C2;
                   k2 = (k2 << 33) | (k2 >> 31);
                   k2 *= C1;
                   h2 ^= k2;
                   break;
        }
        case 11:
        {
                   k2 ^= ulong(a_data[current_index + 10]) << 16;
                   k2 ^= ulong(a_data[current_index + 9]) << 8;
                   k2 ^= ulong(a_data[current_index + 8]) << 0;
                   k2 *= C2;
                   k2 = (k2 << 33) | (k2 >> 31);
                   k2 *= C1;
                   h2 ^= k2;
                   break;
        }
        case 10:
        {
                   k2 ^= ulong(a_data[current_index + 9]) << 8;
                   k2 ^= ulong(a_data[current_index + 8]) << 0;
                   k2 *= C2;
                   k2 = (k2 << 33) | (k2 >> 31);
                   k2 *= C1;
                   h2 ^= k2;
                   break;
        }
        case 9:
        {
                   k2 ^= ulong(a_data[current_index + 8]) << 0;
                   k2 *= C2;
                   k2 = (k2 << 33) | (k2 >> 31);
                   k2 *= C1;
                   h2 ^= k2;
                   break;
        }
    }

    if (length > 8)
        length = 8;

    switch (length)
    {
        case 8:
        {
                   k1 ^= ulong(a_data[current_index + 7]) << 56;
                   k1 ^= ulong(a_data[current_index + 6]) << 48;
                   k1 ^= ulong(a_data[current_index + 5]) << 40;
                   k1 ^= ulong(a_data[current_index + 4]) << 32;
                   k1 ^= ulong(a_data[current_index + 3]) << 24;
                   k1 ^= ulong(a_data[current_index + 2]) << 16;
                   k1 ^= ulong(a_data[current_index + 1]) << 8;
                   k1 ^= ulong(a_data[current_index]) << 0;
                   k1 *= C1;
                   k1 = (k1 << 31) | (k1 >> 33);
                   k1 *= C2;
                   h1 ^= k1;
                   break;
        }
        case 7:
        {
                   k1 ^= ulong(a_data[current_index + 6]) << 48;
                   k1 ^= ulong(a_data[current_index + 5]) << 40;
                   k1 ^= ulong(a_data[current_index + 4]) << 32;
                   k1 ^= ulong(a_data[current_index + 3]) << 24;
                   k1 ^= ulong(a_data[current_index + 2]) << 16;
                   k1 ^= ulong(a_data[current_index + 1]) << 8;
                   k1 ^= ulong(a_data[current_index]) << 0;
                   k1 *= C1;
                   k1 = (k1 << 31) | (k1 >> 33);
                   k1 *= C2;
                   h1 ^= k1;
                   break;
        }
        case 6:
        {
                   k1 ^= ulong(a_data[current_index + 5]) << 40;
                   k1 ^= ulong(a_data[current_index + 4]) << 32;
                   k1 ^= ulong(a_data[current_index + 3]) << 24;
                   k1 ^= ulong(a_data[current_index + 2]) << 16;
                   k1 ^= ulong(a_data[current_index + 1]) << 8;
                   k1 ^= ulong(a_data[current_index]) << 0;
                   k1 *= C1;
                   k1 = (k1 << 31) | (k1 >> 33);
                   k1 *= C2;
                   h1 ^= k1;
                   break;
        }
        case 5:
        {
                   k1 ^= ulong(a_data[current_index + 4]) << 32;
                   k1 ^= ulong(a_data[current_index + 3]) << 24;
                   k1 ^= ulong(a_data[current_index + 2]) << 16;
                   k1 ^= ulong(a_data[current_index + 1]) << 8;
                   k1 ^= ulong(a_data[current_index]) << 0;
                   k1 *= C1;
                   k1 = (k1 << 31) | (k1 >> 33);
                   k1 *= C2;
                   h1 ^= k1;
                   break;
        }
        case 4:
        {
                   k1 ^= ulong(a_data[current_index + 3]) << 24;
                   k1 ^= ulong(a_data[current_index + 2]) << 16;
                   k1 ^= ulong(a_data[current_index + 1]) << 8;
                   k1 ^= ulong(a_data[current_index]) << 0;
                   k1 *= C1;
                   k1 = (k1 << 31) | (k1 >> 33);
                   k1 *= C2;
                   h1 ^= k1;
                   break;
        }
        case 3:
        {
                   k1 ^= ulong(a_data[current_index + 2]) << 16;
                   k1 ^= ulong(a_data[current_index + 1]) << 8;
                   k1 ^= ulong(a_data[current_index]) << 0;
                   k1 *= C1;
                   k1 = (k1 << 31) | (k1 >> 33);
                   k1 *= C2;
                   h1 ^= k1;
                   break;
        }
        case 2:
        {
                   k1 ^= ulong(a_data[current_index + 1]) << 8;
                   k1 ^= ulong(a_data[current_index]) << 0;
                   k1 *= C1;
                   k1 = (k1 << 31) | (k1 >> 33);
                   k1 *= C2;
                   h1 ^= k1;
                   break;
        }
        case 1:
        {
                   k1 ^= ulong(a_data[current_index]) << 0;
                   k1 *= C1; 
                   k1 = (k1 << 31) | (k1 >> 33);
                   k1 *= C2; 
                   h1 ^= k1;
                   break;
        }
    };

    h1 ^= a_length; 
    h2 ^= a_length;

    h1 += h2;
    h2 += h1;

    h1 ^= h1 >> 33;
    h1 *= C5;
    h1 ^= h1 >> 33;
    h1 *= C6;
    h1 ^= h1 >> 33;

    h2 ^= h2 >> 33;
    h2 *= C5;
    h2 ^= h2 >> 33;
    h2 *= C6;
    h2 ^= h2 >> 33;

    h1 += h2;
    h2 += h1;

    byte* out = new byte[16];

    out[0] = (byte)h1;
    out[1] = (byte)(h1 >> 8);
    out[2] = (byte)(h1 >> 16);
    out[3] = (byte)(h1 >> 24);
    out[4] = (byte)(h1 >> 32);
    out[5] = (byte)(h1 >> 40);
    out[6] = (byte)(h1 >> 48);
    out[7] = (byte)(h1 >> 56);

    out[8] = (byte)h2;
    out[9] = (byte)(h2 >> 8);
    out[10] = (byte)(h2 >> 16);
    out[11] = (byte)(h2 >> 24);
    out[12] = (byte)(h2 >> 32);
    out[13] = (byte)(h2 >> 40);
    out[14] = (byte)(h2 >> 48);
    out[15] = (byte)(h2 >> 56);

    return out;
}

}