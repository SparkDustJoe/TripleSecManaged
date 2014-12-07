
#include "stdafx.h"
#include "Luffa.h"

namespace Luffa
{
    const uint LuffaBase::s_IV[40] = 
    {
        0x6d251e69,0x44b051e0,0x4eaa6fb4,0xdbf78465,
        0x6e292011,0x90152df4,0xee058139,0xdef610bb,
        0xc3b44b95,0xd9d2f256,0x70eee9a0,0xde099fa3,
        0x5d9b0557,0x8fc944b3,0xcf1ccf0e,0x746cd581,
        0xf7efc89d,0x5dba5781,0x04016ce5,0xad659c05,
        0x0306194f,0x666d1836,0x24aa230a,0x8b264ae7,
        0x858075d5,0x36d79cce,0xe571f7d7,0x204b1f67,
        0x35870c6a,0x57e9e923,0x14bcb808,0x7cde72ce,
        0x6c68e9be,0x5ec41e22,0xc825b7c7,0xaffb4363,
        0xf5df3999,0x0fc688f1,0xb07224cc,0x03e86cea
    };

    const uint LuffaBase::s_CNS[80] = 
    {
        0x303994a6,0xe0337818,0xc0e65299,0x441ba90d,
        0x6cc33a12,0x7f34d442,0xdc56983e,0x9389217f,
        0x1e00108f,0xe5a8bce6,0x7800423d,0x5274baf4,
        0x8f5b7882,0x26889ba7,0x96e1db12,0x9a226e9d,
        0xb6de10ed,0x01685f3d,0x70f47aae,0x05a17cf4,
        0x0707a3d4,0xbd09caca,0x1c1e8f51,0xf4272b28,
        0x707a3d45,0x144ae5cc,0xaeb28562,0xfaa7ae2b,
        0xbaca1589,0x2e48f1c1,0x40a46f3e,0xb923c704,
        0xfc20d9d2,0xe25e72c1,0x34552e25,0xe623bb72,
        0x7ad8818f,0x5c58a4a4,0x8438764a,0x1e38e2e7,
        0xbb6de032,0x78e38b9d,0xedb780c8,0x27586719,
        0xd9847356,0x36eda57f,0xa2c78434,0x703aace7,
        0xb213afa5,0xe028c9bf,0xc84ebe95,0x44756f91,
        0x4e608a22,0x7e8fce32,0x56d858fe,0x956548be,
        0x343b138f,0xfe191be2,0xd0ec4e3d,0x3cb226e5,
        0x2ceb4882,0x5944a28e,0xb3ad2208,0xa1c4c355,
        0xf0d2e9e3,0x5090d577,0xac11d7fa,0x2d1925ab,
        0x1bcb66f2,0xb46496ac,0x6f2d9bc9,0xd1925ab0,
        0x78602649,0x29131ab6,0x8edae952,0x0fc053c3,
        0x3b6ba548,0x3f014f0c,0xedae9520,0xfc053c31
    };

    void LuffaBase::ByteSwap(uint* a_array, int a_length)
    {
        for (int i=0; i<a_length; i++)
            a_array[i] = ((a_array[i] << 24) | ((a_array[i] & 0x0000ff00) << 8) | ((a_array[i] & 0x00ff0000) >> 8) | (a_array[i] >> 24));
    }

    byte* LuffaBase::ComputeBytes(byte* a_data, int a_length) 
    {
        Initialize();
        TransformBytes(a_data, 0, a_length);
        return TransformFinal();
    }

    void LuffaBase::Initialize()
    {
        m_buffer_pos = 0;

        memcpy(m_state, s_IV, m_iv_length*4);

        memset(m_buffer, 0, BlockSize);
    }

    void LuffaBase::TransformBytes(byte* a_data, int a_index, int a_length) 
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

