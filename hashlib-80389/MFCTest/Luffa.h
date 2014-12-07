
#pragma once

namespace Luffa
{
    class LuffaBase
    {
        public: int HashSize;
        public: int BlockSize;

        private: static const uint s_IV[40];
        protected: static const uint s_CNS[80];

        private: uint m_buffer_pos;
        private: byte* m_buffer;
        protected: uint* m_state;
        protected: int m_result_blocks;
        protected: int m_iv_length;

        protected: void ByteSwap(uint* a_array, int a_length);
        protected: byte* GetResult();
        protected: void Finish();

        public: LuffaBase(int a_hashSize);
        public: ~LuffaBase();

        public: void Initialize();
        public: void TransformBytes(byte* a_data, int a_index, int a_length);
        public: byte* TransformFinal();
        public: byte* ComputeBytes(byte* a_data, int a_length);

        protected: virtual void TransformBlock(byte* a_data, int a_index) = 0;
    };

    class Luffa256Base : public LuffaBase
    {
        protected: virtual void TransformBlock(byte* a_data, int a_index);

        public: Luffa256Base(int a_hashSize)
            : LuffaBase(a_hashSize)
        {
            m_result_blocks = 3;
            m_iv_length = 24;

            Initialize();
        }
    };

    class Luffa384 : public LuffaBase
    {
        protected: virtual void TransformBlock(byte* a_data, int a_index);

        public: Luffa384(int a_hashSize)
            : LuffaBase(a_hashSize)
        {
            m_result_blocks = 4;
            m_iv_length = 32;

            Initialize();
        }
    };

    class Luffa512 : public LuffaBase
    {
        protected: virtual void TransformBlock(byte* a_data, int a_index);

        public: Luffa512(int a_hashSize)
            : LuffaBase(a_hashSize)
        {
            m_result_blocks = 5;
            m_iv_length = 40;

            Initialize();
        }
    };
}