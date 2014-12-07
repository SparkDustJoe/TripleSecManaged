
#include "stdafx.h"
#include "CubeHash.h"

namespace CubeHash
{
    void CubeHash::TransformBlock()
    {
        uint y[16];

        for (int r = 0;r < ROUNDS;++r) 
        {
            for (int i=0; i<16; i++) 
                m_buffer[i + 16] += m_buffer[i];
            for (int i=0; i<16; i++) 
                y[i ^ 8] = m_buffer[i];
            for (int i=0; i<16; i++) 
                m_buffer[i] = y[i] << 7 | y[i] >> 25;
            for (int i=0; i<16; i++) 
                m_buffer[i] ^= m_buffer[i + 16];
            for (int i=0; i<16; i++) 
                y[i ^ 2] = m_buffer[i + 16];
            for (int i=0; i<16; i++) 
                m_buffer[i + 16] = y[i];
            for (int i=0; i<16; i++) 
                m_buffer[i + 16] += m_buffer[i];
            for (int i=0; i<16; i++) 
                y[i ^ 4] = m_buffer[i];
            for (int i=0; i<16; i++) 
                m_buffer[i] = y[i] << 11 | y[i] >> 21;
            for (int i=0; i<16; i++) 
                m_buffer[i] ^= m_buffer[i + 16];
            for (int i=0; i<16; i++) 
                y[i ^ 1] = m_buffer[i + 16];
            for (int i=0; i<16; i++) 
                m_buffer[i + 16] = y[i];
        }
    }

    void CubeHash::Initialize()
    {
        for (int i=0; i<32; i++) 
            m_buffer[i] = 0;

        m_buffer[0] = HashSize;
        m_buffer[1] = BlockSize;
        m_buffer[2] = ROUNDS;

        for (int i = 0; i<10; i++) 
            TransformBlock();

        m_buffer_pos = 0;
    }

    void CubeHash::TransformBytes(byte* a_data, int a_length)
    {
        while (a_length >= 1) 
        {
            uint u = *a_data;
            u <<= 8 * ((m_buffer_pos / 8) % 4);
            m_buffer[m_buffer_pos / 32] ^= u;
            a_data += 1;
            a_length -= 1;
            m_buffer_pos += 8;
            if (m_buffer_pos == 8 * BlockSize) 
            {
                TransformBlock();
                m_buffer_pos = 0;
            }
        }
    }

    byte* CubeHash::TransformFinal()
    {
        Finish();
        byte* result = GetResult();
        Initialize();
        return result;
    }

    void CubeHash::Finish()
    {
        uint u = (128 >> (m_buffer_pos % 8));
        u <<= 8 * ((m_buffer_pos / 8) % 4);
        m_buffer[m_buffer_pos / 32] ^= u;
        TransformBlock();
        m_buffer[31] ^= 1;
        for (int i=0; i<10; i++) 
            TransformBlock();
    }

    byte* CubeHash::GetResult()
    {
        byte* result = new byte[HashSize];

        for (int i=0; i<HashSize; i++) 
            result[i] = m_buffer[i / 4] >> (8 * (i % 4));

        return result;
    }

    byte* CubeHash::ComputeBytes(byte* a_data, int a_length)
    {
        Initialize();
        TransformBytes(a_data, a_length);
        return TransformFinal();
    }
}