            if (m_buffer_pos == BlockSize)
            {
                TransformBlock(m_buffer, 0);
                m_buffer_pos = 0;
            } 
        }

        while (a_length >= BlockSize)
        {
            TransformBlock(a_data, a_index);

            a_index += BlockSize;
            a_length -= BlockSize;
        }

        if (a_length > 0)
        {
            memcpy(&m_buffer[m_buffer_pos], &a_data[a_index], a_length);
            m_buffer_pos += a_length;
        }
    }

    byte* LuffaBase::TransformFinal() 
    {
        Finish();
        byte* result = GetResult();
        Initialize();
        return result;
    }

    void Luffa256Base::TransformBlock(byte* a_data, int a_index)
    {
        uint chainv[8];
        uint tmp;

        uint data[8];
        memcpy(data, &a_data[a_index], BlockSize);
        ByteSwap(data, 8);

        uint t[8];
        memset(t, 0, 32);

        for(int i=0;i<8;i++) 
        {
            for(int j=0;j<3;j++) 
                t[i] ^= m_state[i+8*j];
        }

        tmp = t[7]; 
        t[7] = t[6]; 
        t[6] = t[5]; 
        t[5] = t[4]; 
        t[4] = t[3] ^ tmp; 
        t[3] = t[2] ^ tmp; 
        t[2] = t[1]; 
        t[1] = t[0] ^ tmp; 
        t[0] = tmp;

        for(int j=0;j< 3;j++)
        {
            for(int i=0;i<8;i++)
                m_state[i+8*j] ^= t[i] ^ data[i];

            tmp = data[7];
            data[7] = data[6];
            data[6] = data[5];
            data[5] = data[4];
            data[4] = data[3] ^ tmp;
            data[3] = data[2] ^ tmp;
            data[2] = data[1];
            data[1] = data[0] ^ tmp;
            data[0] = tmp;
        }

        for(int i=0;i<8;i++) 
            chainv[i] = m_state[i];

        for(int i=0;i<8;i++) 
        {
            tmp = chainv[0];
            chainv[0] |= chainv[1];
            chainv[2] ^= chainv[3];
            chainv[1] = ~chainv[1];
            chainv[0] ^= chainv[3];
            chainv[3] &= tmp;
            chainv[1] ^= chainv[3];
            chainv[3] ^= chainv[2];
            chainv[2] &= chainv[0];
            chainv[0] = ~chainv[0];
            chainv[2] ^= chainv[1];
            chainv[1] |= chainv[3]; tmp ^= chainv[1];
            chainv[3] ^= chainv[2];
            chainv[2] &= chainv[1];
            chainv[1] ^= chainv[0];
            chainv[0] = tmp; tmp = chainv[5];
            chainv[5] |= chainv[6];
            chainv[7] ^= chainv[4];
            chainv[6] = ~chainv[6];
            chainv[5] ^= chainv[4];
            chainv[4] &= tmp;
            chainv[6] ^= chainv[4];
            chainv[4] ^= chainv[7];
            chainv[7] &= chainv[5];
            chainv[5] = ~chainv[5];
            chainv[7] ^= chainv[6];
            chainv[6] |= chainv[4]; tmp ^= chainv[6];
            chainv[4] ^= chainv[7];
            chainv[7] &= chainv[6];
            chainv[6] ^= chainv[5];
            chainv[5] = tmp;
            chainv[4] ^= chainv[0];
            chainv[0] = (chainv[0] << 2) | (chainv[0] >> 30);
            chainv[0] ^= chainv[4];
            chainv[4] = (chainv[4] << 14) | (chainv[4] >> 18);
            chainv[4] ^= chainv[0];
            chainv[0] = (chainv[0] << 10) | (chainv[0] >> 22);
            chainv[0] ^= chainv[4];
            chainv[4] = (chainv[4] << 1) | (chainv[4] >> 31);
            chainv[5] ^= chainv[1];
            chainv[1] = (chainv[1] << 2) | (chainv[1] >> 30);
            chainv[1] ^= chainv[5];
            chainv[5] = (chainv[5] << 14) | (chainv[5] >> 18);
            chainv[5] ^= chainv[1];
            chainv[1] = (chainv[1] << 10) | (chainv[1] >> 22);
            chainv[1] ^= chainv[5];
            chainv[5] = (chainv[5] << 1) | (chainv[5] >> 31);
            chainv[6] ^= chainv[2];
            chainv[2] = (chainv[2] << 2) | (chainv[2] >> 30);
            chainv[2] ^= chainv[6];
            chainv[6] = (chainv[6] << 14) | (chainv[6] >> 18);
            chainv[6] ^= chainv[2];
            chainv[2] = (chainv[2] << 10) | (chainv[2] >> 22);
            chainv[2] ^= chainv[6];
            chainv[6] = (chainv[6] << 1) | (chainv[6] >> 31);
            chainv[7] ^= chainv[3];
            chainv[3] = (chainv[3] << 2) | (chainv[3] >> 30);
            chainv[3] ^= chainv[7];
            chainv[7] = (chainv[7] << 14) | (chainv[7] >> 18);
            chainv[7] ^= chainv[3];
            chainv[3] = (chainv[3] << 10) | (chainv[3] >> 22);
            chainv[3] ^= chainv[7];
            chainv[7] = (chainv[7] << 1) | (chainv[7] >> 31);
            chainv[0] ^= s_CNS[(2*i)];
            chainv[4] ^= s_CNS[(2*i)+1];
        }

        for(int i=0;i<8;i++) 
        {
            m_state[i] = chainv[i];
            chainv[i] = m_state[i+8];
        }

        chainv[4] = (chainv[4] << 1) | (chainv[4] >> 31);
        chainv[5] = (chainv[5] << 1) | (chainv[5] >> 31);
        chainv[6] = (chainv[6] << 1) | (chainv[6] >> 31);
        chainv[7] = (chainv[7] << 1) | (chainv[7] >> 31);

        for(int i=0;i<8;i++) 
        {
            tmp = chainv[0];
            chainv[0] |= chainv[1];
            chainv[2] ^= chainv[3];
            chainv[1] = ~chainv[1];
            chainv[0] ^= chainv[3];
            chainv[3] &= tmp;
            chainv[1] ^= chainv[3];
            chainv[3] ^= chainv[2];
            chainv[2] &= chainv[0];
            chainv[0] = ~chainv[0];
            chainv[2] ^= chainv[1];
            chainv[1] |= chainv[3]; tmp ^= chainv[1];
            chainv[3] ^= chainv[2];
            chainv[2] &= chainv[1];
            chainv[1] ^= chainv[0];
            chainv[0] = tmp; tmp = chainv[5];
            chainv[5] |= chainv[6];
            chainv[7] ^= chainv[4];
            chainv[6] = ~chainv[6];
            chainv[5] ^= chainv[4];
            chainv[4] &= tmp;
            chainv[6] ^= chainv[4];
            chainv[4] ^= chainv[7];
            chainv[7] &= chainv[5];
            chainv[5] = ~chainv[5];
            chainv[7] ^= chainv[6];
            chainv[6] |= chainv[4]; tmp ^= chainv[6];
            chainv[4] ^= chainv[7];
            chainv[7] &= chainv[6];
            chainv[6] ^= chainv[5];
            chainv[5] = tmp;
            chainv[4] ^= chainv[0];
            chainv[0] = (chainv[0] << 2) | (chainv[0] >> 30);
            chainv[0] ^= chainv[4];
            chainv[4] = (chainv[4] << 14) | (chainv[4] >> 18);
            chainv[4] ^= chainv[0];
            chainv[0] = (chainv[0] << 10) | (chainv[0] >> 22);
            chainv[0] ^= chainv[4];
            chainv[4] = (chainv[4] << 1) | (chainv[4] >> 31);
            chainv[5] ^= chainv[1];
            chainv[1] = (chainv[1] << 2) | (chainv[1] >> 30);
            chainv[1] ^= chainv[5];
            chainv[5] = (chainv[5] << 14) | (chainv[5] >> 18);
            chainv[5] ^= chainv[1];
            chainv[1] = (chainv[1] << 10) | (chainv[1] >> 22);
            chainv[1] ^= chainv[5];
            chainv[5] = (chainv[5] << 1) | (chainv[5] >> 31);
            chainv[6] ^= chainv[2];
            chainv[2] = (chainv[2] << 2) | (chainv[2] >> 30);
            chainv[2] ^= chainv[6];
            chainv[6] = (chainv[6] << 14) | (chainv[6] >> 18);
            chainv[6] ^= chainv[2];
            chainv[2] = (chainv[2] << 10) | (chainv[2] >> 22);
            chainv[2] ^= chainv[6];
            chainv[6] = (chainv[6] << 1) | (chainv[6] >> 31);
            chainv[7] ^= chainv[3];
            chainv[3] = (chainv[3] << 2) | (chainv[3] >> 30);
            chainv[3] ^= chainv[7];
            chainv[7] = (chainv[7] << 14) | (chainv[7] >> 18);
            chainv[7] ^= chainv[3];
            chainv[3] = (chainv[3] << 10) | (chainv[3] >> 22);
            chainv[3] ^= chainv[7];
            chainv[7] = (chainv[7] << 1) | (chainv[7] >> 31);
            chainv[0] ^= s_CNS[(2*i)+16];
            chainv[4] ^= s_CNS[(2*i)+16+1];
        }

        for(int i=0;i<8;i++) 
        {
            m_state[i+8] = chainv[i];
            chainv[i] = m_state[i+16];
        }

        chainv[4] = (chainv[4] << 2) | (chainv[4] >> 30);
        chainv[5] = (chainv[5] << 2) | (chainv[5] >> 30);
        chainv[6] = (chainv[6] << 2) | (chainv[6] >> 30);
        chainv[7] = (chainv[7] << 2) | (chainv[7] >> 30);

        for(int i=0;i<8;i++) 
        {
            tmp = chainv[0];
            chainv[0] |= chainv[1];
            chainv[2] ^= chainv[3];
            chainv[1] = ~chainv[1];
            chainv[0] ^= chainv[3];
            chainv[3] &= tmp;
            chainv[1] ^= chainv[3];
            chainv[3] ^= chainv[2];
            chainv[2] &= chainv[0];
            chainv[0] = ~chainv[0];
            chainv[2] ^= chainv[1];
            chainv[1] |= chainv[3]; tmp ^= chainv[1];
            chainv[3] ^= chainv[2];
            chainv[2] &= chainv[1];
            chainv[1] ^= chainv[0];
            chainv[0] = tmp; tmp = chainv[5];
            chainv[5] |= chainv[6];
            chainv[7] ^= chainv[4];
            chainv[6] = ~chainv[6];
            chainv[5] ^= chainv[4];
            chainv[4] &= tmp;
            chainv[6] ^= chainv[4];
            chainv[4] ^= chainv[7];
            chainv[7] &= chainv[5];
            chainv[5] = ~chainv[5];
            chainv[7] ^= chainv[6];
            chainv[6] |= chainv[4]; tmp ^= chainv[6];
            chainv[4] ^= chainv[7];
            chainv[7] &= chainv[6];
            chainv[6] ^= chainv[5];
            chainv[5] = tmp;
            chainv[4] ^= chainv[0];
            chainv[0] = (chainv[0] << 2) | (chainv[0] >> 30);
            chainv[0] ^= chainv[4];
            chainv[4] = (chainv[4] << 14) | (chainv[4] >> 18);
            chainv[4] ^= chainv[0];
            chainv[0] = (chainv[0] << 10) | (chainv[0] >> 22);
            chainv[0] ^= chainv[4];
            chainv[4] = (chainv[4] << 1) | (chainv[4] >> 31);
            chainv[5] ^= chainv[1];
            chainv[1] = (chainv[1] << 2) | (chainv[1] >> 30);
            chainv[1] ^= chainv[5];
            chainv[5] = (chainv[5] << 14) | (chainv[5] >> 18);
            chainv[5] ^= chainv[1];
            chainv[1] = (chainv[1] << 10) | (chainv[1] >> 22);
            chainv[1] ^= chainv[5];
            chainv[5] = (chainv[5] << 1) | (chainv[5] >> 31);
            chainv[6] ^= chainv[2];
            chainv[2] = (chainv[2] << 2) | (chainv[2] >> 30);
            chainv[2] ^= chainv[6];
            chainv[6] = (chainv[6] << 14) | (chainv[6] >> 18);
            chainv[6] ^= chainv[2];
            chainv[2] = (chainv[2] << 10) | (chainv[2] >> 22);
            chainv[2] ^= chainv[6];
            chainv[6] = (chainv[6] << 1) | (chainv[6] >> 31);
            chainv[7] ^= chainv[3];
            chainv[3] = (chainv[3] << 2) | (chainv[3] >> 30);
            chainv[3] ^= chainv[7];
            chainv[7] = (chainv[7] << 14) | (chainv[7] >> 18);
            chainv[7] ^= chainv[3];
            chainv[3] = (chainv[3] << 10) | (chainv[3] >> 22);
            chainv[3] ^= chainv[7];
            chainv[7] = (chainv[7] << 1) | (chainv[7] >> 31);
            chainv[0] ^= s_CNS[(2*i)+32];
            chainv[4] ^= s_CNS[(2*i)+32+1];
        }

        for(int i=0;i<8;i++) 
            m_state[i+16] = chainv[i];
    }

    void Luffa384::TransformBlock(byte* a_data, int a_index)
    {
        uint chainv[8];
        uint tmp;

        uint data[8];
        memcpy(data, &a_data[a_index], BlockSize);
        ByteSwap(data, 8);

        uint t[32];
        memset(t, 0, 128);

        for(int i=0;i<8;i++) 
        {
            for(int j=0;j<4;j++) 
                t[i] ^= m_state[i+8*j];
        }

        tmp = t[7];
        t[7] = t[6];
        t[6] = t[5];
        t[5] = t[4];
        t[4] = t[3] ^ tmp;
        t[3] = t[2] ^ tmp;
        t[2] = t[1];
        t[1] = t[0] ^ tmp;
        t[0] = tmp;

        for(int j=0;j<4;j++) 
        {
            for(int i=0;i<8;i++) 
                m_state[i+8*j] ^= t[i];
        }

        for(int j=0;j<4;j++) 
        {
            for(int i=0;i<8;i++) 
                t[i+8*j] = m_state[i+8*j];
        }

        for(int j=0;j<4;j++) 
        {
            tmp = m_state[7 + (8*j)];
            m_state[7 + (8*j)] = m_state[6 + (8*j)];
            m_state[6 + (8*j)] = m_state[5 + (8*j)];
            m_state[5 + (8*j)] = m_state[4 + (8*j)];
            m_state[4 + (8*j)] = m_state[3 + (8*j)] ^ tmp;
            m_state[3 + (8*j)] = m_state[2 + (8*j)] ^ tmp;
            m_state[2 + (8*j)] = m_state[1 + (8*j)];
            m_state[1 + (8*j)] = m_state[0 + (8*j)] ^ tmp;
            m_state[8*j] = tmp;
        }

        for(int j=0;j<4;j++) 
        {
            for(int i=0;i<8;i++) 
                m_state[8*j+i] ^= t[8*((j+3)%4)+i];
        }

        for(int j=0;j<4;j++) 
        {
            for(int i=0;i<8;i++) 
                m_state[i+8*j] ^= data[i];
            
            tmp = data[7];
            data[7] = data[6];
            data[6] = data[5];
            data[5] = data[4];
            data[4] = data[3] ^ tmp;
            data[3] = data[2] ^ tmp;
            data[2] = data[1];
            data[1] = data[0] ^ tmp;
            data[0] = tmp;
        }

        for(int i=0;i<8;i++) 
            chainv[i] = m_state[i];

        for(int i=0;i<8;i++) 
        {
            tmp = chainv[0];
            chainv[0] |= chainv[1];
            chainv[2] ^= chainv[3];
            chainv[1] = ~chainv[1];
            chainv[0] ^= chainv[3];
            chainv[3] &= tmp;
            chainv[1] ^= chainv[3];
            chainv[3] ^= chainv[2];
            chainv[2] &= chainv[0];
            chainv[0] = ~chainv[0];
            chainv[2] ^= chainv[1];
            chainv[1] |= chainv[3]; tmp ^= chainv[1];
            chainv[3] ^= chainv[2];
            chainv[2] &= chainv[1];
            chainv[1] ^= chainv[0];
            chainv[0] = tmp; tmp = chainv[5];
            chainv[5] |= chainv[6];
            chainv[7] ^= chainv[4];
            chainv[6] = ~chainv[6];
            chainv[5] ^= chainv[4];
            chainv[4] &= tmp;
            chainv[6] ^= chainv[4];
            chainv[4] ^= chainv[7];
            chainv[7] &= chainv[5];
            chainv[5] = ~chainv[5];
            chainv[7] ^= chainv[6];
            chainv[6] |= chainv[4]; tmp ^= chainv[6];
            chainv[4] ^= chainv[7];
            chainv[7] &= chainv[6];
            chainv[6] ^= chainv[5];
            chainv[5] = tmp;
            chainv[4] ^= chainv[0];
            chainv[0] = (chainv[0] << 2) | (chainv[0] >> 30);
            chainv[0] ^= chainv[4];
            chainv[4] = (chainv[4] << 14) | (chainv[4] >> 18);
            chainv[4] ^= chainv[0];
            chainv[0] = (chainv[0] << 10) | (chainv[0] >> 22);
            chainv[0] ^= chainv[4];
            chainv[4] = (chainv[4] << 1) | (chainv[4] >> 31);
            chainv[5] ^= chainv[1];
            chainv[1] = (chainv[1] << 2) | (chainv[1] >> 30);
            chainv[1] ^= chainv[5];
            chainv[5] = (chainv[5] << 14) | (chainv[5] >> 18);
            chainv[5] ^= chainv[1];
            chainv[1] = (chainv[1] << 10) | (chainv[1] >> 22);
            chainv[1] ^= chainv[5];
            chainv[5] = (chainv[5] << 1) | (chainv[5] >> 31);
            chainv[6] ^= chainv[2];
            chainv[2] = (chainv[2] << 2) | (chainv[2] >> 30);
            chainv[2] ^= chainv[6];
            chainv[6] = (chainv[6] << 14) | (chainv[6] >> 18);
            chainv[6] ^= chainv[2];
            chainv[2] = (chainv[2] << 10) | (chainv[2] >> 22);
            chainv[2] ^= chainv[6];
            chainv[6] = (chainv[6] << 1) | (chainv[6] >> 31);
            chainv[7] ^= chainv[3];
            chainv[3] = (chainv[3] << 2) | (chainv[3] >> 30);
            chainv[3] ^= chainv[7];
            chainv[7] = (chainv[7] << 14) | (chainv[7] >> 18);
            chainv[7] ^= chainv[3];
            chainv[3] = (chainv[3] << 10) | (chainv[3] >> 22);
            chainv[3] ^= chainv[7];
            chainv[7] = (chainv[7] << 1) | (chainv[7] >> 31);
            chainv[0] ^= s_CNS[(2*i)];
            chainv[4] ^= s_CNS[(2*i)+1];
        }

        for(int i=0;i<8;i++) 
        {
            m_state[i] = chainv[i];
            chainv[i] = m_state[i+8];
        }

        chainv[4] = (chainv[4] << 1) | (chainv[4] >> 31);
        chainv[5] = (chainv[5] << 1) | (chainv[5] >> 31);
        chainv[6] = (chainv[6] << 1) | (chainv[6] >> 31);
        chainv[7] = (chainv[7] << 1) | (chainv[7] >> 31);

        for(int i=0;i<8;i++) 
        {
            tmp = chainv[0];
            chainv[0] |= chainv[1];
            chainv[2] ^= chainv[3];
            chainv[1] = ~chainv[1];
            chainv[0] ^= chainv[3];
            chainv[3] &= tmp;
            chainv[1] ^= chainv[3];
            chainv[3] ^= chainv[2];
            chainv[2] &= chainv[0];
            chainv[0] = ~chainv[0];
            chainv[2] ^= chainv[1];
            chainv[1] |= chainv[3]; tmp ^= chainv[1];
            chainv[3] ^= chainv[2];
            chainv[2] &= chainv[1];
            chainv[1] ^= chainv[0];
            chainv[0] = tmp; tmp = chainv[5];
            chainv[5] |= chainv[6];
            chainv[7] ^= chainv[4];
            chainv[6] = ~chainv[6];
            chainv[5] ^= chainv[4];
            chainv[4] &= tmp;
            chainv[6] ^= chainv[4];
            chainv[4] ^= chainv[7];
            chainv[7] &= chainv[5];
            chainv[5] = ~chainv[5];
            chainv[7] ^= chainv[6];
            chainv[6] |= chainv[4]; tmp ^= chainv[6];
            chainv[4] ^= chainv[7];
            chainv[7] &= chainv[6];
            chainv[6] ^= chainv[5];
            chainv[5] = tmp;
            chainv[4] ^= chainv[0];
            chainv[0] = (chainv[0] << 2) | (chainv[0] >> 30);
            chainv[0] ^= chainv[4];
            chainv[4] = (chainv[4] << 14) | (chainv[4] >> 18);
            chainv[4] ^= chainv[0];
            chainv[0] = (chainv[0] << 10) | (chainv[0] >> 22);
            chainv[0] ^= chainv[4];
            chainv[4] = (chainv[4] << 1) | (chainv[4] >> 31);
            chainv[5] ^= chainv[1];
            chainv[1] = (chainv[1] << 2) | (chainv[1] >> 30);
            chainv[1] ^= chainv[5];
            chainv[5] = (chainv[5] << 14) | (chainv[5] >> 18);
            chainv[5] ^= chainv[1];
            chainv[1] = (chainv[1] << 10) | (chainv[1] >> 22);
            chainv[1] ^= chainv[5];
            chainv[5] = (chainv[5] << 1) | (chainv[5] >> 31);
            chainv[6] ^= chainv[2];
            chainv[2] = (chainv[2] << 2) | (chainv[2] >> 30);
            chainv[2] ^= chainv[6];
            chainv[6] = (chainv[6] << 14) | (chainv[6] >> 18);
            chainv[6] ^= chainv[2];
            chainv[2] = (chainv[2] << 10) | (chainv[2] >> 22);
            chainv[2] ^= chainv[6];
            chainv[6] = (chainv[6] << 1) | (chainv[6] >> 31);
            chainv[7] ^= chainv[3];
            chainv[3] = (chainv[3] << 2) | (chainv[3] >> 30);
            chainv[3] ^= chainv[7];
            chainv[7] = (chainv[7] << 14) | (chainv[7] >> 18);
            chainv[7] ^= chainv[3];
            chainv[3] = (chainv[3] << 10) | (chainv[3] >> 22);
            chainv[3] ^= chainv[7];
            chainv[7] = (chainv[7] << 1) | (chainv[7] >> 31);
            chainv[0] ^= s_CNS[(2*i)+16];
            chainv[4] ^= s_CNS[(2*i)+16+1];
        }

        for(int i=0;i<8;i++) 
        {
            m_state[i+8] = chainv[i];
            chainv[i] = m_state[i+16];
        }

        chainv[4] = (chainv[4] << 2) | (chainv[4] >> 30);
        chainv[5] = (chainv[5] << 2) | (chainv[5] >> 30);
        chainv[6] = (chainv[6] << 2) | (chainv[6] >> 30);
        chainv[7] = (chainv[7] << 2) | (chainv[7] >> 30);

        for(int i=0;i<8;i++) 
        {
            tmp = chainv[0];
            chainv[0] |= chainv[1];
            chainv[2] ^= chainv[3];
            chainv[1] = ~chainv[1];
            chainv[0] ^= chainv[3];
            chainv[3] &= tmp;
            chainv[1] ^= chainv[3];
            chainv[3] ^= chainv[2];
            chainv[2] &= chainv[0];
            chainv[0] = ~chainv[0];
            chainv[2] ^= chainv[1];
            chainv[1] |= chainv[3]; tmp ^= chainv[1];
            chainv[3] ^= chainv[2];
            chainv[2] &= chainv[1];
            chainv[1] ^= chainv[0];
            chainv[0] = tmp; tmp = chainv[5];
            chainv[5] |= chainv[6];
            chainv[7] ^= chainv[4];
            chainv[6] = ~chainv[6];
            chainv[5] ^= chainv[4];
            chainv[4] &= tmp;
            chainv[6] ^= chainv[4];
            chainv[4] ^= chainv[7];
            chainv[7] &= chainv[5];
            chainv[5] = ~chainv[5];
            chainv[7] ^= chainv[6];
            chainv[6] |= chainv[4]; tmp ^= chainv[6];
            chainv[4] ^= chainv[7];
            chainv[7] &= chainv[6];
            chainv[6] ^= chainv[5];
            chainv[5] = tmp;
            chainv[4] ^= chainv[0];
            chainv[0] = (chainv[0] << 2) | (chainv[0] >> 30);
            chainv[0] ^= chainv[4];
            chainv[4] = (chainv[4] << 14) | (chainv[4] >> 18);
            chainv[4] ^= chainv[0];
            chainv[0] = (chainv[0] << 10) | (chainv[0] >> 22);
            chainv[0] ^= chainv[4];
            chainv[4] = (chainv[4] << 1) | (chainv[4] >> 31);
            chainv[5] ^= chainv[1];
            chainv[1] = (chainv[1] << 2) | (chainv[1] >> 30);
            chainv[1] ^= chainv[5];
            chainv[5] = (chainv[5] << 14) | (chainv[5] >> 18);
            chainv[5] ^= chainv[1];
            chainv[1] = (chainv[1] << 10) | (chainv[1] >> 22);
            chainv[1] ^= chainv[5];
            chainv[5] = (chainv[5] << 1) | (chainv[5] >> 31);
            chainv[6] ^= chainv[2];
            chainv[2] = (chainv[2] << 2) | (chainv[2] >> 30);
            chainv[2] ^= chainv[6];
            chainv[6] = (chainv[6] << 14) | (chainv[6] >> 18);
            chainv[6] ^= chainv[2];
            chainv[2] = (chainv[2] << 10) | (chainv[2] >> 22);
            chainv[2] ^= chainv[6];
            chainv[6] = (chainv[6] << 1) | (chainv[6] >> 31);
            chainv[7] ^= chainv[3];
            chainv[3] = (chainv[3] << 2) | (chainv[3] >> 30);
            chainv[3] ^= chainv[7];
            chainv[7] = (chainv[7] << 14) | (chainv[7] >> 18);
            chainv[7] ^= chainv[3];
            chainv[3] = (chainv[3] << 10) | (chainv[3] >> 22);
            chainv[3] ^= chainv[7];
            chainv[7] = (chainv[7] << 1) | (chainv[7] >> 31);
            chainv[0] ^= s_CNS[(2*i)+32];
            chainv[4] ^= s_CNS[(2*i)+32+1];
        }

        for(int i=0;i<8;i++) 
        {
            m_state[i+16] = chainv[i];
            chainv[i] = m_state[i+24];
        }

        chainv[4] = (chainv[4] << 3) | (chainv[4] >> 29);
        chainv[5] = (chainv[5] << 3) | (chainv[5] >> 29);
        chainv[6] = (chainv[6] << 3) | (chainv[6] >> 29);
        chainv[7] = (chainv[7] << 3) | (chainv[7] >> 29);

        for(int i=0;i<8;i++) 
        {
            tmp = chainv[0];
            chainv[0] |= chainv[1];
            chainv[2] ^= chainv[3];
            chainv[1] = ~chainv[1];
            chainv[0] ^= chainv[3];
            chainv[3] &= tmp;
            chainv[1] ^= chainv[3];
            chainv[3] ^= chainv[2];
            chainv[2] &= chainv[0];
            chainv[0] = ~chainv[0];
            chainv[2] ^= chainv[1];
            chainv[1] |= chainv[3]; tmp ^= chainv[1];
            chainv[3] ^= chainv[2];
            chainv[2] &= chainv[1];
            chainv[1] ^= chainv[0];
            chainv[0] = tmp; tmp = chainv[5];
            chainv[5] |= chainv[6];
            chainv[7] ^= chainv[4];
            chainv[6] = ~chainv[6];
            chainv[5] ^= chainv[4];
            chainv[4] &= tmp;
            chainv[6] ^= chainv[4];
            chainv[4] ^= chainv[7];
            chainv[7] &= chainv[5];
            chainv[5] = ~chainv[5];
            chainv[7] ^= chainv[6];
            chainv[6] |= chainv[4]; tmp ^= chainv[6];
            chainv[4] ^= chainv[7];
            chainv[7] &= chainv[6];
            chainv[6] ^= chainv[5];
            chainv[5] = tmp;
            chainv[4] ^= chainv[0];
            chainv[0] = (chainv[0] << 2) | (chainv[0] >> 30);
            chainv[0] ^= chainv[4];
            chainv[4] = (chainv[4] << 14) | (chainv[4] >> 18);
            chainv[4] ^= chainv[0];
            chainv[0] = (chainv[0] << 10) | (chainv[0] >> 22);
            chainv[0] ^= chainv[4];
            chainv[4] = (chainv[4] << 1) | (chainv[4] >> 31);
            chainv[5] ^= chainv[1];
            chainv[1] = (chainv[1] << 2) | (chainv[1] >> 30);
            chainv[1] ^= chainv[5];
            chainv[5] = (chainv[5] << 14) | (chainv[5] >> 18);
            chainv[5] ^= chainv[1];
            chainv[1] = (chainv[1] << 10) | (chainv[1] >> 22);
            chainv[1] ^= chainv[5];
            chainv[5] = (chainv[5] << 1) | (chainv[5] >> 31);
            chainv[6] ^= chainv[2];
            chainv[2] = (chainv[2] << 2) | (chainv[2] >> 30);
            chainv[2] ^= chainv[6];
            chainv[6] = (chainv[6] << 14) | (chainv[6] >> 18);
            chainv[6] ^= chainv[2];
            chainv[2] = (chainv[2] << 10) | (chainv[2] >> 22);
            chainv[2] ^= chainv[6];
            chainv[6] = (chainv[6] << 1) | (chainv[6] >> 31);
            chainv[7] ^= chainv[3];
            chainv[3] = (chainv[3] << 2) | (chainv[3] >> 30);
            chainv[3] ^= chainv[7];
            chainv[7] = (chainv[7] << 14) | (chainv[7] >> 18);
            chainv[7] ^= chainv[3];
            chainv[3] = (chainv[3] << 10) | (chainv[3] >> 22);
            chainv[3] ^= chainv[7];
            chainv[7] = (chainv[7] << 1) | (chainv[7] >> 31);
            chainv[0] ^= s_CNS[(2*i)+48];
            chainv[4] ^= s_CNS[(2*i)+48+1];
        }

        for(int i=0;i<8;i++) 
            m_state[i+24] = chainv[i];
    }

    void Luffa512::TransformBlock(byte* a_data, int a_index)
    {
        uint chainv[8];
        uint tmp;

        uint data[8];
        memcpy(data, &a_data[a_index], BlockSize);
        ByteSwap(data, 8);

        uint t[40];
        memset(t, 0, 160);

        for(int i=0;i<8;i++) 
        {
            for(int j=0;j<5;j++) 
                t[i] ^= m_state[i+8*j];
        }

        tmp = t[7];
        t[7] = t[6];
        t[6] = t[5];
        t[5] = t[4];
        t[4] = t[3] ^ tmp;
        t[3] = t[2] ^ tmp;
        t[2] = t[1];
        t[1] = t[0] ^ tmp;
        t[0] = tmp;

        for(int j=0;j<5;j++) 
        {
            for(int i=0;i<8;i++) 
                m_state[i+8*j] ^= t[i];
        }

        for(int j=0;j<5;j++) 
        {
            for(int i=0;i<8;i++) 
                t[i+8*j] = m_state[i+8*j];
        }

        for(int j=0;j<5;j++) 
        {
            tmp = m_state[7 + (8*j)];
            m_state[7 + (8*j)] = m_state[6 + (8*j)];
            m_state[6 + (8*j)] = m_state[5 + (8*j)];
            m_state[5 + (8*j)] = m_state[4 + (8*j)];
            m_state[4 + (8*j)] = m_state[3 + (8*j)] ^ tmp;
            m_state[3 + (8*j)] = m_state[2 + (8*j)] ^ tmp;
            m_state[2 + (8*j)] = m_state[1 + (8*j)];
            m_state[1 + (8*j)] = m_state[0 + (8*j)] ^ tmp;
            m_state[8*j] = tmp;
        }

        for(int j=0;j<5;j++) 
        {
            for(int i=0;i<8;i++) 
                m_state[8*j+i] ^= t[8*((j+1)%5)+i];
        }

        for(int j=0;j<5;j++) 
        {
            for(int i=0;i<8;i++) 
                t[i+8*j] = m_state[i+8*j];
        }

        for(int j=0;j<5;j++) 
        {
            tmp = m_state[7 + (8*j)];
            m_state[7 + (8*j)] = m_state[6 + (8*j)];
            m_state[6 + (8*j)] = m_state[5 + (8*j)];
            m_state[5 + (8*j)] = m_state[4 + (8*j)];
            m_state[4 + (8*j)] = m_state[3 + (8*j)] ^ tmp;
            m_state[3 + (8*j)] = m_state[2 + (8*j)] ^ tmp;
            m_state[2 + (8*j)] = m_state[1 + (8*j)];
            m_state[1 + (8*j)] = m_state[0 + (8*j)] ^ tmp;
            m_state[8*j] = tmp;
        }

        for(int j=0;j<5;j++) 
        {
            for(int i=0;i<8;i++) 
                m_state[8*j+i] ^= t[8*((j+4)%5)+i];
        }

        for(int j=0;j<5;j++) 
        {
            for(int i=0;i<8;i++) 
                m_state[i+8*j] ^= data[i];

            tmp = data[7];
            data[7] = data[6];
            data[6] = data[5];
            data[5] = data[4];
            data[4] = data[3] ^ tmp;
            data[3] = data[2] ^ tmp;
            data[2] = data[1];
            data[1] = data[0] ^ tmp;
            data[0] = tmp;
        }

        for(int i=0;i<8;i++) 
            chainv[i] = m_state[i];

        for(int i=0;i<8;i++) 
        {
            tmp = chainv[0];
            chainv[0] |= chainv[1];
            chainv[2] ^= chainv[3];
            chainv[1] = ~chainv[1];
            chainv[0] ^= chainv[3];
            chainv[3] &= tmp;
            chainv[1] ^= chainv[3];
            chainv[3] ^= chainv[2];
            chainv[2] &= chainv[0];
            chainv[0] = ~chainv[0];
            chainv[2] ^= chainv[1];
            chainv[1] |= chainv[3]; tmp ^= chainv[1];
            chainv[3] ^= chainv[2];
            chainv[2] &= chainv[1];
            chainv[1] ^= chainv[0];
            chainv[0] = tmp; tmp = chainv[5];
            chainv[5] |= chainv[6];
            chainv[7] ^= chainv[4];
            chainv[6] = ~chainv[6];
            chainv[5] ^= chainv[4];
            chainv[4] &= tmp;
            chainv[6] ^= chainv[4];
            chainv[4] ^= chainv[7];
            chainv[7] &= chainv[5];
            chainv[5] = ~chainv[5];
            chainv[7] ^= chainv[6];
            chainv[6] |= chainv[4]; tmp ^= chainv[6];
            chainv[4] ^= chainv[7];
            chainv[7] &= chainv[6];
            chainv[6] ^= chainv[5];
            chainv[5] = tmp;
            chainv[4] ^= chainv[0];
            chainv[0] = (chainv[0] << 2) | (chainv[0] >> 30);
            chainv[0] ^= chainv[4];
            chainv[4] = (chainv[4] << 14) | (chainv[4] >> 18);
            chainv[4] ^= chainv[0];
            chainv[0] = (chainv[0] << 10) | (chainv[0] >> 22);
            chainv[0] ^= chainv[4];
            chainv[4] = (chainv[4] << 1) | (chainv[4] >> 31);
            chainv[5] ^= chainv[1];
            chainv[1] = (chainv[1] << 2) | (chainv[1] >> 30);
            chainv[1] ^= chainv[5];
            chainv[5] = (chainv[5] << 14) | (chainv[5] >> 18);
            chainv[5] ^= chainv[1];
            chainv[1] = (chainv[1] << 10) | (chainv[1] >> 22);
            chainv[1] ^= chainv[5];
            chainv[5] = (chainv[5] << 1) | (chainv[5] >> 31);
            chainv[6] ^= chainv[2];
            chainv[2] = (chainv[2] << 2) | (chainv[2] >> 30);
            chainv[2] ^= chainv[6];
            chainv[6] = (chainv[6] << 14) | (chainv[6] >> 18);
            chainv[6] ^= chainv[2];
            chainv[2] = (chainv[2] << 10) | (chainv[2] >> 22);
            chainv[2] ^= chainv[6];
            chainv[6] = (chainv[6] << 1) | (chainv[6] >> 31);
            chainv[7] ^= chainv[3];
            chainv[3] = (chainv[3] << 2) | (chainv[3] >> 30);
            chainv[3] ^= chainv[7];
            chainv[7] = (chainv[7] << 14) | (chainv[7] >> 18);
            chainv[7] ^= chainv[3];
            chainv[3] = (chainv[3] << 10) | (chainv[3] >> 22);
            chainv[3] ^= chainv[7];
            chainv[7] = (chainv[7] << 1) | (chainv[7] >> 31);
            chainv[0] ^= s_CNS[(2*i)];
            chainv[4] ^= s_CNS[(2*i)+1];
        }

        for(int i=0;i<8;i++) 
        {
            m_state[i] = chainv[i];
            chainv[i] = m_state[i+8];
        }

        chainv[4] = (chainv[4] << 1) | (chainv[4] >> 31);
        chainv[5] = (chainv[5] << 1) | (chainv[5] >> 31);
        chainv[6] = (chainv[6] << 1) | (chainv[6] >> 31);
        chainv[7] = (chainv[7] << 1) | (chainv[7] >> 31);

        for(int i=0;i<8;i++) 
        {
            tmp = chainv[0];
            chainv[0] |= chainv[1];
            chainv[2] ^= chainv[3];
            chainv[1] = ~chainv[1];
            chainv[0] ^= chainv[3];
            chainv[3] &= tmp;
            chainv[1] ^= chainv[3];
            chainv[3] ^= chainv[2];
            chainv[2] &= chainv[0];
            chainv[0] = ~chainv[0];
            chainv[2] ^= chainv[1];
            chainv[1] |= chainv[3]; tmp ^= chainv[1];
            chainv[3] ^= chainv[2];
            chainv[2] &= chainv[1];
            chainv[1] ^= chainv[0];
            chainv[0] = tmp; tmp = chainv[5];
            chainv[5] |= chainv[6];
            chainv[7] ^= chainv[4];
            chainv[6] = ~chainv[6];
            chainv[5] ^= chainv[4];
            chainv[4] &= tmp;
            chainv[6] ^= chainv[4];
            chainv[4] ^= chainv[7];
            chainv[7] &= chainv[5];
            chainv[5] = ~chainv[5];
            chainv[7] ^= chainv[6];
            chainv[6] |= chainv[4]; tmp ^= chainv[6];
            chainv[4] ^= chainv[7];
            chainv[7] &= chainv[6];
            chainv[6] ^= chainv[5];
            chainv[5] = tmp;
            chainv[4] ^= chainv[0];
            chainv[0] = (chainv[0] << 2) | (chainv[0] >> 30);
            chainv[0] ^= chainv[4];
            chainv[4] = (chainv[4] << 14) | (chainv[4] >> 18);
            chainv[4] ^= chainv[0];
            chainv[0] = (chainv[0] << 10) | (chainv[0] >> 22);
            chainv[0] ^= chainv[4];
            chainv[4] = (chainv[4] << 1) | (chainv[4] >> 31);
            chainv[5] ^= chainv[1];
            chainv[1] = (chainv[1] << 2) | (chainv[1] >> 30);
            chainv[1] ^= chainv[5];
            chainv[5] = (chainv[5] << 14) | (chainv[5] >> 18);
            chainv[5] ^= chainv[1];
            chainv[1] = (chainv[1] << 10) | (chainv[1] >> 22);
            chainv[1] ^= chainv[5];
            chainv[5] = (chainv[5] << 1) | (chainv[5] >> 31);
            chainv[6] ^= chainv[2];
            chainv[2] = (chainv[2] << 2) | (chainv[2] >> 30);
            chainv[2] ^= chainv[6];
            chainv[6] = (chainv[6] << 14) | (chainv[6] >> 18);
            chainv[6] ^= chainv[2];
            chainv[2] = (chainv[2] << 10) | (chainv[2] >> 22);
            chainv[2] ^= chainv[6];
            chainv[6] = (chainv[6] << 1) | (chainv[6] >> 31);
            chainv[7] ^= chainv[3];
            chainv[3] = (chainv[3] << 2) | (chainv[3] >> 30);
            chainv[3] ^= chainv[7];
            chainv[7] = (chainv[7] << 14) | (chainv[7] >> 18);
            chainv[7] ^= chainv[3];
            chainv[3] = (chainv[3] << 10) | (chainv[3] >> 22);
            chainv[3] ^= chainv[7];
            chainv[7] = (chainv[7] << 1) | (chainv[7] >> 31);
            chainv[0] ^= s_CNS[(2*i)+16];
            chainv[4] ^= s_CNS[(2*i)+16+1];
        }

        for(int i=0;i<8;i++) 
        {
            m_state[i+8] = chainv[i];
            chainv[i] = m_state[i+16];
        }

        chainv[4] = (chainv[4] << 2) | (chainv[4] >> 30);
        chainv[5] = (chainv[5] << 2) | (chainv[5] >> 30);
        chainv[6] = (chainv[6] << 2) | (chainv[6] >> 30);
        chainv[7] = (chainv[7] << 2) | (chainv[7] >> 30);

        for(int i=0;i<8;i++) 
        {
            tmp = chainv[0];
            chainv[0] |= chainv[1];
            chainv[2] ^= chainv[3];
            chainv[1] = ~chainv[1];
            chainv[0] ^= chainv[3];
            chainv[3] &= tmp;
            chainv[1] ^= chainv[3];
            chainv[3] ^= chainv[2];
            chainv[2] &= chainv[0];
            chainv[0] = ~chainv[0];
            chainv[2] ^= chainv[1];
            chainv[1] |= chainv[3]; tmp ^= chainv[1];
            chainv[3] ^= chainv[2];
            chainv[2] &= chainv[1];
            chainv[1] ^= chainv[0];
            chainv[0] = tmp; tmp = chainv[5];
            chainv[5] |= chainv[6];
            chainv[7] ^= chainv[4];
            chainv[6] = ~chainv[6];
            chainv[5] ^= chainv[4];
            chainv[4] &= tmp;
            chainv[6] ^= chainv[4];
            chainv[4] ^= chainv[7];
            chainv[7] &= chainv[5];
            chainv[5] = ~chainv[5];
            chainv[7] ^= chainv[6];
            chainv[6] |= chainv[4]; tmp ^= chainv[6];
            chainv[4] ^= chainv[7];
            chainv[7] &= chainv[6];
            chainv[6] ^= chainv[5];
            chainv[5] = tmp;
            chainv[4] ^= chainv[0];
            chainv[0] = (chainv[0] << 2) | (chainv[0] >> 30);
            chainv[0] ^= chainv[4];
            chainv[4] = (chainv[4] << 14) | (chainv[4] >> 18);
            chainv[4] ^= chainv[0];
            chainv[0] = (chainv[0] << 10) | (chainv[0] >> 22);
            chainv[0] ^= chainv[4];
            chainv[4] = (chainv[4] << 1) | (chainv[4] >> 31);
            chainv[5] ^= chainv[1];
            chainv[1] = (chainv[1] << 2) | (chainv[1] >> 30);
            chainv[1] ^= chainv[5];
            chainv[5] = (chainv[5] << 14) | (chainv[5] >> 18);
            chainv[5] ^= chainv[1];
            chainv[1] = (chainv[1] << 10) | (chainv[1] >> 22);
            chainv[1] ^= chainv[5];
            chainv[5] = (chainv[5] << 1) | (chainv[5] >> 31);
            chainv[6] ^= chainv[2];
            chainv[2] = (chainv[2] << 2) | (chainv[2] >> 30);
            chainv[2] ^= chainv[6];
            chainv[6] = (chainv[6] << 14) | (chainv[6] >> 18);
            chainv[6] ^= chainv[2];
            chainv[2] = (chainv[2] << 10) | (chainv[2] >> 22);
            chainv[2] ^= chainv[6];
            chainv[6] = (chainv[6] << 1) | (chainv[6] >> 31);
            chainv[7] ^= chainv[3];
            chainv[3] = (chainv[3] << 2) | (chainv[3] >> 30);
            chainv[3] ^= chainv[7];
            chainv[7] = (chainv[7] << 14) | (chainv[7] >> 18);
            chainv[7] ^= chainv[3];
            chainv[3] = (chainv[3] << 10) | (chainv[3] >> 22);
            chainv[3] ^= chainv[7];
            chainv[7] = (chainv[7] << 1) | (chainv[7] >> 31);
            chainv[0] ^= s_CNS[(2*i)+32];
            chainv[4] ^= s_CNS[(2*i)+32+1];
        }

        for(int i=0;i<8;i++) 
        {
            m_state[i+16] = chainv[i];
            chainv[i] = m_state[i+24];
        }

        chainv[4] = (chainv[4] << 3) | (chainv[4] >> 29);
        chainv[5] = (chainv[5] << 3) | (chainv[5] >> 29);
        chainv[6] = (chainv[6] << 3) | (chainv[6] >> 29);
        chainv[7] = (chainv[7] << 3) | (chainv[7] >> 29);

        for(int i=0;i<8;i++) 
        {
            tmp = chainv[0];
            chainv[0] |= chainv[1];
            chainv[2] ^= chainv[3];
            chainv[1] = ~chainv[1];
            chainv[0] ^= chainv[3];
            chainv[3] &= tmp;
            chainv[1] ^= chainv[3];
            chainv[3] ^= chainv[2];
            chainv[2] &= chainv[0];
            chainv[0] = ~chainv[0];
            chainv[2] ^= chainv[1];
            chainv[1] |= chainv[3]; tmp ^= chainv[1];
            chainv[3] ^= chainv[2];
            chainv[2] &= chainv[1];
            chainv[1] ^= chainv[0];
            chainv[0] = tmp; tmp = chainv[5];
            chainv[5] |= chainv[6];
            chainv[7] ^= chainv[4];
            chainv[6] = ~chainv[6];
            chainv[5] ^= chainv[4];
            chainv[4] &= tmp;
            chainv[6] ^= chainv[4];
            chainv[4] ^= chainv[7];
            chainv[7] &= chainv[5];
            chainv[5] = ~chainv[5];
            chainv[7] ^= chainv[6];
            chainv[6] |= chainv[4]; tmp ^= chainv[6];
            chainv[4] ^= chainv[7];
            chainv[7] &= chainv[6];
            chainv[6] ^= chainv[5];
            chainv[5] = tmp;
            chainv[4] ^= chainv[0];
            chainv[0] = (chainv[0] << 2) | (chainv[0] >> 30);
            chainv[0] ^= chainv[4];
            chainv[4] = (chainv[4] << 14) | (chainv[4] >> 18);
            chainv[4] ^= chainv[0];
            chainv[0] = (chainv[0] << 10) | (chainv[0] >> 22);
            chainv[0] ^= chainv[4];
            chainv[4] = (chainv[4] << 1) | (chainv[4] >> 31);
            chainv[5] ^= chainv[1];
            chainv[1] = (chainv[1] << 2) | (chainv[1] >> 30);
            chainv[1] ^= chainv[5];
            chainv[5] = (chainv[5] << 14) | (chainv[5] >> 18);
            chainv[5] ^= chainv[1];
            chainv[1] = (chainv[1] << 10) | (chainv[1] >> 22);
            chainv[1] ^= chainv[5];
            chainv[5] = (chainv[5] << 1) | (chainv[5] >> 31);
            chainv[6] ^= chainv[2];
            chainv[2] = (chainv[2] << 2) | (chainv[2] >> 30);
            chainv[2] ^= chainv[6];
            chainv[6] = (chainv[6] << 14) | (chainv[6] >> 18);
            chainv[6] ^= chainv[2];
            chainv[2] = (chainv[2] << 10) | (chainv[2] >> 22);
            chainv[2] ^= chainv[6];
            chainv[6] = (chainv[6] << 1) | (chainv[6] >> 31);
            chainv[7] ^= chainv[3];
            chainv[3] = (chainv[3] << 2) | (chainv[3] >> 30);
            chainv[3] ^= chainv[7];
            chainv[7] = (chainv[7] << 14) | (chainv[7] >> 18);
            chainv[7] ^= chainv[3];
            chainv[3] = (chainv[3] << 10) | (chainv[3] >> 22);
            chainv[3] ^= chainv[7];
            chainv[7] = (chainv[7] << 1) | (chainv[7] >> 31);
            chainv[0] ^= s_CNS[(2*i)+48];
            chainv[4] ^= s_CNS[(2*i)+48+1];
        }

        for(int i=0;i<8;i++) 
        {
            m_state[i+24] = chainv[i];
            chainv[i] = m_state[i+32];
        }

        chainv[4] = (chainv[4] << 4) | (chainv[4] >> 28);
        chainv[5] = (chainv[5] << 4) | (chainv[5] >> 28);
        chainv[6] = (chainv[6] << 4) | (chainv[6] >> 28);
        chainv[7] = (chainv[7] << 4) | (chainv[7] >> 28);

        for(int i=0;i<8;i++)
        {
            tmp = chainv[0];
            chainv[0] |= chainv[1];
            chainv[2] ^= chainv[3];
            chainv[1] = ~chainv[1];
            chainv[0] ^= chainv[3];
            chainv[3] &= tmp;
            chainv[1] ^= chainv[3];
            chainv[3] ^= chainv[2];
            chainv[2] &= chainv[0];
            chainv[0] = ~chainv[0];
            chainv[2] ^= chainv[1];
            chainv[1] |= chainv[3]; tmp ^= chainv[1];
            chainv[3] ^= chainv[2];
            chainv[2] &= chainv[1];
            chainv[1] ^= chainv[0];
            chainv[0] = tmp; tmp = chainv[5];
            chainv[5] |= chainv[6];
            chainv[7] ^= chainv[4];
            chainv[6] = ~chainv[6];
            chainv[5] ^= chainv[4];
            chainv[4] &= tmp;
            chainv[6] ^= chainv[4];
            chainv[4] ^= chainv[7];
            chainv[7] &= chainv[5];
            chainv[5] = ~chainv[5];
            chainv[7] ^= chainv[6];
            chainv[6] |= chainv[4]; tmp ^= chainv[6];
            chainv[4] ^= chainv[7];
            chainv[7] &= chainv[6];
            chainv[6] ^= chainv[5];
            chainv[5] = tmp;
            chainv[4] ^= chainv[0];
            chainv[0] = (chainv[0] << 2) | (chainv[0] >> 30);
            chainv[0] ^= chainv[4];
            chainv[4] = (chainv[4] << 14) | (chainv[4] >> 18);
            chainv[4] ^= chainv[0];
            chainv[0] = (chainv[0] << 10) | (chainv[0] >> 22);
            chainv[0] ^= chainv[4];
            chainv[4] = (chainv[4] << 1) | (chainv[4] >> 31);
            chainv[5] ^= chainv[1];
            chainv[1] = (chainv[1] << 2) | (chainv[1] >> 30);
            chainv[1] ^= chainv[5];
            chainv[5] = (chainv[5] << 14) | (chainv[5] >> 18);
            chainv[5] ^= chainv[1];
            chainv[1] = (chainv[1] << 10) | (chainv[1] >> 22);
            chainv[1] ^= chainv[5];
            chainv[5] = (chainv[5] << 1) | (chainv[5] >> 31);
            chainv[6] ^= chainv[2];
            chainv[2] = (chainv[2] << 2) | (chainv[2] >> 30);
            chainv[2] ^= chainv[6];
            chainv[6] = (chainv[6] << 14) | (chainv[6] >> 18);
            chainv[6] ^= chainv[2];
            chainv[2] = (chainv[2] << 10) | (chainv[2] >> 22);
            chainv[2] ^= chainv[6];
            chainv[6] = (chainv[6] << 1) | (chainv[6] >> 31);
            chainv[7] ^= chainv[3];
            chainv[3] = (chainv[3] << 2) | (chainv[3] >> 30);
            chainv[3] ^= chainv[7];
            chainv[7] = (chainv[7] << 14) | (chainv[7] >> 18);
            chainv[7] ^= chainv[3];
            chainv[3] = (chainv[3] << 10) | (chainv[3] >> 22);
            chainv[3] ^= chainv[7];
            chainv[7] = (chainv[7] << 1) | (chainv[7] >> 31);
            chainv[0] ^= s_CNS[(2*i)+64];
            chainv[4] ^= s_CNS[(2*i)+64+1];
        }

        for(int i=0;i<8;i++)
            m_state[i+32] = chainv[i];
    }

    byte* LuffaBase::GetResult()
    {
        memset(m_buffer, 0, BlockSize);

        uint* result = new uint[HashSize/4];
        memset(result, 0, HashSize);

        for (int i=0; i<HashSize/4; i++)
        {
            if (i % 8 == 0)
                TransformBlock(m_buffer, 0);

            for(int j=0;j<m_result_blocks;j++) 
                result[i] ^= m_state[(i%8)+8*j];
        }

        ByteSwap(result, HashSize/4);

        return (byte*)result;
    }

    void LuffaBase::Finish()
    {
        byte* pad = new byte[BlockSize];
        memset(pad, 0, BlockSize);
        pad[0] = 0x80;
        TransformBytes(pad, 0, BlockSize - m_buffer_pos);
        delete pad;
    }

    LuffaBase::LuffaBase(int a_hashSize)
    {
        HashSize = a_hashSize;
        BlockSize = 32;

        m_buffer = new byte[BlockSize];
        m_state = new uint[40];
    }

    LuffaBase::~LuffaBase()
    {
        delete m_buffer;
        delete m_state;
    }
}