
#pragma once

namespace Hamsi
{
    extern const uint s_T256[4][2048];
    extern const uint s_T512[8][4096];

    ref class HamsiBase abstract
    {
        #pragma region Consts

        protected: static array<uint>^ s_alpha1 = 
        {
            0xff00f0f0, 0xccccaaaa, 0xf0f0cccc, 0xff00aaaa, 0xccccaaaa, 0xf0f0ff00, 0xaaaacccc, 0xf0f0ff00,
            0xf0f0cccc, 0xaaaaff00, 0xccccff00, 0xaaaaf0f0, 0xaaaaf0f0, 0xff00cccc, 0xccccf0f0, 0xff00aaaa,
            0xccccaaaa, 0xff00f0f0, 0xff00aaaa, 0xf0f0cccc, 0xf0f0ff00, 0xccccaaaa, 0xf0f0ff00, 0xaaaacccc,
            0xaaaaff00, 0xf0f0cccc, 0xaaaaf0f0, 0xccccff00, 0xff00cccc, 0xaaaaf0f0, 0xff00aaaa, 0xccccf0f0
        };

        protected: static array<uint>^ s_alpha2 = 
        {
            0xcaf9639c, 0x0ff0f9c0, 0x639c0ff0, 0xcaf9f9c0, 0x0ff0f9c0, 0x639ccaf9, 0xf9c00ff0, 0x639ccaf9,
            0x639c0ff0, 0xf9c0caf9, 0x0ff0caf9, 0xf9c0639c, 0xf9c0639c, 0xcaf90ff0, 0x0ff0639c, 0xcaf9f9c0,
            0x0ff0f9c0, 0xcaf9639c, 0xcaf9f9c0, 0x639c0ff0, 0x639ccaf9, 0x0ff0f9c0, 0x639ccaf9, 0xf9c00ff0,
            0xf9c0caf9, 0x639c0ff0, 0xf9c0639c, 0x0ff0caf9, 0xcaf90ff0, 0xf9c0639c, 0xcaf9f9c0, 0x0ff0639c
        };

        protected: static array<uint>^ s_IV224 = 
        { 
            0x3c967a67, 0x3cbc6c20, 0xb4c343c3, 0xa73cbc6b, 0x2c204b61, 0x74686f6c, 0x69656b65, 0x20556e69
        };

        protected: static array<uint>^ s_IV256 = 
        { 
            0x76657273, 0x69746569, 0x74204c65, 0x7576656e, 0x2c204465, 0x70617274, 0x656d656e, 0x7420456c
        };

        protected: static array<uint>^ s_IV384 = 
        { 
            0x656b7472, 0x6f746563, 0x686e6965, 0x6b2c2043, 0x6f6d7075, 0x74657220, 0x53656375, 0x72697479, 
            0x20616e64, 0x20496e64, 0x75737472, 0x69616c20, 0x43727970, 0x746f6772, 0x61706879, 0x2c204b61
        };

        protected: static array<uint>^s_IV512 = 
        { 
            0x73746565, 0x6c706172, 0x6b204172, 0x656e6265, 0x72672031, 0x302c2062, 0x75732032, 0x3434362c, 
            0x20422d33, 0x30303120, 0x4c657576, 0x656e2d48, 0x65766572, 0x6c65652c, 0x2042656c, 0x6769756d
        };

        #pragma endregion

        protected: array<uint>^ m_state;
        protected: ulong m_processedBytes;
        protected: array<uint>^ m_block;
        protected: int m_rounds;
        protected: array<uint>^ m_alpha;
        protected: Trans::HashBuffer^ m_buffer;

        public: HamsiBase(int a_hashSize)
        {
            HashSize = a_hashSize;

            BlockSize = (HashSize <= 32) ? 4 : 8;

            m_buffer = gcnew Trans::HashBuffer(BlockSize);
            m_state = gcnew array<uint>(BlockSize*2);
            m_block = gcnew array<uint>(BlockSize*5);

            Initialize();
        }

        protected: void Finish()
        {
            ulong bits = m_processedBytes*8;

            int padindex = BlockSize - m_buffer->Pos;

            array<byte>^ pad = gcnew array<byte>(padindex + 8);

            pad[0] = 0x80;

            pad[padindex++] = (byte)(bits >> 56);
            pad[padindex++] = (byte)(bits >> 48);
            pad[padindex++] = (byte)(bits >> 40);
            pad[padindex++] = (byte)(bits >> 32);
            pad[padindex++] = (byte)(bits >> 24);
            pad[padindex++] = (byte)(bits >> 16);
            pad[padindex++] = (byte)(bits >> 8);
            pad[padindex++] = (byte)bits;

            TransformBytes(pad, 0, BlockSize - m_buffer->Pos);

            if (HashSize<=32) 
            {
                TransformBlock(pad, padindex - 8);

                m_rounds *= 2;
                m_alpha = s_alpha2;

                TransformBlock(pad, padindex - 4);
            } 
            else
            {
                m_rounds *= 2;
                m_alpha = s_alpha2;

                TransformBlock(pad, padindex - 8);
            }
        }

        protected: array<byte>^ GetResult()
        {
            array<byte>^ result = gcnew array<byte>(HashSize);

            if (HashSize == 48) 
            {
                int T_384[12] = { 0, 1, 3, 4, 5, 6, 8, 9, 10, 12, 13, 15 };
                for (int i=0; i<12; i++) 
                {
                    result[i*4] = (byte)(m_state[T_384[i]] >> 24);
                    result[i*4+1] = (byte)(m_state[T_384[i]] >> 16);
                    result[i*4+2] = (byte)(m_state[T_384[i]] >> 8);
                    result[i*4+3] = (byte)(m_state[T_384[i]]);
                }
            } 
            else
            {
                for (int i=0; i<HashSize/4; i++)
                {
                    result[i*4] = (byte)(m_state[i] >> 24);
                    result[i*4+1] = (byte)(m_state[i] >> 16);
                    result[i*4+2] = (byte)(m_state[i] >> 8);
                    result[i*4+3] = (byte)(m_state[i]);
                }
            }

            return result;
        }

        public: int HashSize;
        public: int BlockSize;

        public: void Initialize()
        {
            m_buffer->Initialize();
            m_processedBytes = 0;

            m_rounds = BlockSize * 3 / 4;
            m_alpha = s_alpha1;
        
            if (HashSize==28)   
                Array::Copy(s_IV224, m_state, 8);
            else if (HashSize==32) 
                Array::Copy(s_IV256, m_state, 8);
            else if (HashSize==48) 
                Array::Copy(s_IV384, m_state, 16);
            else if (HashSize==64) 
                Array::Copy(s_IV512, m_state, 16);
        }

        public: void TransformBytes(array<byte>^ a_data, int a_index, int a_length)
        {
            if (!m_buffer->IsEmpty)
            {
                ulong x = m_processedBytes;
                m_buffer->Feed(a_data, a_index, a_length, x);
                m_processedBytes = x;

                if (m_buffer->IsFull)
                    TransformBlock(m_buffer->GetBytes(), 0);
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
                ulong x = m_processedBytes;
                m_buffer->Feed(a_data, a_index, a_length, x);
                m_processedBytes = x;
            }
        }

        public: array<byte>^ TransformFinal()
        {
            Finish();
            array<byte>^ result = GetResult();
            Initialize();
            return result;
        }

        public: array<byte>^ ComputeBytes(array<byte>^ a_data)
        {
            Initialize();
            TransformBytes(a_data, 0, a_data->Length);
            return TransformFinal();
        }

        protected: virtual void TransformBlock(array<byte>^ a_data, int a_index) = 0;
    };

