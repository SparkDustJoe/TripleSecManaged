
#pragma once 

namespace Keccak
{
    class Keccak
    {
        private:

            ulong m_state[25];
            byte m_buffer[192];
            uint m_buffer_pos;

            byte* GetResult();
            void Finish();

        protected:

            void TransformBlock(byte* a_data, int a_index);

        public:

            uint HashSize;
            uint BlockSize;

            Keccak(int a_hash_size)
            {
                HashSize = a_hash_size;
                BlockSize = 200 - (a_hash_size * 2);

                Initialize();
            }

            void Initialize();
            void TransformBytes(byte* a_data, int a_index, int a_length);
            byte* TransformFinal();
            byte* ComputeBytes(byte* a_data, int a_length);
    };
}