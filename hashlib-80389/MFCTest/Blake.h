

#pragma once

namespace Blake
{
    class BlakeBase
    {
        protected:

            int m_buffer_pos;
            bool m_nullt;
            ulong m_processedBytes;
            byte m_buffer[128];

            virtual void Finish() = 0;
            virtual byte* GetResult() = 0;
            virtual void TransformBlock(byte* a_data, int a_index) = 0;

            uint U8TO32_BE(byte* p);
            void U32TO8_BE(byte* p, uint v);
            ulong U8TO64_BE(byte* p);
            void U64TO8_BE(byte* p, ulong v);

        public:

            int HashSize;
            int BlockSize;

            BlakeBase(int a_hashSize, int a_blockSize)
            {
                HashSize = a_hashSize;
                BlockSize = a_blockSize;
            }

            byte* TransformFinal();

            virtual void Initialize() = 0;

            byte* ComputeBytes(byte* a_data, int a_index, int a_length);

            void TransformBytes(byte* a_data, int a_index, int a_length);

    };
    class Blake256Base : public BlakeBase
    {
        private:

            uint m_state[8];

            virtual void TransformBlock(byte* a_data, int a_index);
            virtual byte* GetResult();
            virtual void Finish();

        public:

            Blake256Base(int a_hashSize)
                : BlakeBase(a_hashSize, 64)
            {
                Initialize();
            }

            virtual void Initialize();
    };

    class Blake512Base : public BlakeBase
    {
        private:

            ulong m_state[8];

            virtual void TransformBlock(byte* a_data, int a_index);
            virtual byte* GetResult();
            virtual void Finish();

        public:

            Blake512Base(int a_hashSize)
                : BlakeBase(a_hashSize, 128)
            {
                Initialize();
            }

            virtual void Initialize();
    };
}