    ref class Hamsi256Base : public HamsiBase
    {
        public: Hamsi256Base(int a_hashSize)
            : HamsiBase(a_hashSize)
        {
        }

        protected: virtual void TransformBlock(array<byte>^ a_data, int a_index) override
        {
            m_block[0] = s_T256[0][a_data[a_index]*8] ^ s_T256[1][a_data[a_index+1]*8] ^ 
                            s_T256[2][a_data[a_index+2]*8] ^ s_T256[3][a_data[a_index+3]*8];
            m_block[1] = s_T256[0][a_data[a_index]*8+1] ^ s_T256[1][a_data[a_index+1]*8+1] ^ 
                            s_T256[2][a_data[a_index+2]*8+1] ^ s_T256[3][a_data[a_index+3]*8+1];
            m_block[2] = m_state[0];
            m_block[3] = m_state[1];
            m_block[4] = m_state[2];
            m_block[5] = m_state[3];
            m_block[6] = s_T256[0][a_data[a_index]*8+2] ^ s_T256[1][a_data[a_index+1]*8+2] ^ 
                            s_T256[2][a_data[a_index+2]*8+2] ^ s_T256[3][a_data[a_index+3]*8+2];
            m_block[7] = s_T256[0][a_data[a_index]*8+3] ^ s_T256[1][a_data[a_index+1]*8+3] ^ 
                            s_T256[2][a_data[a_index+2]*8+3] ^ s_T256[3][a_data[a_index+3]*8+3];
            m_block[8] = s_T256[0][a_data[a_index]*8+4] ^ s_T256[1][a_data[a_index+1]*8+4] ^ 
                            s_T256[2][a_data[a_index+2]*8+4] ^ s_T256[3][a_data[a_index+3]*8+4];
            m_block[9] = s_T256[0][a_data[a_index]*8+5] ^ s_T256[1][a_data[a_index+1]*8+5] ^ 
                            s_T256[2][a_data[a_index+2]*8+5] ^ s_T256[3][a_data[a_index+3]*8+5];
            m_block[10] = m_state[4];
            m_block[11] = m_state[5];
            m_block[12] = m_state[6];
            m_block[13] = m_state[7];
            m_block[14] = s_T256[0][a_data[a_index]*8+6] ^ s_T256[1][a_data[a_index+1]*8+6] ^ 
                            s_T256[2][a_data[a_index+2]*8+6] ^ s_T256[3][a_data[a_index+3]*8+6];
            m_block[15] = s_T256[0][a_data[a_index]*8+7] ^ s_T256[1][a_data[a_index+1]*8+7] ^ 
                            s_T256[2][a_data[a_index+2]*8+7] ^ s_T256[3][a_data[a_index+3]*8+7];

            for (int i=0; i<m_rounds; i++) 
            {
                m_block[0] ^=  m_alpha[0];
                m_block[1] ^=  m_alpha[1] ^ i;
                m_block[2] ^=  m_alpha[2];
                m_block[3] ^=  m_alpha[3];
                m_block[4] ^=  m_alpha[8];
                m_block[5] ^=  m_alpha[9];
                m_block[6] ^=  m_alpha[10];
                m_block[7] ^=  m_alpha[11];
                m_block[8] ^=  m_alpha[16];
                m_block[9] ^=  m_alpha[17];
                m_block[10] ^= m_alpha[18];
                m_block[11] ^= m_alpha[19];
                m_block[12] ^= m_alpha[24];
                m_block[13] ^= m_alpha[25];
                m_block[14] ^= m_alpha[26];
                m_block[15] ^= m_alpha[27];
          
                m_block[16] = m_block[0];
                m_block[17] = m_block[1];
                m_block[18] = m_block[2];
                m_block[19] = m_block[3];

                m_block[0] &= m_block[8];
                m_block[1] &= m_block[9];
                m_block[2] &= m_block[10];
                m_block[3] &= m_block[11];
                m_block[0] ^= m_block[12];
                m_block[1] ^= m_block[13];
                m_block[2] ^= m_block[14];
                m_block[3] ^= m_block[15];
                m_block[8] ^= m_block[4];
                m_block[9] ^= m_block[5];
                m_block[10] ^= m_block[6];
                m_block[11] ^= m_block[7];
                m_block[8] ^= m_block[0];
                m_block[9] ^= m_block[1];
                m_block[10] ^= m_block[2];
                m_block[11] ^= m_block[3];
                m_block[12] |= m_block[16];
                m_block[13] |= m_block[17];
                m_block[14] |= m_block[18];
                m_block[15] |= m_block[19];
                m_block[12] ^= m_block[4];
                m_block[13] ^= m_block[5];
                m_block[14] ^= m_block[6];
                m_block[15] ^= m_block[7];
                m_block[16] ^= m_block[8];
                m_block[17] ^= m_block[9];
                m_block[18] ^= m_block[10];
                m_block[19] ^= m_block[11];
                m_block[4] = m_block[12];
                m_block[5] = m_block[13];
                m_block[6] = m_block[14];
                m_block[7] = m_block[15];
                m_block[12] |= m_block[16];
                m_block[13] |= m_block[17];
                m_block[14] |= m_block[18];
                m_block[15] |= m_block[19];
                m_block[12] ^= m_block[0];
                m_block[13] ^= m_block[1];
                m_block[14] ^= m_block[2];
                m_block[15] ^= m_block[3];
                m_block[0] &= m_block[4];
                m_block[1] &= m_block[5];
                m_block[2] &= m_block[6];
                m_block[3] &= m_block[7];
                m_block[16] ^= m_block[0];
                m_block[17] ^= m_block[1];
                m_block[18] ^= m_block[2];
                m_block[19] ^= m_block[3];
                m_block[4] ^= m_block[12];
                m_block[5] ^= m_block[13];
                m_block[6] ^= m_block[14];
                m_block[7] ^= m_block[15];
                m_block[4] ^= m_block[16];
                m_block[5] ^= m_block[17];
                m_block[6] ^= m_block[18];
                m_block[7] ^= m_block[19];
                m_block[16] = ~m_block[16];
                m_block[17] = ~m_block[17];
                m_block[18] = ~m_block[18];
                m_block[19] = ~m_block[19];
                m_block[0] = m_block[8];
                m_block[1] = m_block[9];
                m_block[2] = m_block[10];
                m_block[3] = m_block[11];
                m_block[8] = m_block[4];
                m_block[9] = m_block[5];
                m_block[10] = m_block[6];
                m_block[11] = m_block[7];
                m_block[4] = m_block[12];
                m_block[5] = m_block[13];
                m_block[6] = m_block[14];
                m_block[7] = m_block[15];
                m_block[12] = m_block[16];
                m_block[13] = m_block[17];
                m_block[14] = m_block[18];
                m_block[15] = m_block[19];
            
                m_block[0] = ((m_block[0] << 13) | (m_block[0] >> 19));
                m_block[10] = ((m_block[10] << 3) | (m_block[10] >> 29));
                m_block[5] ^= m_block[0] ^ m_block[10];
                m_block[15] ^= m_block[10] ^ (m_block[0] << 3);
                m_block[5] = ((m_block[5] << 1) | (m_block[5] >> 31));
                m_block[15] = ((m_block[15] << 7) | (m_block[15] >> 25));
                m_block[0] ^= m_block[5] ^ m_block[15];
                m_block[10] ^= m_block[15] ^ (m_block[5] << 7);
                m_block[0] = ((m_block[0] << 5) | (m_block[0] >> 27));
                m_block[10] = ((m_block[10] << 22) | (m_block[10] >> 10));
                m_block[1] = ((m_block[1] << 13) | (m_block[1] >> 19));
                m_block[11] = ((m_block[11] << 3) | (m_block[11] >> 29));
                m_block[6] ^= m_block[1] ^ m_block[11];
                m_block[12] ^= m_block[11] ^ (m_block[1] << 3);
                m_block[6] = ((m_block[6] << 1) | (m_block[6] >> 31));
                m_block[12] = ((m_block[12] << 7) | (m_block[12] >> 25));
                m_block[1] ^= m_block[6] ^ m_block[12];
                m_block[11] ^= m_block[12] ^ (m_block[6] << 7);
                m_block[1] = ((m_block[1] << 5) | (m_block[1] >> 27));
                m_block[11] = ((m_block[11] << 22) | (m_block[11] >> 10));
                m_block[2] = ((m_block[2] << 13) | (m_block[2] >> 19));
                m_block[8] = ((m_block[8] << 3) | (m_block[8] >> 29));
                m_block[7] ^= m_block[2] ^ m_block[8];
                m_block[13] ^= m_block[8] ^ (m_block[2] << 3);
                m_block[7] = ((m_block[7] << 1) | (m_block[7] >> 31));
                m_block[13] = ((m_block[13] << 7) | (m_block[13] >> 25));
                m_block[2] ^= m_block[7] ^ m_block[13];
                m_block[8] ^= m_block[13] ^ (m_block[7] << 7);
                m_block[2] = ((m_block[2] << 5) | (m_block[2] >> 27));
                m_block[8] = ((m_block[8] << 22) | (m_block[8] >> 10));
                m_block[3] = ((m_block[3] << 13) | (m_block[3] >> 19));
                m_block[9] = ((m_block[9] << 3) | (m_block[9] >> 29));
                m_block[4] ^= m_block[3] ^ m_block[9];
                m_block[14] ^= m_block[9] ^ (m_block[3] << 3);
                m_block[4] = ((m_block[4] << 1) | (m_block[4] >> 31));
                m_block[14] = ((m_block[14] << 7) | (m_block[14] >> 25));
                m_block[3] ^= m_block[4] ^ m_block[14];
                m_block[9] ^= m_block[14] ^ (m_block[4] << 7);
                m_block[3] = ((m_block[3] << 5) | (m_block[3] >> 27));
                m_block[9] = ((m_block[9] << 22) | (m_block[9] >> 10));
            }

            m_state[0] ^= m_block[0]; 
            m_state[1] ^= m_block[1]; 
            m_state[2] ^= m_block[2]; 
            m_state[3] ^= m_block[3]; 
            m_state[4] ^= m_block[8]; 
            m_state[5] ^= m_block[9]; 
            m_state[6] ^= m_block[10]; 
            m_state[7] ^= m_block[11]; 
        } 
    };

