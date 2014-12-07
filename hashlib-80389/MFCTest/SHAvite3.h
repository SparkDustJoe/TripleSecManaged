
#pragma once 

namespace SHAvite3
{
    class SHAvite3Base
    {
        protected: ulong m_processedBytes;
        protected: int m_buffer_pos;
        protected: uint* m_state;
        protected: byte* m_buffer;

        protected: virtual void TransformBlock(byte* a_data, int a_index) = 0;
        protected: virtual void Finish() = 0;
        protected: byte* GetResult();

        public: int HashSize;
        public: int BlockSize;

        public: SHAvite3Base(int a_hashSize, int a_blockSize);
        public: ~SHAvite3Base();

        public: virtual void Initialize();
        public: void TransformBytes(byte* a_data, int a_index, int a_length);
        public: byte* TransformFinal();
        public: byte* ComputeBytes(byte* a_data, int a_length);
    };

    class SHAvite3_256Base : public SHAvite3Base
    {
        protected: virtual void TransformBlock(byte* a_data, int a_index);
        protected: virtual void Finish();

        public: SHAvite3_256Base(int a_hashSize)
                    : SHAvite3Base(a_hashSize, 64)
        {
            Initialize();
        }

        public: virtual void Initialize();
    };

    class SHAvite3_512Base : public SHAvite3Base
    {
        protected: virtual void TransformBlock(byte* a_data, int a_index);
        protected: virtual void Finish();

        public: SHAvite3_512Base(int a_hashSize)
                    : SHAvite3Base(a_hashSize, 128)
        {
            Initialize();
        }

        public: virtual void Initialize();
    };
}