
#pragma once

namespace SIMD
{
    class SIMDBase
    {
        public: int HashSize;
        public: int BlockSize;

        protected: ulong m_processedBytes;
        private: int m_buffer_pos;
        protected: uint* m_state;
        protected: byte* m_buffer;

        protected: void FFT_8(int *y, int stripe);

        protected: virtual void TransformBlock(byte* a_data, int a_index) = 0;

        protected: byte* SIMDBase::GetResult()
        {
            byte* result = new byte[HashSize];
            memcpy(result, m_state, HashSize);
            return result;
        }

        protected: virtual void Finish();

        public: SIMDBase(int a_hashSize, int a_blockSize);

        public: ~SIMDBase()
        {
            delete m_state;
            delete m_buffer;
        }

        public: virtual void Initialize();
        public: void TransformBytes(byte* a_data, int a_index, int a_length);

        public: byte* ComputeBytes(byte* a_data, int a_length)
        {
            Initialize();
            TransformBytes(a_data, 0, a_length);
            return TransformFinal();
        }

        public: byte* TransformFinal() 
        {
            Finish();
            byte* result = GetResult();
            Initialize();
            return result;
        }
    };

    class SIMD256Base : public SIMDBase
    {
        private: void Round4(int* y, int i, int r, int s, int t, int u);
        private: void FFT_64(int* y);
        private: void TransformBlock(byte* a_data, int a_index, int a_final);

        protected: virtual void TransformBlock(byte* a_data, int a_index);
        protected: virtual void Finish();

        public: SIMD256Base(int a_hashSize)
                    : SIMDBase(a_hashSize, 64)
        {
            Initialize();
        }

        public: virtual void Initialize();
    };

    class SIMD512Base : public SIMDBase
    {
        private: void Round8(int* y, int i, int r, int s, int t, int u);
        private: void FFT_128_full(int* y);
        private: virtual void TransformBlock(byte* a_data, int a_index, int a_final);
        private: void FFT_16(int *y, int stripe);

        protected: virtual void TransformBlock(byte* a_data, int a_index);
        protected: virtual void Finish();

        public: SIMD512Base(int a_hashSize)
                    : SIMDBase(a_hashSize, 128)
        {
            Initialize();
        }

        public: virtual void Initialize();
    };
}