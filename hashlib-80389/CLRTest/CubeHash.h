
#pragma once

namespace CubeHash
{
    ref class CubeHash
    {
        private:

            int m_buffer_pos;
            int m_hashSize;
            static const int ROUNDS = 16;
            int m_blockSize;
            array<uint>^ m_buffer;

        protected:

            void TransformBlock()
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

            array<byte>^ GetResult()
            {
                array<byte>^ result = gcnew array<byte>(m_hashSize);

                for (int i=0; i<m_hashSize; i++) 
                    result[i] = m_buffer[i / 4] >> (8 * (i % 4));

                return result;
            }

            void Finish()
            {
                uint u = (128 >> (m_buffer_pos % 8));
                u <<= 8 * ((m_buffer_pos / 8) % 4);
                m_buffer[m_buffer_pos / 32] ^= u;
                TransformBlock();
                m_buffer[31] ^= 1;
                for (int i=0; i<10; i++) 
                    TransformBlock();  
            }

        public:

            CubeHash(int a_hashSize)
            {
                m_buffer = gcnew array<uint>(32);

                m_blockSize = 32;
                m_hashSize = a_hashSize;

                Initialize();
            }

            void Initialize()
            {
                Trans::memset(m_buffer, 0, 32*4);

                m_buffer[0] = m_hashSize;
                m_buffer[1] = m_blockSize;
                m_buffer[2] = ROUNDS;

                for (int i = 0; i<10; i++) 
                    TransformBlock();

                m_buffer_pos = 0;
            }

            void TransformBytes(array<byte>^ a_data, int a_index, int a_length)
            {
                while (a_length >= 1) 
                {
                    uint u = a_data[a_index];
                    u <<= 8 * ((m_buffer_pos / 8) % 4);
                    m_buffer[m_buffer_pos / 32] ^= u;
                    a_length -= 1;
                    m_buffer_pos += 8;
                    if (m_buffer_pos == 8 * m_blockSize) 
                    {
                        TransformBlock();
                        m_buffer_pos = 0;
                    }
                    a_index++;
                }
            }

            array<byte>^ TransformFinal()
            {
                Finish();
                array<byte>^ result = GetResult();
                Initialize();
                return result;
            }

            array<byte>^ ComputeBytes(array<byte>^ a_data)
            {
                Initialize();
                TransformBytes(a_data, 0, a_data->Length);
                return TransformFinal();
            }
    };
}
