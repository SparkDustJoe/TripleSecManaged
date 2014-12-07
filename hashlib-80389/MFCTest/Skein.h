
#pragma once 

namespace Skein
{
    class Skein
    {
        private:

            void Finish();
            byte* GetResult();

        protected:

            virtual void  TransformBlock(byte* a_data, int a_index) = 0;

        public:

            ulong m_flags;
            int  m_buffer_pos;   
            ulong  m_processed_bytes; 
            ulong  m_state[8];    
            byte  m_buffer[64]; 

            int  HashSize;  
            int BlockSize;

            Skein(int a_hash_size)
            {
                HashSize = a_hash_size;
            }

            void Initialize();
            void TransformBytes(byte* a_data, int a_index, int a_legth);
            byte* TransformFinal();
            byte* ComputeBytes(byte* a_data, int a_legth);
    };

    class Skein256 : public Skein
    {
        protected:

            void  TransformBlock(byte* a_data, int a_index) override;

        public:

            Skein256(int a_hash_size) : Skein(a_hash_size)
            {
                BlockSize = 32;
                Initialize();
            }
    };

    class Skein512 : public Skein
    {
        protected:

        void  TransformBlock(byte* a_data, int a_index) override;

        public:

        Skein512(int a_hash_size) : Skein(a_hash_size)
        {
            BlockSize = 64;
            Initialize();
        }
    };
}