
#pragma once

namespace SipHash
{
    class SipHash
    {
        private:

            ulong m_v0;
            ulong m_v1;
            ulong m_v2;
            ulong m_v3;
            ulong m_key0;
            ulong m_key1;
            byte m_buffer[8];
            ulong m_processed_bytes;

            void TransformBlock(byte* a_data, int a_index)
            {
                a_data += a_index;

                ulong m = *((ulong*)a_data);

                m_v3 ^= m;

                m_v0 += m_v1;
                m_v1 = (m_v1 << 13) | (m_v1 >> (64 - 13));
                m_v1 ^= m_v0;
                m_v0 = (m_v0 << 32) | (m_v0 >> (64 - 32));
                m_v2 += m_v3;
                m_v3 = (m_v3 << 16) | (m_v3 >> (64 - 16));
                m_v3 ^= m_v2;
                m_v0 += m_v3;
                m_v3 = (m_v3 << 21) | (m_v3 >> (64 - 21));
                m_v3 ^= m_v0;
                m_v2 += m_v1;
                m_v1 = (m_v1 << 17) | (m_v1 >> (64 - 17));
                m_v1 ^= m_v2;
                m_v2 = (m_v2 << 32) | (m_v2 >> (64 - 32));

                m_v0 += m_v1;
                m_v1 = (m_v1 << 13) | (m_v1 >> (64 - 13));
                m_v1 ^= m_v0;
                m_v0 = (m_v0 << 32) | (m_v0 >> (64 - 32));
                m_v2 += m_v3;
                m_v3 = (m_v3 << 16) | (m_v3 >> (64 - 16));
                m_v3 ^= m_v2;
                m_v0 += m_v3;
                m_v3 = (m_v3 << 21) | (m_v3 >> (64 - 21));
                m_v3 ^= m_v0;
                m_v2 += m_v1;
                m_v1 = (m_v1 << 17) | (m_v1 >> (64 - 17));
                m_v1 ^= m_v2;
                m_v2 = (m_v2 << 32) | (m_v2 >> (64 - 32));

                m_v0 ^= m;
            }

            byte* GetResult()
            {
                ulong b = m_v0 ^ m_v1 ^ m_v2  ^ m_v3;

                byte* result = new byte[8];
                
                result[0] = (byte)b;
                result[1] = (byte)(b >> 8);
                result[2] = (byte)(b >> 16);
                result[3] = (byte)(b >> 24);
                result[4] = (byte)(b >> 32);
                result[5] = (byte)(b >> 40);
                result[6] = (byte)(b >> 48);
                result[7] = (byte)(b >> 56);

                return result;
            }

