
#pragma once

namespace CubeHash
{
    class CubeHash
    {
        private:

            static const int ROUNDS = 16;

            int m_buffer_pos;
            uint m_buffer[32];

            void TransformBlock();
            byte* GetResult();
            void Finish();

        public:

            int HashSize;
            int BlockSize;

            CubeHash(int a_hashSize)
            {
                BlockSize = 32;
                HashSize = a_hashSize;

                Initialize();
            }

            void Initialize();
            void TransformBytes(byte* a_data, int a_length);
            byte* TransformFinal();
            byte* ComputeBytes(byte* a_data, int a_length);
    };
}