    ref class Hamsi512Base : public HamsiBase
    {
        public: Hamsi512Base(int a_hashSize)
            : HamsiBase(a_hashSize)
        {
        }

        protected: virtual void TransformBlock(array<byte>^ a_data, int a_index) override
        {
            m_block[0] = s_T512[0][a_data[a_index]*16] ^ s_T512[1][a_data[a_index+1]*16] ^ 
                            s_T512[2][a_data[a_index+2]*16] ^ s_T512[3][a_data[a_index+3]*16] ^ 
                            s_T512[4][a_data[a_index+4]*16] ^ s_T512[5][a_data[a_index+5]*16] ^ 
                            s_T512[6][a_data[a_index+6]*16] ^ s_T512[7][a_data[a_index+7]*16];
            m_block[1] = s_T512[0][a_data[a_index]*16+1] ^ s_T512[1][a_data[a_index+1]*16+1] ^ 
                            s_T512[2][a_data[a_index+2]*16+1] ^ s_T512[3][a_data[a_index+3]*16+1] ^ 
                            s_T512[4][a_data[a_index+4]*16+1] ^ s_T512[5][a_data[a_index+5]*16+1] ^ 
                            s_T512[6][a_data[a_index+6]*16+1] ^ s_T512[7][a_data[a_index+7]*16+1];
            m_block[2] = m_state[0];
            m_block[3] = m_state[1];
            m_block[4] = s_T512[0][a_data[a_index]*16+2] ^ s_T512[1][a_data[a_index+1]*16+2] ^ 
                            s_T512[2][a_data[a_index+2]*16+2] ^ s_T512[3][a_data[a_index+3]*16+2] ^ 
                            s_T512[4][a_data[a_index+4]*16+2] ^ s_T512[5][a_data[a_index+5]*16+2] ^ 
                            s_T512[6][a_data[a_index+6]*16+2] ^ s_T512[7][a_data[a_index+7]*16+2];
            m_block[5] = s_T512[0][a_data[a_index]*16+3] ^ s_T512[1][a_data[a_index+1]*16+3] ^ 
                            s_T512[2][a_data[a_index+2]*16+3] ^ s_T512[3][a_data[a_index+3]*16+3] ^ 
                            s_T512[4][a_data[a_index+4]*16+3] ^ s_T512[5][a_data[a_index+5]*16+3] ^ 
                            s_T512[6][a_data[a_index+6]*16+3] ^ s_T512[7][a_data[a_index+7]*16+3];
            m_block[6] = m_state[2];
            m_block[7] = m_state[3];
            m_block[8] = m_state[4];
            m_block[9] = m_state[5];
            m_block[10] = s_T512[0][a_data[a_index]*16+4] ^ s_T512[1][a_data[a_index+1]*16+4] ^ 
                            s_T512[2][a_data[a_index+2]*16+4] ^ s_T512[3][a_data[a_index+3]*16+4] ^ 
                            s_T512[4][a_data[a_index+4]*16+4] ^ s_T512[5][a_data[a_index+5]*16+4] ^ 
                            s_T512[6][a_data[a_index+6]*16+4] ^ s_T512[7][a_data[a_index+7]*16+4];
            m_block[11] = s_T512[0][a_data[a_index]*16+5] ^ s_T512[1][a_data[a_index+1]*16+5] ^ 
                            s_T512[2][a_data[a_index+2]*16+5] ^ s_T512[3][a_data[a_index+3]*16+5] ^ 
                            s_T512[4][a_data[a_index+4]*16+5] ^ s_T512[5][a_data[a_index+5]*16+5] ^ 
                            s_T512[6][a_data[a_index+6]*16+5] ^ s_T512[7][a_data[a_index+7]*16+5];
            m_block[12] = m_state[6];
            m_block[13] = m_state[7];
            m_block[14] = s_T512[0][a_data[a_index]*16+6] ^ s_T512[1][a_data[a_index+1]*16+6] ^ 
                            s_T512[2][a_data[a_index+2]*16+6] ^ s_T512[3][a_data[a_index+3]*16+6] ^ 
                            s_T512[4][a_data[a_index+4]*16+6] ^ s_T512[5][a_data[a_index+5]*16+6] ^ 
                            s_T512[6][a_data[a_index+6]*16+6] ^ s_T512[7][a_data[a_index+7]*16+6];
            m_block[15] = s_T512[0][a_data[a_index]*16+7] ^ s_T512[1][a_data[a_index+1]*16+7] ^ 
                            s_T512[2][a_data[a_index+2]*16+7] ^ s_T512[3][a_data[a_index+3]*16+7] ^ 
                            s_T512[4][a_data[a_index+4]*16+7] ^ s_T512[5][a_data[a_index+5]*16+7] ^ 
                            s_T512[6][a_data[a_index+6]*16+7] ^ s_T512[7][a_data[a_index+7]*16+7];
            m_block[16] = s_T512[0][a_data[a_index]*16+8] ^ s_T512[1][a_data[a_index+1]*16+8] ^ 
                            s_T512[2][a_data[a_index+2]*16+8] ^ s_T512[3][a_data[a_index+3]*16+8] ^ 
                            s_T512[4][a_data[a_index+4]*16+8] ^ s_T512[5][a_data[a_index+5]*16+8] ^ 
                            s_T512[6][a_data[a_index+6]*16+8] ^ s_T512[7][a_data[a_index+7]*16+8];
            m_block[17] = s_T512[0][a_data[a_index]*16+9] ^ s_T512[1][a_data[a_index+1]*16+9] ^ 
                            s_T512[2][a_data[a_index+2]*16+9] ^ s_T512[3][a_data[a_index+3]*16+9] ^ 
                            s_T512[4][a_data[a_index+4]*16+9] ^ s_T512[5][a_data[a_index+5]*16+9] ^ 
                            s_T512[6][a_data[a_index+6]*16+9] ^ s_T512[7][a_data[a_index+7]*16+9];
            m_block[18] = m_state[8];
            m_block[19] = m_state[9];
            m_block[20] = s_T512[0][a_data[a_index]*16+10] ^ s_T512[1][a_data[a_index+1]*16+10] ^ 
                            s_T512[2][a_data[a_index+2]*16+10] ^ s_T512[3][a_data[a_index+3]*16+10] ^ 
                            s_T512[4][a_data[a_index+4]*16+10] ^ s_T512[5][a_data[a_index+5]*16+10] ^ 
                            s_T512[6][a_data[a_index+6]*16+10] ^ s_T512[7][a_data[a_index+7]*16+10];
            m_block[21] = s_T512[0][a_data[a_index]*16+11] ^ s_T512[1][a_data[a_index+1]*16+11] ^ 
                            s_T512[2][a_data[a_index+2]*16+11] ^ s_T512[3][a_data[a_index+3]*16+11] ^ 
                            s_T512[4][a_data[a_index+4]*16+11] ^ s_T512[5][a_data[a_index+5]*16+11] ^ 
                            s_T512[6][a_data[a_index+6]*16+11] ^ s_T512[7][a_data[a_index+7]*16+11];
            m_block[22] = m_state[10];
            m_block[23] = m_state[11];
            m_block[24] = m_state[12];
            m_block[25] = m_state[13];
            m_block[26] = s_T512[0][a_data[a_index]*16+12] ^ s_T512[1][a_data[a_index+1]*16+12] ^ 
                            s_T512[2][a_data[a_index+2]*16+12] ^ s_T512[3][a_data[a_index+3]*16+12] ^ 
                            s_T512[4][a_data[a_index+4]*16+12] ^ s_T512[5][a_data[a_index+5]*16+12] ^ 
                            s_T512[6][a_data[a_index+6]*16+12] ^ s_T512[7][a_data[a_index+7]*16+12];
            m_block[27] = s_T512[0][a_data[a_index]*16+13] ^ s_T512[1][a_data[a_index+1]*16+13] ^ 
                            s_T512[2][a_data[a_index+2]*16+13] ^ s_T512[3][a_data[a_index+3]*16+13] ^ 
                            s_T512[4][a_data[a_index+4]*16+13] ^ s_T512[5][a_data[a_index+5]*16+13] ^ 
                            s_T512[6][a_data[a_index+6]*16+13] ^ s_T512[7][a_data[a_index+7]*16+13];
            m_block[28] = m_state[14];
            m_block[29] = m_state[15];
            m_block[30] = s_T512[0][a_data[a_index]*16+14] ^ s_T512[1][a_data[a_index+1]*16+14] ^ 
                            s_T512[2][a_data[a_index+2]*16+14] ^ s_T512[3][a_data[a_index+3]*16+14] ^ 
                            s_T512[4][a_data[a_index+4]*16+14] ^ s_T512[5][a_data[a_index+5]*16+14] ^ 
                            s_T512[6][a_data[a_index+6]*16+14] ^ s_T512[7][a_data[a_index+7]*16+14];
            m_block[31] = s_T512[0][a_data[a_index]*16+15] ^ s_T512[1][a_data[a_index+1]*16+15] ^ 
                            s_T512[2][a_data[a_index+2]*16+15] ^ s_T512[3][a_data[a_index+3]*16+15] ^ 
                            s_T512[4][a_data[a_index+4]*16+15] ^ s_T512[5][a_data[a_index+5]*16+15] ^ 
                            s_T512[6][a_data[a_index+6]*16+15] ^ s_T512[7][a_data[a_index+7]*16+15];

            for (int i=0; i<m_rounds; i++) 
            {
                m_block[0] ^= m_alpha[0];
                m_block[1] ^= m_alpha[1] ^ i;
                m_block[2] ^= m_alpha[2];
                m_block[3] ^= m_alpha[3];
                m_block[4] ^= m_alpha[4];
                m_block[5] ^= m_alpha[5];
                m_block[6] ^= m_alpha[6];
                m_block[7] ^= m_alpha[7];
                m_block[8] ^= m_alpha[8];
                m_block[9] ^= m_alpha[9];
                m_block[10] ^= m_alpha[10];
                m_block[11] ^= m_alpha[11];
                m_block[12] ^= m_alpha[12];
                m_block[13] ^= m_alpha[13];
                m_block[14] ^= m_alpha[14];
                m_block[15] ^= m_alpha[15];
                m_block[16] ^= m_alpha[16];
                m_block[17] ^= m_alpha[17];
                m_block[18] ^= m_alpha[18];
                m_block[19] ^= m_alpha[19];
                m_block[20] ^= m_alpha[20];
                m_block[21] ^= m_alpha[21];
                m_block[22] ^= m_alpha[22];
                m_block[23] ^= m_alpha[23];
                m_block[24] ^= m_alpha[24];
                m_block[25] ^= m_alpha[25];
                m_block[26] ^= m_alpha[26];
                m_block[27] ^= m_alpha[27];
                m_block[28] ^= m_alpha[28];
                m_block[29] ^= m_alpha[29];
                m_block[30] ^= m_alpha[30];
                m_block[31] ^= m_alpha[31];

                m_block[32] = m_block[0];
                m_block[33] = m_block[1];
                m_block[34] = m_block[2];
                m_block[35] = m_block[3];
                m_block[36] = m_block[4];
                m_block[37] = m_block[5];
                m_block[38] = m_block[6];
                m_block[39] = m_block[7];

                m_block[0] &= m_block[16];
                m_block[1] &= m_block[17];
                m_block[2] &= m_block[18];
                m_block[3] &= m_block[19];
                m_block[4] &= m_block[20];
                m_block[5] &= m_block[21];
                m_block[6] &= m_block[22];
                m_block[7] &= m_block[23];
                m_block[0] ^= m_block[24];
                m_block[1] ^= m_block[25];
                m_block[2] ^= m_block[26];
                m_block[3] ^= m_block[27];
                m_block[4] ^= m_block[28];
                m_block[5] ^= m_block[29];
                m_block[6] ^= m_block[30];
                m_block[7] ^= m_block[31];
                m_block[16] ^= m_block[8];
                m_block[17] ^= m_block[9];
                m_block[18] ^= m_block[10];
                m_block[19] ^= m_block[11];
                m_block[20] ^= m_block[12];
                m_block[21] ^= m_block[13];
                m_block[22] ^= m_block[14];
                m_block[23] ^= m_block[15];
                m_block[16] ^= m_block[0];
                m_block[17] ^= m_block[1];
                m_block[18] ^= m_block[2];
                m_block[19] ^= m_block[3];
                m_block[20] ^= m_block[4];
                m_block[21] ^= m_block[5];
                m_block[22] ^= m_block[6];
                m_block[23] ^= m_block[7];
                m_block[24] |= m_block[32];
                m_block[25] |= m_block[33];
                m_block[26] |= m_block[34];
                m_block[27] |= m_block[35];
                m_block[28] |= m_block[36];
                m_block[29] |= m_block[37];
                m_block[30] |= m_block[38];
                m_block[31] |= m_block[39];
                m_block[24] ^= m_block[8];
                m_block[25] ^= m_block[9];
                m_block[26] ^= m_block[10];
                m_block[27] ^= m_block[11];
                m_block[28] ^= m_block[12];
                m_block[29] ^= m_block[13];
                m_block[30] ^= m_block[14];
                m_block[31] ^= m_block[15];
                m_block[32] ^= m_block[16];
                m_block[33] ^= m_block[17];
                m_block[34] ^= m_block[18];
                m_block[35] ^= m_block[19];
                m_block[36] ^= m_block[20];
                m_block[37] ^= m_block[21];
                m_block[38] ^= m_block[22];
                m_block[39] ^= m_block[23];
                m_block[8] = m_block[24];
                m_block[9] = m_block[25];
                m_block[10] = m_block[26];
                m_block[11] = m_block[27];
                m_block[12] = m_block[28];
                m_block[13] = m_block[29];
                m_block[14] = m_block[30];
                m_block[15] = m_block[31];
                m_block[24] |= m_block[32];
                m_block[25] |= m_block[33];
                m_block[26] |= m_block[34];
                m_block[27] |= m_block[35];
                m_block[28] |= m_block[36];
                m_block[29] |= m_block[37];
                m_block[30] |= m_block[38];
                m_block[31] |= m_block[39];
                m_block[24] ^= m_block[0];
                m_block[25] ^= m_block[1];
                m_block[26] ^= m_block[2];
                m_block[27] ^= m_block[3];
                m_block[28] ^= m_block[4];
                m_block[29] ^= m_block[5];
                m_block[30] ^= m_block[6];
                m_block[31] ^= m_block[7];
                m_block[0] &= m_block[8];
                m_block[1] &= m_block[9];
                m_block[2] &= m_block[10];
                m_block[3] &= m_block[11];
                m_block[4] &= m_block[12];
                m_block[5] &= m_block[13];
                m_block[6] &= m_block[14];
                m_block[7] &= m_block[15];
                m_block[32] ^= m_block[0];
                m_block[33] ^= m_block[1];
                m_block[34] ^= m_block[2];
                m_block[35] ^= m_block[3];
                m_block[36] ^= m_block[4];
                m_block[37] ^= m_block[5];
                m_block[38] ^= m_block[6];
                m_block[39] ^= m_block[7];
                m_block[8] ^= m_block[24];
                m_block[9] ^= m_block[25];
                m_block[10] ^= m_block[26];
                m_block[11] ^= m_block[27];
                m_block[12] ^= m_block[28];
                m_block[13] ^= m_block[29];
                m_block[14] ^= m_block[30];
                m_block[15] ^= m_block[31];
                m_block[8] ^= m_block[32];
                m_block[9] ^= m_block[33];
                m_block[10] ^= m_block[34];
                m_block[11] ^= m_block[35];
                m_block[12] ^= m_block[36];
                m_block[13] ^= m_block[37];
                m_block[14] ^= m_block[38];
                m_block[15] ^= m_block[39];
                m_block[32] = ~m_block[32];
                m_block[33] = ~m_block[33];
                m_block[34] = ~m_block[34];
                m_block[35] = ~m_block[35];
                m_block[36] = ~m_block[36];
                m_block[37] = ~m_block[37];
                m_block[38] = ~m_block[38];
                m_block[39] = ~m_block[39];
                m_block[0] = m_block[16];
                m_block[1] = m_block[17];
                m_block[2] = m_block[18];
                m_block[3] = m_block[19];
                m_block[4] = m_block[20];
                m_block[5] = m_block[21];
                m_block[6] = m_block[22];
                m_block[7] = m_block[23];
                m_block[16] = m_block[8];
                m_block[17] = m_block[9];
                m_block[18] = m_block[10];
                m_block[19] = m_block[11];
                m_block[20] = m_block[12];
                m_block[21] = m_block[13];
                m_block[22] = m_block[14];
                m_block[23] = m_block[15];
                m_block[8] = m_block[24];
                m_block[9] = m_block[25];
                m_block[10] = m_block[26];
                m_block[11] = m_block[27];
                m_block[12] = m_block[28];
                m_block[13] = m_block[29];
                m_block[14] = m_block[30];
                m_block[15] = m_block[31];
                m_block[24] = m_block[32];
                m_block[25] = m_block[33];
                m_block[26] = m_block[34];
                m_block[27] = m_block[35];
                m_block[28] = m_block[36];
                m_block[29] = m_block[37];
                m_block[30] = m_block[38];
                m_block[31] = m_block[39];
            
                m_block[0] = ((m_block[0] << 13) | (m_block[0] >> 19));
                m_block[18] = ((m_block[18] << 3) | (m_block[18] >> 29));
                m_block[9] ^= m_block[0] ^ m_block[18];
                m_block[27] ^= m_block[18] ^ (m_block[0] << 3);
                m_block[9] = ((m_block[9] << 1) | (m_block[9] >> 31));
                m_block[27] = ((m_block[27] << 7) | (m_block[27] >> 25));
                m_block[0] ^= m_block[9] ^ m_block[27];
                m_block[18] ^= m_block[27] ^ (m_block[9] << 7);
                m_block[0] = ((m_block[0] << 5) | (m_block[0] >> 27));
                m_block[18] = ((m_block[18] << 22) | (m_block[18] >> 10));
                m_block[1] = ((m_block[1] << 13) | (m_block[1] >> 19));
                m_block[19] = ((m_block[19] << 3) | (m_block[19] >> 29));
                m_block[10] ^= m_block[1] ^ m_block[19];
                m_block[28] ^= m_block[19] ^ (m_block[1] << 3);
                m_block[10] = ((m_block[10] << 1) | (m_block[10] >> 31));
                m_block[28] = ((m_block[28] << 7) | (m_block[28] >> 25));
                m_block[1] ^= m_block[10] ^ m_block[28];
                m_block[19] ^= m_block[28] ^ (m_block[10] << 7);
                m_block[1] = ((m_block[1] << 5) | (m_block[1] >> 27));
                m_block[19] = ((m_block[19] << 22) | (m_block[19] >> 10));
                m_block[2] = ((m_block[2] << 13) | (m_block[2] >> 19));
                m_block[20] = ((m_block[20] << 3) | (m_block[20] >> 29));
                m_block[11] ^= m_block[2] ^ m_block[20];
                m_block[29] ^= m_block[20] ^ (m_block[2] << 3);
                m_block[11] = ((m_block[11] << 1) | (m_block[11] >> 31));
                m_block[29] = ((m_block[29] << 7) | (m_block[29] >> 25));
                m_block[2] ^= m_block[11] ^ m_block[29];
                m_block[20] ^= m_block[29] ^ (m_block[11] << 7);
                m_block[2] = ((m_block[2] << 5) | (m_block[2] >> 27));
                m_block[20] = ((m_block[20] << 22) | (m_block[20] >> 10));
                m_block[3] = ((m_block[3] << 13) | (m_block[3] >> 19));
                m_block[21] = ((m_block[21] << 3) | (m_block[21] >> 29));
                m_block[12] ^= m_block[3] ^ m_block[21];
                m_block[30] ^= m_block[21] ^ (m_block[3] << 3);
                m_block[12] = ((m_block[12] << 1) | (m_block[12] >> 31));
                m_block[30] = ((m_block[30] << 7) | (m_block[30] >> 25));
                m_block[3] ^= m_block[12] ^ m_block[30];
                m_block[21] ^= m_block[30] ^ (m_block[12] << 7);
                m_block[3] = ((m_block[3] << 5) | (m_block[3] >> 27));
                m_block[21] = ((m_block[21] << 22) | (m_block[21] >> 10));
                m_block[4] = ((m_block[4] << 13) | (m_block[4] >> 19));
                m_block[22] = ((m_block[22] << 3) | (m_block[22] >> 29));
                m_block[13] ^= m_block[4] ^ m_block[22];
                m_block[31] ^= m_block[22] ^ (m_block[4] << 3);
                m_block[13] = ((m_block[13] << 1) | (m_block[13] >> 31));
                m_block[31] = ((m_block[31] << 7) | (m_block[31] >> 25));
                m_block[4] ^= m_block[13] ^ m_block[31];
                m_block[22] ^= m_block[31] ^ (m_block[13] << 7);
                m_block[4] = ((m_block[4] << 5) | (m_block[4] >> 27));
                m_block[22] = ((m_block[22] << 22) | (m_block[22] >> 10));
                m_block[5] = ((m_block[5] << 13) | (m_block[5] >> 19));
                m_block[23] = ((m_block[23] << 3) | (m_block[23] >> 29));
                m_block[14] ^= m_block[5] ^ m_block[23];
                m_block[24] ^= m_block[23] ^ (m_block[5] << 3);
                m_block[14] = ((m_block[14] << 1) | (m_block[14] >> 31));
                m_block[24] = ((m_block[24] << 7) | (m_block[24] >> 25));
                m_block[5] ^= m_block[14] ^ m_block[24];
                m_block[23] ^= m_block[24] ^ (m_block[14] << 7);
                m_block[5] = ((m_block[5] << 5) | (m_block[5] >> 27));
                m_block[23] = ((m_block[23] << 22) | (m_block[23] >> 10));
                m_block[6] = ((m_block[6] << 13) | (m_block[6] >> 19));
                m_block[16] = ((m_block[16] << 3) | (m_block[16] >> 29));
                m_block[15] ^= m_block[6] ^ m_block[16];
                m_block[25] ^= m_block[16] ^ (m_block[6] << 3);
                m_block[15] = ((m_block[15] << 1) | (m_block[15] >> 31));
                m_block[25] = ((m_block[25] << 7) | (m_block[25] >> 25));
                m_block[6] ^= m_block[15] ^ m_block[25];
                m_block[16] ^= m_block[25] ^ (m_block[15] << 7);
                m_block[6] = ((m_block[6] << 5) | (m_block[6] >> 27));
                m_block[16] = ((m_block[16] << 22) | (m_block[16] >> 10));
                m_block[7] = ((m_block[7] << 13) | (m_block[7] >> 19));
                m_block[17] = ((m_block[17] << 3) | (m_block[17] >> 29));
                m_block[8] ^= m_block[7] ^ m_block[17];
                m_block[26] ^= m_block[17] ^ (m_block[7] << 3);
                m_block[8] = ((m_block[8] << 1) | (m_block[8] >> 31));
                m_block[26] = ((m_block[26] << 7) | (m_block[26] >> 25));
                m_block[7] ^= m_block[8] ^ m_block[26];
                m_block[17] ^= m_block[26] ^ (m_block[8] << 7);
                m_block[7] = ((m_block[7] << 5) | (m_block[7] >> 27));
                m_block[17] = ((m_block[17] << 22) | (m_block[17] >> 10));
                m_block[0] = ((m_block[0] << 13) | (m_block[0] >> 19));
                m_block[5] = ((m_block[5] << 3) | (m_block[5] >> 29));
                m_block[2] ^= m_block[0] ^ m_block[5];
                m_block[7] ^= m_block[5] ^ (m_block[0] << 3);
                m_block[2] = ((m_block[2] << 1) | (m_block[2] >> 31));
                m_block[7] = ((m_block[7] << 7) | (m_block[7] >> 25));
                m_block[0] ^= m_block[2] ^ m_block[7];
                m_block[5] ^= m_block[7] ^ (m_block[2] << 7);
                m_block[0] = ((m_block[0] << 5) | (m_block[0] >> 27));
                m_block[5] = ((m_block[5] << 22) | (m_block[5] >> 10));
                m_block[9] = ((m_block[9] << 13) | (m_block[9] >> 19));
                m_block[12] = ((m_block[12] << 3) | (m_block[12] >> 29));
                m_block[11] ^= m_block[9] ^ m_block[12];
                m_block[14] ^= m_block[12] ^ (m_block[9] << 3);
                m_block[11] = ((m_block[11] << 1) | (m_block[11] >> 31));
                m_block[14] = ((m_block[14] << 7) | (m_block[14] >> 25));
                m_block[9] ^= m_block[11] ^ m_block[14];
                m_block[12] ^= m_block[14] ^ (m_block[11] << 7);
                m_block[9] = ((m_block[9] << 5) | (m_block[9] >> 27));
                m_block[12] = ((m_block[12] << 22) | (m_block[12] >> 10));
                m_block[16] = ((m_block[16] << 13) | (m_block[16] >> 19));
                m_block[21] = ((m_block[21] << 3) | (m_block[21] >> 29));
                m_block[19] ^= m_block[16] ^ m_block[21];
                m_block[22] ^= m_block[21] ^ (m_block[16] << 3);
                m_block[19] = ((m_block[19] << 1) | (m_block[19] >> 31));
                m_block[22] = ((m_block[22] << 7) | (m_block[22] >> 25));
                m_block[16] ^= m_block[19] ^ m_block[22];
                m_block[21] ^= m_block[22] ^ (m_block[19] << 7);
                m_block[16] = ((m_block[16] << 5) | (m_block[16] >> 27));
                m_block[21] = ((m_block[21] << 22) | (m_block[21] >> 10));
                m_block[25] = ((m_block[25] << 13) | (m_block[25] >> 19));
                m_block[28] = ((m_block[28] << 3) | (m_block[28] >> 29));
                m_block[26] ^= m_block[25] ^ m_block[28];
                m_block[31] ^= m_block[28] ^ (m_block[25] << 3);
                m_block[26] = ((m_block[26] << 1) | (m_block[26] >> 31));
                m_block[31] = ((m_block[31] << 7) | (m_block[31] >> 25));
                m_block[25] ^= m_block[26] ^ m_block[31];
                m_block[28] ^= m_block[31] ^ (m_block[26] << 7);
                m_block[25] = ((m_block[25] << 5) | (m_block[25] >> 27));
                m_block[28] = ((m_block[28] << 22) | (m_block[28] >> 10));
            }

            m_state[0] ^= m_block[0]; 
            m_state[1] ^= m_block[1]; 
            m_state[2] ^= m_block[2]; 
            m_state[3] ^= m_block[3]; 
            m_state[4] ^= m_block[4]; 
            m_state[5] ^= m_block[5]; 
            m_state[6] ^= m_block[6]; 
            m_state[7] ^= m_block[7]; 
            m_state[8] ^= m_block[16]; 
            m_state[9] ^= m_block[17]; 
            m_state[10] ^= m_block[18]; 
            m_state[11] ^= m_block[19]; 
            m_state[12] ^= m_block[20]; 
            m_state[13] ^= m_block[21]; 
            m_state[14] ^= m_block[22]; 
            m_state[15] ^= m_block[23];         
        }
    };
}