            void Finish()
            {
                int left = m_processed_bytes % 8;
                ulong b = m_processed_bytes << 56;
              
                switch (left)
                {
                    case 7:
                    {
                        b |= ((ulong)m_buffer[6]) << 48;
                        b |= ((ulong)m_buffer[5]) << 40;
                        b |= ((ulong)m_buffer[4]) << 32;
                        b |= ((ulong)m_buffer[3]) << 24;
                        b |= ((ulong)m_buffer[2]) << 16;
                        b |= ((ulong)m_buffer[1]) << 8;
                        b |= ((ulong)m_buffer[0]);
                        break;
                    }
                    case 6:
                    {
                        b |= ((ulong)m_buffer[5]) << 40;
                        b |= ((ulong)m_buffer[4]) << 32;
                        b |= ((ulong)m_buffer[3]) << 24;
                        b |= ((ulong)m_buffer[2]) << 16;
                        b |= ((ulong)m_buffer[1]) << 8;
                        b |= ((ulong)m_buffer[0]);
                        break;
                    }
                    case 5:
                    {
                        b |= ((ulong)m_buffer[4]) << 32;
                        b |= ((ulong)m_buffer[3]) << 24;
                        b |= ((ulong)m_buffer[2]) << 16;
                        b |= ((ulong)m_buffer[1]) << 8;
                        b |= ((ulong)m_buffer[0]);
                        break;
                    }
                    case 4:
                    {
                        b |= ((ulong)m_buffer[3]) << 24;
                        b |= ((ulong)m_buffer[2]) << 16;
                        b |= ((ulong)m_buffer[1]) << 8;
                        b |= ((ulong)m_buffer[0]);
                        break;
                    }
                    case 3:
                    {
                        b |= ((ulong)m_buffer[2]) << 16;
                        b |= ((ulong)m_buffer[1]) << 8;
                        b |= ((ulong)m_buffer[0]);
                        break;
                    }
                    case 2:
                    {
                        b |= ((ulong)m_buffer[1]) << 8;
                        b |= ((ulong)m_buffer[0]);
                        break;
                    }
                    case 1:
                    {
                        b |= ((ulong)m_buffer[0]);
                        break;
                    }
                }

                m_v3 ^= b;

                m_v0 += m_v1;
                m_v1 = (m_v1 << 13) | (m_v1 >> (64 - 13));
                m_v1 ^= m_v0;
                m_v0 = (m_v0 << 32) | (m_v0 >> (64 - 32));
                m_v2 += m_v3;
                m_v3 = (m_v3 << 16) | (m_v3 >> (64 - 16));
                m_v3 ^= m_v2;
                m_v0 += m_v3;
                m_v3 = (m_v3 << 21) | (m_v3 >> (64 - 21));
                m_v3 ^= m_v0;
                m_v2 += m_v1;
                m_v1 = (m_v1 << 17) | (m_v1 >> (64 - 17));
                m_v1 ^= m_v2;
                m_v2 = (m_v2 << 32) | (m_v2 >> (64 - 32));

                m_v0 += m_v1;
                m_v1 = (m_v1 << 13) | (m_v1 >> (64 - 13));
                m_v1 ^= m_v0;
                m_v0 = (m_v0 << 32) | (m_v0 >> (64 - 32));
                m_v2 += m_v3;
                m_v3 = (m_v3 << 16) | (m_v3 >> (64 - 16));
                m_v3 ^= m_v2;
                m_v0 += m_v3;
                m_v3 = (m_v3 << 21) | (m_v3 >> (64 - 21));
                m_v3 ^= m_v0;
                m_v2 += m_v1;
                m_v1 = (m_v1 << 17) | (m_v1 >> (64 - 17));
                m_v1 ^= m_v2;
                m_v2 = (m_v2 << 32) | (m_v2 >> (64 - 32));

                m_v0 ^= b;
                m_v2 ^= 0xff;

                m_v0 += m_v1;
                m_v1 = (m_v1 << 13) | (m_v1 >> (64 - 13));
                m_v1 ^= m_v0;
                m_v0 = (m_v0 << 32) | (m_v0 >> (64 - 32));
                m_v2 += m_v3;
                m_v3 = (m_v3 << 16) | (m_v3 >> (64 - 16));
                m_v3 ^= m_v2;
                m_v0 += m_v3;
                m_v3 = (m_v3 << 21) | (m_v3 >> (64 - 21));
                m_v3 ^= m_v0;
                m_v2 += m_v1;
                m_v1 = (m_v1 << 17) | (m_v1 >> (64 - 17));
                m_v1 ^= m_v2;
                m_v2 = (m_v2 << 32) | (m_v2 >> (64 - 32));

                m_v0 += m_v1;
                m_v1 = (m_v1 << 13) | (m_v1 >> (64 - 13));
                m_v1 ^= m_v0;
                m_v0 = (m_v0 << 32) | (m_v0 >> (64 - 32));
                m_v2 += m_v3;
                m_v3 = (m_v3 << 16) | (m_v3 >> (64 - 16));
                m_v3 ^= m_v2;
                m_v0 += m_v3;
                m_v3 = (m_v3 << 21) | (m_v3 >> (64 - 21));
                m_v3 ^= m_v0;
                m_v2 += m_v1;
                m_v1 = (m_v1 << 17) | (m_v1 >> (64 - 17));
                m_v1 ^= m_v2;
                m_v2 = (m_v2 << 32) | (m_v2 >> (64 - 32));

                m_v0 += m_v1;
                m_v1 = (m_v1 << 13) | (m_v1 >> (64 - 13));
                m_v1 ^= m_v0;
                m_v0 = (m_v0 << 32) | (m_v0 >> (64 - 32));
                m_v2 += m_v3;
                m_v3 = (m_v3 << 16) | (m_v3 >> (64 - 16));
                m_v3 ^= m_v2;
                m_v0 += m_v3;
                m_v3 = (m_v3 << 21) | (m_v3 >> (64 - 21));
                m_v3 ^= m_v0;
                m_v2 += m_v1;
                m_v1 = (m_v1 << 17) | (m_v1 >> (64 - 17));
                m_v1 ^= m_v2;
                m_v2 = (m_v2 << 32) | (m_v2 >> (64 - 32));

                m_v0 += m_v1;
                m_v1 = (m_v1 << 13) | (m_v1 >> (64 - 13));
                m_v1 ^= m_v0;
                m_v0 = (m_v0 << 32) | (m_v0 >> (64 - 32));
                m_v2 += m_v3;
                m_v3 = (m_v3 << 16) | (m_v3 >> (64 - 16));
                m_v3 ^= m_v2;
                m_v0 += m_v3;
                m_v3 = (m_v3 << 21) | (m_v3 >> (64 - 21));
                m_v3 ^= m_v0;
                m_v2 += m_v1;
                m_v1 = (m_v1 << 17) | (m_v1 >> (64 - 17));
                m_v1 ^= m_v2;
                m_v2 = (m_v2 << 32) | (m_v2 >> (64 - 32));
            }

