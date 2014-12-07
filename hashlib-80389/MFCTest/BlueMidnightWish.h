
#pragma once 

namespace BMW
{    
    class BMWBase
    {
        protected:

            ulong m_processedBytes;
            byte m_buffer[256];
            int m_buffer_pos;
            
            virtual byte* GetResult() = 0;
            virtual void Finish() = 0;

            virtual void TransformBlock(byte* a_data, int a_index) = 0;

        public:

            int HashSize;
            int BlockSize;

            BMWBase(int a_hashSize, int a_blockSize)
            {
                HashSize = a_hashSize;
                BlockSize = a_blockSize;
            }

            byte* ComputeBytes(byte* a_data, int a_length);

            virtual void Initialize();
            void TransformBytes(byte* a_data, int a_index, int a_length);

            byte* TransformFinal() 
            {
                Finish();
                byte* result = GetResult();
                Initialize();
                return result;
            }
    };

    class BMW256Base : public BMWBase
    {
        protected:

            uint m_state[32];

            virtual void TransformBlock(byte* a_data, int a_index);
            void FinalCompression();

            virtual byte* GetResult();
            virtual void Finish();

        public:

            BMW256Base(int a_hashSize)
                : BMWBase(a_hashSize, 64)
            {
                Initialize();
            }

            virtual void Initialize();
    };

    class BMW512Base : public BMWBase
    {
        protected:

            ulong m_state[32];

            virtual void TransformBlock(byte* a_data, int a_index);
            void FinalCompression();

            virtual byte* GetResult();
            virtual void Finish();

        public:

            BMW512Base(int a_hashSize)
                : BMWBase(a_hashSize, 128)
            {
                Initialize();
            }

            virtual void Initialize();
    };
}