        public:

            int BlockSize = 8;
            int HashSize = 8;
            int KeyLength = 16;

            SipHash()
            {
                byte key[16];
                for (int j = 0; j < 16; j++)
                    key[j] = j;
                SetKey(key);

                Initialize();
            }

            void Initialize()
            {
                m_v0 = 0x736f6d6570736575;
                m_v1 = 0x646f72616e646f6d;
                m_v2 = 0x6c7967656e657261;
                m_v3 = 0x7465646279746573;

                m_v3 ^= m_key1;
                m_v2 ^= m_key0;
                m_v1 ^= m_key1;
                m_v0 ^= m_key0;

                memset(m_buffer, 0, 8);
                m_processed_bytes = 0;
            }

            void TransformBytes(byte* a_data, int a_index, int a_length)
            {
                int last = a_length + a_index - 1;

                if (last < 0)
                    return;

                int i = m_processed_bytes % BlockSize;
                int j = a_index;
                while (i < BlockSize && j <= last)
                {
                    m_buffer[i++] = a_data[j++];
                    m_processed_bytes++;
                }

                if (i < BlockSize)
                    return;

                TransformBlock(m_buffer, 0);
                memset(m_buffer, 0, BlockSize);

                while ((j + BlockSize - 1) <= last)
                {
                    TransformBlock(a_data, j);
                    m_processed_bytes += BlockSize;
                    j += BlockSize;
                }

                i = m_processed_bytes % BlockSize;
                while (j <= last)
                {
                    m_buffer[i++] = a_data[j++];
                    m_processed_bytes++;
                }
            }

            byte* TransformFinal()
            {
                Finish();
                byte* result = GetResult();
                Initialize();
                return result;
            }

            byte* ComputeBytes(byte* a_data, int a_length)
            {
                Initialize();
                TransformBytes(a_data, 0, a_length);
                return TransformFinal();
            }

            void SetKey(byte* a_key)
            {
                m_key0 =
                    (ulong)(a_key[0]) |
                    ((ulong)(a_key[1]) << 8) |
                    ((ulong)(a_key[2]) << 16) |
                    ((ulong)(a_key[3]) << 24) |
                    ((ulong)(a_key[4]) << 32) |
                    ((ulong)(a_key[5]) << 40) |
                    ((ulong)(a_key[6]) << 48) |
                    ((ulong)(a_key[7]) << 56);

                m_key1 =
                    (ulong)(a_key[8]) |
                    ((ulong)(a_key[9]) << 8) |
                    ((ulong)(a_key[10]) << 16) |
                    ((ulong)(a_key[11]) << 24) |
                    ((ulong)(a_key[12]) << 32) |
                    ((ulong)(a_key[13]) << 40) |
                    ((ulong)(a_key[14]) << 48) |
                    ((ulong)(a_key[15]) << 56);

                Initialize();
            }
    };
}